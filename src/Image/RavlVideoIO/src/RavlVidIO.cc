// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////
//! lib=RavlVideoIO
//! file="Ravl/Image/VideoIO/RavlVidIO.cc"

#include "Ravl/Image/VidIO.hh"
#include "Ravl/Image/ImgIO.hh"

namespace RavlImageN {
  
  void InitVidIO() {
    InitRawIOFormat();
    InitCifFormat();
    InitRGBFormat();
    InitSYUVFormat();
    InitJSFormat();
    InitAviFormat();
  }
}

