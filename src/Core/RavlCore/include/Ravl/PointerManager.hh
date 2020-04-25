// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_POINTERMANGER_HEADER
#define RAVL_POINTERMANGER_HEADER 1
//! author="Charles Galambos"
//! lib=RavlCore
//! docentry="Ravl.API.Core.IO.Streams"
//! userlevel=Normal
//! example=exObjIO.cc
//! file="Ravl/Core/System/PointerManager.hh"

#include "Ravl/RefCounter.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/Hash.hh"
#include "Ravl/RCHandleV.hh"
#include "Ravl/SmartPtr.hh"
#include "Ravl/TypeName.hh"
#include <vector>

namespace RavlN {
  
  class IOPtrC;
  class StoredPointerC;
  class BinOStreamC;
  class BinIStreamC;  
  template<typename DataT> class SArray1dC;
  template<typename DataT> class CollectionC;
  
  //! userlevel=Develop
  //: Actions to handle pointers.
  
  class IOPtrActionC {
  public:
    IOPtrActionC()
    {}
    //: Default constructor.

    virtual ~IOPtrActionC();
    //: Destructor.
    
    virtual void Assign(IOPtrC &ptr,StoredPointerC &obj) const = 0;
    //: Assign an object to the pointer.
    
    virtual void Save(const IOPtrC &ptr,BinOStreamC &ostrm) const = 0;
    //: Save the body of the object to a binary stream
    
    virtual void Load(IOPtrC &ptr,BinIStreamC &istrm) const = 0;
    //: Load the body of the object to a binary stream

    virtual void Save(const IOPtrC &ptr,OStreamC &ostrm) const = 0;
    //: Save the body of the object to a binary stream
    
    virtual void Load(IOPtrC &ptr,IStreamC &istrm) const = 0;
    //: Load the body of the object to a binary stream
    
    virtual void Store(StoredPointerC &store,const IOPtrC &ptr) const = 0;
    //: Store pointer for use later.
    
    virtual void Free(void *data) const = 0;
    //: Free stored pointer
    
    virtual const void *Key(const void *ptr) const = 0;
    //: Get key for lookup.

    virtual const std::type_info &TypeInfo() const = 0;
    //: Get type of pointer
  };
  
  //! userlevel=Develop
  //: Managed pointer.
  // This is a temporary object used to mediate saving to and from streams.
  
  class IOPtrC {
  public:
    IOPtrC()
      : ptr(0),
	      actions(0)
    {}
    //: Default constructor.
    
    IOPtrC(void *data,const IOPtrActionC &nActions)
      : ptr(data),
	      actions(&nActions)
    {}
    //: Constructor.
    
    bool IsValid() const
    { return actions != 0; }
    //: Test if IOPtr is valid.

    void Assign(StoredPointerC &data)
    { actions->Assign(*this,data); }
    //: Assign value to the pointer.
    
    void Save(BinOStreamC &os);
    //: Save object to a binary stream.
    
    void Load(BinIStreamC &is);
    //: Load object from a binary stream.
    
    void Save(OStreamC &os);
    //: Save object to a stream.
    
    void Load(IStreamC &is);
    //: Load object from a stream.
    
    void *Pointer()
    { return ptr; }
    //: Access pointer.

    const void *Pointer() const
    { return ptr; }
    //: Access pointer.

    void Pointer(void *nptr)
    { ptr = nptr; }
    //: Access pointer.
    
    bool operator==(const IOPtrC &oth) const
    { return ptr == oth.ptr; }
    //: Are these handles to the same object ?
    
    size_t Hash() const
    { return StdHash((const void *) ptr); }
    //: Create a hash value for the pointer.
    
    //IOPtrActionC *Actions()
    //{ return actions; }
    //: Access action class.

    const IOPtrActionC *Actions() const
    { return actions; }
    //: Access action class.

    const void *Key() const
    { return actions->Key(ptr); }
    //: Get key for index.
    
  protected:
    void *ptr;
    const IOPtrActionC *actions;
  };

  //! userlevel=Develop
  //: Stored reference to an object.
  
  class StoredPointerC {
  public:
    StoredPointerC()
      : obj(0),
	      actions(0),
	      m_complete(false)
    {}
    //: Default constructor.
    
    StoredPointerC(const StoredPointerC &oth)
      : obj(0),
	      actions(0),
        m_complete(false)
    { RavlAssert(oth.obj == 0); }
    //: Copy constructor.
    // Can't copy populated objects.
    
    ~StoredPointerC();
    //: Destructor.
    
    void *Pointer()
    { return obj; }
    //: Access pointer.

    const void *Pointer() const
    { return obj; }
    //: Access pointer.

    void Pointer(void *nptr)
    { obj = nptr; }
    //: Set pointer.

    void Pointer(const void *nptr)
    { obj = const_cast<void *>(nptr); }
    //: Set pointer.

#if 0    
    const StoredPointerC &operator=(const IOPtrC &mp) {
      IOPtrC &lmp = const_cast<IOPtrC &>(mp);
      obj = *((void **)lmp.Pointer()); // Get address of object.
      actions = lmp.Actions();
      return *this;
    }
    // Assign from a managed pointer.
#endif

    void Actions(const IOPtrActionC *nptr)
    { actions = nptr; }
    //: Access pointer.
    
    const IOPtrActionC *Actions() const
    { return actions; }
  protected:
    void *obj;
    const IOPtrActionC *actions;
    bool m_complete;
  };
  
  //! userlevel=Develop
  //: Pointer manager for binary streams.
  
  class PointerManagerBodyC
    : public RCBodyVC
  {
  public:
    PointerManagerBodyC(bool forSave);
    //: Constructor.
    
    virtual ~PointerManagerBodyC();
    //: Destructor.
    
    bool Lookup(const IOPtrC &obj,SizeT &id) const
    { return ptr2id.Lookup(obj.Key(),id); }
    //: Test if an object has already been saved/loaded.
    // If it has then Lookup will return true and id will
    // be set to the id of the object.

    SizeT Insert(const IOPtrC &obj);
    //: Store the address of an object.
    // Returns the id for the ptr.
    
    StoredPointerC *Lookup(SizeT id)
    { return id2ptr.Lookup(id); }
    //: Lookup object by id.

    StoredPointerC *Lookup(const IOPtrC &obj);
    //: Test if an object has already been saved/loaded.

    void Insert(SizeT id,const IOPtrC &obj) {
      //RavlAssert(id == idAlloc); // Check sequence.
      //idAlloc++;
      obj.Actions()->Store(id2ptr[id],obj);
    }
    //: Store by id.
    
    UIntT IdAlloc() const
    { return idAlloc; }
    //: Access current alloc id.
  protected:
    SizeT idAlloc;
    HashC<SizeT,StoredPointerC> id2ptr;
    HashC<const void *,SizeT> ptr2id;
  };
  
  //! userlevel=Advanced
  //: Pointer manager for binary streams.
  // This class manages the saving of structures to prevent
  // duplication of common structures when saving complex objects.
  // i.e. If two classes which have a handle to a common object 
  // (such as a list) are saved to the stream, the list will only
  // be saved once.
  
  class PointerManagerC
    : public RCHandleC<RCBodyVC>
  {
  public:
    PointerManagerC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    PointerManagerC(bool forSave)
      : RCHandleC<RCBodyVC>(*new PointerManagerBodyC(forSave))
    {}
    //: Constructor.

    PointerManagerC(RCHandleC<RCBodyVC> &oth)
      : RCHandleC<RCBodyVC>(oth)
    {}
    //: Constructor.
    
  protected:
    PointerManagerC(PointerManagerBodyC &bod)
      : RCHandleC<RCBodyVC>(bod)
    {}
    //: Body constructor.
    
    PointerManagerBodyC& Body()
    { return static_cast<PointerManagerBodyC &>(RCHandleC<RCBodyVC>::Body()); }
    //: Body access.
    
    const PointerManagerBodyC& Body() const
    { return static_cast<const PointerManagerBodyC &>(RCHandleC<RCBodyVC>::Body()); }
    //: Body access.
    
  public:
    bool Lookup(const IOPtrC &obj,SizeT &id)
    { return Body().Lookup(obj,id); }
    //: Test if an object has already been saved/loaded.
    // If it has then Lookup will return true and id will
    // be set to the id of the object.
    
    StoredPointerC *Lookup(SizeT id)
    { return Body().Lookup(id); }
    //: Lookup object by id.

    StoredPointerC *Lookup(IOPtrC &obj)
    { return Body().Lookup(obj); }
    //: Lookup object by id.

    SizeT Insert(const IOPtrC &obj)
    { return Body().Insert(obj); }
    //: Store the address of an object.
    // returns a unique id for that object.

    void Insert(SizeT id,const IOPtrC &obj)
    { Body().Insert(id,obj); }
    //: Store by id.
    
    SizeT IdAlloc() const
    { return Body().IdAlloc(); }
    //: Access current alloc id.
  };
  
  //! userlevel=Develop
  //: Actions to handle simple pointers.
  // This assumes the pointer held in 'IOPtrC' points
  // to a pointer to the data.
  
  template<class DataT>
  class IOPtrActionSimpleC 
    : public IOPtrActionC
  {
  public:
    IOPtrActionSimpleC()
    {}
    //: Default constructor.
    
    virtual void Assign(IOPtrC &ptr,StoredPointerC &obj) const { 
      RavlAssert(obj.Actions()->TypeInfo() == typeid(DataT));
      RavlAssert(obj.Pointer() != 0);
      *static_cast<DataT **>(ptr.Pointer()) = static_cast<DataT *>(obj.Pointer()); 
    }
    //: Assign an object to the pointer.
    
    virtual void Save(const IOPtrC &ptr,BinOStreamC &ostrm) const
    { ostrm << (**static_cast<const DataT * const *>(ptr.Pointer())); }
    //: Save the body of the object to a binary stream
    
    virtual void Load(IOPtrC &ptr,BinIStreamC &istrm) const { 
      DataT *obj = new DataT();
      istrm >> *obj;
      *static_cast<DataT **>(ptr.Pointer()) = obj;
    }
    //: Load the body of the object from a binary stream

    virtual void Save(const IOPtrC &ptr,OStreamC &ostrm) const
    { ostrm << (**static_cast<const DataT * const *>(ptr.Pointer())); }
    //: Save the body of the object to a binary stream
    
    virtual void Load(IOPtrC &ptr,IStreamC &istrm) const { 
      DataT *obj = new DataT();
      istrm >> *obj;
      *static_cast<DataT **>(ptr.Pointer()) = obj;
    }
    //: Load the body of the object from a binary stream
    
    virtual void Store(StoredPointerC &store,const IOPtrC &ptr) const {
      store.Pointer(*((void **)ptr.Pointer())); // Get address of object.
      store.Actions(ptr.Actions());
    }
    //: Store pointer for use later.
    
    virtual void Free(void *) const
    {}
    //: Free stored pointer
    
    virtual const void *Key(const void *ptr) const
    { return *((void **)ptr); }
    //: Get key for lookup.

    virtual const std::type_info &TypeInfo() const
    { return typeid(DataT); }
    //: Get type of pointer

  };

  //! userlevel=Develop
  //: Actions from reference counted handles
  
  template<class DataT>
  class IOPtrActionHandleC 
    : public IOPtrActionC
  {
  public:
    IOPtrActionHandleC()
    {}
    //: Default constructor.
    
    virtual void Assign(IOPtrC &ptr,StoredPointerC &obj) const
    {
#if 0
      if(obj.Actions()->TypeInfo() != typeid(DataT)) {
        std::cerr << "Type mismatch loading pointer: " << TypeName(obj.Actions()->TypeInfo()) << " and " << TypeName(typeid(DataT)) << "\n";
        RavlAlwaysAssert(0);
      }
#endif
      (*static_cast<DataT *>(ptr.Pointer())) = (*static_cast<DataT *>(obj.Pointer()));
    }
    //: Assign an object to the pointer.
    
    virtual void Save(const IOPtrC &ptr,BinOStreamC &ostrm) const
    { ostrm << (*static_cast<const DataT *>(ptr.Pointer())); }
    //: Save the body of the object to a binary stream
    
    virtual void Load(IOPtrC &ptr,BinIStreamC &istrm) const 
    { istrm >> (*static_cast<DataT *>(ptr.Pointer())); }
    //: Load the body of the object from a binary stream

    virtual void Save(const IOPtrC &ptr,OStreamC &ostrm) const
    { ostrm << (*static_cast<const DataT *>(ptr.Pointer())); }
    //: Save the body of the object to a stream
    
    virtual void Load(IOPtrC &ptr,IStreamC &istrm) const 
    { istrm >> (*static_cast<DataT *>(ptr.Pointer())); }
    //: Load the body of the object from a stream
    
    virtual void Store(StoredPointerC &store,const IOPtrC &ptr) const {
      RavlAssert(ptr.Pointer() != 0);
      const DataT *newobj = new DataT(*static_cast<const DataT *>(ptr.Pointer()));
      store.Pointer(static_cast<const void *>(newobj));
      store.Actions(ptr.Actions());      
    }
    //: Store pointer for use later.
    
    virtual void Free(void *data) const { 
      if(data == 0) return ;
      delete static_cast<DataT *>(data);
    }
    //: Free stored pointer
    
    virtual const void *Key(const void *ptr) const
    { return static_cast<const DataT *>(ptr)->VoidPtr(); }
    //: Get key for lookup.

    virtual const std::type_info &TypeInfo() const
    { return typeid(DataT); }
    //: Get type of pointer

  };

  //! userlevel=Normal
  
  template<class DataT>
  inline IOPtrC ObjIO(DataT *&ptr) { 
    static IOPtrActionSimpleC<DataT> action;
    return IOPtrC((void *) &ptr,action); 
  }
  //: Handle a pointer.

  template<class DataT>
  inline IOPtrC ObjIO(const DataT *&ptr) { 
    static IOPtrActionSimpleC<DataT> action;
    return IOPtrC((void *) &ptr,action); 
  }
  //: Handle a pointer.

  template<class DataT>
  inline IOPtrC ObjIO(const DataT * const & ptr) {
    static IOPtrActionSimpleC<DataT> action;
    return IOPtrC((void *) &ptr,action);
  }
  //: Handle a pointer.

  template<class DataT>
  inline IOPtrC ObjIO(DataT * const & ptr) {
    static IOPtrActionSimpleC<DataT> action;
    return IOPtrC((void *) &ptr,action);
  }
  //: Handle a pointer.

  template<class DataT>
  inline IOPtrC ObjIO(DataT &ptr) { 
    static IOPtrActionHandleC<DataT> action;
    return IOPtrC((void *) &ptr,action); 
  }
  //: Handle a reference counted object.

  template<class DataT>
  inline IOPtrC ObjIO(const DataT &ptr) { 
    static IOPtrActionHandleC<DataT> action;
    return IOPtrC((void *) &ptr,action); 
  }
  //: Handle a reference counted object.

  template<class DataT>
  inline IOPtrC ObjIO(SmartPtrC<DataT> &ptr) {
    static IOPtrActionHandleC<SmartPtrC<DataT> > action;
    return IOPtrC((void *) &ptr,action);
  }
  //: Handle a reference counted object.

  template<class DataT>
  inline IOPtrC ObjIO(const SmartPtrC<DataT> &ptr) {
    static IOPtrActionHandleC<SmartPtrC<DataT> > action;
    return IOPtrC((void *) &ptr,action);
  }
  //: Handle a reference counted object.

  template<class DataT>
  inline IOPtrC ObjIO(SmartPtrC<const DataT> &ptr) {
    static IOPtrActionHandleC<SmartPtrC<DataT> > action;
    return IOPtrC((void *) &ptr,action);
  }
  //: Handle a reference counted object.

  template<class DataT>
  inline IOPtrC ObjIO(const SmartPtrC<const DataT> &ptr) {
    static IOPtrActionHandleC<SmartPtrC<DataT> > action;
    return IOPtrC((void *) &ptr,action);
  }
  //: Handle a reference counted object.

  BinOStreamC &operator<<(BinOStreamC &strm,IOPtrC obj);
  //: Save managed pointer to a binary stream.
  
  BinIStreamC &operator>>(BinIStreamC &strm,IOPtrC obj);
  //: Load managed pointer from a binary stream.
  
  template<class DataT>
  BinOStreamC &WriteSArrayIOPtr(BinOStreamC &strm,const SArray1dC<DataT> &arr) {
    strm << arr.Size();
    for(unsigned i = 0;i < arr.Size();i++)
      strm << ObjIO(arr[i]);
    return strm;
  }

  template<class DataT>
  BinIStreamC &ReadSArrayIOPtr(BinIStreamC &strm,SArray1dC<DataT> &arr) {
    SizeC size;
    strm >> size;
    arr = SArray1dC<DataT>(size);
    for(unsigned i = 0;i < size;i++)
      strm >> ObjIO(arr[i]);
    return strm;
  }

  template<class DataT>
  BinOStreamC &WriteCollectionIOPtr(BinOStreamC &strm,const CollectionC<DataT> &arr) {
    if(!arr.IsValid()) {
      SizeT size = 0;
      strm << size;
      return strm;
    }
    strm << arr.Size();
    for(unsigned i = 0;i < arr.Size();i++)
      strm << ObjIO(arr[i]);
    return strm;
  }

  template<class DataT>
  BinIStreamC &ReadCollectionIOPtr(BinIStreamC &strm,CollectionC<DataT> &arr) {
    SizeC size;
    strm >> size;
    arr = CollectionC<DataT>(size,size);
    for(unsigned i = 0;i < size;i++)
      strm >> ObjIO(arr[i]);
    return strm;
  }

  template<class DataT>
  BinOStreamC &WriteStdVectorIOPtr(BinOStreamC &strm,const std::vector<DataT> &arr) {
    strm << arr.size();
    for(unsigned i = 0;i < arr.size();i++)
      strm << ObjIO(arr[i]);
    return strm;
  }

  template<class DataT>
  BinIStreamC &ReadStdVectorIOPtr(BinIStreamC &strm,std::vector<DataT> &arr) {
    SizeC size;
    strm >> size;
    arr.resize(size.V());
    for(unsigned i = 0;i < size;i++)
      strm >> ObjIO(arr[i]);
    return strm;
  }
  
}
#endif
