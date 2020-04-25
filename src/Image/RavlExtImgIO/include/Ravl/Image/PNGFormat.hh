// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLPNGFORMAT_HEADER
#define RAVLPNGFORMAT_HEADER 1
////////////////////////////////////////////////////////////
//! file="Ravl/Image/ExternalImageIO/PNG/PNGFormat.hh"
//! lib=RavlExtImgIO
//! author="Charles Galambos"
//! docentry="Ravl.API.Images.IO.Formats"
//! date="29/10/98"

#include "Ravl/DP/FileFormat.hh"
#include "Ravl/Image/ImgIOPNGB.hh"

namespace RavlImageN {
  
  //! userlevel=Develop
  //: PNG File format information.
  
  class FileFormatPNGBodyC 
    : public FileFormatBodyC 
  {
  public:
    FileFormatPNGBodyC(bool nonly16Bit,const StringC &id,const StringC &desc);
    //: Constructor.
    
    const std::type_info &ChooseFormat(const std::type_info &obj_type) const;
    //: Try and choose best format for IO.
    
    const std::type_info &ChooseFormat(const std::type_info &obj_type,int bit_depth,int colour_type,int interlace) const;
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
    
    virtual IntT Priority() const { return 0; }
    //: Find the priority of the format. the higher the better.
    // Default is zero, this is better than the default (streams.)
    
    virtual bool IsStream() const { return false; }
    //: Test if format is a fully streamable.
    // i.e. check if you can read/write more than object object.
    // png supports sequences.. but not with this software for now...
    
  protected:
    bool only16Bit; // 
  };
  
  /////////////////////////////
  //! userlevel=Advanced
  //: Create an instance of a PNG File Format.
  
  class FileFormatPNGC : public FileFormatC<ImageC<ByteT> > {
  public:
    FileFormatPNGC(bool nonly16Bit,const StringC &id,const StringC &desc)
      : FileFormatC<ImageC<ByteT> >(*new FileFormatPNGBodyC(nonly16Bit,id,desc))
      {}
  };

}
#endif
