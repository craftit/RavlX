// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/OpenSceneGraph/Cone.cc"
//! lib=RavlGUIOpenSceneGraph
//! author = "Warren Moore"

#include "Ravl/OpenSceneGraph/Cone.hh"
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

  ConeC::ConeC(const RavlN::Vector3dC &position, RavlN::RealT radius,RavlN::RealT height)
  {
    m_Cone = new Cone(Vec3(position.X(), position.Y(), position.Z()), radius,height);
    m_drawable = new ShapeDrawable(m_Cone);
  }
  
  //: XML Factory constructor
  
  
  ConeC::ConeC(const XMLFactoryContextC &factory)
   : DrawableC(factory)
  {
    RavlN::Vector3dC position;
    factory.Attribute("position",position,RavlN::Vector3dC(0,0,0));
    RealT radius = factory.AttributeReal("radius",1.0);
    RealT height = factory.AttributeReal("height",1.0);
    
    m_Cone = new Cone(Vec3(position.X(), position.Y(), position.Z()), radius,height);
    m_drawable = new ShapeDrawable(m_Cone);
    
    RavlImageN::RealRGBAValueC colour(1.0,1.0,1.0,1.0);
    factory.Attribute("colour",colour,RavlImageN::RealRGBAValueC(1.0,1.0,1.0,1.0));
    if(!SetColour(colour)) {
      rWarning("Failed to set Cone colour.");
      RavlAssert(0); // This shouldn't fail!
    }
    
  }


  ConeC::~ConeC()
  {
  }

  bool ConeC::SetPosition(const RavlN::Vector3dC &position)
  {
    RavlAssert(m_Cone);
    m_Cone->setCenter(Vec3(position.X(), position.Y(), position.Z()));
    return true;
  }

  bool ConeC::SetRotation(const RavlN::Quatern3dC &angle) {
    RavlAssert(m_Cone);
    m_Cone->setRotation(osg::Quat(angle[1],angle[2],angle[3],angle[0]));
    return true;
  }

  bool ConeC::SetSize(RavlN::RealT radius)
  {
    RavlAssert(m_Cone);
    m_Cone->setRadius(radius);
    return true;
  }
  
  //: Called when owner handles drop to zero.
  void ConeC::ZeroOwners()
  {
    DrawableC::ZeroOwners();  
  }
  
  void LinkCone()
  {}

  static RavlN::XMLFactoryRegisterConvertC<ConeC,DrawableC> g_registerXMLFactoryCone("RavlOSGN::ConeC");

}
