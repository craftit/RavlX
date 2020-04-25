// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2012, OmniPerception Ltd and Charles Galambos.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/OpenSceneGraph/Capsule.hh"
//! lib=RavlGUIOpenSceneGraph
//! author = "Waren Moore and Charles Galambos"
//! docentry = "Ravl.API.Graphics.OpenSceneGraph"
//! example = "exOpenSceneGraphWidget.cc"

#ifndef RAVLGUI_OPENSCENEGRAPHCapsule_HEADER
#define RAVLGUI_OPENSCENEGRAPHCapsule_HEADER 1

#include "Ravl/OpenSceneGraph/Drawable.hh"
#include "Ravl/Vector3d.hh"
#include "Ravl/Quatern3d.hh"
#include <osg/Shape>

namespace RavlOSGN
{

  //! userlevel=Normal
  //: Capsule object.

  class CapsuleC
  : public DrawableC
  {
  public:
    CapsuleC(const RavlN::Vector3dC &position = RavlN::Vector3dC(0, 0, 0),
               RavlN::RealT radius = 1.0,
               RavlN::RealT height = 1.0);
    //: Ctor.
    
    CapsuleC(const XMLFactoryContextC &factory);
    //: XML Factory constructor

    virtual ~CapsuleC();
    //: Dtor.

    bool SetPosition(const RavlN::Vector3dC &position);
    //: Set the Capsule centre position.

    bool SetSize(RavlN::RealT radius);
    //: Set the Capsule radius.

    bool SetRotation(const RavlN::Quatern3dC &angle);
    //: Set the Box rotation

    typedef RavlN::SmartOwnerPtrC<CapsuleC> RefT;
    typedef RavlN::SmartCallbackPtrC<CapsuleC> CBRefT;
    
  protected:
    virtual void ZeroOwners();
    //: Called when owner handles drop to zero.
    
    osg::ref_ptr<osg::Capsule> m_Capsule;
  };

}

#endif
