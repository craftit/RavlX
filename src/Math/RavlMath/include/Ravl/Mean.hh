// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_MEAN_HEADER
#define RAVL_MEAN_HEADER 1
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! userlevel=Normal
//! docentry="Ravl.API.Math.Statistics"
//! lib=RavlMath
//! file="Ravl/Math/Statistics/MeanCovariance/Mean.hh"

#include "Ravl/Types.hh"
#include "Ravl/SArray1d.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: Mean a single variable.
  
  class MeanC {
  public:
    explicit MeanC(const SArray1dC<RealT> &data);
    //: Calculate the mean and variance from an array of numbers.
    
    MeanC()
      : n(0),
	mean(0)
    {}
    //: Default constructor.
    
    MeanC(SizeT nn,RealT nmean)
      : n(nn),
	mean(nmean)
    {}
    //: Constructor.
    
    SizeT Number() const
    { return n; }
    //: Access the number of samples.
    
    RealT Mean() const
    { return mean; }
    //: Access the mean.
    
    MeanC &operator+=(const MeanC &mv);
    //: Add another Mean to this one.
    
    MeanC &operator-=(const MeanC &mv);
    //: Remove another Mean from this one.
    
    MeanC &operator+=(const RealT &mv);
    //: Add a sample to this one.
    
    MeanC &operator-=(const RealT &mv);
    //: Remove a sample from this one.
    
    void SetZero()
    { n = 0; mean = 0; }
    //: Reset mean and number to zero.
    
    UIntT Hash() const
    { return StdHash(n) + StdHash(mean); }
    //: Provided for compatibility with templates.

  protected:
    SizeT n;
    RealT mean;
  };


  inline
  ostream& operator<<(ostream &s,const MeanC &mv) {
    s << mv.Number() << ' ' << mv.Mean() ;
    return s;
  }
  //: Write to output stream.

  inline
  istream& operator>>(istream &s, MeanC &mv) {
    SizeT n;
    RealT v1;
    s >> n >> v1;
    mv = MeanC(n,v1);
    return s;
  }
  //: Read from input stream.
  
  BinOStreamC& operator<<(BinOStreamC &s,const MeanC &mv);
  //: Write to binary output stream.
  
  BinIStreamC& operator>>(BinIStreamC &s, MeanC &mv);
  //: Read from binary input stream.

}


#endif
