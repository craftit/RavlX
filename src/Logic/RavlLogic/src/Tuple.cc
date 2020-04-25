// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlLogic
//! file="Ravl/Logic/Base/Tuple.cc"

#include "Ravl/Logic/Tuple.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/Logic/BindSet.hh"
#include "Ravl/Logic/LiteralIter.hh"
#include "Ravl/PointerManager.hh"
#include "Ravl/VirtualConstructor.hh"

#define DODEBUG 0
#if DODEBUG 
#define ONDEBUG(x) x
#else
#define ONDEBUG(x) 
#endif

namespace RavlLogicN {

  //: Construct from a binary stream.
  
  TupleBodyC::TupleBodyC(std::istream &strm) 
    : LiteralBodyC(strm)
  {
    UIntT arity;
    strm >> arity;
    args = SArray1dC<LiteralC>(arity);
    for(SArray1dIterC<LiteralC> it(Args());it;it++)
      strm >> *it;        
  }
  
  //: Construct from a binary stream.
  
  TupleBodyC::TupleBodyC(BinIStreamC &strm)
    : LiteralBodyC(strm)
  {
    UInt32T arity;
    strm >> arity;
    args = SArray1dC<LiteralC>(arity);
    for(SArray1dIterC<LiteralC> it(Args());it;it++)
      strm >> ObjIO(*it);    
  }
  
  //: Save to binary stream 'out'.
  
  bool TupleBodyC::Save(std::ostream &out) const {
    if(!LiteralBodyC::Save(out))
      return false;
    out << Arity() << ' ';
    for(SArray1dIterC<LiteralC> it(Args());it;it++)
      out << *it << ' ';
    return true;
  }
  
  //: Save to binary stream 'out'.
  
  bool TupleBodyC::Save(BinOStreamC &out) const {
    if(!LiteralBodyC::Save(out))
      return false;
    out << ((UInt32T) Arity());
    for(SArray1dIterC<LiteralC> it(Args());it;it++)
      out << ObjIO(*it);
    return true;
  }
  
  //: Is this a simple expression with no variables ?
  
  bool TupleBodyC::IsGrounded() const {
    for(SArray1dIterC<LiteralC> it(Args());it;it++)
      if(!it->IsGrounded())
        return false;
    return true;
  }
  
  //: Is this literal a tuple ?
  
  bool TupleBodyC::IsTuple() const 
  { return true; }
  
  //: Unify with another variable.
  
  bool TupleBodyC::Unify(const LiteralC &oth,BindSetC &bs) const {
    TupleC x(oth);
    if(!x.IsValid())
      return false; // Can only unify expr with expr's.
    if(x.Arity() != Arity())
      return false; // Can only unify if equal sizes.
    BindMarkT bm = bs.Mark();
    for(SArray1dIter2C<LiteralC,LiteralC> it(Args(),x.Args());it;it++) {
      RavlAssert(it.Data1().IsValid());
      RavlAssert(it.Data2().IsValid());
      if(!it.Data1().Unify(it.Data2(),bs)) {
	bs.Undo(bm);
	return false;
      }
    }
    return true;
  }

  //: Get a set of all sub literals.
  
  void TupleBodyC::SubLiterals(HSetC<LiteralC> &ret) const {
    if(!ret.Insert(TupleC(const_cast<TupleBodyC &>(*this))))
      return ; // Already added this.
    for(SArray1dIterC<LiteralC> it(Args());it;it++)
      it->SubLiterals(ret);
  }

  //: Return iterator through possibile matches to this literal in 'state', if any.
  
  LiteralIterC TupleBodyC::Solutions(const StateC &state,BindSetC &binds) const {
    return LiteralBodyC::Solutions(state,binds);
  }

  //: Substitute variables in 'binds' for their bound values.
  // This builds a new literal with the substute values (if there
  // are any). The new value is assigned to 'result' <p>
  // Returns true if at least one substitution has been made,
  // false if none.
  
  bool TupleBodyC::Substitute(const BindSetC &binds,LiteralC &result) const {
    SArray1dC<LiteralC> newTup(Args().Size());
    bool ret = false;
    for(SArray1dIter2C<LiteralC,LiteralC> it(Args(),newTup);it;it++)
      ret |= it.Data1().Substitute(binds,it.Data2());
    if(ret)
      result = TupleC(newTup);
    else
      result = TupleC(const_cast<TupleBodyC &>(*this));
    return ret;
  }
  
  //: Unify with simple symb
  
  bool TupleBodyC::UnifyLiteral(const LiteralBodyC &oth,BindSetC &bs) const {
    // Can't unify with a simple Literal.
    return false;
  }
  
  //: Is this equial to another LiteralC ?
  
  bool TupleBodyC::IsEqual(const LiteralC &oth) const {
    ONDEBUG(std::cerr << "TupleBodyC::IsEqual(), Called '" << Name() << "' and '" << oth << "'\n");
    TupleC tup(oth);
    if(!tup.IsValid())
      return false;
    if(Arity() != tup.Arity())
      return false;
    if(this == &tup.Body())
      return true;
    for(SArray1dIter2C<LiteralC,LiteralC> it(Args(),tup.Args());it;it++)
      if(!it.Data1().IsEqual(it.Data2()))
	return false;
    return true;
  }

  //: Get the name of symbol.
  
  StringC TupleBodyC::Name() const {
    StringC out;
    SArray1dIterC<LiteralC> it(Args());
    out += '(';
    if(it) {
      if(it->IsValid())
	out += it->Name();
      else
	out += "-NIL-";
      it++;
      for(;it;it++) {
	out += ' ';
	if(it->IsValid())
	  out += it->Name();
	else
	  out += "-NIL-";
      }
    }
    out += ')';
    return out;
  }
  
  UIntT TupleBodyC::Hash() const {
    UIntT hash = 101;
    int i = 0;
    for(SArray1dIterC<LiteralC> it(Args());it;it++,i++) {
      UIntT v = it->Hash(); 
      hash += v + (v << i);
    }
    return hash;
  }
  
  //: Dump info in human readable format to stream 'out'.
  void TupleBodyC::Dump(std::ostream &out) {
    out << Name();
  }
  
  //: Get an the numbered arg of a tuple.
  //: An invalid literal is returned if types don't match.
  
  LiteralC TupleArg(const LiteralC &s1,UIntT argno) {
    if(!s1.IsValid())
      return LiteralC();
    const TupleBodyC *tb = dynamic_cast<const TupleBodyC *>(&s1.Body());
    if(tb == 0 || argno >= tb->Args().Size())
      return LiteralC();
    return tb->Args()[argno];
  }
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(TupleBodyC,TupleC,LiteralC);
}
