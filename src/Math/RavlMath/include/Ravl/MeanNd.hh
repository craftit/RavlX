// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_MATHMEANND_HEADER
#define RAVL_MATHMEANND_HEADER 1
/////////////////////////////////////////////
//! userlevel=Normal
//! author="Radek Marik"
//! date="26/05/1994"
//! docentry="Ravl.API.Math.Statistics"
//! lib=RavlMath
//! file="Ravl/Math/Statistics/MeanCovariance/MeanNd.hh"

#include "Ravl/Vector.hh"

namespace RavlN {
  template<class DataT> class DListC;
  class MeanCovarianceC;
  
  //: Mean in N-D space
  // The class MeanNdC serves for computation of the mean
  // of a set of N-dimensional vectors.  
  
  class MeanNdC
    : public VectorC
  {
  public:
    MeanNdC()
      : number(0)
    {}
    //: Default constructor.
    
    MeanNdC(const SArray1dC<VectorC> &vecs);
    //: Caluclate the mean of an array of vectors.
    // All the vectors must have the same size, if
    // the array is empty a zero mean is generated.
    
    MeanNdC(const DListC<VectorC> &vecs);
    //: Caluclate the mean of a list of vectors.
    // All the vectors must have the same size, if
    // the list is empty a zero mean is generated.
    
    MeanNdC(const UIntT dim)
      : VectorC(dim),
	number(0)
    { VectorC::Fill(0); }
    //: Creat an N-dimentional mean.
    // Creates zero mean representing the 'n'-dimensional set 
    // containing no data points.
    
    MeanNdC(const VectorC & mean) 
      : VectorC(mean),
	number(1)
    {}
    //: Create a mean with a single point.
    // Creates the mean vector representing
    // the data set containing just one data point.
    // This object is attached to the memory of 'point'.
    
    MeanNdC(RealT n, const VectorC & mean)
      : VectorC(mean),
	number(n)
    {}
    //: Create with a mean an a number of points.
    // Creates the mean vector representing
    // the data set containing 'n' points and represented by the 'mean'.
    // This object is attached to the memory of 'mean'.
    
    MeanNdC Copy() const;
    //: Copy this mean
    // Returns a new physical copy of this object.
    
    //:---------------------------
    // Information about an object

    inline RealT Number() const
    { return number; }
    //: Returns number of points which were added into the object.
    
    inline const VectorC & Mean() const
    { return *this; }
    //: Returns the current value of the mean.
    
    //:-------------------      
    // Object modification
    
    inline void SetZero() 
    { VectorC::Fill(0); number = 0; }
    //: Reset all the counters to zero.
    
    inline void Set(RealT n, const VectorC & m)
    { VectorC::operator=(m); number = n; }
    //: Sets the new content of mean object. 
    // It will represent 'n' points with the mean location 'm'.
    
    inline MeanNdC & operator+=(const VectorC & point);
    //: Adds a point to the set.

    inline MeanNdC & operator+=(const TVectorC<float> & point);
    //: Adds a point to the set.

    inline MeanNdC & operator-=(const VectorC & point);
    //: Removes a point from the set. 
    // Be carefull to remove a point which was already added to 
    // the set, otherwise the representation will not describe a real set.
    
    inline MeanNdC & operator+=(const MeanNdC & mean);
    //: Adds the whole set of points represented by the mean object 'm'.

    inline MeanNdC & operator-=(const MeanNdC & mean);
    //: Removes the whole set of points represented by the mean object 'm'.
    // Be carefull to remove points which was already added to the set,
    // otherwise the representation will not describe a real set.
    
    VectorC &Mean()
    { return (VectorC &) *this; }
    //: Access the mean vector.
    
    UIntT Hash() const
    { return StdHash(number) + StdHash(static_cast<const VectorC &>(*this)); }
    //: Provided for compatibility with templates.

  protected:
    
    // Object representation
    // ---------------------
    
    RealT number;  // The number of points represented by mean object.

    friend class MeanCovarianceC;
    friend istream & operator>>(istream & s, MeanNdC & mean);
    friend BinIStreamC & operator>>(BinIStreamC & s, MeanNdC & mean);
  };
  
  ostream & operator<<(ostream & s, const MeanNdC & mean);
  // Saves the statistical description of the set 'mean' into the output
  // stream 'outS'.
  
  istream & operator>>(istream & s, MeanNdC & mean);
  // Reads and sets the statistical description of the set 'mean'
  // according to the information in the input stream 'inS'.

  BinOStreamC & operator<<(BinOStreamC & s, const MeanNdC & mean);
  // Saves the statistical description of the set 'mean' into the output
  // stream 'outS'.
  
  BinIStreamC & operator>>(BinIStreamC & s, MeanNdC & mean);
  // Reads and sets the statistical description of the set 'mean'
  // according to the information in the input stream 'inS'.
  
  inline MeanNdC &MeanNdC::operator+=(const VectorC & point) {
    if (&Mean() == &point)
      number++;
    else {
      Mean() *= (RealT) number++;
      Mean() += point;
      Mean() /= number;
    }
    return *this;
  }

  inline MeanNdC &MeanNdC::operator+=(const TVectorC<float> & point) {
    Mean() *= (RealT) number++;
    Mean() += point;
    Mean() /= number;
    return *this;
  }

  
  inline MeanNdC &  MeanNdC::operator-=(const VectorC & point) {
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
  
  inline MeanNdC &MeanNdC::operator+=(const MeanNdC & point) {
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
  
  inline MeanNdC &MeanNdC::operator-=(const MeanNdC & point) {
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
