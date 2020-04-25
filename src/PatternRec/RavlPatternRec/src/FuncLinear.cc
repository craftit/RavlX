// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Modeling/Basic/FuncLinear.cc"

#include "Ravl/PatternRec/FuncLinear.hh"
#include "Ravl/PatternRec/DataSet2Iter.hh"
#include "Ravl/PatternRec/SampleVector.hh"
#include "Ravl/MatrixRUT.hh"

#define DODEBUG	0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  void InitPattenRecFuncLinear()
  {}
  
  //: Construct from a transform matrix.
  
  FuncLinearBodyC::FuncLinearBodyC() {
    ONDEBUG(std::cerr << "FuncLinearBodyC::FuncLinearBodyC(), Called. \n");
  }
  
  //: Load from stream.
  
  FuncLinearBodyC::FuncLinearBodyC(std::istream &strm)
    : FuncLinearCoeffBodyC(strm)
  {
    SetTransform(a); // make sure transform is setup properly.
  }
  
  //: Load from binary stream.
  
  FuncLinearBodyC::FuncLinearBodyC(BinIStreamC &strm)
    : FuncLinearCoeffBodyC(strm)
  { 
    SetTransform(a); // make sure transform is setup properly.
  }

  //: Design a linear mapping which multiplies input by 'proj' and adds offset
  
  FuncLinearBodyC::FuncLinearBodyC(const MatrixC &proj,const VectorC &offset)
  {
    RavlAssert(offset.Size() ==  proj.Rows());
    MatrixC na(proj.Rows(),proj.Cols() + 1);
    na.SetColumn(0,offset);
    na.SetSubArray(Index2dC(0,1),proj);
    SetTransform(na);
  }
  
  //: Writes object to stream.
  
  bool FuncLinearBodyC::Save (std::ostream &out) const 
  { return FuncLinearCoeffBodyC::Save(out); }
  
  //: Writes object to binary stream.
  
  bool FuncLinearBodyC::Save (BinOStreamC &out) const
  { return FuncLinearCoeffBodyC::Save(out); }
  
  //: Expand vector to linear coefficients.
  
  VectorC FuncLinearBodyC::MakeInput(const VectorC &X) const {
    VectorC ret(X.Size() + 1);
    ret[0] = 1;
    for(BufferAccessIter2C<RealT,RealT> it(X,ret.From(1));it;it++)
      it.Data2() = it.Data1();
    return ret;
  }
  
  //: Expand vector to linear coefficients.
  
  VectorC FuncLinearBodyC::MakeJacobianInput(const VectorC &X, IntT i) const {
    VectorC ret(1+X.Size());
    ret.Fill(0);
    ret[i] = 1.0;
    return ret;
  }

  //: Calculate the number of coefficents for a given input size.
  
  UIntT FuncLinearBodyC::NumberCoeffs(UIntT inputSize) const {
    return inputSize + 1;
  }
  
  //: Apply function to 'data'
  
  VectorC FuncLinearBodyC::Apply(const VectorC &data) const {
    return proj * data + mean;
  }

  //: Setup the transform.
  
  bool FuncLinearBodyC::SetTransform(const MatrixC &na) {
    if(!FuncLinearCoeffBodyC::SetTransform(na))
      return false;
    
    mean = a.SliceColumn(0);
    IndexRange2dC rng = a.Frame();
    rng.LCol()++;
    proj = SArray2dC<RealT>(a,rng);
    InputSize(proj.Rows());
    OutputSize(mean.Size());
    return true;
  }

  //: Compute matrix's directly from vectors.
  
  bool FuncLinearBodyC::ComputeSums(const SampleC<VectorC> &in,const SampleC<VectorC> &out,MatrixRUTC &aaTu,MatrixC &aTb) {
    SampleVectorC vin(in);
    SampleVectorC vout(out);
    
    DataSet2IterC<SampleVectorC,SampleVectorC> it(in,out);
    if(!it)
      return false;
    VectorC vec(vin.VectorSize() + 1);
    vec[0] = 1;
    SArray1dC<RealT> subVec = vec.From(1);
    {
      for(BufferAccessIter2C<RealT,RealT> sit(it.Data1(),subVec);sit;sit++)
	sit.Data2() = sit.Data1();
    }
    
    aTb = vec.OuterProduct(it.Data2());
    aaTu = OuterProductRUT(vec);
    
    for(it++;it;it++) {
      for(BufferAccessIter2C<RealT,RealT> sit(it.Data1(),subVec);sit;sit++)
	sit.Data2() = sit.Data1();
      aaTu.AddOuterProduct(vec);
      aTb.AddOuterProduct(vec,it.Data2());
    }
    
    return true;
  }

  ////////////////////////////////////////////////////////////////////////
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(FuncLinearBodyC,FuncLinearC,FuncLinearCoeffC);
  
}
