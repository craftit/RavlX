// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_HEMESHBASEVERTEXITER_HEADER
#define RAVL_HEMESHBASEVERTEXITER_HEADER 1
//! rcsid="$Id$"
//! lib=RavlCore
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Graphs.Half Edge Mesh"
//! date="22/5/2002"
//! file="Ravl/Core/Container/Graph/HEMeshBaseVertexIter.hh"

#include "Ravl/HEMeshBaseVertex.hh"
#include "Ravl/IntrDLIter.hh"

namespace RavlN {

  class HEMeshBaseC;

  //! userlevel=Normal
  //: Mesh vertex iterator.
  // Note: It is the users responsibility to ensure the current vertex of the iterator is NOT
  // deleted until the iterator is moved to another element.  
  
  class HEMeshBaseVertexIterC 
    : public IntrDLIterC<HEMeshBaseVertexBodyC>
  {
  public:
    HEMeshBaseVertexIterC()
    {}
    //: Default constructor.

    HEMeshBaseVertexIterC(const IntrDListC<HEMeshBaseVertexBodyC> &iter)
      : IntrDLIterC<HEMeshBaseVertexBodyC>(iter)
    {}
    //: Constructor from a IntrDListC.

    HEMeshBaseVertexC Data()
    { return HEMeshBaseVertexC(IntrDLIterC<HEMeshBaseVertexBodyC>::Data()); }
    //: Access Vertex.

    const HEMeshBaseVertexC Data() const
    { return HEMeshBaseVertexC(const_cast<HEMeshBaseVertexBodyC &>(IntrDLIterC<HEMeshBaseVertexBodyC>::Data())); }
    //: Access Vertex.

    HEMeshBaseVertexC operator*()
    { return HEMeshBaseVertexC(IntrDLIterC<HEMeshBaseVertexBodyC>::Data()); }
    //: Access Vertex.
    
    const HEMeshBaseVertexC operator*() const
    { return HEMeshBaseVertexC(const_cast<HEMeshBaseVertexBodyC &>(IntrDLIterC<HEMeshBaseVertexBodyC>::Data())); }
    //: Access Vertex.
    
  protected:
    
    friend class HEMeshBaseC;
  };
  
}


#endif
