// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Corners/CornerDetector.cc"


#include "Ravl/Image/CornerDetector.hh"

namespace RavlImageN {

  //: Default constructor.
  
  CornerDetectorBodyC::CornerDetectorBodyC()
  {}
  
  //: Get a list of corners from 'img'
  
  DListC<CornerC> CornerDetectorBodyC::Apply(const ImageC<ByteT> &img) {
    DListC<CornerC> ret;
    RavlAssertMsg(0,"CornerDetectorBodyC::Apply(), Abstract method called. ");
    return ret;
  }
  
}
