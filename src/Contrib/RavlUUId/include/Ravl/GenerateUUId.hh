// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2007, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_UUID_HEADER
#define RAVL_UUID_HEADER 1
//! lib=RavlUUId

#include "Ravl/String.hh"
#include "Ravl/UUId.hh"

namespace RavlN {
  
  StringC GenerateUUId();
  //: Generate a UUId in string form.

  void GenerateUUId(UUIdC &anId);
  //: Generate a binary UUId;

}


#endif
