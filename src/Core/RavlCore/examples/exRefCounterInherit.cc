// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCore
//! author="Charles Galambos"
//! file="Ravl/Core/Base/exRefCounterInherit.cc"
//! docentry="Ravl.API.Core.Reference Counting"
//! userlevel=Normal

#include "Ravl/RefCounter.hh"
#include "Ravl/Stream.hh"

using namespace RavlN;

//! userlevel=Develop
//: Example body base class.

class BaseObjectBodyC
  : public RCBodyVC
{
public:
  BaseObjectBodyC(int a)
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
//: Example handle base class.

class BaseObjectC
  : public RCHandleC<BaseObjectBodyC>
{
public:
  BaseObjectC(int a)
    : RCHandleC<BaseObjectBodyC>(*new BaseObjectBodyC(a))
  {}
  //: Construct an object with value a.
  
protected:
  BaseObjectC(BaseObjectBodyC &bod)
    : RCHandleC<BaseObjectBodyC>(bod)
  {}
  //: body constructor.
  
  BaseObjectBodyC &Body()
  { return RCHandleC<BaseObjectBodyC>::Body(); }
  //: Access body.
  // Not strictly needed, but make writting child objects easier.
  
  const BaseObjectBodyC &Body() const
  { return RCHandleC<BaseObjectBodyC>::Body(); }
  //: Access body.
  // Not strictly needed, but make writting child objects easier.
  
public:
  int Data()
  { return Body().Data(); }
  //: Access value from body.

  void SetData(int x)
  { Body().SetData(x); }
  //: Set member data.
  
};

//////////////////////////////////////////////////////////////////////////////////////

//! userlevel=Develop
//: Example body derived class.

class ChildObjectBodyC
  : public BaseObjectBodyC
{
public:
  ChildObjectBodyC(int a,int b)
    : BaseObjectBodyC(a),
      someOtherData(b)
  {}
  //: Constructor.
  
  int OtherData()
  { return someOtherData; }
  //: Access data.
  
  void SetOtherData(int x)
  { someOtherData = x; }
  //: Set member data.
  
protected:
  int someOtherData;
};
  
//! userlevel=Normal
//: Example handle derived class.

class ChildObjectC
  : public BaseObjectC
{
public:
  ChildObjectC(int a,int b)
    : BaseObjectC(*new ChildObjectBodyC(a,b))
  {}
  //: Construct an object with value a.
  
protected:
  ChildObjectBodyC &Body()
  { return static_cast<ChildObjectBodyC &>(BaseObjectC::Body()); }
  //: Access body.
  
  const ChildObjectBodyC &Body() const
  { return static_cast<const ChildObjectBodyC &>(BaseObjectC::Body()); }
  //: Access body.
public:
  int OtherData()
  { return Body().OtherData(); }
  //: Access value from body.
  
  void SetOtherData(int x)
  { Body().SetOtherData(x); }
  //: Set member data.
};

//////////////////////////////////////////////////////////////////////////////////////

int main() {  
  // Create an object
  ChildObjectC anObj(1,2);

  // Make a second handle to the object.
  ChildObjectC secondHandle = anObj;
  
  // Change the value held in the object
  anObj.SetData(2);
  
  // This will print out the value 2, as both 'anObj' and 'secondHandle' point to the same object.
  std::cout << "Data=" << secondHandle.Data() << "\n";
  
  return 0;
}
