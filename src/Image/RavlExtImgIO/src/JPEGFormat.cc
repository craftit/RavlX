// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////
//! lib=RavlExtImgIO
//! file="Ravl/Image/ExternalImageIO/JPEG/JPEGFormat.cc"

#include "Ravl/TypeName.hh"
#include "Ravl/Image/JPEGFormat.hh"
#include "Ravl/Image/ImgIOJPeg.hh"
#include "Ravl/Image/CompressedImageJPEG.hh"
#include "Ravl/TypeName.hh"
#include <ctype.h>

#define DPDEBUG 0

#if DPDEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {

  void InitJPEGFormat()
  {}
  
  // JPEG ////////////////////////////////////////////////////////////////
  
  //: Constructor.
  
  FileFormatJPEGBodyC::FileFormatJPEGBodyC()
    : FileFormatBodyC("jpeg","JPEG rgb colour images."),
      compression(75),
      pri(0),
      asSequence(false)
  {}
  
  //: Constructor.
  
  FileFormatJPEGBodyC::FileFormatJPEGBodyC(int comp,int npri,bool nAsSequence,const StringC &name,const StringC &desc)
    : FileFormatBodyC(name,desc),
      compression(comp),
      pri(npri),
      asSequence(nAsSequence)
  {}
  
  class DoubleImageC;
  
#ifdef __sgi__
  static ImageC<SByteT> stupsgibugfix1;
  static ImageC<Int16T> stupsgibugfix2;
#endif
  
  //: Try and choose best format for IO.
  
  const std::type_info &FileFormatJPEGBodyC::ChooseFormat(const std::type_info &obj_type) const {
    if(obj_type == typeid(CompressedImageJPEGC))
      return obj_type;

    if(obj_type == typeid(ImageC<ByteRGBValueC>))
      return obj_type; // We know how do this as well.
    if(obj_type == typeid(ImageC<ByteYUVValueC>))
      return obj_type; // We know how do this as well.
    if(obj_type == typeid(ImageC<ByteT>))
      return obj_type; // We know how do this as well.
    
    // Look for a single component hint...
    if(obj_type == typeid(ImageC<IntT>) || obj_type == typeid(ImageC<UIntT>) ||
       obj_type == typeid(ImageC<UInt16T>) || obj_type == typeid(ImageC<Int16T>) ||
       obj_type == typeid(ImageC<SByteT>) || obj_type == typeid(ImageC<RealT>) 
       )
      return typeid(ImageC<ByteT>);
    
    // We could have a peak in the file and try and decide what's best here.
    // but just return an RGB image will work, just might be a little slower
    // in some cases.
    
    // Sooo do the default.
    
    return typeid(ImageC<ByteRGBValueC>);   
  }
  
  //: Is stream in std stream format ?
  
  const std::type_info &
  FileFormatJPEGBodyC::ProbeLoad(IStreamC &in,const std::type_info &obj_type) const {
    ONDEBUG(std::cerr << "FileFormatJPEGBodyC::ProbeLoad(), called. \n");
    if(!in.good())
      return typeid(void);
    ByteT buff[3];
    in.read((char*)buff,2);
    in.Unget((char *)buff,2);
    ONDEBUG(std::cerr << "FileFormatJPEGBodyC::ProbeLoad(), called. " << hex << ((UIntT) buff[0])  << " " << ((UIntT) buff[1])  << dec << " \n");
    if(buff[0] != ((ByteT) 0xff))
      return typeid(void);
    if(buff[1] != ((ByteT) 0xd8))
      return typeid(void);
    return ChooseFormat(obj_type);
  }
  
  const std::type_info &
  FileFormatJPEGBodyC::ProbeLoad(const StringC &nfilename,IStreamC &in,const std::type_info &obj_type) const {
    //StringC filename(nfilename);
    //  if(obj_type != typeid(ImageC<ByteRGBValueC>))
    //    return FALSE; // Can only deal with rgb at the moment.
    // For Load, use stream probe its more reliable than extentions.
    return ProbeLoad(in,obj_type);
  }
  
  const std::type_info &
  FileFormatJPEGBodyC::ProbeSave(const StringC &filename,const std::type_info &obj_type,bool forceFormat ) const {
    if(forceFormat)
      return ChooseFormat(obj_type);
    if(filename.IsEmpty() || filename[0]=='@') // Is this a special file ?
      return typeid(void);
    if(Extension(filename) != "jpg" && filename != "-")
      return typeid(void);
    const std::type_info &ret_type = ChooseFormat(obj_type);
    ONDEBUG(std::cerr << "FileFormatJPEGBodyC::ProbeSave(), Using:'" << ret_type.name() << " Requested:'" << obj_type.name() << "'\n");
    return ret_type;
  }
  
  
  //: Create a input port for loading from file 'filename'.
  // Will create an Invalid port if not supported. <p>
  DPIPortBaseC FileFormatJPEGBodyC::CreateInput(const StringC &filename,const std::type_info &obj_type) const {  
    if(obj_type == typeid(ImageC<ByteRGBValueC>))
      return DPIImageJPegByteRGBC(filename);
    if(obj_type == typeid(ImageC<ByteYUVValueC>))
      return DPIImageJPegByteYUVC(filename);
    if(obj_type == typeid(ImageC<ByteT>))
      return DPIImageJPegByteGreyC(filename);

    if (obj_type == typeid(CompressedImageJPEGC))
      return DPIImageJPEGCompressedC(filename);

    return DPIPortBaseC();
  }
  
  //: Create a output port for saving to file 'filename'..
  // Will create an Invalid port if not supported. <p>
  DPOPortBaseC FileFormatJPEGBodyC::CreateOutput(const StringC &filename,const std::type_info &obj_type) const {
    if(obj_type == typeid(ImageC<ByteRGBValueC>))
      return  DPOImageJPegByteRGBC(filename,compression);
    if(obj_type == typeid(ImageC<ByteYUVValueC>))
      return  DPOImageJPegByteYUVC(filename,compression);
    if(obj_type == typeid(ImageC<ByteT>))
      return  DPOImageJPegByteGreyC(filename,compression);

    if (obj_type == typeid(CompressedImageJPEGC))
      return DPOImageJPEGCompressedC(filename);

    return DPOPortBaseC();
  }
  
  //: Create a input port for loading.
  // Will create an Invalid port if not supported.

  DPIPortBaseC FileFormatJPEGBodyC::CreateInput(IStreamC &in,const std::type_info &obj_type) const {
    if(obj_type == typeid(ImageC<ByteRGBValueC>))
      return DPIImageJPegByteRGBC(in);
    if(obj_type == typeid(ImageC<ByteYUVValueC>))
      return DPIImageJPegByteYUVC(in);
    if(obj_type == typeid(ImageC<ByteT>))
      return DPIImageJPegByteGreyC(in);

    if (obj_type == typeid(CompressedImageJPEGC))
      return DPIImageJPEGCompressedC(in);

    return DPIPortBaseC();
  }
  
  //: Create a output port for saving.
  // Will create an Invalid port if not supported.
  
  DPOPortBaseC FileFormatJPEGBodyC::CreateOutput(OStreamC &out,const std::type_info &obj_type) const  {
    if(obj_type == typeid(ImageC<ByteRGBValueC>))
      return  DPOImageJPegByteRGBC(out,compression);
    if(obj_type == typeid(ImageC<ByteYUVValueC>))
      return  DPOImageJPegByteYUVC(out,compression);
    if(obj_type == typeid(ImageC<ByteT>))
      return  DPOImageJPegByteGreyC(out,compression);

    if (obj_type == typeid(CompressedImageJPEGC))
      return DPOImageJPEGCompressedC(out);

    return DPOPortBaseC();
  }
  
  //: Get prefered IO type.
  
  const std::type_info &FileFormatJPEGBodyC::DefaultType() const { 
    return typeid(ImageC<ByteRGBValueC>); 
  }
  
  FileFormatJPEGC RegisterFileFormatJPEG(100,0,false,"jpeg","JPEG image file format. 'Lossless' compression.");
  FileFormatJPEGC RegisterFileFormatJPEGL(75,-25,false,"jpegl","JPEG image file format. 75% lossy compression.");
  
  FileFormatJPEGC RegisterFileFormatMJPEG(100,-1,true,"mjpeg","Sequence of JPEG images in a single file. 'lossless' compression.");
  FileFormatJPEGC RegisterFileFormatMJPEGL(75,-26,true,"mjpegl","Sequence of JPEG images in a single file.  75% lossy compression.");
  
}
