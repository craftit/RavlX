// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! author="Charles Galambos"
//! lib=RavlCore
//! file="Ravl/Core/System/FunctionRegister.cc"

#include "Ravl/FunctionRegister.hh"
#include "Ravl/Hash.hh"
#include "Ravl/DList.hh"
#include "Ravl/MTLocks.hh"
#include "Ravl/RefCounter.hh"

#include <string.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif


namespace RavlN {
  
  typedef int (RCBodyVC::*MethodPtrT)();
  //: Function ptr type.
  
  
  class FPEFuncPtrC {
  public:
    FPEFuncPtrC(const void *ndata,int nsize)
      : size(nsize),
	data((char *) ndata)
    {}
    //: Constructor.

    bool operator==(const FPEFuncPtrC &fp) const { 
      if(size != fp.size) 
	return false;
      for(int i = 0;i < size;i++)
	if(data[i] != fp.data[i])
	  return false;
      return true;
    }
    //: Compair pointers.
    
    UIntT Hash() const { 
      UIntT ret = data[0];
      for(int i = 1;i < size;i++)
	ret += data[i] ^ (data[i] << i);
      return ret;
    }
    //: Create a hash value.
  protected:
    int size;
    const char *data;
  };
  
  class FunctionPointerEntryC {
  public:
    FunctionPointerEntryC(const char *nname,void *data,int nsize,const std::type_info &nptype)
      : name(nname),
	ptype(nptype),
	size(nsize)
    { memcpy(ptrdata,(char *) data,size);}
    //: Constructor.

    const char *Name() const
    { return name; }
    //: Access name.

    bool SetPointer(void *data,const std::type_info &nptype) {
      if(nptype != ptype)
	return false;
      memcpy((char *) data,ptrdata,size);
      return true;
    }
    //: Setup pointers.
    
    FPEFuncPtrC FuncPtr() const
    { return  FPEFuncPtrC(ptrdata,size); }
    //: Access function pointer data.
    
  protected:
    const char *name;
    const std::type_info &ptype;
    char ptrdata[sizeof(MethodPtrT)]; // Method pointers may be larger than normal function pointer.
    int size;
  };
  
  
  
  int NullFunc(int v)
  { return v; };

  static RegisterFuncionC registerNullFunc("NullFunc(int)",&NullFunc);
  
  DListC<FunctionPointerEntryC> &FunctionPointerEntries() {
    static DListC<FunctionPointerEntryC> t;
    return t;
  }
  
  HashC<const char *,FunctionPointerEntryC *> &FunctionPointerName2Pointer() {
    static HashC<const char *,FunctionPointerEntryC *> t;
    return t;
  }
  
  HashC<FPEFuncPtrC,FunctionPointerEntryC *> &FunctionPointerPointer2Name() {
    static HashC<FPEFuncPtrC,FunctionPointerEntryC *> t;
    return t;
  }
  
  //! userlevel=Develop
  //: Register function pointer.
  
  bool BaseFunctionRegister(const char *name,void *ptr,int size,const std::type_info &ti) {
    ONDEBUG(std::cerr << "BaseFunctionRegister(), Called. \n");
    MTWriteLockC lock(0);
    FunctionPointerEntryC *ent;
    if(FunctionPointerName2Pointer().Lookup(name,ent)) {
      std::cerr <<"WARNING: Duplicate function pointer registeration for '" << name << "'\n";
      return false;
    }
    FunctionPointerEntries().InsLast(FunctionPointerEntryC(name,ptr,size,ti));
    ent = &FunctionPointerEntries().Last();
    FunctionPointerName2Pointer()[name] = ent;
    FunctionPointerPointer2Name()[ent->FuncPtr()] = ent;
    return true;
  }
  
  //: Register function pointer.
  
  bool BaseFunctionLookupByName(const char *name,void *ptr,int size,const std::type_info &ti) {
    ONDEBUG(std::cerr << "BaseFunctionLookupByName(), Called. \n");
    FunctionPointerEntryC *fpe;
    MTReadLockC lock(0);
    if(!FunctionPointerName2Pointer().Lookup(name,fpe))
      return false;
    return fpe->SetPointer(ptr,ti);
  }
  
  //: Lookup function by pointer.
  
  const char *BaseFunctionLookupByPointer(void *ptr,int size,const std::type_info &ti) {
    ONDEBUG(std::cerr << "BaseFunctionLookupByPointer(), Called. \n");
    FunctionPointerEntryC *fpe;
    MTReadLockC lock(0);
    if(!FunctionPointerPointer2Name().Lookup(FPEFuncPtrC(ptr,size),fpe))
      return 0;
    return fpe->Name();
  }

}
