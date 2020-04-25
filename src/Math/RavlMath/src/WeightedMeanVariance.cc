// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////
//! lib=RavlMath
//! file="Ravl/Math/Statistics/MeanCovariance/WeightedMeanVariance.cc"

#include "Ravl/WeightedMeanVariance.hh"
#include "Ravl/Statistics.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/StdConst.hh"
#include "Ravl/StdMath.hh"
#include "Ravl/BinStream.hh"

namespace RavlN {
  
  //: Calculate the mean and variance from an array of numbers.
  
  WeightedMeanVarianceC::WeightedMeanVarianceC(const SArray1dC<RealT> &data,bool sampleStatistics) {
    n = data.Size();
    var = 0;
    RealT sum = 0;
    for(SArray1dIterC<RealT> it(data);it;it++) {
      sum += *it;
      var += Sqr(*it);
    }
    RealT rn = (RealT) n;
    mean = sum / rn;
    RealT sn = rn;
    if(sampleStatistics) sn--;
    var = (var - Sqr(sum)/rn)/sn;
  }

  //: Add another WeightedMeanVariance to this one.
  
  WeightedMeanVarianceC &WeightedMeanVarianceC::operator+=(const WeightedMeanVarianceC &mv) {
    if(mv.Number() == 0)
      return *this;
    const RealT number1 = (RealT) Number();
    const RealT number2 = (RealT) mv.Number();
    const RealT nDen    = number1 + number2;
    const RealT p1 = number1 / nDen;
    const RealT p2 = number2 / nDen;
    
    var *= p1;
    var += mv.Variance() * p2;
    var += ((mv.Mean() - Mean()) * p1*p2);
    
    // Update the mean.
    mean = mean * p1 + mv.Mean() * p2;
    n += mv.Number();
    return *this;
  }

  //: Add another sample
  WeightedMeanVarianceC &WeightedMeanVarianceC::operator+=(const RealT &value) {
    RealT rn = n;
    n += 1;
    RealT delta = value - mean;
    mean += delta/n;
    var = (var * (rn) + (delta*(value - mean)))/n;
    return *this;
  }

  //: Remove another WeightedMeanVariance from this one.
  
  WeightedMeanVarianceC &WeightedMeanVarianceC::operator-=(const WeightedMeanVarianceC &mv) {
    if(mv.Number() == 0)
      return *this;
    const RealT number1 = (RealT) Number();
    const RealT number2 = (RealT) mv.Number();
    const RealT nDen    = number1 - number2;
    const RealT p1 = nDen / number1;
    const RealT p2 = number2 / number1;
    
    // Update the mean.
    mean = (mean - mv.Mean() * p2) / p1 ;
    
    // Update the variance.
    var -= ((mv.Mean() - Mean()) * p1*p2);
    var -= mv.Variance() * p2;
    var /= p1;
    
    n -= mv.Number();
    return *this;
  }
  

  //: Add another sample, discounting existing samples by 'discount'

  void WeightedMeanVarianceC::AddExponentialDecay(RealT discount,RealT value)
  {
    n *= discount;
    RealT rn = n;
    n += 1.0;
    RealT delta = value - mean;
    mean += delta/n;
    var = (var * rn + (delta*(value - mean)))/n;
  }

  //: Add another sample with the given weight

  void WeightedMeanVarianceC::AddWeighted(RealT value,RealT weight)
  {
    RealT rn = n;
    n += weight;
    RealT delta = weight *  (value - mean);
    mean += delta/n;
    var = (var * (rn) + (delta*(value - mean)))/n;
  }

  //: Add another sample, discounting all previous samples by 'discount'
  void WeightedMeanVarianceC::AddWeightedExponentialDecay(RealT discount,RealT value,RealT weight)
  {
    n *= discount;
    RealT rn = n;
    n += weight;
    RealT delta = weight * (value - mean);
    mean += delta/n;
    var = (var * rn + (delta*(value - mean)))/n;
  }

  //: Value of the gauss distribution at x.
  
  RealT WeightedMeanVarianceC::Gauss(RealT x) const {
    RealT sig = Sqrt(var);
    return Exp(-0.5 * Sqr((x-mean)/sig)) /(sig * RavlConstN::sqrt2Pi);
  }

  //: Find the probability of getting a sample with value 'at' +/- delta.
  
  RealT WeightedMeanVarianceC::Probability(RealT low,RealT high) const {
    RealT sig = Sqrt(var);
    return (StatNormalQ((low-mean)/sig) - StatNormalQ((high-mean)/sig));
  }
  
  //: Compute the probability of a value higher than a threshold
  
  RealT WeightedMeanVarianceC::ProbabilityOfHigherValue(RealT threshold,bool quickApprox) const {
    // Deal with 0 variance case.
    if(var <= 0) {
      if(mean == threshold)
	return 0.5;
      if(mean < threshold)
	return 0;
      return 1.0;
    }
    // And the rest.
    RealT sig = Sqrt(var);
    return StatNormalQ((threshold-mean)/sig,quickApprox);
  }

  //: Calculate the product of the two probability density functions.
  // (The number of samples is ignored)
  
  WeightedMeanVarianceC WeightedMeanVarianceC::operator*(const WeightedMeanVarianceC &oth) const {
    RealT sum = Variance() + oth.Variance();
    RealT newMean = (Variance() * oth.Mean() / sum) + 
      (oth.Variance() * Mean() / sum);
    RealT newVar = Variance() * oth.Variance() / sum;
    return WeightedMeanVarianceC(Number() + oth.Number(),newMean,newVar);
  }
  
  ostream& operator<<(ostream &s,const WeightedMeanVarianceC &mv) {
    s << mv.Number() << ' ' << mv.Mean() << ' ' << mv.Variance();
    return s;
  }

  istream& operator>>(istream &s, WeightedMeanVarianceC &mv) {
    SizeT n;
    RealT v1,v2;
    s >> n >> v1 >> v2;
    mv = WeightedMeanVarianceC(n,v1,v2);
    return s;
  }

  BinOStreamC& operator<<(BinOStreamC &s,const WeightedMeanVarianceC &mv) {
    s << mv.Number() << mv.Mean() << mv.Variance();
    return s;
  }
  
  BinIStreamC& operator>>(BinIStreamC &s, WeightedMeanVarianceC &mv) {
    SizeT n;
    RealT v1,v2;
    s >> n >> v1 >> v2;
    mv = WeightedMeanVarianceC(n,v1,v2);
    return s;    
  }

  
}
