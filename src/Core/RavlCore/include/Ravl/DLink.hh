// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DLINK_HEADER
#define RAVL_DLINK_HEADER 1
//////////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Core.Lists"
//! file="Ravl/Core/Container/DList/DLink.hh"
//! lib=RavlCore
//! userlevel=Normal
//! author="Radek Marik"

#include "Ravl/Types.hh"

//: Ravl library namespace.

namespace RavlN {  
  
  //! userlevel=Advanced
  //: Double-link element.
  
  class DLinkC {
  public:
    inline DLinkC() {
      succ = this;
      pred = this;
    }
    //: Creates one double-link which points to itself.
    
    inline DLinkC(const DLinkC &) {
      succ = this;
      pred = this;
    }
    //: Copy constructor. 
    // Don't copy link values in copy constructor it makes no
    // sense
    
    inline ~DLinkC() 
    { Unlink(); }
    //: Make sure we're unlinked from a list.
    
    inline void SetSelfPointing(){ 
      succ = this;
      pred = this;
    }
    //: Set both links to point to this object.
    
    inline bool IsSelfPointing() const
    { return succ == this;  }
    // Returns TRUE if the next element is this element.
    
    inline const DLinkC & Next() const
    { return *succ; }
    // Returns the next element in a constant list.
    
    inline const DLinkC & Prev() const
    { return *pred; }
    // Returns the previous element in a constant list.
    
    inline DLinkC & Next()
    { return *succ; }
    // Returns the next element in a list.
    
    inline DLinkC & Prev()
    { return *pred; }
    // Returns the previous element in a list.
    
    inline void Reverse() {
      DLinkC * tmp = succ;
      succ = pred;
      pred = tmp;
    }
    //: Swap links.
    // Swaps the links in all chain elements exchanging meaning of
    // "successor" and "predecessor".
    
    inline DLinkC & LinkBef(DLinkC & elm) {
      elm.succ = this;
      elm.pred = pred;
      pred = pred->succ = &elm;
      return(elm);
    }

    //: Links 'elm' before the element.
    // The links of 'elm' need not to be proper links.
    
    inline DLinkC & LinkAft(DLinkC & elm) {
      elm.succ = succ;
      elm.pred = this;
      succ = succ->pred = &elm;
      return(elm);
    }
    //: Links 'elm' after this element. 
    // The links of 'elm' need not to be proper links.
    
    inline DLinkC & Unlink() {
      pred->succ = succ;
      succ->pred = pred;
      return *(this);
    }
    //: Unlinks the list element from the list. 
    // The predecessor
    // and the successor of this element will be linked to each other.
    // However, the links of this element stay unchanged.
    // Returns this list element.
    
    inline DLinkC &Remove() {
      Unlink();
      SetSelfPointing();
      return *this;
    }
    //: Remove element from the list and set it to self pointing.
    //: This is safer than just using Unlink() as it avoid's
    //: list corruption when DLinkC's are deleted.

    void CutPaste(DLinkC & firstCut, DLinkC & firstNotCut);
    //: Splice list elements between firstCut, and firstNotCut into this list.
    // Cuts the chain of double links elements starting at 'firstCut' and
    // ending at the element previous of 'firstNotCut' from a chain.
    // The rest of the chain is linked together again. The cut part
    // is linked in after this element.
    
  protected:
    DLinkC * succ;  // the next element
    DLinkC * pred;  // the previous element    
    
    friend class DLinkHeadC;
  };
  
  //! userlevel=Advanced
  //: Double-link list element with data.
  
  template<class DataT>
  class DLinkDataC 
    : public DLinkC 
  {
  public:
    DLinkDataC(const DataT &ndat)
      : data(ndat)
    {}
    //: Data constructor.
    
    DataT &Data()
    { return data; }
    //: Access data.
    
    const DataT &Data() const
    { return data; }
    //: Access data.
    
  protected:
    DataT data;
  };

  //! userlevel=Advanced
  //: Double-link list head.
  
  class DLinkHeadC {
  public:
    DLinkHeadC()
    {}
    //: Default constructor.
    // creates an empty list.
    
    bool IsEmpty() const
    { return head.IsSelfPointing(); }
    //: Is list empty ?
    
    SizeT Size() const;
    //: Count the number of elements in the list.
    // This is of order N, where N is the number of elements in
    // the list.
    
    void Reverse();
    //: Reverse the order of the list.
    // This is of order N, where N is the number of elements in
    // the list.
    
    typedef bool (*MergeSortInterCmpT)(DLinkC *l1,DLinkC *l2,void *dat);
    //: Comparison function for merge sort.
    
    void MergeSort(MergeSortInterCmpT ms,void *pass = 0);
    //: Merge sort the list using given comparison function.
    
    DLinkC &Head()
    { return head; }
    //: Get head of list.
    
  protected:
    
    const DLinkC &Head() const
    { return head; }
    //: Get head of list.
    
    void InsFirst(DLinkC &dat)
    { head.LinkAft(dat); }
    //: Push element onto the begining of the list.
    
    void InsLast(DLinkC &dat)
    { head.LinkBef(dat); }
    //: Push element onto the begining of the list.
    
    void MoveFirst(DLinkHeadC &lst)
    { head.CutPaste(lst.Head().Next(), lst.Head()); }
    //: Move the entire contents of 'lst' to the front of this one.
    // this leaves 'lst' empty.
    
    void MoveLast(DLinkHeadC &lst) 
    { head.Prev().CutPaste(lst.Head().Next(), lst.Head()); }
    //: Move the entire contents of 'lst' to the back of this one.
    // this leaves 'lst' empty.
    
    void MoveFirst(DLinkC &at)
    { at.Unlink(); head.LinkAft(at); }
    //: Move the item 'at' to the front of this list
    
    void MoveLast(DLinkC &at) 
    { at.Unlink(); head.LinkBef(at);  }
    //: Move the item 'at' to the back of this list
    
    DLinkC** MergeSortInternal(DLinkC** chead,SizeT n,MergeSortInterCmpT ms,void *pass);
    //: Merge sort the list using given comparison function.
    
    DLinkC head; // Head of list.
  };

  
}

#endif
