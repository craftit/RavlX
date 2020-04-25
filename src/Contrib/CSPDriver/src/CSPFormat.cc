// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=CSPDriver
//! file="Ravl/Contrib/ClipStationPro/CSPFormat.cc"

#include "Ravl/Image/CSPFormat.hh"
#include "Ravl/Image/ImgIOCSP.hh"
#include "Ravl/TypeName.hh"

#define DPDEBUG 0
#if DPDEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {

  void InitCSPFormat()
  {}
  
  // CSP ////////////////////////////////////////////////////////////////
  
  
  //: Constructor.
  FileFormatCSPBodyC::FileFormatCSPBodyC(const StringC &nvName)
    : FileFormatBodyC(nvName,StringC("CSP driver.")),
      vName(nvName)
  {}
  
  

  const type_info &
  FileFormatCSPBodyC::ProbeLoad(IStreamC &in,const type_info &obj_type) const
  { return typeid(void); }


  
  const type_info &
  FileFormatCSPBodyC::ProbeLoad(const StringC &filename,IStreamC &in,const type_info &obj_type) const
  { 
    ONDEBUG(cerr << "FileFormatCSPBodyC::ProbeLoad(const StringC &,IStreamC &,const type_info &), Called. " << filename << " \n");
    if(filename.length() == 0)
      return typeid(void);
    if(filename[0] != '@')
      return typeid(void);
    if(ExtractDevice(filename) != "CSP")
      return typeid(void);
    ONDEBUG(cerr << "FileFormatCSPBodyC::ProbeLoad(), Found. \n");
    if ( obj_type == typeid(ImageC<ByteYUV422ValueC>) )  // yuv images
    	return typeid(ImageC<ByteYUV422ValueC>); 
    else if ( obj_type == typeid(ImageC<ByteRGBValueC>) ) // rgb requested 
    	return typeid(ImageC<ByteRGBValueC> ) ; 
    else return typeid ( ImageC<ByteRGBValueC>) ; // default type 
  }


  
  const type_info &
  FileFormatCSPBodyC::ProbeSave(const StringC &filename,const type_info &obj_type,bool forceFormat) const
{ 
  ONDEBUG(cerr << "FileFormatCSPBodyC::ProbeSave(const StringC& filename, const type_info& ), called " << filename << "\n" ) ; 
  if (filename.length() == 0)
    return typeid(void) ; 
  if (filename[0] != '@') 
    return typeid(void) ;  
  if (ExtractDevice(filename) != "CSP") 
    return typeid(void) ; 
  ONDEBUG(cerr << "FileFormatCSPBodyC::ProbeSave(), Found. \n"); 
  if ( obj_type == typeid(ImageC<ByteYUV422ValueC>) )  // yuv images
    	return typeid(ImageC<ByteYUV422ValueC>); 
    else if ( obj_type == typeid(ImageC<ByteRGBValueC>) ) // rgb requested 
    	return typeid(ImageC<ByteRGBValueC> ) ; 
    else return typeid ( ImageC<ByteRGBValueC>) ; // default type
}  



  //: Create a input port for loading.
  // Will create an Invalid port if not supported.
  DPIPortBaseC FileFormatCSPBodyC::CreateInput(IStreamC &in,const type_info &obj_type) const
{ return DPIPortBaseC(); }  
  


//: Create a output port for saving.
// Will create an Invalid port if not supported.
DPOPortBaseC FileFormatCSPBodyC::CreateOutput(OStreamC &out,const type_info &obj_type) const 
{ return DPOPortBaseC(); }




  //: Create a input port for loading from file 'filename'.
  // Will create an Invalid port if not supported. <p>  
  DPIPortBaseC FileFormatCSPBodyC::CreateInput(const StringC &filename,const type_info &obj_type) const {
    ONDEBUG(cerr << "FileFormatCSPBodyC::CreateInput(const StringC &,const type_info &), Called. \n");
    if(filename.length() == 0)
      return DPIPortBaseC();
    if(filename[0] != '@')
      return DPIPortBaseC();
    StringC fn = ExtractParams(filename);
    if(fn == "")
      fn = "PCI,card=0";
    if(obj_type == typeid(ImageC<ByteYUV422ValueC>)) 
      return DPIImageClipStationProC<ByteYUV422ValueC>(fn);
    else if ( obj_type == typeid(ImageC<ByteRGBValueC> )) 
    	return DPIImageClipStationProC<ByteRGBValueC> (fn) ;  
    return DPIPortBaseC();
  }
  


  //: Create a output port for saving to file 'filename'..
  // Will create an Invalid port if not supported. <p>
  
  DPOPortBaseC FileFormatCSPBodyC::CreateOutput(const StringC &filename,const type_info &obj_type) const
  { 
    ONDEBUG (cerr << "FielFormatCSPBodyC::CreateOutput(const StringC&, const type_info&) const, Called \n"); 
    if (filename.length() ==0) 
      return DPOPortBaseC() ; 
    if (filename[0] != '@') 
      return DPOPortBaseC() ;
    StringC fn = ExtractParams(filename); 
    if (fn == "")  
      fn = "PCI,card=0"; 
    if (obj_type == typeid(ImageC<ByteYUV422ValueC> )) 
      return DPOImageClipStationProC<ByteYUV422ValueC> (fn) ;
    else if ( obj_type == typeid(ImageC<ByteRGBValueC> )) 
      return DPOImageClipStationProC<ByteRGBValueC>  (fn) ; 
    return DPOPortBaseC(); 
  }
  
  //: Get prefered IO type.
  
  const type_info &FileFormatCSPBodyC::DefaultType() const 
  { return typeid(ImageC<ByteYUV422ValueC>); }
  
  // Some common cif formats.
  
  FileFormatCSPC RegisterFileFormatCSP("csp");
}
