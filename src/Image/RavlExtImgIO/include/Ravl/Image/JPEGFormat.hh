// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLJPEGFORMAT_HEADER
#define RAVLJPEGFORMAT_HEADER 1
////////////////////////////////////////////////////////////
//! file="Ravl/Image/ExternalImageIO/JPEG/JPEGFormat.hh"
//! lib=RavlExtImgIO
//! author="Charles Galambos"
//! docentry="Ravl.API.Images.IO.Formats"
//! date="29/10/98"

#include "Ravl/DP/FileFormat.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"

namespace RavlImageN {

  //! userlevel=Develop
  //: JPEG File format information.
  
  class FileFormatJPEGBodyC 
    : public FileFormatBodyC 
  {
  public:
    FileFormatJPEGBodyC();
    //: Default constructor.
    
    FileFormatJPEGBodyC(int comp,int pri,bool asSequence,const StringC &name,const StringC &desc);
    //: Constructor.
    
    const std::type_info &ChooseFormat(const std::type_info &obj_type) const;
    //: Try and choose best format for IO.
    
    virtual const std::type_info &ProbeLoad(IStreamC &in,const std::type_info &obj_type) const;
    //: Is stream in std stream format ?
    
    virtual const std::type_info &ProbeLoad(const StringC &filename,IStreamC &in,const std::type_info &obj_type) const;
    //: Probe for load.
    
    virtual const std::type_info &ProbeSave(const StringC &filename,const std::type_info &obj_type,bool forceFormat) const;
    //: Probe for Save.
    
    virtual DPIPortBaseC CreateInput(const StringC &filename,const std::type_info &obj_type) const;
    //: Create a input port for loading from file 'filename'.
    // Will create an Invalid port if not supported. <p>
    
    virtual DPOPortBaseC CreateOutput(const StringC &filename,const std::type_info &obj_type) const;
    //: Create a output port for saving to file 'filename'..
    // Will create an Invalid port if not supported. <p>
    
    virtual DPIPortBaseC CreateInput(IStreamC &in,const std::type_info &obj_type) const;
    //: Create a input port for loading.
    // Will create an Invalid port if not supported.
    
    virtual DPOPortBaseC CreateOutput(OStreamC &out,const std::type_info &obj_type) const;
    //: Create a output port for saving.
    // Will create an Invalid port if not supported.
  
    virtual const std::type_info &DefaultType() const; 
    //: Get prefered IO type.
    
    virtual IntT Priority() const { return pri; }
    //: Find the priority of the format. the higher the better.
    // Default is zero, this is better than the default (streams.)
    
    virtual bool IsStream() const { return asSequence; }
    //: Test if format is a fully streamable.
    // i.e. check if you can read/write more than object object.
    // jpeg supports sequences.. but not with this software for now...
    
  protected:
    int compression;
    int pri;
    bool asSequence;
  };
  
  /////////////////////////////
  //! userlevel=Advanced
  //: Create an instance of a JPEG File Format.
  
  class FileFormatJPEGC : public FileFormatC<ImageC<ByteT> > {
  public:
    FileFormatJPEGC(int comp,int pri,bool asSequence,const StringC &name,const StringC &desc)
      : FileFormatC<ImageC<ByteT> >(*new FileFormatJPEGBodyC(comp,pri,asSequence,name,desc))
    {}
  };
}

#endif
