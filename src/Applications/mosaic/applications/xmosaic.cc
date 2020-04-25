// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImageProc
//! file="Ravl/Applications/Tracker/xmosaic.cc"
//! author="Phil McLauchlan"
//! docentry="Ravl.Applications.Video"
//! userlevel=Normal

//: Creates a mosaic from a video sequence interactively
//
// Like <a href=mosaic.html>mosaic</a>, but has a GUI and is therefore more interactive.

#include "Ravl/EntryPnt.hh"
#include "Ravl/Option.hh"
#include "Ravl/GUI/Canvas.hh"
#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/Button.hh"
#include "Ravl/GUI/LBox.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/Menu.hh"
#include "Ravl/GUI/MessageBox.hh"
#include "Ravl/GUI/FileSelector.hh"
#include "Ravl/Image/MosaicBuilder.hh"
#include "Ravl/Image/ImageConv.hh"
#include "Ravl/Threads/LaunchThread.hh"
#include "Ravl/Threads/RWLock.hh"
#include "Ravl/IO.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

#define IMAGE_ZHOMOG 100.0
#define MOSAIC_ZHOMOG 1.0

using namespace RavlImageN;
using namespace RavlGUIN;

class XMosaicC;

class XMosaicBodyC
  : public WindowBodyC
{
public:
  XMosaicBodyC(const StringC &title,const DPIPortC<ImageC<ByteRGBValueC> >  &nImgSrc,const MosaicBuilderC &mb);
  //: Constructor.
  
protected:
  void Init();
  //: Setup window.
  
  bool BuilderThread();
  //: Mosaic building thread.
  
  virtual void Destroy();
  //: Undo all references and close down threads.

  bool PauseBuild() { 
    pause.Reset(); 
    pauseButton.SetState(GTK_STATE_INSENSITIVE);
    resumeButton.SetState(GTK_STATE_NORMAL);
    return true;
  }
  //: Pause mosaic building.
  
  bool ResumeBuild() {
    pause.Post(); 
    pauseButton.SetState(GTK_STATE_NORMAL);
    resumeButton.SetState(GTK_STATE_INSENSITIVE);
    return true;
  }
  //:Resume mosaic building.
  
  bool SaveMosaic(StringC &fn);
  //: Save mosaic to a file.
  
  bool Reset() {
    resetMosaic = true;
    canvas.Clear();
    return true;
  }
  //: Clear mosaic and start again.
  
  //:---------------------
  //: Image processing.
  
  DPIPortC<ImageC<ByteRGBValueC> > imgSrc;
  MosaicBuilderC mosaicBuilder;

  //:---------------------
  //: Processing.

  LaunchThreadC buildThread;
  RWLockC accessMosaic; // Access lock for buildMosaic.
  volatile bool finished;
  volatile bool resetMosaic;
  
  //:---------------------
  //: GUI stuff
  
  CanvasC canvas;
  ButtonC pauseButton;
  ButtonC resumeButton;
  
  ThreadEventC pause;
  
  friend class XMosaicC;
};

class XMosaicC
  : public WindowC
{
public:
  XMosaicC(const StringC &title,const DPIPortC<ImageC<ByteRGBValueC> >  &imgSrc,const MosaicBuilderC &mb)
    : WindowC(*new XMosaicBodyC(title,imgSrc,mb))
  {}
  //: Constructor.

protected:
  XMosaicC(XMosaicBodyC &bod)
    : WindowC(bod)
  {}
  //: Body constructor.
  
  XMosaicBodyC &Body()
  { return static_cast<XMosaicBodyC &>(WidgetC::Body()); }
  //: Access body.

  const XMosaicBodyC &Body() const
  { return static_cast<const XMosaicBodyC &>(WidgetC::Body()); }
  //: Access body.
  
  bool BuilderThread()
  { return Body().BuilderThread(); }
  //: Mosaic building thread.

  friend class XMosaicBodyC;
};

//////////////////////////////////////////////////////////////////////////////

//: Constructor.

XMosaicBodyC::XMosaicBodyC(const StringC &title,const DPIPortC<ImageC<ByteRGBValueC> >  &nImgSrc,const MosaicBuilderC &mb)
  : WindowBodyC(500,500,title),
    imgSrc(nImgSrc),
    mosaicBuilder(mb),
    finished(false),
    resetMosaic(false),
    canvas(500,500)
{ Init(); }

bool gui_quit() 
{
  Manager.Quit(); // Initate shutdown.
  return true;
}

//: Setup window.

void XMosaicBodyC::Init() {
  
  resumeButton = ButtonR("Start",*this,&XMosaicBodyC::ResumeBuild);
  pauseButton = ButtonR("Pause",*this,&XMosaicBodyC::PauseBuild);
  
  MenuBarC menuBar(MenuC("File",
			 MenuItemShow("Save Mosaic",FileSelectorR("Save Mosaic",*this,&XMosaicBodyC::SaveMosaic)) +
			 MenuItemSeparator() +
			 MenuItem("Quit",gui_quit) 
			 ));
  
  Add(VBox(menuBar +
	   canvas + 
	   HBox(pauseButton +
		resumeButton +
		ButtonR("Clear",*this,&XMosaicBodyC::Reset)
		)));
  
  Show();
  
  pauseButton.SetState(GTK_STATE_INSENSITIVE);
  resumeButton.SetState(GTK_STATE_NORMAL);
  
  buildThread = LaunchThread(XMosaicC(*this),&XMosaicC::BuilderThread);
}

//: Undo all references and close down threads.

void XMosaicBodyC::Destroy() {
  finished = true; // Start shutdown.
  
  buildThread.WaitForExit(); // Wait for thread to exit.
  
  WindowBodyC::Destroy(); // Call parent destroy.
}

//: Save mosaic to a file.

bool XMosaicBodyC::SaveMosaic(StringC &fn) {
    RWLockHoldC holdMosaic(accessMosaic,RWLOCK_READONLY);
    ImageC<ByteRGBValueC> mosaic = mosaicBuilder.GetMosaic();
    holdMosaic.Unlock(); // Finished reading mosaic.
    if(!RavlN::Save(fn,mosaic)) // Save RGB image.
      AlertBox(StringC("Failed to save mosaic to '") + fn + "'.");
    return true;
}

//: Mosaic building thread.

bool XMosaicBodyC::BuilderThread() {
  ONDEBUG(cerr << "XMosaicBodyC::BuilderThread(), Called. \n");
  while(!finished) {
    pause.Wait();
    ONDEBUG(cerr <<"XMosaicBodyC::BuilderThread(), Got new image. \n");
    ImageC<ByteRGBValueC> img;
    if(!imgSrc.Get(img)) {
      cerr << "XMosaicBodyC::BuilderThread(), Failed to read image from source. \n";
      break;
    }
    //: Update mosaic..
    
    RWLockHoldC holdMosaic(accessMosaic,RWLOCK_WRITE);
    if(resetMosaic) {
      mosaicBuilder.Reset();
      resetMosaic = false;
    } 
    mosaicBuilder.Apply(img);
    holdMosaic.Unlock(); // Finished updating mosaic.
    
    //: Get updated part of image...
    holdMosaic.LockRd(); // Only need a read lock when rendering the image.
#if 0
    // Try and update only the modified section...
    IndexRange2dC cropRect = mosaicBuilder.GetCropRect();
    ImageC<ByteRGBValueC> mosaic(mosaicBuilder.GetMosaic(),cropRect);
#else
    ImageC<ByteRGBValueC> mosaic = mosaicBuilder.GetMosaic();
#endif
    holdMosaic.Unlock(); // Finished reading mosaic.
    
    //: Render it to the canvas.
    canvas.DrawImage(mosaic,mosaic.Frame().Origin());
  }
  ONDEBUG(cerr << "XMosaicBodyC::BuilderThread(), Exiting. \n");
  return true;
}

////////////////////////////////////////////////////////////////////////////////

int xmosaic(int nargs,char **argv) {
  OptionC opt(nargs,argv);
  int newFreq    = opt.Int("nf",5,"Frequency of introducing new features. ");
  int cthreshold = opt.Int("ct",30,"Corner threshold. ");
  int cwidth     = opt.Int("cw",7,"Corner filter width. ");
  int mthreshold = opt.Int("mt",20,"Match threshold. ");
  int mwidth     = opt.Int("mw",17,"Tracker feature width (i.e. minimum distance between features). ");
  int lifeTime   = opt.Int("ml",8,"Lifetime of a point without a match in the incoming images. ");
  int searchSize = opt.Int("ss",25,"Search size. How far to look from the predicted position of the feature.");
  int borderC    = opt.Int("bc", 200, "Width of horizontal border around image");
  int borderR    = opt.Int("br", 200, "Width of vertical border around image");
  int cropT = opt.Int("crt", 0, "Width of cropping region at top of image");
  int cropB = opt.Int("crb", 0, "Width of cropping region at bottom of image");
  int cropL = opt.Int("crl", 0, "Width of cropping region at left of image");
  int cropR = opt.Int("crr", 0, "Width of cropping region at right of image");
  Point2dC pointTL = opt.Point2d("ptl", 0.0, 0.0, "Top-left coordinates of first image");
  Point2dC pointTR = opt.Point2d("ptr", 0.0, 1.0, "Top-right coordinates of first image");
  Point2dC pointBL = opt.Point2d("pbl", 1.0, 0.0, "Bottom-left coordinates of first image");
  Point2dC pointBR = opt.Point2d("pbr", 1.0, 1.0, "Bottom-right coordinates of first image");
  int maxFrames = opt.Int("mf",-1,"Maximum number of frames to process ");
  StringC ifn = opt.String("","@V4LH:/dev/video0","Input sequence. ");
  StringC ofn = opt.String("","@X","Output sequence. ");
  opt.Check();
  
  // Open an input stream.
  DPIPortC<ImageC<ByteRGBValueC> > inp;
  if(!OpenISequence(inp,ifn)) {
    cerr << "Failed to open input '" << ifn << "'\n";
    return 1;
  }
  
  // Create a mosaic class instance
  MosaicBuilderC mosaicBuilder(cthreshold, cwidth, mthreshold, mwidth,
			       lifeTime, searchSize, newFreq,
			       borderC, borderR, IMAGE_ZHOMOG,
			       cropT, cropB, cropL, cropR,
			       pointTL, pointTR, pointBL, pointBR,
			       maxFrames);
  
  XMosaicC win(ifn,inp,mosaicBuilder);
  
  Manager.Start();
  
  return 0;
}

RAVL_ENTRY_POINT(xmosaic);
