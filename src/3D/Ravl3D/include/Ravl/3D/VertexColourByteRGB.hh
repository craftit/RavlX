// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL3D_VERTEX_COLOUR_BYTE_RGB_HEADER
#define RAVL3D_VERTEX_COLOUR_BYTE_RGB_HEADER 1
//! rcsid="$Id$"
//! lib=Ravl3D
//! file="Ravl/3D/Mesh/VertexColourByteRGB.hh"
//! docentry="Ravl.API.3D.Mesh"

#include "Ravl/3D/Vertex.hh"
#include "Ravl/Image/ByteRGBValue.hh"


namespace Ravl3DN
{
  using namespace RavlN;  
  using namespace RavlImageN;
#if RAVL_VISUALCPP_NAMESPACE_BUG
  using RavlN::Vector3dC;
  using RavlImageN::ByteRGBValueC;
  using RavlN::BinOStreamC;
  using RavlN::BinIStreamC;
#endif
  
  //! userlevel=Normal
  //: Vertex in a mesh, with associated colour value
  // This holds a position, a normal, and a colour value in Byte RGB format
  
  class VertexColourByteRGBC : public VertexC
  {
  public:
    VertexColourByteRGBC()
    {}
    //: Default constructor.
    
    VertexColourByteRGBC(const Point3dC &pos,
			 const Vector3dC &norm,
			 const ByteRGBValueC &col) :
      VertexC(pos, norm),
      colour(col)
    {}
    //: Constructor.
    
    
    ByteRGBValueC& Colour()
    { return colour; }
    //: Access colour of vertex

    const ByteRGBValueC& Colour() const
    { return colour; }
    //: Access colour of vertex (read-only)

  protected:
    ByteRGBValueC colour;  // colour of vertex
  };
  
  ostream &operator<<(ostream &s,const VertexColourByteRGBC &v);
  istream &operator>>(istream &s,VertexColourByteRGBC &v);
  
  BinOStreamC &operator<<(BinOStreamC &s,const VertexColourByteRGBC &v);
  BinIStreamC &operator>>(BinIStreamC &s,VertexColourByteRGBC &v);

}

#endif
