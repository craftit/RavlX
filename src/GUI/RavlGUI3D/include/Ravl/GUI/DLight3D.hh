// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_DLIGHT3D_HEADER
#define RAVLGUI_DLIGHT3D_HEADER 1
///////////////////////////////////////////////////
//! docentry="Ravl.API.Graphics.3D"
//! rcsid="$Id$"
//! file="Ravl/GUI/3D/DLight3D.hh"
//! lib=RavlGUI3D
//! author="Charles Galambos"
//! date="18/06/1999"

#include "Ravl/GUI/DObject3D.hh"

namespace RavlGUIN {

  //////////////////////////////////////////////
  //! userlevel=Develop
  //: Draw some lines.

  class DLight3DBodyC : public DObject3DBodyC
  {
  public:
    DLight3DBodyC(const RealRGBValueC &nCol, const Vector3dC &nPos, int nn)
      : n(nn),
	colour(nCol),
	pos(nPos),
	spot(false)
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
    GLenum LightNo(IntT no) const;
    //: Convert Light number.

    IntT n;           // Number of light. 0-7
    RealRGBValueC colour; // Colour of light.
    Vector3dC pos;    // Position of light.
    bool spot;        // Spot light ?
    Vector3dC dir;    // Direction of spot light.
    RealT  ang;       // angle of light.
  };

  //! userlevel=Normal
  //: Draw some lines.
  class DLight3DC : public DObject3DC
  {
  public:
    DLight3DC(const RealRGBValueC &col, const Vector3dC &nPos, int nn = 0)
      : DObject3DC(*new DLight3DBodyC(col, nPos, nn))
    {}
    //: Constructor.
  };
}


#endif
