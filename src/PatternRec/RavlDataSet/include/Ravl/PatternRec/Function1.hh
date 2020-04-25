// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FUNCTION1_HEADER
#define RAVL_FUNCTION1_HEADER 1
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! author="Charles Galambos"
//! docentry="Ravl.API.Pattern Recognition.Functions"
//! file="Ravl/PatternRec/DataSet/Function1.hh"

#include "Ravl/PatternRec/Function.hh"

namespace RavlN {

  //! userlevel=Develop
  //: Map a vector to a single real value.
  
  class Function1BodyC 
    : public FunctionBodyC
  {
  public:
    Function1BodyC()
    { outputSize = 1; }
    //: Default constructor.

    Function1BodyC(const XMLFactoryContextC &factory);
    //: Construct from XML factory

    Function1BodyC(std::istream &strm);
    //: Load from stream.
    
    Function1BodyC(BinIStreamC &strm);
    //: Load from binary stream.

    virtual MatrixC Jacobian(const VectorC &X) const;
    //: Compute the jacobian using jacobian1

    virtual VectorC Jacobian1(const VectorC &X) const;
    //: Calculate Jacobian at X for single valued function
    // The default implementation performs numerical estimation of the Jacobian using differences. This
    // function has and should be overloaded for all cases where the Jacobian
    // can be calculated analytically.
    
    virtual MatrixC Hessian(const VectorC &X) const;
    //: Obtain the hessian of the function at X
    // The default implementation performs numerical estimation of the Jacobian using differences. The
    // default method does not provide a good approximation.
    
    virtual bool EvaluateValueJacobianHessian(const VectorC &X,RealT &value,VectorC &jacobian,MatrixC &hessian) const;
    //: Evaluate the value,jacobian, and hessian of the function at point X
    // Returns true if all values are provide, false if one or more is approximated.
    
    virtual bool Save (std::ostream &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual VectorC Apply(const VectorC &data) const;
    //: Apply function to 'data'
    
    virtual RealT Apply1(const VectorC &data) const;
    //: Apply function to 'data'
    
    virtual float Apply1(const TVectorC<float> &data) const;
    //: Apply to a vector of floats.

    typedef SmartPtrC<Function1BodyC> RefT;
    //: Smart ptr to function
  };

  //! userlevel=Normal
  //: Map a vector to a single real value.

  class Function1C 
    : public FunctionC
  {
  public:
    Function1C()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    Function1C(std::istream &strm);
    //: Load from stream.
    
    Function1C(BinIStreamC &strm);
    //: Load from binary stream.

    Function1C(const FunctionC &base)
      : FunctionC(dynamic_cast<const FunctionBodyC *>(BodyPtr(base)))
    {}
    //: Base class constructor.
    // If class is not of an appropriate type an invalid handle will be created.
    
  protected:
    Function1C(Function1BodyC &bod)
      : FunctionC(bod)
    {}
    //: Body constructor.

    Function1C(const Function1BodyC *bod)
      : FunctionC(bod)
    {}
    //: Body ptr constructor.
    
    Function1BodyC &Body()
    { return static_cast<Function1BodyC &>(FunctionC::Body()); }
    //: Access body.

    const Function1BodyC &Body() const
    { return static_cast<const Function1BodyC &>(FunctionC::Body()); }
    //: Access body.
    
  public:

    MatrixC Hessian(const VectorC &X) const
    { return Body().Hessian(X); }
    //: Obtain the hessian of the function at X
    // The default implementation performs numerical estimation of the Jacobian using differences. The
    // default method does not provide a good approximation.
    
    bool EvaluateValueJacobianHessian(const VectorC &X,RealT &value,VectorC &jacobian,MatrixC &hessian) const
    { return Body().EvaluateValueJacobianHessian(X,value,jacobian,hessian); }
    //: Evaluate the value,jacobian, and hessian of the function at point X
    // Returns true if all values are provide, false if one or more is approximated.
    
    VectorC Jacobian1(const VectorC &X) const
    { return Body().Jacobian1(X); }
    //: Calculate Jacobian at X
    // The default implementation performs numerical estimation of the Jacobian using differences. This
    // function has and should be overloaded for all cases where the Jacobian
    // can be calculated analytically.
    
    RealT Apply1(const VectorC &data) const
    { return Body().Apply1(data); }
    //: Apply function to 'data'
    
    float Apply1(const TVectorC<float> &data) const
    { return Body().Apply1(data); }
    //: Apply to a vector of floats.

  };
  
  inline std::istream &operator>>(std::istream &strm,Function1C &obj) {
    obj = Function1C(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline std::ostream &operator<<(std::ostream &out,const Function1C &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,Function1C &obj) {
    obj = Function1C(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const Function1C &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
}


#endif
