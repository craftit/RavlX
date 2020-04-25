// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FUNCLINEARCOEFF_HEADER
#define RAVL_FUNCLINEARCOEFF_HEADER 1
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! date="21/4/1998"
//! author="Robert Crida and Charles Galambos"
//! docentry="Ravl.API.Pattern Recognition.Numerical Modeling"
//! file="Ravl/PatternRec/Modeling/Basic/FuncLinearCoeff.hh"

#include "Ravl/PatternRec/Function.hh"
#include "Ravl/PatternRec/SampleStream.hh"

namespace RavlN {
  class MatrixRUTC;
  
  //! userlevel=Develop
  //: Function with linear coefficients.
  
  class FuncLinearCoeffBodyC
    : public FunctionBodyC
  {
  public:
    FuncLinearCoeffBodyC();
    //: Default constructor.
    
    FuncLinearCoeffBodyC(UIntT inSize,UIntT outSize)
      : FunctionBodyC(inSize,outSize)
    {}
    //: Create a function with the given number of inputs and outputs.
    
    FuncLinearCoeffBodyC(std::istream &strm);
    //: Load from stream.
    
    FuncLinearCoeffBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (std::ostream &out) const;
    //: Writes object to stream.
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to binary stream.
    
    virtual VectorC Apply(const VectorC &data) const;
    //: Apply function to 'data'
    
    virtual MatrixC Jacobian (const VectorC &X) const;
    //: Calculate Jacobian matrix at X
    
    virtual VectorC MakeInput (const VectorC &X) const;
    //: Expand vector to linear coefficients.
    
    virtual VectorC MakeJacobianInput (const VectorC &X, IntT i) const;
    //: Expand vector to linear coefficients.
    
    virtual UIntT NumberCoeffs(UIntT inputSize) const;
    //: Calculate the number of coefficients for a given input size.
    
    virtual bool SetTransform(const MatrixC &a);
    //: Attempt to set transform matrix.
    
    virtual void Describe(std::ostream &out);
    //: Write a human readable text description of the function.
    
    MatrixC &Transform()
    { return a; }
    //: Access transform matrix.
    // Advanced users only.
    
    virtual bool ComputeSums(const SampleC<VectorC> &in,const SampleC<VectorC> &out,MatrixRUTC &aaTu,MatrixC &aTb);
    //: Compute matrix's directly from vectors.
    
    virtual bool ComputeSums(SampleStream2C<VectorC,VectorC> &in,MatrixRUTC &aaTu,MatrixC &aTb);
    //: Compute matrix's directly from vectors.
    
    
  protected:
    MatrixC a;
  };

  //! userlevel=Develop
  //: Function with linear coefficients.
  
  class FuncLinearCoeffC
    : public FunctionC
  {
  public:
    FuncLinearCoeffC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    FuncLinearCoeffC(std::istream &strm);
    //: Load from stream.
    
    FuncLinearCoeffC(BinIStreamC &strm);
    //: Load from binary stream.
    
    FuncLinearCoeffC(const FunctionC &base)
      : FunctionC(dynamic_cast<const FuncLinearCoeffBodyC *>(BodyPtr(base)))
    {}
    //: Construct from base class.
    // If base isn't derived from FuncLinearCoeff an invalid handle will
    // be created.
    
  protected:
    FuncLinearCoeffC(FuncLinearCoeffBodyC &bod)
      : FunctionC(bod)
    {}
    //: Body constructor.
    
    FuncLinearCoeffC(const FuncLinearCoeffBodyC *bod)
      : FunctionC(bod)
    {}
    //: Body ptr constructor.
    
    FuncLinearCoeffBodyC &Body()
    { return static_cast<FuncLinearCoeffBodyC &>(FunctionC::Body()); }
    //: Access body.
    
    const FuncLinearCoeffBodyC &Body() const
    { return static_cast<const FuncLinearCoeffBodyC &>(FunctionC::Body()); }
    //: Access body.
    
  public:
    bool SetTransform(const MatrixC &a)
    { return Body().SetTransform(a); }
    //: Attempt to set transform matrix.
    
    VectorC MakeInput(const VectorC &X) const
    { return Body().MakeInput(X); }
    //: Expand vector to linear coefficients.
    
    VectorC MakeJacobianInput (const VectorC &X, IntT i) const
    { return Body().MakeJacobianInput(X,i); }
    //: Expand vector to linear coefficients.
    
    UIntT NumberCoeffs(UIntT inputSize) const
    { return Body().NumberCoeffs(inputSize); }
    //: Calculate the number of coefficients for a given input size.
    
    MatrixC &Transform()
    { return Body().Transform(); }
    //: Access transform matrix.
    // Advanced users only.
    
    bool ComputeSums(const SampleC<VectorC> &in,const SampleC<VectorC> &out,MatrixRUTC &aaTu,MatrixC &aTb)
    { return Body().ComputeSums(in,out,aaTu,aTb); }
    //: Compute matrix's directly from vectors.

    bool ComputeSums(SampleStream2C<VectorC,VectorC> &in,MatrixRUTC &aaTu,MatrixC &aTb)
    { return Body().ComputeSums(in,aaTu,aTb); }
    //: Compute matrix's directly from vectors.
  };
  
  inline std::istream &operator>>(std::istream &strm,FuncLinearCoeffC &obj) {
    obj = FuncLinearCoeffC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline std::ostream &operator<<(std::ostream &out,const FuncLinearCoeffC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,FuncLinearCoeffC &obj) {
    obj = FuncLinearCoeffC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const FuncLinearCoeffC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.

}
#endif
