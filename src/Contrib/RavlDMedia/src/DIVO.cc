// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlDMedia
//! file="Ravl/Contrib/DMedia/DIVO.cc"

#include "Ravl/DMedia/DIVO.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/Threads/Signal.hh"
#include "Ravl/Image/ByteVYUValue.hh"
#include "Ravl/Image/ByteYUV422Value.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN
{
  //: Constructor.
  
  DPVideoDIVOBaseBodyC::DPVideoDIVOBaseBodyC(const VLServerC &nserver,const char *nm,const type_info &npixType,bool nframe,bool nglxMode)
    : devName(nm),
      server(nserver),
      active(false),
      glxMode(nglxMode),
      frameMode(nframe),
      pixType(npixType),
      imageReady(0)
  {}
  
  //: Setup path paramiters.
  
  bool DPVideoDIVOBaseBodyC::SetupParamiters() {
    ONDEBUG(cerr << "DPVideoDIVOBaseBodyC::SetupParamiters(), Called.\n");
    node.SetControl(VL_COMPRESSION,VL_COMPRESSION_NONE);  
    
    if(pixType == typeid(ByteVYUValueC)) {
      ONDEBUG(cerr << "DPVideoDIVOBaseBodyC::SetupParamiters(), Setting up for YUV 888.\n");
      node.SetControl(VL_COLORSPACE,VL_COLORSPACE_YUV);
      node.SetControl(VL_PACKING,VL_PACKING_444_8);
    } else if(pixType == typeid(ByteRGBValueC)) {
      ONDEBUG(cerr << "DPVideoDIVOBaseBodyC::SetupParamiters(), Setting up for RGB 888.\n");
      node.SetControl(VL_COLORSPACE,VL_COLORSPACE_RGB);
      node.SetControl(VL_PACKING,VL_PACKING_444_8);
    } else if(pixType == typeid(ByteT)) {
      ONDEBUG(cerr << "DPVideoDIVOBaseBodyC::SetupParamiters(), Setting up for Y 8.\n");
      node.SetControl(VL_COLORSPACE,VL_COLORSPACE_YUV);
      node.SetControl(VL_PACKING,VL_PACKING_4_8);
    }  else if(pixType == typeid(ByteYUV422ValueC)) {
      ONDEBUG(cerr << "DPVideoDIVOBaseBodyC::SetupParamiters(), Setting up for YUV422 8.\n");
      node.SetControl(VL_COLORSPACE,VL_COLORSPACE_YUV);
      node.SetControl(VL_PACKING,VL_PACKING_R242_8);      
    } else {// Don't know.
      cerr << "DPVideoDIVOBaseBodyC::SetupParamiters(), WARNING: Unrecognised pixel type:" << TypeName(pixType) << "\n";
      // Set something up, though its not right.
      node.SetControl(VL_COLORSPACE,VL_COLORSPACE_YUV);      
      node.SetControl(VL_PACKING,VL_PACKING_R444_8);
    }
    
    int vlTiming;
    
    vid.GetControl(VL_TIMING,vlTiming);
    node.SetControl(VL_TIMING,vlTiming);
    
    if(frameMode)
      node.SetControl(VL_CAP_TYPE,VL_CAPTURE_INTERLEAVED);
    else 
      node.SetControl(VL_CAP_TYPE,VL_CAPTURE_FIELDS);
    
    vid.SetControl(VL_FORMAT,VL_FORMAT_DIGITAL_COMPONENT_SERIAL);
    
    int x,y;
    node.GetSize(x,y);
    rect = ImageRectangleC(0,x-1,0,y-1);
    cerr << "DPVideoDIVOBaseBodyC::SetupParamiters(), Image size=" << x << " " << y << "\n";
    return true;
  }
  
  ///////////////////////////////////////////////////////////
  
  //: Constructor
  
  DPIVideoDIVOBaseBodyC::DPIVideoDIVOBaseBodyC(const VLServerC &nserver,
					       const char *nm,
					       const type_info &npixType,
					       bool nframe,
					       bool nglxMode, 
					       IntT noFrames)
    : DPVideoDIVOBaseBodyC(nserver,nm,npixType,nframe,nglxMode),
      subSample(2)
  {
    ONDEBUG(cerr << "DPIVideoDIVOBaseBodyC(), Starting setup. \n");
    VLDeviceC dev = server.FindDevice(nm);
    //noFrames = 1;
    // Setup the path.
    ONDEBUG(cerr << "Using a " << noFrames << " frame buffer.\n");
    vid =  VLNodeVideoC(server,VL_SRC,VL_ANY);
    node = VLNodeMemoryPoolC(server,VL_DRN,noFrames,glxMode);
    path = VLPathC(server,vid,node,dev);
    
    ONDEBUG(cerr << "DPIVideoDIVOBaseBodyC(), Path done. \n");
    // Setup the paramiters.
    
    SetupParamiters();
    
    ONDEBUG(cerr << "DPIVideoDIVOBaseBodyC(), Connecting ready signal\n");
    RavlAssert(path.DataReady().IsValid());
    RavlAssert(imageReady.IsValid());
    Connect(path.DataReady(),imageReady,&SemaphoreRC::Post);
    
    ONDEBUG(cerr << "DPIVideoDIVOBaseBodyC(), Starting transfer.\n");
    if(path.BeginTransfer()) 
      active = true;
    else 
      cerr << "WARNING: Failed to begin video transfer for " << nm << " \n";
    ONDEBUG(cerr << "DPIVideoDIVOBaseBodyC(), Done.\n");
  }
  
  
  //: Constructor
  
  DPOVideoDIVOBaseBodyC::DPOVideoDIVOBaseBodyC(const VLServerC &nserver,const char *nm,const type_info &npixType,bool nframe,bool nglxMode) 
    : DPVideoDIVOBaseBodyC(nserver,nm,npixType,nframe,nglxMode)
  {
    ONDEBUG(cerr << "DPOVideoDIVOBaseBodyC(), Starting setup. \n");
    VLDeviceC dev = server.FindDevice(nm);
    
    // Setup the path.
    
    vid =  VLNodeVideoC(server,VL_DRN,VL_ANY);
    node = VLNodeMemoryPoolC(server,VL_SRC,10,glxMode);
    path = VLPathC(server,vid,node,dev);
    
    // Setup the paramiters.
    
    SetupParamiters();
    
    if(path.BeginTransfer()) 
      active = true;
    ONDEBUG(cerr << "DPOVideoDIVOBaseBodyC(), Setup done. \n");
  }


}

