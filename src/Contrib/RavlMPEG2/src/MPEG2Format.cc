// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlMPEG2
//! file="Ravl/Contrib/MPEG/mpeg2dec/MPEG2Format.cc"
//! author="Charles Galambos"

#include "Ravl/Image/MPEG2Format.hh"
#include "Ravl/Image/ImgIOmpeg2dec.hh"
#include <ctype.h>

#define DPDEBUG 0
#if DPDEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {
  
  void InitMPEG2Format()
  {}
  
  // MPEG ////////////////////////////////////////////////////////////////
  
  //: Constructor.
  
  FileFormatMPEG2BodyC::FileFormatMPEG2BodyC()
    : FileFormatBodyC("mpeg2","MPEG 2 file input. ")
  {}
  
  
  //: Is stream in std stream format ?
  
  const type_info &
  FileFormatMPEG2BodyC::ProbeLoad(IStreamC &in,const type_info &obj_type) const {
    ONDEBUG(cerr << "FileFormatMPEG2BodyC::ProbeLoad(IStreamC &,...), called. \n");
    if(!in.good())
      return typeid(void);
    ByteT buff[9];
    in.read((char*)buff,4);
    in.Unget((char *)buff,4);
    ONDEBUG(cerr << "FileFormatMPEG2BodyC::ProbeLoad(), 0= " << hex << ((UIntT) buff[0])  << " " << ((UIntT) buff[1]) << " " << ((UIntT) buff[2])  << " " << ((UIntT) buff[3]) << dec << " \n");
    
    if((((UIntT) buff[0]) == 0) && (((UIntT) buff[1]) == 0)) {
      if(((UIntT) buff[2]) == 0x01 && ((UIntT) buff[3]) == 0xb3 )
	return typeid(ImageC<ByteYUVValueC>);
    }
    
    ONDEBUG(cerr << "FileFormatMPEG2BodyC::ProbeLoad(), Not a MPEG. \n");
    return typeid(void); 
  }
  
  const type_info &
  FileFormatMPEG2BodyC::ProbeLoad(const StringC &nfilename,IStreamC &in,const type_info &obj_type) const {
    //const type_info &pref = ProbeLoad(in,obj_type);
    StringC ext = Extension(nfilename);
    if(ext == "mpeg")
      return typeid(ImageC<ByteYUVValueC>);
    if(ext == "vob")
      return typeid(ImageC<ByteYUVValueC>);
    //ONDEBUG(cerr << "FileFormatMPEG2BodyC::ProbeLoad(), Req:" <<obj_type.name() << "  Ret:" << pref.name() << " \n");
    return typeid(void);
  }
  
  const type_info &
  FileFormatMPEG2BodyC::ProbeSave(const StringC &filename,const type_info &obj_type,bool forceFormat ) const {
    return typeid(void);   
  }
  
  //: Create a input port for loading from file 'filename'.
  // Will create an Invalid port if not supported. <p>
  
  DPIPortBaseC FileFormatMPEG2BodyC::CreateInput(const StringC &fn,const type_info &obj_type) const {
    StringC ext = Extension(fn);
    if(ext == "vob")
      return DPIImagempeg2decC<ByteYUVValueC>(fn,true);
    return DPIImagempeg2decC<ByteYUVValueC>(fn);
  }
  
  //: Create a output port for saving to file 'filename'..
  // Will create an Invalid port if not supported. <p>
  
  DPOPortBaseC FileFormatMPEG2BodyC::CreateOutput(const StringC &fn,const type_info &obj_type) const {
    return DPOPortBaseC();  
  }
  
  //: Create a input port for loading.
  // Will create an Invalid port if not supported.
  
  DPIPortBaseC FileFormatMPEG2BodyC::CreateInput(IStreamC &in,const type_info &obj_type) const { 
    //return DPIImagempeg2decC<ByteYUVValueC>(in);
    return DPIPortBaseC();
  }
  
  //: Create a output port for saving.
  // Will create an Invalid port if not supported.
  
  DPOPortBaseC FileFormatMPEG2BodyC::CreateOutput(OStreamC &out,const type_info &obj_type) const  {
    return DPOPortBaseC();  
  }
  
  //: Get prefered IO type.
  
  const type_info &FileFormatMPEG2BodyC::DefaultType() const  { 
    return typeid(ImageC<ByteYUVValueC>); 
  }
  
  static FileFormatMPEG2C Init;  
}
