// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Modeling/Basic/FuncLinearCoeff.cc"

#include "Ravl/PatternRec/FuncLinearCoeff.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/PatternRec/DataSet2Iter.hh"
#include "Ravl/PatternRec/SampleVector.hh"
#include "Ravl/MatrixRUT.hh"
#include "Ravl/config.h"

#if RAVL_COMPILER_MIPSPRO 
#include "Ravl/DP/Port.hh"
#include "Ravl/Tuple2.hh"
#include "Ravl/Vector.hh"
#pragma instantiate RavlN::DPIPortBodyC<RavlN::Tuple2C<RavlN::VectorC,RavlN::VectorC> >
#endif 

namespace RavlN {
  
  //: Default constructor.
  
  FuncLinearCoeffBodyC::FuncLinearCoeffBodyC()
  {}

  //: Load from stream.
  
  FuncLinearCoeffBodyC::FuncLinearCoeffBodyC(std::istream &strm)
    : FunctionBodyC(strm)
  { strm >> a; }
  
  //: Load from binary stream.
  
  FuncLinearCoeffBodyC::FuncLinearCoeffBodyC(BinIStreamC &strm) 
    : FunctionBodyC(strm)
  { strm >> a; }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool FuncLinearCoeffBodyC::Save (std::ostream &out) const {
    if(!FunctionBodyC::Save(out))
      return false;
    out << ' ' << a ;
    return true;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool FuncLinearCoeffBodyC::Save (BinOStreamC &out) const {
    if(!FunctionBodyC::Save(out))
      return false;
    out << a;
    return true;
  }
  
  //: Attempt to set transform matrix.
  
  bool FuncLinearCoeffBodyC::SetTransform(const MatrixC &na) {
    a = na;
    return true;
  }

  //: Apply function to 'data'
  
  VectorC FuncLinearCoeffBodyC::Apply(const VectorC &data) const 
  {
    if(!a.IsValid()) // If no a is set assume the identity matrix
      return MakeInput(data);
    return a*MakeInput(data);
  }
  
  //: Calculate Jacobian matrix at X
  
  MatrixC FuncLinearCoeffBodyC::Jacobian (const VectorC &X) const {
    MatrixC ret(OutputSize(),InputSize());
    if(!a.IsValid()) {  // If no a is set assume the identity matrix
      for(UIntT i = 0;i < X.Size();i++)
        ret.SetColumn(i,MakeJacobianInput(X,i));
    } else {
      for(UIntT i = 0;i < X.Size();i++)
        ret.SetColumn(i,a*MakeJacobianInput(X,i));
    }
    return ret;
  }
  
  //: Expand vector to linear coefficients.
  
  VectorC FuncLinearCoeffBodyC::MakeInput(const VectorC &X) const {
    RavlAssertMsg(0,"FuncLinearCoeffBodyC::MakeInput(), Abstract method called. ");
    return VectorC();
  }
  
  //: Expand vector to linear coefficients.
  
  VectorC FuncLinearCoeffBodyC::MakeJacobianInput (const VectorC &X, IntT i) const {
    RavlAssertMsg(0,"FuncLinearCoeffBodyC::MakeJacobianInput(), Abstract method called. ");
    return VectorC();
  }
  
  //: Calculate the number of coefficents for a given input size.
  
  UIntT FuncLinearCoeffBodyC::NumberCoeffs(UIntT inputSize) const {
    RavlAssertMsg(0,"FuncLinearCoeffBodyC::NumberCoeffs(), Abstract method called. ");
    return 0;
  }

  //: Write a human readable text description of the function.
  
  void FuncLinearCoeffBodyC::Describe(std::ostream &out) {
    out << "Inputs:" << InputSize() << " Outputs:" << OutputSize() << "\n";
    out << "Linear model: " << a << "\n";
  }


  //: Compute matrix's directly from vectors.
  
  bool FuncLinearCoeffBodyC::ComputeSums(const SampleC<VectorC> &in,const SampleC<VectorC> &out,MatrixRUTC &aaTu,MatrixC &aTb) {
    SampleVectorC vin(in);
    SampleVectorC vout(out);
    
    DataSet2IterC<SampleVectorC,SampleVectorC> it(in,out);
    
    VectorC vec = MakeInput(it.Data1());
    aTb = vec.OuterProduct(it.Data2());
    aaTu = OuterProductRUT(vec);
    for(it++;it;it++) {
      vec = MakeInput(it.Data1());
      aaTu.AddOuterProduct(vec);
      aTb.AddOuterProduct(vec,it.Data2());
    }
    
    return true;
  }

  //: Compute matrix's directly from vectors.
  
  bool FuncLinearCoeffBodyC::ComputeSums(SampleStream2C<VectorC,VectorC> &in,MatrixRUTC &aaTu,MatrixC &aTb) {
    Tuple2C<VectorC,VectorC> tup;
    if(!in.Get(tup))
      return false;
    
    VectorC vec = MakeInput(tup.Data1());
    aTb = vec.OuterProduct(tup.Data2());
    aaTu = OuterProductRUT(vec);
    while(1) {
      if(!in.Get(tup))
	break;
      vec = MakeInput(tup.Data1());
      aaTu.AddOuterProduct(vec);
      aTb.AddOuterProduct(vec,tup.Data2());
    }
    return true;
  }

  ///////////////////////////////////////////////////////////
  
  //: Load from stream.
  
  FuncLinearCoeffC::FuncLinearCoeffC(std::istream &strm) 
    : FunctionC(RAVL_VIRTUALCONSTRUCTOR(strm,FuncLinearCoeffBodyC))
  {}
  
  //: Load from binary stream.
  
  FuncLinearCoeffC::FuncLinearCoeffC(BinIStreamC &strm) 
    : FunctionC(RAVL_VIRTUALCONSTRUCTOR(strm,FuncLinearCoeffBodyC))
  {}

}
