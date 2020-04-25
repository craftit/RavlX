// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BASE64_HEADER
#define RAVL_BASE64_HEADER 1
//! rcsid="$Id$"
//! lib=RavlCore
//! docentry="Ravl.API.Core.Misc"
//! file="Ravl/Core/System/Base64.hh"

#include "Ravl/String.hh"
#include "Ravl/SArray1d.hh"

namespace RavlN {

  //! userlevel=Normal
  //: Base 64 string encoder / decoder.
  // Based on code from BASE64 by John Walker (http://www.fourmilab.ch/)
  
  class Base64C {
  public:    
    static StringC Encode(const SArray1dC<char> &buffer);
    //: Encode buffer.
    
    static SArray1dC<char> Decode(const StringC &buffer);
    //: Decode a string into raw binary.
  };
}

#endif
