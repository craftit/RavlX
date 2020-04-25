// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImgIOV4L
//! file="Ravl/Contrib/V4L/ImgIOV4LAux.cc"

typedef unsigned long ulong;

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "Ravl/Image/ImgIOV4L.hh"

#include <linux/videodev.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

struct led_ctrl {
  int on;
  int off;
};

struct wb_ctrl {
  // Mode
  int mode;
  // Write
  int red;
  int blue;
  // Read
  int ored;
  int oblue;
};

struct pt_ctrl
{
	int absolute;		/* write-only */
	int pan;		/* degrees * 100 */
	int tilt;		/* degress * 100 */
};

// Restore user settings 
#define PHILIPS_LOAD_SETTINGS		_IO('v', 192)

// Save user settings
#define PHILIPS_SAVE_SETTINGS		_IO('v', 193)

// Factory Reset
#define PHILIPS_FACTORY_RESET _IO('v', 194)

// Desired Compression 
#define PHILIPS_GET_COMPRESSION _IOR('v', 195, int)
#define PHILIPS_SET_COMPRESSION _IOW('v', 195, int)

// AGC setting.
#define PHILIPS_GET_AGC         _IOR('v', 200, int)
#define PHILIPS_SET_AGC         _IOW('v', 200, int)

// Shutter 
#define PHILIPS_SET_SHUTTER	_IOW('v', 201, int)

// White Balance
#define PHILIPS_SET_WHITEBALANCE  _IOW('v', 202, struct wb_ctrl)
#define PHILIPS_GET_WHITEBALANCE  _IOR('v', 202, struct wb_ctrl)

// LED control
#define PHILIPS_SET_LED   _IOW('v', 205, struct led_ctrl)
#define PHILIPS_GET_LED   _IOR('v', 205, struct led_ctrl)

// Sharpness setting
#define PHILIPS_SET_SHARPNESS	_IOW('v', 206, int)
#define PHILIPS_GET_SHARPNESS	_IOR('v', 206, int)

// Backlight 0=Off 1=On
#define PHILIPS_SET_BACKLIGHT	_IOW('v', 207, int)
#define PHILIPS_GET_BACKLIGHT	_IOR('v', 207, int)

// 0=Off 1=On
#define PHILIPS_SET_FLICKER	_IOW('v', 208, int)
#define PHILIPS_GET_FLICKER	_IOR('v', 208, int)  

//  0=Off 3=Full
#define PHILIPS_SET_NOISEREDUCTION	_IOW('v', 209, int)
#define PHILIPS_GET_NOISEREDUCTION	_IOR('v', 209, int)

// Image size
#define PHILIPS_GET_REALSIZE _IOR('v', 210, struct led_ctrl)

// Pan/Tilt
#define PHILIPS_SET_ANGLE	_IOW('v', 212, struct pt_ctrl)
#define PHILIPS_GET_ANGLE	_IOR('v', 212, struct pt_ctrl)

namespace RavlImageN {
  
  //: Set shutter speed 0..65536 -1=Auto.
  
  bool DPIImageBaseV4LBodyC::SetShutterSpeed(int speed) {
    switch(sourceType) {
    case SOURCE_USBWEBCAM_PHILIPS: {
      if(ioctl(fd,PHILIPS_SET_SHUTTER,&speed) < 0) {
	cerr<< "WARNING: Failed to set shutter speed \n";
	return false;
      }
      shutterSpeed = speed;
    } return true; 
    default: break;
    }
    return false;
  }
  
  //: Set brightness speed 0..65536 -1=Auto.
  
  bool DPIImageBaseV4LBodyC::SetBrightness(int brightness) {
    struct video_picture vpic;
    if(ioctl(fd,VIDIOCGPICT,&vpic) < 0) {
      cerr<< "WARNING: Failed to get video_picture. \n";
      return false;
    }
    if(brightness < 0)
      return false; // Don't know what the default is.
    vpic.brightness = brightness;
    if(ioctl(fd,VIDIOCSPICT,&vpic) < 0) {
      cerr<< "WARNING: Failed to set brightness \n";
      return false;
    }
    return true;
  }
  
  //: Set contrast speed 0..65536 -1=Auto.
  
  bool DPIImageBaseV4LBodyC::SetContrast(int contrast) {
    struct video_picture vpic;
    if(ioctl(fd,VIDIOCGPICT,&vpic) < 0) {
      cerr<< "WARNING: Failed to get video_picture. \n";
      return false;
    }
    if(contrast < 0)
      return false; // Don't know what the default is.
    vpic.contrast = contrast;
    if(ioctl(fd,VIDIOCSPICT,&vpic) < 0) {
      cerr<< "WARNING: Failed to set contrast \n";
      return false;
    }
    return true;
  }
  
  //: Get shutter speed 0..65536 -1=Auto.
  
  bool DPIImageBaseV4LBodyC::GetShutterSpeed(int &speed) {
    speed = shutterSpeed;
    return true;
  }
  
  //: Get brightness speed 0..65536 -1=Auto.
  
  bool DPIImageBaseV4LBodyC::GetBrightness(int &brightness) {
    struct video_picture vpic;
    if(ioctl(fd,VIDIOCGPICT,&vpic) < 0) {
      cerr<< "WARNING: Failed to get video_picture. \n";
      return false;
    }
    brightness = vpic.brightness;
    return true;
  }
  
  //: Get contrast speed 0..65536 -1=Auto.
  
  bool DPIImageBaseV4LBodyC::GetContrast(int &contrast) {
    struct video_picture vpic;
    if(ioctl(fd,VIDIOCGPICT,&vpic) < 0) {
      cerr<< "WARNING: Failed to get video_picture. \n";
      return false;
    }
    contrast = vpic.contrast;
    return true;
  }
  
  //: Set gama 0..65536 -1=Auto.
  
  bool DPIImageBaseV4LBodyC::SetGamma(int gamma) {
    switch(sourceType) {
    case SOURCE_USBWEBCAM_PHILIPS: {
      struct video_picture vpic;
      if(ioctl(fd,VIDIOCGPICT,&vpic) < 0) {
	cerr<< "WARNING: Failed to get video_picture. \n";
	return false;
      }
      vpic.whiteness = gamma;
      if(ioctl(fd,VIDIOCSPICT,&vpic) < 0) {
	cerr<< "WARNING: Failed to set gamma \n";
	return false;
      }
    } return true; 
    default: break;
    }
    return false;    
  }

  //: Set gama 0..65536 -1=Auto.
  
  bool DPIImageBaseV4LBodyC::GetGamma(int &gamma) {
    switch(sourceType) {
    case SOURCE_USBWEBCAM_PHILIPS: {
      struct video_picture vpic;
      if(ioctl(fd,VIDIOCGPICT,&vpic) < 0) {
	cerr<< "WARNING: Failed to get video_picture. \n";
	return false;
      }
      gamma = vpic.whiteness;
    } return true; 
    default: break;
    }
    return false;
  }

  //: Set compression speed 0..100 
  
  bool DPIImageBaseV4LBodyC::SetCompression(int compression) {
    switch(sourceType) {
    case SOURCE_USBWEBCAM_PHILIPS: {
      int comp = compression / 25;
      if(ioctl(fd,PHILIPS_SET_COMPRESSION,&comp) < 0) {
	cerr<< "WARNING: Failed to set compression. \n";
	return false;
      }
    } return true; 
    default: break;
    }
    return false;
  }

  //: Get compression speed 0..100 
  
  bool DPIImageBaseV4LBodyC::GetCompression(int &compression) {
    switch(sourceType) {
    case SOURCE_USBWEBCAM_PHILIPS: {
      if(ioctl(fd,PHILIPS_GET_COMPRESSION,&compression) < 0) {
	cerr<< "WARNING: Failed to get compression. \n";
	return false;
      }
      compression *= 25;
    } return true; 
    default: break;
    }
    return false;    
  }
  
  //: Set agc 0..65536 -1=off.
  
  bool DPIImageBaseV4LBodyC::SetAGC(int agc) {
    switch(sourceType) {
    case SOURCE_USBWEBCAM_PHILIPS: {
      if(ioctl(fd,PHILIPS_SET_AGC,&agc) < 0) {
	cerr<< "WARNING: Failed to set agc. \n";
	return false;
      }
    } return true; 
    default: break;
    }    
    return false;
  }

  //: Get agc 0..65536 -1=off.
  
  bool DPIImageBaseV4LBodyC::GetAGC(int &agc) {
    switch(sourceType) {
    case SOURCE_USBWEBCAM_PHILIPS: {
      if(ioctl(fd,PHILIPS_GET_AGC,&agc) < 0) {
	cerr<< "WARNING: Failed to get agc. \n";
	return false;
      }
    } return true; 
    default: break;
    }    
    return false;
  }

  //: Set LED 0=Off -1=On 1-65535 sets flash rate.
  
  bool DPIImageBaseV4LBodyC::SetLED(int on) {
    switch(sourceType) {
    case SOURCE_USBWEBCAM_PHILIPS: {
      led_ctrl val;
      val.on = ((UIntT)on)  & 0xffff;
      val.off = (((UIntT)on) >> 16) & 0xffff;
      if(ioctl(fd,PHILIPS_SET_LED,&val) < 0) {
	cerr<< "WARNING: Failed to set LED. \n";
	return false;
      }
    } return true; 
    default: break;
    }    
    return true;
  }


  //: Save camera settings.
  // (Philips webcams.)
  
  bool DPIImageBaseV4LBodyC::SaveUserSettings() {
    switch(sourceType) {
    case SOURCE_USBWEBCAM_PHILIPS: {
      if(ioctl(fd,PHILIPS_LOAD_SETTINGS) < 0) {
	cerr<< "WARNING: Save user settings failed. \n";
	return false;
      }
    } return true; 
    default: break;
    }    
    return false;    
  }
  
  //: Save camera settings.
  // (Philips webcams.)
  
  bool DPIImageBaseV4LBodyC::LoadUserSettings() {
    switch(sourceType) {
    case SOURCE_USBWEBCAM_PHILIPS: {
      if(ioctl(fd,PHILIPS_LOAD_SETTINGS) < 0) {
	cerr<< "WARNING: Load user settings failed. \n";
	return false;
      }
    } return true; 
    default: break;
    }    
    return false;
  }
  
  
  //: Factory reset
  
  bool DPIImageBaseV4LBodyC::SetReset() {
    switch(sourceType) {
    case SOURCE_USBWEBCAM_PHILIPS: {
      if(ioctl(fd,PHILIPS_FACTORY_RESET) < 0) {
	cerr<< "WARNING: Reset failed. \n";
	return false;
      }
    } return true; 
    default: break;
    }    
    return false;
  }
  
  //: Set white balance
  
  bool DPIImageBaseV4LBodyC::SetWhiteBalance(int mode) {
    switch(sourceType) {
    case SOURCE_USBWEBCAM_PHILIPS: {
      wb_ctrl val;
      if(ioctl(fd,PHILIPS_GET_WHITEBALANCE,&val) < 0) {
	cerr << "WARNING: Failed to get white balance mode. \n";
	return false;
      }
      val.mode = mode;
      if(ioctl(fd,PHILIPS_SET_WHITEBALANCE,&val) < 0) {
	cerr << "WARNING: Failed to set white balance mode. \n";
	return false;
      }
    } return true; 
    default: break;
    }    
    return true;
  }

  //: Get white balance
  
  bool DPIImageBaseV4LBodyC::GetWhiteBalance(int& mode) {
    switch(sourceType) {
    case SOURCE_USBWEBCAM_PHILIPS: {
      wb_ctrl val;
      if(ioctl(fd,PHILIPS_GET_WHITEBALANCE,&val) < 0) {
	cerr<< "WARNING: Failed to get white balance mode. \n";
	return false;
      }
      mode = val.mode;
    } return true; 
    default: break;
    }    
    return true;
  }

  //: Set camera pan
  
  bool DPIImageBaseV4LBodyC::SetPan(int pan) {
    switch(sourceType) {
    case SOURCE_USBWEBCAM_PHILIPS: {
      pt_ctrl val;
      val.absolute = 1;
      if(ioctl(fd,PHILIPS_GET_ANGLE,&val) < 0) {
	cerr << "WARNING: Failed to get pan. \n";
	return false;
      }
      val.pan = pan;
      if(ioctl(fd,PHILIPS_SET_ANGLE,&val) < 0) {
	cerr << "WARNING: Failed to set pan. \n";
	return false;
      }
    } return true; 
    default: break;
    }    
    return false;
  }

  //: Get camera pan
  
  bool DPIImageBaseV4LBodyC::GetPan(int &pan) {
    switch(sourceType) {
    case SOURCE_USBWEBCAM_PHILIPS: {
      pt_ctrl val;
      val.absolute = 1;
      if(ioctl(fd,PHILIPS_GET_ANGLE,&val) < 0) {
	cerr << "WARNING: Failed to get pan. \n";
	return false;
      }
      pan = val.pan;
    } return true; 
    default: break;
    }    
    return false;
  }


  //: Set camera tilt
  
  bool DPIImageBaseV4LBodyC::SetTilt(int tilt) {
    switch(sourceType) {
    case SOURCE_USBWEBCAM_PHILIPS: {
      pt_ctrl val;
      val.absolute = 1;
      if(ioctl(fd,PHILIPS_GET_ANGLE,&val) < 0) {
	cerr << "WARNING: Failed to get tilt. \n";
	return false;
      }
      val.tilt = tilt;
      if(ioctl(fd,PHILIPS_SET_ANGLE,&val) < 0) {
	cerr << "WARNING: Failed to set tilt. \n";
	return false;
      }
    } return true; 
    default: break;
    }    
    return true;
  }

  //: Set camera pan
  
  bool DPIImageBaseV4LBodyC::GetTilt(int &tilt) {
    switch(sourceType) {
    case SOURCE_USBWEBCAM_PHILIPS: {
      pt_ctrl val;
      val.absolute = 1;
      if(ioctl(fd,PHILIPS_GET_ANGLE,&val) < 0) {
	cerr << "WARNING: Failed to get tilt. \n";
	return false;
      }
      tilt = val.tilt;
    } return true; 
    default: break;
    }    
    return true;
  }

  //: Get padding flag.
  
  bool DPIImageBaseV4LBodyC::GetPad(int &pad) {
    
    pad = 0;
    switch(sourceType) {
    case SOURCE_USBWEBCAM_PHILIPS: {
      led_ctrl val;
      if(ioctl(fd,PHILIPS_GET_REALSIZE,&val) < 0) {
	cerr<< "WARNING: Failed to get realsize. \n";
	return false;
      }
      cerr << "DPIImageBaseV4LBodyC::GetPad(), Width=" << val.on << " Height=" << val.off << "\n";
      cerr << "DPIImageBaseV4LBodyC::GetPad(), Rect=" << rect << "\n";
      pad = 0;
    } return true; 
    default: break;
    }    
    return false;
  }


  //: Setup a philips webcam.
  
  bool DPIImageBaseV4LBodyC::SetupCamera() {
    switch(sourceType) {
    case SOURCE_USBWEBCAM_PHILIPS:{
#if 0
      ONDEBUG(cerr << "DPIImageBaseV4LBodyC::SetupCamera(), Called \n");
      int zero = 0;
      int neg = -1;
      ioctl(fd,PHILIPS_SET_SHARPNESS,neg);
      ioctl(fd,PHILIPS_SET_BACKLIGHT,zero);
      ioctl(fd,PHILIPS_SET_FLICKER,zero);
      ioctl(fd,PHILIPS_SET_NOISEREDUCTION,zero);
#endif
    } break;
    case SOURCE_UNKNOWN: 
      break;
    }

    return true;
  }

}
