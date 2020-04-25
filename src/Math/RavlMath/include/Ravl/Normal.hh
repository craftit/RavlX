// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_NORMAL_HEADER
#define RAVL_NORMAL_HEADER 1
////////////////////////////////////////////////////////////////
//! docentry="Ravl.API.Math.Statistics"
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Statistics/MeanCovariance/Normal.hh"

#include "Ravl/Matrix.hh"
#include "Ravl/MeanCovariance.hh"

namespace RavlN {
  
  class NormalC;
  
  ostream &operator<<(ostream &s,const NormalC &norm);
  istream &operator>>(istream &s,NormalC &norm);
  
  BinOStreamC &operator<<(BinOStreamC &s,const NormalC &norm);
  BinIStreamC &operator>>(BinIStreamC &s,NormalC &norm);
  
  //! userlevel=Normal
  //: Model a normal (or gauss) multi-dimensional distribution.
  
  class NormalC {
  public:
    NormalC()
    {}
    //: Default constructor.
    // Creates a distribution with zero dimensions.
    
    NormalC(const MeanCovarianceC &meanCov,bool fixSingular = false)
    { Set(meanCov,fixSingular); }
    //: Construct from a covariance matrix.
    
    bool Set(const MeanCovarianceC &meanCov,bool fixSingular = false);
    //: Setup the distribution.
    // Use the mean and covariance to setup the distribution.
    // This may fail if the covariance is singular.
    
    RealT Evaluate(const VectorC &vec);
    //: Get value of normal distribution at 'vec'.
    
    UIntT Size() const
    { return mean.Size(); }
    //: Get the number of dimensions in the distribution.
    
  protected:
    RealT d;        // Denominator of distribution. 
    VectorC mean;   // Mean of distribution.
    MatrixC invCov; // Inverse covariance.
    
    friend ostream &operator<<(ostream &s,const NormalC &norm);
    friend istream &operator>>(istream &s,NormalC &norm);
    
    friend BinOStreamC &operator<<(BinOStreamC &s,const NormalC &norm);
    friend BinIStreamC &operator>>(BinIStreamC &s,NormalC &norm);
  };
  
}



#endif
