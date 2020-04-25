// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
//////////////////////////////////////////////////////////////////
//! lib=RavlLibMPEG2
//! author = "Charles Galambos"
//! file="Ravl/Contrib/MPEG/LibMPEG2/LibMPEG2Format.cc"

#include "Ravl/Image/LibMPEG2Format.hh"
#include "Ravl/Image/ImgIOMPEG2.hh"
#include "Ravl/Image/MPEG2Demux.hh"
#include <ctype.h>
#include "Ravl/DP/ByteFileIO.hh"
#include "Ravl/DP/SPortAttach.hh"

#define DPDEBUG 0
#if DPDEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {
  
  void InitLibMPEG2Format()
  {}
  
  //: Constructor.
  
  FileFormatLibMPEG2BodyC::FileFormatLibMPEG2BodyC()
    : FileFormatBodyC("mpeg2","LibMPEG 2 file input. ")
  {}
  
  
  //: Is stream in std stream format ?
  
  const type_info &
  FileFormatLibMPEG2BodyC::ProbeLoad(IStreamC &in,const type_info &obj_type) const {
    ONDEBUG(cerr << "FileFormatLibMPEG2BodyC::ProbeLoad(IStreamC &,...), called. \n");
    if(!in.good())
      return typeid(void);
    ByteT buff[9];
    in.read((char*)buff,4);
    in.Unget((char *)buff,4);
    ONDEBUG(cerr << "FileFormatLibMPEG2BodyC::ProbeLoad(), 0= " << hex << ((UIntT) buff[0])  << " " << ((UIntT) buff[1]) << " " << ((UIntT) buff[2])  << " " << ((UIntT) buff[3]) << dec << " \n");
    
    if((((UIntT) buff[0]) == 0) && (((UIntT) buff[1]) == 0)) {
      if(((UIntT) buff[2]) == 0x01 && ((UIntT) buff[3]) == 0xb3 )
        return typeid(ImageC<ByteRGBValueC>);
    }
    
    ONDEBUG(cerr << "FileFormatLibMPEG2BodyC::ProbeLoad(), Not a LibMPEG. \n");
    return typeid(void); 
  }
  
  const type_info &
  FileFormatLibMPEG2BodyC::ProbeLoad(const StringC &nfilename,IStreamC &in,const type_info &obj_type) const {
    //const type_info &pref = ProbeLoad(in,obj_type);

    if(!in.good())
      return typeid(void);
    ByteT buff[9];
    in.read((char*)buff,4);
    in.Unget((char *)buff,4);
    ONDEBUG(cerr << "FileFormatLibMPEG2BodyC::ProbeLoad(), 0= " << hex << ((UIntT) buff[0])  << " " << ((UIntT) buff[1]) << " " << ((UIntT) buff[2])  << " " << ((UIntT) buff[3]) << dec << " \n");
    
    if((((UIntT) buff[0]) == 0) && (((UIntT) buff[1]) == 0)) {
      if(((UIntT) buff[2]) == 0x01 && (((UIntT) buff[3]) == 0xb3 || ((UIntT) buff[3]) == 0xba) )
        return typeid(ImageC<ByteRGBValueC>);
    }
    
    StringC ext = Extension(nfilename);
    if(ext == "mpeg" || ext == "mpg" || ext == "vob")
      return typeid(ImageC<ByteRGBValueC>);
    //ONDEBUG(cerr << "FileFormatLibMPEG2BodyC::ProbeLoad(), Req:" <<obj_type.name() << "  Ret:" << pref.name() << " \n");
    return typeid(void);
  }
  
  const type_info &
  FileFormatLibMPEG2BodyC::ProbeSave(const StringC &filename,const type_info &obj_type,bool forceFormat ) const {
    return typeid(void);   
  }
  
  //: Create a input port for loading from file 'filename'.
  // Will create an Invalid port if not supported. <p>
  
  DPIPortBaseC FileFormatLibMPEG2BodyC::CreateInput(const StringC &fn,const type_info &obj_type) const {
    // Check for a multiplexed stream
    bool system = false;
    IStreamC in(fn, true);
    if (in.good())
    {
      ByteT buf[4];
      in.read((char*)buf, 4);
      if (((UIntT)buf[3]) == 0xba)
        system = true;
    }
    else
      return DPIPortBaseC();
    in.Close();
    
    // Open the stream
    if (system)
      return DPIByteFileC(fn) >> MPEG2DemuxC(0xe0) >> ImgILibMPEG2C(false);
    return SPort(DPIByteFileC(fn) >> ImgILibMPEG2C(true));
  }
  
  //: Create a output port for saving to file 'filename'..
  // Will create an Invalid port if not supported. <p>
  
  DPOPortBaseC FileFormatLibMPEG2BodyC::CreateOutput(const StringC &fn,const type_info &obj_type) const {
    return DPOPortBaseC();  
  }
  
  //: Create a input port for loading.
  // Will create an Invalid port if not supported.
  
  DPIPortBaseC FileFormatLibMPEG2BodyC::CreateInput(IStreamC &in,const type_info &obj_type) const { 
    //return DPIImagempeg2decC<ByteRGBValueC>(in);
    return DPIPortBaseC();
  }
  
  //: Create a output port for saving.
  // Will create an Invalid port if not supported.
  
  DPOPortBaseC FileFormatLibMPEG2BodyC::CreateOutput(OStreamC &out,const type_info &obj_type) const  {
    return DPOPortBaseC();  
  }
  
  //: Get prefered IO type.
  
  const type_info &FileFormatLibMPEG2BodyC::DefaultType() const  { 
    return typeid(ImageC<ByteRGBValueC>); 
  }
  
  static FileFormatLibMPEG2C Init;  
}
