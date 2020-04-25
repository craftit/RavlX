// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_LIB1394DCFORMAT_HEADER
#define RAVL_LIB1394DCFORMAT_HEADER 1
////////////////////////////////////////////////////////////
//! author="Charles Galambos"
//! docentry="Ravl.API.Images.Video.Video IO.IIDC"
//! date="20/8/2003"
//! rcsid="$Id$"
//! file="Ravl/Contrib/dc1394/Lib1394dcFormat.hh"
//! lib=RavlImgIO1394dc
//! author="Charles Galambos"

#include "Ravl/DP/FileFormat.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteYUVValue.hh"

struct video_picture;

namespace RavlImageN {

  //! userlevel=Develop
  //: 1394dc File format information body
  
  class FileFormat1394dcBodyC 
    : public FileFormatBodyC 
  {
  public:
    FileFormat1394dcBodyC(const StringC &vName);
    //: Constructor.
    
    virtual const std::type_info &ProbeLoad(IStreamC &in,const std::type_info &obj_type) const;
    //: Is stream in std stream format ?
    
    virtual const std::type_info &ProbeLoad(const StringC &filename,IStreamC &in,const std::type_info &obj_type) const;
    //: Probe for load.
    
    virtual const std::type_info &ProbeSave(const StringC &filename,const std::type_info &obj_type,bool forceFormat) const;
    //: Probe for Save.
    
    virtual DPIPortBaseC CreateInput(IStreamC &in,const std::type_info &obj_type) const;
    //: Create a input port for loading.
    // This creates a plain 1394dc stream.
    // Will create an Invalid port if not supported.
    
    virtual DPOPortBaseC CreateOutput(OStreamC &out,const std::type_info &obj_type) const;
    //: Create a output port for saving.
    // This creates a plain 1394dc stream.
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
    // i.e. check if you can read/write more than object object.
    
  protected:
    bool CheckRGB(int fd,struct video_picture &pic) const;
    bool CheckYUV(int fd,struct video_picture &pic) const;
    bool CheckYUV422(int fd,struct video_picture &pic) const;
    bool CheckGREY(int fd,struct video_picture &pic) const;
    
    StringC vName;  // Variant name.
  };
  
  /////////////////////////////
  //! userlevel=Advanced
  //: Create an instance of a 1394dc File Format.
  
  class FileFormat1394dcC 
    : public FileFormatC<ImageC<ByteYUVValueC> > 
  {
  public:
    FileFormat1394dcC(const StringC &vName)
      : FileFormatC<ImageC<ByteYUVValueC> >(*new FileFormat1394dcBodyC(vName))
    {}
  };
  
}
#endif
