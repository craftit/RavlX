// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_MEANCOVARIANCEND_HEADER
#define RAVL_MEANCOVARIANCEND_HEADER
//////////////////////////////////////////////////////////////////////
//! userlevel=Normal
//! author="Radek Marik"
//! date="01/01/1994"
//! docentry="Ravl.API.Math.Statistics"
//! lib=RavlMath
//! file="Ravl/Math/Statistics/MeanCovariance/MeanCovariance.hh"

#include "Ravl/Types.hh"
#include "Ravl/MeanNd.hh"
#include "Ravl/MatrixRS.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: Mean and covariance together in N-D space 
  // The class MeanCovarianceNdC serves for computation of the mean
  // and the covariance matrix of a set of N-dimensional data points.
  // The object are able to share a memory with other objects. <br>
  // Note it is more efficent to compute the mean and covariance of a
  // data set using the SumsNd2C class than to accumlate statistics with 
  // this class.
  
  class MeanCovarianceC {
  public:
    MeanCovarianceC()
      : m_haveInvCov(false),
        m_cacheDet(RavlConstN::nanReal)
    {}
    // Empty constructor, creates invalid object

    MeanCovarianceC(const MeanCovarianceC & meanCov)
      : m(meanCov.m), 
	cov(meanCov.cov),
        m_haveInvCov(false),
        m_cacheDet(RavlConstN::nanReal)
    {}
    // The class MeanCovarianceC is implemented as a big object using
    // a reference counter.
    
    explicit MeanCovarianceC(const SizeT n)
      : m(n), 
	cov(n),
        m_haveInvCov(false),
        m_cacheDet(RavlConstN::nanReal)
    { cov.Fill(0); }
    // Creates zero mean and zero covariance matrix representing
    // the 'n'-dimensional set containing no data points.

    explicit MeanCovarianceC(const unsigned n)
      : m(n),
        cov(n),
        m_haveInvCov(false),
        m_cacheDet(RavlConstN::nanReal)
    { cov.Fill(0); }
    // Creates zero mean and zero covariance matrix representing
    // the 'n'-dimensional set containing no data points.

    explicit MeanCovarianceC(const VectorC & point)
      : m(point), 
	cov(point.Size()),
        m_haveInvCov(false),
        m_cacheDet(RavlConstN::nanReal)
    { cov.Fill(0); }
    // Creates the mean vector and zero covariance matrix representing
    // the data set containing just one data point. The vector 'point'
    // is shared.
    
    explicit MeanCovarianceC(const MeanNdC & mean)
      : m(mean), 
	cov(mean.Mean().Size()),
        m_haveInvCov(false),
        m_cacheDet(RavlConstN::nanReal)
    { cov.Fill(0); }
    // Creates the mean vector and zero covariance matrix representing
    // the data set represented by the 'mean'. The structure 'mean'
    // is shared.
    
    MeanCovarianceC(RealT n, 
		    const VectorC & mean, 
		    const MatrixRSC & ncov)
      : m(n,mean), 
	cov(ncov),
        m_haveInvCov(false),
        m_cacheDet(RavlConstN::nanReal)
    {}
    // Creates the mean vector and zero covariance matrix representing
    // the data set containing 'n' points and represented by the 'mean'
    // and the covariance matrix 'cov'. Both 'mean' and 'cov' are
    // shared.
    
    MeanCovarianceC Copy() const;
    // Returns a new physical copy of this object.
    
    MeanCovarianceC(const SArray1dC<VectorC> & data,bool sampleStatistics = true);
    //: Compute the mean and covariance of an array of vectors.
    //!param: data - Array containing data to compute statistics on
    //!param: sampleStatistics - When true compute statistics as a sample of a random variable. (Normalise covariance by n-1 )
    
    MeanCovarianceC(const DListC<VectorC> & data,bool sampleStatistics = true);
    //: Compute the mean and covariance of a list of vectors.
    //!param: data - List containing data to compute statistics on
    //!param: sampleStatistics - When true compute statistics as a sample of a random variable. (Normalise covariance by n-1 )
    
    // Information about an object
    // ---------------------------

    RealT Number() const
    { return m.Number(); }
    // Returns the number of data points which are represented by this object.
    
    const VectorC & Mean() const
    { return m.Mean(); }
    //: Access the mean.
    // Returns the mean vector of data points which are represented
    // by this object.

    VectorC & Mean()
    { return m.Mean(); }
    //: Access the mean.
    // Returns the mean vector of data points which are represented
    // by this object.
    
    const MeanNdC & MeanNd() const
    { return m; }
    //: Access the mean nd object.
    // Returns the mean vector of data points which are represented
    // by this object.
    
    const MatrixRSC & Covariance() const
    { return cov; }
    //: Access the covariance.
    // Returns the covariance matrix of data points which are represented
    // by this object.

    MatrixRSC & Covariance()
    { return cov; }
    //: Access the covariance.
    // Returns the covariance matrix of data points which are represented
    // by this object.

    // Object modification
    // -------------------      
    
    const MeanCovarianceC & SetZero();
    // Total initialization of this object resulting in the representation
    // the empty set of data points.

    const MeanCovarianceC & operator+=(const VectorC & point);
    //: Adds one point to the set of data points.
    // Note, this is NOT a good way to compute the mean and covariance 
    // of a large dataset. Use one of the constructors from a list
    // or array of vectors.
    
    const MeanCovarianceC & operator-=(const VectorC & point);
    // Removes one point from the set of data points. Be carefull to remove
    // a point which was already added to the set, otherwise the representation
    // will not describe a real set.

    const MeanCovarianceC & operator+=(const MeanNdC & mean);
    // Adds a number of data poits represented by the 'mean' and zero
    // covariance matrix to this set.

    const MeanCovarianceC & operator-=(const MeanNdC & mean);
    // Removes a number of data poits represented by the 'mean' and zero
    // covariance matrix from this set. Be carefull to remove
    // points which were already added to the set, otherwise the representation
    // will not describe a real set.

    const MeanCovarianceC & operator+=(const MeanCovarianceC & meanCov);
    // Adds a number of data points represented by the 'meanCov' structure
    // to this set.

    const MeanCovarianceC & operator-=(const MeanCovarianceC & meanCov);
    // Removes a number of data points represented by the 'meanCov' structure
    // from this set. Be carefull to remove
    // points which were already added to the set, otherwise the representation
    // will not describe a real set.

    const MeanCovarianceC & Add(const VectorC & point, const VectorC & var);
    // Updates the mean and the covariance matrix by adding one N-d point
    // whose coordinates are known with the error described by the diagonal
    // convariance matrix represented byt the vector 'var'.

    const MeanCovarianceC &Remove(const VectorC & point, const VectorC & var);
    // Updates the mean and the covariance matrix by removing one N-d point
    // whose coordinates are known with the error described by the diagonal
    // convariance matrix represented byt the vector 'var'.
    // Be carefull to remove the point which was already added
    // to the set, otherwise the representation
    // will not describe a real set.
    
    const MeanCovarianceC & SetSum(const MeanCovarianceC & meanCov1,
				   const MeanCovarianceC & meanCov2);
    //: This object is set to be the union of two set of data points 'meanCov1'
    //: and 'meanCov2'.
    
    MeanCovarianceC operator*(const MeanCovarianceC &oth) const;
    //: Calculate the product of the two probability density functions.
    // This assumes the estimates of the distributions are accurate. (The number
    // of samples is ignored) 
    
    RealT Gauss(const VectorC &vec) const;
    //: Evaluate the value of guassian distribution at 'vec'.
    
    RealT MahalanobisDistance(const VectorC &vec) const;
    //: Compute the Mahalanobis to the point.
    
    void ClearCache()
    { m_haveInvCov = false; }
    //: Clear inverse cache.
    // This must be used if you modify the mean or covariance directly.

    UIntT Hash() const
    { return m.Hash() + cov.Hash(); }
    //: Provided for compatibility with templates.

  protected:
    void CacheInverse() const;
    
    MeanNdC m;   // The mean vector of this data set.
    MatrixRSC cov; // the covariance matrix of this data set.
    
    mutable bool m_haveInvCov;
    mutable MatrixRSC m_cacheInvCov; 
    mutable RealT m_cacheDet;
    
    friend istream & operator>>(istream & inS, MeanCovarianceC & meanCov);
    friend BinIStreamC & operator>>(BinIStreamC & inS, MeanCovarianceC & meanCov);
  };
  
  ostream & operator<<(ostream & outS, const MeanCovarianceC & meanCov);
  // Saves the statistical description of the set 'meanCov' into the output
  // stream 'outS'.
  
  istream & operator>>(istream & inS, MeanCovarianceC & meanCov);
  // Reads and sets the statistical description of the set 'meanCov'
  // according to the information in the input stream 'inS'.
  
  BinOStreamC & operator<<(BinOStreamC & outS, const MeanCovarianceC & meanCov);
  // Saves the statistical description of the set 'meanCov' into the binary output
  // stream 'outS'.
  
  BinIStreamC & operator>>(BinIStreamC & inS, MeanCovarianceC & meanCov);
  // Reads and sets the statistical description of the set 'meanCov'
  // according to the information in the binary input stream 'inS'.
}
#endif

