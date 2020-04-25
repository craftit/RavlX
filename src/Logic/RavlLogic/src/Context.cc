// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! userlevel=Normal
//! author="Charles Galambos"
//! docentry="Ravl.Logic"
//! rcsid="$Id$"
//! lib=RavlLogic
//! file="Ravl/Logic/Base/Context.cc"

#include "Ravl/Logic/Context.hh"
#include "Ravl/Logic/And.hh"
#include "Ravl/Logic/Or.hh"
#include "Ravl/Logic/Not.hh"
#include "Ravl/PointerManager.hh"
#include "Ravl/BinStream.hh"

namespace RavlLogicN {
  
  //: Default constructor.
  
  ContextC::ContextC(const char *nname)
    : name(nname)
  {
    Register("RavlLogicN.And",literalAnd,true);
    Register("RavlLogicN.Or",literalOr,true);
    Register("RavlLogicN.Not",literalNot,true);
  }
  
  //: Register literal.
  
  bool ContextC::Register(const StringC &name,const LiteralC &lit,bool builtinLit) {
    map[name] = Tuple2C<LiteralC,bool>(lit,builtinLit);
    return true;
  }
  
  //: Send context information.
  
  bool ContextC::Send(BinOStreamC &strm) {
    if(!strm.PointerManager().IsValid())
      strm.PointerManager() = PointerManagerC(true);
    PointerManagerC mgr(strm.PointerManager());
    
    Int32T version = 0;
    UInt32T size = map.Size();
    strm << version << name << size;
    for(HashIterC<StringC,Tuple2C<LiteralC,bool> > it(map);it;it++) {
      strm << it.Key(); // Write name.
      IOPtrC objio = ObjIO(it.Data().Data1());
      UInt32T id = mgr.Insert(objio); // Register ID.
      strm << id << it.Data().Data2();
#if RAVL_CHECK
      // Mark saving complete
      mgr.Insert(id,objio);
#endif

    }
    return true;
  }
  
  //: Recieve context information.
  
  bool ContextC::Receive(BinIStreamC &strm) {
    if(!strm.PointerManager().IsValid())
      strm.PointerManager() = PointerManagerC(true);
    PointerManagerC mgr(strm.PointerManager());
    
    Int32T version ;
    UInt32T size;
    StringC otherName;
    strm >> version >> otherName >> size;
    if(version != 0)
      throw ExceptionOutOfRangeC("ContextC::Receive(BinIStreamC &), Unrecognised version number in stream. ");
    for(UIntT i = 0;i < size;i++) {
      StringC name;
      bool builtinLit;
      UInt32T id;
      strm >> name >> id >> builtinLit; // Read name and id
      Tuple2C<LiteralC,bool> info;
      if(!map.Lookup(name,info)) {
	if(builtinLit)
	  std::cerr << "WARNING: Unknown builtin literal '" << name << "' with id " << id << " in context. \n";
	info = Tuple2C<LiteralC,bool>(LiteralC(name),builtinLit);
	map[name] = info;
      }
      IOPtrC objio = ObjIO(info.Data1());
      mgr.Insert(id,objio);
    }
    return true;
  }
  
  BinIStreamC &operator>>(BinIStreamC &strm,ContextC &obj) {
    obj.Receive(strm);
    return strm;
  }
  //: Load from a binary stream.
  
  BinOStreamC &operator<<(BinOStreamC &strm,ContextC &obj) {
    obj.Send(strm);    
    return strm;    
  }
  //: Save to a binary stream.

}
