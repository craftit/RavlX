// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_REFCOUNTER_HEADER
#define RAVL_REFCOUNTER_HEADER 1
///////////////////////////////////////////////////////
//! docentry="Ravl.API.Core.Reference Counting"
//! file="Ravl/Core/Base/RefCounter.hh"
//! lib=RavlCore
//! userlevel=Normal
//! author="Charles Galambos"
//! example=exRefCounter.cc

#include "Ravl/Types.hh"
#include "Ravl/Atomic.hh"
#include "Ravl/Exception.hh"
#include "Ravl/Assert.hh"
#include "Ravl/StdHash.hh"

#if RAVL_HAVE_ANSICPPHEADERS
#include <typeinfo>
#else
#include <typeinfo.h>
#endif

//: Ravl library namespace.

namespace RavlN {  
  
  class BinIStreamC;
  class BinOStreamC;
  
  enum CreateBodyFlagT { CreateBodyE };
  
  template<class BodyT> class RCHandleC;

  // Forward declarations.
  
  template<class BodyT> class RCHandleC;
  
  //! userlevel=Normal
  //: Base class for all reference counted objects
  // This holds a count of the number of handles that
  // are available for this object.
  
  class RCBodyC {
  public:
    RCBodyC()
    { ravl_atomic_set(&counter,0); }
    //: Default constructor.
    // Creates a handle with 0 reference counts.

    RCBodyC(const RCBodyC &)
    { ravl_atomic_set(&counter,0); }
    //: Make sure we don't copy the reference counter.

    ~RCBodyC();
    //: Destructor.

    const RCBodyC &operator=(const RCBodyC &other)
    { return *this; }
    //: Make sure reference counter isn't over written by assigment.

    bool Save(std::ostream &strm) const
    { return true; }
    //: Default text save.

    bool Save(BinOStreamC &strm) const
    { return true; }
    //: Default binary save.

    UIntT References() const
    {
#if QMAKE_PARANOID
      RavlAssert(ravl_atomic_read(&counter) >= 0);
#endif
      return ravl_atomic_read(&counter);
    }
    //: Access count of handles open to this object.
    
    RCBodyC &Copy() const;
    //: Make copy of body.
    // This should be provided in derived classes.
    // this function will issue an assertion failure if called.
    
    RCBodyC &DeepCopy(UIntT levels = ((UIntT) -1)) const;
    //: Make a deep copy of body.
    // This should be provided in derived classes.
    // this function will issue an assertion failure if called.
    
    void IncRefCounter() const
    { ravl_atomic_inc(&counter); }
    //: Increment reference counter.
    
    bool DecRefCounter() const
    { return ravl_atomic_dec_and_test(&counter) != 0; }
    //: Decrement reference counter.
    
    void SetRefCounter(UIntT val) const
    { ravl_atomic_set(&counter,val); }
    // Set reference counter. For use in RCHandle only.
    //: Increment reference counter.

    void RavlRegisterInstance(const std::type_info &type) const;
    //: Register a value has been created

    void RavlUnregisterInstance(const std::type_info &type) const;
    //: Unregister a value

  private:
    mutable ravl_atomic_t counter;
  };
  
  template<class DataT> class SmartPtrC;
  
  //! userlevel=Normal
  //: Base class for handles to reference counter objects
  
  template<class BodyT>
  class RCHandleC {
  public:
    RCHandleC()
      : body(0)
    {}
    //: Default constructor.
    // creates an invalid handle.
    
    RCHandleC(const RCHandleC<BodyT> &oth)
      : body(oth.body)
    { 
      if(body != 0) {
#if QMAKE_PARANOID
        RavlAssert(oth.body->References() > 0);
#endif
	body->IncRefCounter();
      }
    }
    //: Copy Constructor.
    // Creates a new reference to 'oth'
    
    RCHandleC(CreateBodyFlagT)
      : body(new BodyT())
    {
#if QMAKE_PARANOID
      body->RavlRegisterInstance(typeid(BodyT));
#endif
      body->SetRefCounter(1);
    }
    //: Constructor.
    // creates a body class with its default constructor.
    
    RCHandleC(std::istream &is)
      : body(new BodyT())
    { 
#if QMAKE_PARANOID
      body->RavlRegisterInstance(typeid(BodyT));
#endif
      body->SetRefCounter(1);
      is >> *body;
    }
    //: Load body from stream.
    
    ~RCHandleC() {
      if(body == 0)
	return;
      if(body->References() == 1) {
        // To save an expensive atom operation just delete the object
#if RAVL_CHECK
        // Stop the check code getting upset about the reference being non-zero.
        body->SetRefCounter(0);
#endif
#if QMAKE_PARANOID
        body->RavlUnregisterInstance(typeid(*body));
#endif
        delete body;
        return ;
      }
      if(body->DecRefCounter()) {
#if QMAKE_PARANOID
        body->RavlUnregisterInstance(typeid(*body));
#endif
        delete body;
      }
    }
    //: Destructor.
    // Decrement reference count, and delete object if it reaches zero.
    
    bool Save(std::ostream &out) const
    { out << *body; return true; }
    //: Save body to stream.
    
    void SetPtr(const BodyT *ptr)
    {
      if(ptr != 0)
        ptr->IncRefCounter();
      if(body != 0) {
        if(body->DecRefCounter()) {
#if QMAKE_PARANOID
          body->RavlUnregisterInstance(typeid(*body));
#endif
          delete body;
        }
      }
      body = const_cast<BodyT *>(ptr);
    }
    //: Assign to pointer.

    const RCHandleC<BodyT> &operator=(const RCHandleC<BodyT> &oth)
    {
#if QMAKE_PARANOID
      if(oth.IsValid()) { RavlAssert(oth.BodyPtr()->References() > 0); }
#endif
      SetPtr(oth.body);
      return *this;
    }
    //: Assign handle.

    RCHandleC<BodyT> DeepCopy(UIntT levels = ((UIntT) -1)) const
    { return RCHandleC<BodyT>(static_cast<BodyT &>(Body().DeepCopy(levels))); }
    //: Do a deep copy of the object.
    
    bool operator==(const RCHandleC<BodyT> &oth) const
    { return body == oth.body; }
    //: Are handles to the same object ?
    
    bool operator!=(const RCHandleC<BodyT> &oth) const
    { return body != oth.body; }
    //: Are handles to different objects ?
    
    size_t Hash() const
    { return StdHash(reinterpret_cast<const void *>(body)); }
    //: Default hash function.
    // This hashes on the address of the body.
    
    bool IsValid() const
    { return body != 0; }
    //: Test if this is a valid handle. 

    void Invalidate() { 
      if(body == 0) return ;
      if(body->DecRefCounter()) {
#if QMAKE_PARANOID
        body->RavlUnregisterInstance(typeid(*body));
#endif
	delete body;
      }
      body = 0;
    }
    //: Invalidate this handle.
    // Detaches the body from the handle.
    
    template<class DT>
    bool IsHandleType(const DT &/*dummy*/) const
    { return (dynamic_cast<const DT *>(body) != 0); }
    //: Is handle of given type ?
    
    template<class DT>
    void CheckHandleType(const DT &dummy) const RAVL_THROW(ExceptionErrorCastC) { 
      if(!IsHandleType(dummy))
        throw ExceptionErrorCastC("RCHandleC::CheckRCHandleType(), Failed.",
				  typeid(Body()),
				  typeid(DT));
    }
    //: Check handle type. Throw an exception if not.
    
    const void *VoidPtr() const
    { return body; }
    //: Access unique address for this object.
    // Used in PointerManagerC.  Not for general use
    // this interface may changed in future releases.

    BodyT *BodyPtr()
    { return body; }
    //! userlevel=Advanced
    //: Access body pointer.
    // Used for working with SmartPtrC's.
    
    const BodyT *BodyPtr() const
    { return body; }
    //! userlevel=Advanced
    //: Access body pointer.
    // Use for working with SmartPtrC's.

    const std::type_info &BodyTypeInfo() const
    { return typeid(*(this->body)); }
    //: Get the type info of the value pointed to.
    // This  can be used to avoid clang warnings when using methods to provide data to typeid()

  private:
    BodyT *body;
    
  protected:
    RCHandleC(BodyT &bod)
      : body(&bod)
    {
      if(body->References() == 0) {
#if QMAKE_PARANOID
        body->RavlRegisterInstance(typeid(*body));
#endif
        // Avoid unneeded atomic operation, they're slow.
        body->SetRefCounter(1);
      } else
        body->IncRefCounter();
    }
    //: Body constructor.
    
    RCHandleC(const BodyT *bod)
      : body(const_cast<BodyT *>(bod))
    {
      if(body != 0) {
        if(body->References() == 0) {
#if QMAKE_PARANOID
          body->RavlRegisterInstance(typeid(*body));
#endif
          // Avoid unneeded atomic operation, they're slow.
          body->SetRefCounter(1);
        } else
          body->IncRefCounter();
      }
    }
    //: Body base constructor.
    // This is used where the body may be a null ptr, such as
    // in the virtual constructor after a failed load.
    
    BodyT &Body() { 
      RAVL_PARANOID(RavlAssertMsg(IsValid(),"Attempt to access invalid handle. "));
      return *body; 
    }
    //: Access body of object.
    
    const BodyT &Body() const { 
      RAVL_PARANOID(RavlAssertMsg(IsValid(),"Attempt to access invalid handle. "));
      return *body; 
    }
    //: Constant access to body of object.
    
    static BodyT *BodyPtr(RCHandleC<BodyT> &bod)
    { return bod.body; }
    //: Access body point from a derived class.
    
    static const BodyT *BodyPtr(const RCHandleC<BodyT> &bod)
    { return bod.body; }
    //: Access body point from a derived class.
    
  public:
    UIntT References() const
    { return Body().References(); }
    //! userlevel=Normal
    //: Find the number of references to the body of this object.
    
    friend class SmartPtrC<BodyT>; 
  };
  
  
  std::istream &operator>>(std::istream &strm,RCBodyC &obj);
  //: Input body.
  // No-op.
  
  std::ostream &operator<<(std::ostream &strm,const RCBodyC &obj);
  //: Output body.
  // No-op.
  
  BinIStreamC &operator>>(BinIStreamC &strm,RCBodyC &obj);
  //: Input body.
  // No-op.
  
  BinOStreamC &operator<<(BinOStreamC &strm,const RCBodyC &obj);
  //: Output body.
  // No-op.
    
  template<class BodyT>
  std::ostream &operator<<(std::ostream &strm,const RCHandleC<BodyT> &obj) { 
    obj.Save(strm); 
    return strm;
  }
  //: Write a handle to a stream.
  
  template<class BodyT>
  std::istream &operator>>(std::istream &strm,RCHandleC<BodyT> &obj) {
    obj = RCHandleC<BodyT>(strm);
    return strm;
  }
  //: Read a handle from a stream.

  void EnableClassInstanceStats(bool flag);
  //: Enable and disable the collection of instance counts.
  //: This only works in debug builds

  void DumpInstanceCounts(std::ostream &strm,size_t countThreshold = 1);
  //: Write out class instance counts to strm.
  // This only works in debug builds.

  bool IsClassInstanceStatsEnabled();
  //: Test if class stats are enabled
}

#endif
