// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id: SampleStreamVectorLabel.cc 7324 2009-10-21 16:23:20Z craftit $"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/DataSet/SampleStreamVectorLabel.cc"
#include "Ravl/config.h" 
#include "Ravl/PatternRec/SampleStreamVectorLabel.hh"
#include "Ravl/DList.hh"
#include "Ravl/MeanCovariance.hh"
#include "Ravl/MatrixRUT.hh"
#include "Ravl/PatternRec/SampleVector.hh"
#include "Ravl/DP/Compose.hh"
#include "Ravl/DP/SPortAttach.hh"
#include "Ravl/DP/Func2Proc21.hh"
#include "Ravl/PatternRec/ProcessVectorLabelFunction.hh"


#define DODEBUG	0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  static Tuple2C<VectorC, UIntT> float2realVec(const Tuple2C<TVectorC<float>, UIntT> &vec) {
    return Tuple2C<VectorC, UIntT>(VectorC(vec.Data1()), vec.Data2());
  }
  
  static Tuple2C<VectorC, UIntT> byte2realVec(const Tuple2C<TVectorC<ByteT>, UIntT> &vec) {
      VectorC v(vec.Data1().Size());
      for(SArray1dIter2C<ByteT, RealT>it(vec.Data1(), v);it;it++) {
        it.Data2() = (RealT)it.Data1();
      }
      return Tuple2C<VectorC, UIntT>(v, vec.Data2());
  }
  

  //: Convert from a stream of float vectors.
  SampleStreamVectorLabelC::SampleStreamVectorLabelC(const SampleStream2C<TVectorC<float>, UIntT> & port)
    : DPEntityC(true)
  {
    *this = SPort(port >> Process(float2realVec));
  }

  //: Convert from a stream of float vectors.
  SampleStreamVectorLabelC::SampleStreamVectorLabelC(const SampleStream2C<TVectorC<ByteT>, UIntT> & port)
     : DPEntityC(true)
  {
    *this = SPort(port >> Process(byte2realVec));
  }

  //: Convert from a stream of float vectors.
  SampleStreamVectorLabelC::SampleStreamVectorLabelC(const DPISPortC<Tuple2C<VectorC, UIntT> > & stream, const FunctionC & function)
    : DPEntityC(true)
  {    
    ProcessVectorLabelFunctionC pFunc(function);    
    *this = SPort(stream >> pFunc);
  }
  


#if 0
  //: Compute the sum of the outerproducts.
  // This routine increases accuracy by only summing 500 numbers at time.   Not really worth it.
  // For 1000000 numbers the difference in sums is about 10e-14
  
  MatrixC SampleStreamVectorLabelC::SumOuterProducts() {
    if(!First()) { // Goto first element.
      std::cerr << "SampleStreamVectorLabelC::SumOuterProducts(), WARNING: Failed to seek to first element in sample stream.\n";
    }
    VectorC vec;
    UIntT sumLimit = 500;
    DListC<Tuple2C<MatrixRUTC,UIntT> > sums;
    SArray1dC<VectorC> vecs(sumLimit-1);
    // Initialise accumulator.
    MatrixRUTC accum;
    if(!Get(vec)) 
      return accum;
    accum = OuterProductRUT(vec);
    // Recursively accumulate, never sum more than sumLimit numbers in one go.
    
    while(1) {
      IntT n = GetArray(vecs);
      if(n == 0)
	break; // Done ?
      for(SArray1dIterC<VectorC> sit(vecs,n);sit;sit++)
	accum.AddOuterProduct(*sit);
      //cerr << "ssv: accum=" << accum << "\n";
      if(sums.IsEmpty()) {
	sums.InsLast(Tuple2C<MatrixRUTC,UIntT>(accum.Copy(),1));
      } else {
	bool done = false;
	for(DLIterC<Tuple2C<MatrixRUTC,UIntT> > it(sums);it;it++) {
	  it->Data1() += accum;
	  it->Data2()++;
	  if(it->Data2() < sumLimit) {
	    done = true;
	    break;
	  }
	  accum = it->Data1().Copy();
	  it->Data1().Fill(0);
	  it->Data2() = 0;
	}
	if(!done)
	  sums.InsLast(Tuple2C<MatrixRUTC,UIntT>(accum.Copy(),1));
      }
      accum.Fill(0);
    }
    DLIterC<Tuple2C<MatrixRUTC,UIntT> > it(sums);
    accum = it->Data1();
    for(it++;it;it++)
      accum += it->Data1();
    accum.MakeSymmetric();
    return accum;
  }
  
  //: Find the mean and covariance of the sample
  
  MeanCovarianceC SampleStreamVectorLabelC::MeanCovariance()  {
    if(!First()) { // Goto first element.
      std::cerr << "SampleStreamVectorLabelC::MeanCovariance(), WARNING: Failed to seek to first element in sample stream.\n";
    }
    UIntT sumLimit = 500;
    SArray1dC<VectorC> vecs(sumLimit);
    MeanCovarianceC mc;
    while(1) {
      IntT n = GetArray(vecs);
      if(n == 0)
	break;
      SampleVectorC sv(SArray1dC<VectorC>(vecs,n));
      if(mc.Mean().Size() == 0)
	mc = sv.MeanCovariance();
      else
	mc += sv.MeanCovariance();
    }
    return mc;
  }
  
#endif
  

}
