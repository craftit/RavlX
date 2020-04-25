// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_INDLITER_HEADER
#define RAVL_INDLITER_HEADER
////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Core.Lists"
//! file="Ravl/Core/Container/DList/IntrDLIter.hh"
//! lib=RavlCore
//! userlevel=Advanced
//! author="Radek Marik, Charles Galambos"
//! date="24/01/2001"

// IntrDListC is based on code written by Radek Marik.

#include "Ravl/IntrDList.hh"

namespace RavlN {
  
  //: Intrinsic list iterator.
  // See IntrDListC for more details.
  // FIXME :- Most of this need not be templated, should make a base class?
  //  = IntrDListDefaultDeRefC<DataT>
  template <class DataT,typename DeRefT> // default DeRefT = IntrDListDefaultDeRefC<DataT>
  class IntrDLIterC {
  public:
    IntrDLIterC()
      : head(0),
        place(0)
    {}
    //: Default constructor.
    // Creates an invalid iterator.
    
    IntrDLIterC(const IntrDListC<DataT,DeRefT> &lst)
      : head(const_cast<IntrDListC<DataT,DeRefT> *>(&lst)), // Nasty but...
        place(const_cast<DLinkC *>(&lst.Head().Next())),
        m_deRef(lst.m_deRef)
    {}
    //: Constructor.
    // Constructs an iterator pointing to the 
    // first element in the list.
    
    bool IsValid() const
    { return head != 0; }
    //: Is iterator valid ?
    // true is returned for a valid iterator.

    void First() {
      RavlAssert(IsValid());
      place = &head->Head().Next();
    }
    //: Goto first element in the list.
    
    void Last() {
      RavlAssert(IsValid());
      place = &head->Head().Prev();
    }
    //: Goto the last element in the list.
    
    operator bool() const
    { return place != &Head(); }
    //: At a valid node ?
    
    bool IsElm() const
    { return place != &Head(); }
    //: At a valid element in the list ?
    // AMMA compatibility function, use operator bool() instead.

    bool IsFirst() const
    { return place != &Head().Next(); }
    //: At the first element in the list ?
    // AMMA compatibility function.
    
    bool IsLast() const
    { return place != &Head().Next(); }
    //: At the last element in the list ?
    // AMMA compatibility function.

    void operator++()
    { place = &place->Next(); }
    //: Goto next element in list.
    
    void operator++(int)
    { place = &place->Next(); }
    //: Goto next element in list.
    
    void operator--(int)
    { place = &place->Prev(); }
    //: Goto previous element in list.
    
    void Next() 
    { (*this)++; }
    //: Goto next element in list.
    // AMMA compatibility function, use operator++() instead.

    void Prev() 
    { (*this)--; }
    //: Goto previous element in list.
    // AMMA compatibility function, use operator--() instead.

    void NextCrc() { 
      (*this)++; 
      if(!(*this)) (*this)++; 
    }
    //: Goto next element in list, skip the head element.
    // AMMA compatibility function.
    
    void PrevCrc() { 
      (*this)--; 
      if(!(*this)) (*this)--; 
    }
    //: Goto previous element in list, skip the head element.
    // AMMA compatibility function.
    
    DataT &operator*()
    { return m_deRef(*place); }
    //: Access data.
    // NB. The iterator MUST be valid. check with 'operator bool()'.

    const DataT &operator*() const
    { return m_deRef(*place); }
    //:Constant access to data.
    // NB. The iterator MUST be valid. check with 'operator bool()'.

    DataT *operator->()
    { return &m_deRef(*place); }
    //: Access data.
    // NB. The iterator MUST be valid. check with 'operator bool()'.

    const DataT *operator->() const
    { return &m_deRef(*place); }
    //:Constant access to data.
    // NB. The iterator MUST be valid. check with 'operator bool()'.

    DataT &Data()
    { return m_deRef(*place); }
    //: Access data.
    // NB. The iterator MUST be valid. check with 'operator bool()'.
    
    const DataT &Data() const
    { return m_deRef(*place); }
    //:Constant access to data.
    // NB. The iterator MUST be valid. check with 'operator bool()'.
    
    DataT &Extract() {
      RavlAssert(IsValid() && IsElm());
      DLinkC* ret = place;
      place = &place->Prev();
      ret->Unlink();
      return m_deRef(*ret);
    }
    //: Extract the current element from the list. 
    // Do not delete it, the user is responsible for ensuring this happens
    // at an appropriate time. The iteration is left pointing at
    // the element before the one extracted.
    // The iterator must be valid.
    
    void Del() { 
      if(static_cast<IntrDListC<DataT,DeRefT> &>(*head).deleteEntries)
	delete &Extract(); 
      else
	Extract();
    }
    //: Delete the current element from the list.
    // Actually delete the element where appropriate.
    // The iterator must be valid.

    void InsertBef(DataT &dat)
    { place->LinkBef(m_deRef.Ref(dat)); }
    //: Insert data before current element.
    // if at the head of the list  (i.e. operator bool() fails.)
    // then add at end.
    
    void InsertAft(DataT &dat)
    { place->LinkAft(m_deRef.Ref(dat)); }
    //: Insert data after current element.
    // if at the head of the list  (i.e. operator bool() fails.)
    // then add at beginning.
    
    IntrDListC<DataT,DeRefT> &List()
    { return static_cast<IntrDListC<DataT,DeRefT> &>(*head); }
    //: Access the list we're iterating.
    
    const IntrDListC<DataT,DeRefT> &List() const
    { return static_cast<IntrDListC<DataT,DeRefT> &>(*head); }
    //: Access the list we're iterating.
    
    bool Invalidate()
    { place = &Head(); return true; }
    //: Point iterator at an invalid element.
    
  protected:
    
    IntrDLIterC(const IntrDListC<DataT,DeRefT> &lst,DataT &nplace)
      : head(const_cast<IntrDListC<DataT,DeRefT> *>(&lst)), // Nasty but...
        place(&nplace)
    {}
    //: Constructor.
    // Constructs an iterator pointing to the the element 'place'.
    // The user MUST ensure that 'place' is in the given list.

    void SetIter(const IntrDListC<DataT,DeRefT> &lst,DataT &nplace)  {
      head = const_cast<IntrDListC<DataT,DeRefT> *>(&lst);
      place = &nplace;
    }
    //: Set iterator to new value.
    
    DLinkC &Head()
    { return List().Head(); }
    //: Get head of list.
  
    const DLinkC &Head() const
    { return List().Head(); }
    //: Get head of list.
    
    
    DLinkHeadC *head;
    DLinkC *place;
    DeRefT m_deRef;
    
    friend class IntrDListC<DataT,DeRefT>;
  };

}

#endif
