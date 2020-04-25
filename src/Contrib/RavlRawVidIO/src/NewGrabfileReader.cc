// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#include "Ravl/Image/NewGrabfileReader.hh"
//! lib=RavlRawVidIO

#include "Ravl/Image/LegacyGrabfileReader.hh"
#include "Ravl/Image/GrabfileReaderV1.hh"

namespace RavlImageN {

//==========================================================================//

GrabfileReaderC* NewGrabfileReader(const char* const filename, bool legacy)
{
  GrabfileReaderC* p = 0;

  if(legacy) {
    p = new LegacyGrabfileReaderC();
  } else {
    const int version = GrabfileReaderC::FileVersion(filename);
    switch(version) {
    case 1:
      p = new GrabfileReaderV1C();
      break;
    }
  }

  return p;
}

//==========================================================================//

}

