// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlVideoIO
//! file="Ravl/Image/VideoIO/rgbFormat.cc"

#include "Ravl/Image/rgbFormat.hh"
#include "Ravl/Image/ImgIOrgb.hh"
#include "Ravl/TypeName.hh"
#include <ctype.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {
  
  void InitRGBFormat()
  {}
  
  
  // RGB ////////////////////////////////////////////////////////////////
  
  FileFormatRGBBodyC::FileFormatRGBBodyC(const StringC &nvName,const Index2dC &nvSize)
    : FileFormatBodyC(nvName,StringC("Raw rgb images Size:") + StringC(nvSize.Row().V()) + " " + StringC(nvSize.Col().V())),
      vName(nvName),
      vSize(nvSize)
  {}
  //: Constructor.
  
  //: Is stream in std stream format ?
  // No way to tell !!
  // Just say no ??
  
  const std::type_info &
  FileFormatRGBBodyC::ProbeLoad(IStreamC &in,const std::type_info &obj_type) const { 
#if 0
    return typeid(ImageC<ByteRGBValueC>); 
#else
    return typeid(void); 
#endif
  }
  
  const std::type_info &
  FileFormatRGBBodyC::ProbeLoad(const StringC &nfilename,IStreamC &in,const std::type_info &obj_type) const {
    StringC suffix = Extension(nfilename);
    ONDEBUG(cerr << "FileFormatRGBBodyC::ProbeLoad() [" << vName << "] Called. Filename:'"<<nfilename <<" Ext:'" << suffix << "'  LoadType:'" << TypeName(obj_type) << "'\n");
    if (suffix != vName)     
      return typeid(void);
    return typeid(ImageC<ByteRGBValueC>);
  }
  
  const std::type_info &
  FileFormatRGBBodyC::ProbeSave(const StringC &nfilename,const std::type_info &obj_type,bool forceFormat) const {
    if(!forceFormat) {
      StringC suffix = Extension(nfilename);
      ONDEBUG(cerr << "FileFormatRGBBodyC::ProbeSave() [" << vName << "] Called. Filename:'"<<nfilename <<" Ext:'" << suffix << "'  LoadType:'" << TypeName(obj_type) << "'\n");
      if (suffix != vName)     
	return typeid(void);
    }
    return typeid(ImageC<ByteRGBValueC>);
  }
  
  //: Create a input port for loading.
  // Will create an Invalid port if not supported.
  
  DPIPortBaseC FileFormatRGBBodyC::CreateInput(IStreamC &in,const std::type_info &obj_type) const {
    ONDEBUG(cerr << "FileFormatRGBBodyC::CreateInput(IStreamC &,const std::type_info &), Called. \n");
    if(!in.good())
      return DPIPortBaseC();
    if(obj_type == typeid(ImageC<ByteRGBValueC>))
      return DPIImageRGBC(in,vSize);
    return DPIPortBaseC();
  }
  
  //: Create a output port for saving.
  // Will create an Invalid port if not supported.
  
  DPOPortBaseC FileFormatRGBBodyC::CreateOutput(OStreamC &out,const std::type_info &obj_type) const  {
    if(!out.good())
      return DPOPortBaseC();
    if(obj_type == typeid(ImageC<ByteRGBValueC>))
      return DPOImageRGBC(out,vSize);
    return DPOPortBaseC();
  }
  
  //: Create a input port for loading from file 'filename'.
  // Will create an Invalid port if not supported. <p>
  
  DPIPortBaseC FileFormatRGBBodyC::CreateInput(const StringC &filename,const std::type_info &obj_type) const {
    ONDEBUG(cerr << "FileFormatRGBBodyC::CreateInput(const StringC &,const std::type_info &), Called. \n");
    if(obj_type == typeid(ImageC<ByteRGBValueC>)) {
      IStreamC strm(filename);
      if(!strm)
	return DPIPortBaseC();
      return DPIImageRGBC(strm,vSize);
    }
    return DPIPortBaseC();
  }
  
  //: Create a output port for saving to file 'filename'..
  // Will create an Invalid port if not supported. <p>
  
  DPOPortBaseC FileFormatRGBBodyC::CreateOutput(const StringC &filename,const std::type_info &obj_type) const {
    if(obj_type == typeid(ImageC<ByteRGBValueC>)) {
      OStreamC strm(filename);
      if(!strm)
	return DPOPortBaseC();
      return DPOImageRGBC(strm,vSize);
    }
    return DPOPortBaseC();
  }
  
  //: Get prefered IO type.
  
  const std::type_info &FileFormatRGBBodyC::DefaultType() const 
  { return typeid(ImageC<ByteRGBValueC>); }

  // Some common cif formats.

  FileFormatRGBC RegisterFileFormatRGB  ("rgb"  ,Index2dC(242,360));
  FileFormatRGBC RegisterFileFormatRGB2  ("rgb2"  ,Index2dC(288,384));
  FileFormatRGBC RegisterFileFormatRGBfull  ("frgb"  ,Index2dC(484,720));
  FileFormatRGBC RegisterFileFormatRGBfull2 ("frgb2"  ,Index2dC(576,768));
  
}
