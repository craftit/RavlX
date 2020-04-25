// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/OpenSceneGraph/InfinitePlane.cc"
//! lib=RavlGUIOpenSceneGraph
//! author = "Warren Moore"

#include "Ravl/OpenSceneGraph/InfinitePlane.hh"
#include "Ravl/RLog.hh"
#include "Ravl/XMLFactoryRegister.hh"

#include <osg/Shape>
#include <osg/ShapeDrawable>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlOSGN
{
  using RavlN::RealT;
  using namespace osg;

  InfinitePlaneC::InfinitePlaneC()
  {
    m_InfinitePlane = new InfinitePlane();
    m_drawable = new ShapeDrawable(m_InfinitePlane);
  }
  
  //: XML Factory constructor
  
  
  InfinitePlaneC::InfinitePlaneC(const XMLFactoryContextC &factory)
   : DrawableC(factory)
  {
    
    m_InfinitePlane = new InfinitePlane();
    m_drawable = new ShapeDrawable(m_InfinitePlane);
    
    RavlImageN::RealRGBAValueC colour(1.0,1.0,1.0,1.0);
    factory.Attribute("colour",colour,RavlImageN::RealRGBAValueC(1.0,1.0,1.0,1.0));
    if(!SetColour(colour)) {
      rWarning("Failed to set InfinitePlane colour.");
      RavlAssert(0); // This shouldn't fail!
    }
    
  }


  InfinitePlaneC::~InfinitePlaneC()
  {
  }

  
  //: Called when owner handles drop to zero.
  void InfinitePlaneC::ZeroOwners()
  {
    DrawableC::ZeroOwners();  
  }
  
  void LinkInfinitePlane()
  {}

  static RavlN::XMLFactoryRegisterConvertC<InfinitePlaneC,DrawableC> g_registerXMLFactoryInfinitePlane("RavlOSGN::InfinitePlaneC");

}
