// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlGUI3D

#include "Ravl/GUI/DSurfacePointVertexArray.hh"
#include "Ravl/SArray1dIter.hh"

namespace RavlGUIN
{
  void DSurfacePointVertexArray(const SurfacePoint3dArrayC<VertexC>& s)
  {
    glBegin(GL_POINTS);
    for (SArray1dIterC<VertexC> i(s); i; i++)
    {
      glNormal3dv(&(*i).Normal()[0]);
      glVertex3dv(&(*i).Position()[0]);
    }
    glEnd();
  }

  //: Get center of object.
  Vector3dC DSurfacePointVertexArrayBodyC::GUICenter() const
  {
    Vector3dC vec(0,0,0);
    for(SArray1dIterC<VertexC> it(surface);it;it++) {
      vec += it->Position();
    }
    vec /= (RealT)surface.Size();
    return vec;
  }

  //: Get extent of object.
  RealT DSurfacePointVertexArrayBodyC::GUIExtent() const
  {
    Vector3dC vec(0,0,0);
    for(SArray1dIterC<VertexC> it(surface);it;it++) {
      vec += it->Position();
    }
    vec /= (RealT)surface.Size();
    RealT dist = 0;
    for(SArray1dIterC<VertexC> it(surface);it;it++)
      dist = Max(dist,vec.EuclidDistance(it->Position()));
    return dist;
  }

}
