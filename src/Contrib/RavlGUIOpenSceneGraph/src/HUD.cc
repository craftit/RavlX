// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/OpenSceneGraph/HUD.cc"
//! lib=RavlGUIOpenSceneGraph
//! author = "Warren Moore"

#include "Ravl/OpenSceneGraph/HUD.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include "Ravl/RLog.hh"
#include <osg/Projection>
#include <osg/Matrix>
#include <osg/MatrixTransform>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlOSGN
{

  using namespace osg;

  HUDC::HUDC(const RavlN::RealRange2dC &coords)
  : GroupC(false)
  {
    BuildNode(coords);
  }

  HUDC::~HUDC()
  {
  }

  //: Factory constructor.
  HUDC::HUDC(const XMLFactoryContextC &factory)
   : GroupC(false)
  {
    RavlN::RealRange2dC coords;
    factory.Attribute("coords",coords,RavlN::RealRange2dC(0, 1, 0, 1));
    BuildNode(coords);
    GroupC::Setup(factory);
  }
  
  //: Build node.
  bool HUDC::BuildNode(const RavlN::RealRange2dC &coords)
  {
    rDebug("BuildNode Coords:%s ",RavlN::StringOf(coords).data());
    ref_ptr<Projection> projectionMatrix = new Projection;
    projectionMatrix->setMatrix(Matrix::ortho2D(coords.LCol(), coords.RCol(), coords.TRow(), coords.BRow()));

    ref_ptr<MatrixTransform> modelViewMatrix = new osg::MatrixTransform;
    modelViewMatrix->setMatrix(Matrix::identity());
    modelViewMatrix->setReferenceFrame(Transform::ABSOLUTE_RF);
    projectionMatrix->addChild(modelViewMatrix);

    m_node = projectionMatrix;
    m_modelViewMatrix = modelViewMatrix;
    return true;
  }


  bool HUDC::AddChildNode(const NodeC &node)
  {
    if (!m_modelViewMatrix)
      return false;

    m_modelViewMatrix->addChild(const_cast<NodeC &>(node).Node());
    
    return true;
  }

  bool HUDC::RemoveChildNode(const NodeC &node)
  {
    if (!m_modelViewMatrix)
      return false;
    
    m_modelViewMatrix->removeChild(const_cast<NodeC &>(node).Node());
    
    return true;
  }

  //: Zero owners reached.
  void HUDC::ZeroOwners()
  {
    GroupC::ZeroOwners();
  }

  void LinkHUD()
  {}
  
  static RavlN::XMLFactoryRegisterConvertC<HUDC,GroupC> g_registerXMLFactoryGroup("RavlOSGN::HUDC");

}
