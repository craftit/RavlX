// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////
//! rcsid="$Id$"
//! date="24/03/2002"
//! author="Charles Galambos"
//! lib=RavlVideoIO
//! file="Ravl/Image/VideoIO/jsFormat.cc"

#include "Ravl/Image/jsFormat.hh"
#include "Ravl/Image/ImgIOjs.hh"
#include "Ravl/TypeName.hh"
#include <ctype.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

// FIXME :-  Use the magic number at the beginning of the file to identify the
// format. Support IO from streams.

namespace RavlImageN {

  void InitJSFormat() 
  {}
  
  // YUV ////////////////////////////////////////////////////////////////
  
  FileFormatJSBodyC::FileFormatJSBodyC()
    : FileFormatBodyC("js",StringC("A format similar to the ones used on uncompressed drastic video servers."))
  {
    ONDEBUG(cerr << "FileFormatJSBodyC::FileFormatJSBodyC(), Called \n");
  }
  //: Constructor.
  
  //: Is stream in std stream format ?
  
  const std::type_info & FileFormatJSBodyC::ProbeLoad(IStreamC &in,const std::type_info &obj_type) const { 
#if 0
    // FIXME :- Check magic number.
    return typeid(ImageC<ByteYUV422ValueC>); 
#else
    return typeid(void); 
#endif
  }
  
  const std::type_info &
  FileFormatJSBodyC::ProbeLoad(const StringC &nfilename,IStreamC &in,const std::type_info &obj_type) const {
    ONDEBUG(cerr << "FileFormatJSBodyC::ProbeLoad() Called. Filename:'"<<nfilename <<"' Ext:'" << Extension(nfilename) << "'  LoadType:'" << TypeName(obj_type) << "'\n");

    if (!nfilename.IsEmpty() && nfilename[0] == '@')
      return typeid(void);

    // FIXME :- Check magic number.
    StringC suffix = Extension(nfilename);
    if (suffix != "js")
      return typeid(void);
    // Try and load the header to make sure everything's ok.
    DPImageJSBaseBodyC jsInfo(nfilename,true);
    if(!jsInfo.ReadHeader())
      return typeid(void);
    // All seems fine.
    return typeid(ImageC<ByteYUV422ValueC>);
  }
  
  const std::type_info &
  FileFormatJSBodyC::ProbeSave(const StringC &nfilename,const std::type_info &obj_type,bool forceFormat) const {
    ONDEBUG(cerr << "FileFormatJSBodyC::ProbeSave() Called. Filename:'"<< nfilename <<"' Ext:'" << Extension(nfilename) << "'  LoadType:'" << TypeName(obj_type) << "'\n");

    if (!nfilename.IsEmpty() && nfilename[0] == '@')
      return typeid(void);

    StringC suffix = Extension(nfilename);
    if(!forceFormat) {
      if (suffix != "js")
        return typeid(void);
    }
    return typeid(ImageC<ByteYUV422ValueC>);
  }
  
  //: Create a input port for loading.
  // Will create an Invalid port if not supported.
  
  DPIPortBaseC FileFormatJSBodyC::CreateInput(IStreamC &in,const std::type_info &obj_type) const {
    ONDEBUG(cerr << "FileFormatJSBodyC::CreateInput(IStreamC &,const std::type_info &), Called. \n");
#if 0
    if(!in.good())
      return DPIPortBaseC();
    if(obj_type == typeid(ImageC<ByteYUV422ValueC>))
      return DPIImageJSC(in);
#endif
    return DPIPortBaseC();
  }
  
  //: Create a output port for saving.
  // Will create an Invalid port if not supported.
  
  DPOPortBaseC FileFormatJSBodyC::CreateOutput(OStreamC &out,const std::type_info &obj_type) const  {
#if 0
    if(!out.good())
      return DPOPortBaseC();
    if(obj_type == typeid(ImageC<ByteYUV422ValueC>))
      return DPOImageJSC(out);
#endif
    return DPOPortBaseC();
  }
  
  //: Create a input port for loading from file 'filename'.
  // Will create an Invalid port if not supported. <p>
  
  DPIPortBaseC FileFormatJSBodyC::CreateInput(const StringC &filename,const std::type_info &obj_type) const {
    ONDEBUG(cerr << "FileFormatJSBodyC::CreateInput(const StringC &,const std::type_info &), Called. \n");
    if(obj_type == typeid(ImageC<ByteYUV422ValueC>))
      return DPIImageJSC(filename);
    return DPIPortBaseC();
  }
  
  //: Create a output port for saving to file 'filename'..
  // Will create an Invalid port if not supported. <p>
  
  DPOPortBaseC FileFormatJSBodyC::CreateOutput(const StringC &filename,const std::type_info &obj_type) const {
    ONDEBUG(cerr << "FileFormatJSBodyC::CreateOutput(const StringC &,const std::type_info &), Called. \n");
    if(obj_type == typeid(ImageC<ByteYUV422ValueC>))
      return DPOImageJSC(filename);
    return DPOPortBaseC();
  }
  
  //: Get prefered IO type.
  
  const std::type_info &FileFormatJSBodyC::DefaultType() const 
  { return typeid(ImageC<ByteYUV422ValueC>); }
  
  
  // Some common cif formats.
  
  FileFormatJSC RegisterFileFormatJS;
}

