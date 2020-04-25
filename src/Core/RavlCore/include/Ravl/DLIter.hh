// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DLITER_HEADER
#define RAVL_DLITER_HEADER 1
////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Core.Lists"
//! file="Ravl/Core/Container/DList/DLIter.hh"
//! lib=RavlCore
//! userlevel=Normal
//! author="Radek Marik, Charles Galambos"
//! example=exDList.cc

// DLIterC is based on code written by Radek Marik.

#include "Ravl/DList.hh"

namespace RavlN {

  //! userlevel=Normal
  //: Double linked list iterator.
  // Note: This iterator holds a reference to the list it is iterating.
  // (Unlike some other iterators.)
  
  template<class DataT> 
  class DLIterC  {
  public:
    DLIterC()
    { place = &lst.Head(); }
    //: Default constructor.
    
    bool IsValid() const
    { return lst.IsValid(); }
    //: Is a valid iterator ?
    
    void First() 
    { place = &lst.Head().Next(); }
    //: Goto first item in list.
    
    void Last() 
    { place = &lst.Head().Prev(); }
    //: Goto last item in list.
    
    DLIterC(const DListC<DataT> &nlst)
      : lst(nlst)
    { First(); }
    //: Construct an iterator from a list.
    // The iterator is placed at the first
    // element in the list. If there is any.
        
    const DListC<DataT> &List() const
    { return lst; }
    //: Access list we're iterating.
    
    DListC<DataT> Copy() const
    { return List().Copy(); }
    //: Make a copy of this list.
    
    operator bool() const
    { return place != &lst.Head(); }
    //: Is iterator at a valid position ?
    
    bool operator!() const
    { return place == &lst.Head(); }
    //: Is iterator at a invalid position ?
    
    void operator++(int) 
    { place = &place->Next(); }
    //: Goto next element.
    
    DLIterC<DataT> &operator++() 
    { place = &place->Next(); return *this; }
    //: Goto next element.
    
    void operator--(int) 
    { place = &place->Prev(); }
    //: Goto previous element.
    
    DLIterC<DataT> &operator--() 
    { place = &place->Prev(); return *this; }
    //: Goto previous element.
    
    void InsertBef(const DataT &dat)
    { place->LinkBef(*new DLinkDataC<DataT>(dat)); }
    //: Insert data before current element.
    // if at the head of the list  (i.e. operator bool() failes.)
    // then add at end.
    
    void InsertAft(const DataT &dat)
    { place->LinkAft(*new DLinkDataC<DataT>(dat)); }
    //: Insert data after current element.
    // if at the head of the list  (i.e. operator bool() failes.)
    // then add at begining.
    
    void MoveBef(DLIterC<DataT> &it) { 
      if(it.place != place)
	place->LinkBef(it.Extract()); 
    }
    //: Move the list element indicated by 'it' to before
    //: the element in this list.

    void MoveListBefore(DListC<DataT> &alist) { 
      RavlAssert(lst != alist);
      place->Prev().CutPaste(alist.Head().Next(),alist.Head());
    }
    //: Move the list before the current element.
    // Note alist should not be the list we're iterating.
    
    void MoveAft(DLIterC<DataT> &it) { 
      if(it.place != place)
	place->LinkAft(it.Extract());      
    }
    //: Move the list element indicated by 'it' to before
    //: the element in this list.
    
    void MoveListAfter(DListC<DataT> &alist) { 
      RavlAssert(lst != alist);
      place->CutPaste(alist.Head().Next(),alist.Head());
    }
    //: Move the list after the current element.
    // Note alist should not be the list we're iterating.
    
    void Del() {
      RavlAssert(IsValid() && *this);
      DLinkC *next = &place->Prev();
      DListBodyC<DataT>::Delete(*place);
      place = next;
    }
    //: Remove current element from the list.
    // <p>This moves the iterator to the <i>previous</i> element.
    // NB: The iterator must be pointing to a valid element. </p>
    // Because it moves to the prevous element it makes it easy
    // to delete elements from a list in a for() loop. E.g. the
    // following will work correctly:</p>
    // <pre>
    // for(DLIterC<DataT> it(x);it;it++) 
    //   if(*it == 2)
    //     it.Del();</pre>
    // 
    
    void DelMoveNext() {
      RavlAssert(IsValid() && *this);
      DLinkC *next = &place->Next();
      DListBodyC<DataT>::Delete(*place);
      place = next;
    }
    //: Remove current element from the list.
    // This moves the iterator to the following element.
    // NB. The iterator must be pointing to a valid element.    
    
    bool operator==(const DLIterC<DataT> &oth) const
    { return place == oth.place; }
    //: Are these iterators equal ?
    // True if both iterators point to the same element
    // in the same list.  False otherwise.
    
    bool operator!=(const DLIterC<DataT> &oth) const
    { return place != oth.place; }
    //: Are these iterators unequal ?
    // True if the iterators point to different elements
    // in any lists.  False otherwise.

    bool IsElm() const
    { return place != &lst.Head(); }
    //!deprecated:
    //: Is iterator at a valid position ?
    // AMMA compatibility function, use cast to bool instead ie if(iter) {..}
    
    inline bool IsFirst() const
    { return &(lst.Head().Next()) == place; }
    //: Returns true if the current element is the first in the list.
    
    inline bool IsLast() const
    { return &(lst.Head().Prev()) == place; }
    //: Returns true if the current element is the last in the list.
    
    void Next()
    { (*this)++; }
    //: Go to next element.
    
    void Prev()
    { (*this)--; }
    //: Go to previous element.

    void NextCrc() { 
      (*this)++; 
      if(!IsElm()) (*this)++; 
    }
    //: Go to next element, circular
    // If the next element is the head of the list, loop
    // back to the begining of the list.
    
    void PrevCrc() { 
      (*this)--; 
      if(!IsElm()) (*this)--; 
    }
    //: Goto previous element, circular
    // If the previous element is the tail of the list, go
    // to the end of the list.
    
    DLIterC<DataT> &RelNth(IntT n) {
      if(n==0)
	return (*this);
      if (n > 0)
	while (n--) Next();
      else
	while (n++) Prev();
      return (*this);
    }
    //: Move to the n-th element from the current element. 
    // The index 'n' can be positive, zero, or negative. 
    // Thus n = 0 means no move,
    // n = 1 means the move to the next element, 
    // and n = -1 means the move to the previous element. <p>
    // Returns a reference to this iterator.
    
    DLIterC<DataT> &operator+=(IntT n)
    { return RelNth(n); }
    //: Short hand for RelNth(n).
    
    DLIterC<DataT> &operator-=(IntT n)
    { return RelNth(-n); }
    //: Short hand for RelNth(-n).
    
    DLIterC<DataT> operator+(IntT n) const { 
      DLIterC<DataT> it(*this); 
      it.RelNth(n); 
      return it;
    }
    //: Create an iterator positioned 'n' elements forward from this one.
    
    DLIterC<DataT> operator-(IntT n) const { 
      DLIterC<DataT> it(*this); 
      it.RelNth(-n); 
      return it;
    }
    //: Create an iterator positioned 'n' elements back from this one.
    
    DLIterC<DataT> &Nth(IntT n) {
      if(n >= 0) {
	First();
      } else {
	n += 1;
	Last();
      }
      return RelNth(n);
    }
    //: Sets to the n-th element of the list. 
    // The index 'n' can be negative. The first element of the list 
    // has the index 0, the last element has the index -1. It does 
    // not skip the head of the list.<p>
    // Returns a reference to this iterator.
    
    DataT &Data() { 
      RAVL_PARANOID(RavlAssertMsg(IsElm() && IsValid(),"Attempt to access data from invalid iterator."));
      return DLinkData().Data(); 
    }
    //: Access data 
    
    const DataT &Data() const { 
      RAVL_PARANOID(RavlAssertMsg(IsElm() && IsValid(),"Attempt to access data from invalid iterator."));
      return DLinkData().Data(); 
    }
    //: Constant access to data.
    
    DataT &operator*()
    { return Data(); }
    //: Access data.
    
    const DataT &operator*() const
    { return Data(); }
    //: Constant access to data.
    
    DataT *operator->()
    { return &Data(); }
    //: Access member function of data..
    
    const DataT *operator->() const
    { return &Data(); }
    //: Constant access to member function of data..

    DataT &NextData() { 
      RavlAssert(!IsLast());
      return static_cast<DLinkDataC<DataT> &>(place->Next()).Data(); 
    }
    //: Access data following this element.
    // The iterator must not be on the last element of the list.

    const DataT &NextData() const { 
      RavlAssert(!IsLast());
      return static_cast<const DLinkDataC<DataT> &>(place->Next()).Data(); 
    }
    //: Access data following this element.
    // The iterator must not be on the last element of the list.
    
    DataT &PrevData() { 
      RavlAssert(!IsFirst());
      return static_cast<DLinkDataC<DataT> &>(place->Prev()).Data(); 
    }
    //: Access data before this element.
    // The iterator must not be on the first element in the list.

    const DataT &PrevData() const { 
      RavlAssert(!IsFirst());
      return static_cast<const DLinkDataC<DataT> &>(place->Prev()).Data(); 
    }
    //: Access data before this element.
    // The iterator must not be on the first element in the list.
    
    DataT &NextCrcData() { 
      if(IsLast())
	return lst.First();
      return NextData();
    }
    //: Access data following this element, circular
    // If the iterator is on the last element of the list, get the first element.
    
    const DataT &NextCrcData() const { 
      if(IsLast())
	return lst.First();
      return NextData();
    }
    //: Access data following this element, circular
    // If the iterator is on the last element of the list, get the first element.
    
    DataT &PrevCrcData() { 
      if(IsFirst())
	return lst.Last();
      return PrevData();
    }
    //: Access data before this element, circular
    // If the iterator is on the first element of the list, get the last element.
    
    const DataT &PrevCrcData() const { 
      if(IsFirst())
	return lst.Last();
      return PrevData();
    }
    //: Access data before this element, circular
    // If the iterator is on the first element of the list, get the last element.
    
    DListC<DataT> Tail() {
      DListC<DataT> ret;
      ret.Body().Head().CutPaste(place->Next(),lst.Body().Head());
      return ret;
    }
    //: Clip out the tail of the list.
    // Return all of the elements after this element in the list.
    
    DListC<DataT> Head() {
      DListC<DataT> ret;
      //if(place != &(lst.Body().FirstLink()))
      ret.Body().Head().CutPaste(lst.Body().FirstLink(),*place);
      return ret;
    }
    //: Clip out the head of the list.
    // Return all of the elements before this element in the list.
    
    DListC<DataT> InclusiveTail() {
      DListC<DataT> ret;
      RavlAssert(IsElm()); // Must be on a valid element.
      DLinkC *nv = &place->Prev();
      ret.Body().Head().CutPaste(*place,lst.Body().Head());
      place = nv;
      return ret;
    }
    //: Clip out the tail of the list including this element.
    // Returns this element and the following elements
    // from the list. The interator is left point to
    // the last element in the remaining list.
    
    DListC<DataT> InclusiveHead() {
      DListC<DataT> ret;
      RavlAssert(IsElm()); // Must be on a valid element.
      DLinkC *nv = &place->Next();
      ret.Body().Head().CutPaste(lst.Body().FirstLink(),*nv);
      place = nv;
      return ret;
    }
    //: Clip out the head of the list including this element.
    // Returns this element and the following elements
    // from the list. The interator is left point to
    // the last element in the remaining list.
    
    DListC<DataT> Clip(const DLIterC<DataT> &end) {
      DListC<DataT> ret;
      RavlAssertMsg(&end.List().Body() == &lst.Body(),"Iterators must be from the same list.");
      DLinkC *nv = &place->Prev();
      ret.Body().Head().CutPaste(*place,*end.place);
      place = nv;
      return ret;
    }
    //: Clip out section of the list
    // Returns the elements starting from this one to the element before
    // the end. This iterator is moved the previous element. <p>
    // NB. It is the user's responsibility to ensure 'end' is an 
    // element following this element in the same list.
    
  protected:
    DLIterC(const DListBodyC<DataT> &nlst)
      : lst(const_cast<DListBodyC<DataT> &>(nlst)) // A bit evil, but it make life alot easier.
    { First(); }
    //: Construct from a list body.
    
    DLinkDataC<DataT> &DLinkData()
    { return static_cast<DLinkDataC<DataT> &>(*place); }
    //: Access as data element.
    
    const DLinkDataC<DataT> &DLinkData() const
    { return static_cast<const DLinkDataC<DataT> &>(*place); }
    //: Access as data element.
    
    DLinkDataC<DataT> &Extract() {
      RavlAssert(*this); // Have to be at a valid element.
      DLinkDataC<DataT> &cur = DLinkData();
      place->Unlink();
      place = &cur.Prev();
      return cur;
    }
    //: Extract the current element, move the iterator back 1.

    DLinkDataC<DataT> &ExtractNext() {
      RavlAssert(*this); // Have to be at a valid element.
      DLinkDataC<DataT> &cur = DLinkData();
      place->Unlink();
      place = &cur.Next();
      return cur;
    }
    //: Extract the current element, move the iterator to the next element in the list.
    
    DListC<DataT> lst;
    DLinkC *place;
    
    friend class DListBodyC<DataT>;
  };

}

#endif
