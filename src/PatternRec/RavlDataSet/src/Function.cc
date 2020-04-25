// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/DataSet/Function.cc"

#include "Ravl/PatternRec/Function.hh"
#include "Ravl/Matrix.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/PatternRec/Sample.hh"
#include "Ravl/DArray1dIter2.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/SysLog.hh"

namespace RavlN {

  FunctionBodyC::FunctionBodyC(const XMLFactoryContextC &factory)
    : inputSize(factory.AttributeUInt("input_size", 0)),
      outputSize(factory.AttributeUInt("output_size", 0))             
  {
  }
  //: XMLFactoryC constructor.
  
  FunctionBodyC::FunctionBodyC(std::istream &strm)
    : RCBodyVC(strm)
  { strm >> inputSize >> outputSize; }
  
  //: Load from binary stream.
  
  FunctionBodyC::FunctionBodyC(BinIStreamC &strm)
    : RCBodyVC(strm)
  { strm >> inputSize >> outputSize; }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool FunctionBodyC::Save (std::ostream &out) const {
    if(!RCBodyVC::Save(out))
      return false;
    out  << ' ' << inputSize << ' ' << outputSize << ' ';
    return true;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool FunctionBodyC::Save (BinOStreamC &out) const {
    if(!RCBodyVC::Save(out))
      return false;
    out << inputSize << outputSize;
    return true;
  }

  //: Apply function in place to 'data', overwrite values in out if its of the correct size.

  void FunctionBodyC::ApplyInPlace(const VectorC &data,VectorC &out) const
  { out = Apply(data); }


  //: Apply function with float vectors to 'data'

  TVectorC<float> FunctionBodyC::Apply(const TVectorC<float> &data) const {
    VectorC tmp(data);
    TVectorC<float> ret = Apply(tmp);
    return ret;
  }


  //: Apply function in place to 'data', overwrite values in out if its of the correct size.

  void FunctionBodyC::ApplyInPlace(const TVectorC<float> &data,TVectorC<float> &out) const
  { out = Apply(data); }


  //: Apply function to 'data'
  
  VectorC FunctionBodyC::Apply(const VectorC &data) const {
    //RavlAssertMsg(0,"RavlN::Apply(), Abstract method called. \n");
    RavlWarning("Function does nothing to data!");
    return data;
  }
  
  //: Apply function to two data vectors 

  VectorC FunctionBodyC::Apply(const VectorC & data1, const VectorC & data2) const {
    return Apply( data1.Join(data2) ) ; }

  //: Apply transform to whole dataset.
  
  SampleC<VectorC> FunctionBodyC::Apply(const SampleC<VectorC> &data) {
    SampleC<VectorC> ret(data.Size());
    for(DArray1dIterC<VectorC> it(data.DArray());it;it++)
      ret += Apply(*it);
    return ret;
  }
  
  //: Compare the numerical and computed Jacobian, return true if the match.
  // Useful for debugging!

  bool FunctionBodyC::CheckJacobian(const VectorC &X,RealT tolerance,RealT epsilon) const
  {
    MatrixC approx = NumericalJacobian(X,epsilon);
    MatrixC comp = Jacobian (X);
    MatrixC diff = approx - comp;
    RealT err = (diff.SumOfAbs() / (RealT) diff.Size());
    RavlDebug("Jacobian error:%e Epsilon:%f ",err,epsilon);
    RavlDebug("Jacobian   approx:%s ",RavlN::StringOf(approx).c_str());
    RavlDebug("Jacobian computed:%s ",RavlN::StringOf(comp).c_str());
    return err < tolerance;
  }

  //: Calculate numerical approximation of Jacobian matrix at X

  MatrixC FunctionBodyC::NumericalJacobian(const VectorC &X,RealT epsilon) const
  {
    //RavlDebug("Num Jacobian. eps:%f ",epsilon);
    RavlAssert(X.Size() == inputSize);
    MatrixC J (outputSize,inputSize);
    VectorC dX (inputSize);
    dX.Fill(0);
    for(UIntT index = 0;index < inputSize;index++) {
      dX[index] = epsilon;
      VectorC temp = Apply(X+dX) - Apply(X-dX);
      temp /= epsilon * 2;
      J.SetColumn (index,temp);
      dX[index] = 0;
    }
    return J;
  }


  MatrixC FunctionBodyC::Jacobian (const VectorC &X) const
  { return NumericalJacobian(X,1e-6); }
  
  //: Evalate the function and its jacobian at the same time.
  // This method defaults to calling 'Apply' and 'Jacobian' sperately.
  
  VectorMatrixC FunctionBodyC::Evaluate(const VectorC &X) {
    return VectorMatrixC(Apply(X),Jacobian(X));
  }
  
  //: Evaluate the value,jacobian of the function at point X
  // Returns true if all values are provide, false if one or more is approximated.
  
  bool FunctionBodyC::EvaluateValueJacobian(const VectorC &X,VectorC &value,MatrixC &jacobian) const {
    value = Apply(X);
    jacobian = Jacobian (X);
    return false;
  }
  
  //: Evaluate the value of the function at point X
  // Returns true if all values are provide, false if one or more is approximated.
  
  bool FunctionBodyC::EvaluateValue(const VectorC &X,VectorC &value) const {
    value = Apply(X);
    return true;
  }
  
  //: Write a human readable text description of the function.
  void FunctionBodyC::Describe(std::ostream &out) {
    out << "Describe not implemented\n";
  }
  
  //////////////////////////////////////////////////////////
  
  //: Apply transform to whole dataset.
  
  SampleC<VectorC> FunctionC::Apply(const SampleC<VectorC> &data)
  { return Body().Apply(data); }

  //: Load from stream.
  
  FunctionC::FunctionC(std::istream &strm) 
    : RCHandleVC<FunctionBodyC>(RAVL_VIRTUALCONSTRUCTOR(strm,FunctionBodyC))
  {}
    
  //: Load from binary stream.
  
  FunctionC::FunctionC(BinIStreamC &strm) 
    : RCHandleVC<FunctionBodyC>(RAVL_VIRTUALCONSTRUCTOR(strm,FunctionBodyC))    
  {}
  
  //: Setup virtual constructor.
  // As this is an abstract class its not really needed, but its useful to
  // have an example of how it should work here.
  
  //RAVL_INITVIRTUALCONSTRUCTOR(FunctionBodyC);
  

}
