// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLPNMFORMAT_HEADER
#define RAVLPNMFORMAT_HEADER 1
////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Image/ImageIO/PNMFormat.hh"
//! lib=RavlImageIO
//! author="Charles Galambos"
//! docentry="Ravl.API.Images.IO.Formats"
//! date="29/10/98"
//! userlevel=Develop

#include "Ravl/DP/FileFormat.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"

namespace RavlImageN {
  //! userlevel=Develop
  //: PGM File format information.
  
  class FileFormatPGMBodyC 
    : public FileFormatBodyC 
  {
  public:
    FileFormatPGMBodyC();
    //: Default constructor.
    
    virtual const std::type_info &ProbeLoad(IStreamC &in,const std::type_info &obj_type) const;
    //: Is stream in std stream format ?
    
    virtual const std::type_info &ProbeLoad(const StringC &filename,IStreamC &in,const std::type_info &obj_type) const;
    //: Probe for load.

    virtual const std::type_info &ProbeSave(const StringC &filename,const std::type_info &obj_type,bool forceFormat) const;
    //: Probe for Save.
    
    virtual DPIPortBaseC CreateInput(IStreamC &in,const std::type_info &obj_type) const;
    //: Create a input port for loading.
    // Will create an Invalid port if not supported.
    
    virtual DPOPortBaseC CreateOutput(OStreamC &out,const std::type_info &obj_type) const;
    //: Create a output port for saving.
    // Will create an Invalid port if not supported.
    
    virtual const std::type_info &DefaultType() const; 
    //: Get prefered IO type.
    
    virtual IntT Priority() const { return 1; }
    //: Find the priority of the format. the higher the better.
    // Default is zero, this is better than the default (streams.)
    
    virtual bool IsStream() const { return false; }
    //: Test if format is a fully streamable.
    // i.e. check if you can read/write more than object object.
    
  };
  
  //! userlevel=Develop
  //: PPM File format information.
  
  class FileFormatPPMBodyC : public FileFormatBodyC {
  public:
    FileFormatPPMBodyC();
    //: Default constructor.
    
    virtual const std::type_info &ProbeLoad(IStreamC &in,const std::type_info &obj_type) const;
    //: Is stream in std stream format ?
    
    virtual const std::type_info &ProbeLoad(const StringC &filename,IStreamC &in,const std::type_info &obj_type) const;
    //: Probe for load.
    
    virtual const std::type_info &ProbeSave(const StringC &filename,const std::type_info &obj_type,bool forceFormat) const;
    //: Probe for Save.
    
    virtual DPIPortBaseC CreateInput(IStreamC &in,const std::type_info &obj_type) const;
    //: Create a input port for loading.
    // Will create an Invalid port if not supported.
    
    virtual DPOPortBaseC CreateOutput(OStreamC &out,const std::type_info &obj_type) const;
    //: Create a output port for saving.
    // Will create an Invalid port if not supported.
    
    virtual const std::type_info &DefaultType() const;
    //: Get prefered IO type.
    
    virtual IntT Priority() const { return 1; }
    //: Find the priority of the format. the higher the better.
    // Default is zero, this is better than the default (streams.)
    
    
    virtual bool IsStream() const { return false; }
    
    //: Test if format is a fully streamable.
    // i.e. check if you can read/write more than object object.
    
  };
  
  //! userlevel=Develop
  //: PBM File format information.

  class FileFormatPBMBodyC 
    : public FileFormatBodyC 
  {
  public:
    FileFormatPBMBodyC();
    //: Default constructor.
    
    virtual const std::type_info &ProbeLoad(IStreamC &in,const std::type_info &obj_type) const;
    //: Is stream in std stream format ?
    
    virtual const std::type_info &ProbeLoad(const StringC &filename,IStreamC &in,const std::type_info &obj_type) const;
    //: Probe for load.
    
    virtual const std::type_info &ProbeSave(const StringC &filename,const std::type_info &obj_type,bool forceFormat) const;
    //: Probe for Save.
    
    virtual DPIPortBaseC CreateInput(IStreamC &in,const std::type_info &obj_type) const;
    //: Create a input port for loading.
    // Will create an Invalid port if not supported.
    
    virtual DPOPortBaseC CreateOutput(OStreamC &out,const std::type_info &obj_type) const;
    //: Create a output port for saving.
    // Will create an Invalid port if not supported.
    
    virtual const std::type_info &DefaultType() const; 
    //: Get prefered IO type.
    
    //virtual IntT Priority() const { return 1; }
    //: Find the priority of the format. the higher the better.
    // Default is zero, this is better than the default (streams.)
    // THIS IS NOT FULLY IMPLEMENTED!
    
    virtual bool IsStream() const { return false; }
    //: Test if format is a fully streamable.
    // i.e. check if you can read/write more than object object.
  };
  
  
  
  /////////////////////////////
  //! userlevel=Advanced
  //: Create an instance of a PPM File Format.
  
  class FileFormatPPMC : public FileFormatC<ImageC<ByteRGBValueC> > {
  public:
    FileFormatPPMC()
      : FileFormatC<ImageC<ByteRGBValueC> >(*new FileFormatPPMBodyC())
      {}
  };
  
  /////////////////////////////
  //! userlevel=Advanced
  //: Create an instance of a PGM File Format.
  
  class FileFormatPGMC : public FileFormatC<ImageC<ByteT> > {
  public:
    FileFormatPGMC()
      : FileFormatC<ImageC<ByteT> >(*new FileFormatPGMBodyC())
      {}
  };
  
  /////////////////////////////
  //! userlevel=Advanced
  //: Create an instance of a PBM File Format.
  
  class FileFormatPBMC : public FileFormatC<ImageC<ByteT> > {
  public:
    FileFormatPBMC()
      : FileFormatC<ImageC<ByteT> >(*new FileFormatPBMBodyC())
      {}
  };
  
}
#endif
