// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.Images.Video.Video IO"
//! lib=RavlImgIOV4L
//! file="Ravl/Contrib/V4L/V4LFormat.cc"

typedef unsigned long ulong;


#include "Ravl/Image/V4LFormat.hh"
#include "Ravl/Image/ImgIOV4L.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/Image/RealYUVValue.hh"
#include "Ravl/Image/RealRGBValue.hh"

#include <linux/videodev.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define DPDEBUG 0

#if DPDEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {

  void InitV4LFormat()
  {}
  
  // V4L ////////////////////////////////////////////////////////////////
  
  //: Constructor.
  FileFormatV4LBodyC::FileFormatV4LBodyC(const StringC &nvName)
    : FileFormatBodyC(nvName,StringC("V4L driver. (@V4L)")),
      vName(nvName)
  {}
  
  
  const type_info &FileFormatV4LBodyC::ProbeLoad(IStreamC &in,const type_info &obj_type) const
  { return typeid(void); }
  
  bool FileFormatV4LBodyC::CheckRGB(int fd,struct video_picture &vidpic) const {
    vidpic.palette = VIDEO_PALETTE_RGB24;
    if(ioctl(fd,VIDIOCSPICT,&vidpic) >= 0)
      return true; // RGB seems to be supported.
    vidpic.palette = VIDEO_PALETTE_RGB24 | 0x80;
    if(ioctl(fd,VIDIOCSPICT,&vidpic) >= 0)
      return true; // RGB seems to be supported!
    vidpic.palette = VIDEO_PALETTE_YUV420P;
    if(ioctl(fd,VIDIOCSPICT,&vidpic) >= 0)
      return true; // We can convert directly from this format.
    return false;
  }
  
  bool FileFormatV4LBodyC::CheckYUV(int fd,struct video_picture &vidpic) const {
    vidpic.palette = VIDEO_PALETTE_YUYV;
    if(ioctl(fd,VIDIOCSPICT,&vidpic) >= 0)
      return true; // YUV seems to be supported.
    vidpic.palette = VIDEO_PALETTE_UYVY;
    if(ioctl(fd,VIDIOCSPICT,&vidpic) >= 0)
      return true; // YUV seems to be supported.
    vidpic.palette = VIDEO_PALETTE_YUV420P;
    if(ioctl(fd,VIDIOCSPICT,&vidpic) >= 0)
      return true; // YUV seems to be supported.
    return false;
  }

  bool FileFormatV4LBodyC::CheckYUV422(int fd,struct video_picture &vidpic) const {
    vidpic.palette = VIDEO_PALETTE_UYVY;
    if(ioctl(fd,VIDIOCSPICT,&vidpic) >= 0)
      return true; // YUV seems to be supported.
    vidpic.palette = VIDEO_PALETTE_YUYV;
    if(ioctl(fd,VIDIOCSPICT,&vidpic) >= 0)
      return true; // YUV seems to be supported.
    vidpic.palette = VIDEO_PALETTE_YUV420P;
    if(ioctl(fd,VIDIOCSPICT,&vidpic) >= 0)
      return true; // YUV seems to be supported.
    return false;
  }
  
  bool FileFormatV4LBodyC::CheckGREY(int fd,struct video_picture &vidpic) const {
    vidpic.palette = VIDEO_PALETTE_GREY;
    if(ioctl(fd,VIDIOCSPICT,&vidpic) >= 0)
      return true; // Grey level is supported.
    vidpic.palette = VIDEO_PALETTE_YUV420P;
    if(ioctl(fd,VIDIOCSPICT,&vidpic) >= 0)
      return true; // YUV seems to be supported.
    return false;
  }
  
  
  const type_info &
  FileFormatV4LBodyC::ProbeLoad(const StringC &filename,IStreamC &in,const type_info &obj_type) const { 
    if(filename.length() == 0)
      return typeid(void);
    if(filename[0] != '@')
      return typeid(void);
    StringC device = ExtractDevice(filename);
    StringC file = ExtractParams(filename);
    int channel = 0;
    int pn = file.index('#');
    if(pn >= 0) { // Got a channel number ?
      channel = file.after(pn).IntValue();
      file = file.before(pn);
    }
    ONDEBUG(cerr << "FileFormatV4LBodyC::ProbeLoad(), Checking file type." << obj_type.name() << " Device=" << device <<"\n");
    if(device != "V4L" && device != "V4LH")
      return typeid(void);
    
    enum { IMG_RGB, IMG_YUV, IMG_YUV422, IMG_GREY } imgtype = IMG_YUV;
    
    // Some huristics to select the best format to capture date from the
    // card in.   If in doubt get YUV as thats what most video is in anyway.
    
    if(obj_type == typeid(ImageC<ByteRGBValueC>))
      imgtype = IMG_RGB;
    if(obj_type == typeid(ImageC<RealRGBValueC>))
      imgtype = IMG_RGB;
    else if(obj_type == typeid(ImageC<ByteYUVValueC>))
      imgtype = IMG_YUV;
    else if(obj_type == typeid(ImageC<RealYUVValueC>))
      imgtype = IMG_YUV;
    else if(obj_type == typeid(ImageC<ByteYUV422ValueC>))
      imgtype = IMG_YUV422;
    else if(obj_type == typeid(ImageC<ByteT>))
      imgtype = IMG_GREY;
    else if(obj_type == typeid(ImageC<RealT>))
      imgtype = IMG_GREY;
    else if(obj_type == typeid(ImageC<IntT>))
      imgtype = IMG_GREY;
    else if(obj_type == typeid(ImageC<UIntT>))
      imgtype = IMG_GREY;
    
    int fd = open(file,O_RDWR);
    if(fd < 0) { // Failed to open device.
      ONDEBUG(cerr << "FileFormatV4LBodyC::ProbeLoad(), Failed to open file. \n");
      return typeid(void);
    }
    struct video_capability vidcap;
    if(ioctl(fd,VIDIOCGCAP,&vidcap) < 0) { // Supports Video4Linux 1 ?
      close(fd);
      return typeid(void);
    }
    if(!(vidcap.type | VID_TYPE_CAPTURE)) { // Can capture video ?
      close(fd);
      return typeid(void);
    }
    // Look at picture paramiters.
    struct video_picture vidpic;
    if(ioctl(fd,VIDIOCGPICT,&vidpic) < 0) {
      close(fd);
      cerr << "ERROR: Unexpected failure to get picture paramiters. \n";
      return typeid(void); // Somethings not working.
    }
    
    // Try and match the best supported capture format with what the 
    // card can actually do.
    
    ONDEBUG(cerr << "Probing supported formats. Preferred=" << imgtype << "\n");
    switch(imgtype) {
    case IMG_RGB:
      if(CheckRGB(fd,vidpic)) {
	imgtype = IMG_RGB;
	break;
      }
      if(CheckYUV422(fd,vidpic)) {
	imgtype = IMG_YUV422;
	break;
      }
      if(CheckYUV(fd,vidpic)) {
	imgtype = IMG_YUV;
	break;
      }
      if(CheckGREY(fd,vidpic)) {
	imgtype = IMG_GREY;
	break;
      }
      break;
    case IMG_YUV422:
      if(CheckYUV422(fd,vidpic)) {
	imgtype = IMG_YUV422;
	break;
      }
      if(CheckYUV(fd,vidpic)) {
	imgtype = IMG_YUV;
	break;
      }
      if(CheckRGB(fd,vidpic)) {
	imgtype = IMG_RGB;
	break;
      }
      if(CheckGREY(fd,vidpic)) {
	imgtype = IMG_GREY;
	break;
      }
      break;
    case IMG_YUV:
      if(CheckYUV(fd,vidpic)) {
	imgtype = IMG_YUV;
	break;
      }
      if(CheckYUV422(fd,vidpic)) {
	imgtype = IMG_YUV422;
	break;
      }
      if(CheckRGB(fd,vidpic)) {
	imgtype = IMG_RGB;
	break;
      }
      if(CheckGREY(fd,vidpic)) {
	imgtype = IMG_GREY;
	break;
      }
      break;
    case IMG_GREY:
      if(CheckGREY(fd,vidpic)) {
	imgtype = IMG_GREY;
	break;
      }
      if(CheckYUV422(fd,vidpic)) {
	imgtype = IMG_YUV422;
	break;
      }
      if(CheckYUV(fd,vidpic)) {
	imgtype = IMG_YUV;
	break;
      }
      if(CheckRGB(fd,vidpic)) {
	imgtype = IMG_RGB;
	break;
      }
      break;
    }
    ONDEBUG(cerr << "Probe format = " << ((int) imgtype) << "\n");
    close(fd);
    switch(imgtype) {
    case IMG_RGB: return typeid(ImageC<ByteRGBValueC>);
    case IMG_YUV: return typeid(ImageC<ByteYUVValueC>);
    case IMG_GREY: return typeid(ImageC<ByteT>);
    case IMG_YUV422: return typeid(ImageC<ByteYUV422ValueC>);
    }
    return typeid(ImageC<ByteYUVValueC>);
  }
  
  const type_info &
  FileFormatV4LBodyC::ProbeSave(const StringC &nfilename,const type_info &obj_type,bool forceFormat) const
  { return typeid(void); }
  
  //: Create a input port for loading.
  // Will create an Invalid port if not supported.
  
  DPIPortBaseC FileFormatV4LBodyC::CreateInput(IStreamC &in,const type_info &obj_type) const
  { return DPIPortBaseC(); }
  
  //: Create a output port for saving.
  // Will create an Invalid port if not supported.
  
  DPOPortBaseC FileFormatV4LBodyC::CreateOutput(OStreamC &out,const type_info &obj_type) const 
  { return DPOPortBaseC(); }

  //: Create a input port for loading from file 'filename'.
  // Will create an Invalid port if not supported. <p>
  
  DPIPortBaseC FileFormatV4LBodyC::CreateInput(const StringC &filename,const type_info &obj_type) const
  {
    ONDEBUG(cerr << "FileFormatV4LBodyC::CreateInput(const StringC &,const type_info &), Called. \n");
    if(filename[0] != '@')
    return DPIPortBaseC();
    StringC fn = ExtractParams(filename);
    StringC dev = ExtractDevice(filename);
    int channel = 0;
    int pn = fn.index('#');
    if(pn >= 0) { // Got a channel number ?
      channel = fn.after(pn).IntValue();
      fn = fn.before(pn);
    }
    bool half = false;
    if(dev == "V4LH")
      half = true; // Attempt to get images halfed along each dimention.
    if(fn == "")
      fn = "/dev/video0";
    if(obj_type == typeid(ImageC<ByteYUVValueC>))
      return DPIImageV4LC<ByteYUVValueC>(fn,half,channel);
    if(obj_type == typeid(ImageC<ByteYUV422ValueC>))
      return DPIImageV4LC<ByteYUV422ValueC>(fn,half,channel);
    if(obj_type == typeid(ImageC<ByteRGBValueC>))
      return DPIImageV4LC<ByteRGBValueC>(fn,half,channel);
    if(obj_type == typeid(ImageC<ByteT>))
      return DPIImageV4LC<ByteT>(fn,half,channel);
    return DPIPortBaseC();
  }
  
  //: Create a output port for saving to file 'filename'..
  // Will create an Invalid port if not supported. <p>
  
  DPOPortBaseC FileFormatV4LBodyC::CreateOutput(const StringC &filename,const type_info &obj_type) const
  { return DPOPortBaseC(); }
  
  //: Get prefered IO type.
  
  const type_info &FileFormatV4LBodyC::DefaultType() const 
  { return typeid(ImageC<ByteYUVValueC>); }
  
  // Some common cif formats.
  
  FileFormatV4LC RegisterFileFormatV4L("v4l");
}
