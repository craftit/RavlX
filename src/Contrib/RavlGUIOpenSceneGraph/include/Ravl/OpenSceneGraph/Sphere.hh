// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/OpenSceneGraph/Sphere.hh"
//! lib=RavlGUIOpenSceneGraph
//! author = "Warren Moore"
//! docentry = "Ravl.API.Graphics.OpenSceneGraph"
//! example = "exOpenSceneGraphWidget.cc"

#ifndef RAVLGUI_OPENSCENEGRAPHSPHERE_HEADER
#define RAVLGUI_OPENSCENEGRAPHSPHERE_HEADER 1

#include "Ravl/OpenSceneGraph/Drawable.hh"
#include "Ravl/Vector3d.hh"
#include <osg/Shape>

namespace RavlOSGN
{

  //! userlevel=Normal
  //: Sphere object.

  class SphereC
  : public DrawableC
  {
  public:
    SphereC(const RavlN::Vector3dC &position = RavlN::Vector3dC(0, 0, 0),
            RavlN::RealT radius = 1.0);
    //: Ctor.
    
    SphereC(const XMLFactoryContextC &factory);
    //: XML Factory constructor

    virtual ~SphereC();
    //: Dtor.

    bool SetPosition(const RavlN::Vector3dC &position);
    //: Set the sphere centre position.

    bool SetSize(RavlN::RealT radius);
    //: Set the sphere radius.

    typedef RavlN::SmartOwnerPtrC<SphereC> RefT;
    typedef RavlN::SmartCallbackPtrC<SphereC> CBRefT;
    
  protected:
    virtual void ZeroOwners();
    //: Called when owner handles drop to zero.
    
    osg::ref_ptr<osg::Sphere> m_sphere;
  };

}

#endif
