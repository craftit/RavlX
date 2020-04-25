// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlImageIO
//! file="Ravl/Image/ImageIO/PNMFormat.cc"

#include "Ravl/BinStream.hh"
#include "Ravl/Image/PNMFormat.hh"
#include "Ravl/Image/ImgIOPNM.hh"
#include "Ravl/TypeName.hh"
#include <ctype.h>

#define DPDEBUG 0

#if DPDEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {

#ifdef __sgi__
  static ImageC<bool> fixSillysgibug;
#endif
  
  void InitPNMFormat() {
  }
  
  
  // PPM ////////////////////////////////////////////////////////////////
  
  //: Constructor.
  
  FileFormatPPMBodyC::FileFormatPPMBodyC()
    : FileFormatBodyC("ppm","PPM rgb colour images.")
  {}
  
  //: Is stream in std stream format ?
  
  const std::type_info &
  FileFormatPPMBodyC::ProbeLoad(IStreamC &in,const std::type_info &obj_type) const {
    if(!in.good())
      return typeid(void);
    char buff[3];
    in.read(buff,3);
    in.Unget(buff,3);
    if(buff[0] != 'P')
      return typeid(void);
    switch(buff[1])
      {
      case '3': // PPM ascii
      case '6': // PPM raw
	break;
      default:
	return typeid(void);
      }
    if(!(isspace(buff[2]) || buff[2] == '#'))
      return typeid(void);
    return typeid(ImageC<ByteRGBValueC>);
  }

  const std::type_info &
  FileFormatPPMBodyC::ProbeLoad(const StringC &nfilename,IStreamC &in,const std::type_info &obj_type) const {
    //StringC filename(nfilename);
    //  if(obj_type != typeid(ImageC<ByteRGBValueC>))
    //    return false; // Can only deal with rgb at the moment.
    // For Load, use stream probe its more reliable than extentions.
    return ProbeLoad(in,obj_type);
  }
  
  const std::type_info &
  FileFormatPPMBodyC::ProbeSave(const StringC &filename,const std::type_info &obj_type,bool forceFormat ) const {
    if(forceFormat)
      return typeid(ImageC<ByteRGBValueC>);
    if(Extension(filename) != StringC("ppm") && filename != "-")
      return typeid(void);
    return typeid(ImageC<ByteRGBValueC>);
  }
  
  //: Create a input port for loading.
  // Will create an Invalid port if not supported.
  
  DPIPortBaseC FileFormatPPMBodyC::CreateInput(IStreamC &in,const std::type_info &obj_type) const {
    if(obj_type == typeid(ImageC<ByteRGBValueC>))
      return DPIImagePNMByteRGBC(in);
    return DPIPortBaseC();
  }
  
  //: Create a output port for saving.
  // Will create an Invalid port if not supported.
  
  DPOPortBaseC FileFormatPPMBodyC::CreateOutput(OStreamC &out,const std::type_info &obj_type) const  {
    if(obj_type == typeid(ImageC<ByteRGBValueC>))
      return  DPOImagePNMByteRGBC(out);
    return DPOPortBaseC();
  }
  
  //: Get prefered IO type.
  
  const std::type_info &FileFormatPPMBodyC::DefaultType() const 
  { return typeid(ImageC<ByteRGBValueC>); }
  
  
  /// PGM ///////////////////////////////////////////////////////////////
  
  //: Default constructor.
  
  FileFormatPGMBodyC::FileFormatPGMBodyC()
    : FileFormatBodyC("pgm","PGM byte grey level images.")
  {}
  
  //: Is stream in std stream format ?
  
  const std::type_info &
  FileFormatPGMBodyC::ProbeLoad(IStreamC &in,const std::type_info & ONDEBUG(obj_type)) const {
    if(!in.good())
      return typeid(void);
    char buff[3];
    in.read(buff,3);
    in.Unget(buff,3);
    if(buff[0] != 'P')
      return typeid(void);
    switch(buff[1])
      {
      case '2': // PGM ascii
      case '5': // PGM raw
	break;
      default:
	return typeid(void);
      }
    if(!(isspace(buff[2]) || buff[2] == '#'))
      return typeid(void);
    ONDEBUG(cerr << "FileFormatPGMBodyC::ProbeLoad(IStreamC) File is in PGM format!  OT:" << TypeName(obj_type) << endl);
    return typeid(ImageC<ByteT>);
  }
  
  const std::type_info &
  FileFormatPGMBodyC::ProbeLoad(const StringC &nfilename,IStreamC &in,const std::type_info &obj_type) const {
    //StringC filename(nfilename);
    ONDEBUG(cerr << "FileFormatPGMBodyC::ProbeLoad()  FN:" << nfilename << "  OT:" << TypeName(obj_type) << endl);
    // For Load, use stream probe its more reliable than extentions.
    return ProbeLoad(in,obj_type);
  }

  const std::type_info &
  FileFormatPGMBodyC::ProbeSave(const StringC &nfilename,const std::type_info & ONDEBUG(obj_type),bool forceFormat ) const {
    StringC filename(nfilename);
    ONDEBUG(cerr << "FileFormatPGMBodyC::ProbeSave()  FN:" << nfilename << "  OT:" << TypeName(obj_type) << endl);
    if(!forceFormat) {
      if(Extension(filename) != StringC("pgm") && filename != "-")
	return typeid(void);
    }
    return typeid(ImageC<ByteT>);
  }
  
  //: Create a input port for loading.
  // Will create an Invalid port if not supported.
  
  DPIPortBaseC FileFormatPGMBodyC::CreateInput(IStreamC &in,const std::type_info &obj_type) const {
    ONDEBUG(cerr << "FileFormatPGMBodyC::CreateInput()  FN:" << in.Name() << "  T:" << TypeName(obj_type) << endl);
    if(obj_type == typeid(ImageC<ByteT>))
      return  DPIImagePNMByteGreyC(in);
    return DPIPortBaseC();
  }
  
  //: Create a output port for saving.
  // Will create an Invalid port if not supported.
  
  DPOPortBaseC FileFormatPGMBodyC::CreateOutput(OStreamC &out,const std::type_info &obj_type) const {
    if(obj_type == typeid(ImageC<ByteT>))
      return  DPOImagePNMByteGreyC(out);
    return DPOPortBaseC();
  }
  
  
  const std::type_info &FileFormatPGMBodyC::DefaultType() const
  { return typeid(ImageC<ByteT>); }
 
  /// PBM ///////////////////////////////////////////////////////////////
  
  //: Default constructor.
  
  FileFormatPBMBodyC::FileFormatPBMBodyC()
    : FileFormatBodyC("pbm","PBM binary images.")
  {}

  //: Is stream in std stream format ?
  
  const std::type_info &
  FileFormatPBMBodyC::ProbeLoad(IStreamC &in,const std::type_info &obj_type) const {
    if(!in.good())
      return typeid(void);
    char buff[3];
    in.read(buff,3);
    in.Unget(buff,3);
    if(buff[0] != 'P')
      return typeid(void);
    switch(buff[1])
      {
      case '1': // PBM ascii
      case '4': // PBM raw
	break;
      default:
	return typeid(void);
      }
    if(!(isspace(buff[2]) || buff[2] == '#'))
      return typeid(void);
    if(obj_type == typeid(ImageC<ByteT>))
      return typeid(ImageC<ByteT>);
    return typeid(ImageC<ByteT>);
  }
  
  const std::type_info &
  FileFormatPBMBodyC::ProbeLoad(const StringC &nfilename,IStreamC &in,const std::type_info &obj_type) const {
    //   std::cerr << "\n\n In ProbeLoad " ; 
    //StringC filename(nfilename);
    //if(obj_type != typeid(ImageC<bool>) && obj_type != typeid(ImageC<ByteT>))
    //  return typeid(void);
    // For Load, use stream probe its more reliable than extentions.
    return ProbeLoad(in,obj_type);
  }
  

  const std::type_info &
  FileFormatPBMBodyC::ProbeSave(const StringC &nfilename,const std::type_info &obj_type,bool forceFormat ) const {
    StringC filename(nfilename);
    if(!forceFormat) {
      if(Extension(filename) != StringC("pbm")  && filename != "-")
	return typeid(void);
    }
    if(obj_type == typeid(ImageC<ByteT>))
      return typeid(ImageC<ByteT>);
    return typeid(ImageC<ByteT>);
  }
  
  
  //: Create a input port for loading.
  // Will create an Invalid port if not supported.
  
  DPIPortBaseC FileFormatPBMBodyC::CreateInput(IStreamC &in,const std::type_info &obj_type) const {
    if(obj_type == typeid(ImageC<ByteT>))
	return  DPIImagePNMByteGreyC(in); 
    return DPIPortBaseC();
  }
  
  //: Create a output port for saving.
  // Will create an Invalid port if not supported.
  
  DPOPortBaseC FileFormatPBMBodyC::CreateOutput(OStreamC &out,const std::type_info &obj_type) const  {
    if(obj_type == typeid(ImageC<ByteT>))
      return DPOImagePNMByteGreyC(out); // This knows how to load pbm's into byte images.
    return DPOPortBaseC(); // Can't actual write at the moment.
  }
  
  //: Get prefered IO type.
  
  const std::type_info &FileFormatPBMBodyC::DefaultType() const 
  { return typeid(ImageC<ByteT>); }
  
  //////////////////////////////////////////////////////////////////
  
  FileFormatPBMC RegisterFileFormatPBM;
  FileFormatPGMC RegisterFileFormatPGM;
  FileFormatPPMC RegisterFileFormatPPM;
  
  static TypeNameC type2(typeid(ImageC<bool>),"ImageC<bool>");
}
