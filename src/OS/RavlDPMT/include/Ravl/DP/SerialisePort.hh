// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SERIALISEPORT_HEADER
#define RAVL_SERIALISEPORT_HEADER
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlDPMT
//! date="2/5/2003"
//! docentry="Ravl.API.Core.Data Processing.Threads" 
//! file="Ravl/OS/DataProc/SerialisePort.hh"

#include "Ravl/DP/StreamOp.hh"
#include "Ravl/Threads/Mutex.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Serialise requests to port when used by more than one thread .
  
  template<typename DataT>
  class DPISerialisePortBodyC
    : public DPIStreamOpBodyC<DataT,DataT>
  {
  public:
    DPISerialisePortBodyC(const DPIPortC<DataT> &in)
      : DPIStreamOpBodyC<DataT,DataT>(in)
    {}
    //: Constructor.
    
    virtual bool IsGetReady() const {
      MutexLockC lock(access);
      return DPIStreamOpBodyC<DataT,DataT>::IsGetReady(); 
    }
    //: Is some data ready ?
    // true = yes.
    
    virtual bool IsGetEOS() const {
      MutexLockC lock(access);      
      return DPIStreamOpBodyC<DataT,DataT>::IsGetEOS(); 
    }
    //: Has the End Of Stream been reached ?
    // true = yes.
    
    virtual DataT Get()  {
      MutexLockC lock(access);
      return this->input.Get();
    }
    //: Get next piece of data.
    
    virtual bool Get(DataT &buff) { 
      MutexLockC lock(access);
      return this->input.Get(buff);
    }
    //: Try and get next piece of data.
    
    virtual IntT GetArray(SArray1dC<DataT> &data) {
      MutexLockC lock(access);
      return this->input.GetArray(data);
    }
    //: Get an array of data from stream.
    
    virtual bool Discard() { 
      MutexLockC lock(access);
      return this->input.Discard();
    }
    //: Discard the next input datum.
    
    //:-----------------------------------------------------------
    // Attribute handling.
    
    virtual bool GetAttr(const StringC &attrName,StringC &attrValue) {
      MutexLockC lock(access);
      return this->input.GetAttr(attrName,attrValue);
    }
    //: Get a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    virtual bool SetAttr(const StringC &attrName,const StringC &attrValue) {
      MutexLockC lock(access);
      return this->input.SetAttr(attrName,attrValue);
    }
    //: Set a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    virtual bool GetAttr(const StringC &attrName,IntT &attrValue) {
      MutexLockC lock(access);
      return this->input.GetAttr(attrName,attrValue);
    }
    //: Get a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    virtual bool SetAttr(const StringC &attrName,const IntT &attrValue) {
      MutexLockC lock(access);
      return this->input.SetAttr(attrName,attrValue);
    }
    //: Set a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    virtual bool GetAttr(const StringC &attrName,RealT &attrValue) {
      MutexLockC lock(access);
      return this->input.GetAttr(attrName,attrValue);
    }
    //: Get a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    virtual bool SetAttr(const StringC &attrName,const RealT &attrValue) {
      MutexLockC lock(access);
      return this->input.SetAttr(attrName,attrValue);
    }
    //: Set a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    virtual bool GetAttr(const StringC &attrName,bool &attrValue) {
      MutexLockC lock(access);
      return this->input.GetAttr(attrName,attrValue);
    }
    //: Get a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    virtual bool SetAttr(const StringC &attrName,const bool &attrValue) {
      MutexLockC lock(access);
      return this->input.SetAttr(attrName,attrValue);
    }
    //: Set a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    virtual bool GetAttrList(DListC<StringC> &list) const {
      MutexLockC lock(access);
      return this->input.GetAttrList(list);
    }
    //: Get list of attributes available.
    // This method will ADD all available attribute names to 'list'.
    
    virtual bool GetAttrTypes(DListC<AttributeTypeC> &list) const {
      MutexLockC lock(access);
      return this->input.GetAttrTypes(list);
    }
    //: Get a list of available attribute types.
    
#if 0
    virtual AttributeTypeC GetAttrType(const StringC &attrName) const {
      MutexLockC lock(access);
      return this->input.GetAttrType(attrName);
    }
    //: Get type of a particular attribute.
    // Returns an invalid handle if attribute is unknown.
#endif
    
    virtual IntT RegisterChangedSignal(const StringC &attrName,const TriggerC &trig) {
      MutexLockC lock(access);
      return this->input.RegisterChangedSignal(attrName,trig);
    }
    
    //: Register a value changed signal.
    // Note: This method may not be implemented for all AttributeCtrl's.
    // Returns an id for the trigger, or -1 if operation fails.
    
    virtual bool RemoveChangedSignal(IntT id) {
      MutexLockC lock(access);
      return this->input.RemoveChangedSignal(id);
    }
    //: Remove a changed signal.
    // Note: This method may not be implemented for all AttributeCtrl's.
    
    virtual bool RegisterAttribute(const AttributeTypeC &attr) {
      MutexLockC lock(access);
      return this->input.RegisterAttribute(attr);      
    }
    //: Register a new attribute type.
    
  protected:
    MutexC access;
  };
  
  
  //! userlevel=Normal
  //: Serialise requests to port when used by more than one thread .
  
  template<typename DataT>
  class DPISerialisePortC
    : public DPIStreamOpC<DataT,DataT>
  {
  public:
    DPISerialisePortC()
      : DPEntityC(true)
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    DPISerialisePortC(const DPIPortC<DataT> &in)
      : DPEntityC(*new DPISerialisePortBodyC<DataT>(in))
    {}
    //: Construct from port.
    
  };

  
  //! userlevel=Develop
  //: Serialise requests to port when used by more than one thread .
  
  template<typename DataT>
  class DPOSerialisePortBodyC
    : public DPOStreamOpBodyC<DataT,DataT>
  {
  public:
    DPOSerialisePortBodyC(const DPOPortC<DataT> &out)
      : DPOStreamOpBodyC<DataT,DataT>(out)
    {}
    //: Constructor.
    
    virtual void PutEOS() {
      MutexLockC lock(access);
      this->output.PutEOS();
    }
    //: Put End Of Stream marker.
    
    virtual bool IsPutReady() const {
      MutexLockC lock(access);
      return this->output.IsPutReady();
    }
    //: Is port ready for data ?
    
    virtual bool Put(const DataT &data) { 
      MutexLockC lock(access);
      return this->output.Put(data);
    }
    //: Put data.
    // This function MUST be provided by client class.
    
    virtual IntT PutArray(const SArray1dC<DataT> &data) {
      MutexLockC lock(access);
      return this->output.PutArray(data);
    }
    //: Put an array of data to stream.
    
    //:-----------------------------------------------------------
    // Attribute handling.
    
    virtual bool GetAttr(const StringC &attrName,StringC &attrValue) {
      MutexLockC lock(access);
      return this->output.GetAttr(attrName,attrValue);
    }
    //: Get a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    virtual bool SetAttr(const StringC &attrName,const StringC &attrValue) {
      MutexLockC lock(access);
      return this->output.SetAttr(attrName,attrValue);
    }
    //: Set a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    virtual bool GetAttr(const StringC &attrName,IntT &attrValue) {
      MutexLockC lock(access);
      return this->output.GetAttr(attrName,attrValue);
    }
    //: Get a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    virtual bool SetAttr(const StringC &attrName,const IntT &attrValue) {
      MutexLockC lock(access);
      return this->output.SetAttr(attrName,attrValue);
    }
    //: Set a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    virtual bool GetAttr(const StringC &attrName,RealT &attrValue) {
      MutexLockC lock(access);
      return this->output.GetAttr(attrName,attrValue);
    }
    //: Get a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    virtual bool SetAttr(const StringC &attrName,const RealT &attrValue) {
      MutexLockC lock(access);
      return this->output.SetAttr(attrName,attrValue);
    }
    //: Set a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    virtual bool GetAttr(const StringC &attrName,bool &attrValue) {
      MutexLockC lock(access);
      return this->output.GetAttr(attrName,attrValue);
    }
    //: Get a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    virtual bool SetAttr(const StringC &attrName,const bool &attrValue) {
      MutexLockC lock(access);
      return this->output.SetAttr(attrName,attrValue);
    }
    //: Set a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    virtual bool GetAttrList(DListC<StringC> &list) const {
      MutexLockC lock(access);
      return this->output.GetAttrList(list);
    }
    //: Get list of attributes available.
    // This method will ADD all available attribute names to 'list'.
    
    virtual bool GetAttrTypes(DListC<AttributeTypeC> &list) const {
      MutexLockC lock(access);
      return this->output.GetAttrTypes(list);
    }
    //: Get a list of available attribute types.
    
#if 0
    virtual AttributeTypeC GetAttrType(const StringC &attrName) const {
      MutexLockC lock(access);
      return this->output.GetAttrType(attrName);
    }
    //: Get type of a particular attribute.
    // Returns an invalid handle if attribute is unknown.
#endif
    
    virtual IntT RegisterChangedSignal(const StringC &attrName,const TriggerC &trig) {
      MutexLockC lock(access);
      return this->output.RegisterChangedSignal(attrName,trig);
    }
    
    //: Register a value changed signal.
    // Note: This method may not be implemented for all AttributeCtrl's.
    // Returns an id for the trigger, or -1 if operation fails.
    
    virtual bool RemoveChangedSignal(IntT id) {
      MutexLockC lock(access);
      return this->output.RemoveChangedSignal(id);
    }
    //: Remove a changed signal.
    // Note: This method may not be implemented for all AttributeCtrl's.
    
    virtual bool RegisterAttribute(const AttributeTypeC &attr) {
      MutexLockC lock(access);
      return this->output.RegisterAttribute(attr);      
    }
    //: Register a new attribute type.
    
  protected:
    MutexC access;
  };
  
  
  //! userlevel=Normal
  //: Serialise requests to port when used by more than one thread .
  
  template<typename DataT>
  class DPOSerialisePortC
    : public DPOStreamOpC<DataT,DataT>
  {
  public:
    DPOSerialisePortC()
      : DPEntityC(true)
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    DPOSerialisePortC(const DPOPortC<DataT> &out)
      : DPEntityC(*new DPOSerialisePortBodyC<DataT>(out))
    {}
    //: Construct from port.
    
  };
  
  template<typename DataT>
  DPISerialisePortC<DataT> Serialise(const DPIPortC<DataT> &in)
  { return DPISerialisePortC<DataT>(in); }
  //: Serialise acceses to a port.

  template<typename DataT>
  DPOSerialisePortC<DataT> Serialise(const DPOPortC<DataT> &in)
  { return DPOSerialisePortC<DataT>(in); }
  //: Serialise acceses to a port.
  
}

#endif
