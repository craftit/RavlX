// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Statistics/MeanCovariance/MeanCovariance.cc"

#include "Ravl/MeanNd.hh"
#include "Ravl/MatrixRS.hh"
#include "Ravl/Vector.hh"
#include "Ravl/MeanCovariance.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/Stream.hh"
#include "Ravl/StdConst.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/MTLocks.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  //: Compute the mean and covariance of an array of vectors.
  
  MeanCovarianceC::MeanCovarianceC(const SArray1dC<VectorC> & data,bool sampleStatistics) 
    : m(data),
      m_haveInvCov(false)
  {
    //ONDEBUG(cerr << "MeanCovarianceC::MeanCovarianceC(), Called with " << data.Size() << " elements. \n");
    if(data.Size() == 0)
      return;
    m.number = data.Size();
    SArray1dIterC<VectorC> it(data);
    cov = it->OuterProduct();
    m.Mean() = it->Copy();
    it++;
    RealT n = ((RealT) m.number);
    for(;it;it++) {
      m.Mean() += *it;
      cov += it->OuterProduct();
    }
    m.Mean() /= n;
    cov.MulAdd(Mean().OuterProduct(),-n);
    RealT sn = n;
    if(sampleStatistics) sn--;
    cov /= sn;
  }
  
  MeanCovarianceC::MeanCovarianceC(const DListC<VectorC> & data,bool sampleStatistics) 
    : m(data),
      m_haveInvCov(false)
  {
    if(data.Size() == 0)
      return;
    m.number = data.Size();
    DLIterC<VectorC> it(data);
    cov = it->OuterProduct();
    m.Mean() = it->Copy();
    it++;
    RealT n = ((RealT) m.number);
    for(;it;it++) {
      m.Mean() += *it;
      cov += it->OuterProduct();
    }
    m.Mean() /= n;
    cov.MulAdd(Mean().OuterProduct(),-n);
    RealT sn = n;
    if(sampleStatistics) sn--;
    cov /= sn;
  }


  MeanCovarianceC MeanCovarianceC::Copy() const {
    return MeanCovarianceC(Number(), m.Mean().Copy(), cov.Copy());
  }
  
  
  const MeanCovarianceC & MeanCovarianceC::SetZero() {
    m.SetZero();
    cov.Fill(0);
    m_haveInvCov = false;
    return *this;
  }
  
  const MeanCovarianceC & 
  MeanCovarianceC::operator+=(const VectorC & point) {
    // Update the covariance matrix.
    const RealT number = (RealT) Number();
    const RealT p1 = number / (number+1.0);
    const RealT p2 = p1 / (number+1.0);
    cov *= p1;
    cov += (VectorC(point - Mean()).OuterProduct() *= p2);
    
    // Update the mean.
    m += point;
    m_haveInvCov = false;
    
    return *this;
  }
  
  const MeanCovarianceC & 
  MeanCovarianceC::operator-=(const VectorC & point) {
    const RealT number = (RealT) Number();
    const RealT nDen   = number-1.0;
    if (nDen <= 0) {
      SetZero();
    } else {
      // Update the covariance matrix.
      const RealT p1 = number / nDen;
      const RealT p2 = p1 / nDen;
      cov *= p1;
      cov -= (VectorC(point - Mean()).OuterProduct() *= p2);
      
      // Update the mean.
      m -= point;
    }
    m_haveInvCov = false;
    return *this;
  }
  
  const MeanCovarianceC & 
  MeanCovarianceC::operator+=(const MeanNdC & mean) {
    // Update the covariance matrix.
    const RealT number1 = (RealT) Number();
    const RealT number2 = (RealT) mean.Number();
    const RealT nDen    = number1 + number2;
    if (nDen <= 0) { // Both set were obviously empty.
      SetZero();
    } else {
      const RealT p1 = number1 / nDen;
      const RealT p2 = number2 / nDen;
      cov *= p1;
      cov += (VectorC(mean.Mean() - Mean()).OuterProduct() *= p1*p2);
      
      // Update the mean.
      m += mean;
    }
    m_haveInvCov = false;
    return *this;
  }
  
  const MeanCovarianceC &   
  MeanCovarianceC::operator-=(const MeanNdC & mean) {
    const RealT number1 = (RealT) Number();
    const RealT number2 = (RealT) mean.Number();
    const RealT nDen    = number1 - number2;
    if (nDen <= 0) { 
      SetZero();
    } else  {
      // Update the covariance matrix.
      const RealT p1 = number1 / nDen;
      const RealT p2 = number2 / nDen;
      cov *= p1;
      cov -= (VectorC(mean.Mean() - Mean()).OuterProduct() *= p1*p2);
      
      // Update the mean.
      m -= mean;
    }
    m_haveInvCov = false;
    return *this;
  }
  
  const MeanCovarianceC & 
  MeanCovarianceC::operator+=(const MeanCovarianceC & meanCov) {
    // Update the covariance matrix.
    const RealT number1 = (RealT) Number();
    const RealT number2 = (RealT) meanCov.Number();
    const RealT nDen    = number1 + number2;
    if (nDen <= 0) { // Both sets were obviously empty.
      SetZero();
    } else {
      const RealT p1 = number1 / nDen;
      const RealT p2 = number2 / nDen;
      if (&cov != &(meanCov.Covariance())) {
	cov *= p1;
	cov += meanCov.Covariance() * p2;
      }
      cov += (VectorC(meanCov.Mean() - Mean()).OuterProduct() *= (p1*p2));
      
      // Update the mean.
      m += meanCov.m;
    }
    m_haveInvCov = false;
    return *this;
  }
  
  const MeanCovarianceC &  
  MeanCovarianceC::operator-=(const MeanCovarianceC & meanCov) {
    const RealT number1 = (RealT) Number();
    const RealT number2 = (RealT) meanCov.Number();
    const RealT nDen    = number1 - number2;
    if (nDen <= 0) {
      SetZero();
    } else {
	// Update the covariance matrix.
      const RealT p1 = number1 / nDen;
      const RealT p2 = number2 / nDen;
      if (&cov != &(meanCov.Covariance())) {
	cov *= p1;
	cov -= meanCov.Covariance() * p2;
      }
      cov -= (VectorC(meanCov.Mean() - Mean()).OuterProduct() *= p1*p2);
      
      // Update the mean.
      m -= meanCov.m;
    }
    m_haveInvCov = false;
    return *this;
  }
  
  const MeanCovarianceC & 
  MeanCovarianceC::Add(const VectorC & point, const VectorC & var) {
    // Update the covariance matrix.
    const RealT number = (RealT) Number();
    const RealT p1 = number / (number+1.0);
    const RealT p2 = 1.0 / (number+1.0);
    cov *= p1;
    cov.AddDiagonal(var * p2);
    cov += (VectorC(point - Mean()).OuterProduct() *= p1 * p2);
    
    // Update the mean.
    m += point;
    m_haveInvCov = false;
    
    return *this;
  }
  
  const MeanCovarianceC & MeanCovarianceC::Remove(const VectorC & point, const VectorC & var) {
    const RealT number = (RealT) Number();
    const RealT nDen   = number-1.0;
    if (nDen <= 0) {
      SetZero();
    } else {
      // Update the covariance matrix.
      const RealT p1 = number / nDen;
      const RealT p2 = 1.0 / nDen;
      cov *= p1;
      cov.AddDiagonal(var * (-p2));
      cov -= (VectorC(point - Mean()).OuterProduct() *= p1*p2);
      
      // Update the mean.
      m -= point;
    }
    m_haveInvCov = false;
    return *this;
  }
  
  const MeanCovarianceC &MeanCovarianceC::SetSum(const MeanCovarianceC & meanCov1,
                                                 const MeanCovarianceC & meanCov2) {
    m_haveInvCov = false;
    return *this = (meanCov1.Copy()+=meanCov2);
  }
  
  //: Calculate the product of the two probability density functions.
  // This assumes the estimates of the distributions are accurate. (The number
  // of samples is ignored) 
  
  MeanCovarianceC MeanCovarianceC::operator*(const MeanCovarianceC &oth) const {
    MatrixRSC sumCov = Covariance() + oth.Covariance();
    sumCov.InverseIP();
    MatrixRSC cs = Covariance() * sumCov;
    MatrixRSC newCov = cs * oth.Covariance();
    VectorC mean = oth.Covariance() * sumCov * Mean();
    mean += cs * oth.Mean();
    return MeanCovarianceC(Number() + oth.Number(),mean,newCov);
  }
  
  //: Evaluate the value of guassian distribution at 'vec'.
  
  RealT MeanCovarianceC::Gauss(const VectorC &vec) const {
    if(!m_haveInvCov) CacheInverse();
    MatrixRSC invCov = m_cacheInvCov;
    // Check if the matrix could not be inverted
    if (invCov.Cols() != vec.Size())
      return 0.0;
    VectorC diff = vec - m;
    RealT e =  diff.Dot(invCov * diff) / -2;
    RealT det = m_cacheDet;
    if (det < 0.0)
      det = 0.0;
    RealT a = Pow(2 * RavlConstN::pi,(RealT) vec.Size() / 2) * Sqrt(det) ;
    return Exp(e)/a;
  }
  
  //: Compute the Mahalanobis to the point.
  
  RealT MeanCovarianceC::MahalanobisDistance(const VectorC &vec) const {
    if(!m_haveInvCov) CacheInverse();
    MatrixRSC invCov = m_cacheInvCov;
    // Check if the matrix could not be inverted
    if (invCov.Cols() != vec.Size()) {
      if (invCov.Cols() == 0)
        cerr << "MeanCovarianceC::MahalanobisDistance, WARNING: Failed to invert covariance matrix. \n";
      else
        cerr << "MeanCovarianceC::MahalanobisDistance, WARNING: Vector size did not match that of covariance matrix.\n";
      RealT zero = 0.0;
      return 1.0/zero; // Positive infinity.
    }
    VectorC diff = vec - m;
    return Sqrt(diff.Dot(invCov * diff));
  }

  void MeanCovarianceC::CacheInverse() const {
    MTWriteLockC lock; // Make sure we're thread safe.
    if(m_haveInvCov) return ;
    m_cacheInvCov = cov.Inverse(); 
    m_cacheDet = cov.Det();
    m_haveInvCov = true;
  }
  
  
  ostream & operator<<(ostream & outS, const MeanCovarianceC & meanCov) {
    outS << meanCov.MeanNd() << '\n' 
	 << meanCov.Covariance() << '\n';
    return outS;
  }
  
  istream & operator>>(istream & inS, MeanCovarianceC & meanCov) {
    inS >> meanCov.m >> meanCov.cov;
    meanCov.ClearCache();    
    return inS;
  }

  BinOStreamC & operator<<(BinOStreamC & outS, const MeanCovarianceC & meanCov) {
    outS << meanCov.MeanNd() << meanCov.Covariance();
    return outS;
  }
  
  BinIStreamC & operator>>(BinIStreamC & inS, MeanCovarianceC & meanCov) {
    inS >> meanCov.m >> meanCov.cov;
    meanCov.ClearCache();    
    return inS;
  }

  
}



