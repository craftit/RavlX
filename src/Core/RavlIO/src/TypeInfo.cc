// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlIO
//! file="Ravl/Core/IO/TypeInfo.cc"

#include "Ravl/DP/TypeInfo.hh"
#include "Ravl/DList.hh"
#include "Ravl/MTLocks.hh"

namespace RavlN {
  ////////////////////////////
  //: Default constructor.
  
  DPTypeInfoBodyC::DPTypeInfoBodyC(const std::type_info &typeInfo)
    : typeInfo(typeInfo)
  { 
    MTWriteLockC writeLock;
    Types()[typeInfo.name()] = DPTypeInfoC(*this); 
  }
  
  //: Move 'count' elements from input to output.
  // return number of elements actually moved.

  UIntT DPTypeInfoBodyC::Move(DPIPortBaseC &iport,DPOPortBaseC &oport,UIntT count) const
  {
    RavlAssertMsg(0,"abstract method"); // Abstract function called.
    return 0;
  }

  /////////////////////////////
  //: Create instance of the type with the default constructor.
  
  RCWrapAbstractC DPTypeInfoBodyC::Create() const  {
    RavlAssertMsg(0,"abstract method"); // Abstract function called.
    return RCWrapAbstractC();
  }
  
  //: Create instance of the type from stream constructor.
  
  RCWrapAbstractC DPTypeInfoBodyC::Create(std::istream &) const {
    RavlAssertMsg(0,"abstract method"); // Abstract function called.
    return RCWrapAbstractC();
  }

  //: Put generic object to port.
  // Will throw ExceptionErrorCastC if types aren't correct, or in debug mode
  // an assert may fail.
  
  bool 
  DPTypeInfoBodyC::Put(DPOPortBaseC &,const RCWrapAbstractC &) {
    RavlAssertMsg(0,"abstract method"); // Abstract function called.
    return false;
  }
  
  //: Get generic object from port.
  // Will throw ExceptionErrorCastC if types aren't correct, or in debug mode
  // an assert may fail.
  
  RCWrapAbstractC 
  DPTypeInfoBodyC::Get(DPIPortBaseC &) {
    RavlAssertMsg(0,"abstract method"); // Abstract function called.
    return RCWrapAbstractC();
  }
  
  //: Create an input pipe
  
  DPIPipeBaseC DPTypeInfoBodyC::CreateIPipe(const DPEntityC &) const {
    RavlAssertMsg(0,"abstract method"); // Abstract function called.
    return DPIPipeBaseC();
  }
  
  //: Create an output pipe
  
  DPOPipeBaseC DPTypeInfoBodyC::CreateOPipe(const DPEntityC &) const {
    RavlAssertMsg(0,"abstract method"); // Abstract function called.
    return DPOPipeBaseC();
  }

  //: Get an item from the port and write it to the BinOStreamC.
  
  bool DPTypeInfoBodyC::GetAndWrite(DPIPortBaseC &port,BinOStreamC &strm) const {
    RavlAssertMsg(0,"abstract method"); // Abstract function called.
    return false;    
  }



  Int64T DPTypeInfoBodyC::GetAndWriteArray(DPIPortBaseC &port,Int64T size,BinOStreamC &strm) const {
    RavlAssertMsg(0,"abstract method"); // Abstract function called.
    return 0;
  }

  //: Read an item from the binary stream and write it to a port.

  bool DPTypeInfoBodyC::ReadAndPut(BinIStreamC &strm,DPOPortBaseC &port) const {
    RavlAssertMsg(0,"abstract method"); // Abstract function called.
    return false;
  }



  bool DPTypeInfoBodyC::ReadAndPutArray(Int64T size,BinIStreamC &strm,DPOPortBaseC &port) const {
    RavlAssertMsg(0,"abstract method"); // Abstract function called.
    return false;
  }

  //: Convert from an IPort to an stream of abstract handles.
  
  DPIPortC<RCWrapAbstractC> DPTypeInfoBodyC::CreateConvToAbstract(DPIPortBaseC &port) {
    RavlAssertMsg(0,"abstract method");
    return DPIPortC<RCWrapAbstractC>();    
  }
    
  //: Convert from a stream of abstract handles to an IPort  
  
  DPIPortBaseC DPTypeInfoBodyC::CreateConvFromAbstract(DPIPortC<RCWrapAbstractC> &port) {
    RavlAssertMsg(0,"abstract method");
    return DPIPortBaseC();
  }

  //: Convert from an IPort to an stream of abstract handles.
  
  DPOPortC<RCWrapAbstractC> DPTypeInfoBodyC::CreateConvToAbstract(DPOPortBaseC &port) {
    RavlAssertMsg(0,"abstract method");
    return DPOPortC<RCWrapAbstractC>();    
  }
    
  //: Convert from a stream of abstract handles to an IPort  
  
  DPOPortBaseC DPTypeInfoBodyC::CreateConvFromAbstract(DPOPortC<RCWrapAbstractC> &port) {
    RavlAssertMsg(0,"abstract method");
    return DPOPortBaseC();
  }
  
  
  /////////////////////////////
  //: Access type list.
  
  HashC<const char *,DPTypeInfoC> &DPTypeInfoBodyC::Types() {
    static HashC<const char *,DPTypeInfoC> typelist;
    return typelist;
  }
  
  //////// DPTypeInfoC /////////////////////////////////////////////////
  
  //: Lookup type. 
  // Creates an invalid handle if failed.
  
  DPTypeInfoC::DPTypeInfoC(const std::type_info &ti) 
    : DPEntityC(true)
  {
    MTReadLockC readLock;
    DPTypeInfoC *tip = DPTypeInfoBodyC::Types().Lookup(ti.name());
    if(tip != 0)
      (*this) = *tip;
  }
  

  const DPTypeInfoC &TypeInfo(const std::type_info &ti)
  {
    MTReadLockC readLock;
    return DPTypeInfoBodyC::Types()[ti.name()];
  }
  //: Access extra information about a type.

  const DPTypeInfoC &TypeInfo(const char *tn)
  {
    MTReadLockC readLock;
    return DPTypeInfoBodyC::Types()[tn];
  }
  //: Access extra information about a type.


}
