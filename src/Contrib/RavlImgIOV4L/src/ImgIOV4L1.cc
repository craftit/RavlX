// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImgIOV4L
//! file="Ravl/Contrib/V4L/ImgIOV4L1.cc"

typedef unsigned long ulong;


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "Ravl/Image/ImgIOV4L.hh"

#include <sys/mman.h>

#include <linux/videodev.h>

#define USE_MMAP 0

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {

  //: Open a meteor device.
  
  bool DPIImageBaseV4LBodyC::Open1(int nfd,const type_info &npixType,const ImageRectangleC &nrect) {
    fd = nfd;
    int reterr;
    struct video_capability vidcap;
    if((reterr = ioctl(fd,VIDIOCGCAP,&vidcap)) < 0) {
      cerr << "Failed to get video capabilities. \n";
      return false;
    }
    ONDEBUG(cerr << "Video type '" << vidcap.name << "' Type=" << vidcap.type << "\n");
    ONDEBUG(cerr << "Channels=" << vidcap.channels << " Audios=" << vidcap.audios << "\n");
    ONDEBUG(cerr << "MaxWidth=" << vidcap.maxwidth << " MaxHeight=" << vidcap.maxheight << "\n");
    ONDEBUG(cerr << "MinWidth=" << vidcap.minwidth << " MinHeight=" << vidcap.minheight << "\n");
    
    if(!(vidcap.type | VID_TYPE_CAPTURE)) {
      cerr << "ERROR: Not a capture device. \n";
      return false;
    }
    
    sourceType = SOURCE_UNKNOWN;
    //int camtype;
    ONDEBUG(cerr << " Source name = '" << vidcap.name << "'. \n");
    if(strncmp(vidcap.name,"Philips ",8) == 0 ||
       strncmp(vidcap.name,"Logitech QuickCam Orbit",23) == 0) {
      ONDEBUG(cerr << "Got philips webcam. \n");
      sourceType = SOURCE_USBWEBCAM_PHILIPS;
    }
    // Setup capture mode.
    
    struct video_picture vidpic;
    if(ioctl(fd,VIDIOCGPICT,&vidpic) < 0) {
      cerr << "Failed to get video picture paramiters. \n";
      return false;
    }

    // Set channel.
    if(channel >= 0) {
      if(vidcap.channels > channel) {
	struct video_channel vidchannel;
	vidchannel.channel=channel;
	if(ioctl(fd,VIDIOCGCHAN,&vidchannel) < 0) {
	  cerr << "Failed to get video channel paramiters. \n";
	  return false;
	}
	ONDEBUG(cerr << "Channel " << vidchannel.channel << " Name=" << vidchannel.name << " Norm=" << vidchannel.norm << " Type=" << vidchannel.type << " Flags=" << vidchannel.flags << "\n");
	// Should check the channel properties.
	vidchannel.channel=channel;
	if(ioctl(fd,VIDIOCSCHAN,&vidchannel) < 0) {
	  cerr << "Failed to set video channel paramiters. \n";
	  return false;
	}
      } else
	cerr << "Video device has no channel number " << channel << "\n";
    }
    if(npixType == typeid(ByteYUV422ValueC)) {
      vidpic.palette = VIDEO_PALETTE_UYVY;
      if(ioctl(fd,VIDIOCSPICT,&vidpic) < 0) {
	vidpic.palette = VIDEO_PALETTE_YUYV;
	if(ioctl(fd,VIDIOCSPICT,&vidpic) < 0) {
	  vidpic.palette = VIDEO_PALETTE_YUV420P;
	  if(ioctl(fd,VIDIOCSPICT,&vidpic) < 0) {
	    cerr << "Failed to set video picture paramiters. \n";
	    return false;
	  }
	}
      }
    } else if(npixType == typeid(ByteYUVValueC)) {
      vidpic.palette = VIDEO_PALETTE_UYVY;
      if(ioctl(fd,VIDIOCSPICT,&vidpic) < 0) {
	vidpic.palette = VIDEO_PALETTE_YUYV;
	if(ioctl(fd,VIDIOCSPICT,&vidpic) < 0) {
	  vidpic.palette = VIDEO_PALETTE_YUV420P;
	  if(ioctl(fd,VIDIOCSPICT,&vidpic) < 0) {
	    cerr << "Failed to set video picture paramiters. \n";
	    return false;
	  }
	}
      }
    } else if(npixType == typeid(ByteRGBValueC)) {
      vidpic.palette = VIDEO_PALETTE_RGB24 | 0x80;
      if(ioctl(fd,VIDIOCSPICT,&vidpic) < 0) {
	vidpic.palette = VIDEO_PALETTE_RGB24;
	if(ioctl(fd,VIDIOCSPICT,&vidpic) < 0) {
	  vidpic.palette = VIDEO_PALETTE_YUV420P;
	  if(ioctl(fd,VIDIOCSPICT,&vidpic) < 0) {
	    cerr << "Failed to set video picture paramiters. \n";
	    return false;	
	  }
	}
      }
    } else if(npixType == typeid(ByteT)) {
      vidpic.palette = VIDEO_PALETTE_GREY;
      if(ioctl(fd,VIDIOCSPICT,&vidpic) < 0) {
	vidpic.palette = VIDEO_PALETTE_YUV420P;
	if(ioctl(fd,VIDIOCSPICT,&vidpic) < 0) {
	  cerr << "Failed to set video picture paramiters. \n";
	  return false;	
	}
      }
    } else {
      cerr << "Unsupported pixel type " << npixType.name() << "\n";
      return false;
    }
    if(ioctl(fd,VIDIOCGPICT,&vidpic)) {
      cerr << "Failed to get video mode. \n";
      return false;
    }
    palette = vidpic.palette;
#if DODEBUG
    switch(palette) {
    case VIDEO_PALETTE_RGB24: 
      cerr << "Using pallete VIDEO_PALETTE_RGB24 \n";
      break;
    case VIDEO_PALETTE_RGB24 | 0x80:
      cerr << "Using pallete VIDEO_PALETTE_BGR24 \n";
      break;
    case VIDEO_PALETTE_YUYV:
      cerr << "Using pallete VIDEO_PALETTE_YUYV \n";
      break;
    case VIDEO_PALETTE_UYVY:
      cerr << "Using pallete VIDEO_PALETTE_UYVY \n";
      break;
    case VIDEO_PALETTE_YUV420P:
      cerr << "Using pallete VIDEO_PALETTE_YUV420P \n";
      break;
    default:
      cerr << "Using pallete " << palette << "\n";
    }
#endif
    
    struct video_window vidwin;
    vidwin.x = 0;
    vidwin.y = 0;
    if(!half) {
#if 1
      vidwin.width = vidcap.maxwidth;
      vidwin.height = vidcap.maxheight;
#else
      vidwin.width = 768;
      vidwin.height = 576;
#endif
    } else {
      vidwin.width = vidcap.maxwidth /2;
      vidwin.height = vidcap.maxheight/2;
    }
    vidwin.chromakey = 0;
    vidwin.flags = 0;
    vidwin.clips = 0;
    vidwin.clipcount = 0;
    
    if(ioctl(fd,VIDIOCSWIN,&vidwin) < 0) {    
      cerr << "WARNING: Failed to set video window paramiters. \n";
    }
    if(ioctl(fd,VIDIOCGWIN,&vidwin) < 0) {    
      cerr << "WARNING: Failed to get video window paramiters. \n";
    }
    ONDEBUG(cerr << "Capture:\n");
    ONDEBUG(cerr << "X=" << vidwin.x << " Y=" << vidwin.y << "\n");
    ONDEBUG(cerr << "Width=" << vidwin.width << " Height=" << vidwin.height << "\n");
    rect = ImageRectangleC(0,vidwin.height-1,
			   0,vidwin.width -1
			   );

    SetupCamera();
    
#if USE_MMAP
    // Attempt to use memory map.
    buffer = 0;
    struct video_mbuf mbuf;
    bufNo = 0;
    if(ioctl(fd,VIDIOCGMBUF,&mbuf) >= 0) {
      ONDEBUG(cerr << "video_mbuf Size=" << mbuf.size << " Frames=" << mbuf.frames << " \n");
      frameOffsets = SArray1dC<int>(mbuf.frames);
      for(int i = 0;i < mbuf.frames;i++) {
	frameOffsets[i] = mbuf.offsets[i];
	ONDEBUG(cerr << "Offset off=" << frameOffsets[i] << "\n");
      }
      bufLen = mbuf.size;
      buffer = (ByteT *) mmap(0,bufLen,PROT_READ |PROT_WRITE,MAP_PRIVATE,fd,0);
      ONDEBUG(cerr << "Buffer @ " << ((void *) buffer) << " Length=" << bufLen << "\n");
      buf_grey=buffer;
      memmap = true;
      if(buffer == MAP_FAILED) {
	perror("Failed to map video device.");
	//cerr << "Failed to map memory. Errno=" << errno << "\n";
	return false;
      }
      for(int j = 0;j < mbuf.frames;j++) {
	struct video_mmap vmmap;
	vmmap.frame = j;
	vmmap.height = rect.Rows();
	vmmap.width = rect.Cols();
	vmmap.format = palette;
	ONDEBUG(cerr << "mmap Width=" << vmmap.width << " Height=" << vmmap.height << " Palette=" << palette << "\n");
	int rret;
	if((rret = ioctl(fd,VIDIOCMCAPTURE,&vmmap)) < 0) {
	  cerr << "Failed to start memory mapped capture. " << rret << " Errno=" << errno << "\n";
	}
      }
    } 
#endif
    
    if(!memmap) {
      switch(palette) {
      case VIDEO_PALETTE_YUV420P: {
	int area2 = rect.Area()/4;
	buf_grey = new ByteT [rect.Area() + area2 * 2];
	buf_u = &buf_grey[rect.Area()];
	  buf_v = &buf_u[area2];
      } break;
      case VIDEO_PALETTE_UYVY:
      case VIDEO_PALETTE_YUYV:
	buf_grey = new ByteT [rect.Area() * 2];
	buf_u = 0;
	buf_v = 0;
	break;
      default:
	buf_grey = 0;
	buf_u = 0;
	buf_v = 0;
	break;
      }
    }
    return true;
  }
  
}
