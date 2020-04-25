// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_EDGE_HEADER
#define RAVL_EDGE_HEADER 1
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Euclidean/Boundary/Edge.hh"

#include "Ravl/Crack.hh"
#if !defined(VISUAL_CPP)
#warning "EdgeC is now obsolete.  Use CrackC instead."
#endif

namespace RavlN {
  
  //! userlevel=Obsolete
  //: Obsolete class, use CrackC instead.
  
  class EdgeC
    : public CrackC
  {
    EdgeC()
    {}
    //: Creates an crack
    // The value is undefined.
    
    EdgeC(const BVertexC & px,const CrackCodeC & cc)
      : CrackC(px,cc) 
    {}
    // Create the crack with origin in the boundary vertex 'px' and with
    // direction 'cc'.
    
    EdgeC(const BVertexC & begin, const BVertexC & end)
      : CrackC(begin,end)
    {}
    // Create the crack with origin in the boundary vertex 'begin' pointing 
    // towards the boundary vertex 'end'. The direction is automatically 
    // generated.
    
    EdgeC(const Index2dC &pxl, const CrackCodeC & cc)
      : CrackC(pxl,cc)
    {}
    // Creates the crack which starts at one corner of the pixel 'pxl'
    // and has the direction 'cc'. The corner of the pixel is chosen
    // in such way that the elementary crack is an elementary crack of the pixel.
    
  };

}
#endif
