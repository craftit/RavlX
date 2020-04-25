// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLDCUBE3D_HEADER
#define RAVLDCUBE3D_HEADER 1
/////////////////////////////////////////////////
//! docentry="Ravl.API.Graphics.3D"
//! rcsid="$Id$"
//! file="Ravl/GUI/3D/DCube3D.hh"
//! lib=RavlGUI3D
//! author="Charles Galambos"
//! date="12/04/99"

#include "Ravl/GUI/DObject3D.hh"
#include "Ravl/Image/RealRGBValue.hh"

namespace RavlGUIN {

  //! userlevel=Develop
  //: Body of a  object set in a 3D world.
  class DCube3DBodyC : public DObject3DBodyC
  {
  public:
    DCube3DBodyC(const Vector3dC &nDiag, const RealRGBValueC &col)
      : diag(nDiag),
      colour(col)
      {}
    //: Constructor.

    virtual bool GUIRender(Canvas3DC &c3d) const;
    //: Render object.

     virtual Vector3dC GUICenter() const
      { return Vector3dC(0, 0, 0); }
    //: Get center of object.
    // defaults to 0,0,0

    virtual RealT GUIExtent() const
      { return 1; }
    //: Get extent of object.
    // defaults to 1

  protected:
    Vector3dC diag;
    RealRGBValueC colour;
  };

  //! userlevel=Normal
  //: Basic object in a 3d world.
  class DCube3DC : public DObject3DC
  {
  public:
    DCube3DC(const Vector3dC &nDiag = Vector3dC(1, 1, 1),
             const RealRGBValueC &col = RealRGBValueC(1, 0, 0))
      : DObject3DC(*new DCube3DBodyC(nDiag, col))
      {}
    //: Constructor.
  };

}

#endif
