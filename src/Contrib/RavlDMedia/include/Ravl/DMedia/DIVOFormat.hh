// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_DIVOFORMAT_HEADER
#define RAVLIMAGE_DIVOFORMAT_HEADER 1
////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Contrib/DMedia/DIVOFormat.hh"
//! lib=RavlDMedia
//! userlevel=Develop
//! docentry="Ravl.API.Images.Video.Video IO.SGI DMedia"
//! author="Charles Galambos"
//! date="22/11/1999"

#include "Ravl/Image/ByteVYUValue.hh"
#include "Ravl/DMedia/DIVO.hh"
#include "Ravl/DP/FileFormat.hh"
#include "Ravl/HSet.hh"

namespace RavlImageN
{
  using namespace RavlN;
  
  //! userlevel=Develop
  //: DIVO IO format information.
  
  class IOFormatDIVOBodyC 
    : public FileFormatBodyC 
  {
  public:
    IOFormatDIVOBodyC();
    //: Constructor.
    
    virtual const type_info &ProbeLoad(IStreamC &in,const type_info &obj_type) const;
    //: Is stream in std stream format ?
    // Not supported.
    
    virtual const type_info &ProbeLoad(const StringC &filename,IStreamC &in,const type_info &obj_type) const;
    //: Probe for load.
    
    virtual const type_info &ProbeSave(const StringC &filename,const type_info &obj_type,bool forceFormat) const;
    //: Probe for Save.
    
    virtual DPIPortBaseC CreateInput(IStreamC &in,const type_info &obj_type) const;
    //: Create a input port for loading.
    // This creates a plain DIVO stream.
    // Will create an Invalid port if not supported.
    
    virtual DPOPortBaseC CreateOutput(OStreamC &out,const type_info &obj_type) const;
    //: Create a output port for saving.
    // This creates a plain DIVO stream.
    // Will create an Invalid port if not supported.
    
    virtual DPIPortBaseC CreateInput(const StringC &filename,const type_info &obj_type) const;
    //: Create a input port for loading from file 'filename'.
    // Will create an Invalid port if not supported. <p>
    
    virtual DPOPortBaseC CreateOutput(const StringC &filename,const type_info &obj_type) const;
    //: Create a output port for saving to file 'filename'..
    // Will create an Invalid port if not supported. <p>
    
    virtual const type_info &DefaultType() const;
    //: Get prefered IO type.
    
    virtual IntT Priority() const;
    //: Find the priority of the format. the higher the better.
    // Default is zero, this is better than the default (streams.)
    
    virtual bool IsStream() const;
    //: Test if format is a fully streamable.
    // i.e. check if you can read/write more than object object.
    
    bool HaveDevice(const StringC &nm) const;
    //: Check if device 'nm' exists.
    
  protected:
    const type_info &GetBestType(const type_info &type) const;
    //: Find the best type that we can display.
    
    void OpenServer();
    //: Open video server.
    
    HSetC<StringC> devSet;
    VLServerC vidServer;  // Video server. maybe invalid if not opened.
  };
  
  /////////////////////////////
  //! userlevel=Advanced
  //: Create an instance of a DIVO IO Format.
  
  class IOFormatDIVOC 
    : public FileFormatC<ImageC<ByteVYUValueC> > 
  {
  public:
    IOFormatDIVOC(bool)
      : FileFormatC<ImageC<ByteVYUValueC> >(*new IOFormatDIVOBodyC())
      {}
  };
  
}

#endif
