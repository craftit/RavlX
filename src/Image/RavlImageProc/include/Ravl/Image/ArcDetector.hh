// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_ARCDETECTOR_HEADER
#define RAVLIMAGE_ARCDETECTOR_HEADER 1
//! rcsid="$Id$"
//! lib=RavlImageProc
//! author="Charles Galambos"
//! docentry="Ravl.API.Images.Lines"
//! date="19/11/2002"
//! file="Ravl/Image/Processing/Lines/ArcDetector.hh"
//! example="doArcs.cc"

#include "Ravl/Image/Edgel.hh"
#include "Ravl/Arc2d.hh"
#include "Ravl/DList.hh"
#include "Ravl/Array1dIter.hh"

namespace RavlImageN {
  using namespace RavlN;
  
  //! userlevel=Normal
  //: A circular arc and associated edgels.
  // !!EXPERIMENTAL!!
  
  class Arc2dSegmentC 
    : public Arc2dC
  {
  public:
    Arc2dSegmentC()
    {}
    //: Default constructor.
    
    Arc2dSegmentC(const Arc2dC &arc,const Array1dC<Point2dC> &nEdges)
      : Arc2dC(arc),
	edges(nEdges),
	size(nEdges.Size())
    {}
    //: Construct from an arc and a set of edges.
    
    const Array1dC<Point2dC> &Edges() const
    { return edges; }
    //: Access edges on arc.

    Array1dC<Point2dC> &Edges()
    { return edges; }
    //: Access edges on arc.
    
    IntT IMax() const
    { return edges.IMax().V(); }
    //: Maximum index.
    
    IntT IMin() const
    { return edges.IMin().V(); }
    //: Maximum index.

    const IndexRangeC &Range() const
    { return edges.Range(); }
    //: Range of values.
    
    IntT Size() const
    { return size; }
    //: Get length of arc.
    
  protected:
    Array1dC<Point2dC> edges;
    IntT size;
  };
  
  //! userlevel=Normal
  //: Circular Arc Detector.
  // !!EXPERIMENTAL!! <br>
  // This returns a set of circular arcs found in the edge segments passed to the apply method.
  
  class ArcDetectorC {
  public:
    ArcDetectorC(RealT actTolerance = 1.5,RealT minRadius = 4,RealT maxRadius = 300,RealT maxOverlap = 0.5);
    //: Default constructor.
    
    DListC<Arc2dSegmentC> Apply(DListC<DListC<Index2dC> > edges);
    //: Given a set of edge lists create a set of arc's.
    
    DListC<Array1dC<Point2dC> > List2PntArray(DListC<DListC<Index2dC> > edges,IntT minLength = 0);
    //: Convert index list to point arrays.
    
  protected:
    IntT FindArcs(const Array1dC<Point2dC> &pixels,DListC<Arc2dSegmentC> &arcs);
    //: Find all arcs in 'pixels'.
    
    IntT CheckArc(const Arc2dC &arc,const Array1dC<Point2dC> &pixels,int start);
    //: Check candate arc.
    
    IntT MaxArcs(DListC<Arc2dSegmentC> &arcs,DListC<Arc2dSegmentC> &list);
    //: Find longest set of arcs.
    
    RealT arcTolerance; // Tolarance for arc in pixels.
    RealT minRadius;    // Minimum radius.
    RealT maxRadius;    // Maximum radius.
    RealT maxOverlap;
  };
}



#endif
