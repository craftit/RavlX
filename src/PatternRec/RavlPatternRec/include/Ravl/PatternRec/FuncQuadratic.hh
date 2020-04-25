// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FUNCQUADRATIC_HEADER
#define RAVL_FUNCQUADRATIC_HEADER 1
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! author="Robert Crida and Charles Galambos"
//! docentry="Ravl.API.Pattern Recognition.Numerical Modeling"
//! file="Ravl/PatternRec/Modeling/Basic/FuncQuadratic.hh"

#include "Ravl/PatternRec/FuncLinearCoeff.hh"
#include "Ravl/Matrix.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Quadratic function.
  
  class FuncQuadraticBodyC 
    : public FuncLinearCoeffBodyC
  {
  public:
    FuncQuadraticBodyC(int inSize,int outSize = -1);
    //: Constructor.
    //: Setting output size to 0 generates an output for each coefficient.
    
    FuncQuadraticBodyC(std::istream &strm);
    //: Load from stream.
    
    FuncQuadraticBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (std::ostream &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual VectorC MakeInput (const VectorC &X) const;
    //: Expand vector to linear coefficients.
    
    virtual VectorC MakeJacobianInput (const VectorC &X, IntT i) const;
    //: Expand vector to linear coefficients.
    
    virtual UIntT NumberCoeffs(UIntT inputSize) const;
    //: Calculate the number of coefficients for a given input size.
    
    virtual void Describe(std::ostream &out);
    //: Write a human readable text description of the function.
    
  protected:
    
    UIntT noCoeffs; // Number of coefficients.
  };
  
  //! userlevel=Normal
  //: Quadratic function.
  //
  // It provides a concrete class implementation for functions of the
  // form Y=f(X) where each element of Y is an 2nd polynomial of the elements
  // of X including all the cross parameter terms.
  
  class FuncQuadraticC
    : public FuncLinearCoeffC
  {
  public:    
    FuncQuadraticC(int inSize,int outSize = -1)
      : FuncLinearCoeffC(*new FuncQuadraticBodyC(inSize,outSize))
    {}
    //: Constructor.
    // Setting the number of outputs to -1 creates a function which outputs a value for each coefficient.
    
    FuncQuadraticC()
    {}
    //: Default constructor
    // creates an invalid handle.
    
    FuncQuadraticC(std::istream &strm);
    //: Load from stream.
    
    FuncQuadraticC(BinIStreamC &strm);
    //: Load from binary stream.
    
  protected:
    FuncQuadraticC(FuncQuadraticBodyC &bod)
      : FuncLinearCoeffC(bod)
    {}
    //: Body constructor.

    FuncQuadraticC(FuncQuadraticBodyC *bod)
      : FuncLinearCoeffC(bod)
    {}
    //: Body ptr constructor.
    
    FuncQuadraticBodyC &Body()
    { return static_cast<FuncQuadraticBodyC &>(FunctionC::Body()); }
    //: Access body.
    
    const FuncQuadraticBodyC &Body() const
    { return static_cast<const FuncQuadraticBodyC &>(FunctionC::Body()); }
    //: Access body.
    
  public:
    
  };
  
  inline std::istream &operator>>(std::istream &strm,FuncQuadraticC &obj) {
    obj = FuncQuadraticC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline std::ostream &operator<<(std::ostream &out,const FuncQuadraticC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,FuncQuadraticC &obj) {
    obj = FuncQuadraticC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const FuncQuadraticC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
}

#endif
