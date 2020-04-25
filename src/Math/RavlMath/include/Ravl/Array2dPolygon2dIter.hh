// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_ARRAY2DPOLYGON2DITER_HEADER
#define RAVL_ARRAY2DPOLYGON2DITER_HEADER 1
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Math/Sequence/Array2dPolygon2dIter.hh"
//! lib=RavlMath
//! userlevel=Normal
//! author="Robert Crida"
//! docentry="Ravl.API.Math.Sequences"
//! date="08/02/1999"

#include "Ravl/Polygon2dIter.hh"
#include "Ravl/Array1dIter.hh"
#include "Ravl/Array2d.hh"

namespace RavlN {
  //: Iterate across the interior of a polygon on an index grid within an array
  
  template<class DataT>
  class Array2dPolygon2dIterC {
  public:  
    Array2dPolygon2dIterC(const Array2dC<DataT> &array, const Polygon2dC &polygon);
    //: Constructor.
    
    void First();
    //: Goto point of the iterator.
    
    bool IsElm() const
    { return m_arrayIter.IsElm(); }
    //: At valid position ?

    operator bool() const
    { return IsElm(); }
    //: At a valid position ?

    DataT& Data()
    { return m_arrayIter.Data(); }
    //: Access data.

    const DataT& Data() const
    { return m_arrayIter.Data(); }
    //: Constant access to data.

    DataT &operator*()
    { return Data(); }
    //: Access data.
    
    const DataT &operator*() const
    { return Data(); }
    //: Constant access to data.
    
    DataT *operator->()
    { return &Data(); }
    //: Access member function of data..
    
    const DataT *operator->() const
    { return &Data(); }
    //: Constant access to member function of data..

    Index2dC Index()
    { return Index2dC(m_polygonIter.Row(), m_arrayIter.Index()); }
    //: Access to the current index in the array

    bool Next();
    //: Goto next scan line.
    // Returns true if we're now at a valid scanline.
    
    bool operator++(int)
    { return Next(); }
    //: Goto next point.
    // Returns true if we're now at a valid scanline.
  private:
    Array2dC<DataT> m_array;
    Polygon2dIterC m_polygonIter;
    Array1dIterC<DataT> m_arrayIter;
    IndexRangeC m_rowRange;
    IndexRangeC m_colRange;
  };

  template<class DataT>
  Array2dPolygon2dIterC<DataT>::Array2dPolygon2dIterC(const Array2dC<DataT> &array, const Polygon2dC &polygon)
    : m_array(array),
      m_rowRange(array.Range1()),
      m_colRange(array.Range2())
  { 
    m_polygonIter = Polygon2dIterC(polygon);
    First();
  }

  template<class DataT>
  void Array2dPolygon2dIterC<DataT>::First() {
    m_polygonIter.First();
    Next();
  }

  template<class DataT>
  bool Array2dPolygon2dIterC<DataT>::Next() {
    if (m_arrayIter.IsElm()) {
      m_arrayIter.Next();
      if (m_arrayIter.IsElm())
        return true;
    }
    for (; m_polygonIter; m_polygonIter++) {
      IndexC row = m_polygonIter.Row();
      IndexRangeC indexRange = m_polygonIter.IndexRange();
      if (m_rowRange.Contains(row) &&
          m_colRange.IsOverlapping(indexRange)) {
        m_arrayIter = Array1dIterC<DataT>(m_array.SliceRow(row), indexRange.ClipBy(m_colRange));
        m_polygonIter++;
        return true;
      }
    }
    m_arrayIter = Array1dIterC<DataT>();
    return false;
  }

}  

#endif
