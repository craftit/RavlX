// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Base/Index.cc"

#include "Ravl/Index.hh"
#include "Ravl/Stream.hh"

namespace RavlN {
  std::istream & operator>>(std::istream & sss, IndexC & i) {
    sss >> i.V();
    return  sss;
  }

  std::ostream & operator<<(std::ostream & s, const IndexC & i) {
    s << i.V();
    return s;
  }
}





