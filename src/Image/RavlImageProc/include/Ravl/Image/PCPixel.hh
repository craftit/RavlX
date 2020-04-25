// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_PCPIXEL_HEADER
#define RAVL_PCPIXEL_HEADER 1
/////////////////////////////////////////////////
//! docentry="Ravl.API.Images.Lines"
//! file="Ravl/Image/Processing/Lines/PCPixel.hh"
//! lib=RavlImageProc
//! author="Charles Galambos"
//! rcsid="$Id$"
//! date="07/01/1999"

#include "Ravl/Index2d.hh"
#include "Ravl/Point2d.hh"

namespace RavlImageN {
  using namespace RavlN;
#if RAVL_VISUALCPP_NAMESPACE_BUG
  using RavlN::Point2dC;
#endif
  
  //! userlevel=Normal
  //: Pixel near parametric curve.
  
  class PCIndex2dC 
    : public Index2dC 
  {
  public:
    PCIndex2dC()
    {}
    //: Default constructor.
  
    PCIndex2dC(const Index2dC &px,const Point2dC &apnt,RealT ap = 0)
      : Index2dC(px),
	pnt(apnt),
	p(ap)
    {}
    //: Constructor.
    
    PCIndex2dC(const Index2dC &px,RealT ap = 0)
      : Index2dC(px),
	pnt(px),
	p(ap)
    {}
    //: Constructor.
    
    RealT &P() { return p; }
    //: Access P.
    
    RealT P() const { return p; }
    //: Access P.
    
    Point2dC &ClosestPnt() { return pnt; }
    //: Access closest point.
    
    const Point2dC &ClosestPnt() const { return pnt; }
    //: Access closest point.
    
    ostream &WriteGF(ostream &out) const;
    //: Write out to stream.
    
  private:
    Point2dC pnt; // Closest point on curve.
    RealT p;      // Parameter of closest point.
  };
}

#endif
