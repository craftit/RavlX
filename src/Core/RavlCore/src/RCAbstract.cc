// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////
//! userlevel=Normal
//! author="Charles Galambos"
//! docentry="Basic Types.Reference Counting"
//! lib=RavlCore
//! rcsid="$Id$"
//! file="Ravl/Core/Base/RCAbstract.cc"

#include "Ravl/RCAbstract.hh"
#include "Ravl/Stream.hh"

namespace RavlN {

  //: Constructor.
  RCAbstractC::RCAbstractC(std::istream &in)
   : RCHandleVC<RCBodyVC>(in)
  {}

  //: Constructor.
  RCAbstractC::RCAbstractC(BinIStreamC &in)
   : RCHandleVC<RCBodyVC>(in)
  {}

  //: Dump info about handle
  
  void RCAbstractC::Dump(std::ostream &out) const {
    out << "RCAbstractC::Dump(), NOT IMPLEMENTED. \n";
  }
}
