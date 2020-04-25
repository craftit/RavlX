// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_PRIQUEUEL_HEADER
#define RAVL_PRIQUEUEL_HEADER 1
/////////////////////////////////////////////////////////////////
//! author="Charles Galambos"
//! lib=RavlCore
//! userlevel=Normal
//! example=testPriQ.cc
//! date="28/10/1996"
//! file="Ravl/Core/Container/Queue/PriQueueL.hh"
//! docentry="Ravl.API.Core.Queues"
//! rcsid="$Id$"

#include "Ravl/HeapNode.hh"
#include "Ravl/RefCounter.hh"
#include "Ravl/Assert.hh"

#define RAVL_PRIQUEUEL_DEBUG 0

namespace RavlN {

  //! userlevel=Develop
  //: Pointer based version of Priority Queue.  
  
  template <class KeyT,class DataT>
  class PriQueueLBodyC
    : public RCBodyC
  {
  public:
    PriQueueLBodyC()
      : root(0)
    {}
    //: Default constructor.
    
    ~PriQueueLBodyC()
    { Empty(); }
    //: Destructor.
    
    PriQueueLBodyC & operator=(const PriQueueLBodyC<KeyT,DataT> &Oth);
    //: Assignment operator
    
    bool IsElm() const 
    { return (root != 0); }
    //: Does the queue contains any items ?
    
    bool IsEmpty() const 
    { return (root == 0); }
    //: Is the queue empty ?
    
    DataT &Top() 
    { return root->Data(); }
    //: Look/Modify data on top of queue.
  
    const DataT &Top() const 
    { return root->Data(); }
    //: Look at data on top of queue.
    
    const KeyT &TopKey() const 
    { return root->Key(); }
    //: Look at key on top of queue.
    
    void DelTop() 
    { delete &GetTopPair(); }
    //: Delete item on top of queue.
    // NB. IsElm(), must be true before calling this.
    
    Tuple2C<KeyT,DataT> &GetTopPair();
    //: Get Key/Data pair from queue.
    // WARNING: For advanced users only.
    // !! Users responsibility to delete returned info after use. !!
    
    DataT GetTop();
    //: Get Data from top of queue.
    
    void Insert(const KeyT &Key,const DataT &Data);
    //: Insert Data/Key into queue.
    
    void Insert(Tuple2C<KeyT,DataT> &New);
    //: Insert Data/Key into queue.
    
    bool Remove(const Tuple2C<KeyT,DataT> &New);
    //: Remove all instances of Key from queue.
    //!bug: NOT IMPLEMENTED
    // Returns True if found.
    
    bool Remove(const KeyT &Key);
    //: Remove all instances of Key from queue.
    //!bug: NOT IMPLEMENTED
    // Returns True if found.
    
    SizeT Size() const ;
    //: Get number of items in queue. Slow !!!
    
#if RAVL_PRIQUEUEL_DEBUG  
    void Dump(HeapNodeC<KeyT,DataT> *Place = 0,int level = 0);
    //: Dump to stdout
#endif
    
    void Empty();
    //: Empty the queue of all its contents.
    
  protected:
    HeapNodeC<KeyT,DataT> *root;
    
  };
  
  //! userlevel=Normal
  //: Pointer based version of Priority Queue.  
  // It is faster to use plain PriQueueC if  you know the 
  // maximum size of the queue before its constructed.
  // <p><pre>
  // Notes:
  //   This is a BIG OBJECT.
  //   Keys must have operation '<' defined.
  //   This queue assumes: **** Small numbers == High priority. ****
  // </pre>
  // The order in which items of equal priority are retrieved should
  // be treated as 'unknown'. 
  
  template <class KeyT,class DataT>
  class PriQueueLC
    : public RCHandleC<PriQueueLBodyC<KeyT,DataT> >
  {
  public:
    PriQueueLC()
      : RCHandleC<PriQueueLBodyC<KeyT,DataT> >(*new PriQueueLBodyC<KeyT,DataT>())
    {}
    //: Default constructor.
    
    bool IsElm() const 
    { return Body().IsElm(); }
    //: Does the queue contains any items ?
    
    bool IsEmpty() const 
    { return Body().IsEmpty(); }
    //: Is the queue empty ?
  
    DataT &Top() 
    { return Body().Top(); }
    //: Look/Modify data on top of queue.
  
    const DataT &Top() const 
    { return Body().Top(); }
    //: Look at data on top of queue.
    
    const KeyT &TopKey() const 
    { return Body().TopKey(); }
    //: Look at key on top of queue.
    
    void DelTop() 
    { Body().DelTop(); }
    //: Delete item on top of queue.
    // NB. IsElm(), must be true before calling this.
    
    Tuple2C<KeyT,DataT> &GetTopPair()
    { return Body().GetTopPair(); }
    //: Get Key/Data pair from queue.
    // WARNING: For advanced users only.
    // !! Users responsibility to delete returned info after use. !!
    
    DataT GetTop()
    { return Body().GetTop(); }
    //: Get Data from top of queue.
    
    void Insert(const KeyT &key,const DataT &data)
    { Body().Insert(key,data); }
    //: Insert Data/Key into queue.
    
    void Insert(Tuple2C<KeyT,DataT> &newun)
    { Body().Insert(newun); }
    //: Insert Data/Key into queue.
    
    bool Remove(const Tuple2C<KeyT,DataT> &old)
    { return Body().Remove(old); }
    //: Remove all instances of Key from queue.
    //!bug: NOT IMPLEMENTED
    // Returns True if found.
    
    bool Remove(const KeyT &key)
    { return Body().Remove(key); }
    //: Remove all instances of Key from queue.
    //!bug: NOT IMPLEMENTED
    // Returns True if found.
    
    int Size() const 
    { return Body().Size(); }
    //: Get number of items in queue. Slow !!!
    
#if RAVL_PRIQUEUEL_DEBUG  
    void Dump(HeapNodeC<KeyT,DataT> *Place = 0,int level = 0)
    { Body().Dump(Place,level); }
    //: Dump to stdout
#endif
    
    void Empty()
    { Body().Empty(); }
    //: Empty the queue of all its contents.

  protected:
    PriQueueLBodyC<KeyT,DataT> &Body()
    { return RCHandleC<PriQueueLBodyC<KeyT,DataT> >::Body(); }
    //: Access body
    
    const PriQueueLBodyC<KeyT,DataT> &Body() const
    { return RCHandleC<PriQueueLBodyC<KeyT,DataT> >::Body(); }
    //: Access body
    
  };
  
  
  template <class KeyT,class DataT>
  void PriQueueLBodyC<KeyT,DataT>::Empty() {
    if(root != 0) {
      delete root;
      root = 0;
    }
  }
  
  template <class KeyT,class DataT>
  Tuple2C<KeyT,DataT> &PriQueueLBodyC<KeyT,DataT>::GetTopPair() {
    Tuple2C<KeyT,DataT> &TheTop = root->GetPair();
    root->SetPairNULL();
    HeapNodeC<KeyT,DataT> *Place = root,**LastPtr = &root;
    for(;;) {
      HeapNodeC<KeyT,DataT> *Child0 =  Place->Child(0);
      HeapNodeC<KeyT,DataT> *Child1 =  Place->Child(1);
      if(Child0 == 0) {
	if(Child1 == 0) {
	  (*LastPtr) = 0;
	  Place->SetPairNULL();
	  delete Place;
	  break;
	}
	Place->SetPair(Child1->GetPair());
	Place->DeltaBalance(-1);
	LastPtr = &Place->Child(1);
	Place = Child1;
	continue;
      }
      if(Child1 == 0) {
	Place->SetPair(Child0->GetPair());
	Place->DeltaBalance(1);
	LastPtr = &Place->Child(0);
	Place = Child0;
	continue;
      }
      if(Child1->Key() < Child0->Key()) {
	Place->SetPair(Child1->GetPair());
	Place->DeltaBalance(-1);
	LastPtr = &Place->Child(1);
	Place = Child1;
      } else {
	Place->SetPair(Child0->GetPair());
	Place->DeltaBalance(1);
	LastPtr = &Place->Child(0);
	Place = Child0;
      }
    }
    return TheTop;
  }
  
  template <class KeyT,class DataT>
  DataT PriQueueLBodyC<KeyT,DataT>::GetTop() {
    Tuple2C<KeyT,DataT> &Pair = GetTopPair();
    DataT Ret = Pair.Data2();
    delete &Pair;
    return Ret;
  }
  
  template <class KeyT,class DataT>
  void PriQueueLBodyC<KeyT,DataT>::Insert(Tuple2C<KeyT,DataT> &newElem) {
    if(root == 0) {
      root = new HeapNodeC<KeyT,DataT> (newElem);
      return ;
    }
    HeapNodeC<KeyT,DataT> *place = root;
    Tuple2C<KeyT,DataT> *tmp,*hold = &newElem;
    const KeyT &key = newElem.Data1();
    for(;;) {
      RavlAssert(place != 0);
      if(key < place->Key()) { // Swap data.
	tmp = &place->GetPair();
	place->SetPair(*hold);
	hold = tmp;
      }
      if(place->Child(0) == 0) {
	place->Child(0) = new HeapNodeC<KeyT,DataT> (*hold);
	place->DeltaBalance(-1);
	break;
      }
      if(place->Child(1) == 0) {
	place->Child(1) = new HeapNodeC<KeyT,DataT> (*hold);
	place->DeltaBalance(1);
	break;
      }
      if(place->GetBalance() < 0) {
	place->DeltaBalance(1);
	place = place->Child(1);
      } else {
	place->DeltaBalance(-1);
	place = place->Child(0);
      }
    }
  }
  
  ///////////////////////////
  // Insert Data/Key into queue.
  
  template <class KeyT,class DataT>
  void PriQueueLBodyC<KeyT,DataT>::Insert(const KeyT &nKey,const DataT &nData) {
    Tuple2C<KeyT,DataT> &newun = *(new Tuple2C<KeyT,DataT> (nKey,nData));
    Insert(newun);
  }
  
  //////////////////////////////////
  // Remove all instances of Key from queue.
  // Returns True if found.
  
  template <class KeyT,class DataT>
  bool PriQueueLBodyC<KeyT,DataT>::Remove(const KeyT &Key) {
    RavlAssert(0); // NOT IMPLEMENTED.
    return false;
  }
  
  ////////////////////////////////////
  // Remove all instances of Key from queue.
  // Returns True if found.
  
  template <class KeyT,class DataT>
  bool PriQueueLBodyC<KeyT,DataT>::Remove(const Tuple2C<KeyT,DataT> &newun) {
    return Remove(newun.Data1());
  }
  
  
#if RAVL_PRIQUEUEL_DEBUG  
  ////////////////////
  // Dump to stdout
  
  template <class KeyT,class DataT>
  void PriQueueLBodyC<KeyT,DataT>::Dump(HeapNodeC<KeyT,DataT> *Place,int level) {
    bool Theroot = false;
    if(Place == 0) {
      Place = root;
      Theroot = true;
    }
    for(int i = 0;i < level;i++)
      std::cout << "  ";
    std::cout << Place->Key() << "\n";
    //printf("(%p [%p<%d>%p]) ",Place,Place->Child(0),Place->GetBalance(),Place->Child(1));
    if(Place->Child(0) != 0)
      Dump(Place->Child(0),level+1);
    if(Place->Child(1) != 0)
      Dump(Place->Child(1),level+1);
    if(Theroot)
      printf("\n");
  }
#endif
  
  //////////////////////////
  // Get number of items in queue, slow !
  
  template <class KeyT,class DataT>
  SizeT PriQueueLBodyC<KeyT,DataT>::Size() const {
    if(root == 0)
      return 0;
    return root->Size();
  }

}
#endif
