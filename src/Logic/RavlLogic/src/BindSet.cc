// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlLogic
//! file="Ravl/Logic/Base/BindSet.cc"

#include "Ravl/Logic/BindSet.hh"
#include "Ravl/PointerManager.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x) 
#endif

namespace RavlLogicN {

  BinOStreamC &operator<<(BinOStreamC &s,const BindC &bind) { 
    ONDEBUG(std::cerr << "operator<<(BinOStreamC &,BindC &) \n");
    s << ObjIO(bind.Value());
    return s;
  }
  //! userlevel=Advanced
  //: output stream 
  
  BinIStreamC &operator>>(BinIStreamC &s,BindC &bind) {
    ONDEBUG(std::cerr << "operator>>(BinIStreamC &,BindC &) \n");
    s >> ObjIO(bind.Value());
    return s;
  }
  //! userlevel=Advanced
  //: input stream 
  
  BinOStreamC &operator<<(BinOStreamC &out,const HashElemC<LiteralC,BindC> &obj) {  
    ONDEBUG(std::cerr << "operator<<(BinOStreamC &,const HashElemC<LiteralC,BindC> &) \n");
    out << ObjIO(obj.GetKey()) << ObjIO(obj.Data().Value()); 
    return out;
  }
  
  BinIStreamC &operator>>(BinIStreamC &in,HashElemC<LiteralC,BindC> &obj) { 
    ONDEBUG(std::cerr << "operator>>(BinIStreamC &,HashElemC<LiteralC,BindC> &) \n");
    in >> ObjIO(obj.GetKey()) >> ObjIO(obj.Data().Value()); 
    return in;
  }
  
  //: Construct from a binary stream.
  
  BindSetBodyC::BindSetBodyC(std::istream &strm) 
    : top(0)
  { RavlAssert(0);  }
  
  //: Construct from a binary stream.
  
  BindSetBodyC::BindSetBodyC(BinIStreamC &strm) 
    : top(0)
  {
    IntT version;
    strm >> version;
    if(version != 0)
      throw ExceptionOutOfRangeC("Unexpected version number in binary stream");
    UIntT size;
    strm >> size;
    //cerr << "Size=" << size << "\n";
    table = SArray1dC<IntrDListC<HashElemC<LiteralC,BindC> > >(NextPrime(size));
    BindMarkT *lastPtr = &top;
    for(UIntT i = 0;i < size;i++) {
      // Load entry.
      BindMarkT tmp = 0;
      strm >> ObjIO(tmp);
      *lastPtr = tmp;
      lastPtr = &(tmp->Data().Next());
      // Put object into hash table.
      const UIntT hashVal = StdHash(tmp->GetKey());
      tmp->HashVal(hashVal);
      const UIntT ind = hashVal % table.Size();
      table[ind].InsFirst(*tmp);
    }
    *lastPtr = 0;
  }
  
  //: Save to binary stream 'out'.
  
  bool BindSetBodyC::Save(std::ostream &out) const {
    RavlAssert(0);
    return true;
  }
  
  //: Save to binary stream 'out'.
  
  bool BindSetBodyC::Save(BinOStreamC &out) const {
    IntT version = 0;
    out << version << Size();
    for(BindMarkT at = top;at != 0;at = at->Data().Next())
      out << ObjIO(at);
    return true;
  }
  
  //: Copy constructor.
  
  BindSetBodyC::BindSetBodyC(const BindSetC &oth) 
    : HashC<LiteralC,BindC>(NextPrime(oth.Size()))
  {
    BindMarkT place = oth.Mark();
    if(place == 0)
      return ;
    // Take care of the top element.
    UIntT hashVal = place->GetKey().Hash();
    BindMarkT elem = new HashElem(place->GetKey(),hashVal,BindC(place->Data().Value(),0));
    table[hashVal % table.Size()].InsFirst(*elem);
    top = elem;
    BindMarkT lastelem = elem;
    place = place->Data().Next();
    // Take care of the rest.
    for(;place != 0;place = place->Data().Next()) {
      hashVal = place->GetKey().Hash();
      elem = new HashElem(place->GetKey(),hashVal,BindC(place->Data().Value(),0));
      table[hashVal % table.Size()].InsFirst(*elem);
      lastelem->Data().Next() = elem;
      lastelem = elem;
    }
    elements = oth.Size();
  }

  //: Construct from a table of mappings.
  
  BindSetBodyC::BindSetBodyC(const HashC<LiteralC,LiteralC> &tab) {
    for(HashIterC<LiteralC,LiteralC> it(tab);it;it++)
      Bind(it.Key(),it.Data());
  }
  
  //: Attempty to bind a value to var.
  
  bool BindSetBodyC::Bind(const LiteralC &var,const LiteralC &val) {
    RavlAssert(var.IsValid());
    RavlAssert(var.IsVariable());

    SizeT hashVal;
    BindMarkT elem = LookupHV(var,hashVal);
    if(elem != 0)
      return false; // Already bound.
    CheckAdd();
    elem = new HashElem(var,hashVal,BindC(val,top));
    table[hashVal % table.Size()].InsFirst(*elem);
    top = elem;
    return true;
  }

  //: Undo bindings to marked place.
  
  void BindSetBodyC::Undo(BindMarkT bm) {
    ONDEBUG(std::cerr << "Undo to :" << ((void *) bm) << "\n");
    while(top != bm) {
      ONDEBUG(std::cerr << "top :" << ((void *) top) << "\n");
      RavlAssert(top != 0);
      BindMarkT nxt = top->Data().Next();
      Del(top,true);
      top = nxt;
    }
  }
  
  
  //: Undo bindings done after and including var.
  
  void BindSetBodyC::Undo(LiteralC var) {
    ONDEBUG(std::cerr << "Undo to :" << ((void *) bm) << "\n");
    while(top->GetKey() != var) {
      ONDEBUG(std::cerr << "top :" << ((void *) top) << "\n");
      RavlAssert(top != 0);
      BindMarkT nxt = top->Data().Next();
      Del(top,true);
      top = nxt;
    }
    RavlAssert(top != 0);    
    // Do var as well.
    BindMarkT nxt = top->Data().Next();
    Del(top,true);
    top = nxt;
  }
  
  //: Remove all bindings from set.
  
  void BindSetBodyC::Empty() {
    HashC<LiteralC,BindC>::Empty();
    top = 0;
  }

  //: Bind set as string.
  
  StringC BindSetBodyC::Name() const {
    StringC ret;
    HashIterC<LiteralC,BindC> it(const_cast<HashC<LiteralC,BindC> &>((HashC<LiteralC,BindC> &)*this));
    for(;it;it++)
      ret += StringC("(") + it.Key().Name() + "->" + it.Data().Value().Name() + ")";
    return ret;
  }
  
  std::ostream &operator<<(std::ostream &s,const BindSetC &binds) {
    if(!binds.IsValid()) {
      s << "(NIL)";
      return s;
    }
    
    HashIterC<LiteralC,BindC> it(const_cast<BindSetC &>(binds).Iter());
    for(;it;it++) {
      s << '(' << it.Key().Name() << "->" << it.Data().Value().Name() << ')';
    }
    return s;
  }
  //: output stream 
  
  std::istream &operator>>(std::istream &s,BindSetC &binds) {
    RavlAssertMsg(0,"operator<<(std::istream &s,BindSetC &binds), Not implemented. ");
    return s;
  }
  //: input stream 
  
}

