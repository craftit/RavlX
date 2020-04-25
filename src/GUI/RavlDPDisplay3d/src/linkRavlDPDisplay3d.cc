// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlDPDisplay3d

namespace RavlGUIN {
  
  extern void InitDPDisplay3dConv();
  extern void InitDPWindow3dFormat();
  
  void linkRavlDPDisplay3d() {
    InitDPWindow3dFormat();
    InitDPDisplay3dConv();
  }
}
