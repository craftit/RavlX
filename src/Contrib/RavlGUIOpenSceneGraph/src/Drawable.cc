// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/OpenSceneGraph/Drawable.cc"
//! lib=RavlGUIOpenSceneGraph
//! author = "Warren Moore"

#include "Ravl/OpenSceneGraph/Drawable.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include "Ravl/OpenSceneGraph/Geode.hh"
#include "Ravl/RLog.hh"
#include <osg/ShapeDrawable>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlOSGN
{

  using namespace osg;
  
  DrawableC::DrawableC()
  {
  }

  DrawableC::DrawableC(const XMLFactoryContextC &factory)
  {
    Setup(factory);
  }
  

  DrawableC::DrawableC(osg::Drawable *drawable)
    : m_drawable(drawable)
  {}
  
  //! Do setup from factory.
  bool DrawableC::Setup(const XMLFactoryContextC &factory)
  {
    RavlImageN::RealRGBAValueC colour(1.0,1.0,1.0,1.0);
    if(factory.Attribute("colour",colour)){
      SetColour(colour);
    }
    
    return true;
  }
  
  //: Called when owner handles drop to zero.
  void DrawableC::ZeroOwners()
  {
    RavlN::RCLayerBodyC::ZeroOwners();
  }

  bool DrawableC::SetColour(const RavlImageN::RealRGBAValueC &colour)
  {
    if (!m_drawable)
      return false;
    
    ShapeDrawable *shapeDrawableRef = dynamic_cast<ShapeDrawable*>(m_drawable.get());
    if(shapeDrawableRef == 0) {
      rWarning("Can't set colour on non shape drawable. ");
      return false;
    }

    shapeDrawableRef->setColor(Vec4(colour.Red(), colour.Green(), colour.Blue(), colour.Alpha()));

    return true;
  }


  DrawableC::~DrawableC()
  {
  }

  GeodeC::RefT ConvertDrawableToGeode(const DrawableC::RefT &drawable)
  {
    GeodeC::RefT ret = new GeodeC(*drawable);
    return ret;
  }
  
  DP_REGISTER_CONVERSION(ConvertDrawableToGeode,1.0);
  
  
}
