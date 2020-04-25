// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2013, React AI Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL) Version 2.1  See the lgpl.licence file
// for details or see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_IDX_IPORTIDXFORMAT_HEADER
#define RAVL_IDX_IPORTIDXFORMAT_HEADER 1

//! docentry = "Ravl.API.Core.IO.IDX Files"
//! author = "Charles Galambos"
//! lib = RavlFileIDX

#include "Ravl/DP/FileFormat.hh"
#include "Ravl/IO/IPortIDX.hh"
// 
namespace RavlN { namespace ION {

  //: IDX file IO.
  
  class FileFormatIPortIDXBodyC
    : public RavlN::FileFormatBodyC
  {
  public:
    FileFormatIPortIDXBodyC(const StringC &vName);
    //: Constructor.
    
    virtual const std::type_info &ProbeLoad(RavlN::IStreamC &in,const std::type_info &obj_type) const;
    //: Is stream in std stream format ?
    
    virtual const std::type_info &ProbeLoad(const StringC &filename,RavlN::IStreamC &in,const std::type_info &obj_type) const;
    //: Probe for load.
    
    virtual const std::type_info &ProbeSave(const StringC &filename,const std::type_info &obj_type,bool forceFormat) const;
    //: Probe for Save.
    
    virtual RavlN::DPIPortBaseC CreateInput(RavlN::IStreamC &in,const std::type_info &obj_type) const;
    //: Create a input port for loading.
    // Will create an Invalid port if not supported.
    
    virtual RavlN::DPOPortBaseC CreateOutput(RavlN::OStreamC &out,const std::type_info &obj_type) const;
    //: Create a output port for saving.
    // Will create an Invalid port if not supported.
    
    virtual RavlN::DPIPortBaseC CreateInput(const StringC &filename,const std::type_info &obj_type) const;
    //: Create a input port for loading from file 'filename'.
    // Will create an Invalid port if not supported. <p>
    
    virtual RavlN::DPOPortBaseC CreateOutput(const StringC &filename,const std::type_info &obj_type) const;
    //: Create a output port for saving to file 'filename'..
    // Will create an Invalid port if not supported. <p>
    
    virtual const std::type_info &DefaultType() const;
    //: Get prefered IO type.
    
    virtual IntT Priority() const { return 0; }
    //: Find the priority of the format. the higher the better.
    // Default is zero, use this if nothing better.
    
    virtual bool IsStream() const { return true; }
    //: Test if format is a fully streamable.
    // i.e. check if you can read/write more than object object.
    
  protected:
    StringC vName;  // Variant name.
  };
  
  /////////////////////////////
  //! userlevel=Advanced
  //: Create an instance of an IDX File Format.
  
  class FileFormatIPortIDXC
    : public RavlN::FileFormatBaseC
  {
  public:
    FileFormatIPortIDXC(const StringC &vName)
      : RavlN::FileFormatBaseC(*new FileFormatIPortIDXBodyC(vName))
    {}
  };
  
}}

#endif
