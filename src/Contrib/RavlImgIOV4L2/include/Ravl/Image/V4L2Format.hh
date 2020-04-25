// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/V4L2/V4L2Format.hh"
//! lib=RavlImgIOV4L2
//! author = "Warren Moore"
//! docentry = "Ravl.API.Images.Video.Video IO.Video4Linux2"

#ifndef RAVLIMAGE_V4L2FORMAT_HEADER
#define RAVLIMAGE_V4L2FORMAT_HEADER 1

#include "Ravl/DP/FileFormat.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"

namespace RavlImageN
{
  /////////////////////////////
  //! userlevel = Develop
  //: V4L2 file format information.
  
  class FileFormatV4L2BodyC :
    public FileFormatBodyC 
  {
  public:
    FileFormatV4L2BodyC();
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
    
  protected:
    bool FindParams(const StringC &filename, StringC &device, IntT &channel) const;
    //: Determine the parameters from the filename
  };
  
  
  
  /////////////////////////////
  //! userlevel = Advanced
  //: Create an instance of a V4L2 file format.
  // BIG OBJECT
  
  class FileFormatV4L2C :
    public FileFormatC<ImageC<ByteRGBValueC> > 
  {
  public:
    FileFormatV4L2C() :
      FileFormatC<ImageC<ByteRGBValueC> >(*new FileFormatV4L2BodyC())
    {}
  };
  
}

#endif
