// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Modeling/Basic/FuncQuadratic.cc"

#include "Ravl/PatternRec/FuncQuadratic.hh"
#include "Ravl/ScalMath.hh"
#include "Ravl/VirtualConstructor.hh"

namespace RavlN {

  //: Constructor.
  FuncQuadraticBodyC::FuncQuadraticBodyC(int inSize,int outSize)
    : FuncLinearCoeffBodyC(inSize,outSize)
  {
    noCoeffs = NumberCoeffs(inSize);
    if(outSize < 0)
      OutputSize(noCoeffs);
  }

  //: Load from stream.
  
  FuncQuadraticBodyC::FuncQuadraticBodyC(std::istream &strm)
    : FuncLinearCoeffBodyC(strm)
  { noCoeffs = NumberCoeffs(inputSize); }
  
  //: Load from binary stream.
  
  FuncQuadraticBodyC::FuncQuadraticBodyC(BinIStreamC &strm)
    : FuncLinearCoeffBodyC(strm)
  { noCoeffs = NumberCoeffs(inputSize); }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool FuncQuadraticBodyC::Save (std::ostream &out) const {
    return FuncLinearCoeffBodyC::Save(out);
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool FuncQuadraticBodyC::Save (BinOStreamC &out) const {
    return FuncLinearCoeffBodyC::Save(out);    
  }
  
  
  //: Calculate the number of coefficents for a given input size.
  
  UIntT FuncQuadraticBodyC::NumberCoeffs(UIntT inputSize) const {
    return  1+2*inputSize+ (int) BinomalCoeff(inputSize,2);
  }
  
  //: Expand vector to linear coefficients.
  
  VectorC FuncQuadraticBodyC::MakeInput(const VectorC &X) const {
    VectorC ret (noCoeffs);
    SArray1dIterC<RealT> it(ret);
    *it = 1.0;
    it++;
    SArray1dIterC<RealT> it2(X);
    for(;it2;it++,it2++)
      *it = *it2;
    for(it2.First();it2;it2++) {
      for(SArray1dIterC<RealT> it3(it2);it3;it3++,it++)
        *it = (*it2) * (*it3);
    }
    RavlAssert(!it.IsElm());
    return ret;
  }
  
  //: Expand vector to linear coefficients.
  
  VectorC FuncQuadraticBodyC::MakeJacobianInput(const VectorC &X, IntT i) const {
    VectorC ret (noCoeffs);
    ret.Fill(0);
    ret[i + 1] = 1.0;
    UIntT counter = X.Size() + 1;
    for(int j = 0;j < (int) X.Size();j++) {
      for(int k = j;k < (int) X.Size();k++) {
        if (j == i) {
          if(k == i)
            ret[counter] = 2.0*X[j];
          else
            ret[counter] = X[k];
        } else {
          if(k == i)
            ret[counter] = X[j];
          //	  else
          //  ret[counter] = 0.0;
        }
        counter++;
      }
    }
    return ret;
  }

  //: Write a human readable text description of the function.
  
  void FuncQuadraticBodyC::Describe(std::ostream &out) {
    out << "Inputs:" << InputSize() << " Outputs:" << OutputSize() << "\n";
    out << "Quadratic model: " << a << "\n";
  }

  ////////////////////////////////////////////////////////////////////////
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(FuncQuadraticBodyC,FuncQuadraticC,FuncLinearCoeffC);
  
}
