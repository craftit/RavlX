// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_INDEXRANGE1D_HEADER
#define RAVL_INDEXRANGE1D_HEADER 1
//////////////////////////////////////////////////////////////////////////
//! file="Ravl/Core/Base/IndexRange1d.hh"
//! lib=RavlCore
//! userlevel=Normal
//! author="Radek Marik"
//! docentry="Ravl.API.Core.Indexing"
//! rcsid="$Id$"
//! date="2.1.1997"

#include "Ravl/Index.hh"

namespace RavlN {
  class BinIStreamC;
  class BinOStreamC;
  class IndexRangeC;
  
  inline bool IsInside(IndexC i, const IndexRangeC & range);
  
  ////////////////// IndexRangeC ////////////////////////////////////
  
  //: 1D index range of array
  // The class IndexRangeC is a representation of an interval of one
  // dimensional index.
  
  class IndexRangeC {
  public:
    //:----------------------------------------------
    // Constructors, copy, assignment, and destructor.
    
    inline explicit IndexRangeC(size_t dim = 0)
      : minI(0),
        maxI(IndexC(dim)-1)
    {}
    //: Creates the index range <0, dim-1>.
    
    inline explicit IndexRangeC(IndexC dim)
      : minI(0),
        maxI(dim-1)
    {}
    //: Creates the index range <0, dim-1>.

    inline explicit IndexRangeC(SizeC dim)
      : minI(0),
        maxI(IndexC(dim)-1)
    {}
    //: Creates the index range <0, dim-1>.

    inline IndexRangeC(IndexC minIndex, IndexC maxIndex)
      : minI(minIndex),
        maxI(maxIndex)
    {}
    //: Creates the index range <minIndex, maxIndex>.
    
    explicit IndexRangeC(std::istream & s);
    //: Creates the index range from the input stream.
    
    //:---------------------------------
    //: Access to the object information.
    
    inline IntT Size() const
    { return (maxI-minI+1).V(); }
    //: Returns the number of elements in the range.
    
    inline const IndexRangeC & Range() const
    { return *this; }
    //: Returns this object.
    
    inline const IndexC & Min()  const
    { return minI; }
    //: Returns the minimum index of the range.
    
    inline const IndexC & Max()  const
    { return maxI; }
    //: Returns the maximum index of the range.
    
    inline IndexC & Min()
    { return minI; }
    //: Returns the minimum index of the range.
    
    inline IndexC & Max()
    { return maxI; }
    //: Returns the maximum index of the range.
    
    inline IndexC Center() const
    { return (Min() + Max() + 1)/2; }
    //: Returns the index in the middle of the range, eg. (Max()+Min()+1)/2.
    
    inline IndexC CenterD() const
    { return (Min() + Max())/2; }
    //: Returns the index previous the middle of the range, eg. (Max()+Min())/2.
    
    inline IndexC Percentage(const RealT p) const
    { return IntT((Max() - Min()) * p/100.0 + Min()+0.5); }
    //: Returns the index which is in the 'p' % of the whole range.
    
    //:-------------------
    //: Logical operations.
    
    inline bool IsEmpty() const
    { return Min() > Max(); }
    //: Returns true if the minimum limit is bigger than the maximum limit. 
    // The range is empty if Min() > (Max()+1) since zero length ranges are supported
    // Note: if Min() = Max() the range has length 1

    inline bool IsValid() const
    { return Min() <= (Max()+1); }
    //: Returns true if the range is valid
    //: Empty ranges (ie Min() = Max()+1) are valid
    
    inline bool Contains(IndexC i) const
    { return (Min() <= i) && (i <= Max()); }
    //: Returns true if this range contains the index 'i'.
    
    inline bool Contains(RealT val) const
    { return (Floor(val) >= Min()) && (Ceil(val) <= Max()); }
    //: Is a real value inside the range ?
    
    inline bool Contains(const IndexRangeC & range) const
    { return Contains(range.Min()) && Contains(range.Max()); }
    //: Returns true if this range contains the subrange 'range'.
    
    inline bool operator==(const IndexRangeC & range) const
    { return (Min() == range.Min()) && (Max() == range.Max()); }
    //: Returns true if both index ranges have the same limits.
    
    inline bool operator!=(const IndexRangeC & range) const
    { return (Min() != range.Min()) || (Max() != range.Max()); }
    //: Returns true if both the ranges have different limits.
    
    bool In(const IndexRangeC & range) const
    { return IsInside(Min(),range) && IsInside(Max(),range); } 
    //: Returns true if this range is inside of the 'range'.
    
    inline bool IsOverlapping(const IndexRangeC & r) const;
    //: Returns true if this range contains at least one common index with 
    //: the range 'r'.
    
    //:-------------------
    //: Special operations.

    inline const IndexRangeC &SetOrigin(IndexC position) { 
      Max() = position + Max() - Min(); 
      Min() = position;
      return *this;
    }
    //: Set the origin of the range to 'position'.
    // Returns a reference to this range.
    
    inline IndexRangeC &operator++()
    { Min()++; Max()++; return *this; }
    //: Move both the max and min of the range along 1.
    // Returns a reference to this range.
    
    inline IndexRangeC &operator--()
    { Min()--; Max()--; return *this; }
    //: Move both the max and min of the range back 1.
    // Returns a reference to this range.
    
    inline const IndexRangeC & operator+=(IndexC i) {
      Min() += i;
      Max() += i;
      return *this;
    }
    //: Both minimum and maximum limits are shifted by adding the offset 'i'.
    // Returns a reference to this range.
    
    inline const IndexRangeC & operator-=(IndexC i) {
      Min() -= i;
      Max() -= i;
      return *this;
    }
    //: Both minimum and maximum limits are shifted by subtracting the offset 'i'.
    // Returns a reference to this range.

    inline const IndexRangeC & operator /= (IndexC i) {
      Min() /= i ;
      Max() = ((Max() + 1) / i) - 1;
      return *this ;
    }
    //: Both minimum and maximum limits are divided by i 
    // IndexC rounding rules apply 
    // Returns a reference to this range. 

    inline const IndexRangeC & operator *= (IndexC i) {
      Min() *= i ;
      Max() = ((Max() +1) * i) - 1;
      return *this ;
    }
    //: Both minimum and maximum limits are multiplied by i
    // returns a reference to this range 

    inline const IndexRangeC & operator+=(IntT i)
    { return (*this) += IndexC(i); }
    //: Both minimum and maximum limits are shifted by adding the offset 'i'.
    // Returns a reference to this range.
    
    inline const IndexRangeC & operator-=(IntT i)
    { return (*this) -= IndexC(i); }
    //: Both minimum and maximum limits are shifted by subtracting the offset 'i'.
    // Returns a reference to this range.

    inline const IndexRangeC & operator+=(UIntT i)
    { return (*this) += IndexC(i); }
    //: Both minimum and maximum limits are shifted by adding the offset 'i'.
    // Returns a reference to this range.
    
    inline const IndexRangeC & operator-=(UIntT i)
    { return (*this) -= IndexC(i); }
    //: Both minimum and maximum limits are shifted by subtracting the offset 'i'.
    // Returns a reference to this range.
    
    inline IndexRangeC operator+(IndexC i) const
    { return IndexRangeC(Min() + i,Max() + i); }
    //: Create a new IndexRangeC with minimum and maximum limits shifted by adding the offset 'i'.
    
    inline IndexRangeC operator-(IndexC i) const
    { return IndexRangeC(Min() - i,Max() - i); }
    //: Create a new IndexRangeC with minimum and maximum limits shifted by subtracting the offset 'i'.
    
    inline IndexRangeC operator/ (IndexC i) const 
    { return IndexRangeC(Min() / i, ((Max()+1) / i)-1) ; } 
    //: Create a new IndexRangeC with maximum and minimum limits divided by IndexC 
    // IndexC rounding rules apply. 

    inline IndexRangeC operator* (IndexC i) const 
    { return IndexRangeC (Min() * i, ((Max()+1) * i)-1) ; }
    //: Create a new IndexRangeC with maximum and minimum limits multiplied by IndexC 

    inline IndexRangeC operator+(IntT i) const
    { return IndexRangeC(Min() + i,Max() + i); }
    //: Create a new IndexRangeC with minimum and maximum limits shifted by adding the offset 'i'.
    
    inline IndexRangeC operator-(IntT i) const
    { return IndexRangeC(Min() - i,Max() - i); }
    //: Create a new IndexRangeC with minimum and maximum limits shifted by subtracting the offset 'i'. 

    inline IndexRangeC operator+(UIntT i) const
    { return IndexRangeC(Min() + i,Max() + i); }
    //: Create a new IndexRangeC with minimum and maximum limits shifted by adding the offset 'i'.
    
    inline IndexRangeC operator-(UIntT i) const
    { return IndexRangeC(Min() - i,Max() - i); }
    //: Create a new IndexRangeC with minimum and maximum limits shifted by subtracting the offset 'i'.
    
#if RAVL_CPUTYPE_64
    inline IndexRangeC  operator+(UInt64T i) const
     { return IndexRangeC(Min() + i,Max() + i); }
    //: Create a new IndexRangeC with minimum and maximum limits shifted by adding the offset 'i'.

    inline IndexRangeC  operator-(UInt64T i) const
     { return IndexRangeC(Min() - i,Max() - i); }
    //: Create a new IndexRangeC with minimum and maximum limits shifted by subtracting the offset 'i'.

    inline const IndexRangeC & operator-=(UInt64T i)
    { return (*this)-=IndexC(i)  ; }
    //: Both minimum and maximum limits are shifted by subtracting the offset 'i'.
    // Returns a reference to this range.

    inline const IndexRangeC & operator-=(Int64T i)
    { return (*this)-=IndexC(i)  ; }
    //: Both minimum and maximum limits are shifted by subtracting the offset 'i'.
    // Returns a reference to this range.

    inline const IndexRangeC & operator+=(UInt64T i)
    { return (*this) += IndexC(i); }
    //: Both minimum and maximum limits are shifted by adding the offset 'i'.
    // Returns a reference to this range.

    inline const IndexRangeC & operator+=(Int64T i)
    { return (*this) += IndexC(i); }
    //: Both minimum and maximum limits are shifted by adding the offset 'i'.
    // Returns a reference to this range.

 #endif

    inline IndexRangeC operator+(SizeC i) const
    { return IndexRangeC(Min() + i.V(),Max() + i.V()); }
    //: Create a new IndexRangeC with minimum and maximum limits shifted by adding the offset 'i'.

    inline IndexRangeC operator-(SizeC i) const
    { return IndexRangeC(Min() - i.V(),Max() - i.V()); }
    //: Create a new IndexRangeC with minimum and maximum limits shifted by subtracting the offset 'i'.

    inline const IndexRangeC & operator+=(SizeC i)
    { return (*this) += i.V(); }
    //: Both minimum and maximum limits are shifted by adding the offset 'i'.
    // Returns a reference to this range.

    inline const IndexRangeC & operator-=(SizeC i)
    { return (*this) -= i.V(); }
    //: Both minimum and maximum limits are shifted by adding the offset 'i'.
    // Returns a reference to this range.

    inline IndexRangeC & ClipBy(const IndexRangeC & r);
    //: This index range is clipped to contain at most the index range 'r'.

    inline IndexRangeC & Clip(const IndexRangeC & r) const {
      IndexRangeC ret = r;
      return ret.ClipBy(*this);
    }
    //: Clip 'r' by this index range and return it.
    
    inline IndexC Clip(const IndexC & i) const {
      IndexC lower = Min() > i? Min(): i;
      return lower < Max()? lower: Max();
    }
    //: The value 'i' is clipped to be within this range.
    
    inline IndexRangeC FirstHalf() const
    { return IndexRangeC(Min(),Center()); }
    //: Returns the index range < Min(), (Max()+Min()+1)/2 >.
    
    inline IndexRangeC FirstHalfD() const
    { return IndexRangeC(Min(),CenterD()); }
    //: Returns the index range < Min(), (Max()+Min())/2 >.
    
    inline IndexRangeC Enlarge(IndexC f) const
    { return IndexRangeC(Min(), Min() + Size()*f - 1); }
    //: Returns the index range whose number of elements is enlarged by
    //: the factor 'f'. The upper limits is changed.
    
    inline IndexRangeC Expand(IndexC n) const
    { return IndexRangeC(minI - n,maxI + n); }
    //: Returns the range extended by adding 'n' items on both limits of
    //: this range. 
    
    inline IndexRangeC Shrink(IndexC n) const
    { return IndexRangeC(minI + n,maxI - n); }
    //: Returns the range extended by adding 'n' items on both limits of
    //: this range. 
    
    inline IndexRangeC & ShrinkHigh(IndexC n);
    //: Returns the range shrinked by removing of the 
    //: last 'n' items on both limits of this range. 
    
    inline IndexRangeC & Swap(IndexRangeC & r);
    //: Exchanges the contents of this range and range 'r'. The function
    //: returns this range. 

    const IndexRangeC &Involve(IndexC i) { 
      if(minI > i) minI = i;
      if(maxI < i) maxI = i;
      return *this;
    }
    //: Modify this range to ensure index i is contained within it.

    const IndexRangeC &Involve(const IndexRangeC &subRange) {
      if(subRange.Min() < minI)
        minI = subRange.Min();
      if(subRange.Max() > maxI)
        maxI = subRange.Max();
      return *this;
    }
    //: Modify this range to ensure subRange is contained within it.
    
    const IndexRangeC operator+=(const IndexRangeC &subRange) {
      minI += subRange.minI;
      maxI += subRange.maxI;
      return *this;
    }
    //: Add subRange to this one.
    // minI += subRange.minI; minI += subRange.minI <br>
    // returns a reference to this range.
    
    const IndexRangeC operator-=(const IndexRangeC &subRange) {
      minI -= subRange.minI;
      maxI -= subRange.maxI;
      return *this;
    }
    //: Subtract subRange from this one.
    // minI -= subRange.minI;  minI -= subRange.minI <br>
    // returns a reference to this range.
    
    IndexRangeC operator+(const IndexRangeC &subRange) const 
    { return IndexRangeC(minI + subRange.minI,maxI + subRange.maxI); }
    //: Add ranges together.
    // returns minI + subRange.minI, maxI + subRange.maxI
    
    IndexRangeC operator-(const IndexRangeC &subRange) const 
    { return IndexRangeC(minI - subRange.minI,maxI - subRange.maxI); }
    //: Subtract ranges.
    // returns minI - subRange.minI, maxI - subRange.maxI
    
    IndexRangeC AlignWithin(IntT alignment) const {
      IndexRangeC ret((Min() % alignment) == 0 ? Min() : (Min() + alignment - Min() % alignment),
                      Max() - (Max()+1) % alignment);
      if(ret.Min() > ret.Max()) ret.Max() = ret.Min()-1;
      return ret;
    }
    //: Return a range within this range that has start and end points which are integer multples of 'alignment' 
    
    SizeT Hash() const {
      SizeT ret = minI.Hash();
      ret += ret << 11;
      ret += maxI.Hash();
      return ret;
    }
    //: Generate a hash value for the range.
  private:    
    IndexC minI; // Minimum index.
    IndexC maxI; // Maximum index.
    
    friend std::istream & operator>>(std::istream & s, IndexRangeC & range);
  };
  
  inline bool IsInside(IndexC i, const IndexRangeC & range)
  { return (range.Min() <= i) && (i <= range.Max()); }
  //: Returns true if the index 'i' is inside the index range 'r'.
  
  std::istream & operator>>(std::istream & s, IndexRangeC & r);
  // Read information from the input stream 's' and sets the index range
  // according obtained data.
  
  std::ostream & operator<<(std::ostream & s, const IndexRangeC & r);
  // Saves the index range 'r' into the output stream 's'.
  
  BinOStreamC &operator<<(BinOStreamC &s,const IndexRangeC &ir);  
  BinIStreamC &operator>>(BinIStreamC &s,IndexRangeC &ir);
  
  inline bool IndexRangeC::IsOverlapping(const IndexRangeC & r) const {
    return (!IsEmpty() && !r.IsEmpty()) 
      && (((Min() <= r.Max()) && (r.Min() <= Max()))
	  || ((r.Min() <= Max()) && (Min() <= r.Max())));
  }
  
  inline IndexRangeC &IndexRangeC::ClipBy(const IndexRangeC & r) {
    if (Min() < r.Min()) {
      Min() = r.Min();
      if(Max() < r.Min()) // Make sure there is some overlap.
        Max() = r.Min()-1; // Make range zero size.
    }
    if (Max() > r.Max()) {
      Max() = r.Max();
      if(Min() > r.Max()) // Make sure there is some overlap.
        Min() = r.Max()+1; // To make range zero size.
    }
    return *this;
  }
  
  inline IndexRangeC &IndexRangeC::ShrinkHigh(IndexC n) {
    Max() -= n;
    return *this;
  }
  
  inline IndexRangeC &IndexRangeC::Swap(IndexRangeC & r) {
    const IndexRangeC tmp = *this;
    *this = r;
    r = tmp;
    return *this;
  }
  
}
#endif
