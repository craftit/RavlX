// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL3D_BUILDTEXTRIMESH_HEADER
#define RAVL3D_BUILDTEXTRIMESH_HEADER 1
//! docentry="Ravl.API.3D.Mesh"
//! author="Charles Galambos"
//! date="22/2/2008"
//! lib=Ravl3D

#include "Ravl/Math.hh"
#include "Ravl/Collection.hh"
#include "Ravl/Collection.hh"
#include "Ravl/3D/TexTriMesh.hh"
#include "Ravl/Stack.hh"
#include "Ravl/PProjection3d.hh"

namespace RavlN {
  class Affine3dC;
}

namespace RavlImageN {
  class ByteRGBAValueC;
}

namespace Ravl3DN {
  
  //! userlevel=Normal
  //: This is a helper class for building textured tri meshes.
  // This class allows faces to be appended to an existing set. You can
  // create a TexTriMeshC at any time using Build(); 
  // This is a small object.
  
  class BuildTexTriMeshC {
  public:
    //! Constructor.
    BuildTexTriMeshC(UIntT noVerticesEstimate = 100);
    
    //! Append a new texture, its id is returned.
    UIntT AddTexture(const ImageC<ByteRGBValueC> &texture,const StringC &name = "");
    
    //! Append a new texture, its id is returned.
    //! Note: transparency is not currently supported, but that may change in the future.
    UIntT AddTexture(const ImageC<ByteRGBAValueC> &texture,const StringC &name = "");
    
    //! Add face to texture.
    // The texture coorinates are assumed to those of the actual positions in the texture. (NOT scaled 0.0-1.0)
    // At least 3 points are required. The polygon formed in the texture coordinates should not self cross.
    // The 3d points are assumed to lie on a plane.
    bool AddFace(UIntT textureId,const SArray1dC<Point3dC> &position3d,const SArray1dC<Point2dC> &texturePoints);
    
    //! Add an existing mesh to this one.
    void AddMesh(const TexTriMeshC &mesh);
    
    //! Add a transformed mesh to this one.
    void AddMesh(const TexTriMeshC &mesh,const Affine3dC &transform);
    
    //! Build a mesh from current contents.
    // This is a quick operation, as it only takes references
    // to existing array's.
    TexTriMeshC Build() const {
      RavlAssert(m_textures.Size() == m_texFilenames.Size());
      return TexTriMeshC(m_vertices.Array(),
			 m_faces.Array(),
			 m_textures.Array(),
			 m_texFilenames.Array());
    }
    
    //! Push a transform onto the stack.
    void TransformPush(const Affine3dC &transform);
    
    //! Push a transform onto the stack.
    void TransformPush(const PProjection3dC &transform);
    
    //! Pop a transfrom from the stack.
    void TransformPop()
    { m_geomStack.Pop(); }
    
  protected:
    CollectionC<VertexC> m_vertices;
    CollectionC<TriC> m_faces;
    CollectionC<ImageC<ByteRGBValueC> > m_textures;
    CollectionC<StringC> m_texFilenames;
    StackC<PProjection3dC> m_geomStack;
  };
}


#endif
