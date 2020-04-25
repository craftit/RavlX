// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlVideoIO
//! file="Ravl/Image/VideoIO/CifFormat.cc"

#include "Ravl/Image/CifFormat.hh"
#include "Ravl/Image/ImgIOCif.hh"
#include "Ravl/TypeName.hh"
#include <ctype.h>

#define DODEBUG 0

#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {

  void InitCifFormat()
  {}
    
  // Cif ////////////////////////////////////////////////////////////////
  
  FileFormatCifBodyC::FileFormatCifBodyC(const StringC &nvName,const Index2dC &nvSize)
    : FileFormatBodyC(nvName,StringC("Cif YUV colour image sequences. Size:") + StringC(nvSize.Row().V()) + " " + StringC(nvSize.Col().V())),
      vName(nvName),
      vSize(nvSize)
  {}
  //: Constructor.
  

  //: Is stream in std stream format ?
// Assume it is...  if it doesn't work, the user will have to sort it out...

  const std::type_info &
  FileFormatCifBodyC::ProbeLoad(IStreamC &in,const std::type_info &obj_type) const
  { 
#if 1
    return typeid(void);
#else
    return typeid(ImageC<ByteYUVValueC>); 
#endif
  }

  const std::type_info &
  FileFormatCifBodyC::ProbeLoad(const StringC &nfilename,IStreamC &in,const std::type_info &obj_type) const {
    StringC suffix = Extension(nfilename);
    ONDEBUG(cerr << "FileFormatCifBodyC::ProbeLoad() [" << vName << "] Called. Filename:'"<<nfilename <<" Ext:'" << suffix << "'  LoadType:'" << TypeName(obj_type) << "'\n");
    if (suffix != vName)     
      return typeid(void);
    return typeid(ImageC<ByteYUVValueC>);
  }

  const std::type_info &
  FileFormatCifBodyC::ProbeSave(const StringC &nfilename,const std::type_info &obj_type,bool forceFormat) const {
    if(!forceFormat) {
      StringC suffix = Extension(nfilename);
      ONDEBUG(cerr << "FileFormatCifBodyC::ProbeSave() [" << vName << "] Called. Filename:'"<<nfilename <<" Ext:'" << suffix << "'  LoadType:'" << TypeName(obj_type) << "'\n");
      if (suffix != vName)     
	return typeid(void);
    }
    return typeid(ImageC<ByteYUVValueC>);
  }

  //: Create a input port for loading.
  // Will create an Invalid port if not supported.

  DPIPortBaseC FileFormatCifBodyC::CreateInput(IStreamC &in,const std::type_info &obj_type) const {
    ONDEBUG(cerr << "FileFormatCifBodyC::CreateInput(IStreamC &,const std::type_info &), Called. \n");
    if(!in)
      return DPIPortBaseC();
    if(obj_type == typeid(ImageC<ByteYUVValueC>))
      return DPIImageCifC(in,vSize);
    return DPIPortBaseC();
  }

  //: Create a output port for saving.
  // Will create an Invalid port if not supported.

  DPOPortBaseC FileFormatCifBodyC::CreateOutput(OStreamC &out,const std::type_info &obj_type) const  {
    if(!out)
      return DPOPortBaseC();
    if(obj_type == typeid(ImageC<ByteYUVValueC>))
      return DPOImageCifC(out,vSize);
    return DPOPortBaseC();
  }
  
  //: Create a input port for loading from file 'filename'.
  // Will create an Invalid port if not supported. <p>
  
  DPIPortBaseC FileFormatCifBodyC::CreateInput(const StringC &filename,const std::type_info &obj_type) const {
    ONDEBUG(cerr << "FileFormatCifBodyC::CreateInput(const StringC &,const std::type_info &), Called. \n");
    if(obj_type == typeid(ImageC<ByteYUVValueC>)) {
      IStreamC strm(filename);
      if(!strm)
	return DPIPortBaseC();
      return DPIImageCifC(strm,vSize);
    }
    return DPIPortBaseC();
  }
  
  //: Create a output port for saving to file 'filename'..
  // Will create an Invalid port if not supported. <p>
  
  DPOPortBaseC FileFormatCifBodyC::CreateOutput(const StringC &filename,const std::type_info &obj_type) const {
    if(obj_type == typeid(ImageC<ByteYUVValueC>)) {
      OStreamC strm(filename);
      if(!strm)
	return DPOPortBaseC();
      return DPOImageCifC(filename,vSize);
    }
    return DPOPortBaseC();
  }
  
  //: Get prefered IO type.
  
  const std::type_info &FileFormatCifBodyC::DefaultType() const { 
    return typeid(ImageC<ByteYUVValueC>); 
  }

  // Some common cif formats.
  
  FileFormatCifC RegisterFileFormatSQCif("sqcif",Index2dC(72,88));
  FileFormatCifC RegisterFileFormatQCif ("qcif" ,Index2dC(144,176));
  FileFormatCifC RegisterFileFormatCif  ("cif"  ,Index2dC(288,352));
  FileFormatCifC RegisterFileFormat4Cif ("4cif" ,Index2dC(576,704));
  FileFormatCifC RegisterFileFormat16Cif("16cif",Index2dC(1152,1408));
}

