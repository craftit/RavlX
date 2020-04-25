// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/OpenSceneGraph/Drawable.hh"
//! lib=RavlGUIOpenSceneGraph
//! author = "Warren Moore"
//! docentry = "Ravl.API.Graphics.OpenSceneGraph"
//! example = "exOpenSceneGraphWidget.cc"

#ifndef RAVLGUI_OPENSCENEGRAPHDRAWABLE_HEADER
#define RAVLGUI_OPENSCENEGRAPHDRAWABLE_HEADER 1

#include "Ravl/SmartLayerPtr.hh"
#include "Ravl/XMLFactory.hh"
#include "Ravl/Image/RealRGBAValue.hh"
#include <osg/ref_ptr>
#include <osg/Drawable>

namespace RavlOSGN
{
  using RavlN::XMLFactoryContextC;

  //! userlevel=Normal
  //: Drawable object base class.

  class DrawableC
  : public RavlN::RCLayerBodyC
  {
  public:
    DrawableC(osg::Drawable *drawable);
    //: Ctor.

    DrawableC();
    //: Ctor.

    DrawableC(const XMLFactoryContextC &factory);
    //: XML Factory constructor

    virtual ~DrawableC();
    //: Dtor.
    
    virtual bool SetColour(const RavlImageN::RealRGBAValueC &colour);
    //: Set the colour of shape drawables.
    
    osg::ref_ptr<osg::Drawable> Drawable()
    { return m_drawable; }
    //: Access the object smart pointer.
    
    typedef RavlN::SmartOwnerPtrC<DrawableC> RefT;
    typedef RavlN::SmartCallbackPtrC<DrawableC> CBRefT;

  protected:
    virtual void ZeroOwners();
    //: Called when owner handles drop to zero.

    bool Setup(const XMLFactoryContextC &factory);
    //: Do setup from factory.
    
    osg::ref_ptr<osg::Drawable> m_drawable;
  };

}

#endif
