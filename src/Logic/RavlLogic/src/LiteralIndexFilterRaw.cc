// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlLogic
//! file="Ravl/Logic/Index/LiteralIndexFilterRaw.cc"

#include "Ravl/Logic/LiteralIndexFilterRaw.hh"

namespace RavlLogicN {
  
  //: Goto next data element.
  // returns true if next element is valid.
  
  bool LiteralIndexFilterRawBodyC::Next()
  { return LiteralIndexFilterBaseBodyC::Next(); }
  
  //: Goto first data element.
  // returns true if next element is valid.
  
  bool LiteralIndexFilterRawBodyC::First()
  { return LiteralIndexFilterBaseBodyC::First(); }
  
  //: At a valid element ?
  // returns true if iterator is at a valid
  // element.
  bool LiteralIndexFilterRawBodyC::IsElm() const
  { return LiteralIndexFilterBaseBodyC::IsElm(); }
  
  LiteralC LiteralIndexFilterRawBodyC::Data()
  { return LiteralIndexFilterBaseBodyC::Data().Key(); }
  //: At a valid element.
  
  BindSetC &LiteralIndexFilterRawBodyC::Binds()
  { return binds; }
  //: Access binds associated with solution.
    
  const BindSetC &LiteralIndexFilterRawBodyC::Binds() const
  { return binds; }
  //: Access binds associated with solution.

}
