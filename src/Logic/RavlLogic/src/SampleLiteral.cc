// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlLogic
//! file="Ravl/Logic/DecisionTree/SampleLiteral.cc"

#include "Ravl/Logic/SampleLiteral.hh"

namespace RavlLogicN {
  
  //: Make a list of all LiteralC and sub LiteralC's in the sample.
  
  HSetC<LiteralC> SampleLiteralC::Components() const {
    HSetC<LiteralC> ret;
    for(DArray1dIterC<LiteralC> it(*this);it;it++) {
      if(!ret.Insert(*it))
	continue; // Already done.
      it->SubLiterals(ret);
    }
    return ret;
  }

}
