// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlMath
//! userlevel=Normal
//! author="Radek Marik"
//! date="26/12/1994"
//! rcsid="$Id$"
//! file="Ravl/Math/Geometry/Euclidean/Boundary/ConvexHull.cc"

#include "Ravl/DList.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/Boundary.hh"

namespace RavlN {

  //: Creates the boundary from the list of pointers to the elementary edges.
  // The orientation of the boundary is set according to 'orient'.
  
  BoundaryC::BoundaryC(const DListC<DLIterC<CrackC> > & edgeList, bool orient) 
    : orientation(orient)
  {
    for (DLIterC<DLIterC<CrackC> > p(edgeList); p; p++)
      InsLast(p->Data());
  }
  
  ////////////////////////////////////////////////////////////////////////////////
  
  // Checks if the pixel px is locally a convex point supposing that
  // <nextP, pxP, prevP> are vertexes in counterclockwise order. 

  static inline
  bool ConvexVertex(const CrackC & next, 
		    const CrackC & px, 
		    const CrackC & prev)
  {
    return ( ((prev.Row() - px.Row()) * (next.Col() - px.Col()))
	     <((next.Row() - px.Row()) * (prev.Col() - px.Col()))
	     );
  }

  // Checks if the pixel px is locally a concave point supposing that
  // <nextP, pxP, prevP> are vertexes in counterclockwise order. 

  static inline
  bool ConcaveVertex(const CrackC & next, 
		     const CrackC & px, 
		     const CrackC & prev)
  {
    return ( ((prev.Row() - px.Row()) * (next.Col() - px.Col()))
	     > ((next.Row() - px.Row()) * (prev.Col() - px.Col()))
	     );
  }


  DListC<DLIterC<CrackC> >  BoundaryC::ConvexHull() const {
    DListC<DLIterC<CrackC> >   hull;
    if (IsEmpty())
      return hull;
    
    // Find three vertexes which are not colinear.
    DLIterC<CrackC> v1(*this);
    for(; v1.IsElm(); v1.Next()) {
      DLIterC<CrackC> v2(v1);
      DLIterC<CrackC> v3(v2);
      v3.Next();
      v2.Next();
      if (!v3 || !v2)
	// A singular polygon
	return hull;
      if (ConvexVertex(*v3,*v2,*v1) || ConcaveVertex(*v3,*v2,*v1))
	break;
    }

    // Put three non-colinear vertexes into the hull
    // to create a convex triangle. 
    DLIterC<CrackC> v(v1);
    hull.InsFirst(v);
    v.Next();
    DLIterC<CrackC> vn(v); vn.Next();
    DLIterC<CrackC> vp(v); vp.Next();
    if (ConvexVertex(*vn, *v, *vp))
      hull.InsLast(v);
    else
      hull.InsFirst(v);
    v.Next();
    hull.InsFirst(v);
    hull.InsLast(v);

    // Create iterators at the top and bottom of the convex hull.
    DLIterC<DLIterC<CrackC> > vTop1(hull); 
    DLIterC<DLIterC<CrackC> > vTop2(vTop1);
    vTop2.Next(); 
    DLIterC<DLIterC<CrackC> > vBot1(hull);
    vBot1.PrevCrc(); 
    DLIterC<DLIterC<CrackC> > vBot2(vBot1);
    vBot2.Prev(); 
    
    v.NextCrc();
    while(v != v1) {
      // Remove all vertexes which are inside of the current convex hull.
      while(   !ConcaveVertex(*v, *vBot1.Data(), *vBot2.Data())
	       && !ConcaveVertex(*vTop2.Data(), *vTop1.Data(), *v)) {
	v.NextCrc();
	if (v == v1)
	  goto READY;
      }
      
      // A vertex outside the current convex hull has been found.
      
      // Remove all concave vertexes from the top of the convex hull.
      while( !ConvexVertex(*vTop2.Data(), *vTop1.Data(), *v)) {
	vTop1.Next();
	vTop2.Next();
	hull.DelFirst();
      }
      hull.InsFirst(v);
      vTop1.Prev();
      vTop2.Prev();
      
      // Remove all concave vertexes from the bottom of the convex hull.
      while( !ConvexVertex(*v, *vBot1.Data(),*vBot2.Data())) {
	vBot1.Prev();
	vBot2.Prev();
	hull.DelLast();
      }
      hull.InsLast(v);
      vBot1.Next();
      vBot2.Next();
      
      v.NextCrc();
    }
    
  READY:
    // The last vertex was entered 2 times.
    hull.DelLast();
    return hull;
  }
}
