// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/OpenSceneGraph/Sphere.cc"
//! lib=RavlGUIOpenSceneGraph
//! author = "Warren Moore"

#include "Ravl/OpenSceneGraph/Sphere.hh"
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

  SphereC::SphereC(const RavlN::Vector3dC &position, RavlN::RealT radius)
  {
    m_sphere = new Sphere(Vec3(position.X(), position.Y(), position.Z()), radius);
    m_drawable = new ShapeDrawable(m_sphere);
  }
  
  //: XML Factory constructor
  
  
  SphereC::SphereC(const XMLFactoryContextC &factory)
   : DrawableC(factory)
  {
    RavlN::Vector3dC position;
    factory.Attribute("position",position,RavlN::Vector3dC(0,0,0));
    RealT radius = factory.AttributeReal("radius",1.0);
    
    m_sphere = new Sphere(Vec3(position.X(), position.Y(), position.Z()), radius);
    m_drawable = new ShapeDrawable(m_sphere);     
    
    RavlImageN::RealRGBAValueC colour(1.0,1.0,1.0,1.0);
    factory.Attribute("colour",colour,RavlImageN::RealRGBAValueC(1.0,1.0,1.0,1.0));
    if(!SetColour(colour)) {
      rWarning("Failed to set sphere colour.");
      RavlAssert(0); // This shouldn't fail!
    }
    
  }


  SphereC::~SphereC()
  {
  }

  bool SphereC::SetPosition(const RavlN::Vector3dC &position)
  {
    if (!m_sphere)
      return false;

    m_sphere->setCenter(Vec3(position.X(), position.Y(), position.Z()));

    return true;
  }

  bool SphereC::SetSize(RavlN::RealT radius)
  {
    if (!m_sphere)
      return false;

    m_sphere->setRadius(radius);

    return true;
  }
  
  //: Called when owner handles drop to zero.
  void SphereC::ZeroOwners()
  {
    DrawableC::ZeroOwners();  
  }
  
  void LinkSphere()
  {}

  static RavlN::XMLFactoryRegisterConvertC<SphereC,DrawableC> g_registerXMLFactorySphere("RavlOSGN::SphereC");

}
