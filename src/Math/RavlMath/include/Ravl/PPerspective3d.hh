// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_PPERSPECTIVE3D_HH
#define RAVL_PPERSPECTIVE3D_HH 1
//////////////////////////////////////////////////////////////////////////
//! file="Ravl/Math/Geometry/Projective/3D/PPerspective3d.hh"
//! lib=RavlMath
//! userlevel=Normal
//! author="Radek Marik"
//! docentry="Ravl.API.Math.Projective Geometry.3D"
//! date="26.06.1995"
//! rcsid="$Id$"

#include "Ravl/PProjection3d.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: Perspective transformation in 3D space
  // The class PPerspectivity3dC represents a perspective transformation
  // of objects in projective three-space. The perspective is
  // represented by a general projective transformation.

  class PPerspective3dC
    : public PProjection3dC
  {
  public:
    
    //:----------------------------------------
    // Constructors, assigment, and destructor.

    inline PPerspective3dC(const PPoint3dC & e, const PPlane3dC & i);
    // Creates a perspective projection whose center is 'e' and objects are
    // projected onto the image plane 'i'.
    
    inline PPerspective3dC(const Matrix4dC &mat)
      : PProjection3dC(mat)
    {}
  };
  
  ostream & operator<<(ostream & outS, const PPerspective3dC & p);
  // Saves the projection 'p' into the output stream 'outS'.

  istream & operator>>(istream & inS, PPerspective3dC & p);
  // Sets the projective transformation elements according to the values
  // read from input stream 'inS'.
}

#endif
