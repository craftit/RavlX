// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Modeling/Basic/FuncOrthPolynomial.cc"

#include "Ravl/PatternRec/FuncOrthPolynomial.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/BinStream.hh"

namespace RavlN {
  
  //: Construct an orthogonal polynomial of given 'order'.
  
  FuncOrthPolynomialBodyC::FuncOrthPolynomialBodyC(int inSize,int outSize,UIntT norder)
    : FuncLinearCoeffBodyC(inSize,outSize),
      order(norder)
  { noCoeffs = NumberCoeffs(inSize); }
  
  //: Load from stream.
  
  FuncOrthPolynomialBodyC::FuncOrthPolynomialBodyC(std::istream &strm)
    : FuncLinearCoeffBodyC(strm),
      noCoeffs(0),
      order(0)
  { strm >> noCoeffs >> order; }
  
  //: Load from binary stream.
  
  FuncOrthPolynomialBodyC::FuncOrthPolynomialBodyC(BinIStreamC &strm) 
    : FuncLinearCoeffBodyC(strm),
      noCoeffs(0),
      order(0)
  { strm >> noCoeffs >> order; }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool FuncOrthPolynomialBodyC::Save (std::ostream &out) const {
    if(!FuncLinearCoeffBodyC::Save(out))
      return false;
    out  << ' ' << noCoeffs << ' ' << order;
    return true;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool FuncOrthPolynomialBodyC::Save (BinOStreamC &out) const {
    if(!FuncLinearCoeffBodyC::Save(out))
      return false;
    out << noCoeffs << order;
    return true;
  }
  
  //: Calculate the number of coefficients for a given input size.
  
  UIntT FuncOrthPolynomialBodyC::NumberCoeffs(UIntT inputSize) const {
    return 1 + inputSize * order;
  }
  
  //: Expand vector to linear coefficients.
  
  VectorC FuncOrthPolynomialBodyC::MakeInput(const VectorC &X) const {
    VectorC ret(noCoeffs);
    SArray1dIterC<RealT> it(ret);
    *it = 1.0;
    it++;
    for (UIntT i = 1; i <= order; i++) {
      for(SArray1dIterC<RealT> it2(X);it2;it2++,it++)
        *it = Pow(*it2,(IntT) i);
    }
    return ret;
  }
  
  //: Expand vector to linear coefficients.
  
  VectorC FuncOrthPolynomialBodyC::MakeJacobianInput(const VectorC &X, IntT index) const {
    VectorC ret(noCoeffs);
    ret.Fill(0);
    double val = X[index];
    for (UIntT i = 0; i < order; i++) 
      ret[1+InputSize()*i+index] = (i+1) * Pow (val,(IntT) i);
    return ret;
  }
  
  ////////////////////////////////////////////////////////////////////////
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(FuncOrthPolynomialBodyC,FuncOrthPolynomialC,FuncLinearCoeffC);
  
}
