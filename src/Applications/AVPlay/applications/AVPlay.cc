// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Applications/AVPlay/AVPlay.cc"
//! docentry="Ravl.Applications.Video;Ravl.Applications.Audio;"
//! author="Charles Galambos"
//! lib=RavlVPlay
//! userlevel=Normal

//: Play a Video sequence containing audio track.
//
// This program uses the RAVL sequence handling to read a 
// video sequence plus an sudio track, and plays it using the GTK based GUI interface.

#include "Ravl/AVFrame.hh"
#include "Ravl/AVMixer.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteYUVValue.hh"
#include "Ravl/Image/ByteRGBValue.hh"
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
#include "Ravl/GUI/Canvas.hh"
#include "Ravl/GUI/Button.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/LBox.hh"
#include "Ravl/GUI/Menu.hh"
#include "Ravl/GUI/PackInfo.hh"
#include "Ravl/GUI/Label.hh"
#include "Ravl/GUI/FileSelector.hh"

#include "Ravl/GUI/Table.hh"

#include "Ravl/GUI/GUIPlayControl.hh"

#include "Ravl/Image/ImgIOCif.hh"

#include "Ravl/DP/SequenceIO.hh"
#include "Ravl/DP/MethodIO.hh"
#include "Ravl/DP/Func2Stream.hh"
#include "Ravl/IO.hh"

#include "Ravl/TimeCode.hh"

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

volatile bool terminateVPlay = false;

PlayControlC guiPlayControl;

static bool gui_quit(DPIPlayControlC<AVFrameC> &pc) 
{
  //cerr << "Quitting. \n";
  terminateVPlay = true;
  pc.Continue();
  //cerr << "Shutting down play control. \n";
  guiPlayControl.Shutdown();
  //cerr << "Quitting manager. \n";
  Manager.Quit(); // Initate shutdown.
  return true;
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
  cerr << "Saving file '" << filename << "'\n";
  if(!Save(filename,frameGrabbed)) {
    cerr << "ERROR: Failed to save file '" << filename << "'\n";
  }
  return true;
}

bool ExamineFrame(StringC &filename) {
  MutexLockC lock1(frameGrabLock);
  MutexLockC lock2(frameCacheLock);
  frameGrabbed = frameCache;
  lock2.Unlock();
  if(!Save(filename,frameGrabbed)) {
    cerr << "ERROR: Failed to save file '" << filename << "'\n";
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

AVFrameC CacheFrame(const AVFrameC &frame) {
  if(terminateVPlay)
    throw ExceptionEndOfStreamC("Shut down video pump.");
  MutexLockC lock(frameCacheLock);
  frameCache = frame.Image();
  return frame;
}


////////////////////////////////////////////////////

static bool file_selector(StringC &filename,FileSelectorC &fs,Tuple2C<DPIPlayControlC<AVFrameC > ,CanvasC>  &pc) 
{
  if(filename == "") {
    fs.Hide();
    return true;
  }
  DPISPortC<ImageC<ByteRGBValueC> > vidIn;
  if(!OpenISequence(vidIn,filename)) {
    cerr << "ERROR: Failed to open '" << filename << "'\n";
    return true;
  }
  ImageC<ByteRGBValueC> tmp;
  if(!vidIn.Get(tmp)) {
    cerr << "ERROR: Failed to find image size '" << filename << "'\n";
    return true;
  }
  vidIn.DSeek(-1); // Back up.
  ImageRectangleC rect = tmp.Rectangle();
  pc.Data2().Resize(rect.Cols(),rect.Rows());
  pc.Data1().Open(vidIn);
  return true;
}

///////////////////////////////////////////////////////

class AVRenderC {
public:
  AVRenderC(const DPOPortC<ImageC<ByteRGBValueC> > &oi,const DPOPortC<SampleElemC<2,Int16T> > &oa)
    : audioOut(oa),
      imageOut(oi)
  {}
  //: Constructor.
  
  bool RenderFrame(const AVFrameC &frame) {
    if(imageOut.IsValid())
      imageOut.Put(frame.Image());
    if(audioOut.IsValid()) {
      SArray1dC<SampleElemC<2,Int16T> > data = frame.Audio().Stereo();
      if(data.Size() > 0) {
	IntT n = audioOut.PutArray(data);    
	while(n < (IntT) data.Size()) {
	  IntT nw =  audioOut.PutArray(data.From(n));
	  if(nw < 0)
	    break;
	  n += nw;
	}
      }
    }
    return true;
  }
  //: Render an AV frame.
  
protected:
  DPOPortC<SampleElemC<2,Int16T> > audioOut;
  DPOPortC<ImageC<ByteRGBValueC> > imageOut;
};



int doAVPlay(int nargs,char *args[]) 
{
  bool noDisplay = false;
  // Make sure you can list options even when you don't have a display.
  if(getenv("DISPLAY") != 0) { 
    Manager.Init(nargs,args);
  } else {
    noDisplay = true;
  }
    
  OptionC option(nargs,args,true);
  IntT N(option.Int("nf",-1, "Max no. of frames read"));
  IntT start(option.Int("sf", -1, " Start at nth frame"));
  //IntT digits = option.Int("d",3,"Digits in filename. ");
  //IntT scale = option.Int("s",1,"Scale image. ");
  RealT delay = option.Real("t",0.04,"Delay between frames. ");
  bool directDraw = option.Boolean("dd",false,"Direct draw. (For realtime playback of large images) ");
  //bool deinterlace = option.Boolean("di",false,"Deinterlace incoming images. ");
  bool verb = option.Boolean("v",false,"Verbose mode. ");
  bool simpleOnly = option.Boolean("sc",false,"Display Simple Controls only. ");
  //bool deInterlace = option.Boolean("di",false,"De-interlace. (Subsample by 2) ");
  DListC<StringC> attribs = option.List("at","List of attributes to set. ");
  StringC audioOutFile = option.String("ao","@DEVAUDIO:/dev/audio","Audio output stream.");
  StringC audioOutFormat = option.String("aof","","Audio output format. ");
  StringC formatIn = option.String("if","","Input format. ");
  StringC audioinfile = option.String("a","","Audio input filename");  
  StringC audioFormatIn = option.String("aif","","Audio input format. ");
  StringC infile = option.String("","","Input filename");  
  option.Check();
  
  if(noDisplay) {
    cerr << "ERROR: No DISPLAY enviroment set. \n";
    return 1;
  }
  
  ONDEBUG(cerr << "AVPlay: Setting up input stream ... \n");
  // Setup video source.
  // Put in vpCtrl early, else we may not get a seekable port.
  
  DPISPortC<AVFrameC > vidIn;
  
  if(audioinfile.IsEmpty()) { // Open a plain AV file ?
    if(!OpenISequence(vidIn,infile,formatIn,verb)) {
      cerr << "AVPlay: Failed to open '" << infile << "' (Audio/Video)\n";
      exit(1);
    }
  } else {// Open seperate audio and video files.
    DPISPortC<SampleElemC<2,Int16T> > audioIn;
    if(!OpenISequence(audioIn,audioinfile,audioFormatIn,verb)) {
      cerr << "AVPlay: Failed to open '" << audioinfile << "' (Audio)\n";
      exit(1);
    }
    DPISPortC<ImageC<ByteRGBValueC> > imgIn;
    if(!OpenISequence(imgIn,infile,formatIn,verb)) {
      cerr << "AVPlay: Failed to open '" << infile << "' (Video)\n";
      exit(1);
    }
    vidIn = AVMixerC(imgIn,audioIn); // Setup mixer.
  }
  for(DLIterC<StringC> it(attribs);it;it++) {
    StringC attrName = it->before('=').TopAndTail();
    StringC attrValue = it->after('=').TopAndTail();
    if(!vidIn.SetAttr(attrName,attrValue))
      cerr << "WARNING: Failed to set attribute '" << attrName << "' to '" << attrValue <<  "'\n";
  }
   
  //:---------------------------------------------------------------------------------------------
  ONDEBUG(cerr << "AVPlay: Setting up audio output stream ... \n");
  DPOPortC<SampleElemC<2,Int16T> > audioOut;
  if(!OpenOSequence(audioOut,audioOutFile,audioOutFormat,verb)) {
    cerr << "AVPlay: Failed to open '" << infile << "'\n";
    exit(1);
  }
 
  
  ONDEBUG(cerr << "AVPlay: Setting up streams.\n");
  //:---------------------------------------------------------------------------------------------
  if(start < 0) {
    start = vidIn.Tell();
    if(start < 0)
      start = 0;
  }
  UIntT endFrame = N;
  if(N != -1)
    endFrame += start;
  
  DPIPlayControlC<AVFrameC> vpCtrl(vidIn,false,start,endFrame);  
  
  ONDEBUG(cerr << "AVPlay: Play control built.\n");
  
  DPIPortC<AVFrameC> src =  vpCtrl;
  
  ONDEBUG(cerr << "AVPlay: Conversion stream built.\n");
  // Try and establish size of images.
  
  int sx = 300,sy = 300;
  
  ImageRectangleC rect;
  {
    AVFrameC tmp(vidIn.Get());
    ONDEBUG(cerr << "AVPlay: Got inital frame. \n");
    vidIn.DSeek(-1);
    rect = tmp.Image().Rectangle();
    audioOut.SetAttr("samplerate",tmp.Audio().AudioFrequency());
  }
  
  ONDEBUG(cerr << "AVPlay: Image size found..\n");
  sy = rect.Rows();
  sx = rect.Cols();
  if(sx < 1 || sy < 1) {
    cerr << "ERROR: Image size too small opening : '" << infile << "'.\n";
    sy = 100;
    sx = 100;
  }

  //vidIn.Invalidate(); // Finished with vidIn handle.
  
  // Setup GUI stuff.
  
  ONDEBUG(cerr << "Setting up GUI ... \n");
  
  WindowC win(sx,sy,infile);
  CanvasC vidout(sx,sy,directDraw);  
  
  StringC strinfile(infile);
  Tuple2C<DPIPlayControlC<AVFrameC > ,CanvasC> guiData(vpCtrl,vidout);
  
  //void file_selector(StringC &filename,FileSelectorC &fs,Tuple2C<DPIPlayControlC<ImageC<ByteRGBValueC> > ,CanvasC>  &pc)   
  // Setup file selector.
  FileSelectorC inFileSelect = RavlGUIN::FileSelector(StringC("Open Video File"),
						      strinfile,
						      &file_selector,
						      guiData
						      );
  
  // Setup menu.
  MenuBarC menuBar(MenuC("File",
			 MenuItemShow("Open",inFileSelect) +
			 MenuItem("Quit",gui_quit,vpCtrl) 
			 )
		   );
  
  TextEntryC guiTimeCode("",12,false);
  TextEntryC guiFrameRate("",5,false);

  StringC strFrameRate;
  if(vidIn.GetAttr("framerate",strFrameRate))
    frameRate = strFrameRate.RealValue();
  else {
    frameRate = (RealT) 1.0/delay;
    strFrameRate = StringC(frameRate);
  }
  
  guiFrameRate.Text(StringC(frameRate));
  
  guiPlayControl = PlayControlC(vpCtrl,simpleOnly);
  
  Connect(guiPlayControl.SigUpdateFrameNo(),&DisplayTimeCode,1,guiTimeCode);

  TableC table(4,5);
  ButtonC grab = Button("Grab Frame",&GetFileForGrab);
  
  ButtonC examine = Button("Examine",&ExamineFrame,StringC("@X"));
  
  table.AddObject(menuBar,0,4,0,1,(GtkAttachOptions) (GTK_FILL | GTK_EXPAND),(GtkAttachOptions) GTK_FILL);
  LabelC lt(" Time:");
  table.AddObject(lt,0,1,1,2,(GtkAttachOptions) (GTK_FILL | GTK_EXPAND),GTK_FILL);
  table.AddObject(guiTimeCode,1,2,1,2,(GtkAttachOptions) (GTK_FILL | GTK_EXPAND),GTK_FILL);
  LabelC lr(" Rate:");
  table.AddObject(lr,2,3,1,2,(GtkAttachOptions) (GTK_FILL | GTK_EXPAND),GTK_FILL);
  table.AddObject(guiFrameRate,3,4,1,2,(GtkAttachOptions) (GTK_FILL | GTK_EXPAND),GTK_FILL);

  table.AddObject(vidout,0,4,2,3,(GtkAttachOptions) (GTK_FILL),(GtkAttachOptions)(GTK_FILL),5,5);
  table.AddObject(guiPlayControl,0,4,3,4,(GtkAttachOptions) (GTK_FILL | GTK_SHRINK),(GtkAttachOptions)GTK_FILL);
  table.AddObject(grab,2,4,4,5,(GtkAttachOptions) (GTK_FILL| GTK_EXPAND),(GtkAttachOptions)(GTK_FILL));
  table.AddObject(examine,0,2,4,5,(GtkAttachOptions) (GTK_FILL| GTK_EXPAND),(GtkAttachOptions)(GTK_FILL));
  win.Add(table);
  win.Show();
  
  // Get GUI going ...
  
  ONDEBUG(cerr << "Executing manager... \n");
  Manager.Execute();
  Sleep(0.1); // Give it a chance to setup the display before starting the video.
  
  ONDEBUG(cerr << "Starting video... \n");
  DPEventSetC es;  
  
  // Setup play stream.

  AVRenderC avOut(OMethod(vidout,&CanvasC::DrawRGBImage),audioOut);
  
  es += src >> CacheFrame >>= OMethod(avOut,&AVRenderC::RenderFrame);
  
  es.Wait();
  Manager.Shutdown();
  
  ONDEBUG(cerr << "Terminating... \n");
  exit(0);
  return 0;
}


RAVL_ENTRY_POINT(doAVPlay);
