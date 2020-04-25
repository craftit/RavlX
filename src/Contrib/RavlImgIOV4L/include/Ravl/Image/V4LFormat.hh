// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_V4LFORMAT_HEADER
#define RAVL_V4LFORMAT_HEADER 1
////////////////////////////////////////////////////////////
//! author="Charles Galambos"
//! docentry="Ravl.API.Images.Video.Video IO.Video4Linux"
//! date="29/10/1998"
//! rcsid="$Id$"
//! file="Ravl/Contrib/V4L/V4LFormat.hh"
//! example=exImgSeq.cc
//! lib=RavlImgIOV4L

#include "Ravl/DP/FileFormat.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteYUVValue.hh"

struct video_picture;

namespace RavlImageN {

  //! userlevel=Develop
  //: V4L File format information.
  
  class FileFormatV4LBodyC 
    : public FileFormatBodyC 
  {
  public:
    FileFormatV4LBodyC(const StringC &vName);
    //: Constructor.
    
    virtual const type_info &ProbeLoad(IStreamC &in,const type_info &obj_type) const;
    //: Is stream in std stream format ?
    
    virtual const type_info &ProbeLoad(const StringC &filename,IStreamC &in,const type_info &obj_type) const;
    //: Probe for load.
    
    virtual const type_info &ProbeSave(const StringC &filename,const type_info &obj_type,bool forceFormat) const;
    //: Probe for Save.
    
    virtual DPIPortBaseC CreateInput(IStreamC &in,const type_info &obj_type) const;
    //: Create a input port for loading.
    // This creates a plain V4L stream.
    // Will create an Invalid port if not supported.
    
    virtual DPOPortBaseC CreateOutput(OStreamC &out,const type_info &obj_type) const;
    //: Create a output port for saving.
    // This creates a plain V4L stream.
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
    bool CheckRGB(int fd,struct video_picture &pic) const;
    bool CheckYUV(int fd,struct video_picture &pic) const;
    bool CheckYUV422(int fd,struct video_picture &pic) const;
    bool CheckGREY(int fd,struct video_picture &pic) const;
    
    StringC vName;  // Variant name.
  };
  
  /////////////////////////////
  //! userlevel=Advanced
  //: Create an instance of a V4L File Format.
  
  class FileFormatV4LC 
    : public FileFormatC<ImageC<ByteYUVValueC> > 
  {
  public:
    FileFormatV4LC(const StringC &vName)
      : FileFormatC<ImageC<ByteYUVValueC> >(*new FileFormatV4LBodyC(vName))
    {}
  };
  
}
#endif
