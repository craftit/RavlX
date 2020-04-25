// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_HEMESHBASEFACEITER_HEADER
#define RAVL_HEMESHBASEFACEITER_HEADER 1
//! rcsid="$Id$"
//! lib=RavlCore
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Graphs.Half Edge Mesh"
//! date="22/5/2002"
//! file="Ravl/Core/Container/Graph/HEMeshBaseFaceIter.hh"

#include "Ravl/HEMeshBaseFace.hh"
#include "Ravl/IntrDLIter.hh"

namespace RavlN {
  
  class HEMeshBaseC;

  //! userlevel=Normal
  //: Mesh face iterator.
  // Note: It is the users responsibility to ensure the current face of the interator is NOT
  // deleted until the iterator is moved to another element.  Methods such as CollapseEdge()
  // are provided to help ensure this.
  
  class HEMeshBaseFaceIterC 
    : public IntrDLIterC<HEMeshBaseFaceBodyC>
  {
  public:
    HEMeshBaseFaceIterC()
    {}
    //: Default constructor.

    HEMeshBaseFaceIterC(const IntrDListC<HEMeshBaseFaceBodyC> &iter)
      : IntrDLIterC<HEMeshBaseFaceBodyC>(iter)
    {}
    //: Constructor.

    HEMeshBaseFaceC Data()
    { return HEMeshBaseFaceC(IntrDLIterC<HEMeshBaseFaceBodyC>::Data()); }
    //: Access Face.

    const HEMeshBaseFaceC Data() const
    { return HEMeshBaseFaceC(const_cast<HEMeshBaseFaceBodyC &>(IntrDLIterC<HEMeshBaseFaceBodyC>::Data())); }
    //: Access Face.
    
    HEMeshBaseFaceC operator*()
    { return HEMeshBaseFaceC(IntrDLIterC<HEMeshBaseFaceBodyC>::Data()); }
    //: Access Face.
    
    const HEMeshBaseFaceC operator*() const
    { return HEMeshBaseFaceC(const_cast<HEMeshBaseFaceBodyC &>(IntrDLIterC<HEMeshBaseFaceBodyC>::Data())); }
    //: Access Face.
    
    HEMeshBaseFaceBodyC *operator->()
    { return &IntrDLIterC<HEMeshBaseFaceBodyC>::Data(); }
    //: Access Face.
    
    const HEMeshBaseFaceBodyC *operator->() const
    { return &IntrDLIterC<HEMeshBaseFaceBodyC>::Data(); }
    //: Access Face.
    
    bool CollapseEdge(HEMeshBaseEdgeC edge);
    //: Collapse an edge on the current face.
    // If the face is deleted in the process the iterator is moved
    // to the previous valid face. It will return true if the iterator
    // has been changed.
    
  protected:
    
    friend class HEMeshBaseC;
  };
  
}


#endif
