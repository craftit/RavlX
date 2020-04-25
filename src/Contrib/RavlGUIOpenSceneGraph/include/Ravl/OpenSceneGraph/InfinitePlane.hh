// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/OpenSceneGraph/InfinitePlane.hh"
//! lib=RavlGUIOpenSceneGraph
//! author = "Warren Moore"
//! docentry = "Ravl.API.Graphics.OpenSceneGraph"
//! example = "exOpenSceneGraphWidget.cc"

#ifndef RAVLGUI_OPENSCENEGRAPHInfinitePlane_HEADER
#define RAVLGUI_OPENSCENEGRAPHInfinitePlane_HEADER 1

#include "Ravl/OpenSceneGraph/Drawable.hh"
#include "Ravl/Vector3d.hh"
#include <osg/Shape>

namespace RavlOSGN
{

  //! userlevel=Normal
  //: InfinitePlane object.

  class InfinitePlaneC
  : public DrawableC
  {
  public:
    InfinitePlaneC();
    //: Ctor.
    
    InfinitePlaneC(const XMLFactoryContextC &factory);
    //: XML Factory constructor

    virtual ~InfinitePlaneC();
    //: Dtor.


    typedef RavlN::SmartOwnerPtrC<InfinitePlaneC> RefT;
    typedef RavlN::SmartCallbackPtrC<InfinitePlaneC> CBRefT;
    
  protected:
    virtual void ZeroOwners();
    //: Called when owner handles drop to zero.
    
    osg::ref_ptr<osg::InfinitePlane> m_InfinitePlane;
  };

}

#endif
