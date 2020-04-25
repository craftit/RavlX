// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#include "Ravl/Image/NewGrabfileWriter.hh"
#include <stdint.h>
#include "Ravl/Image/GrabfileWriterV1.hh"
//! lib=RavlRawVidIO

namespace RavlImageN {

//==========================================================================//

GrabfileWriterC* NewGrabfileWriter(const int version)
{
  GrabfileWriterC* p = 0;
  switch(version) {
  case 1:
    p = new GrabfileWriterV1C();
    break;
  }

  return p;
}

//==========================================================================//

}

