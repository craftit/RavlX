// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FUNCTION_HEADER
#define RAVL_FUNCTION_HEADER 1
///////////////////////////////////////////////////////////////
//! lib=RavlPatternRec
//! author="Charles Galambos, based on code by Robert Crida"
//! docentry="Ravl.API.Pattern Recognition.Functions"
//! file="Ravl/PatternRec/DataSet/Function.hh"

#include "Ravl/Vector.hh"
#include "Ravl/Matrix.hh"
#include "Ravl/RCHandleV.hh"
#include "Ravl/VectorMatrix.hh"
#include "Ravl/XMLFactory.hh"

namespace RavlN {
  template<class DataT> class SampleC;
  
  //! userlevel=Develop
  //: Map an input vector to an output vector.
  
  class FunctionBodyC
    : public RCBodyVC
  {
  public:
    FunctionBodyC()
      : inputSize(0),
	      outputSize(0)
    {}
    //: Default constructor.
    
    FunctionBodyC(UIntT inSize,UIntT outSize)
      : inputSize(inSize),
	      outputSize(outSize)
    {}
    //: Create function with given number of inputs and outputs.
    
    FunctionBodyC(const XMLFactoryContextC &factory);
    //: Construct from XML factory

    FunctionBodyC(std::istream &strm);
    //: Load from stream.
    
    FunctionBodyC(BinIStreamC &strm);
    //: Load from binary stream.

    virtual bool Save (std::ostream &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor

    virtual void ApplyInPlace(const VectorC &data,VectorC &out) const;
    //: Apply function in place to 'data', overwrite values in out if its of the correct size.
    // This function has been added to allow efficient pipelines to be constructed that
    // don't require memory allocation.  Note, the output vector MAY be replaced,
    // if its not of the expected length, or if the function doesn't support the in place method.

    virtual VectorC Apply(const VectorC &data) const;
    //: Apply function to 'data'

    virtual void ApplyInPlace(const TVectorC<float> &data,TVectorC<float> &out) const;
    //: Apply function in place to 'data', overwrite values in out if its of the correct size.
    // This function has been added to allow efficient pipelines to be constructed that
    // don't require memory allocation.  Note, the output vector MAY be replaced,
    // if its not of the expected length, or if the function doesn't support the in place method.

    virtual TVectorC<float> Apply(const TVectorC<float> &data) const;
    //: Apply function with float vectors to 'data'

    virtual VectorC Apply(const VectorC & data1, const VectorC & data2) const ; 
    //: Apply function to two data vectors 
    // The default behaviour is to concatenate the two vectors  
    // and then call the single vector version of Apply() 

    virtual SampleC<VectorC> Apply(const SampleC<VectorC> &data);
    //: Apply transform to whole dataset.
    
    bool CheckJacobian(const VectorC &X,RealT tolerance = 1e-4,RealT epsilon = 1e-6) const;
    //: Compare the numerical and computed jacobians at X, return true if the match.
    // Useful for debugging!

    MatrixC NumericalJacobian(const VectorC &X,RealT epsilon = 1e-6) const;
    //: Calculate numerical approximation of Jacobian matrix at X

    virtual MatrixC Jacobian (const VectorC &X) const;
    //: Calculate Jacobian matrix at X
    // The default implementation performs numerical estimation of the Jacobian using differences. This
    // function has and should be overloaded for all cases where the Jacobian
    // can be calculated analytically.
    
    virtual VectorMatrixC Evaluate(const VectorC &X);
    //: Evaluate the function and its jacobian at the same time.
    // This method defaults to calling 'Apply' and 'Jacobian' sperately.
    // OBSOLETE Use, bool Evaluate(const VectorC &X,RealT &value,VectorC &jacobian);
    
    virtual bool EvaluateValueJacobian(const VectorC &X,VectorC &value,MatrixC &jacobian) const;
    //: Evaluate the value,jacobian of the function at point X
    // Returns true if all values are provide, false if one or more is approximated.
    
    virtual bool EvaluateValue(const VectorC &X,VectorC &value) const;
    //: Evaluate the value of the function at point X
    
    inline UIntT InputSize() const
    { return inputSize; }
    //: Size of input vectors
    
    inline UIntT OutputSize() const
    { return outputSize; }
    //: Size of output vectors
    
    virtual UIntT InputSize(UIntT inSize)
    { return inputSize = inSize; }
    //: Set the input size.
    // This is for use of designer classes, changing the input size of
    // an existing function has undefined effects.
    
    virtual UIntT OutputSize(UIntT outSize)
    { return outputSize = outSize; }
    //: Set the output size.
    // This is for use of designer classes, changing the output size of
    // an existing function has undefined effects.
    
    virtual void Describe(std::ostream &out);
    //: Write a human readable text description of the function.


    typedef SmartPtrC<FunctionBodyC> RefT;
    //: Smart ptr to function

  protected:
    
    UIntT inputSize;  // (X in amma speak.)
    UIntT outputSize; // (Y in amma speak.)
  };
  
  //! userlevel=Normal
  //: Map an input vector to an output vector.
  
  class FunctionC
    : public RCHandleVC<FunctionBodyC>
  {
  public:
    FunctionC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    FunctionC(std::istream &strm);
    //: Load from stream.

    FunctionC(BinIStreamC &strm);
    //: Load from binary stream.
    
  protected:
    FunctionC(FunctionBodyC &bod)
      : RCHandleVC<FunctionBodyC>(bod)
    {}
    //: Body constructor.
    
    FunctionC(const FunctionBodyC *bod)
      : RCHandleVC<FunctionBodyC>(bod)
    {}
    //: Body ptr constructor.
    
    FunctionBodyC &Body()
    { return RCHandleC<FunctionBodyC>::Body(); }
    //: Access body.
    
    const FunctionBodyC &Body() const
    { return RCHandleC<FunctionBodyC>::Body(); }
    //: Access body.
    
  public:
    VectorC Apply(const VectorC &data) const
    { return Body().Apply(data); }
    //: Apply function to 'data'
    
    inline VectorC Apply(const VectorC & data1, const VectorC & data2) const
      { return Body().Apply(data1, data2) ; }
    //: Apply function to two data vectors 
    // The default behaviour is to concatenate the two vectors  
    // and then call the single vector version of Apply() 
    
    SampleC<VectorC> Apply(const SampleC<VectorC> &data);
    //: Apply transform to whole dataset.
    
    VectorC operator() (const VectorC &X) const 
    { return Body().Apply(X); }
    //: Evaluate Y=f(X)
    
    bool CheckJacobian(const VectorC &X,RealT tolerance = 1e-4,RealT epsilon = 1e-4) const
    { return Body().CheckJacobian(X,tolerance,epsilon); }
    //: Compare the numerical and computed jacobians at X, return true if the match.
    // Useful for debugging!

    MatrixC Jacobian(const VectorC &X) const
    { return Body().Jacobian(X); }    
    //: Calculate Jacobian matrix at X
    // Performs numerical estimation of the Jacobian using differences. This
    // function has and should be overloaded for all cases where the Jacobian
    // can be calculated analytically.
    
    inline VectorMatrixC Evaluate(const VectorC &X)
    { return Body().Evaluate(X); }
    //: Evaluate the function and its jacobian at the same time.
    // This method defaults to calling 'Apply' and 'Jacobian' sperately.
    
    inline bool EvaluateValueJacobian(const VectorC &X,VectorC &value,MatrixC &jacobian) const
    { return Body().EvaluateValueJacobian(X,value,jacobian); }
    //: Evaluate the value,jacobian of the function at point X
    // Returns true if all values are provide, false if one or more is approximated.
    
    inline bool EvaluateValue(const VectorC &X,VectorC &value) const
    { return Body().EvaluateValue(X,value); }
    //: Evaluate the value of the function at point X
    // Returns true if all values are provide, false if one or more is approximated.
    
    inline UIntT InputSize() const
    { return Body().InputSize(); }
    //: Size of input vector
    
    inline UIntT OutputSize() const
    { return Body().OutputSize(); }
    //: Size of output vector

    UIntT InputSize(UIntT inSize)
    { return Body().InputSize(inSize); }
    //: Set the input size.
    // This is for use of designer classes, changing the input size of
    // an existing function has undefined effects.
    
    UIntT OutputSize(UIntT outSize)
    { return Body().OutputSize(outSize); }
    //: Set the output size.
    // This is for use of designer classes, changing the output size of
    // an existing function has undefined effects.
    
    void Describe(std::ostream &out) 
    { return Body().Describe(out); }
    //: Write a human readable text description of the function.
    // May not be implemented for all functions.
  };

  
  inline std::istream &operator>>(std::istream &strm,FunctionC &obj) {
    obj = FunctionC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline std::ostream &operator<<(std::ostream &out,const FunctionC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,FunctionC &obj) {
    obj = FunctionC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const FunctionC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
}

#endif
