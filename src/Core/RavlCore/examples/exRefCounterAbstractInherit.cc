// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id: exRefCounterInherit.cc 5240 2005-12-06 17:16:50Z plugger $"
//! lib=RavlCore
//! author="Charles Galambos; Bill Christmas"
//! file="Ravl/Core/Base/exRefCounterInherit.cc"
//! docentry="Ravl.API.Core.Reference Counting"
//! userlevel=Normal

#include "Ravl/RefCounter.hh"
#include "Ravl/Stream.hh"



//! userlevel=Develop
//: Example abstract body base class.

class BaseObjectBodyC
  : public RavlN::RCBodyVC
{
public:
  BaseObjectBodyC(int a)
    : someData(a)
  {}
  //: Constructor.
  
  int Data()
  { return someData; }
  //: Access data.
  
  virtual int OtherData()=0;

  void SetData(int x)
  { someData = x; }
  //: Set member data.
  
protected:
  int someData;

};
  
//! userlevel=Normal
//: Example handle base class.
// Note that handle base class is *not* abstract.

class BaseObjectC
  : public RavlN::RCHandleC<BaseObjectBodyC>
{
public:
  BaseObjectC()
  {}
  //: Construct an invalid handle.
  
protected:
  BaseObjectC(BaseObjectBodyC &bod)
    : RavlN::RCHandleC<BaseObjectBodyC>(bod)
  {}
  //: body constructor.
  
  BaseObjectBodyC &Body()
  { return RavlN::RCHandleC<BaseObjectBodyC>::Body(); }
  //: Access body.
  // Not strictly needed, but make writting child objects easier.
  
  const BaseObjectBodyC &Body() const
  { return RavlN::RCHandleC<BaseObjectBodyC>::Body(); }
  //: Access body.
  // Not strictly needed, but make writting child objects easier.
  
public:
  int Data()
  { return Body().Data(); }
  //: Access value from body.
  
  int OtherData()
  { return Body().OtherData(); }
  
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
