// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI3D
//! file="Ravl/GUI/3D/DCube3D.cc"

#include "Ravl/GUI/DCube3D.hh"
#include <GL/gl.h>

namespace RavlGUIN {

  //: Render object.
  bool DCube3DBodyC::GUIRender(Canvas3DC &c3d) const
  {
    //cerr << "DCube3DBodyC::Render(), Called. \n";

    GLColour(colour);
    Vector3dC p1 = diag/2;
    Vector3dC p7 = -diag/2;

    Vector3dC
      p2 ( p1.X(), p7.Y(), p1.Z() ),
      p3 ( p7.X(), p7.Y(), p1.Z() ),
      p4 ( p7.X(), p1.Y(), p1.Z() ),

      p5 ( p7.X(), p1.Y(), p7.Z() ),
      p6 ( p1.X(), p1.Y(), p7.Z() ),
      p8 ( p1.X(), p7.Y(), p7.Z() ) ;


    glBegin(GL_QUADS);
    {
      // front
      glNormal3d(0,0,1);
      GLVertex(p1);
      GLVertex(p4);
      GLVertex(p3);
      GLVertex(p2);

      // top face
      glNormal3d(0,1,0) ;
      GLVertex(p1);
      GLVertex(p6);
      GLVertex(p5);
      GLVertex(p4);

       // back
      glNormal3d(0,0,-1) ;
      GLVertex(p8);
      GLVertex(p7);
      GLVertex(p5);
      GLVertex(p6);

      // bottom
      glNormal3d(0,-1,0) ;
      GLVertex(p2);
      GLVertex(p3);
      GLVertex(p7);
      GLVertex(p8);

      // left
      glNormal3d(-1,0,0) ;
      GLVertex(p3);
      GLVertex(p4);
      GLVertex(p5);
      GLVertex(p7);

      // right
      glNormal3d(1,0,0) ;
      GLVertex(p1);
      GLVertex(p2);
      GLVertex(p8);
      GLVertex(p6);
    }
    glEnd();
    return true;
  }
}
