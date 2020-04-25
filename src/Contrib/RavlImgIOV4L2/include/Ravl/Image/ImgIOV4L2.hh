// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/V4L2/ImgIOV4L2.hh"
//! lib=RavlImgIOV4L2
//! author = "Warren Moore"
//! docentry = "Ravl.API.Images.Video.Video IO.Video4Linux2"
//! example = "exV4L2.cc"

#ifndef RAVL_IMGIOV4L2_HEADER
#define RAVL_IMGIOV4L2_HEADER 1

#include "Ravl/DP/Port.hh"
#include "Ravl/DP/SPort.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Image/ByteYUV422Value.hh"
#include "Ravl/Threads/Mutex.hh"
#include "Ravl/HSet.hh"

#include <unistd.h>
#include <linux/videodev2.h>

namespace RavlImageN
{
  using namespace RavlN;
  
  template <class PixelT> class ImgIOV4L2C;
  //: Predeclare class handle
  
  /////////////////////////////
  //! userlevel = Develop
  //: V4L2 un-templated body
  
  class ImgIOV4L2BaseC
  {
  public:
    ImgIOV4L2BaseC(const StringC &device, const UIntT input, const type_info &pixelType);
    //: Constructor

    ImgIOV4L2BaseC(const StringC &device, const UIntT input);
    //: Constructor

    ~ImgIOV4L2BaseC();
    //: Destructor
    
    bool CheckFormat(const type_info &pixelType);
    //: Check if the pixel type is supported

    bool GetFrame(ImageC<ByteRGBValueC> &img, ImgIOV4L2C<ByteRGBValueC> parent);
    //: Get next image.

    bool GetFrame(ImageC<ByteT> &img, ImgIOV4L2C<ByteT> parent);
    //: Get next image.

    bool GetFrame(ImageC<ByteYUV422ValueC> &img, ImgIOV4L2C<ByteYUV422ValueC> parent);
    //: Get next image.

    void ReleaseBuffer(const UIntT id, const UIntT index);
    //: Requeue a capture buffer.
    // Should not be called directly
    
    UIntT LastFrameNum() const
    { return m_seqNum; }
    //: Get the last frame sequence number
    
    StreamPosT LastFrameSeq() const
    { return m_seqNum; }
    //: Get the last frame sequence number
    
    bool IsOpen() const
    { return (m_fd != -1); }
    //: Is the device open?
    
    bool IsConfigured() const
    { return (m_bufferCount > 0); }
    //: Is the device configured and ready for capture?
    
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
    
    bool BuildAttributes(AttributeCtrlBodyC &attrCtrl);
    //: Build list of attributes.
    
  protected:
    bool Open(const StringC &device, const UIntT input);
    //: Open the device.
    // Does not configure the capture buffers
    
    void Close();
    //: Close the device.
    // Does not release the capture buffers
    
    bool CheckInput();
    //: Check the input
    
    bool CheckSize();
    //: Test the capture size limits
    
    bool ConfigureCapture();
    //: Configure the capture device, allocating capture buffers
    
    void ReleaseCapture();
    //: Release the capture device, deallocating capture buffers
    
    bool CaptureBuffer(v4l2_buffer &buffer);
    //: Request a queued capture buffer
    
  protected:
    typedef struct
    {
      UIntT m_id;
      void *m_start;
      size_t m_length;
    } TBuf;
    //: Buffer mmap data
    
  protected:
  /* Device identification */
    const type_info &m_pixelType;       // Desired image type
    StringC m_device;                   // Device name
    StringC m_deviceDriver;             // Device driver name
    StringC m_deviceCard;               // Device card name
    StringC m_deviceBus;                // Device bus name
    UIntT m_input;                      // Input number
    UIntT m_inputMax;                   // Input max
    int m_fd;                           // File descriptor
    MutexC m_lockCapture;               // Capture device lock

  /* Capture parameters */
    UIntT m_width, m_height;            // Captured size
    UIntT m_widthMax, m_heightMax;      // Max capture size
    UIntT m_widthMin, m_heightMin;      // Min capture size
    UIntT m_pixelFormat;                // Stored pixel format
    UIntT m_fieldFormat;                // Captured field selection
    UIntT m_standard;                   // Standard in use
    bool m_fastBufferAvailable;         // Are fast buffers available?
    bool m_fastBufferUsed;              // Are fast buffers in use?

  /* Buffer parameters */
    UIntT m_bufferMax;                  // Number of capture buffers
    UIntT m_bufferCount;                // Number of capture buffers
    TBuf *m_buffers;                    // Array of mmap'd buffers
    UIntT m_bufferIdCount;              // Buffer id counter
    HSetC<UIntT> m_bufferOut;           // Set of valid dequeued buffers
    
  /* Frame attributes */
    StreamPosT m_seqNum;                // Last sequence number
  };
  
  
  
  /////////////////////////////
  //! userlevel = Develop
  //: V4L2 templated body
  
  template <class PixelT>
  class ImgIOV4L2BodyC :
    public DPIPortBodyC< ImageC<PixelT> >,
    public ImgIOV4L2BaseC
  {
  public:
    ImgIOV4L2BodyC(const StringC &device, const UIntT input) :
      ImgIOV4L2BaseC(device, input, typeid(ImageC<PixelT>))
    {
      BuildAttributes(*this);
    }
    //: Constructor.
    
    virtual ~ImgIOV4L2BodyC()
    {
    }
    //: Destructor.
    
    virtual ImageC<PixelT> Get()
    {
      ImageC<PixelT> img;
      if (!Get(img))
        throw DataNotReadyC("Failed to get next frame.");
      return img;
    }
    //: Get next frame.
    
    virtual bool Get(ImageC<PixelT> &img)
    { 
      // Check the device is open
      if (!IsOpen())
        return false;
      
      return GetFrame(img, ImgIOV4L2C<PixelT>(*this));
    }
    //: Get next image.
    
    virtual UIntT Tell() const
    { return LastFrameSeq(); }
    //: Find current location in stream.
    
    virtual StreamPosT Tell64() const
    { return LastFrameSeq(); }
    //: Find current location in stream.
    
    virtual bool IsGetReady() const
    {
      return IsOpen();
    }
    
    virtual bool IsGetEOS() const
    {
      return !IsOpen();
    }
    //: Is it the EOS

    virtual bool GetAttr(const StringC &attrName, StringC &attrValue)
    {
      if (HandleGetAttr(attrName, attrValue))
        return true;
      return DPPortBodyC::GetAttr(attrName, attrValue);
    }
    //: Handle get attribute (string)
    // Returns false if the attribute name is unknown.
    
    virtual bool SetAttr(const StringC &attrName, const StringC &attrValue)
    {
      if (HandleSetAttr(attrName, attrValue))
        return true;
      return DPPortBodyC::SetAttr(attrName, attrValue);
    }
    //: Handle set attribute (string)
    // Returns false if the attribute name is unknown.
    
    virtual bool GetAttr(const StringC &attrName, IntT &attrValue)
    {
      if (HandleGetAttr(attrName, attrValue))
        return true;
      return DPPortBodyC::GetAttr(attrName, attrValue);
    }
    //: Handle get attribute (int)
    // Returns false if the attribute name is unknown.
    
    virtual bool SetAttr(const StringC &attrName, const IntT &attrValue)
    {
      if (HandleSetAttr(attrName, attrValue))
        return true;
      return DPPortBodyC::SetAttr(attrName, attrValue);
    }
    //: Handle set attribute (int)
    // Returns false if the attribute name is unknown.

    virtual bool GetAttr(const StringC &attrName, bool &attrValue)
    {
      if (HandleGetAttr(attrName, attrValue))
        return true;
      return DPPortBodyC::GetAttr(attrName, attrValue);
    }
    //: Handle get attribute (bool)
    // Returns false if the attribute name is unknown.
    
    virtual bool SetAttr(const StringC &attrName, const bool &attrValue)
    {
      if (HandleSetAttr(attrName, attrValue))
        return true;
      return DPPortBodyC::SetAttr(attrName, attrValue);
    }
    //: Handle set attribute (bool)
    // Returns false if the attribute name is unknown.
  };

  
  
  /////////////////////////////
  //! userlevel = Advanced
  //: V4L2 format
  // BIG OBJECT
  
  template <class PixelT>
  class ImgIOV4L2C :
    public DPIPortC< ImageC<PixelT> >
  {
  public:
    ImgIOV4L2C() :
      DPEntityC(true)
    {}
    //: Default constructor.
    // Creates an invalid handle.

    ImgIOV4L2C(const StringC &device, const UIntT input) :
      DPEntityC(*new ImgIOV4L2BodyC<PixelT>(device, input))
    {}
    //: Constructor.

    explicit ImgIOV4L2C(const DPIPortC< ImageC<PixelT> > &port) :
      DPEntityC(port)
    { if (dynamic_cast<ImgIOV4L2BodyC<PixelT>*>(&DPIPortC< ImageC<PixelT> >::Body()) == 0) this->Invalidate(); }
    //: Construct by upcasting from a pre-created port
    
    explicit ImgIOV4L2C(ImgIOV4L2BodyC<PixelT> &body) :
      DPEntityC(body)
    {}
    //: Body constructor.

    void ReleaseBuffer(const UIntT id, const UIntT index)
    { Body().ReleaseBuffer(id, index); }
    //: Requeue a capture buffer.
    // Should not be called directly
    
  protected:
    ImgIOV4L2BodyC<PixelT> &Body()
    { return static_cast<ImgIOV4L2BodyC<PixelT> &>(DPIPortC< ImageC<PixelT> >::Body()); }
    //: Access body.

    const ImgIOV4L2BodyC<PixelT> &Body() const
    { return static_cast<const ImgIOV4L2BodyC<PixelT> &>(DPIPortC< ImageC<PixelT> >::Body()); }
    //: Access body.
  };
}

#endif
