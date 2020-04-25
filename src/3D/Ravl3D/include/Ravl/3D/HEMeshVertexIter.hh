// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL3D_HEMESHVERTEXITER_HEADER
#define RAVL3D_HEMESHVERTEXITER_HEADER 1
//! rcsid="$Id$"
//! lib=Ravl3D
//! author="Charles Galambos"
//! docentry="Ravl.API.3D.Half Edge Mesh"
//! date="22/5/2002"
//! file="Ravl/3D/Mesh/HEMeshVertexIter.hh"

#include "Ravl/3D/HEMeshVertex.hh"
#include "Ravl/IntrDLIter.hh"

namespace Ravl3DN {

  class HEMeshC;

  //! userlevel=Normal
  //: Mesh vertex iterator.
  // Note: It is the users responsibility to ensure the current vertex of the interator is NOT
  // deleted until the iterator is moved to another element.  
  
  class HEMeshVertexIterC 
    : public IntrDLIterC<HEMeshVertexBodyC>
  {
  public:
    HEMeshVertexIterC()
    {}
    //: Default constructor.

    HEMeshVertexIterC(IntrDListC<HEMeshVertexBodyC> &iter)
      : IntrDLIterC<HEMeshVertexBodyC>(iter)
    {}
    //: Constructor from a IntrDListC.

    HEMeshVertexC Data()
    { return HEMeshVertexC(IntrDLIterC<HEMeshVertexBodyC>::Data()); }
    //: Access Vertex.

    const HEMeshVertexC Data() const
    { return HEMeshVertexC(const_cast<HEMeshVertexBodyC &>(IntrDLIterC<HEMeshVertexBodyC>::Data())); }
    //: Access Vertex.

    HEMeshVertexC operator*()
    { return HEMeshVertexC(IntrDLIterC<HEMeshVertexBodyC>::Data()); }
    //: Access Vertex.
    
    const HEMeshVertexC operator*() const
    { return HEMeshVertexC(const_cast<HEMeshVertexBodyC &>(IntrDLIterC<HEMeshVertexBodyC>::Data())); }
    //: Access Vertex.
    
  protected:
    
    friend class HEMeshC;
  };
  
}


#endif
