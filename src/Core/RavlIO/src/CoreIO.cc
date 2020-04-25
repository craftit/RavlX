// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////////
//! lib=RavlIO
//! file="Ravl/Core/IO/CoreIO.cc"

#include "Ravl/DP/CoreIO.hh"

namespace RavlN {
  
  extern void IncludeFileFormatStream();
  extern void InitFileFormatByteFile();
  
  //: Initalise all the core IO.
  
  void InitCoreIO() {
    InitFixedIndexIO();
    IncludeFileFormatStream();
    InitFileFormatByteFile();
  }
  
}

