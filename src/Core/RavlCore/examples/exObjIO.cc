// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCore
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.IO.Streams"
//! userlevel=Normal
//! file="Ravl/Core/System/exObjIO.cc"

#include "Ravl/PointerManager.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/StrStream.hh"
#include "Ravl/DList.hh"

using namespace RavlN;

int main() {
  StrOStreamC os;
  {
    // Create some data to save.
    
    // Pointer to a value.
    int x = 2;
    int *val1 = &x;
    int *val2 = &x;
    
    // A list of numbers.
    
    DListC<int> list1;
    list1.InsLast(1);
    list1.InsLast(2);
    list1.InsLast(3);
    DListC<int> list2 = list1;
    
    // Construct a binary output stream.
    BinOStreamC bos(os);
    
    // Save data.
    bos << ObjIO(val1) << ObjIO(val2) << ObjIO(list1) << ObjIO(list2);
  }
  
  {
    // Create an input stream.
    
    StringC data = os.String();
    StrIStreamC is(data);
    BinIStreamC bis(is);
    
    // Some variables.
    int *valx = 0;
    int *valy = 0;
    
    DListC<int> list1;
    DListC<int> list2;
    
    // Load data
    
    bis >> ObjIO(valx) >> ObjIO(valy) >> ObjIO(list1) >> ObjIO(list2);
    
    // valx and valy will pointer to the same memory location containing
    // the value 2.  The space is allocated with new, and so must be deleted.
    // list1 and list2 will point to the same DListC as when saved.
    
    std::cout << "Valx=" << *valx << " Valy=" << *valy << "\n";
    
    // Free space allocated in load.
    
    delete valx;
  }
  
  return 0;
}
