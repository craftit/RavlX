// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlLogic
//! file="Ravl/Logic/Base/LiteralIter.cc"

#include "Ravl/Logic/LiteralIter.hh"

namespace RavlLogicN {
  
  //: At a valid element ?
  
  bool LiteralIterBodyC::IsElm() const { 
    RavlAssertMsg(0,"LiteralIterBodyC::IsElm(), Abstract method called. ");
    return false; 
  }
  
  //: Get some data.
  
  LiteralC LiteralIterBodyC::Data() { 
    RavlAssertMsg(0,"LiteralIterBodyC::Data(), Abstract method called. ");
    return LiteralC(); 
  }

  //: Goto next data element.
  
  bool LiteralIterBodyC::Next() {
    RavlAssertMsg(0,"LiteralIterBodyC::Next(), Abstract method called. ");
    return false;
  }

  //: Goto first data element.
  // returns true if next element is valid.
  
  bool LiteralIterBodyC::First() {
    RavlAssertMsg(0,"LiteralIterBodyC::First(), Abstract method called. ");
    return false;
  }
  
  //: Access binds associated with solution.
  
  BindSetC &LiteralIterBodyC::Binds() { 
    RavlAssertMsg(0,"LiteralIterBodyC::Binds(), Abstract method called. ");
    static BindSetC binds;
    return binds; 
  }
    
  //: Access binds associated with solution.
  
  const BindSetC &LiteralIterBodyC::Binds() const { 
    RavlAssertMsg(0,"LiteralIterBodyC::Binds() const, Abstract method called. ");
    static BindSetC binds;
    return binds; 
  }
    

}
