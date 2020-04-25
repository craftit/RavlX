// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/OpenSceneGraph/TriMesh.cc"
//! lib=RavlGUIOpenSceneGraph
//! author = "Warren Moore"

#include "Ravl/OpenSceneGraph/TriMesh.hh"
#include "Ravl/IO.hh"
#include "Ravl/RLog.hh"
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
  
  TriMeshC::TriMeshC()
  {
    m_node = new Geode;
  }

  TriMeshC::TriMeshC(const Ravl3DN::TriMeshC &triMesh)
  {
    m_node = new Geode;
    
    SetMesh(triMesh);
  }

  //: XML Factory constructor
  
  TriMeshC::TriMeshC(const XMLFactoryContextC &factory)
   : NodeC(factory)
  {
    Setup(factory);
  }
  
  TriMeshC::~TriMeshC()
  {
  }

  //: Do setup from factory
  bool TriMeshC::Setup(const XMLFactoryContextC &factory)
  {
    NodeC::Setup(factory);
    RavlN::StringC filename;
    if(factory.Attribute("loadTriMesh",filename)) {
      Ravl3DN::TriMeshC triMesh;
      if(RavlN::Load(filename,triMesh)) {
        SetMesh(triMesh);
      } else {
        rWarning("Failed to load '%s' ",filename.data());
      }
    }
    return true;
  }

  bool TriMeshC::SetMesh(const Ravl3DN::TriMeshC &triMesh)
  {
    if (!m_node)
      return false;

    m_triMesh = triMesh;
    
    ref_ptr<Geometry> geometryRef = new Geometry();

    // Add the vertex array
    ref_ptr<Vec3Array> vertexArray = new Vec3Array();
    for (RavlN::SArray1dIterC<Ravl3DN::VertexC> vertexIter(triMesh.Vertices()); vertexIter; vertexIter++)
    {
      vertexArray->push_back(Vec3(vertexIter.Data().Position().X(), vertexIter.Data().Position().Y(), vertexIter.Data().Position().Z()));
    }
    geometryRef->setVertexArray(vertexArray);

    ref_ptr<Vec4Array> colourArray = new Vec4Array();
    ref_ptr<TemplateIndexArray<unsigned int, Array::UIntArrayType, 4, 4> > colourMap = new TemplateIndexArray<unsigned int, Array::UIntArrayType, 4, 4>;
    unsigned int colourIndex = 0;

    ref_ptr<Vec3Array> normalArray = new Vec3Array();

    for (RavlN::SArray1dIterC<Ravl3DN::TriC> faceIter(triMesh.Faces()); faceIter; faceIter++)
    {
      // Add the vertex indices
      ref_ptr<DrawElementsUInt> faceTriangle = new DrawElementsUInt(PrimitiveSet::TRIANGLES, 0);
      faceTriangle->push_back(triMesh.Index(faceIter.Data(), 0));
      faceTriangle->push_back(triMesh.Index(faceIter.Data(), 1));
      faceTriangle->push_back(triMesh.Index(faceIter.Data(), 2));
      geometryRef->addPrimitiveSet(faceTriangle);

      // Add the face colour (this method may introduce redundant colour data)
      RavlN::TFVectorC<RavlN::ByteT, 3> colour = faceIter.Data().Colour();
      colourArray->push_back(Vec4(static_cast<float>(colour[0]) / 255.0,
                                  static_cast<float>(colour[1]) / 255.0,
                                  static_cast<float>(colour[2]) / 255.0,
                                  1.0));

      // Map the face to the colour array index
      colourMap->push_back(colourIndex++);

      // Add the face normal
      normalArray->push_back(Vec3(faceIter.Data().FaceNormal().X(), faceIter.Data().FaceNormal().Y(), faceIter.Data().FaceNormal().Z()));
    }

    geometryRef->setColorArray(colourArray);
    geometryRef->setColorIndices(colourMap);
    geometryRef->setColorBinding(Geometry::BIND_PER_PRIMITIVE_SET);

    geometryRef->setNormalArray(normalArray);
    geometryRef->setNormalBinding(Geometry::BIND_PER_PRIMITIVE_SET);

    ref_ptr<Geode> geodeRef = dynamic_cast<Geode*>(m_node.get());
    if (m_geometry)
      geodeRef->replaceDrawable(m_geometry, geometryRef);
    else
      geodeRef->addDrawable(geometryRef);

    m_geometry = geometryRef;
    
    return true;
  }

  //: Called when owner handles drop to zero.
  void TriMeshC::ZeroOwners()
  {
    NodeC::ZeroOwners();
  }

  static RavlN::XMLFactoryRegisterConvertC<TriMeshC,NodeC> g_registerXMLFactoryTriMesh("RavlOSGN::TriMeshC");

}
