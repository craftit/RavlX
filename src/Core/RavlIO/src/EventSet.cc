// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlIO
//! file="Ravl/Core/IO/EventSet.cc"

#include "Ravl/DP/EventSet.hh"
#include "Ravl/Assert.hh"

namespace RavlN {
  //: Wait for all events in the set to occurre.
  
  bool DPEventSetBodyC::Wait() {
    for(HSetIterC<DPEventC> it(set);it.IsElm();it.Next()) {
      if(!const_cast<DPEventC &>(it.Data()).Wait())
	return false;
    }
    return true;
  }
  
  //: Has event occured ?
  
  IntT DPEventSetBodyC::Occurred()  {
    RavlAssert(0);
    return 0;
  }
  
}
