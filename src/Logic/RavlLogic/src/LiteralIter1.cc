// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlLogic
//! file="Ravl/Logic/Base/LiteralIter1.cc"

#include "Ravl/Logic/LiteralIter1.hh"

namespace RavlLogicN {

  //: Goto next data element.
  // returns true if next element is valid.
  
  bool LiteralIter1BodyC::Next() {
    ready = false;
    return false;
  }
  
  //: Goto first data element.
  // returns true if next element is valid.
  
  bool LiteralIter1BodyC::First() {
    if(lit.IsValid())
      ready = true;
    return ready;
  }
  
  //: At a valid element ?
  // returns true if iterator is at a valid
  // element.
  
  bool LiteralIter1BodyC::IsElm() const {
    return ready;
  }
  
  //: Get current literal.
  
  LiteralC LiteralIter1BodyC::Data() {
    return lit;
  }
  
  //: Access binds associated with solution.
  
  BindSetC &LiteralIter1BodyC::Binds() { 
    return binds; 
  }
  
  //: Access binds associated with solution.
  
  const BindSetC &LiteralIter1BodyC::Binds() const {
    return binds; 
  }

}
