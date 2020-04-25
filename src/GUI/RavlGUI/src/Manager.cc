// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/Manager.cc"

#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/DebugAssertDialog.hh"
#include "Ravl/GUI/ToolTips.hh"
#include "Ravl/Threads/LaunchThread.hh"
#include "Ravl/HashIter.hh"
#include "Ravl/Threads/TimedTriggerQueue.hh"

#if 0
#define RAVL_USE_GTKTHREADS  RAVL_OS_WIN32  /* Use thread based event handling stratagy. */
#else
#define RAVL_USE_GTKTHREADS  0  /* Use thread based event handling stratagy. */
#define RAVL_USE_IDLEMETHOD  1  /* Use thread based event handling stratagy. */
#endif

//#include "Ravl/GUI/Label.hh"
#if RAVL_HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef WIN32
#include <io.h>
#include <fcntl.h>
#endif

#if RAVL_USE_GTKTHREADS
#include <glib/gthread.h>
#endif

#include <gtk/gtk.h>
#include <gdk/gdkrgb.h>
#include <stdio.h>
#include <glib.h>
#include <stdlib.h>

#define DODEBUG 0

#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif


namespace RavlGUIN {
  
#if RAVL_USE_GTKTHREADS
  // Manage gtk locking in an exception safe way.
  class LockGtkThreadC {
  public:
    LockGtkThreadC(ManagerC &man) 
      : manager(man),
	gotLock(true)
    {
      man.ThreadEnterGUI(id);
    }
    //: Constructor.
    
    ~LockGtkThreadC() {
      Unlock();
    }
    //: Destructor.
    
    void Unlock() {
      if(gotLock) {
	gotLock = false;
	manager.ThreadLeaveGUI(id);
      }
    }
    //: Unlock it.

    void Lock() {
      if(!gotLock) {
	gotLock = true;
	manager.ThreadEnterGUI(id);
      }
    }
    //: Unlock it.
    
    ManagerC &manager;
    bool gotLock;
    IntT id;
  };
  
#endif
  
  ManagerC Manager;
  
  extern "C" {   
    void manager_input_callback(gpointer data,gint,GdkInputCondition);
    gint timeout_callback( gpointer data );
  };
  
  //: Default constructor.  
  ManagerC::ManagerC()
    : events(20),
      eventProcPending(false),
      idc(3),
      initCalled(false),
      managerStarted(false),
      shutdownFlag(false),
      guiThreadID(ThisThreadID()), // Start out with the GUI thread as the one that created the manager.
      m_funcGtkGlInit(NULL)
  {
    InitDone() = true;
    m_strDebugWarning = StringC("RAVL DEBUG WARNING: Manager::Queue is being called on GUI thread. This can lead to undefined behaviour.\n");
    m_strDebugWarning += "To fix this, either:\n1. Invoke the function directly if it always called on the GUI thread\n";
    m_strDebugWarning += "2. Replace the call to Queue with a call to QueueOnGUI if the calling thread changes.\n\n";
    m_strDebugWarning += "THIS MESSAGE APPEARS IN DEBUG MODE ONLY\n";
#if !RAVL_USE_GTKTHREADS && !RAVL_USE_IDLEMETHOD
    int p[2];
    if(pipe(p) != 0) {
      perror("ManagerC::ManagerC(), Failed. \n");
      exit(0);
    }
    ifp = p[0];
    ofp = p[1];
    ONDEBUG(std::cerr << "ManagerC::ManagerC(), Using piped event queue. \n");
#else
    ONDEBUG(std::cerr << "ManagerC::ManagerC(), Using threaded event queue. \n");
#endif
  } 
  
  
  //: Tidy up before exiting..
  
  void ManagerC::TidyUp() {
    if(shutdownFlag)
      return ;
    shutdownFlag = true;
    while(!wins.IsEmpty()) {
      WidgetC awin;
      int key;
      {
	HashIterC<IntT,WidgetC> it(wins);
	awin = it.Data();
	key = it.Key();
      }
      awin.Destroy();
      wins.Del(key); // Make sure its removed...
    }
    ONDEBUG(std::cerr << "ManagerC::~ManagerC(), Removing final reference to root win.. \n");
    
    delayEvents.Empty();
#if !RAVL_USE_GTKTHREADS && !RAVL_USE_IDLEMETHOD
    close(ifp);
    close(ofp);
    ifp = -1;
    ofp = -1;
#endif
  }
  
  
  //: Desructor.  
  ManagerC::~ManagerC() {
    ONDEBUG(std::cerr << "ManagerC::~ManagerC(), Started. \n");
    TidyUp();
    if(startupDone) {
      startupDone.WaitForFree();
      if(shutdownDone)
	shutdownDone.WaitForFree();
      else {
	cerr << "ERROR: Destroying manager before shutdown occurred. \n";
      }
    } else {
      if(startupDone.ThreadsWaiting() > 0)
	cerr << "ERROR: Destroying manager before its event started, (Threads waiting) \n";
    }
    
    ONDEBUG(std::cerr << "ManagerC::~ManagerC(), Done.. \n");
  }
  
  //: Initalise system.
  
  void ManagerC::Init(int &nargs,char *args[])  {
    ONDEBUG(std::cerr << "ManagerC::Init(), Called. \n");
    RavlAssert(!initCalled); // Init should only be called once.

#if 0
    // This function is deprecated since GTK version 2.32 of glib
    if(!g_thread_supported ()) {
      //cerr << "Enable glib threads. \n";
      g_thread_init(0);
#if !(RAVL_USE_IDLEMETHOD && RAVL_OS_WIN32)
      // Disable thread support on windows as it seems
      // to cause hangs on most XP boxes
      gdk_threads_init();
#endif
    }
#endif
    
#if  RAVL_USE_GTKTHREADS
    // In theory no other threads should be running yet so the following
    // lock is not necessary.
    
    LockGtkThreadC  gtkLock(*this); 
#endif
    /* this is called in all GTK applications.  arguments are parsed from
     * the command line and are returned to the application. */
    gtk_init (&nargs, &args);

    if (m_funcGtkGlInit)
    {
      (*m_funcGtkGlInit)(&nargs, &args);
    }

    initCalled = true;
    
    //gdk_rgb_set_verbose (true);
    gdk_rgb_init ();  
    gtk_widget_set_default_colormap (gdk_rgb_get_cmap ());
    gtk_widget_set_default_visual (gdk_rgb_get_visual ());
    if(!guiGlobalToolTips.Create())
      std::cerr << "ManagerC::Init(), ERROR : Failed to creat global tool tips. \n";
    ONDEBUG(std::cerr << "ManagerC::Init(), Done. \n");
  }
  
  
#if !RAVL_USE_GTKTHREADS && !RAVL_USE_IDLEMETHOD
  static gboolean manager_input_callback(GIOChannel   *source,
					 GIOCondition  condition,
					 gpointer      data) {
    //cerr << "input_callback(), Called. \n";
    ManagerC &manager =  *((ManagerC *) data);
    if (condition & G_IO_IN)
      manager.HandleNotify();
    return true;
  }
#endif

#if RAVL_USE_IDLEMETHOD
  static gboolean manager_idle_callback(gpointer data) {
    ManagerC &manager =  *((ManagerC *) data);
    manager.HandleNotify();
    return false;
  }
#endif

  bool StartFunc() {
    Manager.Start();  
    return true;
  }
  
  //: Start manager on seperate thread.
  // Call only ONCE.
  
  void ManagerC::Execute() {
#if  RAVL_USE_GTKTHREADS && RAVL_OS_WIN32
    RavlAssertMsg(0,"ManagerC::Execute(), Not supported for windows, use ManagerC::Start() ");
#endif
    if(managerStarted) {
      std::cerr << "ManagerC::Execute(), WARNING: Manager already started. \n";
      return ;
    }
    LaunchThread(Trigger(StartFunc)); // Bit of a hack, but it'll do.
  }
  
  //: Handle over control to manager.
  
  void ManagerC::Start()  {
    ONDEBUG(std::cerr << "ManagerC::Start(), Called.\n");
    // Make sure only one manager is started...
    
    MutexLockC lock(access);
    if(managerStarted) {
      std::cerr << "ManagerC::Start(), WARNING: Manager already started. \n";
      return ;
    }
    guiThreadID = ThisThreadID();
    managerStarted = true;
    lock.Unlock();
    
    // Check setup is done...
    
    if(!initCalled) {
      static int nargs = 2;
      static char *args[3] = {(char *) "aprog",(char *) "arg",0 };
      Init(nargs, args);
    }
    
#if  RAVL_USE_GTKTHREADS
    // Get screen size from GDK
    LockGtkThreadC  gtkLock(*this);
    screensize.Set(gdk_screen_height(),gdk_screen_width());
    physicalScreensize = Point2dC(gdk_screen_height_mm(),gdk_screen_width_mm());
    gtkLock.Unlock();
    
    startupDone.Post();
    ONDEBUG(std::cerr << "ManagerC::Start(), Starting gtk_main().\n");
    gtkLock.Lock();
    gtk_main();
    gtkLock.Unlock();
    ONDEBUG(std::cerr << "ManagerC::Start(), gtk_main() Done.\n");
#else
    
    // Get screen size from GDK
    screensize.Set(gdk_screen_height(),gdk_screen_width());
    physicalScreensize = Point2dC(gdk_screen_height_mm(),gdk_screen_width_mm());
    
#if !RAVL_USE_IDLEMETHOD
    // Setup IO...
    GIOChannel *channel = g_io_channel_unix_new(ifp);
    g_io_add_watch_full(channel,G_PRIORITY_DEFAULT_IDLE+10,G_IO_IN,manager_input_callback, this,0);
    g_io_channel_unref (channel);
#endif
    
    startupDone.Post();
    
    // Pass control over to GTK.
    ONDEBUG(std::cerr << "ManagerC::Start(), gtk_main() Started. ifp=" << ifp << "\n");
    
    gtk_main ();
#endif
    
    managerStarted = false;
    
    shutdownDone.Post();  
    ONDEBUG(std::cerr << "ManagerC::Start(), Start tidy.... \n");
    TidyUp();
    ONDEBUG(std::cerr << "ManagerC::Start(), Start done.... \n");
    
  }
  
  //: Wait for the GUI to be shutdown by the user.
  
  bool ManagerC::Wait() {
    shutdownDone.Wait();
    return true;
  }

  //: Wait for the GUI to be started.
  
  bool ManagerC::WaitForStartup()  {
    startupDone.Wait();
    return true;
  }
  
  //: Finishup and exit.
  
  bool ManagerC::Shutdown() {
#if RAVL_USE_GTKTHREADS || RAVL_USE_IDLEMETHOD
    Queue(TriggerC());
#else
    Notify(0);
#endif
    return true;
  }
  
  void ManagerC::Quit() { 
#if RAVL_USE_GTKTHREADS || RAVL_USE_IDLEMETHOD
    Queue(TriggerC());
    Queue(TriggerC());
#else
    Notify(0);
    Notify(0);
#endif
  }
  
  //: Notify interface of event.
  
  bool ManagerC::Notify(IntT id) {
    //cerr << "Nofity. \n";
#if RAVL_USE_IDLEMETHOD
    g_idle_add_full(G_PRIORITY_HIGH_IDLE,&manager_idle_callback,this,NULL);
#else
#if !RAVL_USE_GTKTHREADS 
    if(write(ofp,&id,sizeof(IntT)) != sizeof(id)) {
      perror("ManagerC::Notify(),  Failed ");
      return false;
    }
#else
    RavlAssert(0);
#endif
#endif
    return true;
  }
  
  
  //: Handle notify request.
  
  bool ManagerC::HandleNotify() {
#if RAVL_USE_IDLEMETHOD
    eventProcPending = false;
    TriggerC trig;
    while(events.TryGet(trig)) {
      if(!trig.IsValid()) {
        gtk_main_quit ();
        return 1;
      }
      trig.Invoke();
    }
#else
#if !RAVL_USE_GTKTHREADS 
    ONDEBUG(std::cerr << "ManagerC::HandleNotify(), Got event. \n");
    IntT r;
    if(read(ifp,&r,sizeof(IntT)) != sizeof(IntT)) {
      perror("ManagerC::HandleNotify(),  Failed ");
      return false;
    }
    ONDEBUG(std::cerr << "ManagerC::HandleNotify() Called on " << r << " Started.\n");
    if(r == 0) { // Shutdown request ?
      gtk_main_quit ();
      return true;
    }
    if(r == 1) {
      eventProcPending = false;
      TriggerC trig;
      while(events.TryGet(trig))
	trig.Invoke();
      return true;
    }
    ONDEBUG(std::cerr << "ManagerC::HandleNotify() Called on " << r << " Done.\n");
#else
    RavlAssert(0);
#endif
#endif
    return true;
  }
  
  //: Access window.
  
  WindowC &ManagerC::GetRootWindow() {
    static WindowC rootWin;
    return rootWin;
  }
  
  
  //: Access init flag.
  
  bool &ManagerC::InitDone() {
    static bool initDone = false;
    return initDone;
  }
  
  //: Set root window.
  
  void ManagerC::SetRootWindow(WindowC &nw) {
    GetRootWindow() = nw;
  }
  
  
  //: Enter GUI thread region.
  
  bool ManagerC::ThreadEnterGUI(SizeT &oldId) {
    gdk_threads_enter();
#if RAVL_USE_GTKTHREADS
    // Mark this thread as being GUI.
    oldId = guiThreadID;
    guiThreadID = ThisThreadID();
#endif
    return true;
  }
  
  //: Leave GUI thread region.
  
  bool ManagerC::ThreadLeaveGUI(SizeT &var) {
    // Unmark the current thread.
#if RAVL_USE_GTKTHREADS
    guiThreadID = var;
#endif
    gdk_threads_leave();
    return true;
  }
  
  void ManagerC::QueueOnGUI(const TriggerC &se) 
  {
     if(Manager.IsGUIThread())
     {
	const_cast<TriggerC &>(se).Invoke();
     }
     else
     {
        Queue(se);
     }
  }
  
  //: Queue an event for running in the GUI thread. 
  void ManagerC::Queue(const TriggerC &se) 
  {
    //Warn developers if this is being called from the GUI thread (message box only appears in debug mode)
#if defined(QMAKE_PARANOID) && 0
    if(IsGUIThread() && se.IsValid()) {
      WindowC* rootWindow = &GetRootWindow();
      DebugDialogWrapperC warnUserDlg(m_strDebugWarning, "Debug Warning", rootWindow);
    }
#endif
    
#if RAVL_USE_GTKTHREADS
    RavlAssertMsg(initCalled,"MangerC::Init(...) must be called before an other method. ");
    if(IsGUIThread()) {
      ONDEBUG(std::cerr << "ManagerC::Queue(), Event Start... \n");
      if(se.IsValid())
	const_cast<TriggerC &>(se).Invoke();
      else 
	gtk_main_quit ();
      ONDEBUG(std::cerr << "ManagerC::Queue(), Event Finished.. \n");
    } else  {
      LockGtkThreadC lock(*this);
      
      ONDEBUG(std::cerr << "ManagerC::Queue(), Event Start... \n");
      if(se.IsValid())
	const_cast<TriggerC &>(se).Invoke();
      else
	gtk_main_quit ();
      
      ONDEBUG(std::cerr << "ManagerC::Queue(), Event Finished.. \n");
      lock.Unlock();
    }
#else
    if(shutdownFlag) {
      std::cerr << "ManagerC::Queue(), WARNING: Called after shutdown started. \n";
      return ;
    }
    if(!events.TryPut(se)) 
    {
      ONDEBUG(std::cerr << "ManagerC::Queue(), WARNING: Event queue full. \n");
      if(!IsGUIThread()) // Are we running in the GUI thread?
      {
	events.Put(se); // Nope, just wait...
      }
      else 
      {
	do {
	  events.Get().Invoke();
	} while(!events.TryPut(se)) ;
      }
    }
    if(!eventProcPending) {
      eventProcPending = true;
      Notify(1);
    }
#endif
  }
  
  static bool doUnrefPixmap(GdkPixmap *&pixmap) {
    gdk_pixmap_unref(pixmap);  
    return true;
  }
  
  //: Ensure that an unref takes place for pixmap on the GUIThread.
  
  void ManagerC::UnrefPixmap(GdkPixmap *pixmap) {
    if(IsGUIThread()) {  // Do this ASAP, to free space in the X server.
      gdk_pixmap_unref(pixmap);
      return ;
    }
    Queue(Trigger(&doUnrefPixmap,pixmap));
  }
  
  
  //: Queue an event for running in the GUI thread after delay 't'.
  
  void ManagerC::Queue(RealT t,const TriggerC &se) {
    if(shutdownFlag) {
      std::cerr << "ManagerC::Queue(), Called after shutdown started. \n";
      return ;
    }
    static TimedTriggerQueueC teQueue(true);
    teQueue.Schedule(t,se);
  }
  
  //: Test if we're in the GUI thread.
  
  bool ManagerC::IsGUIThread() const { 
    SizeT cid = ThisThreadID();
    // Only one of either the native gtk thread or event handling
    // thread will be in GUI code at a time. So if we're on either 
    // we've got an exclusive lock on gtk functions.
    return !managerStarted || guiThreadID == cid; 
  }
  
  //: Register new window.
  
  IntT ManagerC::Register(WidgetBodyC &win) {
    if(shutdownFlag) {
      std::cerr << "ManagerC::Register(), Called after shutdown started. \n";
      return 0;
    }
    MutexLockC lock(access);
    int nid = idc++;
    wins[nid] = WidgetC(win);
    RavlAssert(wins[nid].IsValid());
    return nid;
  }
  
  //: Deregister widget.
  
  void ManagerC::Deregister(IntT widgeID) {
    if(widgeID == 0)
      return ;
    ONDEBUG(std::cerr << "ManagerC::Deregister() WidgetID " << widgeID << " Started. \n");
    MutexLockC lock(access);
    wins.Del(widgeID);
    ONDEBUG(std::cerr << "ManagerC::Deregister() WidgetID " << widgeID << " Done. \n");
  }
  
  //: Deregister new window.
  
  void ManagerC::Deregister(WidgetBodyC &win) {
    if(win.WidgetID() == 0)
      return ;
    int tmpID = win.WidgetID();
    ONDEBUG(std::cerr << "ManagerC::Deregister() WidgetID " << tmpID << " Started. (" << &win << ") \n");
    win.widgetId = 0; // Stop recursions!
    MutexLockC lock(access);
    wins.Del(tmpID);
    ONDEBUG(std::cerr << "ManagerC::Deregister() WidgetID " << tmpID << " Done. (" << &win << ") \n");
  }

}
