// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, OmniPerception Ltd.
// This code may be redistributed under the terms of the MIT
// License. See http://www.opensource.org/licenses/mit-license.html
// file-header-ends-here
#ifndef RAVL_IMGIOUEYE_HEADER
#define RAVL_IMGIOUEYE_HEADER 1
//! author = "Charles Galambos"
//! lib=RavlImgIOuEye

#include <ueye.h>
#include <ueye_deprecated.h>
#include "Ravl/DP/Port.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Threads/Mutex.hh"

namespace RavlImageN {
  
  //! userlevel=Develop
  //: Base class for running uEye camera's
  
  class ImgIOuEyeBaseC {
  public:
    ImgIOuEyeBaseC(const std::type_info &pixelType,UIntT cameraId = 0);    
    //: Constructor.
    
    virtual ~ImgIOuEyeBaseC();
    //: Destructor.
    
    bool Open(UIntT cameraId);
    //: Open camera.
    
    bool SetPixelType(const std::type_info &pixelType);
    //: Set pixel type
    
    bool CaptureImage(char *buffer);
    //: Capture a single image into the given buffer.
    
    bool HandleGetAttr(const StringC &attrName, StringC &attrValue);
    //: Handle get attribute (string)
    // Returns false if the attribute name is unknown.
    
    bool HandleSetAttr(const StringC &attrName, const StringC &attrValue);
    //: Handle set attribute (string)
    // Returns false if the attribute name is unknown.
    
    bool HandleGetAttr(const StringC &attrName, IntT &attrValue);
    //: Handle get attribute (int)
    // Returns false if the attribute name is unknown.
    
    bool HandleSetAttr(const StringC &attrName, const IntT &attrValue);
    //: Handle set attribute (int)
    // Returns false if the attribute name is unknown.
    
    bool HandleGetAttr(const StringC &attrName, bool &attrValue);
    //: Handle get attribute (bool)
    // Returns false if the attribute name is unknown.
    
    bool HandleSetAttr(const StringC &attrName, const bool &attrValue);
    //: Handle set attribute (bool)
    // Returns false if the attribute name is unknown.
    
    bool HandleGetAttr(const StringC &attrName, RealT &attrValue);
    //: Handle get attribute (RealT)
    // Returns false if the attribute name is unknown.
    
    bool HandleSetAttr(const StringC &attrName, const RealT &attrValue);
    //: Handle set attribute (RealT)
    // Returns false if the attribute name is unknown.
    
    bool BuildAttributes(AttributeCtrlBodyC &attrCtrl);
    //: Build list of attributes.

  protected:
    bool AllocateImages();
    //: Allocate image buffers.
    // Should be called with camera lock aquired
    
    SizeT ImageBufferSize() const
    { return m_sensorInfo.nMaxWidth * m_sensorInfo.nMaxHeight * (m_bitsPerPixel / 8); }
    //: Get size of image in bytes.
    
    void ResetImageSize();
    //: Reset image size
    // Should be called with camera lock aquired
    
    void UpdateBuffers();
    //: Update buffer state.
    // Should be called with camera lock aquired
    
    virtual bool SignalAttrChange(const char *name) = 0;
    //: Signal an attribute has been changed.
    
    enum uEyeTrigT { TRIG_OFF,TRIG_HILO,TRIG_LOHI,TRIG_SOFT } m_triggerMode;
    
    enum uEyeRotationT { ROT_0, ROT_90, ROT_180, ROT_270 } m_rotation;

    enum uEyeStateT { UE_NotReady, UE_Ready, UE_Running, UE_TriggerWait } m_state;
    
    bool m_snapshot; // Are we in snapshot mode.

    HIDS m_phf;              // Handle to camera.
    SENSORINFO m_sensorInfo; // Information about current camera sensor.
    
    IndexRange2dC m_captureSize;
    UIntT m_bitsPerPixel;
    float m_timeOutDelay;
    
    // Buffer's for live video feed.
    static const int m_NumBuffers = 4;
    char *m_buffers[m_NumBuffers];
    int m_imgId[m_NumBuffers];
    
    RavlN::MutexC m_accessMutex;
  };
  

  //! userlevel=Develop
  //: Body for uEye
  
  template <typename PixelT>
  class ImgIOuEyeBodyC 
    : public RavlN::DPIPortBodyC< ImageC<PixelT> >,
      public ImgIOuEyeBaseC
  {
  public:
    ImgIOuEyeBodyC(UIntT cameraId = 0)
      : ImgIOuEyeBaseC(typeid(PixelT),cameraId)
    {
      BuildAttributes(*this);
    }
    //: Constructor
    
    virtual ImageC<PixelT> Get() {
      ImageC<PixelT> img;
      if (!Get(img))
        throw DataNotReadyC("Failed to get next frame.");
      return img;
    }
    //: Get next frame.
    
    virtual bool Get(ImageC<PixelT> &img) { 
      // Check the device is open
      if (m_state == UE_NotReady)
        return false;
      if(m_rotation == ROT_0 || m_rotation == ROT_180) {
        if(img.Frame() != m_captureSize)
          img = ImageC<PixelT>(m_captureSize);
      } else {
        IndexRange2dC captureSize(0, m_sensorInfo.nMaxWidth - 1,
                                  0, m_sensorInfo.nMaxHeight - 1);
        if(img.Frame() != captureSize)
          img = ImageC<PixelT>(captureSize);
      }
      return CaptureImage(reinterpret_cast<char *>(&(img[img.Frame().Origin()])));
    }
    //: Get next image.
    
    virtual bool IsGetReady() const 
    { return (m_state != UE_NotReady); }
    //: Are we ready to aquire images ?
    
    virtual bool IsGetEOS() const 
    { return (m_state == UE_NotReady); }
    //: Has the end of stream been reached ?
    
    virtual bool GetAttr(const StringC &attrName, StringC &attrValue) {
      if (HandleGetAttr(attrName, attrValue))
        return true;
      return DPPortBodyC::GetAttr(attrName, attrValue);
    }
    //: Handle get attribute (string)
    // Returns false if the attribute name is unknown.
    
    virtual bool SetAttr(const StringC &attrName, const StringC &attrValue) {
      if (HandleSetAttr(attrName, attrValue))
        return true;
      return DPPortBodyC::SetAttr(attrName, attrValue);
    }
    //: Handle set attribute (string)
    // Returns false if the attribute name is unknown.
    
    virtual bool GetAttr(const StringC &attrName, IntT &attrValue) {
      if (HandleGetAttr(attrName, attrValue))
        return true;
      return DPPortBodyC::GetAttr(attrName, attrValue);
    }
    //: Handle get attribute (int)
    // Returns false if the attribute name is unknown.
    
    virtual bool SetAttr(const StringC &attrName, const IntT &attrValue) {
      if (HandleSetAttr(attrName, attrValue))
        return true;
      return DPPortBodyC::SetAttr(attrName, attrValue);
    }
    //: Handle set attribute (int)
    // Returns false if the attribute name is unknown.

    virtual bool GetAttr(const StringC &attrName, bool &attrValue) {
      if (HandleGetAttr(attrName, attrValue))
        return true;
      return DPPortBodyC::GetAttr(attrName, attrValue);
    }
    //: Handle get attribute (bool)
    // Returns false if the attribute name is unknown.
    
    virtual bool SetAttr(const StringC &attrName, const bool &attrValue) {
      if (HandleSetAttr(attrName, attrValue))
        return true;
      return DPPortBodyC::SetAttr(attrName, attrValue);
    }
    //: Handle set attribute (bool)
    // Returns false if the attribute name is unknown.

    virtual bool GetAttr(const StringC &attrName,RealT &attrValue) {
      if (HandleGetAttr(attrName, attrValue))
        return true;
      return DPPortBodyC::GetAttr(attrName, attrValue);
    }
    //: Get a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    virtual bool SetAttr(const StringC &attrName,const RealT &attrValue) {
      if (HandleSetAttr(attrName, attrValue))
        return true;
      return DPPortBodyC::SetAttr(attrName, attrValue);            
    }
    //: Set a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.

  protected:
    virtual bool SignalAttrChange(const char *name)
    { return this->SignalChange(StringC(name)); }
    //: Signal an attribute has been changed.
    
  };

  //! userlevel=Normal
  //: Handle for connection to uEye camera's
  
  template <class PixelT>
  class ImgIOuEyeC 
    : public RavlN::DPIPortC< ImageC<PixelT> > 
  {
  public:
    ImgIOuEyeC()
      :  RavlN::DPEntityC(true)
    {}
    //: Default constructor.
    
    ImgIOuEyeC(UIntT cameraId)
      : RavlN::DPEntityC(*new ImgIOuEyeBodyC<PixelT>(cameraId))
    {}
    //: Constructor
    
  protected:
    
  };
 
}



#endif
