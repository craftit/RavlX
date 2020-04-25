// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlIO
//! file="Ravl/Core/IO/StreamOp.cc"

#include "Ravl/DP/StreamOp.hh"
#include "Ravl/DP/TypeConverter.hh"
#include "Ravl/Complex.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/SysLog.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  DPStreamOpBodyC::DPStreamOpBodyC()
  {}
  //: Default constructor.

  DPStreamOpBodyC::DPStreamOpBodyC(const StringC &entityName)
   : DPEntityBodyC(entityName)
  {}
  //: Constructor.

  DPStreamOpBodyC::DPStreamOpBodyC(std::istream &in)
    : DPEntityBodyC(in)
  {}
  //: Stream constructor.
  
  DPStreamOpBodyC::DPStreamOpBodyC(BinIStreamC &in)
    : DPEntityBodyC(in)
  {}
  //: Binary stream constructor.

  StringC DPStreamOpBodyC::OpName() const
  { return EntityName(); }
  //: Op type name.

  //: Input plugs.
  
  DListC<DPIPlugBaseC> DPStreamOpBodyC::IPlugs() const {
    return DListC<DPIPlugBaseC>();
  }
  
  //: Output plugs
  
  DListC<DPOPlugBaseC> DPStreamOpBodyC::OPlugs() const {
    return DListC<DPOPlugBaseC>();
  }
  
  //: Input ports.
  
  DListC<DPIPortBaseC> DPStreamOpBodyC::IPorts() const {
    return DListC<DPIPortBaseC>();
  }
    
  //: Output ports
  
  DListC<DPOPortBaseC> DPStreamOpBodyC::OPorts() const {
    return DListC<DPOPortBaseC>();
  }

  //: Get input
  bool DPStreamOpBodyC::GetIPlug(const StringC &name,DPIPlugBaseC &port)
  {
    DListC<DPIPlugBaseC> plugs = IPlugs();
    for(DLIterC<DPIPlugBaseC> it(plugs);it;it++) {
      if(it->EntityName() == name) {
        port = *it;
        return true;
      }
    }
    return false;

  }


  //: Get output
  bool DPStreamOpBodyC::GetOPlug(const StringC &name,DPOPlugBaseC &port)
  {
    DListC<DPOPlugBaseC> plugs = OPlugs();
    for(DLIterC<DPOPlugBaseC> it(plugs);it;it++) {
      if(it->EntityName() == name) {
        port = *it;
        return true;
      }
    }
    return false;
  }


  //: Set an input
  bool DPStreamOpBodyC::SetIPort(const StringC &name,const DPIPortBaseC &port)
  {
    DListC<DPIPlugBaseC> plugs = IPlugs();
    for(DLIterC<DPIPlugBaseC> it(plugs);it;it++) {
      if(it->EntityName() == name) {
        DPIPortBaseC matchedPort;
        if(!SystemTypeConverter().ConvertIPort(port,it->InputType(),matchedPort))
          return false;
        if(!it->ConnectPort(matchedPort)) {
          RavlError("Failed to set port.");
          return false;
        }
        return true;
      }
    }
#if DODEBUG
    RavlError("Failed to find IPlug '%s' from",name.c_str());
    for(DLIterC<DPIPlugBaseC> it(plugs);it;it++) {
      RavlInfo(" %s type %s  ",it->EntityName().c_str(),TypeName(it->InputType()));
    }
#endif
    return false;
  }

  //: Set an output
  bool DPStreamOpBodyC::SetOPort(const StringC &name,const DPOPortBaseC &port)
  {
    DListC<DPOPlugBaseC> plugs = OPlugs();
    for(DLIterC<DPOPlugBaseC> it(plugs);it;it++) {
      if(it->EntityName() == name) {
        DPOPortBaseC matchedPort;
        if(!SystemTypeConverter().ConvertOPort(port,it->OutputType(),matchedPort))
          return false;
        if(!it->ConnectPort(matchedPort)) {
          RavlError("Failed to set port.");
          return false;
        }
        return true;
      }
    }
#if DODEBUG
    RavlError("Failed to find OPlug '%s' from",name.c_str());
    for(DLIterC<DPOPlugBaseC> it(plugs);it;it++) {
      RavlInfo(" %s type %s  ",it->EntityName().c_str(),TypeName(it->OutputType()));
    }
#endif
    return false;
  }

  //: Get output
  bool DPStreamOpBodyC::GetOPort(const StringC &name,DPOPortBaseC &port) {
    DListC<DPOPortBaseC> ports= OPorts();
    for(DLIterC<DPOPortBaseC> it(ports);it;it++) {
      if(it->EntityName() == name) {
        port = *it;
        return true;
      }
    }
    return false;
  }

  //: Get input
  bool DPStreamOpBodyC::GetIPort(const StringC &name,DPIPortBaseC &port) {
    DListC<DPIPortBaseC> ports = IPorts();
    for(DLIterC<DPIPortBaseC> it(ports);it;it++) {
      if(it->EntityName() == name) {
        port = *it;
        return true;
      }
    }
    return false;
  }

  //: Get output as specified type
  bool DPStreamOpBodyC::GetOPort(const StringC &name,DPOPortBaseC &port,const std::type_info &dataType)
  {
    DPOPortBaseC orgPort;
    if(!GetOPort(name,orgPort))
      return false;
    return SystemTypeConverter().ConvertOPort(orgPort,dataType,port);
  }

  //: Get input as specified type
  bool DPStreamOpBodyC::GetIPort(const StringC &name,DPIPortBaseC &port,const std::type_info &dataType)
  {
    DPIPortBaseC orgPort;
    if(!GetIPort(name,orgPort))
      return false;
    return SystemTypeConverter().ConvertIPort(orgPort,dataType,port);
  }


  //: Dump information about the stream op.
  bool DPStreamOpBodyC::Dump(std::ostream &strm) const {
    strm << "DPStreamOpBody '" << EntityName() << "' Type:" << RavlN::TypeName(typeid(*this)) << " \n";
    strm << " IPlugs:\n";
    for(DLIterC<DPIPlugBaseC> it(IPlugs());it;it++){
      strm << "  '" << it->EntityName() << "' Type:" << TypeName(it->InputType()) << "\n";
    }
    strm << " OPlugs:\n";
    for(DLIterC<DPOPlugBaseC> it(OPlugs());it;it++){
      strm << "  '" << it->EntityName() << "' Type:" << TypeName(it->OutputType()) << "\n";
    }
    strm << " IPorts:\n";
    for(DLIterC<DPIPortBaseC> it(IPorts());it;it++){
      strm << "  '" << it->EntityName() << "' Type:" << TypeName(it->InputType()) << "\n";
    }
    strm << " OPorts:\n";
    for(DLIterC<DPOPortBaseC> it(OPorts());it;it++){
      strm << "  '" << it->EntityName() << "' Type:" << TypeName(it->OutputType()) << "\n";
    }
    strm << "Done\n";
    return true;
  }


  static DPEntityBodyC::RefT DPStreamOp0(const DPStreamOpC &bod)
  { return bod.BodyPtr(); }

  DP_REGISTER_CONVERSION(DPStreamOp0,1);

  static DPStreamOpC DPIStreamOp1(const DPIStreamOpC<float,float> &bod)
  { return bod; }

  DP_REGISTER_CONVERSION(DPIStreamOp1,1);

  static DPStreamOpC DPIStreamOp2(const DPIStreamOpC<float,SArray1dC<float> > &bod)
  { return bod; }

  DP_REGISTER_CONVERSION(DPIStreamOp2,1);

  static DPStreamOpC DPIStreamOp3(const DPIStreamOpC<SArray1dC<float>,SArray1dC<float> > &bod)
  { return bod; }

  DP_REGISTER_CONVERSION(DPIStreamOp3,1);

  static DPStreamOpC DPIStreamOp4(const DPIStreamOpC<SArray1dC<RealT>,SArray1dC<RealT> > &bod)
  { return bod; }

  DP_REGISTER_CONVERSION(DPIStreamOp4,1);


  static DPStreamOpC DPIStreamOp5(const DPIStreamOpC<SArray1dC<ComplexC>,SArray1dC<RealT> > &bod)
  { return bod; }

  DP_REGISTER_CONVERSION(DPIStreamOp5,1);

  static DPStreamOpC DPOStreamOp1(const DPOStreamOpC<float,float> &bod)
  { return bod; }

  DP_REGISTER_CONVERSION(DPOStreamOp1,1);


  static DPStreamOpC DPOStreamOp2(const DPOStreamOpC<float,SArray1dC<float> > &bod)
  { return bod; }

  DP_REGISTER_CONVERSION(DPOStreamOp2,1);

  static DPStreamOpC DPOStreamOp3(const DPOStreamOpC<SArray1dC<float>,SArray1dC<float> > &bod)
  { return bod; }

  DP_REGISTER_CONVERSION(DPOStreamOp3,1);

  static DPStreamOpC DPOStreamOp4(const DPOStreamOpC<SArray1dC<RealT>,SArray1dC<RealT> > &bod)
  { return bod; }

  DP_REGISTER_CONVERSION(DPOStreamOp4,1);


  static DPStreamOpC DPOStreamOp5(const DPOStreamOpC<SArray1dC<ComplexC>,SArray1dC<RealT> > &bod)
  { return bod; }

  DP_REGISTER_CONVERSION(DPOStreamOp5,1);


  static DPStreamOpC DPStreamOp6(const DPStreamOpBodyC::RefT &bod)
  { return DPStreamOpC(bod.BodyPtr()); }

  DP_REGISTER_CONVERSION(DPStreamOp6,1);

}
