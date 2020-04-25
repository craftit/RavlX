// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlLogic
//! file="Ravl/Logic/Index/LiteralIndexLeaf.cc"

#include "Ravl/Logic/LiteralIndexLeaf.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlLogicN {

  //: Get name for element.
  // usefull for debuging.
  
  StringC LiteralIndexLeafBodyC::Name() {
#if RAVL_CPUTYPE_64
    return StringC("LILeaf:") + StringC((UInt64T) this);
#else
    return StringC("LILeaf:") + StringC((UIntT) this);
#endif
  }

  //: Lookup next level in tree.
  // returns true if this level is a leaf and leave next unchanged.
  // if no suitable node is found 'next' is made invalid.
  
  bool LiteralIndexLeafBodyC::Lookup(const TupleC &nkey,LiteralIndexElementC &next) {
    if(key == ((LiteralC &)nkey))
      return true;
    next.Invalidate();
    return false;
  }

  //: Filter next level in tree.
  // If there's a choice return an iterator otherwise the next branch
  // in the tree is assigned to 'next'.  var is the variable to which
  // the value of the iterator should be bound.
  
  LiteralMapIterC<LiteralIndexElementC> LiteralIndexLeafBodyC::Filter(const LiteralC &key,LiteralIndexElementC &next,BindSetC &binds,LiteralC &var) {
    ONDEBUG(cerr << "LiteralIndexLeafBodyC::Filter(), Called. Key=" << key << " var=" << var <<"\n");
    LiteralMapIterC<LiteralIndexElementC> ret;
    next.Invalidate();
    return ret;
  }
  
  //: Dump index in human readable form.
  // For debugging only.
  
  void LiteralIndexLeafBodyC::Dump(ostream &out,int level) const {
    out << Indent(level) << "Leaf. Key = " << key.Name() << "\n";
  }

  
}
