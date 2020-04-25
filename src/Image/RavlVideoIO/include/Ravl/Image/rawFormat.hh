// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_RAWFORMAT_HEADER
#define RAVLIMAGE_RAWFORMAT_HEADER 1
////////////////////////////////////////////////////////////
//! author="Charles Galambos"
//! docentry="Ravl.API.Images.Video.Video IO"
//! date="29/10/98"
//! rcsid="$Id$"
//! file="Ravl/Image/VideoIO/rawFormat.hh"
//! example=exImgSeq.cc
//! lib=RavlVideoIO

#include "Ravl/DP/FileFormat.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"

namespace RavlImageN {

  //! userlevel=Develop
  //: raw File format information.
  
  class FileFormatRawBodyC : public FileFormatBodyC {
  public:
    FileFormatRawBodyC();
    //: Constructor.
    
    virtual const std::type_info &ProbeLoad(IStreamC &in,const std::type_info &obj_type) const;
    //: Is stream in std stream format ?
    
    virtual const std::type_info &ProbeLoad(const StringC &filename,IStreamC &in,const std::type_info &obj_type) const;
    //: Probe for load.
    
    virtual const std::type_info &ProbeSave(const StringC &filename,const std::type_info &obj_type,bool forceFormat) const;
    //: Probe for Save.
  
    virtual DPIPortBaseC CreateInput(IStreamC &in,const std::type_info &obj_type) const;
    //: Create a input port for loading.
    // This creates a plain RGB stream.
    // Will create an Invalid port if not supported.
    
    virtual DPOPortBaseC CreateOutput(OStreamC &out,const std::type_info &obj_type) const;
    //: Create a output port for saving.
    // This creates a plain RGB stream.
    // Will create an Invalid port if not supported.
    
    virtual DPIPortBaseC CreateInput(const StringC &filename,const std::type_info &obj_type) const;
    //: Create a input port for loading from file 'filename'.
    // Will create an Invalid port if not supported. <p>
    
    virtual DPOPortBaseC CreateOutput(const StringC &filename,const std::type_info &obj_type) const;
    //: Create a output port for saving to file 'filename'..
    // Will create an Invalid port if not supported. <p>
    
    virtual const std::type_info &DefaultType() const; 
    //: Get prefered IO type.
    
    virtual IntT Priority() const { return -1; }
    //: Find the priority of the format. the higher the better.
    // Default is zero, use this if nothing better.
    
    virtual bool IsStream() const { return true; }
    //: Test if format is a fully streamable.
    // i.e. check if you can read/write more than object object.
    
  protected:
  };
  
  /////////////////////////////
  //! userlevel=Advanced
  //: Create an instance of a RGB File Format.
  
  class FileFormatRawC 
    : public FileFormatC<ImageC<ByteRGBValueC> > 
  {
  public:
    FileFormatRawC()
      : FileFormatC<ImageC<ByteRGBValueC> >(*new FileFormatRawBodyC())
      {}
  };
}

#endif
