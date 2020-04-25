// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_THUMBNAILTIMELINE_HEADER
#define RAVL_THUMBNAILTIMELINE_HEADER 1
//! author="Charles Galambos"
//! rcsid="$Id$"
//! docentry="Ravl.API.Graphics.GTK.Util"
//! lib=RavlGUIUtil

#include "Ravl/Image/Image.hh"
#include "Ravl/DP/SPort.hh"
#include "Ravl/GUI/RawCanvas.hh"
#include "Ravl/GUI/MouseEvent.hh"
#include "Ravl/Cache.hh"
#include "Ravl/Image/WarpScale.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/RealRange1d.hh"
#include "Ravl/Threads/RWLock.hh"
#include "Ravl/Threads/Semaphore.hh"
#include "Ravl/Threads/ThreadEvent.hh"
#include "Ravl/DP/AttributeCtrlUpdateSignal.hh"

namespace RavlGUIN {
  
  using namespace RavlImageN;
  
  class ThumbNailTimeLineC;
  
  //! userlevel=Develop
  //: Video Timeline.
  // Display a piece of video as a set of thumb nails
  
  class ThumbNailTimeLineBodyC
    : public RawCanvasBodyC 
  {
  public:
    ThumbNailTimeLineBodyC(const DPISPortC<ImageC<ByteRGBValueC> > &nvideo,UIntT frameSkip =1);
    //: Constructor.
    
    ThumbNailTimeLineBodyC(UIntT frameSkip =1);
    //: Constructor.
    
    bool GUISetVideo(const DPISPortC<ImageC<ByteRGBValueC> > &nvideo);
    //: Set the current video stream to use.
    
    bool GUIDraw();
    //: Draw image.
    
    bool EventConfigure(GdkEvent* &event);
    //: Configure.
    
    bool EventExpose(GdkEvent* &event);
    //: Expose of area.
    
    bool EventMousePress(MouseEventC &mousePress);
    //: Event mouse press.
    
    bool Goto(UIntT frameNo);
    //: Goto a particular frame.
    
    bool GUIGoto(UIntT frameNo);
    //: Goto a particular frame.
    
    bool SetSkip(UIntT skip);
    //: Set frame skip factor.
    
    bool GUISetSkip(UIntT &skip);
    //: Set frame skip factor.

    IntT GetSkip(void) const;
    //: Get the current skip; 
    
    Signal1C<UIntT> &SigFrameSelected()
    { return frameSelected; }
    //: Access frame selected signal.
    
    Signal1C<RealRangeC> &SigFrameRange()
    { return sigFrameRange; }
    //: Signal issued when the range of frames displayed changes.
    
    bool Terminate();
    //: Terminate display thread.
    
    IntT FrameSkip() const
    { return frameSkip; }
    //: Access current frame skip.
    
    virtual bool GUIShow();
    //: Show timeline.
    
    virtual bool GUIHide();
    //: Hide timeline.
    
    virtual bool Create();
    //: Create the widget.
    
    virtual bool Create(GtkWidget *_widget);
    //: Create the widget.
    
    virtual bool ModifyThumbImage(ImageC<ByteRGBValueC> & image, UIntT frameNo);
    //: Modify the image for thumbnail, scale it, label it....
    
  protected:


    

    virtual bool CommonCreate(GtkWidget *_widget);
    //: Create widget.
    
    virtual bool GUIUpdateDisplayRange();
    //: Compute display range.
    
    bool UpdateDisplayRange();
    //: Compute display range.
    
    bool DisplayUpdateThread();
    //: Display update thread.

    bool GUILocalDrawImage(UIntT anUpdateId,ImageC<ByteRGBValueC> &image,const Index2dC &offset);
    //: Draw an image on the timeline.
    
    bool GUILocalDrawRectangle(UIntT anUpdateId,const IndexRange2dC &rect);
    //: Draw a rectangle.

    bool LocalDrawImage(UIntT anUpdateId,ImageC<ByteRGBValueC> &image,const Index2dC &offset);
    //: Draw an image on the timeline.
    
    bool LocalDrawRectangle(UIntT anUpdateId,const IndexRange2dC &rect);
    //: Draw a rectangle.
    
    ImageC<ByteRGBValueC> GetDisplayImage(UIntT frameNo);
    //: Retrieve the image to display for 'frameNo'
    
    bool GUIClearImageCache(void);
    //: Empty the cache of thumbnail images
    
    bool CBStreamSizeChanged();
    //: Handle stream size changed.

    bool Freeze();
    //: Freeze updates

    bool Thaw();
    //: Un-Freeze updates 
    
    RWLockC access;
    SemaphoreC semaUpdate; // Signal we need an update.
    
    IndexRange2dC displayArea; // Area to use for displaying images.
    RealT aspectRatio; // Aspect ratio for images.
    DPISPortC<ImageC<ByteRGBValueC> > video;    // Handle to video sequence.
    CacheC<UIntT,ImageC<ByteRGBValueC> > imageCache; // Cache of thumb nails.
    WarpScaleC<ByteRGBValueC,ByteRGBValueC> warpScale;
    DListC<Tuple2C<IndexRange2dC,UIntT> > rect2frameno;
    
    AttributeCtrlUpdateSignalC inputStreamSizeChangedSignal;
    
    UIntT requestedMidFrame; // Frame requested to be in the middle of the sequence.
    UIntT midFrame; // Frame to display in the middle of the sequence. (Nearest to reqestedMidFrame in frame skip steps)
    IntT frameSkip; // No of frame's between those being displayed.
    IntT vertSpace; // Space to leave between images.
    UIntT updateId; // Update ID. Use to discard unneed draw requests.
    
    volatile bool terminate;
    volatile bool visible; // Is the timeline visible ?
    
    Signal1C<UIntT> frameSelected; // Frame selected signal
    Signal1C<RealRangeC> sigFrameRange; // Signal issued when the range of frames displayed changes.

    volatile bool m_freeze;
    friend class ThumbNailTimeLineC;
  };
  
  //! userlevel=Normal
  //: Video Timeline.
  // Display a piece of video as a set of thumb nails
  // NOTE: Care should be taken as this class will read from the video sequence on seperate thread.
  
  class ThumbNailTimeLineC
    : public RawCanvasC 
  {
  public:
    ThumbNailTimeLineC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    ThumbNailTimeLineC(const DPISPortC<ImageC<ByteRGBValueC> > &nvideo,UIntT frameSkip = 1);
    //: Constructor.
    
    ThumbNailTimeLineC(UIntT frameSkip);
    //: Constructor.
    
    bool Create() 
    { return Body().Create(); }
    //: Create the widget. 
    //!cwiz:author
    
  protected:
    ThumbNailTimeLineC(ThumbNailTimeLineBodyC &bod)
      : RawCanvasC(bod){}
    //: Body constructor
    
    ThumbNailTimeLineBodyC &Body()
    { return static_cast<ThumbNailTimeLineBodyC &>(WidgetC::Body()); }
    //: Access body.
    
    const ThumbNailTimeLineBodyC &Body() const
    { return static_cast<const ThumbNailTimeLineBodyC &>(WidgetC::Body()); }
    //: Access body.
    
    bool DisplayUpdateThread()
    { return Body().DisplayUpdateThread(); }
    //: Display update thread.
    
    bool GUILocalDrawImage(UIntT anUpdateId,ImageC<ByteRGBValueC> &image,const Index2dC &offset)
    { return Body().GUILocalDrawImage(anUpdateId,image,offset); }
    //: Draw an image on the timeline.
    
    bool GUILocalDrawRectangle(UIntT anUpdateId,const IndexRange2dC &rect)
    { return Body().GUILocalDrawRectangle(anUpdateId,rect); }
    //: Draw a rectangle.
    
    bool LocalDrawImage(UIntT anUpdateId,ImageC<ByteRGBValueC> &image,const Index2dC &offset)
    { return Body().LocalDrawImage(anUpdateId,image,offset); }
    //: Draw an image on the timeline.
    
    bool LocalDrawRectangle(UIntT anUpdateId,const IndexRange2dC &rect)
    { return Body().LocalDrawRectangle(anUpdateId,rect); }
    //: Draw a rectangle.
    
    bool GUIUpdateDisplayRange()
    { return Body().GUIUpdateDisplayRange(); }
    //: Compute display range.
    
    bool UpdateDisplayRange()
    { return Body().UpdateDisplayRange(); }
    //: Compute display range.
    
  public:
    
    bool GUIGoto(UIntT frameNo)
    { return Body().GUIGoto(frameNo); }
    //: Goto a particular frame.
    
    bool Goto(UIntT frameNo)
    { return Body().Goto(frameNo); }
    //: Goto a particular frame.
    
    bool SetSkip(UIntT skip)
    { return Body().SetSkip(skip); }
    //: Set frame skip factor.
    
    bool GUISetSkip(UIntT &skip)
    { return Body().GUISetSkip(skip); }
    //: Set frame skip factor.

    IntT GetSkip(void) const
    { return Body().GetSkip(); }
    //: Access teh currrent skip

    bool GUISetVideo(const DPISPortC<ImageC<ByteRGBValueC> > &nvideo)
    { return Body().GUISetVideo(nvideo); }
    //: Set the current video stream to use.
    // NOTE: The video source must be thread safe.
    
    bool GUIDraw() 
    { return Body().GUIDraw(); }
    //: Draw image. 
    //!cwiz:author
    
    bool EventConfigure(GdkEvent *& event) 
    { return Body().EventConfigure(event); }
    //: Configure. 
    //!cwiz:author
    
    bool EventExpose(GdkEvent *& event) 
    { return Body().EventExpose(event); }
    //: Expose of area. 
    //!cwiz:author
    
    bool EventMousePress(MouseEventC & mousePress) 
    { return Body().EventMousePress(mousePress); }
    //: Event mouse press. 
    //!cwiz:author
    
    Signal1C<UIntT> &SigFrameSelected()
    { return Body().SigFrameSelected(); }
    //: Access frame selected signal.
    
    Signal1C<RealRangeC> &SigFrameRange()
    { return Body().SigFrameRange(); }
    //: Signal issued when the range of frames displayed changes.
    
    bool Terminate()
    { return Body().Terminate(); }
    //: Terminate display thread.
    
    IntT FrameSkip() const
    { return Body().FrameSkip(); }
    //: Access current frame skip.

    bool Freeze()
    { return Body().Freeze(); }

    bool Thaw()
    { return Body().Thaw(); }

    friend class ThumbNailTimeLineBodyC;
  };
  
  
}


#endif
