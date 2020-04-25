// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlThreads

#include "Ravl/Threads/RCRWLock.hh"

namespace RavlN {

  RCRWLockC::RCRWLockC(bool)
    : RCWrapC<RWLockC>(true,true)
  {}
  //: Constructor.
  
}
