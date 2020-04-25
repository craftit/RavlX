// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlVideoIO
//! file="Ravl/Image/VideoIO/syuvFormat.cc"

#include "Ravl/Image/syuvFormat.hh"
#include "Ravl/Image/ImgIOYUV.hh"
#include "Ravl/TypeName.hh"
#include <ctype.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {

  void InitSYUVFormat()
  {}


  // YUV ////////////////////////////////////////////////////////////////
  
  FileFormatSYUVBodyC::FileFormatSYUVBodyC(const StringC &nvName,const Index2dC &nvSize)
    : FileFormatBodyC(nvName,StringC("Abekas yuv image sequence in a single file. Size:") + StringC(nvSize.Row().V()) + " " + StringC(nvSize.Col().V())),
      vName(nvName),
      vSize(nvSize)
  {}
  //: Constructor.
  
  //: Is stream in std stream format ?
  // No way to tell !!
  // Just say no ??
  
  const std::type_info &
  FileFormatSYUVBodyC::ProbeLoad(IStreamC &in,const std::type_info &obj_type) const
  {
#if 0
    // Should we really claim we can load this? there's no way to test what it is!
    return typeid(ImageC<ByteYUVValueC>);
#else
    return typeid(void);
#endif
  }
  
  const std::type_info &
  FileFormatSYUVBodyC::ProbeLoad(const StringC &nfilename,IStreamC &in,const std::type_info &obj_type) const {
    if(nfilename == "-") 
    return typeid(ImageC<ByteYUVValueC>);
    StringC suffix = Extension(nfilename);
    ONDEBUG(cerr << "FileFormatSYUVBodyC::ProbeLoad() [" << vName << "] Called. Filename:'"<<nfilename <<" Ext:'" << suffix << "'  LoadType:'" << TypeName(obj_type) << "'\n");
    if (suffix != vName)     
      return typeid(void);
    return typeid(ImageC<ByteYUVValueC>);
  }
  
  const std::type_info &
  FileFormatSYUVBodyC::ProbeSave(const StringC &nfilename,const std::type_info &obj_type,bool forceFormat) const {
    if(forceFormat) 
      return typeid(ImageC<ByteYUVValueC>);
    StringC suffix = Extension(nfilename);
    ONDEBUG(cerr << "FileFormatSYUVBodyC::ProbeSave() [" << vName << "] Called. Filename:'"<<nfilename <<" Ext:'" << suffix << "'  LoadType:'" << TypeName(obj_type) << "'\n");
    if (suffix != vName)     
      return typeid(void);
    return typeid(ImageC<ByteYUVValueC>);
  }
  
  //: Create a input port for loading.
  // Will create an Invalid port if not supported.
  
  DPIPortBaseC FileFormatSYUVBodyC::CreateInput(IStreamC &in,const std::type_info &obj_type) const {
    ONDEBUG(cerr << "FileFormatSYUVBodyC::CreateInput(IStreamC &,const std::type_info &), Called. \n");
    if(!in.good())
      return DPIPortBaseC();
    if(obj_type == typeid(ImageC<ByteYUVValueC>))
      return DPIImageYUVC(in);
    return DPIPortBaseC();
  }
  
  //: Create a output port for saving.
  // Will create an Invalid port if not supported.
  
  DPOPortBaseC FileFormatSYUVBodyC::CreateOutput(OStreamC &out,const std::type_info &obj_type) const  {
    if(!out.good())
      return DPOPortBaseC();
    if(obj_type == typeid(ImageC<ByteYUVValueC>))
      return DPOImageYUVC(out);
    return DPOPortBaseC();
  }
  
  //: Create a input port for loading from file 'filename'.
  // Will create an Invalid port if not supported. <p>
  
  DPIPortBaseC FileFormatSYUVBodyC::CreateInput(const StringC &filename,const std::type_info &obj_type) const {
    ONDEBUG(cerr << "FileFormatSYUVBodyC::CreateInput(const StringC &,const std::type_info &), Called. \n");
    if(obj_type == typeid(ImageC<ByteYUVValueC>)) {
      IStreamC strm(filename);
      if(!strm)
	return DPIPortBaseC();
      return DPIImageYUVC(strm);
    }
    return DPIPortBaseC();
  }
  
  //: Create a output port for saving to file 'filename'..
  // Will create an Invalid port if not supported. <p>
  
  DPOPortBaseC FileFormatSYUVBodyC::CreateOutput(const StringC &filename,const std::type_info &obj_type) const {
    if(obj_type == typeid(ImageC<ByteYUVValueC>)) {
      OStreamC strm(filename);
      if(!strm)
	return DPOPortBaseC();
      return DPOImageYUVC(strm);
    }
    return DPOPortBaseC();
  }
  
  //: Get prefered IO type.
  
  const std::type_info &FileFormatSYUVBodyC::DefaultType() const 
  { return typeid(ImageC<ByteYUVValueC>); }
  
  // Some common cif formats.
  
  FileFormatSYUVC RegisterFileFormatSYUV  ("syuv"  ,Index2dC(576,720));
  
  
}
