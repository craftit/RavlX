// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlCore
//! author="Charles Galambos"
//! rcsid="$Id$"
//! file="Ravl/Core/Container/Graph/HEMeshBaseFaceIter.cc"

#include "Ravl/HEMeshBaseFaceIter.hh"

namespace RavlN {
  
  //: Collapse an edge on the current face.
  // If the face is deleted in the process the iterator is moved
  // to the previous valid face.
  
  bool HEMeshBaseFaceIterC::CollapseEdge(HEMeshBaseEdgeC edge) {
    RavlAssertMsg(IsElm(),"HEMeshBaseFaceIterC::CollapseEdge(), Iterator not on a valid face. ");
    RavlAssertMsg(edge.IsValid(),"HEMeshBaseFaceIterC::CollapseEdge(), Invalid edge. ");
    bool changed = false;
    HEMeshBaseFaceC face1 = edge.Face();
    if(face1.Sides() < 4) { // Will face be deleted ?
      if(face1 == Data()) {
	Prev();
	if(!IsElm()) // If we're at the beginning of the list we're ok.
	  return true;
	changed = true;
      }
    }
    if(!edge.HasPair()) 
      return changed;
    
    HEMeshBaseEdgeC oe = edge.Pair();
    HEMeshBaseFaceC face2 = oe.Face();
    if(face2.Sides() > 3) // Will face be kept ?
      return changed;
    if(face2 == Data()) {
      Prev();
      changed = true;
    }
    edge.CollapseEdge();
    return changed;
  }

}
