// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Statistics/MeanCovariance/testMeanCovar.cc"

#include "Ravl/Mean.hh"
#include "Ravl/Collection.hh"
#include "Ravl/MeanVariance.hh"
#include "Ravl/FMean.hh"
#include "Ravl/FMeanCovariance.hh"
#include "Ravl/MeanCovariance2d.hh"
#include "Ravl/MeanCovariance3d.hh"
#include "Ravl/MeanCovariance.hh"
#include "Ravl/Matrix2d.hh"
#include "Ravl/SumsNd2.hh"
#include "Ravl/Sums1d2.hh"
#include "Ravl/Stream.hh"
#include "Ravl/Statistics.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/SysLog.hh"
#include "Ravl/UnitTest.hh"

using namespace RavlN;

int testMean();
int testMeanVar();
int testSums1d2();
int testFMean();
int testFMeanCovar();
int testMeanCovar();
int testMeanCovar2d();

int testStatNormalQ();

const RealT small = 0.000000001;

#if RAVL_HAVE_TEMPLATEINSTANTIATE 
template FMeanC<2>;
template FMeanCovarianceC<2>;
#endif

int main() {
  RAVL_RUN_TEST(testMean());
  RAVL_RUN_TEST(testMeanVar());
  RAVL_RUN_TEST(testSums1d2());
  RAVL_RUN_TEST(testFMean());
  RAVL_RUN_TEST(testFMeanCovar());
  RAVL_RUN_TEST(testMeanCovar());
  RAVL_RUN_TEST(testMeanCovar2d());
  RAVL_RUN_TEST(testStatNormalQ());
  cerr << "Test passed ok.\n";
  return 0;
}


int testMean() {
  MeanC m;
  m += 3;
  m += 5;
  if (m.Mean() != 4) return __LINE__;
  if (m.Number() != 2) return __LINE__;
  MeanC m1;
  m1 += 2;
  m1 += 6;
  m += m1;
  if (m.Mean() != 4) return __LINE__;
  if (m.Number() != 4) return __LINE__;
  return 0;
}
  
int testMeanVar() {
  // Do some simple tests...
  MeanVarianceC mvo;
  MeanVarianceC mv1(1,2,3);
  if(mv1.Variance() != 3) return __LINE__;
  if(mv1.Mean() != 2) return __LINE__;
  if(mv1.Number() != 1) return __LINE__;
  MeanVarianceC mv2(2,2,3);
  mvo = mv1;
  mv1 += mv2;
  if(mv1.Number() != 3) return __LINE__;
  // The mean and variance should be the same.
  if(Abs(mv1.Mean() - mvo.Mean()) > small) return __LINE__;
  if(Abs(mv1.Variance() - mvo.Variance()) > small) return __LINE__;
  mv1 -= mv2;
  if(mv1.Number() != 1) return __LINE__;
  // The mean and variance should be the same.
  if(Abs(mv1.Mean() - mvo.Mean()) > small) return __LINE__;
  if(Abs(mv1.Variance() - mvo.Variance()) > small) return __LINE__;
  //cerr << mv1 << "\n";

  SArray1dC<RealT> data(11);
  RealT var = 0;
  for(int i = 0;i < 11;i++) {
    var += Sqr((RealT) i - 5);
    data[i] = (RealT) i;
  }
  MeanVarianceC mv3(data);
  if(Abs(mv3.Mean() - 5) > 0.00000001) return __LINE__;
  if(Abs(mv3.Variance() - (var/10)) > 0.00000001) return __LINE__;  
  
  MeanVarianceC norm1sig(1,0,1);
  RealT v1 = norm1sig.Probability(-0.5,0.7);
  if(Abs(norm1sig.ProbabilityOfHigherValue(0) - 0.5) > 0.0000001) return __LINE__;
  //cerr << "Prob1=" << norm1sig.ProbabilityOfHigherValue(0) << " v1=" << v1 <<"\n";
  
  MeanVarianceC norm2sig(1,0,4);
  RealT v2 = norm2sig.Probability(-0.5*2,0.7*2);
  //cerr << "Prob2=" << norm2sig.ProbabilityOfHigherValue(0) << " v2=" << v2 <<"\n";
  if(Abs(norm2sig.ProbabilityOfHigherValue(0) - 0.5) > 0.0000001) return __LINE__;
  if(Abs(v1 - v2) > 0.000001) return __LINE__;
  
  if(Abs(norm2sig.ProbabilityOfHigherValue(1000)) > 0.0001) return __LINE__;
  if(Abs(norm2sig.ProbabilityOfHigherValue(-1000) - 1.0) > 0.0001) return __LINE__;
  
  return 0;
}

int testSums1d2()
{
  {
    Sums1d2C sum = Sums1d2C::CreateFromMeanVariance(10,5.0,2.0);
    RAVL_TEST_ALMOST_EQUALS(sum.Mean(),5.0,1e-8);
    RAVL_TEST_ALMOST_EQUALS(sum.Variance(),2.0,1e-8);
  }


  {
    // Check sums work ok.
    Sums1d2C sum;
    MeanVarianceC mvinc;
    MeanVarianceC mva;
    MeanVarianceC mvb;
    CollectionC<RealT> data(16);
    for(int i = 0;i < 11;i++) {
      RealT val = Sqr((RealT) i) - 10;
      sum += val;
      mvinc += val;
      if(i % 1) {
        mva += val;
      } else {
        mvb += val;
      }
      data.Append(val);

      MeanVarianceC added = mva;
      added += mvb;
      MeanVarianceC comp(data.Array(),false);
#if 0
      RavlDebug("Val:%f Sum: %f %f  added:%f %f  inc:%f %f comp:%f %f",
          val,
          sum.Mean(),
          sum.Variance(false),
          added.Mean(),
          added.Variance(),
          mvinc.Mean(),
          mvinc.Variance(),
          comp.Mean(),
          comp.Variance()
          );
#endif

      RAVL_TEST_ALMOST_EQUALS(sum.Mean(),added.Mean(),1e-8);
      RAVL_TEST_ALMOST_EQUALS(sum.Variance(false),added.Variance(),1e-8);
      RAVL_TEST_ALMOST_EQUALS(sum.Mean(),mvinc.Mean(),1e-8);
      RAVL_TEST_ALMOST_EQUALS(sum.Variance(false),mvinc.Variance(),1e-8);
      RAVL_TEST_ALMOST_EQUALS(sum.Mean(),comp.Mean(),1e-8);
      RAVL_TEST_ALMOST_EQUALS(sum.Variance(false),comp.Variance(),1e-8);

    }
  }

  return 0;
}

int testFMean() {
  FMeanC<2> mean;
  mean = Vector2dC(0,1);
  mean += Vector2dC(2,5);
  if((mean.Mean() - Vector2dC(1,3)).SumOfSqr() > 0.00001) return __LINE__;
  if(mean.Number() != 2) return __LINE__;
  //cout << "Mean=" << mean << "\n";  
  return 0;
}

int testFMeanCovar() {
  FMeanCovarianceC<2> meanco;
  meanco = Vector2dC(0,1);
  meanco += Vector2dC(2,5);
  if((meanco.Mean() - Vector2dC(1,3)).SumOfSqr() > 0.00001) return __LINE__;

  SArray1dC<FVectorC<2> > xyz(3);
  FMeanCovarianceC<2> meanco2;
  for(int i = 0;i < 3;i++) {
    Vector2dC test((RealT) i,(RealT) i * 2.0);
    xyz[i] = test;
    meanco2 += test;
  }
  FMeanCovarianceC<2> meanco3(xyz);
#if 0
  cerr << "Covar2=" << meanco2.Covariance() << "\n";
  cerr << "Covar3=" << meanco3.Covariance() << "\n";
#endif
  if((meanco2.Covariance() - meanco3.Covariance()).SumOfSqr() > 0.00001) return __LINE__;  
  return 0;
}


int testMeanCovar2d() {
  // Check product operation.
  MeanCovariance2dC mc1;
  mc1 = Vector2dC(0,0);
  mc1 += Vector2dC(0.5,1);
  mc1 += Vector2dC(-0.5,1);
  mc1 += Vector2dC(0,2);
  MeanCovariance2dC mc2;
  mc2 = Vector2dC(0,0);
  mc2 += Vector2dC(1,0.5);
  mc2 += Vector2dC(1,-0.5);
  mc2 += Vector2dC(2,0);
  
  MeanCovariance2dC mc3 = mc1 * mc2;
  //cerr << "Product=" << mc3 << "\n";
  if((mc3.Mean() - Vector2dC(0.2,0.2)).SumOfSqr() > 0.000001) return __LINE__;
  if((mc3.Covariance() - Matrix2dC(0.1,0,0,0.1)).SumOfSqr() > 0.000001) return __LINE__;
  
  Vector2dC at(0.5,0.5);
  RealT val = mc1.Gauss(at);
  cerr << "Gauss=" << val << "\n";
  return 0;
}

int testMeanCovar() {
  MeanCovarianceC meancov2(2);
  SArray1dC<VectorC > xyz(3);
  SumsNd2C sums(2);
  for(int i = 0;i < 3;i++) {
    VectorC test((RealT) i,i * 2.0);;
    xyz[i] = test;
    meancov2 += test;
    sums += test;
  }
  MeanCovarianceC meancov(xyz,false);
  MeanCovarianceC meancov3 = sums.MeanCovariance(false);
#if 0
  cerr << "Mean1=" << meancov.Mean() << " Mean2=" << meancov2.Mean() << "\n";
  cerr << "Covar=" << meancov.Covariance() << "\n";
  cerr << "Covar2=" << meancov2.Covariance() << "\n";
  cerr << "Covar3=" << meancov3.Covariance() << "\n";
#endif
  if((meancov2.Covariance() - meancov.Covariance()).SumOfSqr() > 0.00001) return __LINE__;
  if(VectorC(meancov2.Mean() - meancov.Mean()).SumOfSqr() > 0.00001) return __LINE__;
  
  if((meancov3.Covariance() - meancov.Covariance()).SumOfSqr() > 0.00001) return __LINE__;
  if(VectorC(meancov3.Mean() - meancov.Mean()).SumOfSqr() > 0.00001) return __LINE__;
  
  MeanCovarianceC smeancov(xyz,true);
  MeanCovarianceC smeancov3 = sums.MeanCovariance(true);
#if 0
  cerr << "Sample Covar=" << smeancov.Covariance() << "\n";
  cerr << "Sample Covar3=" << smeancov3.Covariance() << "\n";
#endif
  if((smeancov3.Covariance() - smeancov.Covariance()).SumOfSqr() > 0.00001) return __LINE__;
  if(VectorC(smeancov3.Mean() - smeancov.Mean()).SumOfSqr() > 0.00001) return __LINE__;
  
  
  MeanCovarianceC mcn1(2);
  mcn1 += VectorC(0,0);
  mcn1 += VectorC(0.5,1);
  mcn1 += VectorC(-0.5,1);
  mcn1 += VectorC(0,2);
  MeanCovarianceC mcn2(2);
  mcn2 += VectorC(0,0);
  mcn2 += VectorC(1,0.5);
  mcn2 += VectorC(1,-0.5);
  mcn2 += VectorC(2,0);

  MeanCovarianceC mcn3 = mcn1 * mcn2;
  // cerr << "Product=" << mcn3 << "\n";
  if((mcn3.Mean() - VectorC(0.2,0.2)).SumOfSqr() > 0.000001) return __LINE__;
  if((mcn3.Covariance() - MatrixC(0.1,0,0,0.1)).SumOfSqr() > 0.000001) return __LINE__;
  return 0;
}

int testStatNormalQ() {
#if 0
  RealT sum = 0;
  for(IntT j = 0;j < 2;j++) {
    DateC start = DateC::NowVirtual();
    for(int i = 0; i < 100000;i++) {
      for(RealT v = -10;v < 10;v += 0.1) {
        sum += StatNormalQ(v,j);
      }
    }
    DateC stop = DateC::NowVirtual();
    cerr << " " << j << " Time=" << (stop - start).Double() << "\n";
  }
#endif
  for(RealT v = -10;v < 10;v += 0.1) {
    if(Abs(StatNormalQ(v,true) - StatNormalQ(v,false)) > 1e-5)
      return __LINE__;
    //cerr << v << " Approx=" << StatNormalQ(v,true) << " True=" << StatNormalQ(v,false) << "\n";
  }
  
  
  return 0;
}

