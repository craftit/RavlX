// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlVideoIO
//! file="Ravl/Image/VideoIO/rawFormat.cc"

#include "Ravl/Image/rawFormat.hh"
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

  void InitRawIOFormat()
  {}
  
  // Raw ////////////////////////////////////////////////////////////////
  
  FileFormatRawBodyC::FileFormatRawBodyC()
    : FileFormatBodyC("raw",StringC("Format optimised for realtime IO"))
    
  {
    ONDEBUG(cerr << "Added format 'raw'. \n");
  }
  //: Constructor.
  
  //: Is stream in std stream format ?
  // No way to tell !!
  
  // Just say no ??
  
  const std::type_info &
  FileFormatRawBodyC::ProbeLoad(IStreamC &in,const std::type_info &obj_type) const { 
    ONDEBUG(cerr << "FileFormatRawBodyC::ProbeLoad(IStreamC)  Called.  LoadType:'" << TypeName(obj_type) << "'\n");
    return typeid(void); 
  }
  
  const std::type_info &
  FileFormatRawBodyC::ProbeLoad(const StringC &nfilename,IStreamC &in,const std::type_info &obj_type) const {
    StringC suffix = Extension(nfilename);
    ONDEBUG(cerr << "FileFormatRawBodyC::ProbeLoad()  Called. Filename:'"<<nfilename <<" Ext:'" << suffix << "'  LoadType:'" << TypeName(obj_type) << "'\n");
    if (suffix != "raw")     
      return typeid(void);
    return typeid(ImageC<ByteRGBValueC>);
  }
  
  const std::type_info &
  FileFormatRawBodyC::ProbeSave(const StringC &nfilename,const std::type_info &obj_type,bool forceFormat) const {
    if(!forceFormat) {
      StringC suffix = Extension(nfilename);
      ONDEBUG(cerr << "FileFormatRawBodyC::ProbeSave()  Called. Filename:'"<<nfilename <<" Ext:'" << suffix << "'  LoadType:'" << TypeName(obj_type) << "'\n");
      if (suffix != "raw")     
	return typeid(void);
    }
    return typeid(ImageC<ByteRGBValueC>);
  }
  
  //: Create a input port for loading.
  // Will create an Invalid port if not supported.
  
  DPIPortBaseC FileFormatRawBodyC::CreateInput(IStreamC &in,const std::type_info &obj_type) const {
    ONDEBUG(cerr << "FileFormatRawBodyC::CreateInput(IStreamC &,const std::type_info &), Called. \n");
    if(!in.good())
      return DPIPortBaseC();
    if(obj_type == typeid(ImageC<ByteRGBValueC>))
      return DPIImageRGBC(in);
    return DPIPortBaseC();
  }

  //: Create a output port for saving.
  // Will create an Invalid port if not supported.
  
  DPOPortBaseC FileFormatRawBodyC::CreateOutput(OStreamC &out,const std::type_info &obj_type) const  {
    ONDEBUG(cerr << "FileFormatRawBodyC::CreateOutput(OStreamC &,const std::type_info &), Called. \n");
    if(!out.good())
      return DPOPortBaseC();
    if(obj_type == typeid(ImageC<ByteRGBValueC>))
      return DPOImageRGBC(out);
    return DPOPortBaseC();
  }
  
  //: Create a input port for loading from file 'filename'.
  // Will create an Invalid port if not supported. <p>
  
  DPIPortBaseC FileFormatRawBodyC::CreateInput(const StringC &filename,const std::type_info &obj_type) const {
    ONDEBUG(cerr << "FileFormatRawBodyC::CreateInput(const StringC &,const std::type_info &), Called. \n");
    if(obj_type != typeid(ImageC<ByteRGBValueC>)) 
      return DPIPortBaseC();  
    return DPIImageRGBC(filename);
  }
  
  //: Create a output port for saving to file 'filename'..
  // Will create an Invalid port if not supported. <p>
  
  DPOPortBaseC FileFormatRawBodyC::CreateOutput(const StringC &filename,const std::type_info &obj_type) const {
    ONDEBUG(cerr << "FileFormatRawBodyC::CreateOutput(const StringC &,const std::type_info &), Called. \n");
    if(obj_type != typeid(ImageC<ByteRGBValueC>)) 
      return DPOPortBaseC();    
    return DPOImageRGBC(filename);  
  }
  
  //: Get prefered IO type.
  
  const std::type_info &FileFormatRawBodyC::DefaultType() const 
  { return typeid(ImageC<ByteRGBValueC>); }
  
  FileFormatRawC RegisterFileFormatRaw();
}
