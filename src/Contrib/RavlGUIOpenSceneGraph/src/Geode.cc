// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/OpenSceneGraph/Geode.cc"
//! lib=RavlGUIOpenSceneGraph
//! author = "Warren Moore"

#include "Ravl/OpenSceneGraph/Geode.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include <osg/Geode>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlOSGN
{

  using namespace osg;

  GeodeC::GeodeC()
  {
    m_node = new osg::Geode();
  }
  
  //: XML Factory constructor
  GeodeC::GeodeC(const XMLFactoryContextC &factory)
   : NodeC(factory)
  {
    m_node = new osg::Geode();
    // Short hand for a single child node.
    DrawableC::RefT drawable;
    if(factory.UseComponent("Draw",drawable)) {
      AddDrawable(drawable);
    }
    
    // Full list of drawables.
    factory.UseComponentGroup("Drawables",*this,&GeodeC::AddDrawable);
  }

  GeodeC::GeodeC(const DrawableC &drawable)
  {
    m_node = new osg::Geode();
    AddDrawable(DrawableC::RefT(&drawable));
  }

  GeodeC::~GeodeC()
  {
    
  }

  //: Called when owner handles drop to zero.
  
  void GeodeC::ZeroOwners()
  {
    NodeC::ZeroOwners();
  }

  bool GeodeC::AddDrawable(const DrawableC::RefT &drawable)
  {
    if (!m_node || !drawable.IsValid())
      return false;

    DrawableC::RefT drawableRef = drawable;
    ref_ptr<Geode> geodeRef = m_node->asGeode();
    if (!geodeRef)
      return false;

    geodeRef->addDrawable(drawableRef->Drawable());

    return true;
  }
  
  static RavlN::XMLFactoryRegisterConvertC<GeodeC,NodeC> g_registerXMLFactoryGeode("RavlOSGN::GeodeC");
}
