// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_REALRANGE1D_HEADER
#define RAVL_REALRANGE1D_HEADER 1
//////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Indexing"
//! userlevel=Normal
//! lib=RavlCore
//! file="Ravl/Core/Base/RealRange1d.hh"

#include "Ravl/Types.hh"
#include "Ravl/IndexRange1d.hh"
#include "Ravl/Math.hh"
#include "Ravl/StdHash.hh"

namespace RavlN {
#if RAVL_VISUALCPP_NAMESPACE_BUG && RAVL_HAVE_STDNAMESPACE
  using namespace std;
#endif
  
  class BinIStreamC;
  class BinOStreamC;
  class RealRangeC;
  
  //! userlevel=Normal
  //: 1D Range of real values.
  
  class RealRangeC {
  public:
    //:----------------------------------------------
    // Constructors, copy, assigment, and destructor.
    
    inline RealRangeC(RealT size = 0)
      : minV(0),
	maxV(size)
    {}
    //: Creates the index range <0, dim-1>.
    
    explicit inline RealRangeC(const IndexRangeC &rng)
      : minV(rng.Min()),
	maxV(rng.Max()+1)
    {}
    //: Create real range from an IndexRangeC.
    // Note that the upper limit of the RealRangeC object is incremented by 1
    // to make the range consistent.
    
    inline RealRangeC(RealT minReal, RealT maxReal)
      : minV(minReal),
	maxV(maxReal)
    {}
    //: Creates the index range <minReal, maxReal>.
    
    inline RealRangeC(std::istream & s);
    //: Creates the index range from the input stream.
    
    //:---------------------------------
    //: Access to the object information.
    
    inline RealT Size() const
    { return (maxV-minV); }
    //: Returns the number of elements in the range.
    
    inline const RealRangeC & Range() const
    { return *this; }
    //: Returns this object.
    
    inline const RealT & Min()  const
    { return minV; }
    //: Returns the minimum index of the range.
    
    inline const RealT & Max()  const
    { return maxV; }
    //: Returns the maximum index of the range.
    
    inline RealT & Min()
    { return minV; }
    //: Returns the minimum index of the range.
    
    inline RealT & Max()
    { return maxV; }
    //: Returns the maximum index of the range.
    
    inline RealT Center() const
    { return (Min() + Max())/2; }
    //: Returns the index in the middle of the range, eg. (Max()+Min())/2.
    
    inline RealT CenterD() const
    { return (Min() + Max())/2; }
    //: Returns the index previous the middle of the range, eg. (Max()+Min())/2.

    inline RealT Percentage(RealT p) const
    { return (Max() - Min()) * p/100.0 + Min(); }
    //: Returns the index which is in the 'p' % of the whole range.
    
    //:-------------------
    //: Logical operations.
    
    inline bool IsEmpty() const
    { return Min() > Max(); }
    //: Returns true if the minimum limit is bigger than the maximum limit. 
    
    inline bool IsValid() const
    { return Min() <= Max(); }
    //: Returns true if the minimum limit is smaller than or equal to the maximum value
    
    inline bool Contains(RealT i) const
    { return (Min() <= i) && (i <= Max()); }
    //: Returns true if this range contains the index 'i'.
    
    inline bool Contains(const RealRangeC & range) const
    { return Contains(range.Min()) && Contains(range.Max()); }
    //: Returns true if this range contains the subrange 'range'.
    
    inline bool operator==(const RealRangeC & range) const
    { return (Min() == range.Min()) && (Max() == range.Max()); }
    //: Returns true if both index ranges have the same limits.
    
    inline bool operator!=(const RealRangeC & range) const
    { return (Min() != range.Min()) || (Max() != range.Max()); }
    //: Returns true if both the ranges have different limits.
    
    bool In(const RealRangeC & range) const;
    //: Returns true if this range is inside of the 'range'.
    
    inline bool IsOverlapping(const RealRangeC & r) const;
    //: Returns true if this range contains at least one common index with 
    //: the range 'r'.
    
    //:-------------------
    //: Special operations.

    inline const RealRangeC &SetOrigin(RealT position) { 
      Max() = position + Max() - Min(); 
      Min() = position;
      return *this;
    }
    //: Set the origin of the range to 'position'.
    // Returns a reference to this range.
    
    inline RealRangeC &operator++()
    { Min()++; Max()++; return *this; }
    //: Move both the max and min of the range along 1.
    // Returns a reference to this range.
    
    inline RealRangeC &operator--()
    { Min()--; Max()--; return *this; }
    //: Move both the max and min of the range back 1.
    // Returns a reference to this range.
    
    inline const RealRangeC & operator+=(RealT i);
    //: Both minimum and maximum limits are shifted by adding the offset 'i'.
    // Returns a reference to this range.
    
    inline const RealRangeC & operator-=(RealT i);
    //: Both minimum and maximum limits are shifted by subtracting the offset 'i'.
    // Returns a reference to this range.
    
    inline RealRangeC operator+(RealT i) const
    { return RealRangeC(Min() + i,Max() + i); }
    //: Create a new RealRangeC with minimum and maximum limits shifted by adding the offset 'i'.
    
    inline RealRangeC operator-(RealT i) const
    { return RealRangeC(Min() - i,Max() - i); }
    //: Create a new RealRangeC with minimum and maximum limits shifted by subtracting the offset 'i'.
    
    inline RealRangeC & ClipBy(const RealRangeC & r);
    //: This index range is clipped to contain at most the index range 'r'.

    inline RealT Clip(const RealT & r) const {
      RealT lower = Min() > r? Min(): r;
      return lower < Max()? lower: Max();
    }
    //: The value 'r' is clipped to be within this range.
    
    inline RealRangeC FirstHalf() const
    { return RealRangeC(Min(),Center()); }
    //: Returns the index range < Min(), (Max()+Min())/2 >.
    
    inline RealRangeC FirstHalfD() const
    { return RealRangeC(Min(),CenterD()); }
    //: Returns the index range < Min(), (Max()+Min())/2 >.
    
    inline RealRangeC Enlarge(RealT f) const
    { return RealRangeC(Min(), Min() + Size()*f - 1); }
    //: Returns the index range whose number of elements is enlarged by
    //: the factor 'f'. The upper limits is changed.
    
    inline RealRangeC Expand(RealT n) const
    { return RealRangeC(minV - n,maxV + n); }
    //: Returns the range extended by adding 'n' items on both limits of
    //: this range. 
    
    inline RealRangeC Shrink(RealT n) const
    { return RealRangeC(minV + n,maxV - n); }
    //: Returns the range extended by adding 'n' items on both limits of
    //: this range. 
    
    inline RealRangeC & ShrinkHigh(RealT n)
    { Max() -= n; return *this; }
    //: Returns the range shrinked by removing of the 
    //: last 'n' items on both limits of this range. 
    
    inline RealRangeC & Swap(RealRangeC & r);
    //: Exchanges the contents of this range and range 'r'. The function
    //: returns this range. 

    RealRangeC operator*(RealT scale) const
    { return RealRangeC(Min()*scale,Max()*scale); }
    //: Scale range

    const RealRangeC &Involve(RealT i) { 
      if(minV > i) minV = i;
      if(maxV < i) maxV = i;
      return *this;
    }
    //: Modify this range to ensure index i is contained within it.

    const RealRangeC &Involve(const RealRangeC &subRange) { 
      Involve(subRange.Min());
      Involve(subRange.Max());
      return *this;
    }
    //: Modify this range to ensure subRange is contained within it.
    
    IndexRangeC IndexRange() const
    { return IndexRangeC(Floor(minV),Ceil(maxV));  }
    //: Get the smallest integer range containing the real range.
    
    UIntT Hash() const {
      UIntT ret = StdHash(minV);
      ret += ret << 11;
      ret += StdHash(maxV);
      return ret;
    }
    //: Generate a hash value for the range.
    //: For template compatibility.

  private:
    RealT minV; // Minimum index.
    RealT maxV; // Maximum index.
    
    //friend std::istream & operator>>(std::istream & s, RealRangeC & range);
  };
  
  inline bool IsInside(RealT i, const RealRangeC & range)
  { return (range.Min() <= i) && (i <= range.Max()); }
  //: Returns true if the index 'i' is inside the index range 'r'.
  
  IndexRangeC operator*(const RealRangeC &realRange,const IndexRangeC &indexRange);
  //: Multiply an index range by a real range.
  // Multiplying by a real range of 0-1 is a unit transform.
  
  std::istream & operator>>(std::istream & s, RealRangeC & r);
  //: Read range from input stream.
  // Read information from the intput stream 's' and sets the real range
  // according obtained data.
  
  std::ostream & operator<<(std::ostream & s, const RealRangeC & r);
  //: Saves the index range 'r' into the output stream 's'.
  
  BinOStreamC &operator<<(BinOStreamC &s,const RealRangeC &ir);  
  //: Read range from binary stream.
  
  BinIStreamC &operator>>(BinIStreamC &s,RealRangeC &ir);
  //: Write range to binary stream.
  
  inline bool RealRangeC::IsOverlapping(const RealRangeC & r) const {
    return (!IsEmpty() && !r.IsEmpty()) 
      && (((Min() <= r.Max()) && (r.Min() <= Max()))
	  || ((r.Min() <= Max()) && (Min() <= r.Max())));
  }
  
  inline const RealRangeC & RealRangeC::operator+=(RealT i) {
    Min() += i;
    Max() += i;
    return *this;
  }

  inline const RealRangeC &RealRangeC::operator-=(RealT i) {
    Min() -= i;
    Max() -= i;
    return *this;
  }
  
  inline RealRangeC &RealRangeC::ClipBy(const RealRangeC & r) {
    if (Min() < r.Min()) {
      Min() = r.Min();
      if(Max() < r.Min()) // Make sure there is some overlap.
	Max() = r.Min(); // Make range zero size.
    }
    if (Max() > r.Max()) {
      Max() = r.Max();
      if(Min() > r.Max()) // Make sure there is some overlap.
	Min() = r.Max(); // To make range zero size.
    }
    return *this;
  }

  inline RealRangeC &RealRangeC::Swap(RealRangeC & r) {
    const RealRangeC tmp = *this;
    *this = r;
    r = tmp;
    return *this;
  }
}

#endif
