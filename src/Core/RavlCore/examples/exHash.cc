// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Container/Hash/exHash.cc"
//! docentry="Ravl.API.Core.Hash Tables"
//! userlevel=Normal
//! author="Charles Galambos"

#include "Ravl/Hash.hh"
#include "Ravl/HashIter.hh"

using namespace RavlN;

int main() {

  HashC<int,const char *> hashTable;

  hashTable[2] = "a 2.";
  hashTable[3] = "a 3.";
  hashTable[10] = "a 10.";
  
  std::cout << "Lookup of 3 gives " << hashTable[3] << "\n";
  
  for(HashIterC<int,const char *> it(hashTable);it;it++)
    std::cout << " Key=" << it.Key() << " Data=" << it.Data() << "\n";
  
  return 0;
}
