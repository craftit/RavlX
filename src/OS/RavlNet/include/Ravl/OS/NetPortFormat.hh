// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_NETPORTFORMAT_HEADER
#define RAVL_NETPORTFORMAT_HEADER 1
//////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlNet
//! author="Charles Galambos"
//! docentry="Ravl.API.OS.Network.NetPort"
//! file="Ravl/OS/Network/NetPortFormat.hh"

#include "Ravl/DP/FileFormat.hh"
#include "Ravl/OS/NetIPort.hh"
#include "Ravl/OS/NetOPort.hh"

namespace RavlN {

  //! userlevel=Develop
  //: Data format for NetPort's.
  // By instantiating this class as a static variable you can allow net ports
  // to be open with the standard 'Load' or 'OpenISequence' methods.
  
  template<class DataT>
  class NetPortFormatBodyC 
    : public FileFormatBodyC 
  {
  public:
    NetPortFormatBodyC() 
      : FileFormatBodyC("net","NetPort file format.")
    {}
    //: Default constructor.
    
    virtual const type_info &ProbeLoad(IStreamC &in,const type_info &obj_type) const
    { return typeid(void); }
    //: Is stream in std stream format ?
    
    virtual const type_info &ProbeLoad(const StringC &filename,IStreamC &in,const type_info &obj_type) const {
      StringC dev = ExtractDevice(filename);
      if(dev != "NET")
	return typeid(void);
      return typeid(DataT);
    }
    //: Probe for load.
    
    virtual const type_info &ProbeSave(const StringC &filename,const type_info &obj_type,bool forceFormat) const {
      StringC dev = ExtractDevice(filename);
      if(dev != "NET")
	return typeid(void);      
      return typeid(DataT); 
    }
    //: Probe for Save.
    
    virtual DPIPortBaseC CreateInput(IStreamC &in,const type_info &obj_type) const
    { return DPIPortBaseC(); }
    //: Create a input port for loading.
    // Will create an Invalid port if not supported.
    
    virtual DPOPortBaseC CreateOutput(OStreamC &out,const type_info &obj_type) const
    { return DPOPortBaseC(); }
    //: Create a output port for saving.
    // Will create an Invalid port if not supported.
    
    virtual DPIPortBaseC CreateInput(const StringC &filename,const type_info &obj_type) const {
      StringC fn = ExtractParams(filename);
      if(obj_type != typeid(DataT))
	return DPIPortBaseC();
      StringC portName = fn.after('#');
      StringC addr = fn.before('#');
      return NetISPortC<DataT>(addr,portName);
    }
    //: Create a input port for loading from file 'filename'.
    // Will create an Invalid port if not supported. <p>
    
    virtual DPOPortBaseC CreateOutput(const StringC &filename,const type_info &obj_type) const {
      StringC fn = ExtractParams(filename);
      if(obj_type != typeid(DataT))
	return DPOPortBaseC();
      StringC portName = fn.after('#');
      StringC addr = fn.before('#');
      return NetOSPortC<DataT>(addr,portName);
    }
    //: Create a output port for saving to file 'filename'..
    // Will create an Invalid port if not supported. <p>
    
    virtual const type_info &DefaultType() const
    { return typeid(DataT); }
    //: Get preferred IO type.
    
    virtual IntT Priority() const { return 0; }
    //: Find the priority of the format. the higher the better.
    
    virtual bool IsStream() const { return true; }
    //: Test if format is a fully streamable.
    // i.e. check if you can read/write more than object object.
    
  };


  //! userlevel=Normal
  //: File format information for NetPort's.
  // By instantiating this class as a static variable you can allow net ports
  // to be open with the standard 'Load' or 'OpenISequence' methods. <br>
  // You have to make an instance of this class for each type you wish to
  // handle.
  
  template<class DataT>
  class NetPortFormatC 
    : public FileFormatC<DataT >
  {
  public:
    NetPortFormatC()
      : FileFormatC<DataT>(*new NetPortFormatBodyC<DataT>())
    {}
    //: Constructor.
  };
  
}

#endif
