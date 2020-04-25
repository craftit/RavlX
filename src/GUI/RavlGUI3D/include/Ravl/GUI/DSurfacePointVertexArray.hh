// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlGUI3D
//! docentry="Ravl.API.Graphics.3D"

#ifndef _RAVL_DSURFACE_POINT_VERTEX_ARRAY_HH_
#define _RAVL_DSURFACE_POINT_VERTEX_ARRAY_HH_ 1

#include "Ravl/GUI/DObject3D.hh"
#include "Ravl/3D/SurfacePoint3dArray.hh"
#include "Ravl/3D/Vertex.hh"

namespace RavlGUIN
{
  using namespace Ravl3DN;

  //! userlevel=Normal
  //: Direct OpenGL render of vertex array, using current colour.
  void DSurfacePointVertexArray(const SurfacePoint3dArrayC<VertexC>& s);

  //! userlevel=Develop
  //: Rendering class for vertex array.
  //  Does not set the colour, so will use current OpenGL colour
  class DSurfacePointVertexArrayBodyC : public DObject3DBodyC
  {
  public:
    DSurfacePointVertexArrayBodyC(const SurfacePoint3dArrayC<VertexC>& s)
      : surface(s)
      {}
    //: Constructor.

    virtual bool GUIRender(Canvas3DC &c3d) const
      {
        DSurfacePointVertexArray(surface);
        return true;
      }
    //: Render object.

    virtual Vector3dC GUICenter() const;
    //: Get center of object.

    virtual RealT GUIExtent() const;
    //: Get extent of object.

  protected:
    SurfacePoint3dArrayC<VertexC> surface;
  };

  //! userlevel=Normal
  //: Rendering class for vertex array, using current OpenGL colour
  //  Does not set the colour, so will use current OpenGL colour

  class DSurfacePointVertexArrayC : public DObject3DC
  {
  public:
    DSurfacePointVertexArrayC()
      {}
    //: Default constructor, creates an invalid drawing object.

    DSurfacePointVertexArrayC(const SurfacePoint3dArrayC<VertexC>& s)
      : DObject3DC(*new DSurfacePointVertexArrayBodyC(s))
      {}
    //: Construct drawing object from array of surface points.
  };
}


#endif
