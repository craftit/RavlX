// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, OmniPerception Ltd.
// This code may be redistributed under the terms of the MIT
// License. See http://www.opensource.org/licenses/mit-license.html
// file-header-ends-here
#ifndef RAVLIMAGE_UEYEFORMAT_HEADER
#define RAVLIMAGE_UEYEFORMAT_HEADER 1
//! lib=RavlImgIOuEye

#include "Ravl/DP/FileFormat.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"

namespace RavlImageN
{
  /////////////////////////////
  //! userlevel = Develop
  //: uEye file format information.
  
  class FileFormatuEyeBodyC :
    public FileFormatBodyC 
  {
  public:
    FileFormatuEyeBodyC();
    //: Constructor.
    
    virtual const type_info &ProbeLoad(IStreamC &in, const type_info &obj_type) const;
    //: Is stream in std stream format?
    
    virtual const type_info &ProbeLoad(const StringC &filename, IStreamC &in, const type_info &obj_type) const;
    //: Probe for load.
    
    virtual const type_info &ProbeSave(const StringC &filename, const type_info &obj_type, bool forceFormat) const;
    //: Probe for save.
    
    virtual DPIPortBaseC CreateInput(IStreamC &in, const type_info &obj_type) const;
    //: Create a input port for loading.
    // Will create an invalid port if not supported.
    
    virtual DPOPortBaseC CreateOutput(OStreamC &out, const type_info &obj_type) const;
    //: Create a output port for saving.
    // Will create an invalid port if not supported.
    
    virtual DPIPortBaseC CreateInput(const StringC &filename, const type_info &obj_type) const;
    //: Create a input port for loading from file 'filename'.
    // Will create an invalid port if not supported.
    
    virtual DPOPortBaseC CreateOutput(const StringC &filename, const type_info &obj_type) const;
    //: Create a output port for saving to file 'filename'.
    // Will create an invalid port if not supported.
    
    virtual const type_info& DefaultType() const; 
    //: Get prefered IO type.
    
    virtual IntT Priority() const { return 1; }
    //: Find the priority of the format: the higher the better (default = 0).
    
    virtual bool IsStream() const { return true; }
    //: Test if format is a fully streamable.
    
  };
  
  
  
  /////////////////////////////
  //! userlevel = Advanced
  //: Create an instance of a uEye file format.
  // BIG OBJECT
  
  class FileFormatuEyeC 
    : public FileFormatC<ImageC<ByteT> > 
  {
  public:
    FileFormatuEyeC() 
      : FileFormatC<ImageC<ByteT> >(*new FileFormatuEyeBodyC())
    {}
  };
  
}

#endif
