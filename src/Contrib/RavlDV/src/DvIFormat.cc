// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlDV
//! file="Ravl/Contrib/DV/DvIFormat.cc"

#include "Ravl/Image/DvIFormat.hh"
#include "Ravl/Image/ImgIODv.hh"
#include <ctype.h>
#include "Ravl/TypeName.hh"
#define USE_DvWRITE 0

#define DPDEBUG 0 
#if DPDEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {
  
  void InitDvIFormat()
  {}
  
  // Dv ////////////////////////////////////////////////////////////////
  
  //: Constructor.
  
  FileFormatDvBodyC::FileFormatDvBodyC(const StringC &id,const StringC &desc)
    : FileFormatBodyC(id,desc)
  {}
  
  //: Try and choose best format for IO.
  
  const type_info &FileFormatDvBodyC::ChooseFormat(const type_info &obj_type) const  {
    return typeid(AVFrameC);   
  }
  
  //: Is stream in std stream format ?
  
  const type_info &
  FileFormatDvBodyC::ProbeLoad(IStreamC &in,const type_info &obj_type) const {
    ONDEBUG(cerr << "FileFormatDvBodyC::ProbeLoad(IStreamC &,...), called. \n");
    if(!in.good()) return typeid(void);

    //: Need to do a check to see whether the Dv is valid
    
    return typeid(AVFrameC);  
    
    //ONDEBUG(cerr << "FileFormatDvBodyC::ProbeLoad(), Not a Dv. \n");
    //return typeid(void); 
  }
  
  const type_info &
  FileFormatDvBodyC::ProbeLoad(const StringC &nfilename,IStreamC &in,const type_info &obj_type) const {
    const type_info &pref = ProbeLoad(in,obj_type);
    ONDEBUG(cerr << "FileFormatDvBodyC::ProbeLoad(), Req:" <<obj_type.name() << "  Ret:" << pref.name() << " \n");
    ONDEBUG(cerr << "FileFormatDvBodyC::ProbeLoad(), Req:" <<TypeName(obj_type.name()) << "  Ret:" << TypeName(pref.name()) << " \n");

    if(Extension(nfilename) != StringC("dv") && nfilename != "-")
      return typeid(void);
    //: Need to do a check to see whether the Dv is valid
    return  pref;
  }
  
  const type_info &
  FileFormatDvBodyC::ProbeSave(const StringC &filename,const type_info &obj_type,bool forceFormat ) const {
#if USE_DvWRITE
    if(!forceFormat) {
      if(Extension(filename) != "dv" && filename != "-")
	return typeid(void);
    }
    const type_info &pref = ChooseFormat(obj_type);
    ONDEBUG(cerr << "FileFormatDvBodyC::ProbeSave(), Req:" << obj_type.name() << "  Ret:" << pref.name() << " \n");
    return pref;
#else
    return typeid(void);   
#endif
  }
  
  //: Create a input port for loading from file 'filename'.
  // Will create an Invalid port if not supported. <p>
  DPIPortBaseC FileFormatDvBodyC::CreateInput(const StringC &fn,const type_info &obj_type) const {
    ONDEBUG ( cerr << " FileFormatDvBodyC::CreateInput" ) ; 
    return DPIImageDvC(fn);
  }
  
  //: Create a output port for saving to file 'filename'..
  // Will create an Invalid port if not supported. <p>
  
  DPOPortBaseC FileFormatDvBodyC::CreateOutput(const StringC &fn,const type_info &obj_type) const {
#if USE_DvWRITE
    return DPOImageDvByteRGBC(fn);
#else
    return DPOPortBaseC();  
#endif
  }
  
  //: Create a input port for loading.
  // Will create an Invalid port if not supported.
  
  DPIPortBaseC FileFormatDvBodyC::CreateInput(IStreamC &in,const type_info &obj_type) const { 
    return DPIImageDvC(in); 
  }
  
  //: Create a output port for saving.
  // Will create an Invalid port if not supported.
  
  DPOPortBaseC FileFormatDvBodyC::CreateOutput(OStreamC &out,const type_info &obj_type) const  {
#if USE_DvWRITE
    return DPOImageDvByteRGBC(out);
#else
    return DPOPortBaseC();  
#endif
  }
  
  //: Get prefered IO type.
  
  const type_info &FileFormatDvBodyC::DefaultType() const  { 
    //return typeid(ImageC<ByteRGBValueC>); 
    return typeid (AVFrameC) ;  
  }

#if USE_DvWRITE
  FileFormatDvC RegisterFileFormatDv("dv","Dv file input. ");
#else
  FileFormatDvC RegisterFileFormatDv("dv","Dv file IO. (only reading supported)");
#endif
  
}
