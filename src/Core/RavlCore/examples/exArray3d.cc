// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Container/Array/exArray3.cc"
//! docentry="Ravl.API.Core.Arrays.3D"
//! author="Charles Galambos"
//! userlevel=Normal

#include "Ravl/Array3d.hh"
#include "Ravl/Array3dIter.hh"
#include "Ravl/EntryPnt.hh"
#include "Ravl/Stream.hh"

using namespace RavlN;

int Main(int argc, char* argv[])
{
  Array3dC<IntT> arr(-5,6,-7,8,-9,10);
  std::cout << "array has arr.Range1() = " << arr.Range1() << "  arr.Range2() = " << arr.Range1() << "\n";
  
  // Index each dimension separately.
  
  arr[-2][3][6] = 1;

  // Access using a 2d index.
  
  Index3dC at(2,5,7);
  arr[at] = 4;
  
  // Go through array with an iterator.
  
  for(Array3dIterC<IntT> it(arr);it;it++) 
    if(*it != 0)
      std::cout << "Element " << it.Index() << " is non zero, " << *it << "\n";
  
  return 0;
}

RAVL_ENTRY_POINT(Main);
