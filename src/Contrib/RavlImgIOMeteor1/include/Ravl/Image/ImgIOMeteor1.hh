// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_VIDMETEOR1IO_HEADER
#define RAVL_VIDMETEOR1IO_HEADER 1
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Images.Video.Video IO.Meteor1"
//! lib=RavlImgIOMeteor1
//! author="Charles Galambos"
//! file="Ravl/Contrib/Meteor1/ImgIOMeteor1.hh"

#include "Ravl/DP/SPort.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Stream.hh"

namespace RavlImageN {

  //! userlevel=Develop
  //: Base class for Meteor1 frame grabbers.
  
  class DPIImageBaseMeteor1BodyC
  {
  public:
    DPIImageBaseMeteor1BodyC(const StringC &dev,const type_info &npixType,const ImageRectangleC &nrect);
    //: Constructor.
    
    ~DPIImageBaseMeteor1BodyC();
    //: Destructor.
    
    enum VideoSourceTypeT 
      { VidSrc_PAL = 1, VidSrc_SECAM = 2 ,VidSrc_NTSC = 3 };
    //: Video source types.
    
    enum VideoSourceT 
      { VidSrc_DEV0 = 1, VidSrc_DEV1 = 2, VidSrc_DEV2 = 3, VidSrc_DEV3 = 4,VidSrc_SVIDEO = 5 };
    //: Video source types.
    
    enum VideoParamT {
      VP_HUE = 0,
      VP_CHROMA_GAIN,
      VP_BRIGHTNESS,
      VP_CHROMA_SAT,
      VP_CONTRAST,
      VP_END
    };
    
    const char *ParamName(VideoParamT pr) const;
    //: Get the name of each parameter.
    
    int GetParam(VideoParamT pr) const;
    //: Get parameter value
    
    bool SetParam(VideoParamT pr,int val);
    //: Setup parameter value.
    
    void DumpParam(ostream &out) const;
    //: Dump current settings to 'out'.
    
  protected:
    bool Open(const StringC &dev,const type_info &npixType,const ImageRectangleC &nrect);
    //: Open a meteor device.
    
    bool Close();
    //: Close connection to meteor.
    
  char *NextFrame();
    //: Get next frame from grabber.
    // returns ptr to raw data. (this must be copied from the buffer, before next frame is retrieved.)
    
    ImageRectangleC maxRect;// Maximum size of grabber rectangle.
    ImageRectangleC rect; // Requested size of grabber rectangle.
    
    VideoSourceTypeT sourceType;
  
    int bufFormat; // Format of buffer.
    int bufFrames; // Number of frames in buffer.
    int bufFrameSize; // Buffer frame size in bytes.
    int outFormat; // Conversion to use when copying. 0=Plain copy.
  bool useMemMap; // Use memory mapping ?
    StringC devName;
    int fd;
    int fbfd;
    char *mmbuf; // Memory mapped buffer.
    
    int frameCount;
  };
  
  //! userlevel=Develop
  //: Meteor1 frame grabber.
  
  template<class PixelT>
  class DPIImageMeteor1BodyC
    : public DPIPortBodyC<ImageC<PixelT> >,
      public DPIImageBaseMeteor1BodyC
  {
  public:
    DPIImageMeteor1BodyC(const StringC &dev,const ImageRectangleC &nrect = ImageRectangleC(0,-1,0,-1))
      : DPIImageBaseMeteor1BodyC(dev,typeid(PixelT),nrect)
    {}
    //: Constructor.
    
    virtual ~DPIImageMeteor1BodyC() {}
    //: Destructor

    virtual ImageC<PixelT> Get() {
      char *dat = NextFrame();
      if(dat == 0)
        throw DataNotReadyC("DPIImageMeteor1BodyC::NextFrame(), failed to return image buf. \n");
      ImageC<PixelT> mb(maxRect.Rows(),maxRect.Cols(),(PixelT *)((void *) dat),true);
      if(rect != maxRect)
        return ImageC<PixelT>(mb,rect);
      return mb;
    }
    //: Get next image.
    
    virtual bool Get(ImageC<PixelT> &buff) {
      char *dat = NextFrame();
      if(dat == 0)
        return false;
      ImageC<PixelT> mb(maxRect.Rows(),maxRect.Cols(),(PixelT *)((void *) dat),true);
      if(rect != maxRect)
        buff = ImageC<PixelT>(mb,rect);
      else
        buff = mb;
      return true;
    }
    //: Get next image.
    
    virtual bool IsGetReady() const
    { return fd >= 0; }
    //: Is some data ready ?
    // true = yes.
    // Defaults to !IsGetEOS().
    
    virtual bool IsGetEOS() const
    { return fd < 0; }
    //: Has the End Of Stream been reached ?
    // true = yes.
    
  };
  
  //! userlevel=Normal
  //: Meteor1 frame grabber.
  
  template<class PixelT>
  class DPIImageMeteor1C
    : public DPIPortC<ImageC<PixelT> >
  {
  public:
    DPIImageMeteor1C()
      : DPEntityC(true)
    {}
    //: Default constructor.
    // creates an invalid handle.
    
    DPIImageMeteor1C(const StringC &dev,const ImageRectangleC &nrect = ImageRectangleC(0,-1,0,-1))
      : DPEntityC(*new DPIImageMeteor1BodyC<PixelT>(dev,nrect))
    {}
    //: Constructor.
    
  protected:
    DPIImageMeteor1BodyC<PixelT> &Body()
    { return dynamic_cast<DPIImageMeteor1BodyC<PixelT> &>(DPEntityC::Body()); }
    //: Access body.
    
    const DPIImageMeteor1BodyC<PixelT> &Body() const
    { return dynamic_cast<const DPIImageMeteor1BodyC<PixelT> &>(DPEntityC::Body()); }
    //: Access body.
  public:  
    
    const char *ParamName(DPIImageBaseMeteor1BodyC::VideoParamT pr) const
    { return Body().ParamName(pr); }
    //: Get the name of each parameter.
    
    int GetParam(DPIImageBaseMeteor1BodyC::VideoParamT pr) const
    { return Body().GetParam(pr); }
    //: Get parameter value
    
    bool SetParam(DPIImageBaseMeteor1BodyC::VideoParamT pr,int val)
    { return Body().SetParam(pr,val); }
    //: Setup parameter value.
    
    void DumpParam(ostream &out) const
    { Body().DumpParam(out); }
    //: Dump current settings to 'out'.
    
  };
}
#endif
