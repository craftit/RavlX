// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/OpenSceneGraph/Capsule.cc"
//! lib=RavlGUIOpenSceneGraph
//! author = "Warren Moore"

#include "Ravl/OpenSceneGraph/Capsule.hh"
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

  CapsuleC::CapsuleC(const RavlN::Vector3dC &position, RavlN::RealT radius,RavlN::RealT height)
  {
    m_Capsule = new Capsule(Vec3(position.X(), position.Y(), position.Z()), radius,height);
    m_drawable = new ShapeDrawable(m_Capsule);
  }
  
  //: XML Factory constructor
  
  
  CapsuleC::CapsuleC(const XMLFactoryContextC &factory)
   : DrawableC(factory)
  {
    RavlN::Vector3dC position;
    factory.Attribute("position",position,RavlN::Vector3dC(0,0,0));
    RealT radius = factory.AttributeReal("radius",1.0);
    RealT height = factory.AttributeReal("height",1.0);
    
    m_Capsule = new Capsule(Vec3(position.X(), position.Y(), position.Z()), radius,height);
    m_drawable = new ShapeDrawable(m_Capsule);
    
    RavlImageN::RealRGBAValueC colour(1.0,1.0,1.0,1.0);
    factory.Attribute("colour",colour,RavlImageN::RealRGBAValueC(1.0,1.0,1.0,1.0));
    if(!SetColour(colour)) {
      rWarning("Failed to set Capsule colour.");
      RavlAssert(0); // This shouldn't fail!
    }
    
  }


  CapsuleC::~CapsuleC()
  {
  }

  bool CapsuleC::SetPosition(const RavlN::Vector3dC &position)
  {
    RavlAssert(m_Capsule);
    m_Capsule->setCenter(Vec3(position.X(), position.Y(), position.Z()));
    return true;
  }

  bool CapsuleC::SetRotation(const RavlN::Quatern3dC &angle) {
    RavlAssert(m_Capsule);
    m_Capsule->setRotation(osg::Quat(angle[1],angle[2],angle[3],angle[0]));
    return true;
  }

  bool CapsuleC::SetSize(RavlN::RealT radius)
  {
    RavlAssert(m_Capsule);
    m_Capsule->setRadius(radius);
    return true;
  }
  
  //: Called when owner handles drop to zero.
  void CapsuleC::ZeroOwners()
  {
    DrawableC::ZeroOwners();  
  }
  
  void LinkCapsule()
  {}

  static RavlN::XMLFactoryRegisterConvertC<CapsuleC,DrawableC> g_registerXMLFactoryCapsule("RavlOSGN::CapsuleC");

}
