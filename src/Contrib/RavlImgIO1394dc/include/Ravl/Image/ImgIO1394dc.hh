// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_IMGIO1394DC_HEADER
#define RAVLIMAGE_IMGIO1394DC_HEADER 1
//! rcsid="$Id$"
//! lib=RavlImgIO1394dc
//! author="Charles Galambos"
//! docentry="Ravl.API.Images.Video.Video IO.IIDC"
//! file="Ravl/Contrib/dc1394/ImgIO1394dc.hh"

#include "Ravl/DP/SPort.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/DP/AttributeType.hh"
#include "Ravl/Hash.hh"
#include "Ravl/Tuple2.hh"
#include <libdc1394/dc1394_control.h>

#include "Ravl/DP/AttributeValueTypes.hh"
#include "Ravl/StrStream.hh"
#include "Ravl/Image/ByteYUV422Value.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/MTLocks.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {
  
  struct Feature1394dcC;
  
  //! userlevel=Develop
  //: Firewire dc camera control base class
  
  class ImgIO1394dcBaseC
  {
  public:
    ImgIO1394dcBaseC(UIntT channel=0,const std::type_info &pixelType = typeid(void));
    //: Constructor.
    // channel > 100 means DMA access to channel = channel-100

    ~ImgIO1394dcBaseC();
    //: Destructor.

    bool Open(const StringC &dev);
    //: Open camera on device.

    //bool CaptureImage(ImageC<PixelT> &img);
    //: Capture an image.
    
    bool CaptureImage(void *buffer,UIntT pixelSize);
    //: Capture an image.
    
    bool HandleGetAttr(const StringC &attrName, StringC &attrValue);
    //: Get a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.

    bool HandleSetAttr(const StringC &attrName, const StringC &attrValue);
    //: Set a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.

    bool HandleGetAttr(const StringC &attrName, IntT &attrValue);
    //: Get a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.

    bool HandleSetAttr(const StringC &attrName, const IntT &attrValue);
    //: Set a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.

    bool HandleGetAttr(const StringC &attrName, RealT &attrValue);
    //: Get a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.

    bool HandleSetAttr(const StringC &attrName, const RealT &attrValue);
    //: Set a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.

    bool HandleGetAttr(const StringC &attrName, bool &attrValue);
    //: Get a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.

    bool HandleSetAttr(const StringC &attrName, const bool &attrValue);
    //: Set a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.

    RealT SetFrameRate(RealT speed);
    //: Set capture framerate.
    // Returns the actual frame rate.
    
    bool OneShotMode(bool enable);
    //: Set oneshot mode.
    
    bool OneShotMode() const
    { return oneShotMode; }
    //: Is one shot mode enabled ?
    
  protected:
    void BuildAttrList(AttributeCtrlBodyC &attrCtrl);
    //: Build attribute list.
    
    const Feature1394dcC *FindFeature(int featureNo);
    //: Find information about a feature.
    
    enum ControlTypeT { CT_IntValue, CT_FloatValue, CT_OnOff,CT_Auto };
    
    HashC<StringC,Tuple2C<IntT,ControlTypeT> > name2featureid;

    raw1394handle_t raw1394handle;
    dc1394_cameracapture camera;
    //    nodeid_t cameraNode;
    
    StringC camera_vendor;
    StringC camera_model;
    StringC camera_euid;
    
    int cam_channel;
    int cam_format;
    int cam_mode;
    int cam_speed;
    int cam_framerate;
    quadlet_t available_framerates;
    
    bool oneShotMode;
  private:
  };

  struct FrameRate1394dcC
  {
    RealT speed;
    unsigned int value;
  };

  extern const FrameRate1394dcC frameRates[];

  //! userlevel=Develop
  //: Firewire dc camera grabbing body
  template<typename PixelT>
  class DPIImage1394dcBodyC
    : public DPIPortBodyC<ImageC<PixelT> >,
      public ImgIO1394dcBaseC
  {
  public:
    DPIImage1394dcBodyC(const StringC &dev, UIntT channel=0)
      : ImgIO1394dcBaseC(channel,typeid(PixelT))
    {
      Open(dev);
      BuildAttrList(*this);
    }
    //: Constructor.

    virtual bool IsGetReady() const
    { return raw1394handle != 0; }
    //: Is some data ready ?
    // true = yes.
    // Defaults to !IsGetEOS().

    virtual bool IsGetEOS() const
    { return !IsGetReady(); }
    //: Has the End Of Stream been reached ?
    // true = yes.
    
    virtual bool Get(ImageC<PixelT> &buff) { 
      buff = ImageC<PixelT>(camera.frame_height, camera.frame_width);
      return CaptureImage((void *) &(buff[0][0]),sizeof(PixelT)); 
    }
    //: Get next image.
    
    virtual ImageC<PixelT> Get() {
      ImageC<PixelT> img(camera.frame_height, camera.frame_width);
      if(!CaptureImage((void *) &(img[0][0]),sizeof(PixelT)))
        throw DataNotReadyC("Failed to capture image. ");
      return img;
    }
    //: Get next image.

    virtual bool GetAttr(const StringC &attrName,StringC &attrValue){
      if(HandleGetAttr(attrName,attrValue))
        return true;
      return DPPortBodyC::GetAttr(attrName,attrValue);
    }
    //: Get a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.

    virtual bool SetAttr(const StringC &attrName,const StringC &attrValue) {
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

    virtual bool GetAttr(const StringC &attrName,RealT &attrValue){
      if(HandleGetAttr(attrName,attrValue))
        return true;
      return DPPortBodyC::GetAttr(attrName,attrValue);
    }
    //: Get a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.

    virtual bool SetAttr(const StringC &attrName,const RealT &attrValue) {
      if(HandleSetAttr(attrName,attrValue))
        return true;
      return DPPortBodyC::SetAttr(attrName,attrValue);
    }
    //: Set a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.

    virtual bool GetAttr(const StringC &attrName,bool &attrValue){
      if(HandleGetAttr(attrName,attrValue))
        return true;
      return AttributeCtrlBodyC::GetAttr(attrName,attrValue);
    }
    //: Get a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.

    virtual bool SetAttr(const StringC &attrName,const bool &attrValue) {
      if(HandleSetAttr(attrName,attrValue))
        return true;
      return AttributeCtrlBodyC::SetAttr(attrName,attrValue);
    }
    //: Set a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.

  protected:
  };

  //! userlevel=Develop
  //: Firewire dc camera grabbing

  template<class PixelT>
  class DPIImage1394dcC
    : public DPIPortC<ImageC<PixelT> >
  {
  public:
    DPIImage1394dcC()
      : DPEntityC(true)
    {}
    //: Default constructor.

    DPIImage1394dcC(const StringC &str, UIntT channel=0)
      : DPEntityC(*new DPIImage1394dcBodyC<PixelT>(str, channel))
    {}
  };



}

#endif
