// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/DataSet/SampleStreamVector.cc"
#include "Ravl/config.h" 
#include "Ravl/PatternRec/SampleStreamVector.hh"
#include "Ravl/DList.hh"
#include "Ravl/MeanCovariance.hh"
#include "Ravl/MatrixRUT.hh"
#include "Ravl/PatternRec/SampleVector.hh"
#include "Ravl/DP/Compose.hh"
#include "Ravl/DP/SPortAttach.hh"
#include "Ravl/DP/Func2Proc21.hh"
#include "Ravl/PatternRec/ProcessVectorFunction.hh"

#if RAVL_COMPILER_MIPSPRO 
#pragma instantiate RavlN::DPIPortBodyC<RavlN::VectorC>
#endif


#define DODEBUG	0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  static VectorC float2realVec(const TVectorC<float> &vec) {
    return VectorC(vec);
  }
  
  
  //: Convert from a stream of float vectors.
  SampleStreamVectorC::SampleStreamVectorC(const SampleStreamC<TVectorC<float> > &port)
    : DPEntityC(true)
  {
    *this = SPort(port >> Process(float2realVec));
  }


  //: Convert from a stream of float vectors.
  SampleStreamVectorC::SampleStreamVectorC(const DPISPortC<VectorC> & stream, const FunctionC & function)
    : DPEntityC(true)
  {    
    ProcessVectorFunctionC pFunc(function);    
    *this = SPort(stream >> pFunc);
  }
  

  //: Compute the sum of the outerproducts.
  // This routine increases accuracy by only summing 500 numbers at time.   Not really worth it.
  // For 1000000 numbers the difference in sums is about 10e-14
  
  MatrixC SampleStreamVectorC::SumOuterProducts() {
    if(!First()) { // Goto first element.
      std::cerr << "SampleStreamVectorC::SumOuterProducts(), WARNING: Failed to seek to first element in sample stream.\n";
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
  
  MeanCovarianceC SampleStreamVectorC::MeanCovariance()  {
    if(!First()) { // Goto first element.
      std::cerr << "SampleStreamVectorC::MeanCovariance(), WARNING: Failed to seek to first element in sample stream.\n";
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
  

  

}
