// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FUNCLINEAR_HEADER
#define RAVL_FUNCLINEAR_HEADER 1
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! date="21/4/1998"
//! author="Robert Crida and Charles Galambos"
//! docentry="Ravl.API.Pattern Recognition.Numerical Modeling"
//! file="Ravl/PatternRec/Modeling/Basic/FuncLinear.hh"

#include "Ravl/PatternRec/FuncLinearCoeff.hh"
#include "Ravl/VirtualConstructor.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Linear function.
  
  class FuncLinearBodyC 
    : public FuncLinearCoeffBodyC
  {
  public:
    FuncLinearBodyC();
    //: Construct from a transform matrix.
    
    FuncLinearBodyC(UIntT inSize,UIntT outSize)
      : FuncLinearCoeffBodyC(inSize,outSize)
    {}
    //: Construct with an input and output size.
    
    FuncLinearBodyC(const MatrixC &proj,const VectorC &offset);
    //: Design a linear mapping which multiplies input by 'proj' and adds offset
    
    FuncLinearBodyC(std::istream &strm);
    //: Load from stream.
    
    FuncLinearBodyC(BinIStreamC &strm);
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
    
    virtual VectorC Apply(const VectorC &data) const;
    //: Apply function to 'data'

    virtual bool SetTransform(const MatrixC &a);
    //: Setup the transform.
    
    virtual bool ComputeSums(const SampleC<VectorC> &in,const SampleC<VectorC> &out,MatrixRUTC &aaTu,MatrixC &aTb);
    //: Compute matrix's directly from vectors.
    
    VectorC &Mean()
    { return mean; }
    //: Access mean.
    
    MatrixC &Projection()
    { return proj; }
    //: Access projection.
    
  protected:
    // These are extracted from transform 'a'
    VectorC mean;
    MatrixC proj;
  };
  
  
  //! userlevel=Normal
  //: Linear function.
  
  class FuncLinearC 
    : public FuncLinearCoeffC
  {
  public:
    FuncLinearC()
    {}
    //: Default constructor.
    
    FuncLinearC(UIntT inSize,UIntT outSize)
      : FuncLinearCoeffC(*new FuncLinearBodyC(inSize,outSize))
    {}
    //: Construct a linear function with given input and output size.
    // The transform its self is left undefined.
    
    FuncLinearC(const MatrixC &proj,const VectorC &offset)
      : FuncLinearCoeffC(*new FuncLinearBodyC(proj,offset))
    {}
    //: Design a linear mapping which multiplies input by 'proj' and adds offset
    
    FuncLinearC(std::istream &strm);
    //: Load from stream.
    
    FuncLinearC(BinIStreamC &strm);
    //: Load from binary stream.
    
  protected:
    FuncLinearC(FuncLinearBodyC &bod)
      : FuncLinearCoeffC(bod)
    {}
    //: Body constructor.
    
    FuncLinearC(FuncLinearBodyC *bod)
      : FuncLinearCoeffC(bod)
    {}
    //: Body ptr constructor.
    
    FuncLinearBodyC &Body()
    { return static_cast<FuncLinearBodyC &>(FunctionC::Body()); }
    //: Access body.
    
    const FuncLinearBodyC &Body() const
    { return static_cast<const FuncLinearBodyC &>(FunctionC::Body()); }
    //: Access body.
    
  public:    
    VectorC &Mean()
    { return Body().Mean(); }
    //: Access mean.
    
    MatrixC &Projection()
    { return Body().Projection(); }
    //: Access projection.
  };
  
  inline std::istream &operator>>(std::istream &strm,FuncLinearC &obj) {
    obj = FuncLinearC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline std::ostream &operator<<(std::ostream &out,const FuncLinearC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,FuncLinearC &obj) {
    obj = FuncLinearC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const FuncLinearC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
}


#endif
