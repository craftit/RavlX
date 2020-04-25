// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"  $Id$
//! lib=RavlCore
//! file="Ravl/Core/Container/SArray/exSArr1.cc"
//! author="Radek Marik"
//! docentry="Ravl.API.Core.Arrays.1D"
//! userlevel=Normal

#include "Ravl/SArray1d.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/Stream.hh"

using namespace RavlN;

int main()
{
  std::cout << "Simple 1 dimensional array of char\n"
           << "==================================\n";

  char oString[] = "Hello world    ";  // 12 characters
  SArray1dC<char> stringA(oString, 12, false); //create the 1. array
  SArray1dC<char> stringB(stringA);    //another access to the 1. array
  stringA[3] = '3';

  std::cout << "The original string: " << oString    << '\n'
           << "array stringA      : " << stringA    << '\n'
           << "array stringB      : " << stringB    << '\n'
           << "stringB[9]         : " << stringB[9] << '\n'
           << "\n\n\n";

  // Simple sorting.
  BubbleSort(stringB);
  std::cout << "sorted: " << stringB << '\n';

  std::cout << "2 dimensional array of char\n"
       << "===========================\n";
  SArray1dC< SArray1dC<char> > aa(4);
  
  for(SArray1dIterC<SArray1dC<char> > it(aa);it;it++)
    *it = stringA.Copy();
  
  aa[2][9]   = '5';              // Change the item
  stringA[2] = '2';
  aa[1]      = stringA;          // Change the array
  
  std::cout << "aa: " << aa << "\n\n\n";  
  std::cout << "End of the example\n";
  return 0;
}

