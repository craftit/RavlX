// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlImgIOMeteor1
//! author="Charles Galambos"
//! file="Ravl/Contrib/Meteor1/ImgIOMeteor1.cc"

#include "Ravl/Image/ImgIOMeteor1.hh"
#include "Ravl/Image/ByteBGRAValue.hh"
#include "Ravl/TypeName.hh"
#include <stdio.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include "ioctl_meteor.h"
#include "ioctl_himemfb.h"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

#define USE_MMAP 1

namespace RavlImageN {
  //: Constructor.
  
  DPIImageBaseMeteor1BodyC::DPIImageBaseMeteor1BodyC(const StringC &dev,const type_info &npixType,const ImageRectangleC &nrect)
    : rect(nrect),
#if 1
      sourceType(VidSrc_PAL),
#else
      sourceType(VidSrc_NTSC),
#endif
      bufFormat(0),
      bufFrames(1),
      outFormat(0),
      fd(-1),
      fbfd(-1),
      mmbuf(0),
      frameCount(0)
  {
    if(!Open(dev,npixType,nrect))
      cerr << "DPIImageBaseMeteor1BodyC::DPIImageBaseMeteor1BodyC(), Failed to  open '" << dev << "'\n";
    //DumpParam(cerr);
  }
  
  //: Destructor.
  
  DPIImageBaseMeteor1BodyC::~DPIImageBaseMeteor1BodyC()
  { Close(); }
  
  //: Get paramiter value
  
  int DPIImageBaseMeteor1BodyC::GetParam(VideoParamT pr) const {
    unsigned char val = 0;
    int op = 0;
    switch(pr)
      {
      case VP_HUE:        op = METEORGHUE;  break;
      case VP_CHROMA_GAIN:op = METEORGCHCV; break;
      case VP_BRIGHTNESS: op = METEORGBRIG; break;
      case VP_CHROMA_SAT: op = METEORGCSAT; break;
      case VP_CONTRAST:   op = METEORGCONT; break;
      default:
	cerr << "DPIImageBaseMeteor1BodyC::GetParam(), Unknown paramiter type " << pr << " \n";
	return -1;
      }
    if(ioctl(fd, op, &val) < 0) {
      cerr << "DPIImageBaseMeteor1BodyC::GetParam(), ioctl failed for " << pr << " Errno:" << errno << "\n";
      return -1;
    }
    return val;
  }
  
  //: Setup paramiter value.
  
  bool DPIImageBaseMeteor1BodyC::SetParam(VideoParamT pr,int nval) {
    unsigned char val = nval;
    int op = 0;
    switch(pr)
      {
      case VP_HUE:        op = METEORSHUE;  break;
      case VP_CHROMA_GAIN:op = METEORSCHCV; break;
      case VP_BRIGHTNESS: op = METEORSBRIG; break;
      case VP_CHROMA_SAT: op = METEORSCSAT; break;
      case VP_CONTRAST:   op = METEORSCONT; break;
      default:
	return false;
      }
    if(ioctl(fd, op, &val) < 0) {
      cerr << "DPIImageBaseMeteor1BodyC::SetParam(), ioctl failed for " << pr << " Errno:" << errno << "\n";
      return false;
    }
    return true;
  }
  
  //: Get the name of each paramiter.
  
  const char *DPIImageBaseMeteor1BodyC::ParamName(VideoParamT pr) const {
    const char *prname = "Unknown";
    switch(pr)
      {
      case VP_HUE:        prname = "Hue       "; break;
      case VP_CHROMA_GAIN:prname = "ChromaGain"; break;
      case VP_BRIGHTNESS: prname = "Brighness "; break;
      case VP_CHROMA_SAT: prname = "ChromaSat "; break;
      case VP_CONTRAST:   prname = "Contrast  "; break;
      case VP_END: prname="ERROR"; break;
      }
    return prname;
  }
  
  //: Dump current settings to 'out'.
  
  void DPIImageBaseMeteor1BodyC::DumpParam(ostream &out) const {
    
    for(int i = 0;i < VP_END;i++) {
      cerr << "Param:" << i << " " << ParamName((VideoParamT)i) << " = " << hex << GetParam((VideoParamT)i) << dec <<"\n";
    }
  }
  
  //: Open a meteor device.
  
  bool DPIImageBaseMeteor1BodyC::Open(const StringC &dev,const type_info &npixType,const ImageRectangleC &nrect) {
    
    devName = dev;
    //struct meteor_counts cnt;
    struct meteor_geomet geo;
    
    int fmtType = 0;
    if ((fd = open(devName, O_RDONLY)) < 0) {
      perror("open failed");
      cerr << "Meteor Driver: Failed to open device '" << devName <<"' \n";
      return false;
    }
    if((fbfd = open("/dev/himemfb", O_RDWR)) >= 0) { // Use /dev/himem driver ?
      ONDEBUG(cerr << "DPIImageBaseMeteor1BodyC::Open(), Using '/dev/himemfb'. \n");
      u_char  *phys_fb_addr = 0;
      struct meteor_fbuf vid;
      unsigned int hm_size;
      if (ioctl(fbfd, HM_GETADDR, &phys_fb_addr) < 0) {
	perror("ioctl(HM_GETADDR)");
	return false;
      }
      if (ioctl(fbfd, HM_GETSIZE, &hm_size) < 0) {
	perror("ioctl(HM_GETSIZE)");
	return false;
      }
      vid.addr = (u_long)phys_fb_addr;
      vid.width = 0;
      vid.ramsize = hm_size;
      if (ioctl(fd, METEORSVIDEO, &vid) < 0) {
	perror("ioctl(METEORSVIDEO)");
	return false;
      }
    }
    
    geo.frames = bufFrames;
    useMemMap = true;
    //useMemMap = false;
    
    switch(sourceType) 
      {
      case VidSrc_PAL:
	fmtType = METEOR_FMT_PAL;
#if 0
	geo.rows = 576;
	geo.columns = 768;	
#else
	geo.rows = 480;
	geo.columns = 640;	
#endif
#if 0
	SetParam(VP_CHROMA_GAIN,0x59);
	SetParam(VP_HUE,0);
	SetParam(VP_BRIGHTNESS,0x80);
	SetParam(VP_CHROMA_SAT,0x40);
	SetParam(VP_CONTRAST,0x40);
#endif
	break;
      case VidSrc_SECAM:
	fmtType = METEOR_FMT_SECAM;
	geo.rows = 576;
	geo.columns = 768;
	break;
      case VidSrc_NTSC:
	fmtType = METEOR_FMT_NTSC;
	geo.rows = 480;
	geo.columns = 640;
	break;
      default:
	cerr << "DPIImageBaseMeteor1BodyC::Open(), Unknown format. \n";
	return false;
      }
    
    maxRect = ImageRectangleC(0,geo.rows-1,0,geo.columns-1);
    if(rect.Cols() <= 0 || rect.Rows() <= 0)
      rect = maxRect;
    
    //: Can we handle this pixel type ?
    
    if(npixType == typeid(ByteBGRAValueC)) {
      geo.oformat = METEOR_GEO_RGB24;
      bufFrameSize = maxRect.Area() * sizeof(ByteBGRAValueC);
      useMemMap = false; // Use read, its faster.
      outFormat = 0;     // Use plain copy.
    } else if(npixType == typeid(ByteRGBValueC)) {
      geo.oformat = METEOR_GEO_RGB24;
      bufFrameSize = maxRect.Area() * sizeof(ByteBGRAValueC);
      useMemMap = true; // Use read, its faster.
      outFormat = 1;     // Use converting copy.
    } else {
      cerr << "DPIImageBaseMeteor1BodyC::Open(), Can't handle pixel type '" << TypeName(npixType) << "'\n";
      return false;
    }
    
    ONDEBUG(cerr << "Meteor: Setting up for Size:" << geo.rows << "x" << geo.columns << " Frames:" << geo.frames <<" oformat:" << hex << geo.oformat << dec << "\n"); 
    ONDEBUG(cerr << "Meteor: Capture rectangle :" << rect << " of  " << maxRect << "\n");
    
    // Setup geometry
    if (ioctl(fd, METEORSETGEO, &geo) < 0) {
      perror("ioctl SetGeometry failed");
      Close();
      return false;
    }
    
#if 0
    struct meteor_frame_offset off;
    if (ioctl(fd, METEORGFROFF, &off) < 0) {
      perror("ioctl FrameOffset failed");
      Close();
      return false;
    }
#endif
    
    if (ioctl(fd, METEORSFMT, &fmtType) < 0) {
      perror("ioctl SetFormat failed");
      Close();
      return false;
    }
    
    int c = METEOR_INPUT_DEV_RCA;
    //int c = METEOR_INPUT_DEV_SVIDEO;
    if (ioctl(fd, METEORSINPUT, &c) < 0) {
      perror("ioctl Setinput failed");
      Close();
      return false;
    }
    if(useMemMap) {
      int mapSize = geo.frames*bufFrameSize;
      ONDEBUG(cerr << "Mapping : " << mapSize << " bytes. \n");
      int usefd = fd;
      if(fbfd >= 0) // Using himem style frame buffer ?
	usefd =fbfd;
    
      mmbuf=(char *)mmap((caddr_t)0, mapSize , PROT_READ,MAP_FILE|MAP_SHARED, usefd, (off_t)0);
      if ( mmbuf == (char *)-1 ) {
	perror("mmap failed");
	Close();
	return false;
      }
    }
    
    return true;
  }
  
  
  
  bool DPIImageBaseMeteor1BodyC::Close() {
    if(mmbuf != 0 && mmbuf != (char *)-1)
      munmap(mmbuf,bufFrames*bufFrameSize);
    if(fd >= 0)
      close(fd);
    if(fbfd >= 0)
      close(fbfd);
    fd = -1; // Flag as inactive.
    fbfd = -1;
    return true;
  }
  
  //: Get next frame from grabber.
  // returns ptr to raw data. (this must be copied from the buffer.
  
  char *DPIImageBaseMeteor1BodyC::NextFrame() {
    if(fd < 0)
      return 0; // Should throw data not ready.
    struct meteor_counts  cnts;
    frameCount++;
    // capture frame 
    char *buf = 0;
    bool preAllocedBuff = false;
    if(outFormat != 1 && useMemMap) { // Need to allocate buffer ?
      buf = new char[bufFrameSize];
      preAllocedBuff = true;
    }
    
    if(useMemMap) {
      bool captureok = false;
      do {
#if 1
	if (ioctl(fd, METEORGCOUNT, &cnts) < 0) {
	  perror("ioctl(METEORGCOUNT)");
	  break;
	}
#endif
	//	int c = METEOR_CAP_CONT_ONCE;
	int c = METEOR_CAP_SINGLE;
	if (ioctl(fd, METEORCAPTUR, &c)) {
	  perror("ioctl SingleCapture failed");
	  //return 0;
	} else
	  captureok = true;
      } while(!captureok);
#if 0
      u_long cap;
      if (ioctl(meteor, METEORGCAPT, &cap)) {
	perror("ioctl GetCapt failed");
	retyrn 0;
      }
      fprintf(stderr, "Capture control: 0x%lx\n", cap);
#endif
      if(!preAllocedBuff) // Format 1 does a copy anyway.
	buf = mmbuf;
      else
	memcpy(buf,mmbuf,bufFrameSize);
      //buf = mmbuf + off.frame_offset[0];
    } else {
      // Not using memory map.
      int nr;
      if((nr = read(fd,buf,bufFrameSize)) != bufFrameSize) {
	// This could fail because of signal ?
	cerr << "DPIImageBaseMeteor1BodyC::NextFrame(), WARNING Read failed, Bytes : " << nr << "\n";
	delete [] buf;
	return 0;
      }
      if(outFormat == 0)
	return buf;
    }
    switch(outFormat)
      {
      case 0: break;
      case 1: // BGRX to RGB
	{
	  cerr << "DPIImageBaseMeteor1BodyC::NextFrame(), Converting BGRA to RGB. \n";
	  int pixels = bufFrameSize / sizeof(ByteBGRAValueC);
	  ByteRGBValueC *out = (ByteRGBValueC *)((void *)new char [pixels * sizeof(ByteRGBValueC)]);
	  ByteRGBValueC *place,*end = &out[pixels];
	  ByteBGRAValueC *in = (ByteBGRAValueC *) ((void *)buf);
	  for(place = out;place != end;place++,in++) 
	    *place = in->RGB();
	  if(preAllocedBuff)
	    delete [] buf;
	  buf = (char *) ((void *) out);
	}
	break;
      }
    ONDEBUG(cerr << "DPIImageBaseMeteor1BodyC::NextFrame(), Got frame. at " << ((void *) buf) << " Size:" << bufFrameSize << "\n");
    return buf;
  }
}
