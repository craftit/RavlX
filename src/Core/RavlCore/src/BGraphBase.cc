// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////////
//! date="19/2/1997"
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Container/Branch/BGraphBase.cc"

#include "Ravl/BGraphBase.hh"
#include "Ravl/BGraphBaseIter.hh"
#include "Ravl/Stream.hh"

namespace RavlN {
  
  ////////////////////
  
  // Dump to stream.
  
  void BGraphEdgeBaseC::Dump(std::ostream &out) 
  { out << "BGraphBaseC::Dump() Unimplemented\n"; }
  
  /////////////////////
  // Dump to stream.
  
  void BGraphNodeBaseC::Dump(std::ostream &out) 
  { out << "BGraphBaseC::Dump() Unimplemented\n"; }
  
  BGraphBaseAdjIterC BGraphNodeBaseC::In() 
  { return BGraphBaseAdjIterC(*this,Fwd); }
  
  BGraphBaseAdjIterC BGraphNodeBaseC::Out() 
  { return BGraphBaseAdjIterC(*this,Bkw); }
  
  /////////////////////
  // Dump to stream.
  
  void BGraphBaseC::Dump(std::ostream &out) 
  { out << "BGraphBaseC::Dump() Unimplemented\n"; }
  
}
