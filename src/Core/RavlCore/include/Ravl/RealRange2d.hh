// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_REALRANGE2D_HH
#define RAVL_REALRANGE2D_HH
///////////////////////////////////////////////////////////
//! file="Ravl/Core/Base/RealRange2d.hh"
//! lib=RavlCore
//! author="Radek Marik"
//! docentry="Ravl.API.Core.Indexing"
//! rcsid="$Id$"
//! date="06/08/1995"
//! userlevel=Normal

#include "Ravl/RealRange1d.hh"
#include "Ravl/TFVector.hh"
#include "Ravl/IndexRange2d.hh"

namespace RavlN {
  
  class IndexRange2dC;
  
  class BinIStreamC;
  class BinOStreamC;
  
  //! userlevel=Normal
  //: An index range of a 2D array
  
  class RealRange2dC {
  public:
    RealRange2dC()
    {}
    //: Default constructor.
    
    RealRange2dC(RealT rowNumber, RealT colNumber)
      : rows(0, rowNumber), 
	cols(0, colNumber)
    {}
    //: Constructor.
    
    explicit RealRange2dC(const IndexRange2dC &rng)
      : rows(rng.Range1()),
	cols(rng.Range2())
    {}
    //: Construct from an IndexRange2dC.
    // Note that the upper limits of the RealRange2dC object are incremented by 1
    // to make the range consistent.
    
    RealRange2dC(const RealRangeC & rowRange,
		 const RealRangeC & colRange);
    //: Constructor.
    
    RealRange2dC(const RealRange2dC & range);
    //: Constructor.

    RealRange2dC(const TFVectorC<RealT,2> &org,const TFVectorC<RealT,2> &end)
      : rows(org[0],end[0]), 
	cols(org[1],end[1])
    {}
    //: Create an 2d range from corner points.
    
    RealRange2dC(const TFVectorC<RealT,2> &center,RealT size)
      : rows(center[0]-size,center[0]+size), 
	cols(center[1]-size,center[1]+size)
    {}
    //: Create an 2d range from a center point and a size.
    // Size is the distance from the center to the edge, so
    // a size of 0 gives a single pixel, and a size of 1 generates
    // a 3x3 square.
    
    RealRange2dC(const TFVectorC<RealT,2> &center,RealT nrows,RealT ncols);
    //: Create an 2d range from a center point and a size for rows and cols.
    // The sizes passed to this function are the absolute size of the
    // rectangle, unlike RealRangeC(Real2dC &center,RealT size).
    
    RealRange2dC(RealT minRow, RealT maxRow,
		 RealT minCol, RealT maxCol)
      : rows(minRow,maxRow), 
	cols(minCol,maxCol)
    {}
    //: Create rectangle from indvidual values.
    
    const RealRange2dC &SetOrigin(const TFVectorC<RealT,2> &newOrigin) {
      rows.SetOrigin(newOrigin[0]);
      cols.SetOrigin(newOrigin[1]);
      return *this;
    }
    //: Set the origin of the range to 'newOrigin'
    // Returns a reference to this rectangle.
    
    inline TFVectorC<RealT,2> Origin() const
    { return TFVector2(rows.Min(),cols.Min()); }
    //: Returns the top-left index of the rectangle.
    
    inline TFVectorC<RealT,2>  End() const
    { return TFVector2(rows.Max(),cols.Max()); }
    //: Returns the bottom-right index of the rectangle.
    
    inline TFVectorC<RealT,2> TopRight() const
    { return TFVector2(rows.Min(),cols.Max()); }
    //: Returns the top-right index of the rectangle.
    
    inline TFVectorC<RealT,2> TopLeft() const
    { return TFVector2(rows.Min(),cols.Min()); }
    //: Returns the top-left index of the rectangle.
    
    inline TFVectorC<RealT,2>  BottomLeft() const
    { return TFVector2(rows.Max(),cols.Min()); }
    //: Returns the bottom-left index of the rectangle.
    
    inline TFVectorC<RealT,2>  BottomRight() const
    { return TFVector2(rows.Max(),cols.Max()); }
    //: Returns the bottom-right index of the rectangle.
    
    inline TFVectorC<RealT,2> Center() const
    { return TFVector2(rows.Center(),cols.Center()); }
    //: Returns the index which is in the middle of the rectangle
    
    inline RealT TRow() const
    { return rows.Min(); }
    //: Returns the top row index.
    
    inline RealT LCol() const
    { return cols.Min(); }
    //: Returns the left side column index.
    
    inline RealT BRow() const
    { return rows.Max(); }
    //: Returns the bottom row index.
    
    inline RealT RCol() const
    { return cols.Max(); }
    //: Returns the right side column index.
    
    inline RealT &TRow()
    { return rows.Min(); }
    //: Returns the top row index.
    
    inline RealT &LCol()
    { return cols.Min(); }
    //: Returns the left side column index.
    
    inline RealT &BRow()
    { return rows.Max(); }
    //: Returns the bottom row index.
    
    inline RealT &RCol()
    { return cols.Max(); }
    //: Returns the right side column index.
    
    inline RealT Rows() const
    { return rows.Size(); }
    //: The number of rows in the rectangle.
    
    inline RealT Cols() const
    { return cols.Size(); }
    //: The number of rows in the rectangle.
    
    inline RealT Area() const
    { return (RealT) Rows() * Cols(); }
    //: Returns the area of the image rectangle expressed in number of indexs.
    
    inline RealRange2dC Dilate() const 
    { return RealRange2dC(rows.Expand(1),cols.Expand(1)); }
    //: Returns a new rectangle one index larger on each side.
    
    inline RealRange2dC Erode() const
    { return RealRange2dC(rows.Shrink(1),cols.Shrink(1)); }
    //: Returns an rectangle with each side 1 index closer to the center.
    
    inline RealRange2dC Expand(RealT n) const
    { return RealRange2dC(rows.Expand(n),cols.Expand(n)); }
    //: Returns an rectangle expanded by 'n' indexs on each side.
    
    inline RealRange2dC Shrink(RealT n) const
    { return RealRange2dC(rows.Shrink(n),cols.Shrink(n)); }
    //: Returns a new rectangle which has layer of the width of 'n' indexs
    //: removed.
    
    inline RealRange2dC & ClipBy(const RealRange2dC & r) {
      Range1().ClipBy(r.Range1());
      Range2().ClipBy(r.Range2());
      return *this;
    }
    //: This index range is clipped to contain at most the index range 'r'.

    inline TFVectorC<RealT,2> Clip(const TFVectorC<RealT,2> & r) {
      TFVectorC<RealT,2> result;
      result[0] = Range1().Clip(r[0]);
      result[1] = Range2().Clip(r[1]);
      return result;
    }
    //: The value 'r' is clipped to be within this range.
    
    inline bool Contains(const RealRange2dC & oth) const
    { return Range1().Contains(oth.Range1()) && Range2().Contains(oth.Range2()); }
    //: Returns true if this range contains the subrange 'oth'.
    
    inline bool Contains(const TFVectorC<RealT,2> & oth) const
    { return Range1().Contains(oth[0]) && Range2().Contains(oth[1]); }
    //: Returns true if this range contains the subrange 'oth'.
    
    inline const RealRange2dC & operator+=(const TFVectorC<RealT,2> & offset);
    //: Shifts the rectangle to the new position.
    
    inline const RealRange2dC & operator-=(const TFVectorC<RealT,2> & offset);
    //: Shifts the rectangle to the new position.
    
    inline RealRange2dC operator+(const TFVectorC<RealT,2> & offset) const
    { return RealRange2dC(rows+offset[0],cols+offset[1] ) ; }
    //: Shifts the rectangle to the new position.
    
    inline RealRange2dC operator-(const TFVectorC<RealT,2> & offset) const
    { return RealRange2dC(rows - offset[0],cols - offset[1]); }
    //: Shifts the rectangle to the new position.
    
    inline const RealRangeC & RowRange() const
    { return rows; }
    //: Access row range.
    
    inline const RealRangeC & ColRange() const
    { return cols; }
    //: Access col range.
    
    inline RealRangeC & RowRange()
    { return rows; }
    //: Access row range.
    
    inline RealRangeC & ColRange()
    { return cols; }
    //: Access col range.
    
    inline const RealRangeC & Range1() const
    { return rows; }
    //: Access row range.
    
    inline const RealRangeC & Range2() const
    { return cols; }
    //: Access col range.
    
    inline RealRangeC & Range1()
    { return rows; }
    //: Access row range.
    
    inline RealRangeC & Range2()
    { return cols; }
    //: Access col range.
    
    inline void Involve(const TFVectorC<RealT,2> & index);
    //: Ensures this rectangle contains given index.
    // This method checks and changes, if necessary, the 2 dimensional range
    // to contain the 'index'.
    
    inline void Involve(const RealRange2dC &subrectangle) { 
      Range1().Involve(subrectangle.Range1()); 
      Range2().Involve(subrectangle.Range2()); 
    }
    //: Ensures this rectangle contains given sub rectangle.
    // This method checks and changes, if necessary, the 2 dimensional range
    // to contain the 'subrectangle'.
    
    inline bool IsValid() const 
    { return rows.IsValid() && cols.IsValid(); }
    //: Returns true if this rectangle contains at least one index.

    bool operator==(const RealRange2dC &oth) const
    { return oth.Range1() == Range1() && oth.Range2() == Range2(); }
    //: Are two ranges equal ?
    
    bool operator!=(const RealRange2dC &oth) const
    { return oth.Range1() != Range1() || oth.Range2() != Range2(); }
    //: Are two ranges unequal ?
    
    RealRange2dC Rotate180(TFVectorC<RealT,2> centre);
    //: Rotate rectangle 180 degree's around the given center.
    
    inline bool IsOverlapping(const RealRange2dC & r) const
    { return Range1().IsOverlapping(r.Range1()) && Range2().IsOverlapping(r.Range2()); }
    //: Returns true if this range contains a common area with 
    //: the range 'r'.
    
    IndexRange2dC IndexRange() const
    { return IndexRange2dC(rows.IndexRange(),cols.IndexRange());  }
    //: Get the smallest integer range containing the real range.

    RealRange2dC operator*(const TFVectorC<RealT,2> &scale) const
    { return RealRange2dC(Range1() * scale[0],Range2() * scale[1]); }
    //: Scale range

    RealRange2dC operator*(RealT scale) const
    { return RealRange2dC(Range1() * scale,Range2() * scale); }
    //: Scale range

    UIntT Hash() const {
      UIntT ret = StdHash(rows);
      ret += ret << 11;
      ret += StdHash(cols);
      return ret;
    }
    //: Generate a hash value for the range.
    //: For template compatibility

    RealRangeC &operator[](UIntT ind) {
#if RAVL_CHECK
      if(ind > 1)
        IssueError(__FILE__,__LINE__,"Index %u out of range, 0 - 1",ind);
#endif
      if(ind <= 0)
        return rows;
      return cols;
    }
    //: Access item.

    const RealRangeC &operator[](UIntT ind) const {
#if RAVL_CHECK
      if(ind > 1)
        IssueError(__FILE__,__LINE__,"Index %u out of range, 0 - 1",ind);
#endif
      if(ind <= 0)
        return rows;
      return cols;
    }
    //: Access item.

  protected:
    inline const RealRange2dC & Range() const
    { return(*this); }
    
  private:
    RealRangeC rows;
    RealRangeC cols;
  };
  
  IndexRange2dC operator*(const RealRange2dC &realRange,const IndexRange2dC &indexRange);
  //: Multiply a 2d index range by a real 2d range.
  // Multiplying by a real range of 0-1,0-1 is a unit transform.
  
  std::ostream &operator<<(std::ostream &s,const RealRange2dC &ir);
  std::istream &operator>>(std::istream &s,RealRange2dC &ir);
  
  BinOStreamC &operator<<(BinOStreamC &s,const RealRange2dC &ir);  
  BinIStreamC &operator>>(BinIStreamC &s,RealRange2dC &ir);
  
  ///////////////////////////////////////////////////////
  
  inline 
  const RealRange2dC &RealRange2dC::operator+=(const TFVectorC<RealT,2> & offset) {
    rows += offset[0];
    cols += offset[1];
    return *this;
  }
  
  inline 
  const RealRange2dC & RealRange2dC::operator-=(const TFVectorC<RealT,2> & offset) {
    rows -= offset[0];
    cols -= offset[1];
    return *this;
  }
  
  inline 
  void RealRange2dC::Involve(const TFVectorC<RealT,2> & index) {
    if (rows.Min() > index[0]) rows.Min() = index[0];
    if (rows.Max() < index[0]) rows.Max() = index[0];
    if (cols.Min() > index[1]) cols.Min() = index[1];
    if (cols.Max() < index[1]) cols.Max() = index[1];
  }
  
}
#endif
