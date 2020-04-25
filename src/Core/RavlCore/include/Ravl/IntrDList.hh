// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_INTRDLIST_HEADER
#define RAVL_INTRDLIST_HEADER 1
//////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Core.Lists"
//! userlevel=Advanced
//! file="Ravl/Core/Container/DList/IntrDList.hh"
//! lib=RavlCore
//! author="Radek Marik, Charles Galambos"

// IntrDListC is based on code written by Radek Marik.

#include "Ravl/DLink.hh"
#include "Ravl/Assert.hh"
#include "Ravl/Types.hh"

namespace RavlN {

  
  //! userlevel=Advanced
  //: Default class for handling list element derefrence's
  // This allows IntrDListC to use link elements that are class
  // members.  The default is to use the DLinkC element from the
  // inheritence tree.
  
  template<typename DataT>
  class IntrDListDefaultDeRefC {
  public:
    IntrDListDefaultDeRefC()
    {}
    
    DataT &operator()(DLinkC &link) const
    { return static_cast<DataT &>(link); }
    
    const DataT &operator()(const DLinkC &link) const
    { return static_cast<const DataT &>(link); }
    
    DLinkC &Ref(DataT &link) const
    { return static_cast<DLinkC &>(link); }
    
    const DLinkC &Ref(const DataT &link) const
    { return static_cast<const DLinkC &>(link); }
    
    
  };

  template <class DataT,typename DeRefT = IntrDListDefaultDeRefC<DataT> > class IntrDListC;
  template <class DataT,typename DeRefT = IntrDListDefaultDeRefC<DataT> > class IntrDLIterC;
  template <class DataT,typename DeRefT>
  std::ostream & operator<<(std::ostream & s, const IntrDListC<DataT,DeRefT> & list);

  //! userlevel=Advanced
  //: Default class for handling list element derefrence's
  // This allows IntrDListC to use link elements that are class
  // members.  The default is to use the DLinkC element from the
  // inheritence tree.
  
  template<typename DataT,int offset>
  class IntrDListOffsetDeRefC {
  public:
    DataT &operator()(DLinkC &link) const
    { return *reinterpret_cast<DataT *>(reinterpret_cast<char *>(&link) - offset); }
    
    const DataT &operator()(const DLinkC &link) const
    { return *reinterpret_cast<const DataT *>(reinterpret_cast<const char *>(&link) - offset); }
    
    DLinkC &Ref(DataT &link) const
    { return *reinterpret_cast<DLinkC *>(reinterpret_cast<char *>(&link) + offset); }
    
    const DLinkC &Ref(const DataT &link) const
    { return *reinterpret_cast<const DLinkC *>(reinterpret_cast<char *>(&link) + offset); }

  };
  
  
  //! userlevel=Advanced
  //: Double-linked circular list
  // The IntrDListC class represents intrusive double-linked list of elements. 
  // These elements must be derived from the class DLinkC.
  // The list contains a head element and a chain of
  // elements, so the empty list contains just its head element.
  // Because of efficiency references to elements of a list are not
  // checked if they are proper elements of a list or its head.
  // The class serves as a base class for more complex dynamic structures
  // as graphs. <p>
  // NB. This is a SMALL object.
  
  template <typename DataT,typename DeRefT> // default DeRefT = IntrDListDefaultDeRefC<DataT>
  class IntrDListC
    : public DLinkHeadC
  {
  public:
    inline IntrDListC(bool ndeleteEntries = true,const DeRefT &deRefInit = DeRefT())
      : deleteEntries(ndeleteEntries),
        m_deRef(deRefInit)
    {}
    //: Construct an empty list.
    
    IntrDListC(const IntrDListC<DataT,DeRefT> &oth);
    //: Copy constructor.
    // This makes a copy of each entry in the list. <p>
    // NB. This will only work on intrinsic lists which 
    // manage the destruction of there own entries. i.e.
    // deleteEntries is true.
    
    const IntrDListC<DataT,DeRefT> &operator=(const IntrDListC<DataT,DeRefT> &oth);
    //: Assignment.
    // The contents of this list are replaced by a copy of the contents 
    // of 'oth'.
    // NB. This will only work on intrinsic lists which 
    // manage the destruction of there own entries. i.e.
    // deleteEntries is true.
    
    inline ~IntrDListC() 
    { Empty(); }
    // Destructor, each list element is destroyed.
    
    void Empty() {
      if(deleteEntries) {
	while(&head.Next() != &head)
	  IntrDListC<DataT,DeRefT>::Delete(head.Next());
      } else {
	// Unlink all items in the list to stop
	// the unlink destructors doing anything silly.
	DLinkC *place = &head.Next();
	while(place != &head) {
	  DLinkC *np = &place->Next();
	  place->SetSelfPointing();
	  place = np;
	}
      }
    }
    //: Empty the list of all contents
    
    //---------- Access to the elements -----------------------------
    
    DataT &First()
    { return m_deRef(head.Next()); }
    //: Get first link in list.
    
    DataT &Last()
    { return m_deRef(head.Prev()); }
    //: Get the last ilink in the list.
    
    const DataT &First() const
    { return m_deRef(head.Next()); }
    //: Get first link in list.
    
    const DataT &Last() const
    { return m_deRef(head.Prev()); }
    //: Get the last ilink in the list.
    
    //---------- Elementary changes in the list -------------
    
    void InsFirst(DataT &dat)
    { DLinkHeadC::InsFirst(m_deRef.Ref(dat)); }
    //: Push element onto the begining of the list.
    
    void InsLast(DataT &dat)
    { DLinkHeadC::InsLast(m_deRef.Ref(dat)); }
    //: Push element onto the end of the list.
    
    DataT &PopFirst()  { 
      RavlAssert(!IsEmpty());
      DataT &tmp = First();
      head.Next().Remove();
      return tmp;
    }
    //: Pop item off front of list and set it to self pointing.
    // It is up to the user to ensure the object is deleted.
    
    DataT &PopLast()  { 
      RavlAssert(!IsEmpty());
      DataT &tmp = Last();
      head.Prev().Remove();
      return tmp;
    }
    //: Pop item off back of list and set it to self pointing.
    // It is up to the user to ensure the object is deleted.

    void DelFirst()  { 
      RavlAssert(!IsEmpty());
      IntrDListC<DataT,DeRefT>::Delete(head.Next());
    }
    //: Pop item off front of list.
    // It is up to the user to ensure the object is deleted.
    
    void DelLast()  { 
      RavlAssert(!IsEmpty());
      IntrDListC<DataT,DeRefT>::Delete(head.Prev().Unlink());
    }
    //: Pop item off back of list.
    // It is up to the user to ensure the object is deleted.

    
    typedef IntrDLIterC<DataT,DeRefT> IteratorT;
    //: Type def to the appropriate iterator.
  protected:
    
    void Delete(DLinkC &elm) 
    { delete &m_deRef(elm); }
    //: Unlink and delete an element from the list.
    
    DLinkC &Head()
    { return DLinkHeadC::Head(); }
    //: Get head of list.
  
    const DLinkC &Head() const
    { return DLinkHeadC::Head(); }
    //: Get head of list.
    
    bool deleteEntries; // Delete entries when finished with them ?
    DeRefT m_deRef;
    friend class IntrDLIterC<DataT,DeRefT>;
  };

}

#include "Ravl/IntrDLIter.hh"

namespace RavlN {  
  
  template <class DataT,typename DeRefT>
  IntrDListC<DataT,DeRefT>::IntrDListC(const IntrDListC<DataT,DeRefT> &oth)
    : deleteEntries(oth.deleteEntries),
      m_deRef(oth.m_deRef)
  {
    RavlAssert(deleteEntries); // A way to check this makes sense.
    for(IntrDLIterC<DataT,DeRefT> it(oth);it;it++)
      InsLast(*new DataT(*it));
  }
  
  template <class DataT,typename DeRefT>
  const IntrDListC<DataT,DeRefT> &IntrDListC<DataT,DeRefT>::operator=(const IntrDListC<DataT,DeRefT> &oth) {
    Empty();
    deleteEntries = oth.deleteEntries;
    m_deRef = oth.m_deRef;
    RavlAssert(deleteEntries); // A way to check this makes sense.
    for(IntrDLIterC<DataT,DeRefT> it(oth);it;it++)
      InsLast(*new DataT(*it));
    return *this;
  }

}



#endif
