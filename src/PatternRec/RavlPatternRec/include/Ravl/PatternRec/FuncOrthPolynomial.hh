// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_ORTHPOLYNOMIAL_HEADER
#define RAVL_ORTHPOLYNOMIAL_HEADER 1
//! rcsid="$Id$"
//! date="21/4/1998"
//! author="Robert Crida and Charles Galambos"
//! docentry="Ravl.API.Pattern Recognition.Numerical Modeling"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Modeling/Basic/FuncOrthPolynomial.hh"

#include "Ravl/PatternRec/FuncLinearCoeff.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Orthogonal Polynomial function.
  // See handle for more information.
  
  class FuncOrthPolynomialBodyC 
    : public FuncLinearCoeffBodyC
  {
  public:
    FuncOrthPolynomialBodyC(int inSize,int outSize,UIntT order);
    //: Construct an orthogonal polynomial of given 'order'.
    
    FuncOrthPolynomialBodyC(std::istream &strm);
    //: Load from stream.
    
    FuncOrthPolynomialBodyC(BinIStreamC &strm);
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
    
  protected:    
    UIntT noCoeffs; // Number of coefficients.
    UIntT order;    // Order of polynomial
  };
  
  //! userlevel=Normal
  //: Orthogonal Polynomial function.
  
  // It provides a class implementation functions of the form Y=f(X)
  // where each element of Y is an k'th polynomial of the elements of X.<p>
  //
  // For example, when a second order polynomial is selected and X and Y have
  // 2 and 3 elements respectively and A is the matrix of model weights then:<p>
  // <pre>
  // Y[0] = A[0][0] + A[0][1]X[0] + A[0][2]X[0]^2 + A[0][3]X[1] + A[0][4]X[1]^2
  // Y[1] = A[1][0] + A[1][1]X[0] + A[1][2]X[0]^2 + A[1][3]X[1] + A[1][4]X[1]^2
  // Y[2] = A[2][0] + A[2][1]X[0] + A[2][2]X[0]^2 + A[2][3]X[1] + A[2][4]X[1]^2
  // </pre>
  // Note that in this model there are no cross parameter terms regardless of
  // the model order selected, ie each term is only a power of an individual
  // element of X.
  
  class FuncOrthPolynomialC
    : public FuncLinearCoeffC
  {
  public:
    FuncOrthPolynomialC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    FuncOrthPolynomialC(std::istream &strm);
    //: Load from stream.
    
    FuncOrthPolynomialC(BinIStreamC &strm);
    //: Load from binary stream.
    
    FuncOrthPolynomialC(int inSize,int outSize,UIntT order)
      : FuncLinearCoeffC(*new FuncOrthPolynomialBodyC(inSize,outSize,order))
    {}
    //: Construct an orthogonal polynomial of given 'order'.
    
  protected:
    FuncOrthPolynomialC(FuncOrthPolynomialBodyC &bod)
      : FuncLinearCoeffC(bod)
    {}
    //: Body constructor.
    
    FuncOrthPolynomialC(FuncOrthPolynomialBodyC *bod)
      : FuncLinearCoeffC(bod)
    {}
    //: Body ptr constructor.
    
    FuncOrthPolynomialBodyC &Body()
    { return static_cast<FuncOrthPolynomialBodyC &>(FunctionC::Body()); }
    //: Access body.
    
    const FuncOrthPolynomialBodyC &Body() const
    { return static_cast<const FuncOrthPolynomialBodyC &>(FunctionC::Body()); }
    //: Access body.
    
  public:
    
  };
  
  inline std::istream &operator>>(std::istream &strm,FuncOrthPolynomialC &obj) {
    obj = FuncOrthPolynomialC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline std::ostream &operator<<(std::ostream &out,const FuncOrthPolynomialC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,FuncOrthPolynomialC &obj) {
    obj = FuncOrthPolynomialC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const FuncOrthPolynomialC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
}


#endif
