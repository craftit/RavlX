// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SMARTPTR_HEADER 
#define RAVL_SMARTPTR_HEADER 1
//! rcsid="$Id$"
//! lib=RavlCore
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Reference Counting"
//! file="Ravl/Core/Base/SmartPtr.hh"

#include "Ravl/RCHandleV.hh"
#include "Ravl/RCAbstract.hh"
#include "Ravl/Exception.hh"

namespace RavlN {
  
  //! userlevel=Advanced
  //: Smart pointer.
  
  template<class DataT>
  class SmartPtrC
    : public RCHandleC<DataT>
  {
  public:
    typedef DataT PtrTargetT;

    SmartPtrC()
    {}
    //: Default constructor.
    
    SmartPtrC(const RCHandleC<DataT> &handle)
      : RCHandleC<DataT>(handle)
    {}
    //: Construct from a normal handle.
    
    SmartPtrC(DataT &data)
      : RCHandleC<DataT>(data)
    {}
    //: Construct from data.

    SmartPtrC(const DataT *data)
      : RCHandleC<DataT>(data)
    {}
    //: Construct from data.
    
    DataT *BodyPtr()
    { return this->body; }
    //: Access pointer to body.
    
    const DataT *BodyPtr() const
    { return this->body; }
    //: Access pointer to body.
    
    DataT *operator->()
    { return &RCHandleC<DataT>::Body(); }
    //: Access body.
    
    const DataT *operator->() const
    { return &RCHandleC<DataT>::Body(); }
    //: Access body.
    
    bool operator==(const SmartPtrC<DataT> &other) const
    { return this->body == other.body; }
    //: Equality test with another smart ptr.
    
    bool operator==(const DataT *other) const
    { return this->body == other; }
    //: Equality test with a direct ptr.

    bool operator==(const DataT &other) const
    { return this->body == &other; }
    //: Equality test with a reference to an object.

    operator DataT *()
    { return this->body; }
    //: Access body.
    
    operator const DataT *() const
    { return this->body; }
    //: Access body.
    
    const SmartPtrC<DataT> &operator=(const DataT *other) {
      DataT *oth = const_cast<DataT *>(other);
      if(oth != 0) {
        if(oth->References() == 0) {
  #if QMAKE_PARANOID
          oth->RavlRegisterInstance(typeid(*oth));
  #endif
          // Avoid unneeded atomic operation, they're slow.
          oth->SetRefCounter(1);
        } else {
          oth->IncRefCounter();
        }
      }
      if(this->body != 0) {
	if(this->body->DecRefCounter()) {
#if QMAKE_PARANOID
	  this->body->RavlUnregisterInstance(typeid(*this->body));
#endif
  	  delete this->body;
	}
      }
      this->body = oth;
      return *this;
    }
    //: Assign handle.
    
    SmartPtrC<DataT> Copy() const
    {
      if(!this->IsValid())
        return SmartPtrC<DataT>();
      return dynamic_cast<DataT &>(this->body->Copy());
    }
    //: Make a copy of the referenced object.
    
    void reset()
    { this->Invalidate(); }
    //: Boost compatibility. Release handle
    
    void reset(const DataT *other) 
    { (*this) = other; }
    //: Boost compatibility. Assign to another object.
    
    DataT *get()
    { return this->body; }
    //: Boost compatibility. Get Ptr.

    const std::type_info &TypeInfo() const
    { return typeid(*(this->body)); }
    //: Get the type info of the value pointed to.
    // This  can be used to avoid clang warnings 'typeid(*ptr)' would cause.
  };
  
  //!userlevel=Develop
  
  template<typename StreamT,typename BodyT>
  void SaveStreamImpl(StreamT &strm,const SmartPtrC<BodyT> &ptr,const RCBodyVC &) {
    char handleStatus = ptr.IsValid() ? 'V' : 'I';
    strm << handleStatus;
    if(ptr.IsValid()) ptr->Save(strm);
  }  
  
  template<typename StreamT,typename BodyT>
  void SaveStreamImpl(StreamT &strm,const SmartPtrC<BodyT> &ptr,const RCBodyC &) {
    char handleStatus = ptr.IsValid() ? 'V' : 'I';
    strm << handleStatus;
    if(ptr.IsValid()) ptr->Save(strm);
  }
  
  
  template<typename StreamT,typename BodyT>
  void LoadStreamImpl(StreamT &strm,SmartPtrC<BodyT> &ptr,const RCBodyC &) {
    char handleStatus = 0;
    strm >> handleStatus;
    if(handleStatus == 'V') {
      ptr = new BodyT(strm);
    } else {
      if(handleStatus != 'I')
        throw ExceptionOperationFailedC("Unexpected value in input stream.");
      ptr = 0; // Ensure an invalid handle.
    }
  }
  
  template<typename StreamT,typename BodyT>
  void LoadStreamImpl(StreamT &strm,SmartPtrC<BodyT> &ptr,const RCBodyVC &) {
    char handleStatus = 0;
    strm >> handleStatus;
    if(handleStatus == 'V') {
      ptr = RAVL_VIRTUALCONSTRUCTOR(strm,BodyT);
    } else {
      if(handleStatus != 'I')
        throw ExceptionOperationFailedC("Unexpected value in input stream.");
      ptr = 0; // Ensure an invalid handle.
    }
  }
  
  //! Convert a smart pointer to a RCAbstract handle
  template<typename ValueT>
  inline RCAbstractC ToRCAbstract(const RavlN::SmartPtrC<ValueT> &value) {
    return RCAbstractC(const_cast<RavlN::RCBodyVC *>(static_cast<const RavlN::RCBodyVC *>(value.BodyPtr())));
  }

  //! Convert a smart pointer to from a RCAbstract handle
  template<typename ValueT>
  inline void FromRCAbstract(const RavlN::RCAbstractC &val,RavlN::SmartPtrC<ValueT> &value) {
    value = dynamic_cast<const ValueT *>(val.BodyPtr());
  }

  //!userlevel=Normal
  
  //: Write entity from binary stream
  template<typename BodyT>
  BinOStreamC &operator<<(BinOStreamC &strm,const SmartPtrC<BodyT> &ptr) {
    SaveStreamImpl(strm,ptr,*ptr);
    return strm;
  }
  
  //: Read entity from binary stream
  template<typename BodyT>
  BinIStreamC &operator>>(BinIStreamC &strm,SmartPtrC<BodyT> &ptr) {
    LoadStreamImpl(strm,ptr,*ptr);
    return strm;
  }

  //: Write entity from text stream
  template<typename BodyT>
  std::ostream &operator<<(std::ostream &strm,const SmartPtrC<BodyT> &ptr) {
    SaveStreamImpl(strm,ptr,*ptr);
    return strm;
  }
  
  //: Read entity from text stream
  template<typename BodyT>
  std::istream &operator>>(std::istream &strm,SmartPtrC<BodyT> &ptr) {
    LoadStreamImpl(strm,ptr,*ptr);
    return strm;
  }
}
#endif
