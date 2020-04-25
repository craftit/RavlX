// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/DVDRead/DVDFormat.hh"
//! lib=RavlDVDRead
//! author = "Warren Moore"
//! docentry = "Ravl.API.Images.Video.Video IO.LibDVDRead"

#ifndef RAVL_DVDREADFORMAT_HEADER
#define RAVL_DVDREADFORMAT_HEADER 1

#include "Ravl/DP/FileFormat.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"

namespace RavlN
{

  using namespace RavlImageN;
 
  //! userlevel = Develop
  //: DVD device information.
  
  class FileFormatDVDBodyC :
    public FileFormatBodyC 
  {
  public:
    FileFormatDVDBodyC();
    //: Constructor.
    
    virtual const type_info &ProbeLoad(IStreamC &in, const type_info &obj_type) const;
    //: Is stream in std stream format ?
    
    virtual const type_info &ProbeLoad(const StringC &filename, IStreamC &in, const type_info &obj_type) const;
    //: Probe for load.
    
    virtual const type_info &ProbeSave(const StringC &filename,const type_info &obj_type, bool forceFormat) const;
    //: Probe for Save.
    
    virtual DPIPortBaseC CreateInput(IStreamC &in, const type_info &obj_type) const;
    //: Create a input port for loading.
    // This creates a plain V4L stream.
    // Will create an Invalid port if not supported.
    
    virtual DPOPortBaseC CreateOutput(OStreamC &out, const type_info &obj_type) const;
    //: Create a output port for saving.
    // This creates a plain V4L stream.
    // Will create an Invalid port if not supported.
    
    virtual DPIPortBaseC CreateInput(const StringC &filename, const type_info &obj_type) const;
    //: Create a input port for loading from file 'filename'.
    // Will create an Invalid port if not supported. <p>
    
    virtual DPOPortBaseC CreateOutput(const StringC &filename, const type_info &obj_type) const;
    //: Create a output port for saving to file 'filename'..
    // Will create an Invalid port if not supported. <p>
    
    virtual const type_info &DefaultType() const; 
    //: Get prefered IO type.
    
    virtual IntT Priority() const { return 1; }
    //: Find the priority of the format. the higher the better.
    // Default is zero, this is better than the default (streams.)
    
    virtual bool IsStream() const { return true; }
    //: Test if format is a fully streamable.
    // i.e. check if you can read/write more than object object.
    
  protected:
  };
  
  //! userlevel = Advanced
  //: Create an instance of a DVD device format.
  
  class FileFormatDVDC :
    public FileFormatC< ImageC<ByteRGBValueC> > 
  {
  public:
    FileFormatDVDC() :
      FileFormatC< ImageC<ByteRGBValueC> >(*new FileFormatDVDBodyC())
    {}
  };
  
}

#endif
