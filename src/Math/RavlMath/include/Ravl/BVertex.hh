// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BVERTEX_HEADER
#define RAVL_BVERTEX_HEADER
/////////////////////////////////////////////////////////////////////////
//! file="Ravl/Math/Geometry/Euclidean/Boundary/BVertex.hh"
//! lib=RavlMath
//! userlevel=Normal
//! author="Radek Marik"
//! date="26/06/1994"
//! docentry="Ravl.API.Images.Boundaries;Ravl.API.Math.Geometry.2D.Boundary"
//! rcsid="$Id$"

#include "Ravl/Index2d.hh"
#include "Ravl/Point2d.hh"
#include "Ravl/Stream.hh"
#include "Ravl/BinStream.hh"

namespace RavlN {

  class Point2dC;

  //! userlevel=Normal
  //: Boundary vertex
  // The relationship between pixel coordinates and boundary
  // vertex is that the boundary vertex [i,j] is the up-left corner
  // of the pixel (i,j). The figure contains the orientation of edges too.
  //
  //<pre>
  //        [i,j] <------------ [i,j+1]
  //        |                    /^\         .
  //        |      (i,j)          |       
  //        V                     |       
  //       [i+1,j] ---------> [i+1, j+1]
  //</pre>
  
  class BVertexC
    : public Index2dC
  {
  public:
    BVertexC()
    {}
    //: Default constructor.
    // The value of the vertex is undefined.
    
    BVertexC(IndexC row, IndexC col)
      : Index2dC(row,col)
    {}
    //: Create the boundary vertex from two coordinates.

    BVertexC(const Index2dC & px)
      : Index2dC(px)
    {}
    //: Create the upper-left vertex of the pixel 'px'.
    
    BVertexC(const Point2dC & point)
      : Index2dC(point)
    {}
    //: Transform the point coordinates into the vertex.
    
    inline const BVertexC & BVertex() const
    { return(*this); }
    //: Access to the constant object

    inline BVertexC & BVertex()
    { return(*this); }
    //: Access to the object
  };

  inline ostream &operator<<(ostream &s,const BVertexC &v) { 
    s << (const Index2dC &) v;
    return s;
  }
  //: Write vertex to ostream.
  
  inline istream &operator>>(istream &s,BVertexC &v) {
    s >> (Index2dC &) v;
    return s;
  }
  //: Read vertex from ostream.
  
  inline BinOStreamC &operator<<(BinOStreamC &s,const BVertexC &v) { 
    s << (const Index2dC &) v;
    return s;
  }
  //: Write vertex to BinOStreamC.
  
  inline BinIStreamC &operator>>(BinIStreamC &s,BVertexC &v) {
    s >> (Index2dC &) v;
    return s;
  }
  //: Read vertex from BinIStreamC.

}
#endif
