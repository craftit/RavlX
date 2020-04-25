// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/LinearAlgebra/General/TSMatrixSparse.cc"

#include "Ravl/TSMatrixSparse.hh"

namespace RavlN {
  
  //: Find element with given index.
  // Assumes an ordered list.
  
  const IndexDLinkC *IndexDListC::Find(int i) const {
    for(IntrDLIterC<IndexDLinkC> it(*this);it;it++) {
      if(it->Index() == i)
	return &(*it);
      if(it->Index() > i)
	return 0;
    }
    return 0;
  } 
    
  //: Add an item to the list.
  // returns false if its already present.
  
  bool IndexDListC::Insert(IndexDLinkC &newun) {
    IndexC i = newun.Index();
    for(IntrDLIterC<IndexDLinkC > it(*this);it;it++) {
      if(it->Index() == i)
	return false;
      if(it->Index() > i) {
	it.InsertBef(newun);
	return true;
      }
    }
    InsLast(newun);
    return true;
  }

}
