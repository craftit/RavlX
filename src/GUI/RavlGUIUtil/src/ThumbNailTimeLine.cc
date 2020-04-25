// This file is part of Magellan, Broadcast Video Library 
// Copyright (C) 2003, OmniPerception Ltd.
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlGUIUtil
//! file="Magellan/GUI/VideoSampleTimeLine.cc"

#include "Ravl/GUI/ThumbNailTimeLine.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/Image/Font.hh"
#include "Ravl/Image/WarpScale.hh"
#include "Ravl/Threads/LaunchThread.hh"
#include "Ravl/CallMethods.hh"
#include "Ravl/OS/SysLog.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlGUIN {
  
  
  ThumbNailTimeLineBodyC::ThumbNailTimeLineBodyC(const DPISPortC<ImageC<ByteRGBValueC> > &nvideo,UIntT nFrameSkip)
    : RawCanvasBodyC(100,100),
      semaUpdate(0),
      displayArea(0,0),
      aspectRatio(1.333),
      video(nvideo),
      imageCache(100),
      warpScale(IndexRange2dC(0,100,0,100),WARPSCALE_NEARESTNEIGHBOUR),
      requestedMidFrame(10),
      midFrame(10),
      frameSkip(nFrameSkip),
      vertSpace(4),
      updateId(0),
      terminate(false),
      visible(true),
      frameSelected((UIntT) 0),
      sigFrameRange(RealRangeC(0,1)),
      m_freeze(false)
  {
    inputStreamSizeChangedSignal.Connect("size",TriggerR(*this,&ThumbNailTimeLineBodyC::CBStreamSizeChanged));
  }
  
  //: Constructor.
  
  ThumbNailTimeLineBodyC::ThumbNailTimeLineBodyC(UIntT nFrameSkip) 
    : RawCanvasBodyC(100,100),
      semaUpdate(0),
      displayArea(0,0),
      aspectRatio(1.333),
      imageCache(100),
      warpScale(IndexRange2dC(0,100,0,100),WARPSCALE_NEARESTNEIGHBOUR),
      requestedMidFrame(10),
      midFrame(10),
      frameSkip(nFrameSkip),
      vertSpace(4),
      updateId(0),
      terminate(false),
      visible(true),
      frameSelected((UIntT) 0),
      sigFrameRange(RealRangeC(0,1)),
      m_freeze(false)
  {
    inputStreamSizeChangedSignal.Connect("size",TriggerR(*this,&ThumbNailTimeLineBodyC::CBStreamSizeChanged));
  }

  
  //: Freeze updates
  bool ThumbNailTimeLineBodyC::Freeze()
  {
    m_freeze = true ;
    return true;
  }
  
  //: Thaw updates 
   
  bool ThumbNailTimeLineBodyC::Thaw()
  {
    m_freeze = false ;
    semaUpdate.Post() ;
    return true;
  }
  
  
  
  //: Show timeline.
  
  bool ThumbNailTimeLineBodyC::GUIShow() {
    visible = true;
    semaUpdate.Post(); // Make sure an update happens.
    return WidgetBodyC::GUIShow();
  }
  
  
  //: Hide timeline.
  
  bool ThumbNailTimeLineBodyC::GUIHide() {
    visible = false;
    return WidgetBodyC::GUIHide();
  }
  
  //: Terminate display thread.
  
  bool ThumbNailTimeLineBodyC::Terminate() {
    terminate = true;
    semaUpdate.Post();
    return true;
  }
  
  //: Create the widget.
  
  bool ThumbNailTimeLineBodyC::Create() {
    return CommonCreate(0);
  }
    
  //: Create the widget.
  
  bool ThumbNailTimeLineBodyC::Create(GtkWidget *_widget) {
    return CommonCreate(_widget);
  }
  
  //: Create the widget.
  
  bool ThumbNailTimeLineBodyC::CommonCreate(GtkWidget *_widget) {
    ConnectRef(Signal("expose_event"),*this,&ThumbNailTimeLineBodyC::EventExpose);
    ConnectRef(Signal("configure_event"),*this,&ThumbNailTimeLineBodyC::EventConfigure);
    ConnectRef(Signal("button_press_event"),*this,&ThumbNailTimeLineBodyC::EventMousePress);
    
    bool ret = false;
    if (_widget == NULL)
      ret = RawCanvasBodyC::Create();
    else
      ret = RawCanvasBodyC::Create(_widget);
    
    return ret;
  }
  
  //: Set the current video stream to use.
  
  bool ThumbNailTimeLineBodyC::GUISetVideo(const DPISPortC<ImageC<ByteRGBValueC> > &nvideo) {
    ONDEBUG(cerr << "bool ThumbNailTimeLineBodyC::GUISetVideo(const DPISPortC<ImageC<ByteRGBValueC> > &nvideo)\n");
    RWLockHoldC hold(access,RWLOCK_WRITE);
    imageCache.Empty();
    video = nvideo;
    requestedMidFrame = 0;
    midFrame = 0;
    inputStreamSizeChangedSignal.ChangeControl(nvideo);
    hold.Unlock();
    
    GUIDraw(); // Update the display.
    return true;
  }
  
  //: Retrieve the image to display for 'frameNo'
  
  ImageC<ByteRGBValueC> ThumbNailTimeLineBodyC::GetDisplayImage(UIntT frameNo) {

    ImageC<ByteRGBValueC> img;
    
    // Try getting image from cache with just a read lock.
    RWLockHoldC hold(access,RWLOCK_READONLY);
    DPISPortC<ImageC<ByteRGBValueC> > videoIn;
    do {
      if(imageCache.LookupR(frameNo,img))
        return img;
      videoIn = video;
      hold.Unlock();
      
      // Ok the image isn't in the cache when we looked then, 
      // to avoid deadlocks don't obtain a write lock until the last
      // possible time.
      
      // Got a good video stream ?
      if(!videoIn.IsValid())
        return img;
      
      // Read the relavent frame of video.
      if(!videoIn.GetAt(frameNo,img)) {
        //cerr << "ThumbNailTimeLineBodyC::GUIGetDisplayImage(), WARNING: Failed to get image. \n";
        return img;
      }
      
      // Lock for writting when updating the cache.
      hold.LockWr();
    } while(videoIn != video) ; // Has the video source changed when we're not looking ? If so start again.
    
    // Double check its not been added while we weren't looking.
    if(imageCache.LookupR(frameNo,img))
      return img;
    
    img = warpScale.Apply(img); // Warp to appropriate size.
    ModifyThumbImage(img, frameNo);
    imageCache.Insert(frameNo,img); // Put image into cache.
    return img;
  
  }
  
  
  //: Modify the image for the timeline. 
  bool ThumbNailTimeLineBodyC::ModifyThumbImage(ImageC<ByteRGBValueC> & img, UIntT frameNo)
  {
    FontC &font = DefaultFont();
    Index2dC off(3,3);
    Index2dC boarder(2,2);
    if(font.IsValid() && img.Frame().Contains(off) && img.Frame().Contains(off+Index2dC(10,10))) { // Check we've got a font, and image is of reasonable size.
      ByteRGBValueC col;
      col = ByteRGBValueC(255,255,255);
      StringC str(frameNo);
      Index2dC size = font.Size(str);
      IndexRange2dC rect(off,off + size + (boarder*2));
      rect.ClipBy(img.Frame());
      ImageC<ByteRGBValueC> textBack(img,rect);
      textBack /= ByteRGBValueC(2,2,2); // Darken.
      DrawText(font,col,off + boarder,str,img);
    }
    return true;
  }
  
  //: Configure.
  
  bool ThumbNailTimeLineBodyC::EventConfigure(GdkEvent* &event) {
    ONDEBUG(cerr << "ThumbNailTimeLineBodyC::EventConfigure(GdkEvent* &event) Called. \n");
    IndexRange2dC newRec;
    TranslateConfigureEvent(event,newRec);
    ONDEBUG(cerr << " Widget=" << newRec << "\n");
    if(newRec == displayArea)
      return true; // Same size, nothing to do!
    RWLockHoldC hold(access,RWLOCK_WRITE);
    displayArea = newRec;

    // Images are the same size in current version, so no need to clear check if 
    // we need to redo these bits
    
    IndexRange2dC viewRec(newRec.Rows(),Round(newRec.Rows() * aspectRatio));
    warpScale.SetRectangle(viewRec); // Setup new rectangle.
    imageCache.Empty(); 
    hold.Unlock();
    
    
    GUIUpdateDisplayRange(); // Send signal about update.
    return true;
  }
  
  //: Expose of area.
  
  bool ThumbNailTimeLineBodyC::EventExpose(GdkEvent* &event) {
    ONDEBUG(cerr << "ThumbNailTimeLineBodyC::EventExpose(GdkEvent* &event) Called. \n");
    IntT toFollow;
    IndexRange2dC newRec;
    TranslateExposeEvent(event,newRec,toFollow);
    GUIDraw(); // Set off redraw.
    return true;
  }

  //: Event mouse press.
  
  bool ThumbNailTimeLineBodyC::EventMousePress(MouseEventC &mousePress) {
    ONDEBUG(cerr << "ThumbNailTimeLineBodyC::EventMousePress(MouseEventC &) Called. \n");
    // Look through list of rectangles.
    
    RWLockHoldC hold(access,RWLOCK_READONLY);
    DLIterC<Tuple2C<IndexRange2dC,UIntT> > it(rect2frameno); // Only ever replace this list, so just need a lock when we get it.
    ONDEBUG(cerr << "ThumbNailTimeLineBodyC::EventMousePress(MouseEventC &) List=" << rect2frameno.Size() << " \n");
    hold.Unlock();
    
    for(;it;it++) {
      if(it->Data1().Contains(mousePress.At())) {
	ONDEBUG(cerr << "ThumbNailTimeLineBodyC::EventMousePress(MouseEventC &) Got " << it->Data2() << " \n");
	frameSelected(it->Data2()); // Issue signal.
	break;
      }
    }
    return true;
  }
  

  //: Compute display range.
  
  bool ThumbNailTimeLineBodyC::UpdateDisplayRange() {    
    if(Manager.IsGUIThread()) { // If we're on the GUI thread call directly.
      GUIUpdateDisplayRange();
      return true;
    }
    Manager.Queue(Trigger(ThumbNailTimeLineC(*this),&ThumbNailTimeLineC::GUIUpdateDisplayRange));
    return true;
  }
  
  //: Compute display range.
  
  bool ThumbNailTimeLineBodyC::GUIUpdateDisplayRange() {
    RWLockHoldC hold(access,RWLOCK_READONLY);
    
    int width = warpScale.OutputRectangle().Cols() + vertSpace;
    RealT realFrames = (RealT) displayArea.Cols() / (RealT) width;
    
    // Compute display width.
    RealT frameSpan = (realFrames * (RealT) frameSkip)/2;
    RealT offset = (RealT) midFrame + (RealT) frameSkip/2 - ((RealT)vertSpace/(RealT)displayArea.Cols()) * (RealT)frameSpan;
    RealRangeC rng(offset - frameSpan,offset + frameSpan);
    hold.Unlock();
    sigFrameRange(rng);
    return true;
  }
  
  //: Draw image.  
  
  bool ThumbNailTimeLineBodyC::GUIDraw() {
    if(widget == 0) // Are we ready yet ?
      return true;
    //cerr << "ThumbNailTimeLineBodyC::GUIDraw(), Display =" << displayArea << "\n";
    semaUpdate.Post();
    
    return true;
  }
  
  //: Draw an image on the timeline.
  
  bool ThumbNailTimeLineBodyC::GUILocalDrawImage(UIntT anUpdateId,ImageC<ByteRGBValueC> &image,const Index2dC &at) {
    if(anUpdateId != updateId)
      return true; // Don't bother, another update is on its way.
    GUIDrawImage(image,at,true);
    return true;
  }
  
  //: Draw a rectangle.
  
  bool ThumbNailTimeLineBodyC::GUILocalDrawRectangle(UIntT anUpdateId,const IndexRange2dC &rect) {
    if(anUpdateId != updateId)
      return true; // Don't bother, another update is on its way.
    GUIDrawRectangle(GUIDrawGCWhite(),rect,true);
    return true;
  }
  
  //: Draw an image on the timeline.
  
  bool ThumbNailTimeLineBodyC::LocalDrawImage(UIntT anUpdateId,ImageC<ByteRGBValueC> &image,const Index2dC &offset) {
    Manager.Queue(Trigger(ThumbNailTimeLineC(*this),&ThumbNailTimeLineC::GUILocalDrawImage,anUpdateId,image,offset));
    return true;
  }
  
  //: Draw a rectangle.
  
  bool ThumbNailTimeLineBodyC::LocalDrawRectangle(UIntT anUpdateId,const IndexRange2dC &rect) {
    Manager.Queue(Trigger(ThumbNailTimeLineC(*this),&ThumbNailTimeLineC::GUILocalDrawRectangle,anUpdateId,rect));
    return true;
  }
  
  //: Display update thread.
  
  bool ThumbNailTimeLineBodyC::DisplayUpdateThread() {
    ONDEBUG(cerr << "ThumbNailTimeLineBodyC::DisplayUpdateThread(), Started. \n");

    bool restart = false;
    // If refrences reaches 1, we're the only thing with a handle so quit.
    while(!terminate && References() > 1) { 
      if(!restart && !semaUpdate.Wait(5))
        continue;

      // Ignore update while frozen.
      if (m_freeze)
        continue;


      ONDEBUG(cerr << "ThumbNailTimeLineBodyC::DisplayUpdateThread(), Processing.... \n");
      
      if(!visible)
        continue;  // No point in updating the display unless the timeline is visible!
      
      restart = false;
      DListC<Tuple2C<IndexRange2dC,UIntT> > newRect2frameno;
      
      // Get information about current setup.
      
      RWLockHoldC hold(access,RWLOCK_READONLY);
      while(semaUpdate.TryWait()) ; // Eat up any pending signals.
      
      IndexRange2dC workingDisplayArea = displayArea;
      IndexRange2dC thumbSize = warpScale.OutputRectangle();
      IntT workingFrameSkip = frameSkip;
      IntT workingMidFrame = midFrame;
      
      updateId++;  // Do we need to lock this ??
      hold.Unlock();
      
      ONDEBUG(cerr << "ThumbNailTimeLineBodyC::DisplayUpdateThread(), Drawing.... \n");
      //cerr << "Range=" << rng << " Mid=" << midFrame << " Span=" << frameSpan << "\n";
      
      //rect2frameno.Empty();
      if(workingDisplayArea.Cols() < 1 || workingDisplayArea.Rows() < 1)
	continue;
      
      // Compute the numer of frames visible.
      int width = thumbSize.Cols() + vertSpace;
      RealT realFrames = (RealT) workingDisplayArea.Cols() / (RealT) width;
      int noFrames = Floor(realFrames) + 1;
      if((noFrames % 2) == 0)
	noFrames++;
      //ONDEBUG(cerr << "Noframes=" << noFrames << "\n");
      int startFrame = -noFrames/2;
      int endFrame = noFrames + startFrame;
      for(int i = startFrame;i < endFrame;i++) {
	if(semaUpdate.TryWait()) { // Check if we've been interupted.
	  restart = true;
	  break;
	}
	
	int frameNo = (i * workingFrameSkip) + workingMidFrame;
	//ONDEBUG(cerr << "FrameNo=" << frameNo << " i=" << i << "\n");
	Index2dC at(0,IndexC(workingDisplayArea.Cols())/2 + i * width  + vertSpace/2 - IndexC(thumbSize.Cols())/2);
	ImageC<ByteRGBValueC> img;
	if(frameNo >= 0) {
	  img = GetDisplayImage(frameNo);
    ONDEBUG(cerr << "ThumbNailTimeLineBodyC::DisplayUpdateThread(), got image for frame " << frameNo << endl);
  }
    //ONDEBUG(cerr << "Frame=" << img.Frame() << "\n");
	if(img.IsEmpty()) { // Do we have an image to display?
	  //ONDEBUG(cerr << "Empty frame. \n");
	  // Draw a white rectangle.
	  IndexRange2dC rect = thumbSize + at;
	  rect.ClipBy(workingDisplayArea);
	  //ONDEBUG(cerr << "  Rect=" << rect << " DisplayArea=" << workingDisplayArea << "\n");
	  if(rect.Area() > 0)
	    LocalDrawRectangle(updateId,rect);
	  continue; 
	}
	
	if(semaUpdate.TryWait()) { // Check if we've been interupted.
	  restart = true;
	  break;
	}
	
	//cerr << "atx=" << atx << "\n";
  ONDEBUG(cerr << "ThumbNailTimeLineBodyC::DisplayUpdateThread(), Draw image....\n");
	LocalDrawImage(updateId,img,at);
	newRect2frameno.InsLast(Tuple2C<IndexRange2dC,UIntT>(img.Frame()+at,frameNo));
      }
      
      ONDEBUG(cerr << "ThumbNailTimeLineBodyC::DisplayUpdateThread(), Finished update. " << restart << " \n");
      if(!restart) { // Did we get interupted ?
	// Update select list.
	hold.LockWr();
	rect2frameno = newRect2frameno;
	hold.Unlock();
      }
    }
    
    ONDEBUG(cerr << "ThumbNailTimeLineBodyC::DisplayUpdateThread(), Exiting. \n");
    return true;
  }

  //: Goto a particular frame.
  
  bool ThumbNailTimeLineBodyC::Goto(UIntT newFrameNo) {
    RWLockHoldC hold(access,RWLOCK_WRITE);
    requestedMidFrame = newFrameNo;
    UIntT frameNo = Floor((RealT) newFrameNo / frameSkip) * frameSkip;
    if(midFrame == frameNo) return true;
    midFrame = frameNo;
    hold.Unlock();
    UpdateDisplayRange();
    semaUpdate.Post(); // Flag update.
    return true;
  }
  
  //: Goto a particular frame.
  
  bool ThumbNailTimeLineBodyC::GUIGoto(UIntT newFrameNo) 
  {
    if(!Goto(newFrameNo))
      return false;
    return true;
  }
  
  //: Set frame skip factor.
  
  bool ThumbNailTimeLineBodyC::SetSkip(UIntT skip) {
    RWLockHoldC hold(access,RWLOCK_WRITE);
    if(frameSkip == (int) skip)
      return true;
    frameSkip = skip;
    if (frameSkip==0)
      frameSkip = 1; 
    
    // Re-center as needed.
    midFrame = Floor((RealT) requestedMidFrame / frameSkip) * frameSkip;
    
    hold.Unlock();
    UpdateDisplayRange();
    semaUpdate.Post(); // Flag update.
    return true;
  }



  //: Get frame skip factor
  IntT ThumbNailTimeLineBodyC::GetSkip() const
  {
    RWLockHoldC hold(access, RWLOCK_READONLY);
    return frameSkip;
  }



  //: Set frame skip factor.
  
  bool ThumbNailTimeLineBodyC::GUISetSkip(UIntT &skip) 
  { 
    if(!SetSkip(skip))
      return false;
    return true;
  }
  
  //: Handle stream size changed.
  
  bool ThumbNailTimeLineBodyC::CBStreamSizeChanged() {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "ThumbNailTimeLineBodyC::CBStreamSizeChanged, Called. ");
    semaUpdate.Post(); // Flag update.
    return true;
  }

  //:-----------------------------------------------------------------------------------
  
  //: Constructor.
  
  ThumbNailTimeLineC::ThumbNailTimeLineC(const DPISPortC<ImageC<ByteRGBValueC> > &nvideo,UIntT frameSkip)
    : RawCanvasC(*new ThumbNailTimeLineBodyC(nvideo,frameSkip))
  { LaunchThread(*this,&ThumbNailTimeLineC::DisplayUpdateThread); }
  
  //: Constructor.
  
  ThumbNailTimeLineC::ThumbNailTimeLineC(UIntT frameSkip)
    : RawCanvasC(*new ThumbNailTimeLineBodyC(frameSkip))
  { LaunchThread(*this,&ThumbNailTimeLineC::DisplayUpdateThread); }
  

  
}
