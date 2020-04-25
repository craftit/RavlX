// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlLogic
//! file="Ravl/Logic/Base/MinTerm.cc"

#include "Ravl/Logic/MinTerm.hh"
#include "Ravl/Logic/Not.hh"
#include "Ravl/Logic/Or.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/Logic/BindSet.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/PointerManager.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlLogicN {

  //: Default constructor.
  
  MinTermBodyC::MinTermBodyC() 
    : AndBodyC(3),
      t(true),
      n(true)
  {
    args[1] = t;
    args[2] = !n;
    ONDEBUG(std::cerr << "MinTermBodyC::MinTermBodyC() = " << Name() << "\n");
    RavlAssert(t.IsValid());
    RavlAssert(n.IsValid());
  }

  //: Constructor.
  
  MinTermBodyC::MinTermBodyC(const AndC &at,const OrC &ot) 
    : AndBodyC(3),
      t(at),
      n(ot)
  {
    if(!n.IsValid())
      n = OrC(true);
    if(!t.IsValid())
      t = AndC(true);
    args[1] = t;
    args[2] = !n;
    ONDEBUG(std::cerr << "MinTermBodyC::MinTermBodyC(const AndC &,const OrC &) = " << Name() << "\n");
    RavlAssert(t.IsValid());
    RavlAssert(n.IsValid());
  }
  
  //: Construct from a single literal.
  // Effectively add !lit if negate is true.
  
  MinTermBodyC::MinTermBodyC(const LiteralC &lit,bool negate) 
    : AndBodyC(3),
      t(true),
      n(true)
  {
    
    args[1] = t;
    args[2] = !n;
    if(negate)
      AndNotAdd(lit);
    else
      AndAdd(lit);
    ONDEBUG(std::cerr << "MinTermBodyC::MinTermBodyC(const LiteralC &,bool) Lit=" << lit << " this=" << Name() << "\n");  
    RavlAssert(t.IsValid());
    RavlAssert(n.IsValid());
  }
  
  //: Constructor
  
  MinTermBodyC::MinTermBodyC(const SArray1dC<LiteralC> &ts,const SArray1dC<LiteralC> &ns,bool useArrayDirectly) 
    : AndBodyC(3)
  {
    SetTerms(ts,ns,useArrayDirectly); 
    ONDEBUG(std::cerr << "MinTermBodyC::MinTermBodyC(const SArray1dC<LiteralC> &,const SArray1dC<LiteralC> &,bool) = " << Name() << "\n");  
    RavlAssert(t.IsValid());
    RavlAssert(n.IsValid());
  }
  
  //: Setup terms.
  
  void MinTermBodyC::SetTerms(const SArray1dC<LiteralC> &ts,const SArray1dC<LiteralC> &ns,bool useArrayDirectly) {
    OrC orv(ns,useArrayDirectly);
    AndC xa(ts,useArrayDirectly);
    t = xa;
    n = orv;
    args[1] = t;
    args[2] = !n;
    ONDEBUG(std::cerr << "MinTermBodyC::SetTerms(), " << Name() << "\n");
  }
  
  //: Construct from a binary stream.
  
  MinTermBodyC::MinTermBodyC(std::istream &strm)
    : AndBodyC(strm)
  {}
  
  //: Construct from a binary stream.
  
  MinTermBodyC::MinTermBodyC(BinIStreamC &strm)
    : AndBodyC(strm)
  { strm >> ObjIO(t) >> ObjIO(n); }
  
  //: Save to binary stream 'out'.
  
  bool MinTermBodyC::Save(std::ostream &out) const
  { return AndBodyC::Save(out); }
  
  //: Save to binary stream 'out'.
  
  bool MinTermBodyC::Save(BinOStreamC &out) const { 
    if(!AndBodyC::Save(out)) 
      return false;
    out << ObjIO(t) << ObjIO(n);
    return true;
  }
  
  //: Copy minterm.
  
  RCBodyVC &MinTermBodyC::Copy() const {
    return *new MinTermBodyC(Pos().Copy(),Neg().Copy(),true); 
  }
  
  
  //: Add another term to the minterm.
  
  bool MinTermBodyC::AndAdd(const LiteralC &lit) {
    if(!lit.IsValid())
      return true;
    ONDEBUG(std::cerr << "MinTermBodyC::AndAdd(),'" << Name() << "' * '" << lit.Name() << "'\n");
    MinTermC mt(lit);
    if(mt.IsValid()) {
      PosTerm().AndAdd(mt.PosTerm());
      NegTerm().OrAdd(mt.NegTerm());
      return true;
    }
    AndC andt(lit);
    if(andt.IsValid()) {
      SArray1dIterC<LiteralC> it(andt.Terms());
      RavlAssert(it);
      it++;
      bool ret = true;
      for(;it;it++)
	ret &= AndAdd(*it);
      return ret;
    }
    NotC nt(lit);
    if(nt.IsValid()) {
      AndNotAdd(nt.Term());
    } else
      t.AndAdd(lit);
    ONDEBUG(std::cerr << "MinTermBodyC::AndAdd(), Add '" << lit.Name() << "' = '" << Name() << "'\n");
    return true;
  }
  
  //: Add a negated term to the minterm.
  
  bool MinTermBodyC::AndNotAdd(const LiteralC &lit) {
    if(!lit.IsValid())
      return true;
    NotC nt(lit);
    if(nt.IsValid())
      return AndAdd(nt.Term());
    AndC andt(lit);
    if(andt.IsValid()) {
      SArray1dIterC<LiteralC> it(andt.Terms());
      RavlAssert(it);
      for(it++;it;it++) {
	NotC nt2(*it);
	if(nt2.IsValid()) {
	  AndAdd(nt2.Term());
	} else
	  n.OrAdd(*it);
      }
      return true;
    }
    n.OrAdd(lit);
    return true;
  }

  //: Substitute variables in 'binds' for their bound values.
  // This builds a new literal with the substute values (if there
  // are any). The new value is assigned to 'result' <p>
  // Returns true if at least one substitution has been made,
  // false if none.
  
  bool MinTermBodyC::Substitute(const BindSetC &binds,LiteralC &result) const {
    AndC nt;
    OrC nn;
    bool sub = t.Substitute(binds,nt);
    sub |= n.Substitute(binds,nn);
    if(sub) {
      result = MinTermC(nt,nn);
      return true;
    }
    result = MinTermC(const_cast<MinTermBodyC &>(*this));
    return false;
  }
  
  //: Does this minterm cover all terms of mt ?
  
  bool MinTermBodyC::Covers(const MinTermC &mt,BindSetC &bs) const {
    if(!mt.IsValid())
      return false;
    RavlAssert(bs.IsValid());
    BindMarkT ibm = bs.Mark();
    // FIXME:- Do some kind of backtracking.
    // Check positive terms.
    SArray1dIterC<LiteralC> it(mt.Pos());
    RavlAssert(it);
    it++;
    for(;it;it++) {
      bool found = false;
      SArray1dIterC<LiteralC> it2(t.Args());
      RavlAssert(it2);
      it2++;
      for(;it2;it2++) {
	if(it2->Unify(*it,bs)) {
	  found = true;
	  break;
	}
      }
      if(!found) {
	bs.Undo(ibm);
	return false;
      }
    }
    // Check negative terms.
    RavlAssert(mt.NegTerm().IsValid());
    RavlAssert(mt.Neg().IsValid());
    it = mt.Neg();
    RavlAssert(it);
    it++;
    for(;it;it++) { 
      bool found = false;
      SArray1dIterC<LiteralC> it2(t.Args());
      RavlAssert(it2);
      it2++;
      for(;it2;it2++) {
	if(it2->Unify(*it,bs)) {
	  found = true;
	  break;
	}
      }
      if(!found) {
	bs.Undo(ibm);
	return false;
      }     
    }
    return true;
  }


  MinTermC operator*(const MinTermC &mt1,const MinTermC &mt2) {
    MinTermC ret(mt1.PosTerm(),mt1.NegTerm());
    ret.NegTerm().OrAdd(mt2.Neg());
    ret.PosTerm().AndAdd(mt2.Pos());
    return ret;
  }
  //: And two min-terms.
  
  MinTermC operator*(const MinTermC &mt1,const LiteralC &t2) {
    MinTermC xt(t2);
    if(xt.IsValid())
      return mt1 * xt;
    MinTermC ret(mt1.PosTerm(),mt1.NegTerm());
    NotC nt(t2);
    if(nt.IsValid())
      ret.NegTerm().OrAdd(nt.Term());	    
    else
      ret.PosTerm().AndAdd(t2);
    return ret;
  }
  //: And a min term and a literal.
  
  MinTermC operator*(const LiteralC &t1,const MinTermC &mt2) {
    MinTermC xt(t1);
    if(xt.IsValid())
      return xt * mt2;
    MinTermC ret(mt2.PosTerm(),mt2.NegTerm());
    NotC nt(t1);
    if(nt.IsValid())
      ret.NegTerm().OrAdd(nt.Term());	    
    else
      ret.PosTerm().AndAdd(t1);
    return ret;    
  }
  //: And a min term and a literal.
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(MinTermBodyC,MinTermC,AndC);
  
}
