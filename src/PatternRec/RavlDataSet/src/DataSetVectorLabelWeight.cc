// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/DataSet/DataSetVectorLabelWeight.cc"

#include "Ravl/PatternRec/DataSetVectorLabelWeight.hh"
#include "Ravl/PatternRec/DataSet3Iter.hh"
#include "Ravl/Collection.hh"
#include "Ravl/CollectionIter.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/PatternRec/DataSet1Iter.hh"
#include "Ravl/Tuple3.hh"
#include "Ravl/SumsNd2.hh"

namespace RavlN {

  //: Create a seperate sample for each label.
  
  SArray1dC<DataSet2C<SampleVectorC,SampleC<RealT> > > DataSetVectorLabelWeightBodyC::SeperateLabels() const {
    SArray1dC<DataSet2C<SampleVectorC,SampleC<RealT> > > ret(Sample2().MaxValue()+1);
    for(DataSet3IterC<SampleVectorC,SampleLabelC,SampleC<RealT> > it(Sample1(),Sample2(),Sample3());it;it++)
      ret[it.Data2()].Append(it.Data1(),it.Data3());
    return ret;
  }
  
  VectorC DataSetVectorLabelWeightBodyC::GlobalMean() const {
    return Sample1().Mean(Sample3());
  }
  
  SArray1dC<VectorC> DataSetVectorLabelWeightBodyC::ClassMeans () const {
    CollectionC<Tuple2C<VectorC,RealT> > means(10);
    DataSet3IterC<SampleVectorC,SampleLabelC,SampleC<RealT> > it(Sample1(),Sample2(),Sample3());
    UIntT dim = it.Data1().Size();
    for(;it;it++) {
      if(means.Size() <= it.Data2()) {
	VectorC vec(dim);
	vec.Fill(0);
	means.Insert(Tuple2C<VectorC,RealT>(vec,0));
      }
      means[it.Data2()].Data1().MulAdd(it.Data1(),it.Data3());
      means[it.Data2()].Data2() += it.Data3();
    }
    SArray1dC<VectorC> ret(means.Size());
    for(SArray1dIter2C<Tuple2C<VectorC,RealT>,VectorC > it(means.Array(),ret);it;it++)
      it.Data2() = it.Data1().Data1() / it.Data1().Data2();
    return ret;
  }
  
  DataSetVectorLabelC DataSetVectorLabelWeightBodyC::ClassMeansLabels() const {
    SampleVectorC means(Sample2().MaxValue()+1);
    SampleLabelC  labels(Sample2().MaxValue()+1);
    SArray1dC<VectorC> smeans = ClassMeans();
    means.Append(smeans);
    for (UIntT i = 0;i < smeans.Size();i++)
      labels.Append(i);
    return DataSetVectorLabelC(means, labels);
  }

  SArray1dC<UIntT> DataSetVectorLabelWeightBodyC::ClassNums ()  const {
    CollectionC<UIntT> num(10);
    for(DataSet1IterC<SampleLabelC> it(Sample2());it;it++) {
      if(num.Size() <= *it)
	num.Insert(0);
      num[*it]++;
    }
    return num.Array();
  }
  
  SArray1dC<MeanCovarianceC> DataSetVectorLabelWeightBodyC::ClassStats (bool sampleStatistics) const {
    CollectionC<SumsNd2C> stats(10);
    DataSet3IterC<SampleVectorC,SampleLabelC,SampleC<RealT> > it(Sample1(),Sample2(),Sample3());
    if(!it) return SArray1dC<MeanCovarianceC>();
    UIntT dim = it.Data1().Size();
    for(;it;it++) {
      if(stats.Size() <= it.Data2())	
	stats.Insert(SumsNd2C(dim));
      stats[it.Data2()].Add(it.Data1(),it.Data3());
    }
    SArray1dC<MeanCovarianceC> meancovs(stats.Size());
    for(SArray1dIter2C<MeanCovarianceC,SumsNd2C > it(meancovs,stats.Array());it;it++)
      it.Data1() = it.Data2().MeanCovariance(sampleStatistics);
    return meancovs;
  }
    
  MatrixC DataSetVectorLabelWeightBodyC::BetweenClassScatter () const {
    CollectionC<Tuple2C<VectorC,RealT> > means(10);
    DataSet3IterC<SampleVectorC,SampleLabelC,SampleC<RealT> > it(Sample1(),Sample2(),Sample3());
    UIntT dim = it.Data1().Size();
    for(;it;it++) {
      if(means.Size() <= it.Data2()) {
	VectorC vec(dim);
	vec.Fill(0);
	means.Insert(Tuple2C<VectorC,RealT>(vec,0));
      }
      means[it.Data2()].Data1().MulAdd(it.Data1(),it.Data3());
      means[it.Data2()].Data2() += it.Data3();
    }
    SArray1dIterC<Tuple2C<VectorC,RealT> > mit(means.Array());
    VectorC globalMean = mit->Data1().Copy();
    RealT total = mit->Data2();
    for(mit++;mit;mit++) {
      globalMean += mit->Data1();
      total += mit->Data2();
    }
    globalMean /= total;
    MatrixC Sb(globalMean.Size(),globalMean.Size(),0.0);
    for(mit.First();mit;mit++) {
      VectorC diff = (mit->Data1() / mit->Data2()) - globalMean;
      Sb.AddOuterProduct(diff,diff);
    }
    return Sb;
  }

  MatrixC DataSetVectorLabelWeightBodyC::WithinClassScatter (bool sampleStatistics)  const {
    SArray1dC<MeanCovarianceC> stats = ClassStats(sampleStatistics);
    SArray1dIterC<MeanCovarianceC> it (stats);
    if(!it) return MatrixC();
    MatrixC Sw = it->Covariance() * it->Number();
    RealT total = it->Number();
    for (it++; it; it++) {
      total += it->Number();
      Sw.MulAdd(it->Covariance(),it->Number());
    }
    Sw /= total;
    return Sw;
  }
  
  DataSetVectorLabelWeightC DataSetVectorLabelWeightBodyC::ExtractPerLabel(UIntT numSamples)  const {
    RavlAssertMsg(0,"Not implemented. ");
    return DataSetVectorLabelWeightC();
  }

    

}

