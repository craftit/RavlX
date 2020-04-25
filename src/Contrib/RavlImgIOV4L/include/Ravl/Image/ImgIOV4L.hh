// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_VIDIOV4L_HEADER
#define RAVLIMAGE_VIDIOV4L_HEADER 1
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Images.Video.Video IO.Video4Linux"
//! author="Charles Galambos"
//! lib=RavlImgIOV4L
//! file="Ravl/Contrib/V4L/ImgIOV4L.hh"

#include "Ravl/DP/SPort.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteYUV422Value.hh"
#include "Ravl/Image/ByteYUVValue.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Stream.hh"
#include "Ravl/DP/AttributeType.hh"

namespace RavlImageN {

  //! userlevel=Basic
  //: White balance modes
  
  enum V4LWhiteBalanceMode {
    wbIndoor = 0,
    wbOutdoor = 1,
    wbFluorescent = 2,
    wbManual = 3,
    wbAuto = 4
  };

  //! userlevel=Develop
  //: Base class for Meteor1 frame grabbers.
  
  class DPIImageBaseV4LBodyC
  {
  public:
    DPIImageBaseV4LBodyC(const StringC &dev,const type_info &npixType,const ImageRectangleC &nrect,int channel = -1);
    //: Constructor.
    
    DPIImageBaseV4LBodyC(const StringC &dev,const type_info &npixType,bool half,int channel = -1);
    //: Constructor.
    
    ~DPIImageBaseV4LBodyC();
    //: Destructor.
    
  protected:
    bool Open(const StringC &dev,const type_info &npixType,const ImageRectangleC &nrect);
    //: Open a video for linux device.
    
    bool Open1(int nfd,const type_info &npixType,const ImageRectangleC &nrect);
    //: Open a video for linux 1 device.
    
    bool Close();
    //: Close connection to meteor.
    
    bool NextPre();
    //: Pre next frame capture.
    
    bool NextPost();
    //: Post next frame capture.
    
    bool NextFrame(ImageC<ByteYUV422ValueC> &ret);
    //: Get next YUV frame from grabber.
    
    bool NextFrame(ImageC<ByteYUVValueC> &ret);
    //: Get next YUV frame from grabber.
    
    bool NextFrame(ImageC<ByteRGBValueC> &ret);
    //: Get next RGB frame from grabber.

    bool NextFrame(ImageC<ByteT> &ret);
    //: Get next grey level frame from grabber.
    
    bool SetupCamera();
    //: Setup camera specific stuff.
    
    bool SetShutterSpeed(int speed);
    //: Set shutter speed 0..65536 -1=Auto.
    
    bool SetBrightness(int brightness);
    //: Set brightness speed 0..65536 -1=Auto.
    
    bool SetContrast(int contrast);
    //: Set contrast speed 0..65536 -1=Auto.
    
    bool SetGamma(int gamma);
    //: Set gama 0..65536 -1=Auto.
    
    bool SetCompression(int compression);
    //: Set compression speed 0..100 
    
    bool SetAGC(int agcGain);
    //: Set agc 0..65536 -1=off.
    
    bool SetLED(int on);
    //: Set LED, bits 0..15 on , 16..31 off time
    
    bool SetReset();
    //: Perform factory reset
    
    bool SaveUserSettings();
    //: Save camera settings.
    // (Philips webcams.)
    
    bool LoadUserSettings();
    //: Save camera settings.
    // (Philips webcams.)
    
    bool SetWhiteBalance(int mode);
    //: Set white balance mode
    
    bool SetPan(int pan);
    //: Set camera pan.
    
    bool SetTilt(int tilt);
    //: Set camera tilt.
    
    bool GetWhiteBalance(int& mode);
    //: Get white balance mode
    
    bool GetShutterSpeed(int &speed);
    //: Get shutter speed 0..65536 -1=Auto.
    
    bool GetBrightness(int &brightness);
    //: Get brightness speed 0..65536 -1=Auto.
    
    bool GetContrast(int &contrast);
    //: Get contrast speed 0..65536 -1=Auto.

    bool GetCompression(int &compression);
    //: Set compression speed 0..100 -1=default.
    
    bool GetGamma(int &gamma);
    //: Set gama 0..65536 -1=Auto.
    
    bool GetAGC(int &agcGain);
    //: Set agc 0..65536 -1=off.
    
    bool GetPad(int &pad);
    //: Get padding flag.
    
    bool GetPan(int &pan);
    //: Get camera pan.
    
    bool GetTilt(int &tilt);
    //: Get camera tilt.
    
    bool HandleGetAttr(const StringC &attrName,StringC &attrValue);
    //: Handle get attrib.
    
    bool HandleSetAttr(const StringC &attrName,const StringC &attrValue);
    //: Handle Set attrib.
    
    bool HandleGetAttr(const StringC &attrName,IntT &attrValue);
    //: Get a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.
    
    bool HandleSetAttr(const StringC &attrName,const IntT &attrValue);
    //: Set a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.
    
    bool BuildAttributes(AttributeCtrlBodyC &attrCtrl);
    //: Build list of attributes.
    
    enum SourceTypeT { SOURCE_UNKNOWN , SOURCE_USBWEBCAM_PHILIPS };
    
    SourceTypeT sourceType;
    ImageRectangleC maxRect;// Maximum size of grabber rectangle.
    ImageRectangleC rect; // Requested size of grabber rectangle.
    
    int palette; // Current palette mode.    
    int fd;
    ByteT *buffer;
    ByteT *buf_grey;
    ByteT *buf_u;
    ByteT *buf_v;
    int bufLen;
    bool half; // Attempt to get half size images?
    bool memmap; // Use memory mapping.
    SArray1dC<int> frameOffsets;
    int bufNo;
    int channel;
    int shutterSpeed;
  };
  
  //! userlevel=Develop
  //: V4L frame grabber.
  
  template<class PixelT>
  class DPIImageV4LBodyC
    : public DPIPortBodyC<ImageC<PixelT> >,
      public DPIImageBaseV4LBodyC
  {
  public:
    DPIImageV4LBodyC(const StringC &dev,const ImageRectangleC &nrect = ImageRectangleC(0,-1,0,-1),int channel = -1)
      : DPIImageBaseV4LBodyC(dev,typeid(PixelT),nrect,channel)
    { BuildAttributes(*this); }
    //: Constructor.
    
    DPIImageV4LBodyC(const StringC &dev,bool half,int channel = -1)
      : DPIImageBaseV4LBodyC(dev,typeid(PixelT),half,channel)
    { BuildAttributes(*this); }
    //: Constructor.
    
    virtual ImageC<PixelT> Get() {
      ImageC<PixelT> ret;
      NextFrame(ret);
      return ret;
    }
    //: Get next image.
    
    virtual bool Get(ImageC<PixelT> &buff) 
    { return NextFrame(buff); }
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
    
    virtual bool GetAttr(const StringC &attrName,StringC &attrValue) { 
      if(HandleGetAttr(attrName,attrValue))
	return true;
      return DPPortBodyC::GetAttr(attrName,attrValue);
    }
    //: Get a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.
    
    virtual bool SetAttr(const StringC &attrName,const StringC &attrValue){ 
      if(HandleSetAttr(attrName,attrValue))
	return true;
      return DPPortBodyC::SetAttr(attrName,attrValue);
    }
    //: Set a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.
    
    virtual bool GetAttr(const StringC &attrName,IntT &attrValue){ 
      if(HandleGetAttr(attrName,attrValue))
	return true;
      return DPPortBodyC::GetAttr(attrName,attrValue);
    }
    //: Get a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.
    
    virtual bool SetAttr(const StringC &attrName,const IntT &attrValue) { 
      if(HandleSetAttr(attrName,attrValue))
	return true;
      return DPPortBodyC::SetAttr(attrName,attrValue);
    }
    //: Set a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.
  };
  
  //! userlevel=Normal
  //: V4L frame grabber.
  
  template<class PixelT>
  class DPIImageV4LC
    : public DPIPortC<ImageC<PixelT> >
  {
  public:
    DPIImageV4LC()
      : DPEntityC(true)
    {}
    //: Default constructor.
    // creates an invalid handle.
    
    DPIImageV4LC(const StringC &dev,const ImageRectangleC &nrect = ImageRectangleC(0,-1,0,-1),int channel = -1)
      : DPEntityC(*new DPIImageV4LBodyC<PixelT>(dev,nrect,channel))
    {}
    //: Constructor.
    
    DPIImageV4LC(const StringC &dev,bool half,int channel = -1)
      : DPEntityC(*new DPIImageV4LBodyC<PixelT>(dev,half,channel))
    {}
    //: Constructor.
    
  protected:
    DPIImageV4LBodyC<PixelT> &Body()
    { return dynamic_cast<DPIImageV4LBodyC<PixelT> &>(DPEntityC::Body()); }
    //: Access body.
    
    const DPIImageV4LBodyC<PixelT> &Body() const
    { return dynamic_cast<const DPIImageV4LBodyC<PixelT> &>(DPEntityC::Body()); }
    //: Access body.
  public:  
    
  };

}
#endif
