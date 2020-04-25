// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_MPEG2FORMAT_HEADER
#define RAVL_MPEG2FORMAT_HEADER 1
////////////////////////////////////////////////////////////
//! author="Charles Galambos"
//! docentry="Ravl.API.Images.Video.Video IO.MPEG2"
//! date="29/10/2003"
//! rcsid="$Id$"
//! author="Charles Galambos"
//! lib=RavlMPEG2
//! file="Ravl/Contrib/MPEG/mpeg2dec/MPEG2Format.hh"

#include "Ravl/DP/FileFormat.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteYUVValue.hh"

namespace RavlImageN {

  //! userlevel=Develop
  //: MPEG File format information.
  
  class FileFormatMPEG2BodyC 
    : public FileFormatBodyC 
  {
  public:
    FileFormatMPEG2BodyC();
    //: Constructor.
    
    virtual const type_info &ProbeLoad(IStreamC &in,const type_info &obj_type) const;
    //: Is stream in std stream format ?
    
    virtual const type_info &ProbeLoad(const StringC &filename,IStreamC &in,const type_info &obj_type) const;
    //: Probe for load.
    
    virtual const type_info &ProbeSave(const StringC &filename,const type_info &obj_type,bool forceFormat) const;
    //: Probe for Save.
    
    virtual DPIPortBaseC CreateInput(IStreamC &in,const type_info &obj_type) const;
    //: Create a input port for loading.
    // This creates a plain MPEG stream.
    // Will create an Invalid port if not supported.
    
    virtual DPOPortBaseC CreateOutput(OStreamC &out,const type_info &obj_type) const;
    //: Create a output port for saving.
    // This creates a plain MPEG stream.
    // Will create an Invalid port if not supported.
    
    virtual DPIPortBaseC CreateInput(const StringC &filename,const type_info &obj_type) const;
    //: Create a input port for loading from file 'filename'.
    // Will create an Invalid port if not supported. <p>
    
    virtual DPOPortBaseC CreateOutput(const StringC &filename,const type_info &obj_type) const;
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
  
  /////////////////////////////
  //! userlevel=Advanced
  //: Create an instance of an MPEG File Format.
  
  class FileFormatMPEG2C 
    : public FileFormatC<ImageC<ByteYUVValueC> > 
  {
  public:
    FileFormatMPEG2C()
      : FileFormatC<ImageC<ByteYUVValueC> >(*new FileFormatMPEG2BodyC())
    {}
  };
  
}

#endif
