// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlLogic
//! file="Ravl/Logic/Index/LiteralIndexNode.cc"

#include "Ravl/Logic/LiteralIndexNode.hh"
#include "Ravl/Logic/LiteralIndexIter.hh"
#include "Ravl/Logic/LiteralIndexFilter.hh"
#include "Ravl/Logic/Tuple.hh"
#include "Ravl/HashIter.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlLogicN {
  
  //: Get name for element.
  // usefull for debuging.
  
  StringC LiteralIndexNodeBodyC::Name() {
#if RAVL_CPUTYPE_64
    return StringC("LINode:") + StringC((UInt64T) this);
 #else
    return StringC("LINode:") + StringC((UIntT) this);
#endif
 }
  
  //: Lookup next level in tree.
  // returns true if this level is a leaf and leave next unchanged.
  // if no suitable node is found 'next' is made invalid.
  
  bool LiteralIndexNodeBodyC::Lookup(const TupleC &tuple,LiteralIndexElementC &next) {
    if(term >= tuple.Arity()) {
      next = none;
      return false;
    }
    const LiteralC &subKey = tuple[term];
    RavlAssert(subKey.IsValid());
    if(!index.Lookup(subKey,next))
      next.Invalidate();
    return false;
  }

  //: Delete branch with index 'key'.
  
  bool LiteralIndexNodeBodyC::Del(const TupleC &tuple) {
    if(term >= tuple.Arity()) {
      LiteralIndexLeafC leaf(none);      
      RavlAssert(leaf.IsValid());
      none.Invalidate();
      return true;
    }
    const LiteralC &subKey = tuple[term];
    RavlAssert(subKey.IsValid());
    return index.Del(subKey);
  }
  
  //: Filter next level in tree.
  // If there's a choice return an iterator otherwise the next branch
  // in the tree is assigned to 'next'.  var is the variable to which
  // the value of the iterator should be bound.
  
  LiteralMapIterC<LiteralIndexElementC> LiteralIndexNodeBodyC::Filter(const LiteralC &key,
                                                                      LiteralIndexElementC &next,
                                                                      BindSetC &binds,
                                                                      LiteralC &var) 
  {
    ONDEBUG(cerr << "LiteralIndexNodeBodyC::Filter(), Called Key:" << key << "  Var="  << var << " Term=" << term << "\n");
    TupleC tup(key);
    RavlAssert(tup.IsValid());
    const LiteralC &subkey = tup[term];
    var = subkey;
    if(subkey.IsGrounded() && index.IsGrounded()) {
      // There are no variables in the sub-key so we can just do 
      // a lookup.
      if(!index.Lookup(subkey,next))
	next.Invalidate(); // The lookup failed.
      return LiteralMapIterC<LiteralIndexElementC>();
    }
    
    next.Invalidate();
    // Return an iterator through possible sub keys.
    return LiteralIndexFilterC<LiteralIndexElementC>(index,subkey,binds);
  }

  //: Insert/Replace element into the node.
  // returns false if element already exists.
  
  bool LiteralIndexNodeBodyC::Insert(const TupleC &key,LiteralIndexElementC &next) {
    RavlAssert(key.IsValid());
    if(term >= key.Arity()) {
      none = next;
      return true;
    }
    index[key[term]] = next;
    return true;
  }

  //: Return the size of the index.
  // The number of branches from this node is returned.
  
  UIntT LiteralIndexNodeBodyC::Size() const {
    UIntT ret = index.Size();
    if(none.IsValid())
      ret++;
    return ret;
  }
  
  
  //: Dump index in human readable form.
  // For debugging only.
  
  void LiteralIndexNodeBodyC::Dump(ostream &out,int level) const {
    //out << Indent(level) <<"LiteralIndexNodeC Term=" << term <<"\n";
    if(none.IsValid()) {
      out << Indent(level) <<"[" << term << "]=-None-\n";
      none.Dump(out,level+2);
    }
    for(LiteralIndexIterC<LiteralIndexElementC> it(index);it;it.Next()) {
      out << Indent(level) << "[" << term << "]=" << it.Data().Name() << "\n";
      it.MappedData().Dump(out,level+2);
    }
  }

}
