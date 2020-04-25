// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/OpenSceneGraph/TexTriMesh.cc"
//! lib=RavlGUIOpenSceneGraph
//! author = "Warren Moore"

#include "Ravl/OpenSceneGraph/TexTriMesh.hh"
#include "Ravl/RLog.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include "Ravl/IO.hh"

#include <osg/Geode>
#include <osg/Geometry>
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
  
  TexTriMeshC::TexTriMeshC()
  {
    m_node = new Geode;
  }

  TexTriMeshC::TexTriMeshC(const Ravl3DN::TexTriMeshC &texTriMesh)
  {
    m_node = new Geode;

    SetMesh(texTriMesh);
  }

  //: XML Factory constructor
  
  TexTriMeshC::TexTriMeshC(const XMLFactoryContextC &factory)
  {
    m_node = new Geode;
    Setup(factory);
  }

  TexTriMeshC::~TexTriMeshC()
  {
  }
  
  //: Do setup from factory
  bool TexTriMeshC::Setup(const XMLFactoryContextC &factory)
  {
    NodeC::Setup(factory);
    RavlN::StringC filename;
    if(factory.Attribute("loadTriMesh",filename)) {
      Ravl3DN::TexTriMeshC triMesh;
      if(RavlN::Load(filename,triMesh)) {
        SetMesh(triMesh);
      } else {
        rWarning("Failed to load '%s' ",filename.data());
      }
    }
    return true;

  }


  bool TexTriMeshC::SetMesh(const Ravl3DN::TexTriMeshC &texTriMesh)
  {
    if (!m_node)
      return false;

    // Check the images before creating the mesh
    for (RavlN::SArray1dIterC<RavlImageN::ImageC<RavlImageN::ByteRGBValueC> > textureIter(texTriMesh.Textures()); textureIter; textureIter++)
    {
      if (!textureIter.Data().IsValid() || !textureIter.Data().Size() > 0 || !textureIter.Data().IsContinuous())
        return false;
    }

    ref_ptr<Geode> geodeRef = dynamic_cast<Geode*>(m_node.get());
    if (!geodeRef)
      return false;

    // Create the mesh
    if (!TriMeshC::SetMesh(texTriMesh))
      return false;

    ref_ptr<Geometry> geometryRef = dynamic_cast<Geometry*>(m_geometry.get());
    if (!geometryRef)
      return false;

    const int vertexCount = texTriMesh.Vertices().Size().V();
    ref_ptr<Vec2Array> textureCoords = new Vec2Array(vertexCount);
    for (RavlN::SArray1dIterC<Ravl3DN::TriC> faceIter(texTriMesh.Faces()); faceIter; faceIter++)
    {
      (*textureCoords)[texTriMesh.Index(faceIter.Data(), 0)].set(faceIter.Data().TextureCoord(0).X(), faceIter.Data().TextureCoord(0).Y());
      (*textureCoords)[texTriMesh.Index(faceIter.Data(), 1)].set(faceIter.Data().TextureCoord(1).X(), faceIter.Data().TextureCoord(1).Y());
      (*textureCoords)[texTriMesh.Index(faceIter.Data(), 2)].set(faceIter.Data().TextureCoord(2).X(), faceIter.Data().TextureCoord(2).Y());
    }

    for (int i = 0; i < texTriMesh.NumTextures(); i++)
      geometryRef->setTexCoordArray(i, textureCoords);

    ref_ptr<StateSet> stateSetRef = geodeRef->getOrCreateStateSet();

    // Look over textures
    int textureIndex = 0;
    for (RavlN::SArray1dIterC<RavlImageN::ImageC<RavlImageN::ByteRGBValueC> > textureIter(texTriMesh.Textures()); textureIter; textureIter++)
    {
      ref_ptr<Texture2D> textureRef = new Texture2D();
      textureRef->setDataVariance(Object::DYNAMIC);

      RavlImageN::ImageC<RavlImageN::ByteRGBValueC> image = textureIter.Data();
      int width = image.Cols().V();
      int height = image.Rows().V();
      RavlN::BufferAccessC<RavlImageN::ByteRGBValueC> rowAccess = image.RowPtr(0);
      unsigned char* imageData = reinterpret_cast<unsigned char*>(rowAccess.ReferenceVoid());
      if (imageData == NULL)
        return false;
      ref_ptr<Image> imageRef = new Image();
      imageRef->setImage(width, height, 1, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, (unsigned char*)imageData, Image::NO_DELETE, 1);

      textureRef->setImage(imageRef);

      stateSetRef->setTextureAttributeAndModes(textureIndex, textureRef, StateAttribute::ON);
      textureIndex++;
    }

    geodeRef->setStateSet(stateSetRef);

    return true;
  }
  
  //: Called when owner handles drop to zero.
  
  void TexTriMeshC::ZeroOwners() {
    TriMeshC::ZeroOwners();
  }

  static RavlN::XMLFactoryRegisterConvertC<TexTriMeshC,TriMeshC> g_registerXMLFactoryTexTriMesh("RavlOSGN::TexTriMeshC");

}
