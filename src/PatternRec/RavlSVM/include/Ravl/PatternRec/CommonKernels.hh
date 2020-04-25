// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here

//file contains common for SVM kernels
#ifndef RAVL_COMMONKERNELS_HEADER
#define RAVL_COMMONKERNELS_HEADER 1

//////////////////////////////////////////////////////////////////////////////
//! file =      "Ravl/PatternRec/Svm/CommonKernels.hh"
//! author =    "Alexey Kostin"
//! lib=RavlSVM
//! date =      "3/07/03"
//! rcsid =     "$Id: CommonKernels.hh 12208 2008-02-07 12:54:07Z alex $"
//! docentry =  "Ravl.API.Pattern_Recognition.Classifier.SVM.Kernel functions"
//////////////////////////////////////////////////////////////////////////////

#include "Ravl/PatternRec/KernelFunc.hh"

namespace RavlN
{
using namespace RavlN;
//------------------------------------------------------------------------------
//! userlevel = Develop
//: Linear kernel function

class LinearKernelBodyC : public KernelFunctionBodyC
{
public:
  LinearKernelBodyC(RealT Scale) : KernelFunctionBodyC()
    { scale = Scale; }
  //: Constructor.
  //!param: Scale - result of inner product of two vectors is multiplied by the number.

  LinearKernelBodyC(const XMLFactoryContextC &factory);
  //: Construct from XML factory
  //!param scale result of inner products is multiplied by this scale

  LinearKernelBodyC(istream &strm);
  //: Load from stream.

  LinearKernelBodyC(BinIStreamC &strm);
  //: Load from binary stream.

  virtual bool Save(ostream &out) const;
  //: Writes object to stream

  virtual bool Save(BinOStreamC &out) const;
  //: Writes object to stream

  virtual RealT Apply(int Size, const RealT *X1, const RealT *X2) const;
  //: Calculates kernel functions for two vectors X1 and X2

  RealT Scale() const
    { return scale; }
private:
  RealT scale;
  //: Multiplier for result of inner product
};

//! userlevel = Normal
//: Linear kernel function
class LinearKernelC : public KernelFunctionC
{
public:
  LinearKernelC(){};
  //: Default constructor

  LinearKernelC(RealT Scale)
    : KernelFunctionC(*new LinearKernelBodyC(Scale))
  {}
  //: Constructor
  // K(X1, X2) = (X1 . X2) * Scale
  // <p>Usually Scale is equal to 1, but in some cases when inner product
  // is very big number or very small number it is necessary to scale it,
  // so classifier will not have overflow problems.

  LinearKernelC(const XMLFactoryContextC &factory)
    :  KernelFunctionC(*new LinearKernelBodyC(factory))
  {}
  //: Construct from XML factory
  //!param scale result of inner products is multiplied by this scale
  
  LinearKernelC(istream &strm);
  //: Load from stream.

  LinearKernelC(BinIStreamC &strm);
  //: Load from binary stream.

  RealT Scale() const
    { return Body().Scale(); }

  LinearKernelBodyC &Body()
    { return static_cast<LinearKernelBodyC &>(KernelFunctionC::Body()); }
  //: Access body.

  const LinearKernelBodyC &Body() const
    { return static_cast<const LinearKernelBodyC &>(KernelFunctionC::Body()); }
  //: Access body.
protected:
};

//------------------------------------------------------------------------------
//! userlevel = Develop
//: Quadratic kernel function

class QuadraticKernelBodyC : public KernelFunctionBodyC
{
public:
  QuadraticKernelBodyC(RealT Scale) : KernelFunctionBodyC()
    { scale = Scale; }
  //: Constructor.
  //!param: Scale - result of inner product of two vectors is multiplied by the number.

  QuadraticKernelBodyC(const XMLFactoryContextC &factory);
  //: Construct from XML factory
  //!param scale result of inner products is multiplied by this scale

  QuadraticKernelBodyC(istream &strm);
  //: Load from stream.

  QuadraticKernelBodyC(BinIStreamC &strm);
  //: Load from binary stream.

  virtual bool Save(ostream &out) const;
  //: Writes object to stream

  virtual bool Save(BinOStreamC &out) const;
  //: Writes object to stream

  virtual RealT Apply(int Size, const RealT *X1, const RealT *X2) const;
  //: Calculates kernel functions for two vectors X1 and X2

  RealT Scale() const
    { return scale; }
private:
  RealT scale;
  //: Multiplier for result of inner product
};

//! userlevel = Normal
//: Quadratic kernel function
class QuadraticKernelC : public KernelFunctionC
{
  public:
  QuadraticKernelC(){};
  //: Default constructor

  QuadraticKernelC(RealT Scale)
    : KernelFunctionC(*new QuadraticKernelBodyC(Scale))
    {}
  //: Constructor
  // K(X1, X2) = (X1 . X2 + X1^2 . X2^2) * Scale
  // <p>Usually Scale is equal to 1, but in some cases when inner product
  // is very big number or very small number it is necessary to scale it,
  // so classifier will not have overflow problems.

  QuadraticKernelC(const XMLFactoryContextC &factory)
    :  KernelFunctionC(*new QuadraticKernelBodyC(factory))
  {}
  //: Construct from XML factory
  //!param scale result of inner products is multiplied by this scale

  QuadraticKernelC(istream &strm);
  //: Load from stream.

  QuadraticKernelC(BinIStreamC &strm);
  //: Load from binary stream.

  
  RealT Scale() const
    { return Body().Scale(); }
  QuadraticKernelBodyC &Body()
    { return static_cast<QuadraticKernelBodyC &>(KernelFunctionC::Body()); }
  //: Access body.

  const QuadraticKernelBodyC &Body() const
    { return static_cast<const QuadraticKernelBodyC &>(KernelFunctionC::Body()); }
  //: Access body.
protected:
};

//------------------------------------------------------------------------------
//! userlevel = Develop
//: Polynomial kernel function
class PolynomialKernelBodyC : public KernelFunctionBodyC
{
public:
  PolynomialKernelBodyC(RealT Power, RealT Scale, RealT B) : KernelFunctionBodyC()
    { power = Power; scale = Scale; b = B; }
  //: Constructor.

  PolynomialKernelBodyC(const XMLFactoryContextC &factory);
  //: Construct from XML factory
  //!param power 
  //!param scale result of inner products is multiplied by this scale
  //!param b
  
  PolynomialKernelBodyC(istream &strm);
  //: Load from stream.

  PolynomialKernelBodyC(BinIStreamC &strm);
  //: Load from binary stream.

  virtual bool Save(ostream &out) const;
  //: Writes object to stream

  virtual bool Save(BinOStreamC &out) const;
  //: Writes object to stream

  virtual RealT Apply(int Size, const RealT *X1, const RealT *X2) const;
  //: Calculates kernel functions for two vectors X1 and X2
private:
  RealT power, scale, b;
};

//! userlevel = Normal
//: Polynomial kernel function
class PolynomialKernelC : public KernelFunctionC
{
public:
  PolynomialKernelC(){};
  //: Default constructor
  
  PolynomialKernelC(RealT Power, RealT Scale, RealT B)
    : KernelFunctionC(*new PolynomialKernelBodyC(Power, Scale, B))
  {}
  //: Constructor
  // K(X1, X2) = {(X1 . X2) * Scale + B} ^ Power
  // <p>Power can be non-integer number.
  
  PolynomialKernelC(const XMLFactoryContextC &factory)
    :  KernelFunctionC(*new PolynomialKernelBodyC(factory))
  {}
  //: Construct from XML factory
  //!param power 
  //!param scale result of inner products is multiplied by this scale
  //!param b
  // K(X1, X2) = {(X1 . X2) * Scale + B} ^ Power

  PolynomialKernelC(istream &strm);
  //: Load from stream.

  PolynomialKernelC(BinIStreamC &strm);
  //: Load from binary stream.

  PolynomialKernelBodyC &Body()
    { return static_cast<PolynomialKernelBodyC &>(KernelFunctionC::Body()); }
  //: Access body.

  const PolynomialKernelBodyC &Body() const
    { return static_cast<const PolynomialKernelBodyC &>(KernelFunctionC::Body()); }
  //: Access body.
protected:
};

//------------------------------------------------------------------------------
//! userlevel = Develop
//: Radial basis kernel function
class RBFKernelBodyC : public KernelFunctionBodyC
{
public:
  RBFKernelBodyC(RealT Gamma) : KernelFunctionBodyC()
    { gamma = Gamma; }
  //: Constructor.

  RBFKernelBodyC(const XMLFactoryContextC &factory);
  //: Construct from XML factory
  //!param gamma

  RBFKernelBodyC(istream &strm);
  //: Load from stream.

  RBFKernelBodyC(BinIStreamC &strm);
  //: Load from binary stream.

  virtual bool Save(ostream &out) const;
  //: Writes object to stream

  virtual bool Save(BinOStreamC &out) const;
  //: Writes object to stream

  virtual RealT Apply(int Size, const RealT *X1, const RealT *X2) const;
  //: Calculates kernel functions for two vectors X1 and X2
private:
  RealT gamma;
};

//! userlevel = Normal
//: Radial basis kernel function
class RBFKernelC : public KernelFunctionC
{
public:
  RBFKernelC(){};
  //: Default constructor

  RBFKernelC(RealT Gamma)
    : KernelFunctionC(*new RBFKernelBodyC(Gamma))
  {}
  //: Constructor
  // K(X1, X2) = exp( - ||X1 - X2|| ^ 2 / Gamma)

  RBFKernelC(const XMLFactoryContextC &factory)
    :  KernelFunctionC(*new RBFKernelBodyC(factory))
  {}
  //: Construct from XML factory
  //!param gamma 

  RBFKernelC(istream &strm);
  //: Load from stream.

  RBFKernelC(BinIStreamC &strm);
  //: Load from binary stream.

  RBFKernelBodyC &Body()
    { return static_cast<RBFKernelBodyC &>(KernelFunctionC::Body()); }
  //: Access body.

  const RBFKernelBodyC &Body() const
    { return static_cast<const RBFKernelBodyC &>(KernelFunctionC::Body()); }
  //: Access body.
protected:
};

//------------------------------------------------------------------------------
//! userlevel = Develop
//: Chi^2 kernel function
class Chi2KernelBodyC : public KernelFunctionBodyC
{
public:
  Chi2KernelBodyC(RealT Gamma) : KernelFunctionBodyC()
    { gamma = Gamma; }
  //: Constructor.

  Chi2KernelBodyC(const XMLFactoryContextC &factory);
  //: Construct from XML factory
  //!param gamma

  Chi2KernelBodyC(istream &strm);
  //: Load from stream.

  Chi2KernelBodyC(BinIStreamC &strm);
  //: Load from binary stream.

  virtual bool Save(ostream &out) const;
  //: Writes object to stream

  virtual bool Save(BinOStreamC &out) const;
  //: Writes object to stream

  virtual RealT Apply(int Size, const RealT *X1, const RealT *X2) const;
  //: Calculates kernel functions for two vectors X1 and X2

private:
  RealT gamma;
};

//! userlevel = Normal
//: Chi^2 kernel function
// This kernel is typically used in conjunction with histogram values.  Hence it does not expect vector values to be negative; if negative values are present, the kernel will return a value of 0.0.


class Chi2KernelC : public KernelFunctionC
{
public:
  Chi2KernelC(){};
  //: Default constructor

  Chi2KernelC(RealT Gamma)
    : KernelFunctionC(*new Chi2KernelBodyC(Gamma))
  {}
  //: Constructor
  // K(X1, X2) = exp( - D(X1, X2) / Gamma), 
  // where D(X1,X2) = 0.5 \sum_{k=1}^{K} ((x1[k] - x2[k])^2)/(x1[k] + x2[k])
  // Gamma should be estimated externally by processing the 
  // whole training set: Gamma = (1/N^2) \sum_{i,j}^N D(Xi,Xj), 
  // where N is the training set size

  Chi2KernelC(const XMLFactoryContextC &factory)
    :  KernelFunctionC(*new Chi2KernelBodyC(factory))
  {}
  //: Construct from XML factory
  //!param gamma 

  Chi2KernelC(istream &strm);
  //: Load from stream.

  Chi2KernelC(BinIStreamC &strm);
  //: Load from binary stream.

  Chi2KernelBodyC &Body()
    { return static_cast<Chi2KernelBodyC &>(KernelFunctionC::Body()); }
  //: Access body.

  const Chi2KernelBodyC &Body() const
    { return static_cast<const Chi2KernelBodyC &>(KernelFunctionC::Body()); }
  //: Access body.
protected:
};

} // end of namespace Ravl
#endif

