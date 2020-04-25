// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_MANAGER_HEADER
#define RAVLGUI_MANAGER_HEADER 1
//////////////////////////////////////////////////////////////
//! rcsid="$Id$
//! file="Ravl/GUI/GTK/Manager.hh"
//! lib=RavlGUI
//! author="Charles Galambos"
//! date="23/03/1999"
//! example=exCanvas.cc
//! docentry="Ravl.API.Graphics.GTK.Internal"

#include "Ravl/GUI/Widget.hh"
#include "Ravl/Threads/Mutex.hh"
#include "Ravl/Threads/Semaphore.hh"
#include "Ravl/Trigger.hh"
#include "Ravl/CallMethods.hh"
#include "Ravl/Types.hh"
#include "Ravl/Hash.hh"
#include "Ravl/Threads/ThreadEvent.hh"
#include "Ravl/Threads/MessageQueue.hh"
#include "Ravl/Index2d.hh"
#include "Ravl/Point2d.hh"

namespace RavlGUIN {
  using namespace RavlN;
  class ReadBackLockC;  
  class WindowC;
  class LockGtkThreadC;
  
  typedef void (*FuncGtkGlInitT)(int *argc, char **argv[]);

  //! userlevel=Normal
  //: GUI Manager class
  // As a user you should never need to create an instance of this
  // class; instead you should use the pre-defined global variable
  // <code>Manager</code> to access the relevant functions.
  
  class ManagerC  {
  public:
    ManagerC();
    //: Default constructor.
    
    ~ManagerC();
    //: Destructor.
    
    void Init(int &nargs,char *args[]);
    //: Initialise system.
    
    void Execute();
    //: Start manager on separate thread.
    // Call only ONCE.
    
    void Start();
    //: Handle over control to manager.
    // This will only return when the GUI is exited by the
    // user.
    
    bool Notify(IntT id);
    //: Notify interface of event.
    
    bool HandleNotify();
    //: Handle notify request.
    
    bool Shutdown();
    //: Finish up and exit.
    // This will not returns until the shutdown is complete.
    // It should NOT be used from within the GUI interface.
    
    bool Wait();
    //: Wait for the GUI to be shutdown by the user.
    
    bool WaitForStartup();
    //: Wait for the GUI to be started.
    
    void Quit();
    //: Start shutdown, use if called from a button.
    
    WindowC &GetRootWindow();
    //: Access window.
    
    void SetRootWindow(WindowC &);
    //: Set root window.
    
    void Queue(const TriggerC &se);
    //: Queue an event for running in the GUI thread.
    // Thread safe.
    
    void QueueOnGUI(const TriggerC &se);
    //: Queue an event for running in the GUI thread if it's not on the GUI thread already.
    // Thread safe.
    
    void Queue(RealT t,const TriggerC &se);
    //: Queue an event for running after delay 't'
    // NB. These events are NOT run on the GUI thread! <p>
    // Implemented with TimedEventQueue.
    // Thread safe.
    
    bool IsGUIThread() const;
    //: Test if we're in the GUI thread.
    
    bool IsManagerStarted() const
    { return managerStarted; }
    //: Test the GUI manger has been started.
    
    void UnrefPixmap(GdkPixmap *);
    //: Ensure that an unref takes place for pixmap on the GUIThread.
    
    void TidyUp();
    //: Tidy up before exiting..
    
    Index2dC ScreenSize() const 
    { return screensize; }
    //: Get the current screen resolution
    // Only meaningful after the manager has been started.
    
    Point2dC PhysicalScreenSize() const 
    { return physicalScreensize; }
    //: Get the current screen size in mm
    // Only meaningful after the manager has been started.

    void RegisterGtkGlInit(FuncGtkGlInitT func)
    { m_funcGtkGlInit = func; }
    
  protected:
    
    bool ThreadEnterGUI(SizeT &var);
    //: Enter GUI thread region.
    
    bool ThreadLeaveGUI(SizeT &var);
    //: Leave GUI thread region.
    
    IntT Register(WidgetBodyC &win);
    //: Register new window.
    
    void Deregister(WidgetBodyC &win);
    //: Deregister widget.
    
    void Deregister(IntT widgeID);
    //: Deregister widget.
    
    void SetupEvent(RealT &delay,TriggerC &se);
    //: Setup delayed event.
    // GUI thread only.
    
    static int timeout_callback(void *data);
    //: Timeout callback.
    
    bool &InitDone();
    //: Access init flag.
    
    MessageQueueC<TriggerC> events;
    HashC<IntT,TriggerC> delayEvents;
    int eCount;
    
    bool eventProcPending;
    
    MutexC access;
    HashC<IntT,WidgetC> wins; // Table of windows.
    IntT idc;                    // Window id counter.
    IntT ifp,ofp;  // File handle for event pipe.
    bool initCalled;
    bool managerStarted;
    volatile bool shutdownFlag;
    StringC m_strDebugWarning;
    ThreadEventC shutdownDone;
    ThreadEventC startupDone;
    SizeT guiThreadID;
    Index2dC screensize;
    Point2dC physicalScreensize;
    FuncGtkGlInitT m_funcGtkGlInit;
    friend class WidgetBodyC;
    friend class WindowBodyC;
    friend class ReadBackLockC;
    friend class LockGtkThreadC;  
  };
  
  extern ManagerC Manager;
  
}
#endif
