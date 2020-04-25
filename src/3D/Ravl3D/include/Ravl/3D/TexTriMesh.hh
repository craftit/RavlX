// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL3D_TEXTRIMESH_HEADER
#define RAVL3D_TEXTRIMESH_HEADER 1
////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/3D/Mesh/TexTriMesh.hh"
//! lib=Ravl3D
//! docentry="Ravl.API.3D.Mesh"
//! author="Jonathan Starck"

#include "Ravl/RefCounter.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/3D/TriMesh.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"

namespace Ravl3DN {
  using namespace RavlN;
  using namespace RavlImageN;

  class TexTriMeshBodyC;
  class TexTriMeshC;
  
  //! userlevel=Develop
  //: Textured tri mesh body.
  // Adds texture to a TriMeshC
  
  class TexTriMeshBodyC
    : public TriMeshBodyC
  {
  public:
    TexTriMeshBodyC()
    {}
    //: Default constructor

    TexTriMeshBodyC(const TexTriMeshBodyC& oth);
    //: Copy constructor

    TexTriMeshBodyC(const SArray1dC<Vector3dC> &vertices,
		    const SArray1dC<UIntT> &faceIndices)
      : TriMeshBodyC(vertices,faceIndices)
    { GenerateTextureMap(); }
    //: Construct from an array of vertices and an array of indices.
    // Successive triples are taken from faceIndices to form the faces in the mesh.
    // A texture map and texture coordinates are generated for the mesh.
    
    TexTriMeshBodyC(const SArray1dC<VertexC> &vertices,
		    const SArray1dC<TriC> &faces,
                    bool generateTexCoords = true)
      : TriMeshBodyC(vertices,faces)
    { if(generateTexCoords) GenerateTextureMap(); }
    //: Construct from an array of vertices and an array of tri's.
    // The TriC's must refer to elements in 'v'.
    // A texture map and texture coordinates are generated for the mesh.

    TexTriMeshBodyC(const SArray1dC<VertexC> &vertices,
		    const SArray1dC<TriC> &faces,
		    const SArray1dC< ImageC<ByteRGBValueC> >& textures,
		    const SArray1dC< StringC >& texFilenames = SArray1dC< StringC >())
      : TriMeshBodyC(vertices,faces,true),
	m_textures(textures),
	m_strFilenames(texFilenames)
    { }
    //: Construct from an array of vertices, an array of tri's and the texture images.
    // The TriC's must refer to elements in 'v' and must have texture coordinates.
    
    RCBodyVC& Copy() const
    { return *new TexTriMeshBodyC(*this); }
    //: Make a copy.
    
  public:
    SArray1dC< ImageC<ByteRGBValueC> >& Textures()
    { return m_textures; }
    //: Access the textures.

    const SArray1dC< ImageC<ByteRGBValueC> >& Textures() const
    { return m_textures; }
    //: Access the textures.

    IntT NumTextures() const
    { return m_textures.Size(); }
    //: Return the number of textures

    SArray1dC< StringC >& TexFilenames()
    { return m_strFilenames; }
    //: Access the textures.

    const SArray1dC< StringC >& TexFilenames() const
    { return m_strFilenames; }
    //: Access the textures.
    
    bool GenerateTextureMap(void);
    //: Automatically generate texture coordinates and a texture image
    
  protected:
    SArray1dC< ImageC<ByteRGBValueC> > m_textures;
    SArray1dC< StringC > m_strFilenames;
    // The texture images and filenames
  };
  
  
  //! userlevel=Normal
  //: Textured tri mesh body.
  // Adds texture to a TriMeshC
  
  class TexTriMeshC
    : public TriMeshC
  {
  public:
    TexTriMeshC()
    {}
    //: Default constructor.
    // Creates an invalid handle.

    TexTriMeshC(const SArray1dC<Vector3dC> &vertices,
		const SArray1dC<UIntT> &faceIndices)
      : TriMeshC(*new TexTriMeshBodyC(vertices,faceIndices))
    {}
    //: Construct from an array of vertices and an array of indices.
    // Successive triples are taken from faceIndices to form the faces in the mesh.
    // A texture map and texture coordinates are generated for the mesh.

    TexTriMeshC(const SArray1dC<VertexC> &vertices,
		const SArray1dC<TriC> &faces,
                bool generateTexCoord = true)
      : TriMeshC(*new TexTriMeshBodyC(vertices,faces,generateTexCoord))
    {}
    //: Construct from an array of vertices and an array of tri's.
    // The TriC's must refer to elements in 'v'.
    // A texture map and texture coordinates are generated for the mesh.
    
    TexTriMeshC(const TriMeshC& mesh,bool forceConv = true)
      : TriMeshC(dynamic_cast<const TexTriMeshBodyC *>(BodyPtr(mesh)))
    {
      // Don't like this, but it maintains compatability with old code...
      if(forceConv && !IsValid() && mesh.IsValid())
        (*this) = TexTriMeshC(mesh.Vertices(),mesh.Faces(),!mesh.HaveTextureCoord());
    }
    //: Construct from a TriMesh.
    // Creates a textured mesh that references the vertices and faces in the TriMesh.
    // A texture map and texture coordinates are generated for the mesh.

    TexTriMeshC(const SArray1dC<VertexC> &vertices,
		const SArray1dC<TriC> &faces,
		const SArray1dC< ImageC<ByteRGBValueC> >& textures,
		const SArray1dC< StringC >& texFilenames  = SArray1dC< StringC >())
      : TriMeshC(*new TexTriMeshBodyC(vertices,faces,textures,texFilenames))
    {}
    //: Construct from an array of vertices, an array of tri's and the texture images.
    // The TriC's must refer to elements in 'v' and must have texture coordinates.
    
  protected:
    TexTriMeshC(TexTriMeshBodyC &bod)
      : TriMeshC(bod)
    {}
    //: Body constructor.
    
    TexTriMeshC(const TexTriMeshBodyC *bod)
      : TriMeshC(bod)
    {}
    //: Body constructor.
    
    TexTriMeshBodyC &Body()
    { return static_cast<TexTriMeshBodyC &>(TriMeshC::Body()); }
    //: Access body.

    const TexTriMeshBodyC &Body() const
    { return static_cast<const TexTriMeshBodyC &>(TriMeshC::Body()); }
    //: Access body.
    
  public:
    TexTriMeshC Copy() const
    { return TexTriMeshC(static_cast<TexTriMeshBodyC &>(Body().Copy())); }
    //: Make a copy of the mesh.
    
    bool GenerateTextureMap(void)
    { return Body().GenerateTextureMap(); }
    //: Generate a default set of texture coordinates and a texture image
    
    SArray1dC< ImageC<ByteRGBValueC> >& Textures()
    { return Body().Textures(); }
    //: Access the textures.

    const SArray1dC< ImageC<ByteRGBValueC> >& Textures() const
    { return Body().Textures(); }
    //: Access the textures.

    IntT NumTextures() const
    { return Body().NumTextures(); }
    //: Return the number of textures
    
    SArray1dC< StringC >& TexFilenames()
    { return Body().TexFilenames(); }
    //: Access the textures.

    const SArray1dC< StringC >& TexFilenames() const
    { return Body().TexFilenames(); }
    //: Access the textures.

  };

  ostream &operator<<(ostream &s,const TexTriMeshC &ts);
  istream &operator>>(istream &s,TexTriMeshC &ts);
  
  BinIStreamC &operator>>(BinIStreamC &s,TexTriMeshC &ts);
  BinOStreamC &operator<<(BinOStreamC &s,const TexTriMeshC &ts);
  
}

#endif
