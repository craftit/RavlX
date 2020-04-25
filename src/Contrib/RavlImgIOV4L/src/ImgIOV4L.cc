// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlImgIOV4L
//! file="Ravl/Contrib/V4L/ImgIOV4L.cc"

#include "Ravl/Image/Image.hh"
#include "Ravl/DP/FileFormatIO.hh"
#include "Ravl/Image/SubSample.hh"
#include "Ravl/Image/ImgIOV4L.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/Image/RGBcYUV.hh"
#include "Ravl/DList.hh"
#include "Ravl/DP/AttributeValueTypes.hh"

typedef unsigned long ulong;


#include <sys/select.h>
//#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <sys/mman.h>

#include <linux/videodev.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

#define USE_V4L1 1

namespace RavlImageN {
  
  //: Constructor.
  
  DPIImageBaseV4LBodyC::DPIImageBaseV4LBodyC(const StringC &dev,const type_info &npixType,const ImageRectangleC &nrect,int nchannel)
    : rect(nrect),
      fd(-1),
      buf_grey(0),
      buf_u(0),
      buf_v(0),
      half(false),
      memmap(false),
      channel(nchannel),
      shutterSpeed(-1)
  {
    sourceType = SOURCE_UNKNOWN;
    if(!Open(dev,npixType,nrect))
      cerr << "DPIImageBaseV4LBodyC::DPIImageBaseV4LBodyC(), Failed to  open '" << dev << "'\n";
    //DumpParam(cerr);
  }

  DPIImageBaseV4LBodyC::DPIImageBaseV4LBodyC(const StringC &dev,const type_info &npixType,bool nhalf,int nchannel)
    : rect(0,-1,0,-1),
      fd(-1),
      buf_grey(0),
      buf_u(0),
      buf_v(0),
      half(nhalf),
      memmap(false),
      channel(nchannel)
  {
    sourceType = SOURCE_UNKNOWN;
    if(!Open(dev,npixType,rect))
      cerr << "DPIImageBaseV4LBodyC::DPIImageBaseV4LBodyC(), Failed to  open '" << dev << "'\n";
    //DumpParam(cerr);
  }
  
  //: Destructor.
  
  DPIImageBaseV4LBodyC::~DPIImageBaseV4LBodyC() {
    Close();
  }

  //: Open a video for linux device.
  
  bool DPIImageBaseV4LBodyC::Open(const StringC &dev,const type_info &npixType,const ImageRectangleC &nrect) {
    int nfd = open(dev.chars(),O_RDWR);
    if(nfd == 0) {
      cerr << "Failed to open video device. '" << dev << "' \n";
      return false;
    }
#if USE_V4L1
    if(!Open1(nfd,npixType,nrect)) {
      cerr << "Not a video device. '" << dev << "' \n";
      return false;
    }
#else
    cerr << "Not a video device. '" << dev << "' \n";
#endif
    return true;
  }
    
  bool DPIImageBaseV4LBodyC::Close() {
    if(fd < 0)
      return true; // Already closed.
    close(fd);
    fd = -1;
    if(memmap)
      munmap(buffer,bufLen);
    else
      delete [] buf_grey;
    return true;
  }
  
  //: Pre next frame capture.
  
  bool DPIImageBaseV4LBodyC::NextPre() {
    //ONDEBUG(cerr << "DPIImageBaseV4LBodyC::NextPre(), Called. (BufNo=" << bufNo << ")\n");
    if(fd < 0) {
      cerr << "ERROR: No filehandle \n";
      return false;
    }
    int rret;
    if(memmap) {            
      buf_grey = (ByteT *) &buffer[frameOffsets[bufNo]];
      errno = 0;
      int errors = 0;
      for(;;) {
	if((rret = ioctl(fd,VIDIOCSYNC,&bufNo)) >= 0)
	  break; // Succeded.
	if(errno != EINTR) // This is relatively frequent, so silently ignore.
	  cerr << "WARNING: V4L driver failed to sync buffer. return:" << rret << " errno:" << errno << " buffer:" << bufNo << "\n";
	errors++;
	if(errors <= 3 && 
	   !(errno == EFAULT || errno == EINVAL)) { // Don't bother with retry if we don't think we can recover.
	  
	  if(errno != EINTR) // This is relatively frequent, so silently ignore.
	    cerr << " Retrying... (" << errors << ")\n";
	  continue;
	}
	throw DataNotReadyC("V4L:Error syncing buffer. ");
      }
      //cerr << "Buf=" << bufNo << "\n";
    }
    return true;
  }
  
  //: Post next frame capture.
  
  bool DPIImageBaseV4LBodyC::NextPost() {
    int rret;
    if(memmap) {
      struct video_mmap vmmap;
      vmmap.frame = bufNo;
      vmmap.height = rect.Rows();
      vmmap.width = rect.Cols();
      vmmap.format = palette;
      
      int errors = 0;
      
      
      for(;;) {
	// This can occasionaly fail, try again a few times if it does.
	if((rret = ioctl(fd,VIDIOCMCAPTURE,&vmmap)) >= 0) 
	  break; // Is succeeded...
	
	cerr << "WARNING: V4L failed to start memory mapped capture. " << rret << " Errno=" << errno << " buf=" << bufNo << "\n";
	errors++;
	if(errors <= 3 &&
	   !(errno == EFAULT || errno == EINVAL)) { // Don't bother with retry if we don't think we can recover.
	  cerr << " Retrying... (" << errors << ")\n";
	  continue;
	}
	throw DataNotReadyC("V4L:Error starting capture. ");
      }
      
      // Increment buffer no.
      bufNo++;
      if((UIntT) bufNo >= frameOffsets.Size())
	bufNo = 0;
    }
    return true;
  }
  
  //: Get next YUV frame from grabber.
  
  bool DPIImageBaseV4LBodyC::NextFrame(ImageC<ByteYUV422ValueC> &ret) {
    if(!NextPre()) {
      ret =  ImageC<ByteYUV422ValueC>();
      return false;
    }
    int rsize = 0,rret;
    //ONDEBUG(cerr << "DPIImageBaseV4LBodyC::NextFrame(ImageC<ByteYUV422ValueC>&) Called \n");
    switch(palette) {
    case VIDEO_PALETTE_YUV420P: {
      int area2 = rect.Area()/4;
      buf_u = &buf_grey[rect.Area()];
      buf_v = &buf_u[area2];
      if(!memmap) {
	rsize = rect.Area() + rect.Area()/2;
	if((rret = read(fd,buf_grey,rsize)) != rsize) {
	  cerr << "Read failed. Bytes read = " << rret << " \n";
	  return false;
	}
      }
      ret = ImageC<ByteYUV422ValueC>(rect);
      ByteT *gat = buf_grey;
      ByteT *uat = buf_u;
      ByteT *vat = buf_v;
      ByteT *urow = buf_u;
      ByteT *vrow = buf_v;
      for(Array2dIterC<ByteYUV422ValueC> it(ret);it;) {
	uat = urow;
	vat = vrow;
	do {
	  *it = ByteYUV422ValueC(*(uat++),*gat);
	  it++; gat++;
	  *it = ByteYUV422ValueC(*(vat++),*gat);
	  gat++;
	} while(it.Next());
	uat = urow;
	vat = vrow;
	do {
	  *it = ByteYUV422ValueC(*(uat++),*gat);
	  it++; gat++;
	  *it = ByteYUV422ValueC(*(vat++),*gat);
	  gat++; 
	} while(it.Next());
	urow += rect.Cols()/2;
	vrow += rect.Cols()/2;
      }
    } break;
    case VIDEO_PALETTE_UYVY: {
      RavlAssert(buf_grey != 0);
      ret = ImageC<ByteYUV422ValueC>(rect);
      rsize = rect.Area() * 2;
      if(!memmap) {
	if((rret = read(fd,buf_grey,rsize)) != rsize) {
	  cerr << "Read failed. Bytes read = " << rret << "\n";
	  return false;
	}
      } else {
	memcpy(&(ret[rect.Origin()]),buf_grey,rsize);
      }
    } break;
    case VIDEO_PALETTE_YUYV: {
      RavlAssert(buf_grey != 0);
      ret = ImageC<ByteYUV422ValueC>(rect);
      if(!memmap) {
	rsize = rect.Area() * 2;
	if((rret = read(fd,&(ret[rect.Origin()]),rsize)) != rsize) {
	  cerr << "Read failed. Bytes read = " << rret << "\n";
	  return false;
	}
      }
      ByteT *gat = buf_grey;
      for(Array2dIterC<ByteYUV422ValueC> it(ret);it;) {
	ByteT *y = gat;
	gat++;
	ByteT *x = gat;
	gat++;
	*it = ByteYUV422ValueC(*x,*y);
      }
    } break;
    default:
      cerr << "DPIImageBaseV4LBodyC::NextFrame(ImageC<ByteYUVValueC>), Don't know how to handle palette mode: " << hex << palette << dec << "\n";
      NextPost();
      return false;
    }
    return NextPost();
  }
  
  bool DPIImageBaseV4LBodyC::NextFrame(ImageC<ByteYUVValueC> &ret) {
    if(!NextPre()) {
      ret =  ImageC<ByteYUVValueC>();
      return false;
    }
    int rsize,rret;
    //ONDEBUG(cerr << "DPIImageBaseV4LBodyC::NextFrame(ImageC<ByteYUVValueC>&) Called \n");
    switch(palette)  {
    case VIDEO_PALETTE_YUV420P: {
      int area2 = rect.Area()/4;
      buf_u = &buf_grey[rect.Area()];
      buf_v = &buf_u[area2];
      if(!memmap) {
	rsize = rect.Area() + rect.Area()/2;
	if((rret = read(fd,buf_grey,rsize)) != rsize) {
	  cerr << "Read failed. Bytes read = " << rret << " \n";
	  return false;
	}
      }
      ret = ImageC<ByteYUVValueC>(rect);
      ByteT *gat = buf_grey;
      ByteT *uat = buf_u;
      ByteT *vat = buf_v;
      ByteT *urow = buf_u;
      ByteT *vrow = buf_v;
      for(Array2dIterC<ByteYUVValueC> it(ret);it;) {
	uat = urow;
	vat = vrow;
	do {
	  ByteT u = *uat+128;
	  ByteT v = *vat+128;
	  *it = ByteYUVValueC(*gat,u,v);
	  it++; gat++;
	  *it = ByteYUVValueC(*gat,u,v);
	  gat++; uat++; vat++;
	} while(it.Next());
	uat = urow;
	vat = vrow;
	do {
	  ByteT u = *uat+128;
	  ByteT v = *vat+128;
	  *it = ByteYUVValueC(*gat,u,v);
	  it++; gat++;
	  *it = ByteYUVValueC(*gat,u,v);
	  gat++; uat++; vat++;
	} while(it.Next());
	urow += rect.Cols()/2;
	vrow += rect.Cols()/2;
      }
    } break;
    case VIDEO_PALETTE_YUYV: {
      RavlAssert(buf_grey != 0);
      ret = ImageC<ByteYUVValueC>(rect);
      if(!memmap) {
	rsize = rect.Area() * 2;
	if((rret = read(fd,buf_grey,rsize)) != rsize) {
	  cerr << "Read failed. Bytes read = " << rret << "\n";
	  return false;
	}
      }
      ByteT *gat = buf_grey;
      for(Array2dIterC<ByteYUVValueC> it(ret);it;) {
	ByteT y1 = *gat++;
	ByteT u = (*gat++) + 128;
	ByteT y2 = *gat++;
	ByteT v = (*gat++) + 128;
	*it = ByteYUVValueC(y1,u,v);
	it++;
	*it = ByteYUVValueC(y2,u,v);
	it++;
      }
    } break;
    case VIDEO_PALETTE_UYVY: {
      RavlAssert(buf_grey != 0);
      ret = ImageC<ByteYUVValueC>(rect);
      rsize = rect.Area() * 2;
      if(!memmap) {
	if((rret = read(fd,buf_grey,rsize)) != rsize) {
	  cerr << "Read failed. Bytes read = " << rret << "\n";
	  return false;
	}
      }
      ByteT *gat = buf_grey;
      for(Array2dIterC<ByteYUVValueC> it(ret);it;) {
	ByteT u = (*gat++) + 128;
	ByteT y1 = *gat++;
	ByteT v = (*gat++) + 128;
	ByteT y2 = *gat++;
	*it = ByteYUVValueC(y1,u,v);
	it++;
	*it = ByteYUVValueC(y2,u,v);
	it++;
      }
    } break;
    default:
      cerr << "DPIImageBaseV4LBodyC::NextFrame(ImageC<ByteYUVValueC>), Don't know how to handle palette mode: " << palette << "\n";
      NextPost();
      return false;
    }
    return NextPost();
  }
  
  
  //: Get next RGB frame from grabber.
  
  bool DPIImageBaseV4LBodyC::NextFrame(ImageC<ByteRGBValueC> &ret) {
    if(!NextPre()) {
      ret =  ImageC<ByteRGBValueC>();
      return false;
    }
    int rret;
    //ONDEBUG(cerr << "DPIImageBaseV4LBodyC::NextFrame(ImageC<ByteRGBValueC>&) Called \n");
    switch(palette) {
    case VIDEO_PALETTE_RGB24:
      ret = ImageC<ByteRGBValueC>(rect);
      if(!memmap)
	read(fd,&(ret[rect.Origin()]),rect.Area() * 3);
      else
	memcpy(&(ret[rect.Origin()]),buf_grey,rect.Area() * 3);
      // Swap blue and red.
      for(Array2dIterC<ByteRGBValueC> it(ret);it;it++) {
	ByteT x = it.Data().Blue();
	it.Data().Blue() = it.Data().Red();
	it.Data().Red() = x;
      }
      break;
    case VIDEO_PALETTE_RGB24 | 0x80:
      ret = ImageC<ByteRGBValueC>(rect);
      if(!memmap)
	read(fd,&(ret[rect.Origin()]),rect.Area() * 3);
      else
	memcpy(&(ret[rect.Origin()]),buf_grey,rect.Area() * 3);
      break;
    case VIDEO_PALETTE_YUV420P: {
      int area2 = rect.Area()/4;
      buf_u = &buf_grey[rect.Area()];
      buf_v = &buf_u[area2];
      if(!memmap) {
	IntT rsize = rect.Area() + rect.Area()/2;
	if((rret = read(fd,buf_grey,rsize)) != rsize) {
	  cerr << "Read failed. Bytes read = " << rret << " \n";
	  return false;
	}
      }
      ret = ImageC<ByteRGBValueC>(rect);
      ByteT *gat1 = buf_grey;
      ByteT *uat = buf_u;
      ByteT *vat = buf_v;
      ByteT *gat2;
      
      IntT offset = ret.LCol().V();
      IntT width = ret.Cols();
      ByteRGBValueC *at1,*at2,*eol;
      
      for(IndexC row = ret.TRow();row <= ret.BRow();row += 2) {
	at1 = &(ret[row][offset]);
	at2 = &(ret[row+1][offset]);
	eol = &(at1[width]);
	gat2 = &(gat1[width]);
	
	for(;at1 < eol;) {
	  SByteT u = *(uat++)+128;
	  SByteT v = *(vat++)+128;
	  RealT ru = u;
	  RealT rv = v;
	  
	  // Convert the colours...
	  // Maybe speed this up by generating a lookup table for rx and bx components ??
	  RealT rx =                                  rv * ImageYUVtoRGBMatrix[0][2];
	  RealT gx = ru * ImageYUVtoRGBMatrix[1][1] + rv * ImageYUVtoRGBMatrix[1][2];
	  RealT bx = ru * ImageYUVtoRGBMatrix[2][1];
	  
	  RealT i1 = *(gat1++);
	  RealT i2 = *(gat2++);
	  
	  RealRGBValueC rgb1(i1 + rx ,i1 + gx ,i1 + bx);
	  rgb1.Limit(0,255);
	  RealRGBValueC rgb2(i2 + rx ,i2 + gx ,i2 + bx);
	  rgb2.Limit(0,255);
	  
	  *(at1++) = ByteRGBValueC(rgb1);
	  *(at2++) = ByteRGBValueC(rgb2);
	  
	  i1 = *(gat1++);
	  i2 = *(gat2++);
	  
	  rgb1 = RealRGBValueC(i1 + rx ,i1 + gx ,i1 + bx);
	  rgb1.Limit(0,255);
	  rgb2 = RealRGBValueC(i2 + rx ,i2 + gx ,i2 + bx);
	  rgb2.Limit(0,255);
	  *(at1++) = ByteRGBValueC(rgb1);
	  *(at2++) = ByteRGBValueC(rgb2);
	}
	gat1 += width;
      }      
    } break;
    default:
      cerr << "DPIImageBaseV4LBodyC::NextFrame(ImageC<ByteRGBValueC>), Don't know how to handle palette mode: " << palette << "\n";
      NextPost();
      return false;
    }
    return NextPost();
  }

  //: Get next grey level frame from grabber.
  
  bool DPIImageBaseV4LBodyC::NextFrame(ImageC<ByteT> &ret) {
    if(!NextPre()) {
      ret =  ImageC<ByteT>();
      return false;
    }
    ONDEBUG(cerr << "DPIImageBaseV4LBodyC::NextFrame(ImageC<ByteT>&) Called \n");
    int rsize,rret;
    switch(palette) {
    case VIDEO_PALETTE_GREY: 
      ret = ImageC<ByteT>(rect);
      if(!memmap) {
	// Read in grey level info.
	rsize = rect.Area();
	if((rret = read(fd,&(ret[rect.Origin()]),rsize)) != rsize) {
	  cerr << "Read failed. Bytes read = " << rret << " \n";
	  return false;
	}
      } else
	memcpy(&(ret[rect.Origin()]),buf_grey,rect.Area());	
      break;
    case VIDEO_PALETTE_YUV420P: 
      ret = ImageC<ByteT>(rect);
      if(!memmap) {
	// Read in grey level info.
	rsize = rect.Area();
	if((rret = read(fd,&(ret[rect.Origin()]),rsize)) != rsize) {
	  cerr << "Read failed. Bytes read = " << rret << " \n";
	  return false;
	}
	// Discard colour information.
	rsize = rect.Area()/2;
	if((rret = read(fd,buf_grey,rsize)) != rsize) {
	  cerr << "Read failed. Bytes read = " << rret << " \n";
	  return false;
	}
      } else
	memcpy(&(ret[rect.Origin()]),buf_grey,rect.Area());
      break;
    default:
      cerr << "DPIImageBaseV4LBodyC::NextFrame(ImageC<ByteT>), Don't know how to handle palette mode: " << palette << "\n";
      NextPost();
      return false;
    }
    
    return NextPost();
  }

  //: Handle get attrib.
  
  bool DPIImageBaseV4LBodyC::HandleGetAttr(const StringC &attrName,StringC &attrValue) {
    IntT tmp;
    if(attrName == "shutter_speed") {
      if(!GetShutterSpeed(tmp))
	return false;
      attrValue = StringC(tmp);
      return true;
    }
    if(attrName == "brightness") {
      if(!GetBrightness(tmp))
	return false;
      attrValue = StringC(tmp);
      return true;      
    }
    if(attrName == "contrast") {
      if(!GetContrast(tmp))
	return false;
      attrValue = StringC(tmp);
      return true;      
    }
    if(attrName == "agc") {
      if(!GetAGC(tmp))
	return false;
      attrValue = StringC(tmp);
      return true;      
    }
    if(attrName == "gamma") {
      if(!GetGamma(tmp))
	return false;
      attrValue = StringC(tmp);
      return true;      
    }
    if(attrName == "padded") {
      if(!GetPad(tmp))
	return false;
      attrValue = StringC(tmp);
      return true;      
    }
    if(attrName == "compression") {
      if(!GetCompression(tmp))
	return false;
      attrValue = StringC(tmp);
      return true;      
    }
    if(attrName == "whitebalance") {
      if(!GetWhiteBalance(tmp))
	return false;
      attrValue = StringC(tmp);
      return true;      
    }
    ONDEBUG(cerr << "DPIImageBaseV4LBodyC::HandleGetAttr(),  '" << attrName << "'  not found\n");
    return false;
  }
  
  //: Handle Set attrib.
  
  bool DPIImageBaseV4LBodyC::HandleSetAttr(const StringC &attrName,const StringC &attrValue) {
    if(attrName == "shutter_speed")
      return SetShutterSpeed(attrValue.IntValue());
    if(attrName == "brightness")
      return SetBrightness(attrValue.IntValue());
    if(attrName == "contrast")
      return SetContrast(attrValue.IntValue());
    if(attrName == "agc")
      return SetAGC(attrValue.IntValue());
    if(attrName == "indicator")
      return SetLED(attrValue.IntValue());
    if(attrName == "gamma")
      return SetGamma(attrValue.IntValue());
    if(attrName == "compression")
      return SetCompression(attrValue.IntValue());
    if(attrName == "whitebalance")
      return SetWhiteBalance(attrValue.IntValue());
    if(attrName == "reset")
      return SetReset();
    if(attrName == "saveusersettings")
      return SaveUserSettings();
    if(attrName == "loadusersettings")
      return LoadUserSettings();
    ONDEBUG(cerr << "DPIImageBaseV4LBodyC::HandleSetAttr(),  '" << attrName << "'  not found\n");
    return false;
  }
  
  //: Get a stream attribute.
  // Returns false if the attribute name is unknown.
  // This is for handling stream attributes such as frame rate, and compression ratios.
  
  bool DPIImageBaseV4LBodyC::HandleGetAttr(const StringC &attrName,IntT &attrValue) {
    ONDEBUG(cerr << "DPIImageBaseV4LBodyC::HandleGetAttr(), Called '" << attrName << "'\n");
    if(attrName == "shutter_speed")
      return GetShutterSpeed(attrValue);
    if(attrName == "brightness")
      return GetBrightness(attrValue);
    if(attrName == "contrast")
      return GetContrast(attrValue);
    if(attrName == "agc")
      return GetAGC(attrValue);
    if(attrName == "compression")
      return GetCompression(attrValue);
    if(attrName == "gamma")
      return GetGamma(attrValue);
    if(attrName == "padded")
      return GetPad(attrValue);
    if(attrName == "whitebalance")
      return GetWhiteBalance(attrValue);
    if(attrName == "pan")
      return GetPan(attrValue);
    if(attrName == "tilt")
      return GetTilt(attrValue);
    ONDEBUG(cerr << "DPIImageBaseV4LBodyC::HandleGetAttr(),  '" << attrName << "'  not found\n");
    return false;
  }
  
  //: Set a stream attribute.
  // Returns false if the attribute name is unknown.
  // This is for handling stream attributes such as frame rate, and compression ratios.
  
  bool DPIImageBaseV4LBodyC::HandleSetAttr(const StringC &attrName,const IntT &attrValue) {
    if(attrName == "shutter_speed")
      return SetShutterSpeed(attrValue);
    if(attrName == "brightness")
      return SetBrightness(attrValue);
    if(attrName == "contrast")
      return SetContrast(attrValue);
    if(attrName == "agc")
      return SetAGC(attrValue);
    if(attrName == "compression")
      return SetCompression(attrValue);
    if(attrName == "indicator")
      return SetLED(attrValue);
    if(attrName == "gamma")
      return SetGamma(attrValue);
    if(attrName == "whitebalance")
      return SetWhiteBalance(attrValue);
    if(attrName == "reset")
      return SetReset();
    if(attrName == "saveusersettings")
      return SaveUserSettings();
    if(attrName == "loadusersettings")
      return LoadUserSettings();
    if(attrName == "pan")
      return SetPan(attrValue);
    if(attrName == "tilt")
      return SetTilt(attrValue);
    ONDEBUG(cerr << "DPIImageBaseV4LBodyC::HandleSetAttr(),  '" << attrName << "'  not found\n");
    return false;
  }


  //: Build list of attributes.
  
  bool DPIImageBaseV4LBodyC::BuildAttributes(AttributeCtrlBodyC &attrCtrl) {
    attrCtrl.RegisterAttribute(AttributeTypeNumC<IntT>("shutter_speed","Camera shutter speed",true,true,-1,65000,1,-1));
    attrCtrl.RegisterAttribute(AttributeTypeNumC<IntT>("brightness","Brightness",true,true,-1,65000,1,-1));
    attrCtrl.RegisterAttribute(AttributeTypeNumC<IntT>("contrast","Contrast",true,true,-1,65000,1,-1));
    attrCtrl.RegisterAttribute(AttributeTypeNumC<IntT>("gamma","Gamma correction",true,true,0,65000,1,-1));
    attrCtrl.RegisterAttribute(AttributeTypeNumC<IntT>("agc","Gamma correction",true,true,-1,65000,1,-1));
    attrCtrl.RegisterAttribute(AttributeTypeNumC<IntT>("indicator","Turn on off indicator LED.",true,true,0,65000,1,0));
    attrCtrl.RegisterAttribute(AttributeTypeNumC<IntT>("compression","Set amount of compression to use on images.",true,true,0,65000,1,0));
    attrCtrl.RegisterAttribute(AttributeTypeNumC<IntT>("whitebalance","Image whitebalance.",true,true,0,65000,1,0));
    attrCtrl.RegisterAttribute(AttributeTypeNumC<IntT>("pan","Pan.",true,true,-7000,7000,1,0));
    attrCtrl.RegisterAttribute(AttributeTypeNumC<IntT>("tilt","Tilt.",true,true,-2500,2500,1,0));
    return true;
  }
  
#if 0
  //: Get list of attributes available.
  // This method will ADD all available attribute names to 'list'.
  
  bool DPIImageBaseV4LBodyC::HandleGetAttrList(DListC<StringC> &list) const {
    list.InsLast("shutter_speed");
    list.InsLast("brightness");
    list.InsLast("contrast");
    list.InsLast("gamma");
    list.InsLast("agc");
    list.InsLast("indicator");
    list.InsLast("padded");
    list.InsLast("compression");
    list.InsLast("whitebalance");
    list.InsLast("reset");
    list.InsLast("pan");
    list.InsLast("tilt");
    return true;
  }
#endif
  
}

