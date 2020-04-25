// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FUNCTIONREGISTER_HEADER
#define RAVL_FUNCTIONREGISTER_HEADER 1
//////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.IO.Function Register"
//! lib=RavlCore
//! file="Ravl/Core/System/FunctionRegister.hh"

#include "Ravl/Types.hh"
#include "Ravl/BiHash.hh"
#include "Ravl/FunctionRegister.hh"
#include "Ravl/Stream.hh"
#include "Ravl/BinStream.hh"

#if RAVL_HAVE_ANSICPPHEADERS
#include <typeinfo>
#else
#include <typeinfo.h>
#endif

namespace RavlN {

  class BinIStreamC;
  class BinOStreamC;
  
  bool BaseFunctionRegister(const char *name,void *ptr,int size,const std::type_info &ti);
  //! userlevel=Develop
  //: Register function pointer.
  
  bool BaseFunctionLookupByName(const char *name,void *ptr,int size,const std::type_info &ti);
  //: Register function pointer.

  const char *BaseFunctionLookupByPointer(void *ptr,int size,const std::type_info &ti);
  //: Lookup function by pointer.
  
  template<class DataT>
  bool RegisterFunction(const char *name,DataT funcPtr)
  { return BaseFunctionRegister(name,(void *) &funcPtr,sizeof(funcPtr),typeid(DataT)); }
  //! userlevel=Normal
  //: Register function ptr.
  
  template<class DataT>
  bool LookupFunctionByName(const char *name,DataT &funcPtr)
  { return BaseFunctionLookupByName(name,(void *) &funcPtr,sizeof(funcPtr),typeid(DataT)); }
  //: Lookup function pointer by name.
  
  template<class DataT>
  const char *LookupFunctionByPointer(DataT &funcPtr)
  { return BaseFunctionLookupByPointer((void *) &funcPtr,sizeof(funcPtr),typeid(DataT)); }
  //: Lookup function name by pointer.

  template<class DataT>
  bool LoadFunctionPointer(BinIStreamC &strm,DataT &funcPtr) { 
    StringC fnName;
    strm >> fnName;
    if(!LookupFunctionByName(fnName,funcPtr)) {
      std::cerr << "Failed to find function '" << fnName << "\n";
      throw ExceptionOperationFailedC("Failed to find function pointer.");
    }
    return true;
  }
  //: Load function pointer.
  
  template<class DataT>
  bool LoadFunctionPointer(std::istream &strm,DataT &funcPtr) { 
    StringC fnName;
    strm >> fnName;
    if(!LookupFunctionByName(fnName,funcPtr)) {
      std::cerr << "Failed to find function '" << fnName << "\n";
      throw ExceptionOperationFailedC("Failed to find function pointer.");
    }
    return true;
  }
  //: Load function pointer.

  template<class DataT>
  bool SaveFunctionPointer(std::ostream &strm,DataT &funcPtr) { 
    const char *fnName = LookupFunctionByPointer(funcPtr);
    if(fnName == 0) {
      std::cerr << "Internal error: Asked to save unregistered function. \n";
      throw ExceptionOperationFailedC("Failed to find function name.");
    }
    strm << fnName << ' ';
    return true;
  }
  //: Save function pointer.
  
  template<class DataT>
  bool SaveFunctionPointer(BinOStreamC &strm,const DataT &funcPtr) { 
    const char *fnName = LookupFunctionByPointer(funcPtr);
    if(fnName == 0) {
      std::cerr << "Internal error: Asked to save unregistered function. \n";
      throw ExceptionOperationFailedC("Failed to find function name.");
    }
    strm << fnName;
    return true;
  }
  //: Save function pointer.
  
  //! userlevel=Normal
  //: Register function pointer.
  // Class to make it easy to register functions. Use as global
  // variables. <p>
  // e.g. for class xyzC declare the following as global
  // in a .cc file preferably in the same file as the definition of XyzC <p>
  //  static TypeNameC typeNameXyz(typeid(XyzC),"XyzC");
  
  class RegisterFuncionC {
  public:
    template<class DataT>
    RegisterFuncionC(const char *newname,DataT func) 
    { RegisterFunction(newname,func); }
    //: Constructor.
    
  };

  
}

#endif
