// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.Contrib.Video IO"
//! lib=RavlImgIO1394dc
//! file="Ravl/Contrib/dc1394/Lib1394dcFormat.cc"
//! author="Charles Galambos"

#include "Ravl/Image/Lib1394dcFormat.hh"
#include "Ravl/Image/ImgIO1394dc.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/Image/RealYUVValue.hh"
#include "Ravl/Image/RealRGBValue.hh"
#include "Ravl/Image/ByteYUVValue.hh"

#define DPDEBUG 0
#include "Ravl/TypeName.hh"
#if DPDEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {

  void Init1394dcFormat()
  {}
  
  // 1394dc ////////////////////////////////////////////////////////////////
  
  //: Constructor.
  FileFormat1394dcBodyC::FileFormat1394dcBodyC(const StringC &nvName)
    : FileFormatBodyC(nvName,StringC("Lib1394dc firewire camera driver. (@IIDC)")),
      vName(nvName)
  {}


  const std::type_info &FileFormat1394dcBodyC::ProbeLoad(IStreamC &in,const std::type_info &obj_type) const
  { return typeid(void); }


  const std::type_info &
  FileFormat1394dcBodyC::ProbeLoad(const StringC &filename,IStreamC &in,const std::type_info &obj_type) const {
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
      if(channel > 100) // Requested DMA mode ?
	channel -= 100;
      file = file.before(pn);
    }
    ONDEBUG(std::cerr << "FileFormat1394dcBodyC::ProbeLoad(), Checking file type." << TypeName(obj_type.name()) << " Device='" << device <<"'\n");
    if(device != "IIDC")
      return typeid(void);
    
#if 0
    // Need to find out what conversions the camera will do before this is useful.
    
    enum { IMG_RGB, IMG_YUV, IMG_YUV422, IMG_GREY } imgtype = IMG_GREY;

    // Some heuristics to select the best format to capture date from the
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
    else if(obj_type == typeid(ImageC<RealT>))
      imgtype = IMG_GREY;
    else if(obj_type == typeid(ImageC<IntT>))
      imgtype = IMG_GREY;
    else if(obj_type == typeid(ImageC<UIntT>))
      imgtype = IMG_GREY;
    //else
    if(obj_type == typeid(ImageC<ByteT>))
      imgtype = IMG_GREY;
    
#endif
    
    // Go find out about the camera.
    
    raw1394handle_t raw1394handle = dc1394_create_handle(0); //assume only one port=0
    if(raw1394handle == 0)
      return typeid(void); // No camera found.
    IntT numCameras;
    nodeid_t * camera_nodes = dc1394_get_camera_nodes(raw1394handle,&numCameras,1);
    if(numCameras == 0) { // No camera's found.
      raw1394_destroy_handle(raw1394handle);
      return typeid(void);
    }
    nodeid_t cameraNode = camera_nodes[channel];
    //dc1394_query_supported_modes(raw1394handle_t handle, nodeid_t node,unsigned int format, quadlet_t *value);
    
    int ret = dc1394_init_camera(raw1394handle,cameraNode);
    //cerr << "CameraInit=" << ret << "\n";
    
    // Assume the camera is reset to format with most information.
    dc1394_miscinfo miscInfo;
    ret = dc1394_get_camera_misc_info(raw1394handle,cameraNode,&miscInfo);
    //cerr << "Misc=" << miscInfo.format << " " << miscInfo.mode << "\n";
    
    const std::type_info *pixelType = &typeid(ImageC<ByteT>);
    
#if 0    
    quadlet_t tmp = MODE_640x480_RGB;
    quadlet_t formats = FORMAT_VGA_NONCOMPRESSED;
    ret = dc1394_query_supported_formats(raw1394handle,cameraNode,&formats);
    std::cerr << "Formats=" << hex << formats << " ret=" << ret << "\n";
    ret = dc1394_query_supported_modes(raw1394handle,cameraNode,FORMAT_VGA_NONCOMPRESSED,&tmp);
    std::cerr << "MONO Ret=" << ret << " tmp=" << hex << tmp << "\n";
    
    switch(imgtype) {
    case IMG_GREY: 
      pixelType = &typeid(ImageC<ByteT>);
      break;
    case IMG_RGB: 
      pixelType = &typeid(ImageC<ByteRGBValueC>);
      break;
    case IMG_YUV422: 
      pixelType = &typeid(ImageC<ByteYUV422ValueC>);
      break;
    case IMG_YUV: //return typeid(ImageC<ByteYUVValueC>);
      std::cerr << "Unsupported pixel type. \n";
      pixelType = &typeid(void);
    }
#endif
    
    switch(miscInfo.mode) {
    case MODE_160x120_YUV444: pixelType = &typeid(ImageC<ByteYUVValueC>);    break;
    case MODE_640x480_YUV422:
    case MODE_320x240_YUV422: 
    case MODE_640x480_YUV411: pixelType = &typeid(ImageC<ByteYUV422ValueC>); break;
    case MODE_640x480_RGB:    pixelType = &typeid(ImageC<ByteRGBValueC>);    break;
    case MODE_640x480_MONO:   pixelType = &typeid(ImageC<ByteT>);            break;
    case MODE_640x480_MONO16: pixelType = &typeid(ImageC<UInt16T>);          break;
    default:
      std::cerr << "Unhandled camera mode " << miscInfo.mode << "\n";
      break;
    }
    // Clean up.
    
    raw1394_destroy_handle(raw1394handle);

    return *pixelType;
  }

  const std::type_info &
  FileFormat1394dcBodyC::ProbeSave(const StringC &nfilename,const std::type_info &obj_type,bool forceFormat) const
  { return typeid(void); }

  //: Create a input port for loading.
  // Will create an Invalid port if not supported.

  DPIPortBaseC FileFormat1394dcBodyC::CreateInput(IStreamC &in,const std::type_info &obj_type) const
  { return DPIPortBaseC(); }

  //: Create a output port for saving.
  // Will create an Invalid port if not supported.

  DPOPortBaseC FileFormat1394dcBodyC::CreateOutput(OStreamC &out,const std::type_info &obj_type) const
  { return DPOPortBaseC(); }

  //: Create a input port for loading from file 'filename'.
  // Will create an Invalid port if not supported. <p>

  DPIPortBaseC FileFormat1394dcBodyC::CreateInput(const StringC &filename,const std::type_info &obj_type) const
  {
    ONDEBUG(std::cerr << "FileFormat1394dcBodyC::CreateInput(const StringC &,const std::type_info &), Called. \n");
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
    //bool half = false;
    //if(dev == "IIDC")
    //  half = true; // Attempt to get images halved along each dimension.
    if(fn == "")
    {
      if(channel >= 100) // DMA access
        fn = "/dev/video1394";
      else              // normal access
        fn = "/dev/raw1394";
    }
    if(obj_type == typeid(ImageC<ByteYUVValueC>))
      return DPIImage1394dcC<ByteYUVValueC>(fn,channel);
    if(obj_type == typeid(ImageC<ByteYUV422ValueC>))
      return DPIImage1394dcC<ByteYUV422ValueC>(fn,channel);
    if(obj_type == typeid(ImageC<ByteRGBValueC>))
      return DPIImage1394dcC<ByteRGBValueC>(fn,channel);
    if(obj_type == typeid(ImageC<ByteT>))
      return DPIImage1394dcC<ByteT>(fn,channel);
    if(obj_type == typeid(ImageC<UInt16T>))
      return DPIImage1394dcC<UInt16T>(fn,channel);
    return DPIPortBaseC();
  }

  //: Create a output port for saving to file 'filename'..
  // Will create an Invalid port if not supported. <p>

  DPOPortBaseC FileFormat1394dcBodyC::CreateOutput(const StringC &filename,const std::type_info &obj_type) const
  { return DPOPortBaseC(); }

  //: Get preferred IO type.

  const std::type_info &FileFormat1394dcBodyC::DefaultType() const
  { return typeid(ImageC<ByteT>); }
  
  // Some common cif formats.
  
  FileFormat1394dcC RegisterFileFormat1394dc("1394dc");
}
