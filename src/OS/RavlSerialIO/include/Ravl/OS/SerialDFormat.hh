// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the MIT
// License. See http://www.opensource.org/licenses/mit-license.html
// file-header-ends-here
#ifndef RAVL_SERIAL_DIRECT_FORMAT_HEADER
#define RAVL_SERIAL_DIRECT_FORMAT_HEADER 1
//! lib=RavlSerialIO

#ifndef VISUAL_CPP
#include "Ravl/DP/FileFormat.hh"
#include "Ravl/OS/SerialAbstract.hh"
#include "Ravl/SmartPtr.hh"

namespace RavlN {

  /////////////////////////////
  //! userlevel = Develop
  //: SerialDirect file format information.
  
  class FileFormatSerialDirectBodyC :
    public FileFormatBodyC 
  {
  public:
    FileFormatSerialDirectBodyC();
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
    //: Get preferred IO type.
    
    virtual IntT Priority() const { return 1; }
    //: Find the priority of the format: the higher the better (default = 0).
    
    virtual bool IsStream() const { return true; }
    //: Test if format is a fully streamable.
    
  };
  
  /////////////////////////////
  //! userlevel = Advanced
  //: Create an instance of a SerialDirect file format.
  // BIG OBJECT
  
  class FileFormatSerialDirectC
    : public FileFormatC<SmartPtrC<SerialAbstractC> >
  {
  public:
    FileFormatSerialDirectC()
      : FileFormatC<SmartPtrC<SerialAbstractC> >(*new FileFormatSerialDirectBodyC())
    {}
  };
  
}

#endif
#endif

