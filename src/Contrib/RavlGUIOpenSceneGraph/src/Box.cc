// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/OpenSceneGraph/Box.cc"
//! lib=RavlGUIOpenSceneGraph
//! author = "Warren Moore"

#include "Ravl/OpenSceneGraph/Box.hh"
#include "Ravl/OpenSceneGraph/TypeConvert.hh"
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

  using namespace osg;

  BoxC::BoxC(const RavlN::Vector3dC &position, const RavlN::Vector3dC &size)
  {
    BuildNode(position,size);
  }

  //: XML Factory constructor
  
  BoxC::BoxC(const XMLFactoryContextC &factory)
  {
    Setup(factory);
  }

  BoxC::~BoxC()
  {
  }
  
  //: Create the node.
  
  bool BoxC::BuildNode(const RavlN::Vector3dC &position,
                       const RavlN::Vector3dC &size)
  {    
    m_box = new osg::Box(MakeVec3(position),size.X(), size.Y(), size.Z());
    m_drawable = new ShapeDrawable(m_box);
    return true;
  }

  bool BoxC::SetRotation(const RavlN::Quatern3dC &angle) {
    RavlAssert(m_box);
    m_box->setRotation(osg::Quat(angle[1],angle[2],angle[3],angle[0]));
    return true;
  }


  bool BoxC::SetPosition(const RavlN::Vector3dC &position)
  {
    RavlAssert(m_box);
    m_box->setCenter(Vec3(position.X(), position.Y(), position.Z()));
    return true;
  }

  bool BoxC::SetSize(const RavlN::Vector3dC &size)
  {
    RavlAssert(m_box);
    m_box->setHalfLengths(Vec3(size.X(), size.Y(), size.Z()));
    return true;
  }

  //: Setup box.
  bool BoxC::Setup(const XMLFactoryContextC &factory)
  {
    RavlN::Vector3dC position(0,0,0);
    RavlN::Vector3dC size(1,1,1);
    factory.Attribute("position",position);
    factory.Attribute("size",size);
    BuildNode(position,size);

    DrawableC::Setup(factory); 
    
    return true;
  }
  
  //: Called when owner handles drop to zero.
  void BoxC::ZeroOwners()
  {
    DrawableC::ZeroOwners();
  }
  
  void LinkBox()
  {}

  static RavlN::XMLFactoryRegisterConvertC<BoxC,DrawableC> g_registerXMLFactoryBox("RavlOSGN::BoxC");


}
