// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlDMedia
//! file="Ravl/Contrib/DMedia/Video.cc"

#include "Ravl/DMedia/Video.hh"

namespace RavlImageN
{
  //: Constructor.
  // type  = VL_SRC,VL_DRN
  
  VLNodeVideoBodyC::VLNodeVideoBodyC(const VLServerC &srv,int type,int number)
    : VLNodeBodyC(srv,type,VL_VIDEO,number)
  {}
  
  //: Initalise componentes.
  
  bool VLNodeVideoBodyC::Init() {
    //SetC
    return VLNodeBodyC::Init();
  }
  

}
