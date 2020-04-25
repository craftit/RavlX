// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlLogic
//! file="Ravl/Logic/Index/LiteralIndexLeafVarIter.cc"

#include "Ravl/Logic/LiteralIndexLeafVarIter.hh"

namespace RavlLogicN {
  
  //: Constructor
  
  LiteralIndexLeafVarBodyIterC::LiteralIndexLeafVarBodyIterC(const LiteralIndexBaseC &ind,const LiteralC &nFixedLit,const LiteralIndexLeafC &nFixedLeaf)
    : isFirst(true),
      fixedLit(nFixedLit),
      fixedLeaf(nFixedLeaf)
  { hit = ind.Body().varMap; }
  
  //: Goto first element.
  
  bool LiteralIndexLeafVarBodyIterC::First() {
    isFirst = true;
    hit.First();
    return true;
  }
    
  //: Goto next element.
  // returns true if its valid.
  bool LiteralIndexLeafVarBodyIterC::Next() { 
    if(isFirst) {
      isFirst = false;
      return hit.IsElm();
    }
    return hit.Next(); 
  }
    
  //: At a valid element ?
  
  bool LiteralIndexLeafVarBodyIterC::IsElm() const { 
    if(isFirst) return true;
    return hit.IsElm(); 
  }
  
  //: Get key for current item.
  
  LiteralC LiteralIndexLeafVarBodyIterC::Data() { 
    if(isFirst) return fixedLit;
    return hit.Key(); 
  }
    
  //: Access data.
  
  LiteralIndexElementC &LiteralIndexLeafVarBodyIterC::MappedData(){ 
    if(isFirst) return fixedLeaf;
    return hit.Data(); 
  }
    
  //: Access data.
  
  const LiteralIndexElementC &LiteralIndexLeafVarBodyIterC::MappedData() const {
    if(isFirst) return fixedLeaf;
    return hit.Data(); 
  }
  
  //:---------------------------------------------------------------------------
  //: Constructor.
  
  LiteralIndexLeafVarIterC::LiteralIndexLeafVarIterC(const LiteralIndexBaseC &ind,const LiteralC &fixedLit,const LiteralIndexLeafC &fixedLeaf)
    : LiteralMapIterC<LiteralIndexElementC>(*new LiteralIndexLeafVarBodyIterC(ind,fixedLit,fixedLeaf))
  {}

}

