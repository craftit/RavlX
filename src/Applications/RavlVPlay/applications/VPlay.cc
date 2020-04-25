// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Applications/VPlay/VPlay.cc"
//! docentry="Ravl.Applications.Video"
//! author="Charles Galambos"
//! lib=RavlVPlay
//! userlevel=Normal

//: Play a Video sequence.
//
// This program uses the RAVL sequence handling to read a 
// video sequence and display it using the GTK based GUI interface.

#include "Ravl/Image/DeinterlaceStream.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteYUVValue.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Image/RealRGBValue.hh"
#include "Ravl/Tuple2.hh"

#include "Ravl/Image/ImageConv.hh"

#include "Ravl/DP/Compose.hh"
#include "Ravl/DP/MTIOConnect.hh"
#include "Ravl/DP/Governor.hh"
#include "Ravl/DP/EventSet.hh"

#include "Ravl/EntryPnt.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/Option.hh"

#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/Frame.hh"
#include "Ravl/GUI/Canvas.hh"
#include "Ravl/GUI/Button.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/LBox.hh"
#include "Ravl/GUI/Menu.hh"
#include "Ravl/GUI/PackInfo.hh"
#include "Ravl/GUI/Label.hh"
#include "Ravl/GUI/FileSelector.hh"
#include "Ravl/GUI/MouseEvent.hh"
#include "Ravl/GUI/Widget.hh"
#include "Ravl/GUI/ScrolledArea.hh"

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeys.h>
#include <gdk/gdkevents.h>
#include <gdk/gdkkeysyms.h>
#include "Ravl/GUI/Table.hh"
#include "Ravl/Text/TextCursor.hh"
#include "Ravl/GUI/AttributeEditorWindow.hh"
#include "Ravl/GUI/GUIPlayControl.hh"
#include "Ravl/Image/ImgIOCif.hh"
#include "Ravl/DP/SequenceIO.hh"
#include "Ravl/DP/MethodIO.hh"
#include "Ravl/DP/Func2Stream.hh"
#include "Ravl/IO.hh"
#include "Ravl/TimeCode.hh"
#include "Ravl/OS/NetPortFormat.hh"
#include "Ravl/DP/PrintIOInfo.hh"
#include "Ravl/CDLIter.hh"
#include <stdlib.h>

#define DODEBUG 0

#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

using namespace RavlN;
using namespace RavlGUIN;
using namespace RavlImageN;

NetPortFormatC<ImageC<ByteRGBValueC> > formatNetRGBImage;
NetPortFormatC<ImageC<ByteYUVValueC> > formatNetYUVImage;
WindowC win;
TableC table;
bool set = false;
volatile bool terminateVPlay = false;

PlayControlC guiPlayControl;
//IntT textBoxSelected;

static bool gui_quit(DPIPlayControlC<ImageC<ByteRGBValueC> > &pc) 
{
  std::cerr << "Quitting. \n";
  //cout << "Quitting." << std::endl;
  terminateVPlay = true;
  pc.Continue();
  //cerr << "Shutting down play control. \n";
  guiPlayControl.Shutdown();
  //cerr << "Quitting manager. \n";
  Manager.Quit(); // Initate shutdown.
  return true;
}

static bool textskip(MouseEventC &me) {
   guiPlayControl.setTextBoxSelected(1);
   guiPlayControl.textskip().GUIGrabFocus();
   return true;
}

static bool textstart() {
   guiPlayControl.setTextBoxSelected(2);
   guiPlayControl.textstart().GUIGrabFocus();
   return true;
}

static bool textend() {
   guiPlayControl.setTextBoxSelected(3);
   guiPlayControl.textend().GUIGrabFocus();
   return true;
}

//Handle the intercepted key presses
static bool HandleKeyPress(GdkEventKey * keyEvent) {
  //cerr << keyEvent->hardware_keycode << " Event state is " << keyEvent->state << " \n";
  switch(keyEvent->keyval) {
  case(GDK_s) :
     guiPlayControl.Stop();
     break;
  //case(GDK_0) :
     //guiPlayControl.Stop();
     //break;
  case(GDK_KP_0) :
     guiPlayControl.Stop();
     break;
  case(GDK_p) :
     guiPlayControl.Play();
     break;
  case(GDK_b) :
     guiPlayControl.Back();
     break;
  case(GDK_Up) :
     switch(keyEvent->state) {
     case(GDK_SHIFT_MASK) :
        guiPlayControl.Playx2();
        break;
     default:
        guiPlayControl.Play();
        break;
     }
     break;
  case(GDK_Down) :
     switch(keyEvent->state) {
     case(GDK_SHIFT_MASK) :
        guiPlayControl.Backx2();
        break;
     default:
        guiPlayControl.Back();
        break;
     }
     break;
  case(GDK_q) :
       std::cerr << terminateVPlay << " \n"; 
       win.Signal("delete_event").Invoke();
       break;
  case(GDK_Left) :
        guiPlayControl.JBkw();
        break;
  case(GDK_Right) :
        guiPlayControl.JFwd();
        break;
  case(GDK_Home) :
        guiPlayControl.Rewind();
        break;
  case(GDK_End) :
        guiPlayControl.TheEnd();
        break;
  default:
     StringC tmp(keyEvent->string);
     switch(guiPlayControl.getTextBoxSelected()) {
         case(1) :
             if(keyEvent->keyval == GDK_BackSpace) {
                 IntT len = guiPlayControl.textskip().Text().length()-1;
                 StringC current = guiPlayControl.textskip().Text();
                 current.del(len,1);
                 guiPlayControl.setSkip(current);
             }
             else {
                StringC sc = guiPlayControl.textskip().Text() + tmp;
                guiPlayControl.setSkip(sc);
             }
             break;
         case(2) :
             if(keyEvent->keyval == GDK_BackSpace) {
                 IntT len = guiPlayControl.textstart().Text().length()-1;
                 StringC current = guiPlayControl.textstart().Text();
                 current.del(len,1);
                 guiPlayControl.setsubStart(current);
             }
             else {
                StringC sc = guiPlayControl.textstart().Text() + tmp;
                guiPlayControl.setsubStart(sc);
             }
             break;
         case(3) :
             if(keyEvent->keyval == GDK_BackSpace) {
                 IntT len =guiPlayControl.textend().Text().length()-1;
                 StringC current = guiPlayControl.textend().Text();
                 current.del(len,1);
                 guiPlayControl.setsubEnd(current);
             }
             else {
                StringC sc = guiPlayControl.textend().Text() + tmp;
                guiPlayControl.setsubEnd(sc);
             }
             break;
         default :
            std::cerr << "unrecognised key pressed. \n";
            break;
      } break;
  }
  return false;
}


////////// TIME CODES //////////////////////////////////////////

RealT frameRate = 25.0;

bool DisplayTimeCode(IntT &val,TextEntryC &te) {
  TimeCodeC tc(val,frameRate);
  te.Text(tc.ToText());
  return true;
}

////////// CODE FOR FRAME GRAB //////////////////////////////////////////

ImageC<ByteRGBValueC> frameCache;
MutexC frameCacheLock;
ImageC<ByteRGBValueC> frameGrabbed;
MutexC frameGrabLock;

bool GrabFrame(StringC &filename) {
  MutexLockC lock(frameGrabLock);
  std::cerr << "Saving file '" << filename << "'\n";
  if(!Save(filename,frameGrabbed)) {
    std::cerr << "ERROR: Failed to save file '" << filename << "'\n";
  }
  return true;
}

bool ExamineFrame(StringC &filename) {
  MutexLockC lock1(frameGrabLock);
  MutexLockC lock2(frameCacheLock);
  frameGrabbed = frameCache;
  lock2.Unlock();
  if(!Save(filename,frameGrabbed)) {
    std::cerr << "ERROR: Failed to save file '" << filename << "'\n";
  }
  lock1.Unlock();
  return true;
}

bool GetFileForGrab() {
  MutexLockC lock1(frameGrabLock);
  MutexLockC lock2(frameCacheLock);
  frameGrabbed = frameCache;
  lock2.Unlock();
  lock1.Unlock();
  static FileSelectorC fs = FileSelector(StringC("Grab frame."),
					 &GrabFrame);
  fs.Show();
  return true;
}

ImageC<ByteRGBValueC> CacheFrame(const ImageC<ByteRGBValueC> &frame) {
  if(terminateVPlay)
    throw ExceptionEndOfStreamC("Shut down video pump.");
  MutexLockC lock(frameCacheLock);
  frameCache = frame;
  return frame;
}

////////////////////////////////////////////////////

static bool file_selector(StringC &filename,FileSelectorC &fs,Tuple2C<DPIPlayControlC<ImageC<ByteRGBValueC> > ,CanvasC>  &pc) 
{
  if(filename == "") {
    fs.Hide();
    return true;
  }
  DPISPortC<ImageC<ByteRGBValueC> > vidIn;
  if(!OpenISequence(vidIn,filename)) {
    std::cerr << "ERROR: Failed to open '" << filename << "'\n";
    return true;
  }
  ImageC<ByteRGBValueC> tmp;
  if(!vidIn.Get(tmp)) {
    std::cerr << "ERROR: Failed to find image size '" << filename << "'\n";
    return true;
  }
  vidIn.DSeek(-1); // Back up.
  ImageRectangleC rect = tmp.Rectangle();
  pc.Data2().Resize(rect.Cols(),rect.Rows());
  pc.Data1().Open(vidIn);
  return true;
}


int doVPlay(int nargs,char *args[]) 
{
  bool noDisplay = false;
  // Make sure you can list options even when you don't have a display.
#if !RAVL_OS_WIN32
  if(getenv("DISPLAY") != 0) { 
    Manager.Init(nargs,args);
  } else {
    noDisplay = true;
  }
#else
  Manager.Init(nargs,args);
#endif
    
  OptionC option(nargs,args,true);
  IntT noOfFrames(option.Int("nf",-1, "Max no. of frames read"));
  IntT start(option.Int("sf", -1, " Start at nth frame"));
  //IntT digits = option.Int("d",3,"Digits in filename. ");
  //IntT scale = option.Int("s",1,"Scale image. ");
  bool deinterlace = option.Boolean("di",false,"Deinterlace incoming images. ");
  RealT delay = option.Real("t",deinterlace ? 0.02 : 0.04,"Delay between frames. ");
  bool directDraw = option.Boolean("dd",false,"Direct draw. (For realtime playback of large images) ");
  bool verb = option.Boolean("v",false,"Verbose mode. ");
  bool simpleOnly = option.Boolean("sc",false,"Display Simple Controls only. ");
  //bool deInterlace = option.Boolean("di",false,"De-interlace. (Subsample by 2) ");
  DListC<StringC> attribs = option.List("a","List of attributes to set. ");
  bool listAttribs = option.Boolean("la", "List all available attributes of this video");
  StringC formatIn = option.String("if","","Input format. ");
  bool noSeek = option.Boolean("ns",false,"Suppress seeking. ");
  bool forceScrollBox = option.Boolean("fs",false,"Force scroll box ");
  bool listFormats    = option.Boolean("lf",    false,                    "Print list of available data formats. ");
  bool listConversions= option.Boolean("lc",    false,                    "Print list of available data converters. ");
  StringC infile = option.String("","","Input filename");  
  if(infile.IsEmpty())
    infile = option.String("","in.pgm","Input filename");
  option.Comment("\nKeyboard short cuts:\n p\tplay\n s\tstop\n 0\tstop\n b\tback\n q\tquit\n ->\tjump forward\n <-\tjump back\n uparrow  \tplay\n downarrow\tback\n shift + uparrow  \tplayx2\n shift + downarrow\tbackx2\n Home\tstart\n End\tend");  
  option.Check();
  
  // Does the user just want some information about support formats ?
  
  if(listFormats) {
    PrintIOFormats(std::cout);
    return 0;
  }
  
  if(listConversions) {
    PrintIOConversions(std::cout);
    return 0;
  }
  
  if(noDisplay) {
    std::cerr << "ERROR: No DISPLAY environment set. \n";
    return 1;
  }
  
  ONDEBUG(std::cerr << "VPlay: Setting up input stream ... \n");
  // Setup video source.
  // Put in vpCtrl early, else we may not get a seekable port.
  
  DPISPortC<ImageC<ByteRGBValueC> > vidIn;
  
  if(!OpenISequence(vidIn,infile,formatIn,verb)) {
    std::cerr << "VPlay: Failed to open '" << infile << "'\n";
    exit(1);
  }
  for(DLIterC<StringC> it(attribs);it;it++) {
    StringC attrName = it->before('=').TopAndTail();
    StringC attrValue = it->after('=').TopAndTail();
    if(!vidIn.SetAttr(attrName,attrValue))
      std::cerr << "WARNING: Failed to set attribute '" << attrName << "' to '" << attrValue <<  "'\n";
  }
  if (listAttribs) {
    vidIn.GetAttrList(attribs);
    std::cout << "Available attributes are:\n";
    StringC attr;
    for(ConstDLIterC<StringC> it(attribs);it;it++) {
      vidIn.GetAttr(*it, attr);
      std::cout << "  " << *it << " " << attr << std::endl;
    }
    exit (0);
  }
  
  if(deinterlace)
    vidIn = DeinterlaceStreamC<ByteRGBValueC>(vidIn);

  ONDEBUG(std::cerr << "VPlay: Sequence opened.\n");
  if(start < 0) {
    start = vidIn.Tell();
    if(start < 0)
      start = 0;
  }
  UIntT endFrame = (noOfFrames<0) ? -1 : start + noOfFrames - 1;
  
  
  DPIPlayControlC<ImageC<ByteRGBValueC> > vpCtrl(vidIn,false,start,endFrame);  

  ONDEBUG(std::cerr << "VPlay: Play control built.\n");
  
  DPIPortC<ImageC<ByteRGBValueC> > src =  vpCtrl;// >> YUVImageCT2RGBImageCT;
  
  ONDEBUG(std::cerr << "VPlay: Conversion stream built.\n");
  // Try and establish size of images.
  
  int sx = 300,sy = 300;
  
  ImageRectangleC rect;
  {
    ImageC<ByteRGBValueC> tmp(vidIn.Get());
    ONDEBUG(std::cerr << "VPlay: Got initial frame. \n");
    if(!noSeek) vidIn.DSeek(-1);
    rect = tmp.Rectangle();
  }
  
  sy = rect.Rows();
  sx = rect.Cols();
  ONDEBUG(std::cerr << "VPlay: Image size found: " << sx << " x " << sy << "\n");
  if(sx < 1 || sy < 1) {
    std::cerr << "ERROR: Image size too small opening : '" << infile << "'.\n";
    sy = 100;
    sx = 100;
  }

  //vidIn.Invalidate(); // Finished with vidIn handle.
  
  // Setup GUI stuff.
  
  ONDEBUG(std::cerr << "Setting up GUI ... \n");
  
  //WindowC win(sx,sy,infile);
  win = WindowC(sx,sy,infile);
  Connect(win.Signal("delete_event"),gui_quit,vpCtrl);

  //Connect Key event handler to the frame widget.
  Connect(win.Signal("key_press_event"),&HandleKeyPress);
  

  CanvasC vidout(sx,sy,directDraw);  
  
  WidgetC scrolledVideo;

  if(sx > 1000 || sy > 700 || forceScrollBox) {
    scrolledVideo = ScrolledAreaC(vidout,RavlN::Min(sx,1000),RavlN::Min(sy,700));
  } else {
    scrolledVideo = vidout;
  }

  StringC strinfile(infile);
  Tuple2C<DPIPlayControlC<ImageC<ByteRGBValueC> > ,CanvasC> guiData(vpCtrl,vidout);
  
  //void file_selector(StringC &filename,FileSelectorC &fs,Tuple2C<DPIPlayControlC<ImageC<ByteRGBValueC> > ,CanvasC>  &pc)   
  // Setup file selector.
  FileSelectorC inFileSelect = RavlGUIN::FileSelector(StringC("Open Video File"),
						      strinfile,
						      &file_selector,
						      guiData
						      );

  AttributeEditorWindowC attributeEditor(infile,vpCtrl);
  
  // Setup menu.
  MenuBarC menuBar(MenuC("File",
			 MenuItemShow("Open",inFileSelect) +
			 MenuItemShow("Stream attributes",attributeEditor) +
			 MenuItem("Quit",gui_quit,vpCtrl) 
			 )
		   );
  
  TextEntryC guiTimeCode("",12,false);
  TextEntryC guiFrameRate("",5,false);

  if(!vidIn.GetAttr("framerate",frameRate))
    frameRate = (RealT) 1.0/delay;
  
  guiFrameRate.GUISetText(StringC(frameRate));
  
  guiPlayControl = PlayControlC(vpCtrl,simpleOnly);
  
  Connect(guiPlayControl.SigUpdateFrameNo(),&DisplayTimeCode,1,guiTimeCode);
  Connect(guiPlayControl.textskip().Signal("button_press_event"),&textskip);
  Connect(guiPlayControl.textstart().Signal("button_press_event"),&textstart);
  Connect(guiPlayControl.textend().Signal("button_press_event"),&textend);

  TableC table(4,5);

  ButtonC grab = Button("Grab Frame",&GetFileForGrab);
  
  ButtonC examine = Button("Examine",&ExamineFrame,StringC("@X"));
  
  table.GUIAddObject(menuBar,0,4,0,1,(GtkAttachOptions) (GTK_FILL | GTK_EXPAND),(GtkAttachOptions) GTK_FILL);
  LabelC lt(" Time:");
  table.GUIAddObject(lt,0,1,1,2,(GtkAttachOptions) (GTK_FILL | GTK_EXPAND),GTK_FILL);
  table.GUIAddObject(guiTimeCode,1,2,1,2,(GtkAttachOptions) (GTK_FILL | GTK_EXPAND),GTK_FILL);
  LabelC lr(" Rate:");
  table.GUIAddObject(lr,2,3,1,2,(GtkAttachOptions) (GTK_FILL | GTK_EXPAND),GTK_FILL);
  table.GUIAddObject(guiFrameRate,3,4,1,2,(GtkAttachOptions) (GTK_FILL | GTK_EXPAND),GTK_FILL);

  table.GUIAddObject(scrolledVideo,0,4,2,3,(GtkAttachOptions) (GTK_FILL| GTK_EXPAND),(GtkAttachOptions)(GTK_FILL| GTK_EXPAND),5,5);
  table.GUIAddObject(guiPlayControl,0,4,3,4,(GtkAttachOptions) (GTK_FILL | GTK_SHRINK),(GtkAttachOptions)GTK_FILL);
  table.GUIAddObject(grab,2,4,4,5,(GtkAttachOptions) (GTK_FILL| GTK_EXPAND),(GtkAttachOptions)(GTK_FILL));
  table.GUIAddObject(examine,0,2,4,5,(GtkAttachOptions) (GTK_FILL| GTK_EXPAND),(GtkAttachOptions)(GTK_FILL));
  win.Add(table);
  win.GUIShow();
  
  // Get GUI going ...
  
  ONDEBUG(std::cerr << "Executing manager... \n");
#if !RAVL_OS_WIN32
  // On windows the gui thread must be the main thread.
  Manager.Execute();
  Sleep(0.1); // Give it a chance to setup the display before starting the video.
#endif

  ONDEBUG(std::cerr << "Starting video... \n");
  DPEventSetC es;  
  
  // Setup play stream.
  
  es += src >> DPGovernorC<ImageC<ByteRGBValueC> >(delay) >> CacheFrame >>=  OMethod(vidout,&CanvasC::DrawRGBImage);
  
#if !RAVL_OS_WIN32
  es.Wait();
  Manager.Shutdown();
#else
  // On windows the gui thread must be the main thread.
  Manager.Start();
#endif

  ONDEBUG(std::cerr << "Terminating... \n");
  exit(0);
  return 0;
}


RAVL_ENTRY_POINT(doVPlay);
