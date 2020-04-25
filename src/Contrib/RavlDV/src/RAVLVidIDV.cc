// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlDV
//! file="Ravl/Contrib/DV/RAVLVidIDV.cc"

namespace RavlImageN {

  extern void InitDvFrameConvert();
  extern void InitDvIFormat();

  void ExtVidIO_Init() {
    InitDvIFormat();
    InitDvFrameConvert();
  }
}

