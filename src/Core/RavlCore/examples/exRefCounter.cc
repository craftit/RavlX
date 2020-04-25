// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCore
//! author="Charles Galambos"
//! file="Ravl/Core/Base/exRefCounter.cc"
//! docentry="Ravl.API.Core.Reference Counting"
//! userlevel=Normal

#include "Ravl/RefCounter.hh"
#include "Ravl/Stream.hh"

//! userlevel=Develop
//: Example body class.

class AnObjectBodyC
  : public RavlN::RCBodyC
{
public:
  AnObjectBodyC(int a)
    : someData(a)
  {}
  //: Constructor.
  
  int Data()
  { return someData; }
  //: Access data.
  
  void SetData(int x)
  { someData = x; }
  //: Set member data.
  
protected:
  int someData;
};
  
//! userlevel=Normal
//: Example handle class.

class AnObjectC
  : public RavlN::RCHandleC<AnObjectBodyC>
{
public:
  AnObjectC(int a)
    : RavlN::RCHandleC<AnObjectBodyC>(*new AnObjectBodyC(a))
  {}
  //: Construct an object with value a.
  
  int Data()
  { return Body().Data(); }
  //: Access value from body.

  void SetData(int x)
  { Body().SetData(x); }
  //: Set member data.
  
};


int main() {
  // Create an object
  AnObjectC anObj(1);

  // Make a second handle to the object.
  AnObjectC secondHandle = anObj;
  
  // Change the value held in the object
  anObj.SetData(2);
  
  // This will print out the value 2, as both 'anObj' and 'secondHandle' point to the same object.
  std::cout << "Data=" << secondHandle.Data() << "\n";
  
  return 0;
}
