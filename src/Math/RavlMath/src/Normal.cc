// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Statistics/MeanCovariance/Normal.cc"

#include "Ravl/Normal.hh"
#include "Ravl/StdConst.hh"
#include "Ravl/BinStream.hh"

namespace RavlN {
  
  //: Setup the distribution.
  // Use the mean and covariance to setup the distribution.
  // This may fail if the covariance is singular.
  
  bool NormalC::Set(const MeanCovarianceC &meanCov,bool fixSingular) {
    MatrixC temp = meanCov.Covariance().Copy();
    mean = meanCov.Mean();
    RealT aver = temp.Sum() / Sqr(temp.Rows());
    // Check for small variances
    RealT thresh = (1e-6 * aver);
    for (UIntT i2 = 0; i2 < temp.Rows(); i2++) {
      RealT &val = temp[i2][i2];
      if (val < thresh) {
	if (!fixSingular) {
	  cerr << "NormalC::Set(), WARNING: Singular covariance matrix\n";
	  cerr << mean << endl << temp << endl;
	  return false;
	}
	val = thresh;
      }
    }
    invCov = temp.Inverse(); // precompute inverse cov
    RealT det = temp.Det();  // precompute cov determinant
    
    // Pre calculate the denominator.
    d = Pow(2*RavlConstN::pi,mean.Size()/2.0)*sqrt(det);
    return true;
  }
  
  //: Get value of normal distribution at 'vec'.
  
  RealT NormalC::Evaluate(const VectorC &vec) {
    VectorC x = vec - mean;   // dist from class i mean
    return Exp(-(x.Dot(invCov*x))/2.0) / d;
  }
  
  ostream &operator<<(ostream &s,const NormalC &norm) {
    s << norm.d << ' ' << norm.mean << ' ' << norm.invCov;
    return s;
  }
  
  istream &operator>>(istream &s,NormalC &norm) {
    s >> norm.d >> norm.mean >> norm.invCov;
    return s;
  }
  
  BinOStreamC &operator<<(BinOStreamC &s,const NormalC &norm) {
    s << norm.d << norm.mean << norm.invCov;
    return s;
  }
  
  BinIStreamC &operator>>(BinIStreamC &s,NormalC &norm) {
    s >> norm.d >> norm.mean >> norm.invCov;
    return s;
  }

  
}
