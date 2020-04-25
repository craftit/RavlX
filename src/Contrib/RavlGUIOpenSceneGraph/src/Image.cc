// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/OpenSceneGraph/Image.cc"
//! lib=RavlGUIOpenSceneGraph
//! author = "Warren Moore"

#include "Ravl/OpenSceneGraph/Image.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include "Ravl/RLog.hh"
#include <osg/PrimitiveSet>
#include <osg/Geode>
#include <osg/Texture2D>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlOSGN
{

  using namespace osg;
  
  //: Factory constructor.
  ImageC::ImageC(const XMLFactoryContextC &factory)
   : m_alphaImageEnabled(factory.AttributeBool("enableAlpha",false)),
     m_alpha(factory.AttributeReal("alpha",1.0))
  {
    RavlN::RealRange2dC coords;
    factory.Attribute("coords",coords,RavlN::RealRange2dC(-1,1));
    BuildNode(coords);    
  }
  
  ImageC::ImageC(const RavlN::RealRange2dC &coords)
  : m_alphaImageEnabled(false),
    m_alpha(1.0)
  {
    BuildNode(coords);
  }

  ImageC::~ImageC()
  {
  }
  
  //: Build the node
  bool ImageC::BuildNode(const RavlN::RealRange2dC &coords)
  {
    ONDEBUG(rDebug("Building image node. Range=%s ",RavlN::StringOf(coords).data()));
    ref_ptr<Geometry> geometryRef = new Geometry();

    ref_ptr<Vec3Array> vertexArray = new Vec3Array();
    vertexArray->push_back(Vec3(coords.LCol(), coords.BRow(), 0));
    vertexArray->push_back(Vec3(coords.RCol(), coords.BRow(), 0));
    vertexArray->push_back(Vec3(coords.RCol(), coords.TRow(), 0));
    vertexArray->push_back(Vec3(coords.LCol(), coords.TRow(), 0));
    geometryRef->setVertexArray(vertexArray);

    ref_ptr<DrawElementsUInt> imageQuad = new DrawElementsUInt(PrimitiveSet::QUADS, 0);
    imageQuad->push_back(0);
    imageQuad->push_back(1);
    imageQuad->push_back(2);
    imageQuad->push_back(3);
    geometryRef->addPrimitiveSet(imageQuad);

    ref_ptr<Vec4Array> colourArray = new Vec4Array();
    colourArray->push_back(Vec4(1, 1, 1, m_alpha));
    geometryRef->setColorArray(colourArray);

    ref_ptr<TemplateIndexArray<unsigned int, Array::UIntArrayType, 4, 4> > colourMap = new TemplateIndexArray<unsigned int, Array::UIntArrayType, 4, 4>;
    colourMap->push_back(0);
    geometryRef->setColorIndices(colourMap);
    geometryRef->setColorBinding(Geometry::BIND_PER_PRIMITIVE_SET);

    ref_ptr<Vec3Array> normalArray = new Vec3Array();
    normalArray->push_back(Vec3(0, 0, 1));
    geometryRef->setNormalArray(normalArray);
    geometryRef->setNormalBinding(Geometry::BIND_PER_PRIMITIVE_SET);

    ref_ptr<Vec2Array> textureCoords = new Vec2Array(4);
    (*textureCoords)[0].set(0, 1);
    (*textureCoords)[1].set(1, 1);
    (*textureCoords)[2].set(1, 0);
    (*textureCoords)[3].set(0, 0);
    geometryRef->setTexCoordArray(0, textureCoords);

    ref_ptr<Geode> geodeRef = new Geode();
    geodeRef->addDrawable(geometryRef);

    m_node = geodeRef;
    m_geometry = geometryRef;
    return true;
  }


  bool ImageC::SetSize(const RavlN::RealRange2dC &coords)
  {
    if (!m_geometry)
      return false;

    ref_ptr<Vec3Array> vertexArray = new Vec3Array();
    vertexArray->push_back(Vec3(coords.LCol(), coords.BRow(), 0));
    vertexArray->push_back(Vec3(coords.RCol(), coords.BRow(), 0));
    vertexArray->push_back(Vec3(coords.RCol(), coords.TRow(), 0));
    vertexArray->push_back(Vec3(coords.LCol(), coords.TRow(), 0));
    m_geometry->setVertexArray(vertexArray);

    return true;
  }

  bool ImageC::AlphaImageEnable(bool alphaImageEnable)
  {
    if (!m_node)
      return false;

    m_alphaImageEnabled = alphaImageEnable;

    ref_ptr<Geode> geodeRef = dynamic_cast<Geode*>(m_node.get());
    if (!geodeRef)
      return false;

    ref_ptr<StateSet> stateSetRef = geodeRef->getOrCreateStateSet();

    if (!UpdateAlphaRenderState(stateSetRef))
      return false;

    geodeRef->setStateSet(stateSetRef);

    return true;
  }

  bool ImageC::SetAlpha(float alpha)
  {
    if (!m_node || !m_geometry)
      return false;

    m_alpha = alpha;

    ref_ptr<Vec4Array> colourArray = new Vec4Array();
    colourArray->push_back(Vec4(1, 1, 1, m_alpha));
    m_geometry->setColorArray(colourArray);
    
    ref_ptr<TemplateIndexArray<unsigned int, Array::UIntArrayType, 4, 4> > colourMap = new TemplateIndexArray<unsigned int, Array::UIntArrayType, 4, 4>;
    colourMap->push_back(0);
    m_geometry->setColorIndices(colourMap);
    m_geometry->setColorBinding(Geometry::BIND_PER_PRIMITIVE_SET);

    ref_ptr<Geode> geodeRef = dynamic_cast<Geode*>(m_node.get());
    if (!geodeRef)
      return false;

    ref_ptr<StateSet> stateSetRef = geodeRef->getOrCreateStateSet();

    if (!UpdateAlphaRenderState(stateSetRef))
      return false;

    geodeRef->setStateSet(stateSetRef);

    return true;
  }

  bool ImageC::UpdateAlphaRenderState(ref_ptr<StateSet> &stateSetRef)
  {
    RavlAssert(stateSetRef);

    if (m_alpha < 1.0 || m_alphaImageEnabled)
    {
      stateSetRef->setMode(GL_BLEND, StateAttribute::ON);
      stateSetRef->setRenderingHint(StateSet::TRANSPARENT_BIN);
    }
    else
    {
      stateSetRef->setMode(GL_BLEND, StateAttribute::OFF);
      stateSetRef->setRenderingHint(StateSet::OPAQUE_BIN);
    }

    return true;
  }

  //: Zero owners reached.
  void ImageC::ZeroOwners()
  {
    NodeC::ZeroOwners();
  }
  
  static RavlN::XMLFactoryRegisterConvertC<ImageC,NodeC> g_registerXMLFactoryImageRGB("RavlOSGN::ImageC");

}
