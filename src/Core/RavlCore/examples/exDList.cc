// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Container/DList/exDList.cc"
//! userlevel=Normal
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Lists"

//: Simple example of DList's.

#include "Ravl/DList.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/Stream.hh"

using namespace RavlN;

static bool IntGrEq(const int &f1, const int &f2)
{ return f1 >= f2; }

int main() {

  DListC<int> aList;
  
  // Put the numbers 1,2,3 into the list in a roundabout way.
  
  aList.InsFirst(2);
  aList.InsFirst(1);
  aList.InsLast(3);
  aList.InsFirst(4);

  // To prove it go through the list with an iterator 
  // and print out elements.
  
  std::cout << "List:";
  for(DLIterC<int> it(aList);it;it++)
    std::cout << " " << *it;
  std::cout << "\n";

  // To sort the list into ascending order, use the sort method with default
  // comparison function:
  aList.MergeSort();
  std::cout << "List:";
  for(DLIterC<int> it(aList);it;it++)
    std::cout << " " << *it;
  std::cout << "\n";

  // To sort the list into *descending* order, you have to define your own
  // comparison function (see IntGrEq() above), and use it like this:
  aList.MergeSort(IntGrEq);
  std::cout << "List:";
  for(DLIterC<int> it(aList);it;it++)
    std::cout << " " << *it;
  std::cout << "\n";

  
  return 0;
}
