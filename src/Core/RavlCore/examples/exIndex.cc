// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Base/exIndex.cc"
//! docentry="Ravl.API.Core.Indexing"

#include "Ravl/EntryPnt.hh"
#include "Ravl/Index.hh"
#include "Ravl/Stream.hh"

using namespace RavlN;

// Class AccessC enables to use class IndexC as normal index for 
// arrays of objects which do not conform IndexC interface. Therefore
// it is not necessary to call the member function IndexC::V().
template <class DataC>
struct AccessC
{
  AccessC(DataC * p)              :ptr(p) {}
  DataC & operator[](IndexC & i)  { return ptr[i.V()];}
  DataC * ptr;
};

int exIndex(int, char * [] )
{
  std::cout << "Index access example\n"
       << "====================\n";
  
  char buffer[] = "absdefghijklmnopqrstuvwxyz\n";
  AccessC<char> acc(buffer);
  UIntT mi = 2;  // a machine oriented index
  IndexC  i  = mi; // an index with a proper arithmetic operations
  std::cout << "Buffer[2]:    " << buffer[mi] << " == " << acc[i] << '\n'; 
  std::cout << "End of the example\n";
  return 0;
}

RAVL_ENTRY_POINT(exIndex)




