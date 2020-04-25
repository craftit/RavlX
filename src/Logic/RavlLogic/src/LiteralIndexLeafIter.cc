// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlLogic
//! file="Ravl/Logic/Index/LiteralIndexLeafIter.cc"

#include "Ravl/Logic/LiteralIndexLeafIter.hh"

namespace RavlLogicN {
  
  //: Constructor
  
  LiteralIndexLeafBodyIterC::LiteralIndexLeafBodyIterC(const LiteralIndexBaseC &ind,bool nVarsOnly)
    : varsOnly(nVarsOnly)
  {
    if(varsOnly)
      hit = ind.Body().varMap;
    else 
      hit = ind.Body().map;
  }
  
  bool LiteralIndexLeafBodyIterC::First()
  { return hit.First(); }
  //: Goto first element.
    
  bool LiteralIndexLeafBodyIterC::Next()
  { return hit.Next(); }
  //: Goto next element.
  // returns true if its valid.
    
  bool LiteralIndexLeafBodyIterC::IsElm() const 
  { return hit.IsElm(); }
  //: At a valid element ?
    
  LiteralC LiteralIndexLeafBodyIterC::Data()
  { return hit.Key(); }
  //: Get key for current item.
    
  LiteralIndexElementC &LiteralIndexLeafBodyIterC::MappedData()
  { return hit.Data(); }
  //: Access data.
    
  const LiteralIndexElementC &LiteralIndexLeafBodyIterC::MappedData() const
  { return hit.Data(); }
  //: Access data.
  
  //:----------------------------------------------------------------------------------
  
  LiteralIndexLeafIterC::LiteralIndexLeafIterC(const LiteralIndexBaseC &ind,bool varsOnly)
    : LiteralMapIterC<LiteralIndexElementC>(*new LiteralIndexLeafBodyIterC(ind,varsOnly))
  {}
  //: Constructor.

}

