// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_MATHFMEAN_HEADER
#define RAVL_MATHFMEAN_HEADER 1
/////////////////////////////////////////////
//! rcsid="$Id$"
//! userlevel=Normal
//! author="Radek Marik"
//! date="26/05/1994"
//! docentry="Ravl.API.Math.Statistics"
//! lib=RavlMath
//! file="Ravl/Math/Statistics/MeanCovariance/FMean.hh"

#include "Ravl/FVector.hh"
#include "Ravl/Vector2d.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/Stream.hh"

namespace RavlN {

  template<unsigned int N> class FMeanC;
  template<unsigned int N> istream & operator>>(istream & s, FMeanC<N> & mean);
  template<unsigned int N> BinIStreamC & operator>>(BinIStreamC & s, FMeanC<N> & mean);
  
  //: Mean in N-D space
  // The class MeanNdC serves for computation of the mean
  // of a set of N-dimensional vectors.  
  
  template<unsigned int N>
  class FMeanC
    : public FVectorC<N>
  {
  public:
    FMeanC()
    { SetZero(); }
    //: Default constructor.
    
    FMeanC(const SArray1dC<FVectorC<N> > &vecs);
    //: Caluclate the mean of an array of vectors.
    // All the vectors must have the same size, if
    // the array is empty a zero mean is generated.
    
    FMeanC(const FVectorC<N> & mean) 
      : FVectorC<N>(mean),
	number(1)
    {}
    //: Create a mean with a single point.
    // Creates the mean vector representing
    // the data set containing just one data point.
    // This object is attached to the memory of 'point'.
    
    FMeanC(SizeT n, const FVectorC<N> & mean)
      : FVectorC<N>(mean),
	number(n)
    {}
    //: Create with a mean an a number of points.
    // Creates the mean vector representing
    // the data set containing 'n' points and represented by the 'mean'.
    // This object is attached to the memory of 'mean'.
    
    FMeanC<N> Copy() const;
    //: Copy this mean
    // Returns a new physical copy of this object.
    
    // Information about an object
    // ---------------------------

    inline SizeT Number() const
    { return number; }
    //: Returns number of points which were added into the object.
    
    inline const FVectorC<N> & Mean() const
    { return *this; }
    //: Returns the current value of the mean.

    FVectorC<N> &Mean()
    { return (FVectorC<N> &) *this; }
    //: Access the mean vector.
    
    //:-------------------      
    // Object modification
    
    inline void SetZero() 
    { FVectorC<N>::Fill(0); number = 0; }
    //: Reset all the counters to zero.
    
    inline void Set(SizeT n, const FVectorC<N> & m)
    { FVectorC<N>::operator=(m); number = n; }
    //: Sets the new content of mean object. 
    // It will represent 'n' points with the mean location 'm'.
    
    inline FMeanC<N> & operator+=(const FVectorC<N> & point);
    //: Adds a point to the set.

    inline FMeanC<N> & operator-=(const FVectorC<N> & point);
    //: Removes a point from the set. 
    // Be carefull to remove a point which was already added to 
    // the set, otherwise the representation will not describe a real set.
    
    inline FMeanC<N> & operator+=(const FMeanC<N> & mean);
    //: Adds the whole set of points represented by the mean object 'm'.

    inline FMeanC<N> & operator-=(const FMeanC<N> & mean);
    //: Removes the whole set of points represented by the mean object 'm'.
    // Be carefull to remove points which was already added to the set,
    // otherwise the representation will not describe a real set.
    
    UIntT Hash() const
    { return StdHash(number) + StdHash(static_cast<const FVectorC<N> &>(*this)); }
    //: Provided for compatibility with templates.

  protected:
    
    // Object representation
    // ---------------------
    
    SizeT number;  // The number of points represented by mean object.
    
    friend istream & operator>> <>(istream & s, FMeanC<N> & mean);
    friend BinIStreamC & operator>> <>(BinIStreamC & s, FMeanC<N> & mean);
  };
  
  template<unsigned int N>
  ostream & operator<<(ostream & s, const FMeanC<N> & mean) {
    s << mean.Number() << ' ' << mean.Mean();
    return s;
  }
  // Saves the statistical description of the set 'mean' into the output
  // stream 'outS'.
  
  template<unsigned int N>
  istream & operator>>(istream & s, FMeanC<N> & mean) {
    s >>  mean.number >>  mean.Mean();
    return s;
  }
  // Reads and sets the statistical description of the set 'mean'
  // according to the information in the input stream 'inS'.

  template<unsigned int N>
  BinOStreamC & operator<<(BinOStreamC & s, const FMeanC<N> & mean) {
    s << mean.Number() << mean.Mean();
    return s;
  }
  // Saves the statistical description of the set 'mean' into the binary output
  // stream 'outS'.
  
  template<unsigned int N>
  BinIStreamC & operator>>(BinIStreamC & s, FMeanC<N> & mean) {
    s >>  mean.number >>  mean.Mean();
    return s;
  }
  // Reads and sets the statistical description of the set 'mean'
  // according to the information in the binary input stream 'inS'.

  
  template<unsigned int N>
  FMeanC<N>::FMeanC(const SArray1dC<FVectorC<N> > &vecs) {
    number = vecs.Size();
    if(number == 0)
      return ;
    SArray1dIterC<FVectorC<N> > it(vecs);
    Mean() = *it;
    it++;
    for(;it;it++)
      Mean() += *it;
    Mean() /= ((RealT) number);
  }
  
  template<unsigned int N>
  FMeanC<N> FMeanC<N>::Copy() const 
  { return *this; }
  
  template<unsigned int N>
  inline FMeanC<N> &FMeanC<N>::operator+=(const FVectorC<N> & point) {
    if (&Mean() == &point)
      number++;
    else {
      Mean() *= (RealT) number++;
      Mean() += point;
      Mean() /= number;
    }
    return *this;
  }
  
  template<unsigned int N>
  inline FMeanC<N> &  FMeanC<N>::operator-=(const FVectorC<N> & point) {
    if (&Mean() == &point) {
      number--;
      if (number == 0)
	SetZero();
    } else {
      Mean() *= (RealT)number--;
      Mean() -= point;
      if (number == 0)
	SetZero();
      else
	Mean() /= (RealT) number;
    }
    return *this;
  }
  
  template<unsigned int N>
  inline FMeanC<N> &FMeanC<N>::operator+=(const FMeanC<N> & point) {
    if (this == &point)
      number += number;
    else {
      Mean() *= (RealT)number;
      number  += point.number;
      if (number == 0)
	SetZero();
      else {
	Mean() += point * (RealT)point.number;
	Mean() /= (RealT)number;
      }
    }
    return *this;
  }
  
  template<unsigned int N>
  inline FMeanC<N> &FMeanC<N>::operator-=(const FMeanC<N> & point) {
    if (this == &point)
      SetZero();
    else {
      Mean() *= (RealT)number;
      number  -= point.number;
      Mean() -= point * (RealT)point.number;
      if (number == 0)
	SetZero();
      else
	Mean() /= (RealT)number;
    }
    return *this;
  }
  
}

#endif
