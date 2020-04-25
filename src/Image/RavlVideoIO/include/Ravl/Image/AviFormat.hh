// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_AVIFORMAT_HEADER
#define RAVLIMAGE_AVIFORMAT_HEADER 1
////////////////////////////////////////////////////////////
//! author="Daniele Muntoni"
//! docentry="Ravl.API.Images.Video.Video IO"
//! date="26/03/2003"
//! rcsid="$Id$"
//! file="Ravl/Image/VideoIO/AviFormat.hh"
//! example=exImgSeq.cc
//! lib=RavlVideoIO


#include "Ravl/DP/FileFormat.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteBGRValue.hh"

namespace RavlImageN {

  //! userlevel=Develop
  //: Avi File format information.
  
  class FileFormatAviBodyC 
    : public FileFormatBodyC 
  {
  public:
    FileFormatAviBodyC(const StringC &vName);
    //: Constructor.
    
    virtual const std::type_info &ProbeLoad(IStreamC &in,const std::type_info &obj_type) const;
    //: Is stream in std stream format ?
    
    virtual const std::type_info &ProbeLoad(const StringC &filename,IStreamC &in,const std::type_info &obj_type) const;
    //: Probe for load.
    
    virtual const std::type_info &ProbeSave(const StringC &filename,const std::type_info &obj_type,bool forceFormat) const;
    //: Probe for Save.
    
    virtual DPIPortBaseC CreateInput(IStreamC &in,const std::type_info &obj_type) const;
    //: Create a input port for loading.
    // This creates a plain Avi stream.
    // Will create an Invalid port if not supported.
    
    virtual DPOPortBaseC CreateOutput(OStreamC &out,const std::type_info &obj_type) const;
    //: Create a output port for saving.
    // This creates a plain Avi stream.
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
  };
  
  /////////////////////////////
  //! userlevel=Advanced
  //: Create an instance of a Avi File Format.
  
  class FileFormatAviC : public FileFormatC<ImageC<ByteBGRValueC> > {
  public:
    FileFormatAviC(const StringC &vName)
      : FileFormatC<ImageC<ByteBGRValueC> >(*new FileFormatAviBodyC(vName))
      {}
  };
  
}
#endif
  
