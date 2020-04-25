// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Container/DList/DLink.cc"

// The following people contributed to the design of these list 
// structures:
// B. Stroustrup, G. Matas, R. Sara (K320870@edvz.uni-linz.ac.at)

#include "Ravl/DLink.hh"

namespace RavlN {
  
  //// DLinkC /////////////////////////////////////////////////
  
  void DLinkC::CutPaste(DLinkC & firstCut, DLinkC & firstNotCut) {
    if (&firstCut == &firstNotCut) 
      return;  // Nothing to cut.
    
    DLinkC *lastCut = firstNotCut.pred; 
    
    // Cut the chain
    // -------------
    firstCut.pred->succ = &firstNotCut;     // forward chain fixed
    firstNotCut.pred = firstCut.pred;       // backward chain fixed
    
    // Paste the chain to this chain
    // ------------------------------
    
    // chain of predecessors
    succ->pred = lastCut;    // from this chain back to the cut chain
    firstCut.pred = this;    // from the cut chain back to this one
    
    // chain of successors
    lastCut->succ = succ;     // from the cut chain to this one
    succ = &firstCut;         // from this chain to the cut chain
  }
  
  //// DLinkHeadC /////////////////////////////////////////////////
  
  //: Count the number of elements in the list.
  // This is of order N, where N is the number of elements in
  // the list.
  
  SizeT DLinkHeadC::Size() const {
    SizeT count = 0;
    for(const DLinkC *place = &head.Next();place != &head;place = &place->Next())
      count++;
    return count;
  }

  //: Reverse the order of the list.
  // This is of order N, where N is the number of elements in
  // the list.

  void DLinkHeadC::Reverse() {
    DLinkC *place = &head.Next();
    head.Reverse();
    for(;place != &head;place = &place->Prev())
      place->Reverse();
  }
  
  //: Merge sort the list using given comparison function.
  
  void DLinkHeadC::MergeSort(MergeSortInterCmpT LessOrEqual,void *pass) {
    MergeSortInternal(&Head().succ,Size(),LessOrEqual,pass);
    //: Fix backward links.
    DLinkC * ptr = &head;
    do {
      ptr->succ->pred = ptr;
      ptr = ptr->succ;
    } while (ptr != &head);
  }
  
  //: Merge sort the list using given comparison function.
  // FIXME:- Get rid of this define.
#define getlink(elm)   ((elm)->succ)
  
  DLinkC** DLinkHeadC::MergeSortInternal(DLinkC** chead,SizeT n,MergeSortInterCmpT LessOrEqual,void *pass) {
    DLinkC* p1, *p2;
    DLinkC **h2, **t2;
    unsigned m;
    
    switch (n) 
      {
      case 0: return chead;
      case 1: return &getlink(*chead);
      case 2:
	p2 = getlink(p1 = *chead);
	if (LessOrEqual(p1, p2,pass)) 
	  return &getlink(p2);
	getlink(p1) = getlink(*chead=p2);
	return &getlink(getlink(p2) = p1);
      }
    
    n -= m = n / 2;
    
    t2 = MergeSortInternal(h2 = MergeSortInternal(chead, n,LessOrEqual,pass), m,LessOrEqual,pass);
    
    if (LessOrEqual(p1 = *chead, p2 = *h2,pass)) {
      do {
	if (!--n)
	  return *h2 = p2, t2;
      } while (LessOrEqual(p1=*(chead=&getlink(p1)), p2,pass));
    }
    
    for (;;) {
      *chead = p2;
      do {
	if (!--m)
	  return *h2 = *t2, *t2 = p1, h2;
      } while (!LessOrEqual(p1, p2=*(chead=&getlink(p2)),pass));
      *chead = p1;
      do {
	if (!--n)
	  return *h2 = p2, t2;
      } while (LessOrEqual(p1=*(chead=&getlink(p1)), p2,pass));
    }
    
    return 0;
  }
  
}
