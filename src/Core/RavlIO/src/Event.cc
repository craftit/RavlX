// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlIO
//! file="Ravl/Core/IO/Event.cc"

#include "Ravl/DP/Event.hh"
#include "Ravl/Stream.hh"
#include "Ravl/DP/Converter.hh"

namespace RavlN {

  //: Wait for event.
  // Return when event occurred.
  
  bool DPEventBodyC::Wait()
  { return false; }
  
  //: Has event occurred ?
  // Returns number of events since last call.
  
  IntT DPEventBodyC::Occurred()
  { return 0; }
  
  //: Write to output.
  
  bool DPEventBodyC::Save(std::ostream &out) const {
    out << "DPEventBodyC::Save()\n";
    return false;
  }
  
  /////////////////////////////////////////////
  
  //: Dummy function.
  
  void DPEventC::Dummy(void)
  {}
  

}
