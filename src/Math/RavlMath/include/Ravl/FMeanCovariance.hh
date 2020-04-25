// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FMEANCOVARIANCE_HEADER
#define RAVL_FMEANCOVARIANCE_HEADER
//////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! userlevel=Normal
//! author="Radek Marik"
//! date="01/01/1994"
//! docentry="Ravl.API.Math.Statistics"
//! lib=RavlMath
//! file="Ravl/Math/Statistics/MeanCovariance/FMeanCovariance.hh"

#include "Ravl/Types.hh"
#include "Ravl/FMean.hh"
#include "Ravl/FMatrix.hh"
#include "Ravl/StdConst.hh"

namespace RavlN {
  template<unsigned int N> class FMeanCovarianceC ;
  template<unsigned int N> istream & operator>>(istream & inS, FMeanCovarianceC<N> & meanCov);
  template<unsigned int N> BinIStreamC & operator>>(BinIStreamC & inS, FMeanCovarianceC<N> & meanCov);

  //! userlevel=Normal
  //: Mean and covariance together in N-D space
  // The class FMeanCovarianceC serves for computation of the mean
  // and the covariance matrix of a set of fixed dimensional data points.

  template<unsigned int N>
  class FMeanCovarianceC {
  public:
    FMeanCovarianceC<N>()
    { cov.Fill(0); }
    //: Constructor.
    // Creates zero mean and zero covariance matrix representing
    // the fixed-dimensional set containing no data points.

    FMeanCovarianceC(const FVectorC<N> & point)
      : m(point)
    { cov.Fill(0); }
    //: Constructor
    // Creates the mean vector and zero covariance matrix representing
    // the data set containing just one data point.

    FMeanCovarianceC(const FMeanC<N> & mean)
      : m(mean)
    { cov.Fill(0); }
    // Creates the mean vector and zero covariance matrix representing
    // the data set represented by the 'mean'.

    FMeanCovarianceC(SizeT  n,
                     const FVectorC<N> & mean,
                     const FMatrixC<N,N> & ncov)
      : m(n,mean),
        cov(ncov)
    {}
    // Creates the mean vector and zero covariance matrix representing
    // the data set containing 'n' points and represented by the 'mean'
    // and the covariance matrix 'cov'.

    FMeanCovarianceC<N> Copy() const;
    //: Returns a new physical copy of this object.

    FMeanCovarianceC<N>(const SArray1dC<FVectorC<N> > & data);
    //: Compute the mean and covariance of an array of vectors.

    //:---------------------------
    // Information about an object

    SizeT Number() const
      { return m.Number(); }
    //: Returns the number of data points which are represented by this object.

    const FVectorC<N> & Mean() const
    { return m.Mean(); }
    //: Access the mean.
    // Returns the mean vector of data points which are represented
    // by this object.

    const FMatrixC<N,N> & Covariance() const
    { return cov; }
    //: Access the covariance.
    // Returns the covariance matrix of data points which are represented
    // by this object.

    // Object modification
    // -------------------

    const FMeanCovarianceC<N> & SetZero();
    //: Total initialization of this object resulting in the representation
    //: the empty set of data points.

    const FMeanCovarianceC<N> & operator+=(const FVectorC<N> & point);
    //: Adds one point to the set of data points.

    const FMeanCovarianceC<N> & operator-=(const FVectorC<N> & point);
    //: Removes one point from the set of data points.
    // Be carefull to remove a point which was already added to the set,
    // otherwise the representation will not describe a real set.

    const FMeanCovarianceC<N> & operator+=(const FMeanC<N> & mean);
    //: Adds a number of data poits represented by the 'mean' and zero
    //: covariance matrix to this set.

    const FMeanCovarianceC<N> & operator-=(const FMeanC<N> & mean);
    //: Removes a number of data poits represented by the 'mean' and
    //: zero covariance matrix from this set.
    // Be carefull to remove
    // points which were already added to the set, otherwise the representation
    // will not describe a real set.

    const FMeanCovarianceC<N> & operator+=(const FMeanCovarianceC<N> & meanCov);
    //: Adds a number of data points represented by the 'meanCov' structure
    //: to this set.

    const FMeanCovarianceC<N> & operator-=(const FMeanCovarianceC<N> & meanCov);
    //: Removes a number of data points represented by the 'meanCov' structure
    //: from this set.
    // Be carefull to remove points which were already added to the set, otherwise
    // the representation will not describe a real set.

    const FMeanCovarianceC<N> & Add(const FVectorC<N> & point, const FVectorC<N> & var);
    // Updates the mean and the covariance matrix by adding one N-d point
    // whose coordinates are known with the error described by the diagonal
    // convariance matrix represented byt the vector 'var'.

    const FMeanCovarianceC<N> &Remove(const FVectorC<N> & point, const FVectorC<N> & var);
    // Updates the mean and the covariance matrix by removing one N-d point
    // whose coordinates are known with the error described by the diagonal
    // convariance matrix represented byt the vector 'var'.
    // Be carefull to remove the point which was already added
    // to the set, otherwise the representation
    // will not describe a real set.

    const FMeanCovarianceC<N> & SetSum(const FMeanCovarianceC<N> & meanCov1,
               const FMeanCovarianceC<N> & meanCov2);
    //: This object is set to be the union of two set of data points 'meanCov1'
    //: and 'meanCov2'.

    FMeanCovarianceC<N> operator*(const FMeanCovarianceC<N> &oth) const;
    //: Calculate the product of the two probability density functions.
    // This assumes the estimates of the distributions are accurate. (The number
    // of samples is ignored)

    RealT Gauss(const FVectorC<N> & point) const;
    //: Evaulate value of normal distribution at given point.

    bool Save(BinOStreamC &outS) const;
    //: Save the object to a binary stream.

    UIntT Hash() const
    { return StdHash(m) + StdHash(cov); }
    //: Provided for compatibility with templates.

  protected:
    FMeanC<N> m;   // The mean vector of this data set.
    FMatrixC<N,N> cov; // the covariance matrix of this data set.

    friend istream & operator>> <>(istream & inS, FMeanCovarianceC<N> & meanCov);
    friend BinIStreamC & operator>> <>(BinIStreamC & inS, FMeanCovarianceC<N> & meanCov);
  };

  template<unsigned int N>
  ostream & operator<<(ostream & outS, const FMeanCovarianceC<N> & meanCov) {
    outS << meanCov.Mean() << '\n'
    << meanCov.Covariance() << '\n';
    return outS;
  }
  // Saves the statistical description of the set 'meanCov' into the output
  // stream 'outS'.

  template<unsigned int N>
  BinOStreamC & operator<<(BinOStreamC & outS, const FMeanCovarianceC<N> & meanCov) {
    meanCov.Save(outS);
    return outS;
  }
  // Saves the statistical description of the set 'meanCov' into the output
  // stream 'outS'.

  template<unsigned int N>
  istream & operator>>(istream & inS, FMeanCovarianceC<N> & meanCov) {
    inS >> meanCov.m >> meanCov.cov;
    return inS;
  }
  // Reads and sets the statistical description of the set 'meanCov'
  // according to the information in the input stream 'inS'.

  template<unsigned int N>
  BinIStreamC & operator>>(BinIStreamC & inS, FMeanCovarianceC<N> & meanCov) {
    inS >> meanCov.m >> meanCov.cov;
    return inS;
  }
  // Reads and sets the statistical description of the set 'meanCov'
  // according to the information in the input stream 'inS'.

  //: Compute the mean and covariance of an array of vectors.

  template<unsigned int N>
  FMeanCovarianceC<N>::FMeanCovarianceC(const SArray1dC<FVectorC<N> > & data)
    : m(data)
  {
    if(Number() == 0)
      return ;
    SArray1dIterC<FVectorC<N> > it(data);
    it->OuterProduct(cov);
    it++;
    FMatrixC<N,N> tmp;
    for(;it;it++)
      cov += it->OuterProduct(tmp);
    cov /= (RealT) Number();
    cov -= Mean().OuterProduct(tmp);
  }

  template<unsigned int N>
  const FMeanCovarianceC<N> & FMeanCovarianceC<N>::SetZero() {
    m.Fill(0);
    cov.Fill(0);
    return *this;
  }

  template<unsigned int N>
  const FMeanCovarianceC<N> &FMeanCovarianceC<N>::operator+=(const FVectorC<N> & point) {
    // Update the covariance matrix.
    const RealT number = (RealT) Number();
    const RealT p1 = number / (number+1.0);
    const RealT p2 = p1 / (number+1.0);
    FMatrixC<N,N> tmp;
    cov *= p1;
    cov += (FVectorC<N>(point - Mean()).OuterProduct(tmp) *= p2);

    // Update the mean.
    m += point;

    return *this;
  }

  template<unsigned int N>
  const FMeanCovarianceC<N> & FMeanCovarianceC<N>::operator-=(const FVectorC<N> & point) {
    const RealT number = (RealT) Number();
    const RealT nDen   = number-1.0;
    if (nDen <= 0) {
      SetZero();
    } else {
      // Update the covariance matrix.
      FMatrixC<N,N> tmp;
      const RealT p1 = number / nDen;
      const RealT p2 = p1 / nDen;
      cov *= p1;
      cov -= (FVectorC<N>(point - Mean()).OuterProduct(tmp) *= p2);

      // Update the mean.
      m -= point;
    }
    return *this;
  }

  template<unsigned int N>
  const FMeanCovarianceC<N> & FMeanCovarianceC<N>::operator+=(const FMeanC<N> & mean) {
    // Update the covariance matrix.
    const RealT number1 = (RealT) Number();
    const RealT number2 = (RealT) mean.Number();
    const RealT nDen    = number1 + number2;
    if (nDen <= 0) { // Both set were obviously empty.
      SetZero();
    } else {
      FMatrixC<N,N> tmp;
      const RealT p1 = number1 / nDen;
      const RealT p2 = number2 / nDen;
      cov *= p1;
      cov += (FVectorC<N>(mean.Mean() - Mean()).OuterProduct(tmp) *= p1*p2);

      // Update the mean.
      m += mean;
    }
    return *this;
  }

  template<unsigned int N>
  const FMeanCovarianceC<N> & FMeanCovarianceC<N>::operator-=(const FMeanC<N> & mean) {
    const RealT number1 = (RealT) Number();
    const RealT number2 = (RealT) mean.Number();
    const RealT nDen    = number1 - number2;
    if (nDen <= 0) {
      SetZero();
    } else  {
      // Update the covariance matrix.
      FMatrixC<N,N> tmp;
      const RealT p1 = number1 / nDen;
      const RealT p2 = number2 / nDen;
      cov *= p1;
      cov -= (FVectorC<N>(mean.Mean() - Mean()).OuterProduct(tmp) *= p1*p2);

      // Update the mean.
      m -= mean;
    }
    return *this;
  }

  template<unsigned int N>
  const FMeanCovarianceC<N> & FMeanCovarianceC<N>::operator+=(const FMeanCovarianceC<N> & meanCov) {
    // Update the covariance matrix.
    const RealT number1 = (RealT) Number();
    const RealT number2 = (RealT) meanCov.Number();
    const RealT nDen    = number1 + number2;
    if (nDen <= 0) { // Both sets were obviously empty.
      SetZero();
    } else {
      FMatrixC<N,N> tmp;
      const RealT p1 = number1 / nDen;
      const RealT p2 = number2 / nDen;
      if (&cov != &(meanCov.Covariance())) {
        cov *= p1;
        cov += meanCov.Covariance() * p2;
      }
      cov += (FVectorC<N>(meanCov.Mean() - Mean()).OuterProduct(tmp) *= p1*p2);

      // Update the mean.
      m += meanCov.m;
    }
    return *this;
  }

  template<unsigned int N>
  const FMeanCovarianceC<N> & FMeanCovarianceC<N>::operator-=(const FMeanCovarianceC<N> & meanCov) {
    const RealT number1 = (RealT) Number();
    const RealT number2 = (RealT) meanCov.Number();
    const RealT nDen    = number1 - number2;
    if (nDen <= 0) {
      SetZero();
    } else {
      // Update the covariance matrix.
      FMatrixC<N,N> tmp;
      const RealT p1 = number1 / nDen;
      const RealT p2 = number2 / nDen;
      if (&cov != &(meanCov.Covariance())) {
        cov *= p1;
        cov -= meanCov.Covariance() * p2;
      }
      cov -= (FVectorC<N>(meanCov.Mean() - Mean()).OuterProduct(tmp) *= p1*p2);

      // Update the mean.
      m -= meanCov.m;
    }
    return *this;
  }

  template<unsigned int N>
  const FMeanCovarianceC<N> & FMeanCovarianceC<N>::Add(const FVectorC<N> & point, const FVectorC<N> & var) {
    // Update the covariance matrix.
    const RealT number = (RealT) Number();
    const RealT p1 = number / (number+1.0);
    const RealT p2 = 1.0 / (number+1.0);
    FMatrixC<N,N> tmp;
    cov *= p1;
    cov.AddDiagonal(var * p2);
    cov += (FVectorC<N>(point - Mean()).OuterProduct(tmp) *= p1 * p2);

    // Update the mean.
    m += point;

    return *this;
  }

  template<unsigned int N>
  const FMeanCovarianceC<N> & FMeanCovarianceC<N>::Remove(const FVectorC<N> & point, const FVectorC<N> & var) {
    const RealT number = (RealT) Number();
    const RealT nDen   = number-1.0;
    if (nDen <= 0) {
      SetZero();
    } else {
      // Update the covariance matrix.
      FMatrixC<N,N> tmp;
      const RealT p1 = number / nDen;
      const RealT p2 = 1.0 / nDen;
      cov *= p1;
      cov.AddDiagonal(var * (-p2));
      cov -= (FVectorC<N>(point - Mean()).OuterProduct(tmp) *= p1*p2);

      // Update the mean.
      m -= point;
    }
    return *this;
  }

  template<unsigned int N>
  const FMeanCovarianceC<N> &
  FMeanCovarianceC<N>::SetSum(const FMeanCovarianceC<N> & meanCov1,
                              const FMeanCovarianceC<N> & meanCov2) {
    *this = meanCov1;
    *this += meanCov2;
    return *this;
  }

  template<unsigned int N>
  FMeanCovarianceC<N> FMeanCovarianceC<N>::operator*(const FMeanCovarianceC<N> &oth) const {
    FMatrixC<N,N> sumCov(Covariance());
    sumCov += oth.Covariance();
    sumCov.InverseIP();
    FVectorC<N> mean = oth.Covariance() * sumCov * Mean();
    FMatrixC<N,N> cs = Covariance() * sumCov;
    mean += cs * oth.Mean();
    return FMeanCovarianceC<N>(Number() + oth.Number(),
             									 mean,
                               cs * oth.Covariance());
  }

  template<unsigned int N>
  RealT FMeanCovarianceC<N>::Gauss(const FVectorC<N> & point) const {
    FMatrixC<N,N> invCov = Covariance();
    invCov.InverseIP();
    FVectorC<N> diff = point - m;
    RealT e =  diff.Dot(invCov * diff) / -2;
    RealT a = Pow(2 * RavlConstN::pi,(RealT) N / 2) * Sqrt(Determinant(cov)) ;
    return Exp(e)/a;
  }

  template<unsigned int N>
  bool FMeanCovarianceC<N>::Save(BinOStreamC &outS) const {
    outS << m << cov;
    return true;
  }

}
#endif

