// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_INDEXRANGE2D_HH
#define RAVL_INDEXRANGE2D_HH
///////////////////////////////////////////////////////////
//! file="Ravl/Core/Base/IndexRange2d.hh"
//! lib=RavlCore
//! author="Radek Marik, Bill Christmas"
//! docentry="Ravl.API.Core.Indexing"
//! rcsid="$Id$"
//! date="06/08/1995"
//! userlevel=Default

#include "Ravl/Index.hh"
#include "Ravl/IndexRange1d.hh"
#include "Ravl/Index2d.hh"

namespace RavlN {

  class BinIStreamC;
  class BinOStreamC;

  //! userlevel=Normal
  //: An index range of a 2D array

  class IndexRange2dC {
  public:

  //:-----------------
  //: Constructors

    IndexRange2dC()
    {}
    //: Default constructor.

    IndexRange2dC(IndexC rowNumber, IndexC colNumber)
      : rows(0, rowNumber-1),
	cols(0, colNumber-1)
    {}
    //: Constructs from number of rows and columns.

#if 0
    IndexRange2dC(IntT rowNumber,IntT colNumber)
      : rows(0, rowNumber-1),
	cols(0, colNumber-1)
    {}
    //: Constructs from number of rows and columns.
#endif

    IndexRange2dC(const IndexRangeC & rowRange,
		  const IndexRangeC & colRange)
      : rows(rowRange), cols(colRange)
    {}
    //: Constructs from row and column ranges.

    IndexRange2dC(const Index2dC &org,const Index2dC &end)
      : rows(org.Row(), end.Row()),
	cols(org.Col(),end.Col())
    {}
    //: Constructs from top left and bottom right corner points.

    IndexRange2dC(const Index2dC &centre,SizeT size)
      : rows(centre.Row()-size, centre.Row()+size),
	cols(centre.Col()-size,centre.Col()+size)
    {}
    //: Constructs from a centre point and a size.
    // "size" is the distance from the centre to the edge, so
    // a size of 0 gives a single element, and a size of 2 generates
    // a 5x5 square.

    IndexRange2dC(const Index2dC &centre,SizeT nrows,SizeT ncols);
    //: Constructs from a centre point and numbers of rows and cols.
    //The sizes passed to this function are the absolute size of the
    //rectangle, unlike <code>IndexRangeC(Index2dC &centre,SizeT size)</code>.  Note: if
    //<code>nrows</code> or <code>ncols</code> or both are even, there will be
    // offset of &frac12; in the centre of the rectangle towards the top left.

    IndexRange2dC(IndexC minRow, IndexC maxRow,
		  IndexC minCol, IndexC maxCol)
      : rows(minRow, maxRow),
	cols(minCol,maxCol)
    {}
    //: Create rectangle from individual values.

  //:-----------------
  //: Information about rectangle (including lvalue methods)

    inline IndexC TRow() const
    { return rows.Min(); }
    //: Returns the top row index.

    inline IndexC &TRow()
    { return rows.Min(); }
    //: Returns the top row index.

    inline IndexC BRow() const
    { return rows.Max(); }
    //: Returns the bottom row index.

    inline IndexC &BRow()
    { return rows.Max(); }
    //: Returns the bottom row index.

    inline IndexC LCol() const
    { return cols.Min(); }
    //: Returns the left side column index.

    inline IndexC &LCol()
    { return cols.Min(); }
    //: Returns the left side column index.

    inline IndexC RCol() const
    { return cols.Max(); }
    //: Returns the right side column index.

    inline IndexC &RCol()
    { return cols.Max(); }
    //: Returns the right side column index.

    inline Index2dC TopLeft() const
    { return Index2dC(rows.Min(),cols.Min()); }
    //: Returns the top-left index of the rectangle.

    inline Index2dC TopRight() const
    { return Index2dC(rows.Min(),cols.Max()); }
    //: Returns the top-right index of the rectangle.

    inline Index2dC BottomLeft() const
    { return Index2dC(rows.Max(),cols.Min()); }
    //: Returns the bottom-left index of the rectangle.

    inline Index2dC BottomRight() const
    { return Index2dC(rows.Max(),cols.Max()); }
    //: Returns the bottom-right index of the rectangle.

    inline Index2dC Center() const
    { return Index2dC(rows.Center(),cols.Center()); }
    //: Returns the index which is in the middle of the rectangle

    inline SizeT Rows() const
    { return rows.Size(); }
    //: The number of rows in the rectangle.

    inline SizeT Cols() const
    { return cols.Size(); }
    //: The number of rows in the rectangle.

    inline SizeT Area() const
    { return static_cast<SizeT>(Rows() * Cols()); }
    //: Area of rectangle (= rows &times; cols)

    inline Index2dC Size() const
    { return Index2dC(rows.Size(),cols.Size()); }
    //: Size as (rows, cols).

    inline const IndexRangeC & RowRange() const
    { return rows; }
    //: Access row range.

    inline IndexRangeC & RowRange()
    { return rows; }
    //: Access row range.

    inline const IndexRangeC & ColRange() const
    { return cols; }
    //: Access col range.

    inline IndexRangeC & ColRange()
    { return cols; }
    //: Access col range.

    inline const IndexRangeC & Range1() const
    { return rows; }
    //: Access row range.

    inline IndexRangeC & Range1()
    { return rows; }
    //: Access row range.

    inline const IndexRangeC & Range2() const
    { return cols; }
    //: Access col range.

    inline IndexRangeC & Range2()
    { return cols; }
    //: Access col range.

    inline bool IsValid() const
    { return rows.IsValid() && cols.IsValid(); }
    //: Returns true if both of the index ranges are valid.

    inline bool IsEmpty() const
    { return rows.IsEmpty() || cols.IsEmpty(); }
    //: Returns true if either of the index ranges is empty.

  //:-----------------
  //: Relationship of rectangle to other data

    inline bool Contains(const IndexRange2dC & oth) const
    { return Range1().Contains(oth.Range1()) && Range2().Contains(oth.Range2()); }
    //: Returns true if this range contains the subrange 'oth'.

    inline bool Contains(const Index2dC & oth) const
    { return Range1().Contains(oth.Row()) && Range2().Contains(oth.Col()); }
    //: Returns true if this range contains the subrange 'oth'.

    inline bool Contains(const TFVectorC<RealT,2> &vec) const
    { return Range1().Contains(vec[0]) && Range2().Contains(vec[1]); }
    //: Is a vector inside the rectangle ?

    inline bool IsOverlapping(const IndexRange2dC & r) const
    { return Range1().IsOverlapping(r.Range1()) && Range2().IsOverlapping(r.Range2()); }
    //: Returns true if this range contains at least one common index with
    //: the range 'r'.

  //:-----------------
  //: Changing this rectangle

    const IndexRange2dC &SetOrigin(const Index2dC &newOrigin) {
      rows.SetOrigin(newOrigin.Row());
      cols.SetOrigin(newOrigin.Col());
      return *this;
    }
    //: Set the origin of the range to 'newOrigin'
    // Returns a reference to this rectangle.

    inline IndexRange2dC & ClipBy(const IndexRange2dC & r) {
      Range1().ClipBy(r.Range1());
      Range2().ClipBy(r.Range2());
      return *this;
    }
    //: This index range is clipped to contain at most the index range 'r'.
    // NB. This is inplace.

    inline IndexRange2dC Involve(const Index2dC & index);
    //: Ensures this rectangle contains given index.
    // This method checks and changes, if necessary, the 2 dimensional range
    // to contain the 'index'.

    inline IndexRange2dC Involve(const IndexRange2dC &subrectangle) {
      Range1().Involve(subrectangle.Range1());
      Range2().Involve(subrectangle.Range2());
      return *this;
    }
    //: Ensures this rectangle contains given sub rectangle.
    // This method checks and changes, if necessary, the 2 dimensional range
    // to contain the 'subrectangle'.

  //:-----------------
  //: Creating new rectangle

    inline IndexRange2dC Dilate() const
    { return IndexRange2dC(rows.Expand(1),cols.Expand(1)); }
    //: Returns a new rectangle one index larger on each side.

    inline IndexRange2dC Erode() const
    { return IndexRange2dC(rows.Shrink(1),cols.Shrink(1)); }
    //: Returns an rectangle with each side 1 index closer to the centre.

    inline IndexRange2dC Expand(IndexC n) const
    { return IndexRange2dC(rows.Expand(n),cols.Expand(n)); }
    //: Returns an rectangle expanded by 'n' indexs on each side.

    inline IndexRange2dC Shrink(IndexC n) const
    { return IndexRange2dC(rows.Shrink(n),cols.Shrink(n)); }
    //: Returns a new rectangle which has layer of the width of 'n' indexs
    //: removed.

    IndexRange2dC Rotate90(Index2dC centre = Index2dC(0,0)) const;
    //: Return a rotated rectangle 90&deg; around the given centre

    IndexRange2dC Rotate180(Index2dC centre = Index2dC(0,0)) const;
    //: Return a rotated rectangle 180&deg; around the given centre.

    IndexRange2dC Rotate270(Index2dC centre = Index2dC(0,0)) const;
    //: Return a rotated rectangle 270&deg; around the given centre.

    IndexRange2dC AlignWithin(IntT alignment) const
    { return IndexRange2dC(Range1().AlignWithin(alignment),Range2().AlignWithin(alignment)); }
    //: Return a range within this range that has start and end points which are integer multples of 'alignment'

    inline IndexRange2dC Clip(const IndexRange2dC & r) const {
      IndexRange2dC ret(r);
      return ret.ClipBy(*this);
    }
    //: Return rectangle 'r' clipped by this one.

    inline Index2dC Clip(const Index2dC & i) const {
      Index2dC result(Range1().Clip(i[0]), Range2().Clip(i[1]));
      return result;
    }
    //: The value 'i' is clipped to be within this range.

    inline IndexRange2dC Intersection(const IndexRange2dC & r) const
    { IndexRange2dC ret = (*this); ret.ClipBy(r); return ret; }
    //: Return the intersection of this range and range 'r'.

  //:-----------------
  //: Operators

    bool operator==(const IndexRange2dC &oth) const
    { return oth.Range1() == Range1() && oth.Range2() == Range2(); }
    //: Are two ranges equal ?

    bool operator!=(const IndexRange2dC &oth) const
    { return oth.Range1() != Range1() || oth.Range2() != Range2(); }
    //: Are two ranges unequal ?

    inline IndexRange2dC operator+(const Index2dC & offset) const
    { return IndexRange2dC(Range1() + offset[0],Range2() + offset[1]); }
    //: Shifts the rectangle to the new position.

    inline const IndexRange2dC & operator+=(const Index2dC & offset);
    //: Shifts this rectangle to the new position.

    inline IndexRange2dC operator-(const Index2dC & offset) const
    { return IndexRange2dC(Range1() - offset[0],Range2() - offset[1]); }
    //: Shifts the rectangle to the new position.

    inline const IndexRange2dC & operator-=(const Index2dC & offset);
    //: Shifts this rectangle to the new position.

    inline IndexRange2dC operator*( const IndexC & factor) const
      { return IndexRange2dC ( Range1() * factor, Range2() * factor) ; }
    //: Multiply the ranges by a factor

    inline IndexRange2dC operator/( const IndexC & factor) const
      { return IndexRange2dC ( Range1() / factor, Range2()  / factor ) ; }
    //: Divide the ranges by a factor
    // IndexC rounding rules apply


    const IndexRange2dC &operator+=(const IndexRange2dC &sr) {
      TRow() += sr.TRow();
      BRow() += sr.BRow();
      LCol() += sr.LCol();
      RCol() += sr.RCol();
      return *this;
    }
    //: Add the 'sr' to this rectangle.
    // This does TRow() += sr.TRow(), BRow() += sr.BRow().....

    IndexRange2dC operator+(const IndexRange2dC &sr) const {
      IndexRange2dC ret(*this);
      ret.TRow() += sr.TRow();
      ret.BRow() += sr.BRow();
      ret.LCol() += sr.LCol();
      ret.RCol() += sr.RCol();
      return ret;
    }
    //: Add the 'sr' to this rectangle.
    // As operator+=(const IndexRange2dC &sr), but returns the modified rectangle.

    const IndexRange2dC &operator-=(const IndexRange2dC &sr) {
      TRow() -= sr.TRow();
      BRow() -= sr.BRow();
      LCol() -= sr.LCol();
      RCol() -= sr.RCol();
      return *this;
    }
    //: Subtract the 'sr' from this rectangle.
    // This does TRow() -= sr.TRow(), BRow() -= sr.BRow().....

    IndexRange2dC operator-(const IndexRange2dC &sr) const {
      IndexRange2dC ret(*this);
      ret.TRow() -= sr.TRow();
      ret.BRow() -= sr.BRow();
      ret.LCol() -= sr.LCol();
      ret.RCol() -= sr.RCol();
      return ret;
    }
    //: Subtract the 'sr' from this rectangle.
    // As operator-=(const IndexRange2dC &sr), but returns the modified rectangle.

    const IndexRange2dC & operator *= (const IndexC & factor) {
      Range1() *= factor ;
      Range2() *= factor ;
      return *this ;
    }
    //: Multiply the ranges by a factor


    const IndexRange2dC & operator /= (const IndexC & factor ) {
      Range1() /= factor ;
      Range2() /= factor ;
      return *this ;
    }
    //: Divides the ranges by a factor
    // IndexC rounding rules apply

  //:-----------------
  //: Obsolete

    inline Index2dC Origin() const
    { return Index2dC(rows.Min(),cols.Min()); }
    //: This method is obsolete: use TopLeft() instead.

    inline Index2dC  End() const
    { return Index2dC(rows.Max(),cols.Max()); }
    //: This method is obsolete: use BottomRight() instead.

    SizeT Hash() const {
      SizeT ret = rows.Hash();
      ret += ret << 11;
      ret += cols.Hash();
      return ret;
    }
    //: Generate a hash value for the range.

  protected:
    inline const IndexRange2dC & Range() const
    { return(*this); }

  private:
    IndexRangeC rows;
    IndexRangeC cols;
  };

  std::ostream &operator<<(std::ostream &s,const IndexRange2dC &ir);
  std::istream &operator>>(std::istream &s,IndexRange2dC &ir);

  BinOStreamC &operator<<(BinOStreamC &s,const IndexRange2dC &ir);
  BinIStreamC &operator>>(BinIStreamC &s,IndexRange2dC &ir);

  ///////////////////////////////////////////////////////

  inline
  const IndexRange2dC &IndexRange2dC::operator+=(const Index2dC & offset) {
    rows += offset.Row();
    cols += offset.Col();
    return *this;
  }

  inline
  const IndexRange2dC & IndexRange2dC::operator-=(const Index2dC & offset) {
    rows -= offset.Row();
    cols -= offset.Col();
    return *this;
  }

  inline
  IndexRange2dC
  IndexRange2dC::Involve(const Index2dC & index) {
    if (rows.Min() > index.Row()) rows.Min() = index.Row();
    if (rows.Max() < index.Row()) rows.Max() = index.Row();
    if (cols.Min() > index.Col()) cols.Min() = index.Col();
    if (cols.Max() < index.Col()) cols.Max() = index.Col();
    return *this;
  }

}
#endif
