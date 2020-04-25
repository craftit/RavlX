// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Container/Array/exArray1.cc"
//! docentry="Ravl.API.Core.Arrays.1D"
//! author="Charles Galambos"
//! userlevel=Normal

#include "Ravl/Array1d.hh"
#include "Ravl/Array1dIter.hh"
#include "Ravl/EntryPnt.hh"
#include "Ravl/Stream.hh"

using namespace RavlN;

int Main(int argc, char* argv[])
{
  
  Array1dC<IntT> arr(-10,10);
  
  std::cout << "arr.Range() = " << arr.Range() << "\n";
  std::cout << "arr.Size()  = " << arr.Size() << "\n";
  
  arr.Fill(0); // Fill array with 0's
  
  arr[-2] = 1;
  arr[3] = 2;
  
  for(Array1dIterC<IntT> it(arr);it;it++)
    if(*it != 0)
      std::cout << "Array element arr[" << it.Index() << "] is non zero, " << *it << "\n";
  
  return 0;
}

RAVL_ENTRY_POINT(Main);
