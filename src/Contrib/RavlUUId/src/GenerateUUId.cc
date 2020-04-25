// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2007, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlUUId

#include "Ravl/MTLocks.hh"
#include "Ravl/UUId.hh"
#include "Ravl/GenerateUUId.hh"

#if RAVL_COMPILER_VISUALCPP
  #include <Rpc.h>
  #include <comutil.h>
  #include <atlbase.h>
#else
  #include <uuid/uuid.h>
#endif

namespace RavlN {
  
  //: Generate a binary UUId;

  void GenerateUUId(UUIdC &anId) {
#if RAVL_COMPILER_VISUALCPP
    anId = UUIdC(GenerateUUId().data());
#else
    uuid_generate(anId.Raw());
#endif
  }

  //: Generate a UUId in string form.
    
  StringC GenerateUUId() 
  {    
    // Not sure if these routines are thread safe, so lock to be sure.
    
    MTWriteLockC lock(4);
    StringC buffer;

#if RAVL_COMPILER_VISUALCPP    

    char* emptyString = "";
    GUID newGUID = GUID_NULL;
    if(::CoCreateGuid(&newGUID) == S_OK)
    {
      const int maxChar = 256;
      OLECHAR szGuid[maxChar]={0};
      ::StringFromGUID2(newGUID, szGuid, maxChar);
      CComBSTR strComGUID = szGuid;
   
      _bstr_t bs((BSTR)strComGUID);
      
      char* strBuff = ((char*)bs == NULL ? emptyString : (char*)bs);
      buffer = StringC(strBuff);
    }
    else
    {
      buffer = "";
    }
    

#else

    uuid_t uuid;
    uuid_generate(uuid);
    
    char strGUID[256];
    uuid_unparse(uuid,strGUID);
    buffer = StringC(strGUID);
#endif

    return buffer;
  }
  
}
