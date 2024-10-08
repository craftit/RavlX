// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlDPDisplay
//! file="Ravl/GUI/DPDisplay/RavlGUIDisplay.cc"

#include "Ravl/GUI/DPDisplay.hh"

namespace RavlGUIN {
  extern void InitDPWindowFormat();
  extern void InitDPDisplayImage();
  extern void InitDPDisplayPoints();
  extern void InitDPDisplayPolygon2d();
  
  void InitRavlGUIDisplay() {
    InitDPWindowFormat();
    InitDPDisplayImage();
    InitDPDisplayPoints();
    InitDPDisplayPolygon2d();
  }
}
