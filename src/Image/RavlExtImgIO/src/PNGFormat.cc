// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////
//! lib=RavlExtImgIO
//! file="Ravl/Image/ExternalImageIO/PNG/PNGFormat.cc"

#include <png.h>

#include "Ravl/Image/PNGFormat.hh"
#include "Ravl/Image/UInt16RGBValue.hh"
#include "Ravl/Image/ByteRGBAValue.hh"
#include "Ravl/TypeName.hh"
#include <ctype.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {

  void InitPNGFormat()
  {}

  
  // PNG ////////////////////////////////////////////////////////////////
  
  //: Constructor.
  
  FileFormatPNGBodyC::FileFormatPNGBodyC(bool nonly16Bit,const StringC &id,const StringC &desc)
    : FileFormatBodyC(id,desc),
      only16Bit(nonly16Bit)
  {}

  //: Try and choose best format for IO.
  
  const std::type_info &FileFormatPNGBodyC::ChooseFormat(const std::type_info &obj_type,int bit_depth,int colour_type,int interlace) const {
    if(bit_depth <= 8) {
      if(obj_type == typeid(ImageC<ByteRGBValueC>))
	return obj_type; // We know how do this as well.
      if(obj_type == typeid(ImageC<ByteRGBAValueC>))
	return obj_type; // We know how do this as well.  
      if(obj_type == typeid(ImageC<ByteT>))
	return obj_type; // We know how do this as well.
      
      // PNG doesn't know to convert 8 bits images to 16.
      if(obj_type == typeid(ImageC<UInt16T>))
	return typeid(ImageC<ByteT>);
      if(obj_type == typeid(ImageC<UInt16RGBValueC>)) 
	return typeid(ImageC<ByteRGBValueC>);
    }
    
    if(bit_depth == 16) {
      if(obj_type == typeid(ImageC<ByteRGBValueC>))
	return obj_type;
      if(obj_type == typeid(ImageC<ByteRGBAValueC>))
	return obj_type; // Need a 16 bit RGBX ??
      if(obj_type == typeid(ImageC<ByteT>))
	return obj_type;
      
      // Use some hints...
      if(obj_type == typeid(ImageC<SByteT>))
	return typeid(ImageC<ByteT>);
    }
    
    
    // Look for a single component hints...
    
    if(obj_type == typeid(ImageC<IntT>) || obj_type == typeid(ImageC<UIntT>) ||
       obj_type == typeid(ImageC<UInt16T>) || obj_type == typeid(ImageC<Int16T>) ||
       obj_type == typeid(ImageC<RealT>) 
       ) {
      if(bit_depth == 16) 
	return typeid(ImageC<UInt16T>);
      return typeid(ImageC<ByteT>);
    }
    
    // We could have a peak in the file and try and decide what's best here.
    // but just return an RGB image will work, just might be a little slower
    // in some cases.
    
    // Sooo do the default.
    
    if(bit_depth == 16) 
      return typeid(ImageC<UInt16RGBValueC>);
    return typeid(ImageC<ByteRGBValueC>);   
  }
  
  //: Try and choose best format for IO.
  
  const std::type_info &FileFormatPNGBodyC::ChooseFormat(const std::type_info &obj_type) const {
    if(!only16Bit) {
      if(obj_type == typeid(ImageC<ByteRGBValueC>))
	return obj_type; // We know how do this as well.
      if(obj_type == typeid(ImageC<ByteT>))
	return obj_type; // We know how do this as well.
      if(obj_type == typeid(ImageC<ByteRGBAValueC>))
	return obj_type; // We know how do this as well.  
      if(obj_type == typeid(ImageC<ByteT>)) 
	return obj_type; // We know how do this as well.    
    } else {
      
      if(obj_type == typeid(ImageC<ByteRGBValueC>))
	return typeid(ImageC<UInt16RGBValueC>);
      if(obj_type == typeid(ImageC<ByteRGBAValueC>))
	return typeid(ImageC<UInt16RGBValueC>); // Need a 16 bit RGBX ??
      if(obj_type == typeid(ImageC<ByteT>))
	return typeid(ImageC<UInt16T>); 
      if(obj_type == typeid(ImageC<ByteT>)) 
	return typeid(ImageC<UInt16T>); 
    }
    if(obj_type == typeid(ImageC<UInt16T>))
      return obj_type; // We know how do this as well.      
    if(obj_type == typeid(ImageC<UInt16RGBValueC>)) 
      return obj_type; // We know how do this as well.      
    
    
    // Use some hints...
    
    
    if(obj_type == typeid(ImageC<SByteT>)) {
      if(only16Bit) 
	return typeid(ImageC<UInt16T>);
      return typeid(ImageC<ByteT>);
    }
    
    // Look for a single component hints...
    
    if(obj_type == typeid(ImageC<IntT>) || obj_type == typeid(ImageC<UIntT>) ||
       obj_type == typeid(ImageC<UInt16T>) || obj_type == typeid(ImageC<Int16T>) ||
       obj_type == typeid(ImageC<RealT>) 
       )
      return typeid(ImageC<UInt16T>);
    
    // We could have a peak in the file and try and decide what's best here.
    // but just return an RGB image will work, just might be a little slower
    // in some cases.
    
    // Sooo do the default.
    
    if(only16Bit) 
      return typeid(ImageC<UInt16RGBValueC>);
    return typeid(ImageC<ByteRGBValueC>);   
  }
  
  //: Is stream in std stream format ?
  
  const std::type_info &
  FileFormatPNGBodyC::ProbeLoad(IStreamC &in,const std::type_info &obj_type) const {
    ONDEBUG(std::cerr << "FileFormatPNGBodyC::ProbeLoad(), called.. \n");
    if(!in.good())
      return typeid(void);
    ByteT buff[9];
    in.read((char*)buff,4);
    in.Unget((char *)buff,4);
    ONDEBUG(std::cerr << "FileFormatPNGBodyC::ProbeLoad(), called. " << hex << ((UIntT) buff[0])  << " " << ((UIntT) buff[1]) << " " << ((UIntT) buff[2])  << " " << ((UIntT) buff[3]) << dec << " \n");
    
    if(buff[0] != (ByteT) 0x89 || buff[1] != (ByteT)0x50 || buff[2] != (ByteT)0x4e ||  buff[3] != (ByteT)0x47)
      return typeid(void);
    
    streampos at = in.Tell();
    DPIImageIOPNGBaseC info(in);
    int bit_depth, colourType, interlace;
    
    if(!info.ReadHeaderInfo(bit_depth,colourType,interlace)) {
      in.Seek(at);
      return typeid(void); // Not a PNG ?!
    }
    in.Seek(at);
    return ChooseFormat(obj_type,bit_depth,colourType,interlace);
  }
  
  const std::type_info &
  FileFormatPNGBodyC::ProbeLoad(const StringC &nfilename,IStreamC &in,const std::type_info &obj_type) const {
    const std::type_info &pref = ProbeLoad(in,obj_type);
    ONDEBUG(std::cerr << "FileFormatPNGBodyC::ProbeLoad(), Req:" <<obj_type.name() << "  Ret:" << pref.name() << " \n");
    return  pref;
  }
  
  const std::type_info &
  FileFormatPNGBodyC::ProbeSave(const StringC &filename,const std::type_info &obj_type,bool forceFormat ) const {
    if(!forceFormat) {
      if(filename.IsEmpty() || filename[0]=='@') // Is this a special file ?
        return typeid(void);
      if(Extension(filename) != "png" && filename != "-")
	return typeid(void);
    }
    const std::type_info &pref = ChooseFormat(obj_type);
    ONDEBUG(std::cerr << "FileFormatPNGBodyC::ProbeSave(), Req:" << obj_type.name() << "  Ret:" << pref.name() << " \n");
    return pref;
  }
  
  //: Create a input port for loading from file 'filename'.
  // Will create an Invalid port if not supported. <p>
  DPIPortBaseC FileFormatPNGBodyC::CreateInput(const StringC &fn,const std::type_info &obj_type) const {
    if(obj_type == typeid(ImageC<ByteRGBValueC>))
      return DPIImagePNGC<ByteRGBValueC>(fn);
    if(obj_type == typeid(ImageC<ByteT>))
      return DPIImagePNGC<ByteT>(fn);
    if(obj_type == typeid(ImageC<ByteT>)) 
      return DPIImagePNGC<ByteT>(fn);
    if(obj_type == typeid(ImageC<UInt16T>)) 
      return DPIImagePNGC<UInt16T>(fn);
    if(obj_type == typeid(ImageC<ByteRGBAValueC>)) 
      return DPIImagePNGC<ByteRGBAValueC>(fn);
    if(obj_type == typeid(ImageC<UInt16RGBValueC>)) 
      return DPIImagePNGC<UInt16RGBValueC>(fn);
    return DPIPortBaseC();  
  }
  
  //: Create a output port for saving to file 'filename'..
  // Will create an Invalid port if not supported. <p>
  
  DPOPortBaseC FileFormatPNGBodyC::CreateOutput(const StringC &fn,const std::type_info &obj_type) const {
    if(obj_type == typeid(ImageC<ByteRGBValueC>))
      return DPOImagePNGC<ByteRGBValueC>(fn);
    if(obj_type == typeid(ImageC<ByteT>))
      return DPOImagePNGC<ByteT>(fn);
    if(obj_type == typeid(ImageC<ByteT>)) 
      return DPOImagePNGC<ByteT>(fn);
    if(obj_type == typeid(ImageC<UInt16T>)) 
      return DPOImagePNGC<UInt16T>(fn);
    if(obj_type == typeid(ImageC<ByteRGBAValueC>)) 
      return DPOImagePNGC<ByteRGBAValueC>(fn);
    if(obj_type == typeid(ImageC<UInt16RGBValueC>)) 
      return DPOImagePNGC<UInt16RGBValueC>(fn);
    return DPOPortBaseC();
  }
  
  //: Create a input port for loading.
  // Will create an Invalid port if not supported.
  
  
  DPIPortBaseC FileFormatPNGBodyC::CreateInput(IStreamC &in,const std::type_info &obj_type) const {
    if(obj_type == typeid(ImageC<ByteRGBValueC>))
      return DPIImagePNGC<ByteRGBValueC>(in);
    if(obj_type == typeid(ImageC<ByteT>))
      return DPIImagePNGC<ByteT>(in);
    if(obj_type == typeid(ImageC<ByteT>)) 
      return DPIImagePNGC<ByteT>(in);
    if(obj_type == typeid(ImageC<UInt16T>)) 
      return DPIImagePNGC<UInt16T>(in);
    if(obj_type == typeid(ImageC<ByteRGBAValueC>)) 
      return DPIImagePNGC<ByteRGBAValueC>(in);
    if(obj_type == typeid(ImageC<UInt16RGBValueC>)) 
      return DPIImagePNGC<UInt16RGBValueC>(in);
    return DPIPortBaseC();
  }

  //: Create a output port for saving.
  // Will create an Invalid port if not supported.
  
  DPOPortBaseC FileFormatPNGBodyC::CreateOutput(OStreamC &out,const std::type_info &obj_type) const  {
    if(obj_type == typeid(ImageC<ByteRGBValueC>))
      return DPOImagePNGC<ByteRGBValueC>(out);
    if(obj_type == typeid(ImageC<ByteT>))
      return DPOImagePNGC<ByteT>(out);
    if(obj_type == typeid(ImageC<ByteT>)) 
      return DPOImagePNGC<ByteT>(out);
    if(obj_type == typeid(ImageC<UInt16T>)) 
      return DPOImagePNGC<UInt16T>(out);
    if(obj_type == typeid(ImageC<ByteRGBAValueC>)) 
      return DPOImagePNGC<ByteRGBAValueC>(out);
    if(obj_type == typeid(ImageC<UInt16RGBValueC>)) 
      return DPOImagePNGC<UInt16RGBValueC>(out);
    return DPOPortBaseC();
  }
  
  //: Get prefered IO type.
  
  const std::type_info &FileFormatPNGBodyC::DefaultType() const  { 
    if(only16Bit)
      return typeid(ImageC<UInt16RGBValueC>);
    return typeid(ImageC<ByteRGBValueC>); 
  }
  
  FileFormatPNGC RegisterFileFormatPNG(false,"png","General png file IO. ");
  FileFormatPNGC RegisterFileFormatPNG16(true,"png16","16 bit only png file IO. ");
}

