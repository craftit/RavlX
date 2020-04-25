// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////
// L2World/NLPCausalLink.cc   12/12/96   By Charles Galambos
// $Id$
//! rcsid="$Id$"
//! lib=RavlLogicNLP
//! file="Ravl/Logic/NonLinearPlanner/NLPCausalLink.cc"

#include "Ravl/Logic/NLPCausalLink.hh"

namespace RavlLogicN {

  ////////////////////////
  // Get name of link.
  
  StringC NLPCausalLinkC::Name() const {
    StringC buff;
    if(!aCond.IsValid())
      buff = "Order";
    else
      buff = "Cause{" + aCond.Name() + "}";
    return buff;
  }
  
  /////////////////////////
  // Dump to stdout.
  
  void NLPCausalLinkC::Dump(void) const 
  { cerr << "NLPCausalLinkC::Dump(), " << Name() << " \n"; }
  
}
