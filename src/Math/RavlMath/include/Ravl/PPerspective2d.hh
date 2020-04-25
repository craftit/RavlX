// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_PPERSPECTIVE2D_HH
#define RAVL_PPERSPECTIVE2D_HH 1
//////////////////////////////////////////////////////////////////////////
//! file="Ravl/Math/Geometry/Projective/2D/PPerspective2d.hh"
//! lib=RavlMath
//! userlevel=Normal
//! author="Radek Marik"
//! docentry="Ravl.API.Math.Projective Geometry.2D"
//! date="26.06.1995"
//! rcsid="$Id$"

#include "Ravl/PProjection2d.hh"

namespace RavlN {
  //! userlevel=Normal
  //: Perspective transformation in 2D space
  // The class PPerspective2dC represents a perspective transformation
  // of objects in projective three-space. The perspective is
  // represented by a general projective transformation.
  
  class PPerspective2dC
    : public PProjection2dC
  {
  public:
    
    //:----------------------------------------
    // Constructors, assigment, and destructor.

    inline PPerspective2dC(const PPoint2dC & e, const PLine2dC & i)     
      : PProjection2dC(  i.P2()*e.P2() + i.P3()*e.P3(),
			 -i.P2()*e.P1(),
			 -i.P3()*e.P1(),
			 
			 -i.P1()*e.P2(),
			 i.P1()*e.P1() + i.P3()*e.P3(),
			 -i.P3()*e.P2(),
			 
			 -i.P1()*e.P3(),
			 -i.P2()*e.P3(),
			 i.P1()*e.P1() + i.P2()*e.P2())
    {}
    // Creates a perspective projection whose center is 'e' and objects are
    // projected onto the image plane 'i'.
    
  };    

  ostream & operator<<(ostream & outS, const PPerspective2dC & p);
  // Saves the projection 'p' into the output stream 'outS'.
  
  istream & operator>>(istream & inS, PPerspective2dC & p);
  // Sets the projective transformation elements according to the values
  // read from input stream 'inS'.
  
}

#endif
