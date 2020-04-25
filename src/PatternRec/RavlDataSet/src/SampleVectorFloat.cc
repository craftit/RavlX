// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id: SampleVector.cc 5410 2006-03-12 21:37:43Z craftit $"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/DataSet/SampleVector.cc"

#include "Ravl/PatternRec/SampleVectorFloat.hh"
#include "Ravl/DArray1dIter.hh"
#include "Ravl/DArray1dIter2.hh"
#include "Ravl/DArray1dIter3.hh"
#include "Ravl/MeanCovariance.hh"
#include "Ravl/MatrixRUT.hh"
#include "Ravl/PatternRec/DataSet2Iter.hh"
#include "Ravl/PatternRec/SampleIter.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/SumsNd2.hh"

namespace RavlN {

  //: Construct a new sample set with a reduced set of features
  
  SampleVectorFloatC::SampleVectorFloatC(const SampleC<TVectorC<float> > &svec, const SArray1dC<IndexC> &featureSet)
    : SampleC<TVectorC<float> >(svec.Size())
  {
    UIntT numFeatures = featureSet.Size();
    for(DataSet2IterC<SampleC<TVectorC<float> >,SampleC<TVectorC<float> > > it(svec,*this); it; it++) {
      TVectorC<float>  out(numFeatures);
      for(SArray1dIter2C<IndexC,float> itf(featureSet,out); itf; itf++)
	itf.Data2() = it.Data1()[itf.Data1()];
      it.Data2() = out;
    }
  }
  
  //: Get the size of vectors in this sample.
  
  UIntT SampleVectorFloatC::VectorSize() const {
    if(IsEmpty())
      return 0;
    return First().Size();
  }

  //: Find the mean vector.
  
  VectorC SampleVectorFloatC::Mean() const {
    DArray1dIterC<TVectorC<float> > it(*this);
    if(!it) return TVectorC<float> ();
    VectorC total(*it);
    for(it++;it;it++)
      total += *it;
    return total/ ((RealT) Size());
  }
  
  //: Find the weighted mean vector of the sample.
  
  VectorC SampleVectorFloatC::Mean(const SampleC<float> &weights) const {
    DArray1dIter2C<TVectorC<float> ,float> it(*this,weights.DArray());
    if(!it) return TVectorC<float> ();
    VectorC total(it.Data1() * it.Data2());
    RealT n = 0;
    for(it++;it;it++) {
      total.MulAdd(it.Data1(),it.Data2());
      n += it.Data2();
    }
    return total / n;
  }
  
  //: Find the mean and covariance of the sample
  
  MeanCovarianceC SampleVectorFloatC::MeanCovariance(bool sampleStatistics) const {
    DArray1dIterC<TVectorC<float> > it(*this);
    if(!it) return MeanCovarianceC();
    SumsNd2C sums(1.0,it->Copy(),OuterProductRUT(*it));
    for(it++;it;it++)
      sums += *it;
    return sums.MeanCovariance(sampleStatistics);
  }
  
  //: Find the mean and covariance of a weighted sample
  
  MeanCovarianceC SampleVectorFloatC::MeanCovariance(const SampleC<float> &weights,bool sampleStatistics) const {
    RavlAssert(Size() == weights.Size());
    DArray1dIter2C<TVectorC<float> ,float> it(*this,weights.DArray());
    if(!it) return MeanCovarianceC();
    SumsNd2C sums(it.Data2(),it.Data1() * it.Data2(),OuterProductRUT(it.Data1(),it.Data2()));
    for(it++;it;it++) 
      sums.Add(it.Data1(),it.Data2());
    return sums.MeanCovariance(sampleStatistics);
  }
  
  //: Compute the sum of the outerproducts.
  
  MatrixRUTC SampleVectorFloatC::SumOuterProducts() const {
    MatrixRUTC ret;
    DArray1dIterC<TVectorC<float> > it(*this);
    if(!it) return ret; // No samples.
    ret = OuterProductRUT(*it);
    for(it++;it;it++)
      ret.AddOuterProduct(*it);
    return ret;
  }
  
  //: Compute the sum of the outerproducts.
  // sam2 must have the same size as this sample vector.
  
  MatrixC SampleVectorFloatC::TMul(const SampleC<TVectorC<float> > &sam2) const {
    RavlAssert(Size() == sam2.Size());
    MatrixC ret;
    DArray1dIter2C<TVectorC<float> ,TVectorC<float> > it(*this,sam2.DArray());
    if(!it) return ret; // No samples.    
    ret = it.Data1().OuterProduct(it.Data2());
    for(it++;it;it++)
      ret.AddOuterProduct(it.Data1(),it.Data2());
    return ret;
  }

  //: Compute the sum of the outerproducts weighting each with the corresponding value from 'w'.
  
  MatrixRUTC SampleVectorFloatC::SumOuterProducts(const SampleC<float> &w) const {
    RavlAssert(Size() == w.Size());
    MatrixRUTC ret;
    DArray1dIter2C<TVectorC<float> ,float> it(*this,w.DArray());
    if(!it) return ret; // No samples.
    ret = OuterProductRUT(it.Data1(),it.Data2());
    for(it++;it;it++)
      ret.AddOuterProduct(it.Data1(),it.Data2());
    return ret;
  }
  
  //: Compute the sum of the outerproducts weighting each with the corresponding value from 'w'.
  // sam2 must have the same size as this sample vector.
  
  MatrixC SampleVectorFloatC::TMul(const SampleC<TVectorC<float> > &sam2,const SampleC<float> &w) const {
    RavlAssert(sam2.Size() == w.Size());
    RavlAssert(Size() == sam2.Size());
    MatrixC ret;
    DArray1dIter3C<TVectorC<float> ,TVectorC<float> ,float> it(*this,sam2.DArray(),w.DArray());
    if(!it) return ret; // No samples.    
    ret = it.Data1().OuterProduct(it.Data2(),it.Data3());
    for(it++;it;it++)
      ret.AddOuterProduct(it.Data1(),it.Data2(),it.Data3());
    return ret;    
  }

  void SampleVectorFloatC::Normalise(const MeanCovarianceC & stats)  {
    if(Size() < 2)
      return ; // Can't normalise only 1 sample!
    UIntT d = VectorSize();
    TVectorC<float>  stdDev(d);
    for(UIntT i=0;i<d;i++) {
      if(stats.Covariance()[i][i] > 0) 
	stdDev[i] = stats.Covariance()[i][i];
      else 
	stdDev[i] = stats.Mean()[i];
    }
    for(UIntT i=0;i<d;i++)
      stdDev[i] = Sqrt(stdDev[i]);
    stdDev.Reciprocal ();
    
    for(SampleIterC<TVectorC<float> >it(*this);it;it++)
      *it =  (*it - stats.Mean()) * stdDev;
  }
  
  //: Undo the normalisation done by 'Normalise()'.
  
  void SampleVectorFloatC::UndoNormalisation(const MeanCovarianceC & stats) {
    if(Size() < 2)
      return ; // Can't normalise only 1 sample!
    UIntT d = VectorSize();
    TVectorC<float>  stdDev(d);
    for(UIntT i=0;i<d;i++) {
      if(stats.Covariance()[i][i] > 0) 
	stdDev[i] = stats.Covariance()[i][i];
      else 
	stdDev[i] = stats.Mean()[i];
    }
    for(UIntT i=0;i<d;i++)
      stdDev[i] = Sqrt(stdDev[i]);
    
    for(SampleIterC<TVectorC<float> > it(*this);it;it++)
      *it =  (*it  * stdDev) + stats.Mean();
  }

  
}

