// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL3D_HEMESHFACEITER_HEADER
#define RAVL3D_HEMESHFACEITER_HEADER 1
//! rcsid="$Id$"
//! lib=Ravl3D
//! author="Charles Galambos"
//! docentry="Ravl.API.3D.Half Edge Mesh"
//! date="22/5/2002"
//! file="Ravl/3D/Mesh/HEMeshFaceIter.hh"

#include "Ravl/3D/HEMeshFace.hh"
#include "Ravl/IntrDLIter.hh"

namespace Ravl3DN {
  
  class HEMeshC;

  //! userlevel=Normal
  //: Mesh face iterator.
  // Note: It is the users responsibility to ensure the current face of the interator is NOT
  // deleted until the iterator is moved to another element.  Methods such as CollapseEdge()
  // are provided to help ensure this.
  
  class HEMeshFaceIterC 
    : public IntrDLIterC<HEMeshFaceBodyC>
  {
  public:
    HEMeshFaceIterC()
    {}
    //: Default constructor.

    HEMeshFaceIterC(IntrDListC<HEMeshFaceBodyC> &iter)
      : IntrDLIterC<HEMeshFaceBodyC>(iter)
    {}
    //: Constructor.

    HEMeshFaceC Data()
    { return HEMeshFaceC(IntrDLIterC<HEMeshFaceBodyC>::Data()); }
    //: Access Face.

    const HEMeshFaceC Data() const
    { return HEMeshFaceC(const_cast<HEMeshFaceBodyC &>(IntrDLIterC<HEMeshFaceBodyC>::Data())); }
    //: Access Face.

    HEMeshFaceC operator*()
    { return HEMeshFaceC(IntrDLIterC<HEMeshFaceBodyC>::Data()); }
    //: Access Face.
    
    const HEMeshFaceC operator*() const
    { return HEMeshFaceC(const_cast<HEMeshFaceBodyC &>(IntrDLIterC<HEMeshFaceBodyC>::Data())); }
    //: Access Face.
    
    bool CollapseEdge(HEMeshEdgeC edge);
    //: Collapse an edge on the current face.
    // If the face is deleted in the process the iterator is moved
    // to the previous valid face. It will return true if the iterator
    // has been changed.
    
  protected:
    
    friend class HEMeshC;
  };
  
}


#endif
