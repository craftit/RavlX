// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/V4L2/ImgIOV4L2.cc"
//! lib=RavlImgIOV4L2
//! author = "Warren Moore"

#include "Ravl/Image/ImgIOV4L2.hh"
#include "Ravl/DP/AttributeValueTypes.hh"
#include "Ravl/Image/V4L2Buffer.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/DList.hh"
#include "Ravl/SysLog.hh"
#include "Ravl/TypeName.hh"

#include <string.h>
#include <linux/types.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/time.h>

#define DODEBUG 0

#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

#define USE_V4L2 1

#define CHAR_STREAM_FROM_4CC(n) \
     (char)((n      ) & 0xff) \
  << (char)((n >>  8) & 0xff) \
  << (char)((n >> 16) & 0xff) \
  << (char)((n >> 24) & 0xff)

namespace RavlImageN
{
  
  struct SupportedFormatT
  {
    const type_info &m_objectType;          //: Ref to type id
    const UIntT m_pixelFormat;              //: 4CC of required capture mode
    const bool m_fastBuffer;                //: Fast buffers available

    SupportedFormatT(const type_info &objectType, const UIntT pixelFormat, const bool fastBuffer) :
      m_objectType(objectType),
      m_pixelFormat(pixelFormat),
      m_fastBuffer(fastBuffer)
    {}
  };
  //: Structure used to store supported pixel formats



  const static SupportedFormatT g_supportedFormat[] =
  {
    SupportedFormatT(typeid(ImageC<ByteT           >), v4l2_fourcc('G', 'R', 'E', 'Y'),  true),
    SupportedFormatT(typeid(ImageC<ByteT           >), v4l2_fourcc('Y', 'U', '1', '2'), false),
    SupportedFormatT(typeid(ImageC<ByteRGBValueC   >), v4l2_fourcc('B', 'G', 'R', '4'), false),
    SupportedFormatT(typeid(ImageC<ByteYUV422ValueC>), v4l2_fourcc('Y', 'U', 'Y', 'V'), false),
  };
  const static UIntT g_supportedFormats = sizeof(g_supportedFormat) / sizeof(SupportedFormatT);
  //: Mapping from image type id required V4L2 capture mode.
  // Note: Only supports a 1:1 mapping at the moment, this should be changed to support more hardware



  struct SupportedStandardT
  {
    const StringC m_name;                   //: Name
    const v4l2_std_id m_id;                 //: V4L2 id

    SupportedStandardT(const StringC &name, const v4l2_std_id id) :
      m_name(name), m_id(id) {}
  };
  //: Structure used to store supported standards



  const static SupportedStandardT g_supportedStandard[] =
  {
    SupportedStandardT("PAL",   V4L2_STD_PAL),
    SupportedStandardT("NTSC",  V4L2_STD_NTSC),
    SupportedStandardT("SECAM", V4L2_STD_SECAM)
  };
  const static UIntT g_supportedStandards = sizeof(g_supportedStandard) / sizeof(SupportedStandardT);
  //: Mapping from name to V4L2 format id.



  struct SupportedFieldT
  {
    const StringC m_name;                   //: Name
    const v4l2_field m_field;               //: V4L2 field id

    SupportedFieldT(const StringC &name, const v4l2_field field) :
      m_name(name), m_field(field) {}
  };
  //: Structure used to store supported standards



  const static SupportedFieldT g_supportedField[] =
  {
    SupportedFieldT("ANY",        V4L2_FIELD_ANY),
    SupportedFieldT("NONE",       V4L2_FIELD_NONE),
    SupportedFieldT("INTERLACED", V4L2_FIELD_INTERLACED),
    SupportedFieldT("TOP",        V4L2_FIELD_TOP),
    SupportedFieldT("BOTTOM",     V4L2_FIELD_BOTTOM)
  };
  const static UIntT g_supportedFields = sizeof(g_supportedField) / sizeof(SupportedFieldT);
  //: Mapping from name to V4L2 format id.



  const static UIntT g_defaultWidth = 720; //: Default capture width (usually overridden by getting with initial format)
  const static UIntT g_defaultHeight = 576; //: Default capture height (usually overridden by getting with initial format)

  const static UIntT g_defaultWidthMaximum = 1024; //: Test capability to this maximum width
  const static UIntT g_defaultHeightMaximum = 768; //: Test capability to this maximum height
  const static UIntT g_defaultWidthMinimum = 8; //: Test capability to this minimum width
  const static UIntT g_defaultHeightMinimum = 8; //: Test capability to this minimum height
  
  const static UIntT g_defaultBuffers = 3;
  //: Default number of capture buffers
  

  ImgIOV4L2BaseC::ImgIOV4L2BaseC(const StringC &device, const UIntT input)
    : m_pixelType(typeid(void)),
      m_device(device),
      m_input(input),
      m_inputMax(0),
      m_fd(-1),
      m_width(g_defaultWidth),
      m_height(g_defaultHeight),
      m_widthMax(g_defaultWidthMaximum),
      m_heightMax(g_defaultHeightMaximum),
      m_widthMin(g_defaultWidthMinimum),
      m_heightMin(g_defaultHeightMinimum),
      m_fieldFormat(0),
      m_standard(0),
      m_fastBufferUsed(false),
      m_bufferMax(g_defaultBuffers),
      m_bufferCount(0),
      m_buffers(NULL),
      m_bufferIdCount(0),
      m_seqNum(-1)
  {
    // Lock the capture device
    MutexLockC lockCapture(m_lockCapture);

    // Open the device
    Open(device, input);
  }


  ImgIOV4L2BaseC::ImgIOV4L2BaseC(const StringC &device, const UIntT input, const type_info &pixelType)
  : m_pixelType(pixelType),
    m_device(device),
    m_input(input),
    m_inputMax(0),
    m_fd(-1),
    m_width(g_defaultWidth),
    m_height(g_defaultHeight),
    m_widthMax(g_defaultWidthMaximum),
    m_heightMax(g_defaultHeightMaximum),
    m_widthMin(g_defaultWidthMinimum),
    m_heightMin(g_defaultHeightMinimum),
    m_fieldFormat(0),
    m_standard(0),
    m_fastBufferUsed(false),
    m_bufferMax(g_defaultBuffers),
    m_bufferCount(0),
    m_buffers(NULL),
    m_bufferIdCount(0),
    m_seqNum(-1)
  {
    // Lock the capture device
    MutexLockC lockCapture(m_lockCapture);

    // Open the device
    Open(device, input);
    
    // Check the format
    if (IsOpen())
    {
      // Check for a supported format
      if (CheckFormat(pixelType))
      {
        // Do final initialisation...
      }
      else
      {
        // Failed to find supported format
        RavlError("Can't open video device, unsupported image format.");
        Close();
      }
    }
  }
  
  
  
  ImgIOV4L2BaseC::~ImgIOV4L2BaseC()
  {
    // Lock the capture device
    MutexLockC lockCapture(m_lockCapture);

    // Release, if configured
    if (IsConfigured())
      ReleaseCapture();
    
    // Close, if open
    if (IsOpen())
      Close();
  }
  
  
  
  bool ImgIOV4L2BaseC::GetFrame(ImageC<ByteRGBValueC> &img, ImgIOV4L2C<ByteRGBValueC> parent)
  {
    // Lock the capture device
    MutexLockC lockCapture(m_lockCapture);

    // Get a capture buffer
    v4l2_buffer buffer;
    if (!CaptureBuffer(buffer))
      return false;
    
    // Create the image
    img = ImageC<ByteRGBValueC>(m_height, m_width);
    ByteT *iData = (ByteT*)m_buffers[buffer.index].m_start;
    for(Array2dIterC<ByteRGBValueC> it(img); it; it++)
    {
      it.Data() = ByteRGBValueC(iData[2], iData[1], iData[0]);
      iData += 4;
    }
    
    // Unlock
    lockCapture.Unlock();

    // Manually release the buffer
    parent.ReleaseBuffer(m_buffers[buffer.index].m_id, buffer.index);
    
    return true;
  }



  bool ImgIOV4L2BaseC::GetFrame(ImageC<ByteT> &img, ImgIOV4L2C<ByteT> parent)
  {
    // Lock the capture device
    MutexLockC lockCapture(m_lockCapture);

    // Get a capture buffer
    v4l2_buffer buffer;
    if (!CaptureBuffer(buffer))
      return false;
    
    // Create the fast buffer image
    RavlAssertMsg(buffer.memory == V4L2_MEMORY_MMAP, "ImgIOV4L2BaseC::GetFrame<ByteT> buffer not mmap-ed");
    img = ImageC<ByteT>(m_height, m_width,
                        V4L2BufferC<ByteT>(parent, m_buffers[buffer.index].m_id, buffer.index, (ByteT*)m_buffers[buffer.index].m_start, (UIntT)m_buffers[buffer.index].m_length));
    
    // Unlock
    lockCapture.Unlock();

    // Copy the image if fast buffers are created yet not used
    if (!m_fastBufferUsed)
    {
      img = ImageC<ByteT>(img.Copy());
    }
    
    return true;
  }



  bool ImgIOV4L2BaseC::GetFrame(ImageC<ByteYUV422ValueC> &img, ImgIOV4L2C<ByteYUV422ValueC> parent)
  {
    // Lock the capture device
    MutexLockC lockCapture(m_lockCapture);

    // Get a capture buffer
    v4l2_buffer buffer;
    if (!CaptureBuffer(buffer))
      return false;
    
    // Create the image
    img = ImageC<ByteYUV422ValueC>(m_height, m_width);
    ByteT *iData = (ByteT*)m_buffers[buffer.index].m_start;
    for(Array2dIterC<ByteYUV422ValueC> it(img); it; it++)
    {
      it.Data() = ByteYUV422ValueC(iData[1], iData[0]); // Fast buffers can't currently be used as
                                                        // ByteYUV422ValueC is actually UYVY, not YUYV
      iData += 2;
    }
    
    // Unlock
    lockCapture.Unlock();

    // Manually release the buffer
    parent.ReleaseBuffer(m_buffers[buffer.index].m_id, buffer.index);
    
    return true;
  }



  void ImgIOV4L2BaseC::ReleaseBuffer(const UIntT id, const UIntT index)
  {
    // If not open and configured, then don't bother doing anything
    if (IsOpen() && IsConfigured())
    {
      // Lock the capture device
      MutexLockC lockCapture(m_lockCapture);
  
      // Check the buffer is in the active buffer out set
      if (m_bufferOut.Contains(id))
      {
        RavlAssertMsg(m_buffers[index].m_id == id, "ImgIOV4L2BaseC::ReleaseBuffer released and stored id mismatch");
        
        // Query the buffer
        v4l2_buffer buffer;
        memset(&buffer,0,sizeof(v4l2_buffer));
        buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buffer.memory = V4L2_MEMORY_MMAP;
        buffer.index = index;
        if (ioctl(m_fd, VIDIOC_QUERYBUF, &buffer) == -1)
        {
          cerr << "ImgIOV4L2BaseC::ReleaseBuffer ioctl(VIDIOC_QUERYBUF) failed id(" << id << ") index(" << index << ")" << endl;
          return;
        }
      
        // Re-queue the buffer
        if (ioctl(m_fd, VIDIOC_QBUF, &buffer) == -1)
        {
          cerr << "ImgIOV4L2BaseC::ReleaseBuffer ioctl(VIDIOC_QBUF) failed id(" << id << ") index(" << index << ")" << endl;
        }
        else
        {
//          ONDEBUG(cerr << "ImgIOV4L2BaseC::ReleaseBuffer ioctl(VIDIOC_QBUF) requeued id(" << id << ") index(" << index << ")" << endl;)
          
          // Remove the id from the buffer out set
          m_bufferOut.Remove(id);
        }
      }
      else
      {
        ONDEBUG(cerr << "ImgIOV4L2BaseC::ReleaseBuffer ignoring id(" << id << ")" << endl;)
      }
    }
  }
  
  
  
  bool ImgIOV4L2BaseC::Open(const StringC &device, const UIntT input)
  {
    RavlAssertMsg(!IsOpen(), "ImgIOV4L2BaseC::Open called on open device");
    
    // Open the device
    m_fd = open(device, O_RDWR);
    
    // Reset the params
    m_width = g_defaultWidth;
    m_height = g_defaultHeight;
    m_seqNum = -1;
    
    return (m_fd != -1);
  }
  
  
  
  void ImgIOV4L2BaseC::Close()
  {
    RavlAssertMsg(!IsConfigured(), "ImgIOV4L2BaseC::Close called on closed device");
    RavlAssertMsg(IsOpen(), "ImgIOV4L2BaseC::Close called on closed device");
    
    // All done, close the device
    close(m_fd);
  }
  
  
  
  bool ImgIOV4L2BaseC::CheckFormat(const type_info &pixelType)
  {
    RavlAssertMsg(IsOpen(), "ImgIOV4L2BaseC::Open called without open device");
    ONDEBUG(RavlDebug("Checking pixel type %s ",RavlN::TypeName(pixelType)));
    // Is capture supported?
    v4l2_capability cap;
    if (ioctl(m_fd, VIDIOC_QUERYCAP, &cap) != -1)
    {
      ONDEBUG( \
        cerr << "ImgIOV4L2BaseC::CheckFormat device(" << m_device << ")" << endl; \
        cerr << "  ioctl(VIDIOC_QUERYCAP) driver (" << cap.driver << ")" << endl; \
        cerr << "  ioctl(VIDIOC_QUERYCAP) card (" << cap.card << ")" << endl; \
        cerr << "  ioctl(VIDIOC_QUERYCAP) bus (" << cap.bus_info << ")" << endl; \
      )
      m_deviceDriver = reinterpret_cast<const char*>(cap.driver);
      m_deviceCard   = reinterpret_cast<const char*>(cap.card);
      m_deviceBus    = reinterpret_cast<const char*>(cap.bus_info);
    }
    else
    {
      RavlError("ioctl(VIDIOC_QUERYCAP) device(%s) capture not supported", m_device.data());
      return false;
    }
    
    // Check for a valid input
    if (!CheckInput())
    {
      RavlError(" input(%u) not supported", m_input);
      return false;
    }

    // Get the current settings format
    v4l2_format fmt;
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    v4l2_pix_format *pfmt = (v4l2_pix_format*)&(fmt.fmt);
    if (ioctl(m_fd, VIDIOC_G_FMT, &fmt) != -1)
    {
      ONDEBUG( \
        cerr << "ImgIOV4L2BaseC::CheckFormat ioctl(VIDIOC_G_FMT)" << endl; \
        cerr << "  width(" << pfmt->width << ")" << endl; \
        cerr << "  height(" << pfmt->height << ")" << endl; \
        cerr << "  4cc(" << CHAR_STREAM_FROM_4CC(pfmt->pixelformat) << ")" << endl; \
        cerr << "  field(" << pfmt->field << ")" << endl; \
      )
      
      // Store the default width and height
      m_width = pfmt->width;
      m_height = pfmt->height;
    }

    // Enumerate the capture formats
    bool supported = false;
    v4l2_fmtdesc desc;
    desc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    desc.index = 0;
    ONDEBUG(cerr << "ImgIOV4L2BaseC::CheckFormat ioctl(VIDIOC_ENUM_FMT)" << endl;)
    while (ioctl(m_fd, VIDIOC_ENUM_FMT, &desc) != -1)
    {
      ONDEBUG( \
        cerr << "  [" << desc.index << "] ";
        cerr << "4cc(" << CHAR_STREAM_FROM_4CC(desc.pixelformat) << ") "; \
        cerr << "desc(" << desc.description << ")" << endl; \
      )
      desc.index++;

      bool found = false;

      UIntT pixelIndex = 0;
      while (pixelIndex < g_supportedFormats)
      {
        // Search the table
        if (pixelType == g_supportedFormat[pixelIndex].m_objectType &&
             desc.pixelformat == g_supportedFormat[pixelIndex].m_pixelFormat)
        {
          ONDEBUG( \
            cerr << "ImgIOV4L2BaseC::CheckFormat requires format(" << CHAR_STREAM_FROM_4CC(g_supportedFormat[pixelIndex].m_pixelFormat) << ")" << endl; \
          )
          m_pixelFormat = desc.pixelformat;
          m_fastBufferAvailable = g_supportedFormat[pixelIndex].m_fastBuffer;
          found = true;
          break;
        }

        // Try the next entry
        pixelIndex++;
      }

      // Check the pixel format is supported
      if (found)
      {
        ONDEBUG(cerr << "ImgIOV4L2BaseC::CheckFormat pixel format supported by device" << endl;)
        supported = true;
        break;
      }
    }
    
    // Check the size limits
    if (supported)
    {
      CheckSize();
    }
    ONDEBUG(RavlDebug("Result %d ",(int) supported));
    return supported;
  }
  
  
  
  bool ImgIOV4L2BaseC::CheckInput()
  {
    RavlAssertMsg(IsOpen(), "ImgIOV4L2BaseC::CheckInput device not open");
    RavlAssertMsg(!IsConfigured(), "ImgIOV4L2BaseC::CheckInput device already configured");

    // Enumerate the inputs
    m_inputMax = 0;
    v4l2_input input;
    input.index = 0;
    ONDEBUG(RavlDebug("ImgIOV4L2BaseC::CheckInput ioctl(VIDIOC_ENUMINPUT)"));
    while (ioctl(m_fd, VIDIOC_ENUMINPUT, &input) != -1)
    {
      ONDEBUG(RavlDebug("  [%u] name(%s)",m_inputMax,input.name));
      m_inputMax++;
      input.index = m_inputMax;
    }

    // If there is only 1 input, there's no point in changing it.
    if(m_inputMax <= 1)
      return true;

    // Are we trying to do something sensible ?
    if(m_input >= m_inputMax) {
      RavlWarning("Requested input out of range. Input %u of %u available",m_input,m_inputMax);
      return false;
    }


    // Set the input
    return (ioctl(m_fd, VIDIOC_S_INPUT, &m_input) != -1);
  }

  
  
  bool ImgIOV4L2BaseC::CheckSize()
  {
    RavlAssertMsg(IsOpen(), "ImgIOV4L2BaseC::CheckSize device not open");
    RavlAssertMsg(!IsConfigured(), "ImgIOV4L2BaseC::CheckSize device already configured");

    // Format params
    v4l2_format fmt;
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    v4l2_pix_format *pfmt = (v4l2_pix_format*)&(fmt.fmt);

    // Get the maximum width
    m_widthMax = m_width;
    for(UIntT i = m_width;i < g_defaultWidthMaximum;i++)
    {
      pfmt->width = i;
      pfmt->height = m_height;
      pfmt->pixelformat = m_pixelFormat;
      pfmt->field = g_supportedField[m_fieldFormat].m_field;
      if (ioctl(m_fd, VIDIOC_TRY_FMT, &fmt) != -1)
      {
        if (pfmt->width == i)
          m_widthMax = pfmt->width;
      }
    }
    ONDEBUG(cerr << "ImgIOV4L2BaseC::CheckSize width max(" << m_widthMax << ")" << endl);
    
    // Get the minimum width
    m_widthMin = m_width;
    for(UIntT i = m_width;i > g_defaultWidthMinimum;i--)
    {
      pfmt->width = i;
      pfmt->height = m_height;
      pfmt->pixelformat = m_pixelFormat;
      pfmt->field = g_supportedField[m_fieldFormat].m_field;
      if (ioctl(m_fd, VIDIOC_TRY_FMT, &fmt) != -1)
      {
        if (pfmt->width == i)
          m_widthMin = pfmt->width;
      }
    } 
    ONDEBUG(cerr << "ImgIOV4L2BaseC::CheckSize width min(" << m_widthMin << ")" << endl);
    
    // Get the maximum height
    m_heightMax = m_height;
    for(UIntT i = m_height;i < g_defaultHeightMaximum;i++)
    {
      pfmt->width = m_width;
      pfmt->height = i;
      pfmt->pixelformat = m_pixelFormat;
      pfmt->field = g_supportedField[m_fieldFormat].m_field;
      if (ioctl(m_fd, VIDIOC_TRY_FMT, &fmt) != -1)
      {
        if (pfmt->height == i)
          m_heightMax = pfmt->height;
      }
    }
    ONDEBUG(cerr << "ImgIOV4L2BaseC::CheckSize height max(" << m_heightMax << ")" << endl;)
    
    // Get the minimum height
    m_heightMin = m_height;
    for(UIntT i = m_height;i > g_defaultHeightMinimum;i--)
    {
      pfmt->width = m_width;
      pfmt->height = i;
      pfmt->pixelformat = m_pixelFormat;
      pfmt->field = g_supportedField[m_fieldFormat].m_field;
      if (ioctl(m_fd, VIDIOC_TRY_FMT, &fmt) != -1)
      {
        if (pfmt->height == i)
          m_heightMin = pfmt->height;
      }
    } 
    ONDEBUG(cerr << "ImgIOV4L2BaseC::CheckSize height min(" << m_heightMin << ")" << endl;)
    ONDEBUG(RavlDebug("Restoring Width:%u Height:%u ",m_width,m_height));
    // Reset the initial size
    pfmt->width = m_width;
    pfmt->height = m_height;
    pfmt->pixelformat = m_pixelFormat;
    pfmt->field = g_supportedField[m_fieldFormat].m_field;
    if(ioctl(m_fd, VIDIOC_S_FMT, &fmt) == -1) {
      RavlWarning("Failed to restore state. ");
    }
    
    return true;
  }
  
  
  bool ImgIOV4L2BaseC::ConfigureCapture()
  {
    RavlAssertMsg(IsOpen(), "ImgIOV4L2BaseC::ConfigureCapture device not open");
    RavlAssertMsg(!IsConfigured(), "ImgIOV4L2BaseC::ConfigureCapture device already configured");

    // Set the desired standard
    v4l2_std_id stdId = g_supportedStandard[m_standard].m_id;
    if (ioctl(m_fd, VIDIOC_S_STD, &stdId) == -1)
    {
      RavlWarning("Failed to set standard(%s) " ,g_supportedStandard[m_standard].m_name.data());
      //return false;
    }
    
    // Set the input
    if (!CheckInput())
    {
      RavlWarning("Failed to set input(%u) " ,m_input);
      //return false;
    }

    // Set the capture mode 
    v4l2_format fmt;
    memset(&fmt,0,sizeof(v4l2_format));

    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    v4l2_pix_format *pfmt = (v4l2_pix_format*)&(fmt.fmt);
    pfmt->width = m_width;
    pfmt->height = m_height;
    pfmt->pixelformat = m_pixelFormat;
    pfmt->field = g_supportedField[m_fieldFormat].m_field;
    ONDEBUG(RavlDebug("Setting capture format. Size:%u %u Format:%u Field:%u",m_width,m_height,m_pixelFormat,m_fieldFormat));
    if (ioctl(m_fd, VIDIOC_S_FMT, &fmt) == -1)
    {
      RavlError("Unable to set capture format. Size:%u %u Format:%u Field:%u",m_width,m_height,m_pixelFormat,m_fieldFormat);
      return false;
    }
    
    // Check we got what we asked for
    if (pfmt->width != m_width || pfmt->height != m_height)
    {
      RavlError("Failed to get requested size(%u,%u) ",m_height,m_width);
      return false;
    }
    
    // Request the required amount of memory-mapped buffers
    v4l2_requestbuffers reqbuf;
    reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    reqbuf.memory = V4L2_MEMORY_MMAP;
    reqbuf.count = m_bufferMax;
    if (ioctl(m_fd, VIDIOC_REQBUFS, &reqbuf) != -1)
    {
      if (m_bufferMax != reqbuf.count)
      {
        RavlWarning("ImgIOV4L2BaseC::ConfigureCapture ioctl(VIDIOC_REQBUFS) requested(%u) buffers, got(%u)", m_bufferMax,reqbuf.count );
      }
      
      if (reqbuf.count <= 0)
      {
        ONDEBUG(cerr << "ImgIOV4L2BaseC::ConfigureCapture ioctl(VIDIOC_REQBUFS) unable to allocate any mmap-ed buffers" << endl;)
        return false;
      }

      // Allocate the buffer table
      m_buffers = (TBuf*)calloc(reqbuf.count, sizeof(*m_buffers));
      RavlAssertMsg(m_buffers != NULL, "ImgIOV4L2BaseC::ConfigureCapture failed to allocate buffers");

      // Configure each buffers mmap entry
      for (m_bufferCount = 0; m_bufferCount < reqbuf.count; m_bufferCount++)
      {
        // Query the buffer
        v4l2_buffer buffer;
        memset(&buffer,0,sizeof(v4l2_buffer));
        buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buffer.memory = V4L2_MEMORY_MMAP;
        buffer.index = m_bufferCount;
        if (ioctl(m_fd, VIDIOC_QUERYBUF, &buffer) == -1)
        {
          RavlError("ImgIOV4L2BaseC::ConfigureCapture ioctl(VIDIOC_QUERYBUF) failed buffer(%u)",m_bufferCount);
          break;
        }

        ONDEBUG(RavlDebug("Allocating buffer size %u (Pixels:%u) ",buffer.length,m_width * m_height));

        // Map the buffer
        m_buffers[m_bufferCount].m_id = m_bufferIdCount++;
        m_buffers[m_bufferCount].m_length = buffer.length;
        m_buffers[m_bufferCount].m_start = mmap(NULL,
                                                buffer.length,
                                                PROT_READ | PROT_WRITE, // required
                                                MAP_SHARED,             // recommended
                                                m_fd,
                                                buffer.m.offset);
      
        // Verify
        if (m_buffers[m_bufferCount].m_start == MAP_FAILED)
        {
          ONDEBUG(RavlError("ImgIOV4L2BaseC::ConfigureCapture mmap failed buffer(%u)",m_bufferCount));
          break;
        }
        
        // Queue the buffer
        if (ioctl(m_fd, VIDIOC_QBUF, &buffer) == -1)
        {
          ONDEBUG(cerr << "ImgIOV4L2BaseC::ConfigureCapture ioctl(VIDIOC_QBUF) failed buffer(" << m_bufferCount << ")" << endl;)
          break;
        }
      }

      if (reqbuf.count != m_bufferCount)
      {
        // Report its not quite what we wanted..
        RavlDebug("ImgIOV4L2BaseC::ConfigureCapture allocated(%u) buffers, mmap-ed(%u) buffers",reqbuf.count,m_bufferCount);
      }
      else
      {
        ONDEBUG(cerr << "ImgIOV4L2BaseC::ConfigureCapture allocated(" << reqbuf.count << ") buffers, mmap-ed(" << m_bufferCount << ") buffers" << endl;)
      }
    }

    // Stream-on, Daniel-san
    const int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(m_fd, VIDIOC_STREAMON, &type) == -1)
    {
      RavlError("ImgIOV4L2BaseC::ConfigureCapture ioctl(VIDIOC_STREAMON) failed (%d)",errno);
      ReleaseCapture();
      return false;
    }
    
    return m_bufferCount > 0;
  }
  
  
  
  void ImgIOV4L2BaseC::ReleaseCapture()
  {
    RavlAssertMsg(IsConfigured(), "ImgIOV4L2BaseC::ReleaseCapture called on unconfigured device");
    RavlAssertMsg(IsOpen(), "ImgIOV4L2BaseC::ReleaseCapture called on closed device");
    
    // Stream-off, Daniel-san
    const int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(m_fd, VIDIOC_STREAMOFF, &type) == -1)
    {
      RavlError("ImgIOV4L2BaseC::ReleaseCapture ioctl(VIDIOC_STREAMOFF) failed");
    }

    // Unmap the buffers
    RavlAssertMsg(m_buffers != NULL, "ImgIOV4L2BaseC::ReleaseCapture null buffer pointer");
    for (UIntT i = 0; i < m_bufferCount; i++)
      munmap(m_buffers[i].m_start, m_buffers[i].m_length);
    free(m_buffers);
    m_buffers = NULL;
    
    // Reset the buffer variables
    m_bufferCount = 0;
    m_bufferOut.Empty();
  }



  bool ImgIOV4L2BaseC::CaptureBuffer(v4l2_buffer &buffer)
  {
    RavlAssertMsg(IsOpen(), "ImgIOV4L2BaseC::CaptureBuffer device not open");
    
    // Configure the device, if not
    if (!IsConfigured())
    {
      if (!ConfigureCapture())
      {
        Close();
        return false;
      }
    }
      
    // Dequeue a buffer
    buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buffer.memory = V4L2_MEMORY_MMAP;
    if (ioctl(m_fd, VIDIOC_DQBUF, &buffer) == -1)
    {
      RavlError("CaptureBuffer ioctl(VIDIOC_DQBUF) failed");
      return false;
    }
    /*
    ONDEBUG( \
      cerr << "ImgIOV4L2BaseC::CaptureBuffer ioctl(VIDIOC_DQBUF)" << endl; \
      cerr << "  index(" << buffer.index << ")" << endl; \
      cerr << "  field(" << buffer.field << ")" << endl; \
      cerr << "  seq(" << buffer.sequence << ")" << endl; \
    )
    */
    
    // Store the capture info
    m_seqNum = buffer.sequence;
    
    // Store the buffer as out
//    ONDEBUG(cerr << "ImgIOV4L2BaseC::CaptureBuffer id(" << m_buffers[buffer.index].m_id << ") index(" << buffer.index << ")" << endl;)
    m_bufferOut.Insert(m_buffers[buffer.index].m_id);
    
    return true;
  }
  
  
  
  bool ImgIOV4L2BaseC::HandleGetAttr(const StringC &attrName, StringC &attrValue)
  {
    // Process the int attributes
    if (attrName == "width" ||
        attrName == "height" || 
        attrName == "input")
    {
      IntT val = 0;
      bool ret = HandleGetAttr(attrName, val);
      attrValue = StringC(val);
      return ret;
    }
    
    // Driver
    if (attrName == "driver")
    {
      attrValue = m_deviceDriver;
      return true;
    }
    
    // Card
    if (attrName == "card")
    {
      attrValue = m_deviceCard;
      return true;
    }
    
    // Bus
    if (attrName == "bus")
    {
      attrValue = m_deviceBus;
      return true;
    }
    
    // Standard
    if (attrName == "standard")
    {
      attrValue = g_supportedStandard[m_standard].m_name;
      return true;
    }
    

    return false;
  }
  
  
  
  bool ImgIOV4L2BaseC::HandleSetAttr(const StringC &attrName, const StringC &attrValue)
  {
    // Process the int attributes
    if (attrName == "width" ||
        attrName == "height" ||
        attrName == "input")
    {
      return HandleSetAttr(attrName, attrValue.IntValue());
    }

    // Standard
    if (attrName == "standard")
    {
      UIntT stdCount = 0;
      while (stdCount < g_supportedStandards)
      {
        if (g_supportedStandard[stdCount].m_name == attrValue)
          break;
        stdCount++;
      }
      
      if (stdCount < g_supportedStandards)
      {
        // Lock the capture device
        MutexLockC lockCapture(m_lockCapture);
    
        // Set the width
        UIntT tempStd = m_standard;
        m_standard = stdCount;
        ONDEBUG(cerr << "ImgIOV4L2BaseC::HandleSetAttr standard(" << g_supportedStandard[stdCount].m_name << ")" << endl;)
  
        // If configure, test reconfiguring
        if (IsConfigured())
        {
          // Release the current capture config
          ReleaseCapture();
          
          // Try the new config, and restore the old if failed
          if (!ConfigureCapture())
          {
            RavlWarning("ImgIOV4L2BaseC::HandleSetAttr failed to set standard(%s)",attrValue.data());
            m_standard = tempStd;
          }
        }
      }
      else
      {
        ONDEBUG(cerr << "ImgIOV4L2BaseC::HandleSetAttr failed to find standard(" << attrValue << ")" << endl;)
      }

      return true;
    }
    
    // Field
    if (attrName == "field")
    {
      UIntT fieldCount = 0;
      while (fieldCount < g_supportedFields)
      {
        if (g_supportedField[fieldCount].m_name == attrValue)
          break;
        fieldCount++;
      }
      
      if (fieldCount < g_supportedFields)
      {
        // Lock the capture device
        MutexLockC lockCapture(m_lockCapture);
    
        // Set the width
        UIntT tempField = m_fieldFormat;
        m_fieldFormat = fieldCount;
        ONDEBUG(cerr << "ImgIOV4L2BaseC::HandleSetAttr field(" << g_supportedField[fieldCount].m_name << ")" << endl;)
  
        // If configure, test reconfiguring
        if (IsConfigured())
        {
          // Release the current capture config
          ReleaseCapture();
          
          // Try the new config, and restore the old if failed
          if (!ConfigureCapture())
          {
            RavlError("ImgIOV4L2BaseC::HandleSetAttr failed to set field(%s)",attrValue.data());
            m_fieldFormat = tempField;
          }
        }
      }
      else
      {
        ONDEBUG(cerr << "ImgIOV4L2BaseC::HandleSetAttr failed to find field(" << attrValue << ")" << endl;)
      }

      return true;
    }
    
    return false;
  }
  
  
  
  bool ImgIOV4L2BaseC::HandleGetAttr(const StringC &attrName, IntT &attrValue)
  {
    // Width
    if (attrName == "width")
    {
      attrValue = m_width;
      return true;
    }
    
    // Height
    if (attrName == "height")
    {
      attrValue = m_height;
      return true;
    }
    
    // Input
    if (attrName == "input")
    {
      attrValue = m_input;
      return true;
    }
    
    return false;
  }
  
  
  
  bool ImgIOV4L2BaseC::HandleSetAttr(const StringC &attrName, const IntT &attrValue)
  {
    // Width
    if (attrName == "width")
    {
      // Set the width
      if (attrValue >= 0)
      {
        if (static_cast<UIntT>(attrValue) >= m_widthMin && static_cast<UIntT>(attrValue) <= m_widthMax)
        {
          // Lock the capture device
          MutexLockC lockCapture(m_lockCapture);
      
          // Set the width
          UIntT tempWidth = m_width;
          m_width = attrValue;
          ONDEBUG(cerr << "ImgIOV4L2BaseC::HandleSetAttr width(" << m_width << ")" << endl);
  
          // If configure, test reconfiguring
          if (IsConfigured())
          {
            // Release the current capture config
            ReleaseCapture();
            
            // Try the new config, and restore the old if failed
            if (!ConfigureCapture())
            {
              m_width = tempWidth;
            }
          }
        }
        else {
          RavlError("ImgIOV4L2BaseC::HandleSetAttr failed to set width (%d) within range (%u - %u)",attrValue, m_widthMin,m_widthMax);
        }
      }
      else {
        RavlError("ImgIOV4L2BaseC::HandleSetAttr failed to set width less than zero (%d)",attrValue);
      }

      return true;
    }
    
    // Height
    if (attrName == "height")
    {
      // Set the height
      if (attrValue >= 0)
      {
        if (static_cast<UIntT>(attrValue) >= m_heightMin && static_cast<UIntT>(attrValue) <= m_heightMax)
        {
          // Lock the capture device
          MutexLockC lockCapture(m_lockCapture);
      
          // Set the height
          UIntT tempWidth = m_height;
          m_height = attrValue;
          ONDEBUG(cerr << "ImgIOV4L2BaseC::HandleSetAttr height(" << m_height << ")" << endl);
    
          // If configure, test reconfiguring
          if (IsConfigured())
          {
            // Release the current capture config
            ReleaseCapture();
            
            // Try the new config, and restore the old if failed
            if (!ConfigureCapture())
            {
              m_height = tempWidth;
            }
          }
        }
        else {
          RavlError("ImgIOV4L2BaseC::HandleSetAttr failed to set height (%d) within range (%u - %u)",attrValue,m_heightMin,m_heightMax);
        }
      }
      else {
        RavlError("ImgIOV4L2BaseC::HandleSetAttr failed to set height less than zero (%d)",attrValue);
      }
      
      return true;
    }

    // Input
    if (attrName == "input")
    {
      // Set the height
      if (attrValue >= 0 && attrValue < (IntT)m_inputMax)
      {
        // Lock the capture device
        MutexLockC lockCapture(m_lockCapture);
    
        // Set the input
        UIntT tempInput = m_input;
        m_input = attrValue;
        ONDEBUG(cerr << "ImgIOV4L2BaseC::HandleSetAttr input(" << m_input << ")" << endl;)

        // If configure, test reconfiguring
        if (IsConfigured())
        {
          // Release the current capture config
          ReleaseCapture();
          
          // Try the new config, and restore the old if failed
          if (!ConfigureCapture())
          {
            m_input = tempInput;
          }
        }
      }
      else
      {
        RavlError("ImgIOV4L2BaseC::HandleSetAttr failed to set input");
      }

      return true;
    }
    
    return false;
  }



  bool ImgIOV4L2BaseC::HandleGetAttr(const StringC &attrName, bool &attrValue)
  {
    // Fast buffers available
    if (attrName == "fast_buffers_available")
    {
      attrValue = m_fastBufferAvailable;
      return true;
    }

    // Fast buffers used
    if (attrName == "fast_buffers_used")
    {
      attrValue = m_fastBufferUsed;
      return true;
    }

    return false;
  }



  bool ImgIOV4L2BaseC::HandleSetAttr(const StringC &attrName, const bool &attrValue)
  {
    // Fast buffers used
    if (attrName == "fast_buffers_used")
    {
      if (m_fastBufferAvailable)
        m_fastBufferUsed = attrValue;
      else {
        RavlError("ImgIOV4L2BaseC::HandleSetAttr failed to set fast buffers");
      }
      return true;
    }
    
    return false;
  }
  
  

  bool ImgIOV4L2BaseC::BuildAttributes(AttributeCtrlBodyC &attrCtrl)
  {
    attrCtrl.RegisterAttribute(AttributeTypeStringC("driver", "V4L2 device driver", true, false));
    attrCtrl.RegisterAttribute(AttributeTypeStringC("card",   "Capture device",     true, false));
    attrCtrl.RegisterAttribute(AttributeTypeStringC("bus",    "Device bus",         true, false));

    attrCtrl.RegisterAttribute(AttributeTypeNumC<IntT>("input",         "Input",  true, true,           0,  m_inputMax - 1, 1,        0));
    attrCtrl.RegisterAttribute(AttributeTypeNumC<IntT>("width",         "Width",  true, true,  m_widthMin,      m_widthMax, 1,  m_width));
    attrCtrl.RegisterAttribute(AttributeTypeNumC<IntT>("height",        "Height", true, true, m_heightMin,     m_heightMax, 1, m_height));

    DListC<StringC> stdList;
    for (UIntT i = 0; i < g_supportedStandards; i++)
      stdList.InsLast(g_supportedStandard[i].m_name);
    attrCtrl.RegisterAttribute(AttributeTypeEnumC("standard", "Standard", true, true, stdList, stdList.First()));

    DListC<StringC> fieldList;
    for (UIntT i = 0; i < g_supportedFields; i++)
      fieldList.InsLast(g_supportedField[i].m_name);
    attrCtrl.RegisterAttribute(AttributeTypeEnumC("field", "Field", true, true, fieldList, fieldList.First()));

    attrCtrl.RegisterAttribute(AttributeTypeBoolC("fast_buffers_available", "Fast buffers available", true,                 false, m_fastBufferAvailable));
    attrCtrl.RegisterAttribute(AttributeTypeBoolC("fast_buffers_used",      "Fast buffers used",      true, m_fastBufferAvailable,      m_fastBufferUsed));

    return true;
  }
}

