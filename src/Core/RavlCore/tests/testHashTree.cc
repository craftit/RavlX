// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Container/Trees/testHashTree.cc"
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Trees"
//! userlevel=Develop

#include "Ravl/String.hh"
#include "Ravl/HashTree.hh"

using namespace RavlN;

int main() {
  
  HashTreeC<int,StringC> tree(true);
  
  tree.Add(1,StringC("Hello"));
  
  std::cerr << tree;
  return 0;
}

namespace RavlN {
  template
  class HashTreeC<int, StringC>;
}