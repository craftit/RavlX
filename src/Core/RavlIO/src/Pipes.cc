// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlIO
//! file="Ravl/Core/IO/Pipes.cc"

#include "Ravl/DP/Pipes.hh"
#include "Ravl/Assert.hh"

namespace RavlN {

  //: Access input port.
  
  DPIPortBaseC DPIPipeBaseBodyC::Port() {
    RavlAssert(0); // Abstract base class.
    return DPIPortBaseC();
  }
  
  //: Setup input.
  
  bool 
  DPIPipeBaseBodyC::SetInput(const DPIPortBaseC &)
  { return false; }

  
  //: Access input port.
  
  DPOPortBaseC DPOPipeBaseBodyC::Port() {
    RavlAssert(0); // Abstract base class.
    return DPOPortBaseC();
  }
  
  //: Setup output.
  
  bool 
  DPOPipeBaseBodyC::SetOutput(const DPOPortBaseC &)
  { return false; }
}

