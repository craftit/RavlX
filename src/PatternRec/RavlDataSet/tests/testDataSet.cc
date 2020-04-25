// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/DataSet/testDataSet.cc"
//! docentry="Ravl.API.Pattern Recognition.Data Set"
//! userlevel=Develop

#include "Ravl/PatternRec/DataSet1.hh"
#include "Ravl/PatternRec/DataSet2.hh"
#include "Ravl/PatternRec/DataSet3.hh"
#include "Ravl/PatternRec/DataSet4.hh"
#include "Ravl/PatternRec/DataSet1Iter.hh"
#include "Ravl/PatternRec/DataSet2Iter.hh"
#include "Ravl/PatternRec/DataSet3Iter.hh"
#include "Ravl/PatternRec/DataSet4Iter.hh"
#include "Ravl/PatternRec/SampleIO.hh"
#include "Ravl/PatternRec/SampleVector.hh"
#include "Ravl/PatternRec/SampleLabel.hh"
#include "Ravl/PatternRec/SampleIter.hh"
#include "Ravl/PatternRec/SampleStreamVector.hh"
#include "Ravl/PatternRec/DataSetVectorLabel.hh"
#include "Ravl/PatternRec/Function1.hh"
#include "Ravl/PatternRec/FunctionSlice.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/Stream.hh"
#include "Ravl/StrStream.hh"
#include "Ravl/MatrixRUT.hh"
#include "Ravl/MeanCovariance.hh"
#include "Ravl/DP/ListIO.hh"

using namespace RavlN;

#define USE_SPEEDTEST 0

int testSample();
int testSampleIO();
int testDataSet1();
int testDataSet2();
int testDataSet3();
int testDataSet4();
int testSampleVector();
int testSampleStreamVector();
int testSampleStreamVectorFunction();
int testDataSetVectorLabel();

#if USE_SPEEDTEST
#include "Ravl/DList.hh"
int testSpeed();
#endif

#if RAVL_HAVE_TEMPLATEINSTANTIATE 
template DataSet1C<SampleC<IntT> >;
template DataSet2C<SampleC<IntT>,SampleC<RealT> >;
template DataSet3C<SampleC<IntT>,SampleC<RealT>,SampleC<bool> >;
#endif

int main() {
  int ln;
  if((ln = testSample()) != 0) {
    std::cerr << "Test failed line " << ln << "\n";
    return 1;
  }
  if((ln = testSampleIO()) != 0) {
    std::cerr << "Test failed line " << ln << "\n";
    return 1;
  }
  if((ln = testDataSet1()) != 0) {
    std::cerr << "Test failed line " << ln << "\n";
    return 1;
  }
  if((ln = testDataSet2()) != 0) {
    std::cerr << "Test failed line " << ln << "\n";
    return 1;
  }
  if((ln = testDataSet3()) != 0) {
    std::cerr << "Test failed line " << ln << "\n";
    return 1;
  }
  if((ln = testDataSet4()) != 0) {
    std::cerr << "Test failed line " << ln << "\n";
    return 1;
  }
  if((ln = testSampleVector()) != 0) {
    std::cerr << "Test failed line " << ln << "\n";
    return 1;
  }
  if((ln = testDataSetVectorLabel()) != 0) {
    std::cerr << "Test failed line " << ln << "\n";
    return 1;
  }
  if((ln = testSampleStreamVector()) != 0) {
    std::cerr << "Test failed line " << ln << "\n";
    return 1;
  }
  if((ln = testSampleStreamVectorFunction()) != 0) {
    std::cerr << "Test failed line " << ln << "\n";
    return 1;
  }
#if USE_SPEEDTEST
  if((ln = testSpeed()) != 0) {
    std::cerr << "Test failed line " << ln << "\n";
    return 1;
  }
#endif
  std::cout << "Test passed ok. \n";
  return 0;
}

int testSample() {
  SampleC<IntT> sample;
  int i = 0;
  for(;i < 10;i++)
    sample.Append(i);
  i = 0;
  for(DArray1dIterC<IntT> it(sample.DArray());it;it++,i++)
    if(*it != i) return __LINE__;
  if(sample.Size() != 10) return __LINE__;
  return 0;
}

int testSampleIO() {
  SampleC<IntT> sample;
  sample.Append(10);
  sample.Append(11);
  sample.Append(12);
  FilenameC tmpFile("/tmp/test.strm");
  if(!SaveSample(tmpFile,sample,"",true))
    return __LINE__;
  SampleC<IntT> sample2;
  if(!LoadSample(tmpFile,sample2,((UIntT) -1),"",true))
    return __LINE__;
  if(sample.Size() != sample2.Size())
    return __LINE__;
  // Should check the loaded set.
  tmpFile.Remove();
  
  StringC dataStr;
  {
    StrOStreamC ostr;
    ostr << sample;
    dataStr = ostr.String();
  }
  SampleC<IntT> sample3;
  StrIStreamC istr(dataStr);
  istr >> sample3;
  //cerr << "DataStr=" << dataStr << "\n";
  if(sample.Size() != sample3.Size()) {
    std::cerr << "Size1=" << sample.Size() << " Size3=" << sample3.Size() << "\n";
    return __LINE__;
  }
  
  return 0;
}

int testDataSet1() {
  DataSet1C<SampleC<IntT> > dataset(true);
  dataset.Append(10);
  dataset.Append(11);
  dataset.Append(12);
  dataset.Append(13);
  UIntT count = 0;
  for(DataSet1IterC<SampleC<IntT> > it(dataset);it;it++)
    count++;
  if(count != 4) return __LINE__;
  return 0;
}

int testDataSet2() {
  DataSet2C<SampleC<IntT>,SampleC<RealT> > dataset(true);
  dataset.Append(10,0.3);
  dataset.Append(11,0.4);
  UIntT count = 0;
  for(DataSet2IterC<SampleC<IntT>,SampleC<RealT> > it(dataset);it;it++)
    count++;
  if(count != 2) return __LINE__;

  DataSet2C<SampleVectorC, SampleLabelC>dset(10);
  VectorC x1(1.0, 2.0);
  VectorC x2(3.0, 4.0);
  dset.Append(x1, 0);
  dset.Append(x2, 0);
  std::cout << dset << std::endl;
  DataSet2C<SampleVectorC, SampleLabelC>dset2(10);
  dset2.Append(dset);
  std::cout << dset2.Sample1() << std::endl;

  return 0;
}

int testDataSet3() {
  DataSet3C<SampleC<IntT>,SampleC<RealT>,SampleC<bool> > dataset(true);
  dataset.Append(10,0.3,true);
  dataset.Append(11,0.4,false);
  dataset.Append(12,0.5,false);
  UIntT count = 0;
  for(DataSet3IterC<SampleC<IntT>,SampleC<RealT>,SampleC<bool> > it(dataset);it;it++)
    count++;
  if(count != 3) return __LINE__;
  return 0;
}

int testDataSet4() {
  DataSet4C<SampleC<IntT>,SampleC<RealT>,SampleC<bool>,SampleC<char> > dataset(true);
  dataset.Append(10,0.3,true,'a');
  dataset.Append(11,0.4,false,'b');
  dataset.Append(12,0.5,false,'c');
  UIntT count = 0;
  for(DataSet4IterC<SampleC<IntT>,SampleC<RealT>,SampleC<bool>,SampleC<char> > it(dataset);it;it++)
    count++;
  if(count != 3) return __LINE__;
  return 0;
}

int testSampleVector() {
  std::cerr << "testSampleVector() \n";
  SampleVectorC sv;
  sv += VectorC(1,2,3);
  sv += VectorC(1,1,1);
  if(sv.Size() != 2) return __LINE__;
  MatrixRUTC sop = sv.SumOuterProducts();
  sop.MakeSymmetric();
  MatrixC t(3,3);
  t.Fill(0);
  t += VectorC(1,2,3).OuterProduct();
  t += VectorC(1,1,1).OuterProduct();
  if((sop - t).SumOfSqr() > 0.0000001) return __LINE__;
  //cerr << "SOP=" << sop << "\n";;
  
  // Check mean covariance computation.
  MeanCovarianceC mc = sv.MeanCovariance(false);
  MeanCovarianceC cmc(3);
  cmc += VectorC(1,2,3);
  cmc += VectorC(1,1,1);
#if 0
  std::cerr << "sv mc=" << mc << "\n";
  std::cerr << "lc mc=" << cmc << "\n";
#endif
  
  if((mc.Covariance() - cmc.Covariance()).SumOfSqr() > 0.00001) return __LINE__;
  if((mc.Mean() - cmc.Mean()).SumOfSqr() > 0.00001) return __LINE__;
  if(mc.Number() != cmc.Number()) return __LINE__;

  StringC dataStr;
  {
    StrOStreamC ostr;
    ostr << sv;
    dataStr = ostr.String();
  }
  StrIStreamC istr(dataStr);
  SampleVectorC sv2;
  istr >> sv2;
  if(sv2.Size() != sv.Size()) return __LINE__;
  return 0;
}

int testDataSetVectorLabel() {
  std::cerr << "testDataSetVectorLabel()\n";
  DataSetVectorLabelC svl(2);
  svl.Append(VectorC(2,2,3),1);
  svl.Append(VectorC(2,1,1),1);
  svl.Append(VectorC(3,4,4),1);
  svl.Append(VectorC(1,2,3),0);
  svl.Append(VectorC(2,1,1),0);
  
  SArray1dC<UIntT> nums = svl.ClassNums();
  if(nums.Size() != 2) return __LINE__;
  if(nums[0] != 2) return __LINE__;
  if(nums[1] != 3) return __LINE__;
  SArray1dC<VectorC> means = svl.ClassMeans();
  if(means.Size() != 2) return __LINE__;
  SArray1dC<MeanCovarianceC> stats = svl.ClassStats(false);
  if(stats.Size() != 2) return __LINE__;
  // Check mean covariance computation.
  MeanCovarianceC mc = stats[0];
  MeanCovarianceC cmc(3);
  cmc += VectorC(1,2,3);
  cmc += VectorC(2,1,1);
  if((mc.Covariance() - cmc.Covariance()).SumOfSqr() > 0.00001) return __LINE__;
  if((mc.Mean() - cmc.Mean()).SumOfSqr() > 0.00001) return __LINE__;
  if((means[0] - cmc.Mean()).SumOfSqr() > 0.00001) return __LINE__;
  if(mc.Number() != cmc.Number()) return __LINE__;
  
  mc = stats[1];
  MeanCovarianceC cmc1(3);
  cmc1 += VectorC(2,2,3);
  cmc1 += VectorC(2,1,1);
  cmc1 += VectorC(3,4,4);
  if((mc.Covariance() - cmc1.Covariance()).SumOfSqr() > 0.00001) return __LINE__;
  if((mc.Mean() - cmc1.Mean()).SumOfSqr() > 0.00001) return __LINE__;
  if((means[1] - cmc1.Mean()).SumOfSqr() > 0.00001) return __LINE__;
  if(mc.Number() != cmc1.Number()) return __LINE__;
  
  MeanCovarianceC global = cmc.Copy();
  global += cmc1;
  VectorC lglobal = svl.GlobalMean();
  if((lglobal - global.Mean()).SumOfSqr() > 0.000001) return __LINE__;
  
  MatrixC tbcs = VectorC(cmc.Mean() - global.Mean()).OuterProduct();
  tbcs += VectorC(cmc1.Mean() - global.Mean()).OuterProduct();
  
  std::cerr << "tbcs=" << tbcs << "\n";
  MatrixC bcs = svl.BetweenClassScatter();
  std::cerr << "bcs=" << bcs << "\n";
  if((tbcs - bcs).SumOfSqr() > 0.00001) return __LINE__;
  
  MatrixC tics = cmc1.Covariance() * (3.0/5.0) + cmc.Covariance() * (2.0/5.0);
  MatrixC ics = svl.WithinClassScatter(false);
  std::cerr << "tics=" << tics << "\n";
  std::cerr << "ics=" << ics << "\n";
  if((tics - ics).SumOfSqr() > 0.00001) return __LINE__;
  
  return 0;
}

int testSampleStreamVector() {
  //cerr << "testSampleStreamVector(), Called. \n";
  DListC<VectorC> lst;
  MatrixRUTC accum(4);
  accum.Fill(0);
  MeanCovarianceC mc(4);
  for(int i = 0;i < 100000;i++) {
    VectorC vec = RandomVector(4,1000);
    accum.AddOuterProduct(vec);
    lst.InsLast(vec);
    mc += vec;
  }
  DPISListC<VectorC> vecs(lst);
  SampleStreamVectorC ssv(vecs);
  accum.MakeSymmetric();
  MatrixRUTC accum2 = ssv.SumOuterProducts();
  //cerr << "Accum=" << accum << "\n";
  //cerr << "Accum2=" << accum2 << "\n";
  //cerr << "Diff=" << (accum - accum2) << "\n";
  if((accum - accum2).SumOfSqr() > 0.01) return __LINE__;
  MeanCovarianceC mcn = ssv.MeanCovariance();
  //cerr << "mc=" << mc << "\n";
  //cerr << "mnc=" << mcn << "\n";
  MeanCovarianceC lmc(lst);
  //cerr << "lmc=" << mcn << "\n";
  if((mc.Mean() - mc.Mean()).SumOfSqr() > 0.0001) return __LINE__;
  if((mc.Covariance() - mc.Covariance()).SumOfSqr() > 0.0001) return __LINE__;
  return 0;
}

int testSampleStreamVectorFunction() {
  DListC<VectorC> lst;
  MatrixRUTC accum(4);
  accum.Fill(0);
  MeanCovarianceC mc(4);
  for(int i = 0;i < 10000;i++) {
    VectorC vec = RandomVector(4,1000);
    accum.AddOuterProduct(vec);
    lst.InsLast(vec);
    mc += vec;
  }
  accum.MakeSymmetric();
  DPISListC<VectorC> vecs(lst);
  // Just get a slice
  IndexRangeC range(1,2);
  FunctionSliceC slice(4, range);
  SampleStreamVectorC ssv(vecs, slice);
  VectorC vec;
  MatrixRUTC accum2 = ssv.SumOuterProducts();
  //cerr << accum << std::endl;
  //cerr << accum2 << std::endl;
  MatrixC mat(accum[1][1], accum[1][2], accum[2][1], accum[2][2]);
  if((mat - accum2).SumOfSqr() > 0.1) return __LINE__;

    return 0;
}

#if USE_SPEEDTEST
const int testSize = 1000000;
int testSpeed() {
  DataSet1C<SampleC<IntT> > dataset1(true);
  DateC start(true);
  for(int i = 0;i < testSize;i++) 
    dataset1.Append(i);
  DateC end(true);
  std::cerr << "Create Time1 = " << (end.Double() - start.Double()) << "\n";
  
  DataSet2C<SampleC<IntT>,SampleC<RealT> > dataset2(true);
  start.SetToNow();
  for(int i = 0;i < testSize;i++) 
    dataset2.Append(i,0.3);
  end.SetToNow();
  std::cerr << "Create Time2 = " << (end.Double() - start.Double()) << "\n";
  
  DataSet3C<SampleC<IntT>,SampleC<RealT>,SampleC<bool> > dataset3(true);
  start.SetToNow();
  for(int i = 0;i < testSize;i++) 
    dataset3.Append(i,0.3,true);
  end.SetToNow();
  std::cerr << "Create Time3 = " << (end.Double() - start.Double()) << "\n";

  start.SetToNow();
  DListC<int> aList;
  for(int i = 0;i < testSize;i++) 
    aList.InsLast(i);
  end.SetToNow();
  std::cerr << "DList Create Time = " << (end.Double() - start.Double()) << "\n";
  
  UIntT val = 0;
  start.SetToNow();
  for(DataSet1IterC<SampleC<IntT> > it1(dataset1);it1;it1++)
    val += it1.Data();
  end.SetToNow();
  std::cerr << "Iter Time1 = " << (end.Double() - start.Double()) << "\n";
  
  start.SetToNow();
  for(DataSet2IterC<SampleC<IntT>,SampleC<RealT> > it2(dataset2);it2;it2++) {
    val += it2.Data1() + it2.Data2();
  }
  end.SetToNow();
  std::cerr << "Iter Time2 = " << (end.Double() - start.Double()) << "\n";

  start.SetToNow();
  for(DataSet3IterC<SampleC<IntT>,SampleC<RealT>,SampleC<bool> > it3(dataset3);it3;it3++) {
    val += it3.Data1() + it3.Data2() + ((int) it3.Data3());
  }
  end.SetToNow();
  std::cerr << "Iter Time3 = " << (end.Double() - start.Double()) << "\n";
  start.SetToNow();
  for(DLIterC<int > itl(aList);itl;itl++)
    val += itl.Data();
  end.SetToNow();

  std::cerr << "DList Iter Time = " << (end.Double() - start.Double()) << "\n";
  
  return 0;
}


#endif
