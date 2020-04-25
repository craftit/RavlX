// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlCore

#include "Ravl/RCLayer.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/VirtualConstructor.hh"

namespace RavlN {
  
  //: Called when owner handles drop to zero.
  
  void RCLayerBodyC::ZeroOwners() {
    //... Abstract method
  }
  
  static TypeNameC g_type(typeid(RCLayerBodyC),"RavlN::RCLayerBodyC");

  RAVL_INITVIRTUALCONSTRUCTOR_NAMED(RCLayerBodyC,"RavlN::RCLayerBodyC");

}
