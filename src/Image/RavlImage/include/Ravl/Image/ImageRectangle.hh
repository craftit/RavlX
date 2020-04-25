// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_IMAGERECTANGLE_HEADER
#define RAVLIMAGE_IMAGERECTANGLE_HEADER 1
////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Image/Base/ImageRectangle.hh"
//! lib=RavlImage
//! userlevel=Normal
//! author="Charles Galambos"
//! date="24/01/2001"
//! docentry="Ravl.API.Images"

#include "Ravl/IndexRange2d.hh"

namespace RavlImageN {
  using namespace RavlN;
  
#if RAVL_VISUALCPP_NAMESPACE_BUG
  using RavlN::IndexRange2dC;
  using RavlN::IntT;
  using RavlN::Index2dC;
  using RavlN::SizeT;
  using RavlN::IndexC;
#endif
  
  //! userlevel=Normal
  //: Image Rectangle.
  
  class ImageRectangleC 
    : public IndexRange2dC
  {
  public:
    ImageRectangleC()
    {}
    //: Default constructor.
    // creates an undefined rectangle.
    
    ImageRectangleC(const IndexRange2dC &oth)
      : IndexRange2dC(oth)
    {}
    //: Base constructor.

    ImageRectangleC(IntT rows,IntT cols)
      : IndexRange2dC(0,rows-1,0,cols-1)
    {}
    //: Construct a rectangle of the given size with an origin of 0,0
    
    ImageRectangleC(const Index2dC &org,const Index2dC &end)
      : IndexRange2dC(org,end)
    {}
    //: Construct from corner points.

    ImageRectangleC(const Index2dC &center,SizeT size)
      : IndexRange2dC(center,size)
    {}
    //: Construct a square from center and size.
    // "size" is the distance from the centre to the edge, so
    // a size of 0 gives a single pixel, and a size of 2 generates
    // a 5x5 square.
    
    ImageRectangleC(const Index2dC &centre,SizeT rows,SizeT cols)
      : IndexRange2dC(centre,rows,cols)
    {}
    //: Create a rectangle from a centre point and sizes for rows and cols.
    // The sizes passed to this function are the absolute size of the
    // rectangle.
    // Note: if the rows or cols are even there will be a half pixel offset in
    // the center of the rectangle. 

    ImageRectangleC(IndexC TRow, IndexC BRow,
		    IndexC LCol, IndexC RCol)
      : IndexRange2dC(TRow,BRow,LCol,RCol)
    {}
    //: Create rectangle from indvidual values.

    ImageRectangleC & ReflectUp() {
      RowRange() -= RowRange().Size();
      return *this;
    }
    //: Mirrors the image rectangle along its top edge.
    // Returns a reference to this rectangle.
    
    ImageRectangleC & ReflectDown() {
      RowRange() += RowRange().Size();
      return *this;      
    }
    //: Mirrors the image rectangle along its bottom edge.
    // Returns a reference to this rectangle.
    
    ImageRectangleC & ReflectLeft() {
      ColRange() -= ColRange().Size();
      return *this;
    }
    //: Mirrors the image rectangle along its left edge.
    // Returns a reference to this rectangle.
    
    ImageRectangleC & ReflectRight() {
      ColRange() += ColRange().Size();
      return *this;
    }
    //: Mirrors the image rectangle along its right edge.
    // Returns a reference to this rectangle.
    
    RealT AspectRatio() const {
      return static_cast<RealT>(Cols())/static_cast<RealT>(Rows());
    }
    //: Calculate the aspect ratio of the image rectangle
    // aspect = number of columns / number of rows

    Index2dC Nearest(const Index2dC& pixel) const;
    //: Nearest pixel to rectangle
    // If pixel is outside rectangle, returns nearest pixel to rectangle; otherwise returns pixel

  };
  
  //: Nearest pixel to rectangle
  inline Index2dC ImageRectangleC::Nearest(const Index2dC& pixel) const {
    Index2dC nearest(pixel);
    if (this->Contains(pixel)) return nearest;
    if(nearest[0] < TRow())  nearest[0] = TRow();
    if(nearest[0] > BRow())  nearest[0] = BRow();
    if(nearest[1] < LCol())  nearest[1] = LCol();
    if(nearest[1] > RCol())  nearest[1] = RCol();
    return nearest;
  }


}




#endif
