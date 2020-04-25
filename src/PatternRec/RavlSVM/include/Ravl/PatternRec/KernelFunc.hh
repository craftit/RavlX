// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here

// Map two input vectors to a single real value.
#ifndef RAVL_KERNELFUNCTION_HEADER
#define RAVL_KERNELFUNCTION_HEADER 1

//////////////////////////////////////////////////////////////////////////////
//! file =      "Ravl/PatternRec/Svm/KernelFunc.hh"
//! author =    "Alexey Kostin"
//! lib=RavlSVM
//! date =      "3/07/03"
//! rcsid =     "$Id: KernelFunc.hh 7752 2006-01-31 14:46:56Z alex $"
//! docentry =  "Ravl.API.Pattern_Recognition.Classifier.SVM.Kernel functions"
//////////////////////////////////////////////////////////////////////////////

#include "Ravl/SArray1d.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/XMLFactory.hh"

// KernelFunctionBodyC ment to be pure abstract class but ideology of Ravl
// does not allow convenient use of abstract classes. All functions of the class
// have implementation and cause RavlAssertMsg which stops program at runtime

namespace RavlN
{

//! userlevel = Develop
//: Base class for kernel functions

class KernelFunctionBodyC : public RCBodyVC
{
public:
  KernelFunctionBodyC(){};
  //: Default constructor.
  // Creates wrong object.

  KernelFunctionBodyC(const XMLFactoryContextC & factory);
  //: XML Factory contructor
  
  KernelFunctionBodyC(istream &strm);
  //: Load from stream constructor.
  // The constructor present in the class only for compatibility with
  // the virtual construction of classes mechanism in Ravl.

  KernelFunctionBodyC(BinIStreamC &strm);
  //: Load from binary stream.
  // The constructor present in the class only for compatibility with
  // the virtual construction of classes mechanism in Ravl.

  virtual bool Save(ostream &out) const;
  //: Writes object to stream
  // Virtual function should never be called for base class.

  virtual bool Save(BinOStreamC &out) const;
  //: Writes object to stream
  // Virtual function should never be called for base class.

  RealT Apply(const SArray1dC<RealT> &X1, const SArray1dC<RealT> &X2) const
    { return Apply(X1.Size(), X1.ReferenceElm(), X2.ReferenceElm()); }
  //: Calculates kernel functions for two vectors X1 and X2
  // Virtual function should never be called for base class.
  
  virtual RealT Apply(int Size, const RealT *X1, const RealT *X2) const;
  //: Calculates kernel functions for two vectors X1 and X2
  // Virtual function should never be called for base class.
};

//! userlevel = Normal
//: Base class for kernel functions
class KernelFunctionC : public RCHandleVC<KernelFunctionBodyC>
{
public:
  KernelFunctionC(){};
  //: Default constructor.
  // Creates wrong object. If used then object created by the constructor
  // should only be lvalue.

  KernelFunctionC(const XMLFactoryContextC & factory)
    : RCHandleVC<KernelFunctionBodyC>(*new KernelFunctionBodyC(factory))
  {}
  //: XML Factory constructor

  KernelFunctionC(istream &strm);
  //: Load from stream.
  // The constructor present in the class only for compatibility with
  // the virtual construction of classes mechanism in Ravl.

  KernelFunctionC(BinIStreamC &strm);
  //: Load from binary stream.
  // The constructor present in the class only for compatibility with
  // the virtual construction of classes mechanism in Ravl.

  RealT operator()(const SArray1dC<RealT> &X1, const SArray1dC<RealT> &X2) const
    { return Body().Apply(X1, X2); }
  //: Calculates kernel functions for two vectors X1 and X2
  // The function assumes that X1 and X2 are of the same size.
  
  RealT Apply(int Size, const RealT *X1, const RealT *X2) const
    { return Body().Apply(Size, X1, X2); }
  //: Calculates kernel functions for two vectors X1 and X2
  // The function assumes that X1 and X2 are of the same size.
  
  KernelFunctionBodyC &Body()
    { return RCHandleC<KernelFunctionBodyC>::Body(); }
  //: Access body.

  const KernelFunctionBodyC &Body() const
    { return RCHandleC<KernelFunctionBodyC>::Body(); }
  //: Access body.

protected:
  KernelFunctionC(KernelFunctionBodyC &Bod)
    : RCHandleVC<KernelFunctionBodyC>(Bod)
    {}
  //: body constructor.

  KernelFunctionC(KernelFunctionBodyC *Bod)
    : RCHandleVC<KernelFunctionBodyC>(Bod)
    {}
  //: body constructor.
  // Will be used in child classes during virtual construction.
};

//! userlevel = Normal
//:--------------------------------------------------------------------------
//: IO stream functions
//:-
//:inline ostream &operator<<(ostream &out, KernelFunctionC &obj)
//:-
//:inline BinOStreamC &operator<<(BinOStreamC &out, KernelFunctionC &obj)
//:-
//:inline ostream &operator<<(ostream &out, const KernelFunctionC &obj)
//:-
//:inline BinOStreamC &operator<<(BinOStreamC &out,const KernelFunctionC &obj)

//! userlevel = Develop
//:-
//: Note to IO stream functions
// Stream read operators defined in RAVL_INITVIRTUALCONSTRUCTOR_FULL macro.
// Implementation of 'load from stream' constructors defined there as well.
// Stream write operators are defined in RCHandleVC
// and call virtual Save function of body class.

} //end of namespace

#endif

