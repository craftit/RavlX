// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLYUVFORMAT_HEADER
#define RAVLYUVFORMAT_HEADER 1
////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Image/ImageIO/YUVFormat.hh"
//! lib=RavlImageIO
//! author="Bill Christmas"
//! docentry="Ravl.API.Images.IO.Formats"
//! date="4/11/2013"
//! userlevel=Default

#include "Ravl/DP/FileFormat.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteYUVValue.hh"

namespace RavlImageN {
  //! userlevel=Develop
  //: YUV File format information.
  
  class FileFormatYUVBodyC 
    : public FileFormatBodyC 
  {
  public:
    FileFormatYUVBodyC();
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
    //: Get preferred IO type.
    
    virtual IntT Priority() const { return 1; }
    //: Find the priority of the format: the higher the better.
    // Default is zero, this is better than the default (streams.)
    
    virtual bool IsStream() const { return false; }
    //: Test if format is a fully streamable.
    // i.e. check if you can read/write more than object object.
    
  };


  /////////////////////////////
  //! userlevel=Advanced
  //: Create an instance of a PPM File Format.
  
  class FileFormatYUVC : public FileFormatC<ImageC<ByteYUVValueC> > {
  public:
    FileFormatYUVC()
      : FileFormatC<ImageC<ByteYUVValueC> >(*new FileFormatYUVBodyC())
      {}
  };
  
 
}
#endif
