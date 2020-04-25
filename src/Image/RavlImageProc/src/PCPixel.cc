// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Lines/PCPixel.cc"

#include "Ravl/Image/PCPixel.hh"
#include "Ravl/Stream.hh"

namespace RavlImageN {

  //: Write out to stream.
  
  ostream &PCIndex2dC::WriteGF(ostream &out) const {
    out << Col() << " " << Row() << " ";
    return out;
  }
  
}
