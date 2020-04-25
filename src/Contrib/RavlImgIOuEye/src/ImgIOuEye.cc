// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2008, OmniPerception Ltd.
// This code may be redistributed under the terms of the MIT
// License. See http://www.opensource.org/licenses/mit-license.html
// file-header-ends-here
//! author = "Charles Galambos"
//! lib=RavlImgIOuEye

#include "Ravl/Image/ImgIOuEye.hh"
#include "Ravl/Exception.hh"
#include "Ravl/OS/SysLog.hh"
#include "Ravl/DP/AttributeValueTypes.hh"
#include <string.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {

  static const char *g_triggerModeNames[] = { "OFF","HI_LO","LO_HI","SOFTWARE",0 };


  //: Constructor.

  ImgIOuEyeBaseC::ImgIOuEyeBaseC(const std::type_info &pixelType,UIntT cameraId)
    : m_triggerMode(TRIG_OFF),
      m_rotation(ROT_0),
      m_state(UE_NotReady),
      m_snapshot(false),
      m_timeOutDelay(0.75)
  {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "Open uEye. ");
    for(int i = 0;i < m_NumBuffers;i++)
      m_buffers[i] = 0;

    if(!Open(cameraId)) {
      throw ExceptionOperationFailedC("Failed to open camera ");
    }
    if(!SetPixelType(pixelType)) {
      RavlSysLog(SYSLOG_ERR) << "Failed to set pixel type. ";
      m_state = UE_NotReady;
    }
  }


  //: Destructor.

  ImgIOuEyeBaseC::~ImgIOuEyeBaseC()
  {
    if(m_state == UE_Running) {
      // Need to stop anything ?
      if(is_StopLiveVideo( m_phf, IS_WAIT ) != IS_SUCCESS) {
        RavlSysLog(SYSLOG_ERR) << "Failed to stop video. ";
      }
      m_state = UE_Ready;
    }

    // Free buffers.
    for(int i = 0;i < m_NumBuffers;i++) {
      if(m_buffers[i] == 0)
        continue;

      if(is_FreeImageMem(m_phf,m_buffers[i],m_imgId[i]) != IS_SUCCESS) {
        RavlSysLog(SYSLOG_WARNING) << "Failed to free memory. ";
      }
    }

    is_ExitCamera(m_phf);
  }

  //: Open camera.

  bool ImgIOuEyeBaseC::Open(UIntT cameraId) {
    if(m_state != UE_NotReady) {
      RavlSysLog(SYSLOG_WARNING) << "Camera already open. ";
      return false;
    }
    m_phf = (HIDS) cameraId;
    int ret;
    RavlN::MutexLockC accessLock(m_accessMutex);

    if((ret = is_InitCamera(&m_phf,0)) != IS_SUCCESS) {
      RavlSysLog(SYSLOG_ERR) << "Failed to open camera id " << cameraId << ". Return code=" << ret << " ";
      return false;
    }
    if((ret = is_GetSensorInfo(m_phf, &m_sensorInfo)) != IS_SUCCESS) {
      RavlSysLog(SYSLOG_ERR) << "Failed to retrieve sensor infor for camera " << cameraId << " Return code=" << ret << " ";
      return false;
    }
    if(is_SetImageSize(m_phf, m_sensorInfo.nMaxWidth, m_sensorInfo.nMaxHeight) != IS_SUCCESS) {
      RavlSysLog(SYSLOG_ERR) << "Failed to set image size for camera " << cameraId << " Return code=" << ret << " ";
      return false;
    }
    m_captureSize = IndexRange2dC(0,m_sensorInfo.nMaxHeight-1,
                                  0,m_sensorInfo.nMaxWidth-1);

    m_state = UE_Ready;
    if(is_EnableEvent( m_phf, IS_SET_EVENT_FRAME) != IS_SUCCESS) {
      RavlSysLog(SYSLOG_ERR) << "Failed to enable event. ";
    }

    return true;
  }

  //: Allocate image buffers.
  // Should be called with camera lock aquired

  bool ImgIOuEyeBaseC::AllocateImages() {
    // Allocate images.
    for(int i = 0;i < m_NumBuffers;i++) {
      // Free buffer is it exists.
      if(m_buffers[i] != 0)
        is_FreeImageMem(m_phf,m_buffers[i],m_imgId[i]);

      // Allocate new entry of the correct size.
      if(is_AllocImageMem (m_phf, m_sensorInfo.nMaxWidth,m_sensorInfo.nMaxHeight,m_bitsPerPixel, &m_buffers[i],&m_imgId[i]) != IS_SUCCESS) {
        RavlSysLog(SYSLOG_ERR) << "Failed to allocate image memory. ";
        return false;
      }

      // Add to ring buffer.
      if (is_AddToSequence( m_phf,m_buffers[i],m_imgId[i]) != IS_SUCCESS)
        return false;
    }
    return true;
  }

  //: Set pixel type
  // Should be called with camera lock aquired

  bool ImgIOuEyeBaseC::SetPixelType(const std::type_info &pixelType) {
    if(m_state == UE_Running) {
      // Need to stop anything ?
      if(is_StopLiveVideo( m_phf, IS_WAIT ) != IS_SUCCESS) {
        RavlSysLog(SYSLOG_ERR) << "Failed to stop video. ";
      }
      m_state = UE_Ready;
    }

    if(pixelType == typeid(ByteT)) {
      m_bitsPerPixel = 8;
      return (is_SetColorMode(m_phf,IS_SET_CM_Y8) == IS_SUCCESS);
    } else if(pixelType == typeid(ByteRGBValueC)) {
      m_bitsPerPixel = 24;
      return (is_SetColorMode(m_phf,IS_SET_CM_RGB24) == IS_SUCCESS);
    }
    return false;
  }

  //: Capture a single image into the given buffer.

  bool ImgIOuEyeBaseC::CaptureImage(char *buffer) {
    RavlAssert(buffer != 0);
    if(m_state == UE_NotReady)
      return false;
    int ret = 0;

    RavlN::MutexLockC accessLock(m_accessMutex);

    // Start video capture if needed.
    UpdateBuffers();

    IntT maxDelay = m_timeOutDelay*1000.0;
    if(maxDelay < 10) maxDelay = 10;
    if(maxDelay > 10000) maxDelay = 10000;

    if(m_snapshot && m_state != UE_TriggerWait) {
      // Note: Maximum wait is 1 second at the moment.
      ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "Freezing video for capture. ");
      if((ret = is_FreezeVideo(m_phf,maxDelay)) != IS_SUCCESS) {
        RavlSysLog(SYSLOG_ERR) << "Failed to freeze video for capture. ErrorCode:" << ret << " ";
        return false;
      }
    } else {
      //ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "Waiting for frame to arrive. ");
      // Wait for frame to arrive.

      if((ret = is_WaitEvent(m_phf,IS_SET_EVENT_FRAME,maxDelay)) != IS_SUCCESS) {
        RavlSysLog(SYSLOG_ERR) << "Failed to wait for event. Delay=" << maxDelay << "  Error:" << ret;
        return false;
      }
    }

    // Find the last full frame to arrive.
    int dummy = 0;
    char *pMem,*pLast;
    if(is_GetActSeqBuf(m_phf, &dummy, &pMem, &pLast ) != IS_SUCCESS) {
      RavlSysLog(SYSLOG_ERR) << "Failed to get active buffer. ";
      return false;
    }

    // Copy that buffer.
    UIntT imgId = 0;
    for(int i = 0;i < m_NumBuffers;i++) {
      if(m_buffers[i] == pLast) {
        imgId = m_imgId[i];
        break;
      }
    }

    if(is_LockSeqBuf(m_phf,imgId,pLast) != IS_SUCCESS) {
      RavlSysLog(SYSLOG_ERR) << "Failed to lock sequence buffer. ";
      return false;
    }
    // copy & rotate the image
    switch(m_rotation) {
      case ROT_0: {
        int pixelLen = m_bitsPerPixel / 8;
        if(pixelLen == 1) {
          memcpy(buffer, pLast, ImageBufferSize());
        } else {
          int width = m_captureSize.Cols();
          int height = m_captureSize.Rows();
          const char *srcPtr = pLast;
          char *resPtr = reinterpret_cast<char *>(buffer);
          for(int y = height; y > 0; y--) {
            for(int x = width; x > 0; x--) {
              *resPtr++ = srcPtr[2];
              *resPtr++ = srcPtr[1];
              *resPtr++ = srcPtr[0];
              srcPtr += 3;
            }
          }
        }
        break;
      }
      case ROT_90: {
        int width = m_captureSize.Cols();
        int height = m_captureSize.Rows();
        int imageLength = ImageBufferSize();
        int pixelLen = m_bitsPerPixel / 8;

        if(pixelLen == 1) {
          const char *srcPtr = pLast + width - 1;
          char *resPtr = reinterpret_cast<char *>(buffer);
          for(int x = width; x > 0; x--) {
            for(int y = height; y > 0; y--) {
              *resPtr++ = *srcPtr;
              srcPtr += width;
            }
            srcPtr -= imageLength + 1;
          }
        } else {
          const char *srcPtr = pLast + (width - 1) * 3;
          char *resPtr = reinterpret_cast<char *>(buffer);
          for(int x = width; x > 0; x--) {
            for(int y = height; y > 0; y--) {
              *resPtr++ = srcPtr[2];
              *resPtr++ = srcPtr[1];
              *resPtr++ = srcPtr[0];
              srcPtr += width * 3;
            }
            srcPtr -= imageLength + 1 * 3;
          }
        }
        break;
      }
      case ROT_180: {
        int imageLength = ImageBufferSize();
        int pixelLen = m_bitsPerPixel / 8;
        if(pixelLen == 1) {
          const char *srcPtr = pLast + imageLength - 1;
          char *resPtr = reinterpret_cast<char *>(buffer);
          while(srcPtr >= pLast) {
            *resPtr++ = *srcPtr--;
          }
        } else {
          const char *srcPtr = pLast + imageLength - 1;
          char *resPtr = reinterpret_cast<char *>(buffer);
          while(srcPtr >= pLast) {
            *resPtr++ = *srcPtr--;
          }
        }
        break;
      }
      case ROT_270: {
        int width = m_captureSize.Cols();
        int height = m_captureSize.Rows();
        int imageLength = ImageBufferSize();
        int pixelLen = m_bitsPerPixel / 8;
        if(pixelLen == 1) {
          const char *srcPtr = pLast + width * (height - 1);
          char *resPtr = reinterpret_cast<char *>(buffer);
          for(int x = width; x > 0; x--) {
            for(int y = height; y > 0; y--) {
              *resPtr++ = *srcPtr;
              srcPtr -= width;
            }
            srcPtr += 1 + imageLength;
          }
        } else {
          const char *srcPtr = pLast + width * (height - 1) * 3;
          char *resPtr = reinterpret_cast<char *>(buffer);
          for(int x = width; x > 0; x--) {
            for(int y = height; y > 0; y--) {
              *resPtr++ = srcPtr[2];
              *resPtr++ = srcPtr[1];
              *resPtr++ = srcPtr[0];
              srcPtr -= width * 3;
            }
            srcPtr += 1 * 3 + imageLength;
          }
        }
        break;
      }
    }

    RavlAssert(buffer != 0);
    if(is_UnlockSeqBuf(m_phf,imgId,pLast) != IS_SUCCESS) {
      RavlSysLog(SYSLOG_ERR) << "Failed to unlock sequence buffer. ";
    }

#if 1
    // Setup trigger for next frame.
    if(m_snapshot && m_triggerMode != TRIG_OFF) {
      // Note: Maximum wait is 1 second at the moment.
      ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "Started waiting for next trigger event. ");
      if((ret = is_FreezeVideo(m_phf,IS_DONT_WAIT)) != IS_SUCCESS) {
        RavlSysLog(SYSLOG_ERR) << "Failed to freeze video. ErrorCode:" << ret << " ";
        return false;
      }
      m_state = UE_TriggerWait;
    }
#endif

    return true;
  }


  bool ImgIOuEyeBaseC::HandleGetAttr(const StringC &attrName, StringC &attrValue)
  {
    // Process the int attributes
    if (attrName == "width" || attrName == "height")
    {
      IntT val;
      bool ret = HandleGetAttr(attrName, val);
      attrValue = StringC(val);
      return ret;
    }
    if(attrName == "trigger") {
      attrValue = g_triggerModeNames[m_triggerMode];
      return true;
    }
    if(attrName == "rotation") {
      attrValue = StringC((int)(m_rotation) * 90);
      return true;
    }
    if(attrName == "driver") {
      attrValue = "ueye";
      return true;
    }
    if(attrName == "card") {
      attrValue = m_sensorInfo.strSensorName;
      return true;
    }

    return false;
  }

  
  bool ImgIOuEyeBaseC::HandleSetAttr(const StringC &attrName, const StringC &attrValue)
  {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "ImgIOuEyeBaseC::SetAttr (StringC) " << attrName << " Value='" << attrValue << "'\n");

    // Process the int attributes
    if(attrName == "width" || attrName == "height")
      return HandleSetAttr(attrName, attrValue.IntValue());

    if(attrName == "trigger") {
      for(IntT i = 0;g_triggerModeNames[i] != 0;i++) {
        if(attrValue == g_triggerModeNames[i]) {
          HandleSetAttr(attrName,i);
          return true;
        }
      }
      RavlSysLog(SYSLOG_ERR) << "Failed to set unrecognised trigger mode '" << attrValue << "'. ";
      return true;
    }
    if(attrName == "rotation") {
      for(IntT i = 0; i < 360; i += 90) {
        if(attrValue == StringC(i)) {
          HandleSetAttr(attrName, i);
          return true;
        }
      }
      RavlSysLog(SYSLOG_ERR) << "Failed to set unrecognised rotation '" << attrValue << "'. ";
      return true;
    }
    return false;
  }



  bool ImgIOuEyeBaseC::HandleGetAttr(const StringC &attrName, IntT &attrValue)
  {
    // Width
    if (attrName == "width") {
      attrValue = (m_rotation == ROT_0 || ROT_180) ? m_captureSize.Cols() : m_captureSize.Rows();
      return true;
    }

    // Height
    if (attrName == "height") {
      attrValue = (m_rotation == ROT_0 || ROT_180) ? m_captureSize.Rows() : m_captureSize.Cols();
      return true;
    }
    if(attrName == "trigger") {
      attrValue = static_cast<IntT>(m_triggerMode);
      return true;
    }
    if(attrName == "rotation") {
      switch(m_rotation) {
        case ROT_0:   attrValue = 0;   break;
        case ROT_90:  attrValue = 90;  break;
        case ROT_180: attrValue = 180; break;
        case ROT_270: attrValue = 270; break;
      }
      return true;
    }
    if(attrName == "binning_vertical") {
      RavlN::MutexLockC accessLock(m_accessMutex);
      int mode = is_SetBinning(m_phf,IS_GET_BINNING);
      if(mode & IS_BINNING_2X_VERTICAL) {
        attrValue = 2;
      } if(mode & IS_BINNING_4X_VERTICAL) {
        attrValue = 4;
      } else
        attrValue = 1;
      return true;
    }
    if(attrName == "binning_horizontal") {
      RavlN::MutexLockC accessLock(m_accessMutex);
      int mode = is_SetBinning(m_phf,IS_GET_BINNING);
      if(mode & IS_BINNING_2X_HORIZONTAL) {
        attrValue = 2;
      } if(mode & IS_BINNING_4X_HORIZONTAL) {
        attrValue = 4;
      } else
        attrValue = 1;
      return true;
    }
    if(attrName == "framerate") {
      RealT val;
      if(HandleGetAttr(attrName,val)) {
        attrValue = RavlN::Round(val);
        return true;
      }
    }
    if(attrName == "pixel_clock") {
      RealT val;
      if(HandleGetAttr(attrName,val)) {
        attrValue = RavlN::Round(val);
        return true;
      }
    }

    return false;
  }



  bool ImgIOuEyeBaseC::HandleSetAttr(const StringC &attrName, const IntT &attrValue)
  {
    int ret;
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "ImgIOuEyeBaseC::SetAttr (int) " << attrName << " Value=" << attrValue << "\n");

    // Width
    if (attrName == "width") {
      RavlSysLog(SYSLOG_DEBUG) << "Setting width not implemented. ";
      return false;
    }

    // Height
    if (attrName == "height") {
      RavlSysLog(SYSLOG_DEBUG) << "Setting height not implemented. ";
      return false;
    }

    if(attrName == "trigger") {
      uEyeTrigT newTrig = static_cast<uEyeTrigT>(attrValue);
      // Anything to change ?
      int mode = 0;
      switch(newTrig) {
      case TRIG_OFF:  mode = IS_SET_TRIG_OFF;   break;
      case TRIG_HILO: mode = IS_SET_TRIG_HI_LO; break;
      case TRIG_LOHI: mode = IS_SET_TRIG_LO_HI; break;
      case TRIG_SOFT: mode = IS_SET_TRIG_SOFTWARE;   break;
      }
      int ret;
      if(newTrig == TRIG_OFF && m_state == UE_TriggerWait && m_snapshot) {
        // Switch out of trigger mode.
        m_state = UE_Running;
      }

      RavlN::MutexLockC accessLock(m_accessMutex);
      UpdateBuffers();

      if((ret = is_SetExternalTrigger(m_phf,mode)) != IS_SUCCESS) {
        RavlSysLog(SYSLOG_ERR) << "Failed to set trigger mode '" << g_triggerModeNames[attrValue] << "' . ErrorCode:" << ret << " ";
        return true;
      }

      m_triggerMode = newTrig;
      if(newTrig == TRIG_OFF)
        return true;

      if(m_snapshot && m_state != UE_TriggerWait) {
        // Note: Maximum wait is 1 second at the moment.
        ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "Started waiting for trigger event. ");
        if((ret = is_FreezeVideo(m_phf,IS_DONT_WAIT)) != IS_SUCCESS) {
          RavlSysLog(SYSLOG_ERR) << "Failed to freeze video. ErrorCode:" << ret << " ";
          return false;
        }
        m_state = UE_TriggerWait;
      }
      accessLock.Unlock();
      SignalAttrChange("trigger");

      return true;
    }

    if(attrName == "rotation") {
      RavlN::MutexLockC accessLock(m_accessMutex);
      switch(attrValue) {
      case 90:  m_rotation = ROT_90;  break;
      case 180: m_rotation = ROT_180; break;
      case 270: m_rotation = ROT_270; break;
      default:  m_rotation = ROT_0;   break;
      }
      accessLock.Unlock();
      SignalAttrChange("rotation");

      return true;
    }

    if(attrName == "binning_vertical") {
      std::cerr << "Binning modes=" << is_SetBinning(m_phf,IS_GET_SUPPORTED_BINNING) << "\n";
      RavlN::MutexLockC accessLock(m_accessMutex);
      int mode = is_SetBinning(m_phf,IS_GET_BINNING) & ~IS_BINNING_MASK_VERTICAL;
      switch(attrValue) {
      case 1: break;
      case 2: mode = mode | IS_BINNING_2X_VERTICAL ; break;
      case 3:
      case 4: mode = mode | IS_BINNING_4X_VERTICAL ; break;
      default:
        RavlSysLog(SYSLOG_ERR) << "Unsupported binning request. ";
        return true;
      }
      if((ret = is_SetBinning(m_phf,mode)) != IS_SUCCESS) {
        RavlSysLog(SYSLOG_ERR) << "Failed to set binning mode '" << attrValue  << "' . ErrorCode:" << ret << " ";
        return true;
      }

      ResetImageSize();
      accessLock.Unlock();
      SignalAttrChange("binning_vertical");

      // Make sure images are reallocated.
      return true;
    }
    if(attrName == "binning_horizontal") {
      std::cerr << "Binning modes=" << is_SetBinning(m_phf,IS_GET_SUPPORTED_BINNING) << "\n";
      RavlN::MutexLockC accessLock(m_accessMutex);
      int mode = is_SetBinning(m_phf,IS_GET_BINNING) & ~IS_BINNING_MASK_HORIZONTAL;
      switch(attrValue) {
      case 1: break;
      case 2: mode = mode | IS_BINNING_2X_HORIZONTAL ; break;
      case 3:
      case 4: mode = mode | IS_BINNING_4X_HORIZONTAL ; break;
      default:
        RavlSysLog(SYSLOG_ERR) << "Unsupported binning request. ";
        return true;
      }
      if((ret = is_SetBinning(m_phf,mode)) != IS_SUCCESS) {
        RavlSysLog(SYSLOG_ERR) << "Failed to set binning mode '" << attrValue  << "' . ErrorCode:" << ret << " ";
        return true;
      }

      ResetImageSize();
      accessLock.Unlock();
      SignalAttrChange("binning_horizontal");

      // Make sure images are reallocated.
      return true;
    }
    if(attrName == "framerate") {
      RealT val = attrValue;
      HandleSetAttr(attrName,val);
      return true;
    }
    if(attrName == "pixel_clock") {
      RealT val = attrValue;
      HandleSetAttr(attrName,val);
      return true;
    }
    return false;
  }

  //: Reset image size
  // Should be called with lock aquired

  void ImgIOuEyeBaseC::ResetImageSize() {
    int ret = 0;
    if(m_state == UE_Running) {
      // Need to stop anything ?
      if((ret = is_StopLiveVideo( m_phf, IS_DONT_WAIT )) != IS_SUCCESS) {
        RavlSysLog(SYSLOG_ERR) << "Failed to stop video. ErrorCode:" << ret;
      }
      m_state = UE_Ready;
    }
  }

  //: Update buffer state.
  // Should be called with lock aquired

  void ImgIOuEyeBaseC::UpdateBuffers() {
    int ret;
    if(m_state == UE_Ready) {
      // Make sure ring buffer is allocated so we're ready to
      // trigger.

      AllocateImages();

      if(!m_snapshot) {
        ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "Starting streaming video capture. ");
        if((ret = is_CaptureVideo(m_phf,IS_DONT_WAIT)) != IS_SUCCESS) {
          RavlSysLog(SYSLOG_ERR) << "Failed to capture video. Status:" << ret << " Snapshot=" << m_snapshot << " ";
        }
      }
      m_state = UE_Running;
    }
  }


  bool ImgIOuEyeBaseC::HandleGetAttr(const StringC &attrName, bool &attrValue)
  {
    int ret;
    if(attrName == "snapshot") {
      attrValue = m_snapshot;
      return true;
    }
    if(attrName == "hardware_gamma") {
      RavlN::MutexLockC accessLock(m_accessMutex);
      attrValue = is_SetHardwareGamma(m_phf,IS_GET_HW_GAMMA);
      return true;
    }
    if(attrName == "auto_shutter") {
      RavlN::MutexLockC accessLock(m_accessMutex);
      double v1 = 0,v2 =0;
      if((ret = is_SetAutoParameter(m_phf,IS_GET_ENABLE_AUTO_SHUTTER,&v1,&v2)) != IS_SUCCESS) {
        RavlSysLog(SYSLOG_ERR) << "Failed to read auto shutter state. Error:" << ret;
      }
      attrValue = v1 > 0;
      return true;
    }
    if(attrName == "auto_gain") {
      RavlN::MutexLockC accessLock(m_accessMutex);
      double v1 = 0,v2 =0;
      if((ret = is_SetAutoParameter(m_phf,IS_GET_ENABLE_AUTO_GAIN,&v1,&v2)) != IS_SUCCESS) {
        RavlSysLog(SYSLOG_ERR) << "Failed to read auto gain state. Error:" << ret;
      }
      attrValue = v1 > 0;
      return true;
    }
    if(attrName == "gain_boost") {
      RavlN::MutexLockC accessLock(m_accessMutex);
      ret = is_SetGainBoost(m_phf, IS_GET_GAINBOOST);
      attrValue = (ret == IS_SET_GAINBOOST_ON);
      return true;
    }
    if(attrName == "auto_framerate") {
      RavlN::MutexLockC accessLock(m_accessMutex);
      double v1 = 0,v2 =0;
      if((ret = is_SetAutoParameter(m_phf,IS_GET_ENABLE_AUTO_FRAMERATE,&v1,&v2)) != IS_SUCCESS) {
        RavlSysLog(SYSLOG_ERR) << "Failed to read auto framerate state. Error:" << ret;
      }
      attrValue = v1 > 0;
      return true;
    }
    if(attrName == "global_shutter") {
      RavlN::MutexLockC accessLock(m_accessMutex);
      ret = is_SetGlobalShutter(m_phf, IS_GET_GLOBAL_SHUTTER);
      attrValue = (ret == IS_SET_GLOBAL_SHUTTER_ON);
      return true;
    }

    return false;
  }

  bool ImgIOuEyeBaseC::HandleSetAttr(const StringC &attrName, const bool &attrValue)
  {
    int ret;
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "ImgIOuEyeBaseC::SetAttr (bool) " << attrName << " Value=" << attrValue << " ");

    if(attrName == "snapshot") {
      if(m_snapshot == attrValue)
        return true;
      // Make sure capture is restarted when we leave snapshot mode.
      if(m_snapshot)
        m_state = UE_Ready;
      else {
        if(m_state == UE_Running) {
          RavlN::MutexLockC accessLock(m_accessMutex);
          if((ret = is_StopLiveVideo (m_phf,IS_DONT_WAIT)) != IS_SUCCESS) {
            RavlSysLog(SYSLOG_ERR) << "Failed to stop live video. ErrorCode:" << ret << " ";
          }
        }
      }
      m_snapshot = attrValue;
      return true;
    }
    if(attrName == "hardware_gamma") {
      int mode = attrValue ? IS_SET_HW_GAMMA_ON : IS_SET_HW_GAMMA_OFF;
      RavlN::MutexLockC accessLock(m_accessMutex);
      if((ret = is_SetHardwareGamma(m_phf,mode)) != IS_SUCCESS) {
        RavlSysLog(SYSLOG_ERR) << "Failed to set hardware gain to " << attrValue << " Error:" << ret;
      }
      accessLock.Unlock();
      SignalAttrChange("hardware_gamma");
      return true;
    }
    if(attrName == "auto_shutter") {
      double v1 = attrValue ? 1.0 : 0.0;
      double v2 = 0;
      RavlN::MutexLockC accessLock(m_accessMutex);
      if((ret = is_SetAutoParameter(m_phf,IS_SET_ENABLE_AUTO_SHUTTER,&v1,&v2)) != IS_SUCCESS) {
        RavlSysLog(SYSLOG_ERR) << "Failed to set auto shutter to " << attrValue << " Error:" << ret;
      }
      accessLock.Unlock();
      SignalAttrChange("auto_shutter");
      return true;
    }
    if(attrName == "auto_gain") {
      double v1 = attrValue ? 1.0 : 0.0;
      double v2 = 0;
      RavlN::MutexLockC accessLock(m_accessMutex);
      if((ret = is_SetAutoParameter(m_phf,IS_SET_ENABLE_AUTO_GAIN,&v1,&v2)) != IS_SUCCESS) {
        RavlSysLog(SYSLOG_ERR) << "Failed to set auto gain to " << attrValue << " Error:" << ret;
      }
      accessLock.Unlock();
      SignalAttrChange("auto_gain");
      return true;
    }
    if(attrName == "gain_boost") {
      int mode = attrValue ? IS_SET_GAINBOOST_ON : IS_SET_GAINBOOST_OFF;
      RavlN::MutexLockC accessLock(m_accessMutex);
      if((ret = is_SetGainBoost(m_phf, mode)) != IS_SUCCESS) {
        RavlSysLog(SYSLOG_ERR) << "Failed to set gain boost to " << attrValue << " Error:" << ret;
      }
      accessLock.Unlock();
      SignalAttrChange("gain_boost");
      return true;
    }
    if(attrName == "auto_framerate") {
      double v1 = attrValue ? 1.0 : 0.0;
      double v2 = 0;
      RavlN::MutexLockC accessLock(m_accessMutex);
      if((ret = is_SetAutoParameter(m_phf,IS_SET_ENABLE_AUTO_FRAMERATE,&v1,&v2)) != IS_SUCCESS) {
        RavlSysLog(SYSLOG_ERR) << "Failed to set auto framerate to " << attrValue << " Error:" << ret;
      }
      accessLock.Unlock();
      SignalAttrChange("auto_framerate");
      return true;
    }
    if(attrName == "trigger_soft") {
      if(attrValue) {
        IntT maxDelay = m_timeOutDelay*1000.0;
        if(maxDelay < 10) maxDelay = 10;
        if(maxDelay > 10000) maxDelay = 10000;

        if((ret = is_FreezeVideo(m_phf,maxDelay)) != IS_SUCCESS) {
          RavlSysLog(SYSLOG_ERR) << "Failed to freeze video for capture. ErrorCode:" << ret << " ";
        }
        return true;
      }
      return true;
    }



    if(attrName == "global_shutter") {
      RavlN::MutexLockC accessLock(m_accessMutex);
      int mode = attrValue ? IS_SET_GLOBAL_SHUTTER_ON : IS_SET_GLOBAL_SHUTTER_OFF;
      if((ret = is_SetGlobalShutter(m_phf,mode)) != IS_SUCCESS) {
        RavlSysLog(SYSLOG_ERR) << "Failed to set global shutter. ErrorCode:" << ret << " ";
        if(ret == IS_NOT_SUPPORTED)
          RavlSysLog(SYSLOG_ERR) << "  Error code indicates global shutter not supported. Trigger mode may have to be enable to use this feature.";
        return true;
      }

      return true;
    }
    return false;
  }

  //: Handle get attribute (RealT)
  // Returns false if the attribute name is unknown.

  bool ImgIOuEyeBaseC::HandleGetAttr(const StringC &attrName, RealT &attrValue) {
    int ret;
    if(attrName == "trigger_delay") {
      RavlN::MutexLockC accessLock(m_accessMutex);
      attrValue = (RealT) is_SetTriggerDelay(m_phf,IS_GET_TRIGGER_DELAY) * 1e-6;
      return true;
    }
    if(attrName == "shutter_speed") {
      RavlN::MutexLockC accessLock(m_accessMutex);
      attrValue = 0; // Incase things go wrong, set to a known value.
      if((ret = is_SetExposureTime (m_phf, IS_GET_EXPOSURE_TIME,&attrValue)) != IS_SUCCESS) {
        RavlSysLog(SYSLOG_ERR) << "Failed to get shutter speed. ErrorCode:" << ret << "\n";
      }
      attrValue /= 1000; // Put time into seconds.
      return true;
    }
    if(attrName == "pixel_clock") {
      RavlN::MutexLockC accessLock(m_accessMutex);
      attrValue = (RealT) is_SetPixelClock(m_phf,IS_GET_PIXEL_CLOCK) * 1.0e6;
      return true;
    }
    if(attrName == "gain") {
      RavlN::MutexLockC accessLock(m_accessMutex);
      attrValue = (RealT) is_SetHardwareGain(m_phf, IS_GET_MASTER_GAIN,IS_IGNORE_PARAMETER,IS_IGNORE_PARAMETER,IS_IGNORE_PARAMETER) / 100.0;
      return true;
    }
    if(attrName == "gamma") {
      RavlN::MutexLockC accessLock(m_accessMutex);
      attrValue = ((RealT) is_SetGamma(m_phf,IS_GET_GAMMA)) / 100.0;
      return true;
    }
    if(attrName == "brightness") {
      RavlN::MutexLockC accessLock(m_accessMutex);
      attrValue = (RealT) is_SetBrightness(m_phf,IS_GET_BRIGHTNESS) / 255.0;
      return true;
    }
    if(attrName == "contrast") {
      RavlN::MutexLockC accessLock(m_accessMutex);
      attrValue = (RealT) is_SetBrightness(m_phf,IS_GET_CONTRAST) / 255.0;
      return true;
    }
    if(attrName == "framerate") {
      RavlN::MutexLockC accessLock(m_accessMutex);
      double framerate = 25;
      if((ret = is_SetFrameRate(m_phf,IS_GET_FRAMERATE,&framerate)) != IS_SUCCESS)
        RavlSysLog(SYSLOG_WARNING) << "Failed to get the current framerate. ";
      attrValue = framerate;
      return true;
    }
    if(attrName == "timeout") {
      attrValue = m_timeOutDelay;
      return true;
    }
    if(attrName == "shutter_min") {
      double minExp = 0,maxExp=100,intExp =0.0001;
      if((ret = is_GetExposureRange (m_phf, &minExp,&maxExp,&intExp)) != IS_SUCCESS) {
        RavlSysLog(SYSLOG_WARNING) << "Failed to get the exposure range. ";
      }
      // Change things to seconds.
      attrValue = minExp / 1000.0;
      return true;
    }
    if(attrName == "shutter_max") {
      double minExp = 0,maxExp=100,intExp =0.0001;
      if((ret = is_GetExposureRange (m_phf, &minExp,&maxExp,&intExp)) != IS_SUCCESS) {
        RavlSysLog(SYSLOG_WARNING) << "Failed to get the exposure range. ";
      }
      // Change things to seconds.
      attrValue = maxExp / 1000.0;
      return true;
    }
    if(attrName == "shutter_step") {
      double minExp = 0,maxExp=100,intExp =0.0001;
      if((ret = is_GetExposureRange (m_phf, &minExp,&maxExp,&intExp)) != IS_SUCCESS) {
        RavlSysLog(SYSLOG_WARNING) << "Failed to get the exposure range. ";
      }
      // Change things to seconds.
      attrValue = intExp / 1000.0;
      return true;
    }
    if(attrName == "global_flash_delay") {
      ULONG theDelay = 0,theExposure = 0;
      if((ret = is_GetGlobalFlashDelays(m_phf, &theDelay,&theExposure)) != IS_SUCCESS) {
        RavlSysLog(SYSLOG_WARNING) << "Failed to get the global flash delays. ";
      }
      // Change things to seconds.
      attrValue = ((double)theDelay) * 1.0e-6; // Delay is in us
      return true;
    }
    if(attrName == "global_flash_duration") {
      ULONG theDelay = 0,theExposure = 0;
      if((ret = is_GetGlobalFlashDelays(m_phf, &theDelay,&theExposure)) != IS_SUCCESS) {
        RavlSysLog(SYSLOG_WARNING) << "Failed to get the global flash delays. ";
      }
      // Change things to seconds.
      attrValue = ((double)theExposure) * 1.0e-6; // shutter time in us
      return true;
    }

    return false;
  }

  //: Handle set attribute (RealT)
  // Returns false if the attribute name is unknown.

  bool ImgIOuEyeBaseC::HandleSetAttr(const StringC &attrName, const RealT &attrValue) {
    int ret;
    if(attrName == "trigger_delay") {
      // Check value is sensible.
      if(attrValue < 0.0 || attrValue > 30.0)
        return true;
      // Try and set it.
      int value = Round(attrValue * 1e6);
      int ret;
      RavlN::MutexLockC accessLock(m_accessMutex);
      if((ret = is_SetTriggerDelay(m_phf,value)) != IS_SUCCESS) {
        RavlSysLog(SYSLOG_ERR) << "Failed to set trigger delay to " << value << " ErrorCode:" << ret << "\n";
      }
      accessLock.Unlock();
      SignalAttrChange("trigger_delay");
      return true;
    }
    if(attrName == "shutter_speed") {

      RavlN::MutexLockC accessLock(m_accessMutex);
      double tmp = 0;
      if((ret = is_SetExposureTime (m_phf, attrValue*1000.0,&tmp)) != IS_SUCCESS) {
        RavlSysLog(SYSLOG_ERR) << "Failed to set shutter speed. ErrorCode:" << ret << "\n";
      }
      SignalAttrChange("global_flash_delay");
      SignalAttrChange("global_flash_duration");
      SignalAttrChange("shutter_speed");
      return true;
    }

    if(attrName == "pixel_clock") {
      RavlN::MutexLockC accessLock(m_accessMutex);
      if((ret = is_SetPixelClock(m_phf,attrValue/1.0e6)) != IS_SUCCESS)
        RavlSysLog(SYSLOG_ERR) << "Failed to set pixel clock to " << attrValue << " Error:" << ret;
      accessLock.Unlock();
      SignalAttrChange("pixel_clock");
      SignalAttrChange("shutter_min");
      SignalAttrChange("shutter_max");
      SignalAttrChange("shutter_step");
      SignalAttrChange("shutter_speed");
      SignalAttrChange("framerate");
      SignalAttrChange("global_flash_delay");
      SignalAttrChange("global_flash_duration");
      return true;
    }
    if(attrName == "gain") {
      RealT req = attrValue;
      if(req < 0) req = 0;
      if(req > 1.0) req = 1.0;
      int newGain = RavlN::Round(req * 100.0);
      RavlN::MutexLockC accessLock(m_accessMutex);
      if((ret = is_SetHardwareGain(m_phf,newGain,IS_IGNORE_PARAMETER,IS_IGNORE_PARAMETER,IS_IGNORE_PARAMETER)) != 0) {
        RavlSysLog(SYSLOG_ERR) << "Failed to gain to " << attrValue << " (" << newGain << ")\n";
      }
      accessLock.Unlock();
      SignalAttrChange("gain");
      return true;
    }
    if(attrName == "gamma") {
      int newGamma = (RealT) attrValue * 100.0;
      if(newGamma < 1) newGamma = 1;
      if(newGamma > 1000) newGamma = 1000;
      RavlN::MutexLockC accessLock(m_accessMutex);
      if((ret = is_SetGamma(m_phf,newGamma)) != IS_SUCCESS) {
        RavlSysLog(SYSLOG_ERR) << "Failed to set gamma to " << attrValue << " Error:" << ret;
      }
      accessLock.Unlock();
      SignalAttrChange("gamma");
      return true;
    }
    if(attrName == "brightness") {
      int newBrightness = Round(attrValue * 255.0);
      if(newBrightness < 0) newBrightness = 0;
      if(newBrightness > 255) newBrightness = 255;
      RavlN::MutexLockC accessLock(m_accessMutex);
      if((ret = is_SetBrightness(m_phf,newBrightness)) != IS_SUCCESS) {
        RavlSysLog(SYSLOG_ERR) << "Failed to set brightness to " << attrValue << " Error:" << ret;
      }
      accessLock.Unlock();
      SignalAttrChange("brightness");
      return true;
    }
    if(attrName == "contrast") {
      int newContrast = Round(attrValue * 255.0);
      if(newContrast < 0) newContrast = 0;
      if(newContrast > 511) newContrast = 511;
      RavlN::MutexLockC accessLock(m_accessMutex);
      if((ret = is_SetContrast(m_phf,newContrast)) != IS_SUCCESS) {
        RavlSysLog(SYSLOG_ERR) << "Failed to set contrast to " << attrValue << " Error:" << ret;
      }
      accessLock.Unlock();
      SignalAttrChange("contrast");
      return true;
    }
    if(attrName == "framerate") {
      double framerate = attrValue;
      RavlN::MutexLockC accessLock(m_accessMutex);
      if((ret = is_SetFrameRate(m_phf,framerate,&framerate)) != IS_SUCCESS)
        RavlSysLog(SYSLOG_WARNING) << "Failed to set the framerate to " << framerate << " ";
      accessLock.Unlock();
      SignalAttrChange("framerate");
      SignalAttrChange("shutter_max");
      return true;
    }
    if(attrName == "timeout") {
      m_timeOutDelay = attrValue;
      SignalAttrChange("timeout");
      return true;
    }
    return false;
  }

  bool ImgIOuEyeBaseC::BuildAttributes(AttributeCtrlBodyC &attrCtrl)
  {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "Setting up attribute. ");
    int ret;
    RavlN::MutexLockC accessLock(m_accessMutex);

    // Information about the driver.
    attrCtrl.RegisterAttribute(AttributeTypeStringC("driver", "Name of driver", true, false,"ueye"));
    attrCtrl.RegisterAttribute(AttributeTypeStringC("card", "Capture card", true, false,"-?-"));

    // Image size.
    attrCtrl.RegisterAttribute(AttributeTypeNumC<IntT>("width",  "Width",  true, true,  1,m_sensorInfo.nMaxWidth,  1,  m_sensorInfo.nMaxWidth));
    attrCtrl.RegisterAttribute(AttributeTypeNumC<IntT>("height", "Height", true, true,  1,m_sensorInfo.nMaxHeight, 1, m_sensorInfo.nMaxHeight));

    // TODO: Check available modes.
    attrCtrl.RegisterAttribute(AttributeTypeNumC<IntT>("binning_vertical", "Binning level 1,2,4 pixels.", true, true,  1,4, 1, 1));
    attrCtrl.RegisterAttribute(AttributeTypeNumC<IntT>("binning_horizontal", "Binning level 1,2,4 pixels.", true, true,  1,4, 1, 1));

#if 0
    // TODO: Check available modes.
    attrCtrl.RegisterAttribute(AttributeTypeNumC<IntT>("subsample_vertical", "Binning level 1,2,4 pixels.", true, true,  1,4, 1, 1));
    attrCtrl.RegisterAttribute(AttributeTypeNumC<IntT>("subsample_horizontal", "Binning level 1,2,4 pixels.", true, true,  1,4, 1, 1));
#endif

    // Setup trigger modes.
    DListC<StringC> triggerList;
    for(int i = 0;g_triggerModeNames[i] != 0;i++)
      triggerList.InsLast(g_triggerModeNames[i]);
    attrCtrl.RegisterAttribute(AttributeTypeEnumC("trigger", "External triggering mode.", true, true, triggerList, triggerList.First()));

    attrCtrl.RegisterAttribute(AttributeTypeBoolC("trigger_soft", "Set off a software trigger", false, true,false));

    // Setup trigger delay.
    RealT minDelay = (RealT) is_SetTriggerDelay(m_phf,IS_GET_MIN_TRIGGER_DELAY) * 1e-6;
    RealT maxDelay = (RealT) is_SetTriggerDelay(m_phf,IS_GET_MAX_TRIGGER_DELAY) * 1e-6;
    RealT curDelay = (RealT) is_SetTriggerDelay(m_phf,IS_GET_TRIGGER_DELAY) * 1e-6;
    attrCtrl.RegisterAttribute(AttributeTypeNumC<RealT>("trigger_delay", "delay after trigger to capture the frame.", true, true, minDelay,maxDelay,0.00001,curDelay));

    // Sort out exposure time.
    double curExposure = 0;
    if((ret = is_SetExposureTime (m_phf, IS_GET_EXPOSURE_TIME,&curExposure)) == IS_SUCCESS) {
      RavlSysLog(SYSLOG_WARNING) << "Failed to get current exposure setting. ";
    }

    double minExp = 0,maxExp=100,intExp =0.0001;
    if((ret = is_GetExposureRange (m_phf, &minExp,&maxExp,&intExp)) != IS_SUCCESS) {
      RavlSysLog(SYSLOG_WARNING) << "Failed to get the exposure range. ";
    }
    // Change things to seconds.
    curExposure /= 1000.0;
    minExp /= 1000.0;
    maxExp /= 1000.0;
    intExp /= 1000.0;

    attrCtrl.RegisterAttribute(AttributeTypeNumC<RealT>("shutter_speed", "Shutter speed in seconds", true, true, minExp,maxExp,intExp,curExposure));

    attrCtrl.RegisterAttribute(AttributeTypeNumC<RealT>("shutter_min", "Minumum shutter time in seconds", true, false,0.000001,10.0,0.000001,minExp));
    attrCtrl.RegisterAttribute(AttributeTypeNumC<RealT>("shutter_step", "Shutter speed in seconds", true, false,0.0000001,10.0,0.000001,intExp));
    attrCtrl.RegisterAttribute(AttributeTypeNumC<RealT>("shutter_max", "Maximum shutter time in seconds", true, false,0.000001,10.0,0.000001,maxExp));

    // Hardware Gain
    double curGain;
    curGain = (RealT) is_SetHardwareGain(m_phf, IS_GET_MASTER_GAIN,IS_IGNORE_PARAMETER,IS_IGNORE_PARAMETER,IS_IGNORE_PARAMETER) / 100.0;
    attrCtrl.RegisterAttribute(AttributeTypeNumC<RealT>("gain", "Gain 0.0 to 1.0", true, true, 0,1,0.01,curGain));

    // Snapshot mode.
    attrCtrl.RegisterAttribute(AttributeTypeBoolC("snapshot", "Snapshot image capture, (for use with triggering)", true, true, m_snapshot));

    // Apply gamma correction ?
    bool gammaMode = is_SetHardwareGamma(m_phf,IS_GET_HW_GAMMA);
    attrCtrl.RegisterAttribute(AttributeTypeBoolC("hardware_gamma", "Use hardware gamma correction", true, true,gammaMode));

    RealT gammaValue = (RealT) is_SetGamma(m_phf,IS_GET_GAMMA) / 100.0;
    attrCtrl.RegisterAttribute(AttributeTypeNumC<RealT>("gamma", "Gamma correction", true, true, 0.01,10,0.01,gammaValue));

    // Brightness
    RealT defaultBrightness = (RealT) is_SetBrightness(m_phf,IS_GET_BRIGHTNESS) / 255.0;
    attrCtrl.RegisterAttribute(AttributeTypeNumC<RealT>("brightness", "Brightness as a fraction of available values", true, true, 0,1,0.01,defaultBrightness));

    // Contrast
    RealT defaultContrast = (RealT) is_SetContrast(m_phf,IS_GET_CONTRAST) / 255.0;
    attrCtrl.RegisterAttribute(AttributeTypeNumC<RealT>("contrast", "Contrast as a fraction of available values 0.0 to 2.0", true, true, 0,2,0.005,defaultContrast));

    // Frame rate
    double frameRate = 25;
    if((ret = is_SetFrameRate(m_phf,IS_GET_FRAMERATE,&frameRate)) != IS_SUCCESS)
      RavlSysLog(SYSLOG_WARNING) << "Failed to get the current framerate. ";
    attrCtrl.RegisterAttribute(AttributeTypeNumC<RealT>("framerate", "Frames per second", true, true, 0,160,0.1,frameRate));

    // Pixel clock
    RealT curPixelClock = is_SetPixelClock(m_phf,IS_GET_PIXEL_CLOCK);
    attrCtrl.RegisterAttribute(AttributeTypeNumC<RealT>("pixel_clock", "Pixel clock", true, true, 0,100e6,1e6,curPixelClock));

    // Auto shutter
    double v1 = 0,v2 = 0;
    if((ret = is_SetAutoParameter(m_phf,IS_GET_ENABLE_AUTO_SHUTTER,&v1,&v2)) != IS_SUCCESS) {
      RavlSysLog(SYSLOG_WARNING) << "Failed to get auto shutter state. ";
    }
    bool autoShutter = v1 > 0;;
    attrCtrl.RegisterAttribute(AttributeTypeBoolC("auto_shutter", "Automaticly set the shutter speed", true, true,autoShutter));

    // Auto gain
    v1 = 0;
    if((ret = is_SetAutoParameter(m_phf,IS_GET_ENABLE_AUTO_GAIN,&v1,&v2)) != IS_SUCCESS) {
      RavlSysLog(SYSLOG_WARNING) << "Failed to get auto gain state. ";
    }
    bool autoGain = v1 > 0;
    attrCtrl.RegisterAttribute(AttributeTypeBoolC("auto_gain", "Automaticly set gain", true, true,autoGain));

    // gain boost
    ret = is_SetGainBoost(m_phf, IS_GET_GAINBOOST);
    attrCtrl.RegisterAttribute(AttributeTypeBoolC("gain_boost", "Gain boost", true, true, ret == IS_SET_GAINBOOST_ON));

    // Auto framerate
    v1 = 0;
    if((ret = is_SetAutoParameter(m_phf,IS_GET_ENABLE_AUTO_FRAMERATE,&v1,&v2)) != IS_SUCCESS) {
      RavlSysLog(SYSLOG_WARNING) << "Failed to get auto framerate state. ";
    }
    bool autoFramerate = v1 > 0;
    attrCtrl.RegisterAttribute(AttributeTypeBoolC("auto_framerate", "Automaticly set framerate", true, true,autoFramerate));

    // Timeout delay
    attrCtrl.RegisterAttribute(AttributeTypeNumC<RealT>("timeout", "Timeout period for image capture", true, true, 0.01,100,0.01,m_timeOutDelay));

    // Setup rotation modes.
    DListC<StringC> rotationList;
    for(int i = 0; i < 360; i += 90)
      rotationList.InsLast(StringC(i));
    attrCtrl.RegisterAttribute(AttributeTypeEnumC("rotation", "Image rotation.", true, true, rotationList, rotationList.First()));
    attrCtrl.RegisterAttribute(AttributeTypeBoolC("trigger_soft", "Set off a software trigger", false, true,false));

#ifdef IS_GET_SUPPORTED_GLOBAL_SHUTTER
    // Global shutter
    ret = is_SetGlobalShutter(m_phf, IS_GET_SUPPORTED_GLOBAL_SHUTTER);
    if(ret != IS_NOT_SUPPORTED) {
      ret = is_SetGlobalShutter(m_phf, IS_GET_GLOBAL_SHUTTER);
      attrCtrl.RegisterAttribute(AttributeTypeBoolC("global_shutter", "Is global shutter enabled.", true, true, ret == IS_SET_GLOBAL_SHUTTER_ON));
    } else {
      RavlSysLog(SYSLOG_DEBUG) << "Global shutter return code:" << (int) ret << ". \n";
    }
#endif

    // Measure time until all rows are open with current setup.
    attrCtrl.RegisterAttribute(AttributeTypeNumC<RealT>("global_flash_delay", "When using a rolling shutter time until all rows are open with the current pixel clock.", true, false, 0.0,1.0,0.000001,0));
    attrCtrl.RegisterAttribute(AttributeTypeNumC<RealT>("global_flash_duration", "When using a rolling shutter this is the length of time all rows are open with the current pixel clock.", true, false, 0.0,1.0,0.000001,0));


    return true;
  }


}
