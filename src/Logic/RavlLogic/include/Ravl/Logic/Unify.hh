// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_UNIFY_HEADER
#define RAVLLOGIC_UNIFY_HEADER 1
//////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Logic"
//! lib=RavlLogic
//! author="Charles Galambos"
//! userlevel=Normal
//! file="Ravl/Logic/Base/Unify.hh"

#include "Ravl/Logic/Literal.hh"
#include "Ravl/Logic/BindSet.hh"

namespace RavlLogicN {
  inline bool Unify(const LiteralC &s1,const LiteralC &s2,BindSetC &bs) {
    return s1.Unify(s2,bs);
  }
  //: Unify s1 and s2 with binds bs.
  // any extra binding needed will be added to bs.

  inline bool Unify(const LiteralC &s1,const LiteralC &s2) {
    BindSetC bs(true);
    return s1.Unify(s2,bs);
  }
  //: Test if s1 and s2 can be unified.
  
}

#endif
