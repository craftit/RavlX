// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SMARTLAYERPTR_HEADER 
#define RAVL_SMARTLAYERPTR_HEADER 1
//! rcsid="$Id: SmartPtr.hh 5240 2005-12-06 17:16:50Z plugger $"
//! lib=RavlCore
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Reference Counting"
//! file="Ravl/Core/Base/SmartLayerPtr.hh"

#include "Ravl/RCLayer.hh"
#include "Ravl/SmartPtr.hh"

namespace RavlN {
  
  //! userlevel=Advanced
  //: Callback ptr.
  // See RCLayerC for an explanation of the difference between this and a plain SmartPtrC.
  
  template<class DataT>
  class SmartCallbackPtrC 
    : public SmartPtrC<DataT>
  {
  public:
    SmartCallbackPtrC()
    {}
    //: Default constructor.
    
    SmartCallbackPtrC(const RCHandleC<DataT> &handle)
      : SmartPtrC<DataT>(handle)
    {}
    //: Construct from a normal handle.
    
    SmartCallbackPtrC(DataT &data)
      : SmartPtrC<DataT>(data)
    {}
    //: Construct from data.
    
    SmartCallbackPtrC(const DataT *data)
      : SmartPtrC<DataT>(data)
    {}
    //: Construct from data.
    
  };
  
  //! userlevel=Advanced
  //: Smart owner pointer.
  // See RCLayerC for an explanation of the difference between this and a plain SmartPtrC.
  
  template<class DataT>
  class SmartOwnerPtrC
    : public RCLayerC<DataT>
  {
  public:
    SmartOwnerPtrC()
    {}
    //: Default constructor.
    
    SmartOwnerPtrC(const SmartCallbackPtrC<DataT> &handle,RCLayerHandleT handleType)
      : RCLayerC<DataT>(handle.BodyPtr(),handleType)
    {}
    //: Construct from a normal handle.
    // This assumes the handle is a callback handle.
    
    SmartOwnerPtrC(const RCHandleC<DataT> &handle,RCLayerHandleT handleType = RCLH_OWNER)
      : RCLayerC<DataT>(handle,handleType)
    {}
    //: Construct from a normal handle.
    // Constructs an owner handle by default.
    
    SmartOwnerPtrC(DataT &data,RCLayerHandleT handleType = RCLH_OWNER)
      : RCLayerC<DataT>(data,handleType)
    {}
    //: Construct from data.
    // Constructs an owner handle by default.
    
    SmartOwnerPtrC(const DataT *data,RCLayerHandleT handleType = RCLH_OWNER)
      : RCLayerC<DataT>(data,handleType)
    {}
    //: Construct from data.
    // Constructs an owner handle by default.
    
    DataT *BodyPtr()
    { return RCLayerC<DataT>::BodyPtr(); }
    //: Access pointer to body.
    
    const DataT *BodyPtr() const
    { return RCLayerC<DataT>::BodyPtr(); }
    //: Access pointer to body.
    
    DataT *operator->()
    { return RCLayerC<DataT>::BodyPtr(); }
    //: Access body.
    
    const DataT *operator->() const
    { return RCLayerC<DataT>::BodyPtr(); }
    //: Access body.
    
    operator DataT *()
    { return RCLayerC<DataT>::BodyPtr(); }
    //: Access body.
    
    operator const DataT *() const
    { return RCLayerC<DataT>::BodyPtr(); }
    //: Access body.
    
    UIntT Hash() const 
    { return StdHash(reinterpret_cast<const void *>(BodyPtr())); }
    //: Compute hash value for handle.

    const std::type_info &TypeInfo() const
    { return RCLayerC<DataT>::BodyTypeInfo(); }
    //: Get type of object we're pointing to.

  };
  
  // ------------------------------------------------

  template<typename StreamT,typename BodyT>
  void SaveStreamImpl(StreamT &strm,const SmartOwnerPtrC<BodyT> &ptr,const RCBodyVC &) {
    char handleStatus = ptr.IsValid() ? 'V' : 'I';
    strm << handleStatus;
    if(ptr.IsValid()) ptr->Save(strm);
  }  
  
  template<typename StreamT,typename BodyT>
  void SaveStreamImpl(StreamT &strm,const SmartOwnerPtrC<BodyT> &ptr,const RCBodyC &) {
    char handleStatus = ptr.IsValid() ? 'V' : 'I';
    strm << handleStatus;
    if(ptr.IsValid()) ptr->Save(strm);
  }
  
  template<typename StreamT,typename BodyT>
  void LoadStreamImpl(StreamT &strm,SmartOwnerPtrC<BodyT> &ptr,const RCBodyC &) {
    char handleStatus = 0;
    strm >> handleStatus;
    if(handleStatus == 'V') {
      ptr = new BodyT(strm);
    } else {
      if(handleStatus != 'I')
        throw ExceptionOperationFailedC("Unexpected value in input stream.");
      ptr.Invalidate(); // Ensure an invalid handle.
    }
  }
  
  template<typename StreamT,typename BodyT>
  void LoadStreamImpl(StreamT &strm,SmartOwnerPtrC<BodyT> &ptr,const RCBodyVC &) {
    char handleStatus = 0;
    strm >> handleStatus;
    if(handleStatus == 'V') {
      ptr = RAVL_VIRTUALCONSTRUCTOR(strm,BodyT);
    } else {
      if(handleStatus != 'I')
        throw ExceptionOperationFailedC("Unexpected value in input stream.");
      ptr.Invalidate(); // Ensure an invalid handle.
    }
  }
  
  // ------------------------------------------------
  
  //: Write entity from binary stream
  template<typename BodyT>
  BinOStreamC &operator<<(BinOStreamC &strm,const SmartOwnerPtrC<BodyT> &ptr) {
    SaveStreamImpl(strm,ptr,*ptr);
    return strm;
  }
  
  //: Read entity from binary stream
  template<typename BodyT>
  BinIStreamC &operator>>(BinIStreamC &strm,SmartOwnerPtrC<BodyT> &ptr) {
    LoadStreamImpl(strm,ptr,*ptr);
    return strm;
  }

  //: Write entity from text stream
  template<typename BodyT>
  std::ostream &operator<<(std::ostream &strm,const SmartOwnerPtrC<BodyT> &ptr) {
    SaveStreamImpl(strm,ptr,*ptr);
    return strm;
  }
  
  //: Read entity from text stream
  template<typename BodyT>
  std::istream &operator>>(std::istream &strm,SmartOwnerPtrC<BodyT> &ptr) {
    LoadStreamImpl(strm,ptr,*ptr);
    return strm;
  }

}
#endif
