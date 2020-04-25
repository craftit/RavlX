// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_CIFFORMAT_HEADER
#define RAVLIMAGE_CIFFORMAT_HEADER 1
////////////////////////////////////////////////////////////
//! author="Charles Galambos"
//! docentry="Ravl.API.Images.Video.Video IO"
//! date="29/10/1998"
//! rcsid="$Id$"
//! file="Ravl/Image/VideoIO/CifFormat.hh"
//! example=exImgSeq.cc
//! lib=RavlVideoIO

#include "Ravl/DP/FileFormat.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteYUVValue.hh"

namespace RavlImageN {

  //! userlevel=Develop
  //: Cif File format information.
  
  class FileFormatCifBodyC 
    : public FileFormatBodyC 
  {
  public:
    FileFormatCifBodyC(const StringC &vName,const Index2dC &vSize);
    //: Constructor.
    
    virtual const std::type_info &ProbeLoad(IStreamC &in,const std::type_info &obj_type) const;
    //: Is stream in std stream format ?
    
    virtual const std::type_info &ProbeLoad(const StringC &filename,IStreamC &in,const std::type_info &obj_type) const;
    //: Probe for load.
    
    virtual const std::type_info &ProbeSave(const StringC &filename,const std::type_info &obj_type,bool forceFormat) const;
    //: Probe for Save.
    
    virtual DPIPortBaseC CreateInput(IStreamC &in,const std::type_info &obj_type) const;
    //: Create a input port for loading.
    // This creates a plain Cif stream.
    // Will create an Invalid port if not supported.
    
    virtual DPOPortBaseC CreateOutput(OStreamC &out,const std::type_info &obj_type) const;
    //: Create a output port for saving.
    // This creates a plain Cif stream.
    // Will create an Invalid port if not supported.
    
    virtual DPIPortBaseC CreateInput(const StringC &filename,const std::type_info &obj_type) const;
    //: Create a input port for loading from file 'filename'.
    // Will create an Invalid port if not supported. <p>
    
    virtual DPOPortBaseC CreateOutput(const StringC &filename,const std::type_info &obj_type) const;
    //: Create a output port for saving to file 'filename'..
    // Will create an Invalid port if not supported. <p>
    
    virtual const std::type_info &DefaultType() const; 
    //: Get prefered IO type.
    
    virtual IntT Priority() const { return 1; }
    //: Find the priority of the format. the higher the better.
    // Default is zero, this is better than the default (streams.)
    
    virtual bool IsStream() const { return true; }
    //: Test if format is a fully streamable.
    // check if you can read/write more than object object.
    
  protected:
    StringC vName;  // Variant name.
    Index2dC vSize; // Variant image size.
  };
  
  /////////////////////////////
  //! userlevel=Advanced
  //: Create an instance of a Cif File Format.
  
  class FileFormatCifC : public FileFormatC<ImageC<ByteYUVValueC> > {
  public:
    FileFormatCifC(const StringC &vName,const Index2dC &vSize)
      : FileFormatC<ImageC<ByteYUVValueC> >(*new FileFormatCifBodyC(vName,vSize))
      {}
  };
  
}
#endif
  
