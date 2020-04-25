// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/System/PointerManager.cc"

#include "Ravl/PointerManager.hh"
#include "Ravl/SysLog.hh"
#include "Ravl/Exception.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x) 
#endif

namespace RavlN {

  //: Destructor.

  IOPtrActionC::~IOPtrActionC()
  {}

  //:----------------------------------------------------

  void IOPtrC::Save(BinOStreamC &os)
  { actions->Save(*this,os); }
  //: Save object to a binary stream.

  void IOPtrC::Load(BinIStreamC &is)
  { actions->Load(*this,is); }
  //: Load object from a binary stream.

  void IOPtrC::Save(OStreamC &os)
  { actions->Save(*this,os); }
  //: Save object to a stream.

  void IOPtrC::Load(IStreamC &is)
  { actions->Load(*this,is); }
  //: Load object from a stream.

  //:----------------------------------------------------
  
  //: Destructor.
  
  StoredPointerC::~StoredPointerC() {
    ONDEBUG(std::cerr << "StoredPointerC::~StoredPointerC(), Called. " << (void *) actions << " " << (void *) obj << "\n");
    if(actions != 0 && obj != 0)
      actions->Free(obj);
  }
  
  //:----------------------------------------------------
  
  //: Default constructor.
  
  PointerManagerBodyC::PointerManagerBodyC(bool forSave) 
    : idAlloc(1)
  {}
  
  //: Destructor.
  
  PointerManagerBodyC::~PointerManagerBodyC() 
  { ONDEBUG(std::cerr << "PointerManagerBodyC::~PointerManagerBodyC(), Called. \n"); }
  

  //: Test if an object has already been saved/loaded.

  StoredPointerC *PointerManagerBodyC::Lookup(const IOPtrC &obj) {
    SizeT id;
    if(!Lookup(obj,id))
      return 0;
    return id2ptr.Lookup(id);
  }

  //: Store the address of an object.
  // Returns the id for the ptr.

  SizeT PointerManagerBodyC::Insert(const IOPtrC &obj) {
    SizeT id = idAlloc++;
    ONDEBUG(std::cerr << "Storing id " << id << " \n");
    ptr2id[obj.Key()] = id;
    return id;
  }



  //:----------------------------------------------------
  
  BinOStreamC &operator<<(BinOStreamC &strm,IOPtrC obj) {
    if(!strm.PointerManager().IsValid())
      strm.PointerManager() = PointerManagerC(true);
    PointerManagerC mgr(strm.PointerManager());
    SizeT id;
    ONDEBUG(std::cerr << "BinOStreamC<<(strm,IOPtrC) " << *((void **)obj.Pointer()) << "\n");

    if(mgr.Lookup(obj,id)) { // Seen this object already ?
      ONDEBUG(std::cerr << "Storing id " << id << " from " << *((void **)obj.Pointer()) << "\n");
#if RAVL_CHECK
      // Check for cycles, had save been completed?
      if(mgr.Lookup(id) == 0) {
        RavlError("Cycle detected in pointers being saved, saving this structure is unsupported. ");
        RavlAssertMsg(0,"Cycle in pointers");
        throw RavlN::ExceptionOperationFailedC("Cycle in pointers");
      }
#endif
      strm << id;
    } else {
      // Not seen before, save id then object.
      id = mgr.Insert(obj);
      ONDEBUG(std::cerr << "Started storing data with id " << id << " \n");
      strm << id;
      obj.Save(strm);
      ONDEBUG(std::cerr << "Finished storing data with id " << id << " \n");
#if RAVL_CHECK
      // Mark saving complete
      mgr.Insert(id,obj);
#endif
    }
    return strm;
  }
  
  //: Load managed pointer from a stream.
  
  BinIStreamC &operator>>(BinIStreamC &strm,IOPtrC obj) {
    if(!strm.PointerManager().IsValid())
      strm.PointerManager() = PointerManagerC(false);
    PointerManagerC mgr(strm.PointerManager());
    SizeT id;
    strm >> id;
    StoredPointerC *ptr = mgr.Lookup(id);
    if(ptr != 0) { // Seen this object already ?
      ONDEBUG(std::cerr << "Loading id " << id << " \n");
      obj.Assign(*ptr);
    } else {
      // Not seen before, save id then object.
      ONDEBUG(std::cerr << "Loading data with id " << id << "\n");
      obj.Load(strm);
      mgr.Insert(id,obj);
    }
    return strm;
  }


}
