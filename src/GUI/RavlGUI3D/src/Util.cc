// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI3D
//! file="Ravl/GUI/3D/Util.cc"

#include "Ravl/GUI/Util.hh"
#include "Ravl/GUI/Canvas3D.hh"

namespace RavlGUIN {

  //: Render object.
  bool DTransform3DBodyC::GUIRender(Canvas3DC &c3d) const
  {
    //glPushMatrix();

    glMatrixMode(GL_MODELVIEW);
    //glMatrixMode(GL_PROJECTION);
    if(mode & DTransform3D_RESET)
      glLoadIdentity();

    if(mode & DTransform3D_ROTATION)
      glRotated(angle, axis.X(), axis.Y(), axis.Z());

    if(mode & DTransform3D_TRANSLATION)
      glTranslated(trans.X(), trans.Y(), trans.Z());

    for(DLIterC<DObject3DC> it(parts); it.IsElm(); it.Next())
      it.Data().GUIRenderDL(c3d);

    //glPopMatrix();
    return true;
  }
}
