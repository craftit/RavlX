// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FIXQUEUE_HEADER
#define RAVL_FIXQUEUE_HEADER 1
//////////////////////////////////////////////////////////////
//! file="Ravl/Core/Container/Queue/FixedQueue.hh"
//! lib=RavlCore
//! userlevel=Normal
//! author="Charles Galambos"
//! date="6/6/1997"
//! docentry="Ravl.API.Core.Queues"

#include "Ravl/SArray1d.hh"
#include "Ravl/Assert.hh"

namespace RavlN {
  template<class T> class FixedQueueIterC;
  template<class T> class FixedQueueRevIterC;
  
  //! userlevel=Normal
  //: Fixed size circular queue.
  // A SMALL object.
  // This is sometimes known as a ring buffer.
  // FIXME :- Make FixedQueueC big. <p>
  // Note, this does not destroy items removed from
  // the queue until the element is overwritten by another.
  
  template<class T>
  class FixedQueueC 
    : protected SArray1dC<T> 
  {
  public:
    inline FixedQueueC();
    //: Construct an empty queue, queue must be resized before use.

    explicit inline FixedQueueC(SizeT Size);
    //: Constructor.
    
    void Resize(SizeT newSize);
    //: Resize the queue.
    // The new size must be larger than the number of used elements.

    inline bool IsSpace() const;
    //: Is there space to add an item into ring ?
    
    inline void InsLast(const T &Obj);
    //: Insert data at end of queue.
    // There must be space in the queue.
    
    inline void ForceInsLast(const T &Obj);
    //: Insert data at end of queue, if no space discard oldest element.
    
    inline void ShiftInsLast(const T &Obj);
    //: Insert new data, and delete oldest data from the queue
    
    inline bool IsEmpty() const 
    { return head == tail; }
    //: Is ring empty ?
    
    inline T GetFirst();
    //: Get first item from queue.

    inline const T &First() const { 
      RavlAssert(!IsEmpty());
      return *tail; 
    }
    //: Access first the first element in the queue.

    inline T &First() { 
      RavlAssert(!IsEmpty());
      return *tail; 
    }
    //: Access first the first element in the queue.
    
    inline T &Last() { 
      RavlAssert(!IsEmpty());
      T *l = head - 1;
      if(l < ArrayStart())
	l = eoa-1;
      return *l; 
    }
    //: Look at the item most recently placed in the queue.

    inline const T &Last() const { 
      RavlAssert(!IsEmpty());
      T *l = head - 1;
      if(l < ArrayStart())
	l = eoa-1;
      return *l; 
    }
    //: Look at the item most recently placed in the queue.
    
    inline void DelFirst();
    //: Delete first item in buffer 
    
    inline SizeT Size() const;
    //: No of items in the ring.

    inline SizeT MaxSize() const
    {
      if(SArray1dC<T>::Size() == 0) return 0;
      return SArray1dC<T>::Size()-1;
    }
    //: Get maximum size of queue.
    
    inline bool IsInRing(UIntT P) const;
    //: Is P an index in the ring.
    
    inline void Empty();
    //: Empty the queue of all contents. 
    // Reset to head=0, tail=0
    
    T &operator[](IndexC i) {
      RavlAssert(i < this->Size());
      T *elem = &(tail[i.V()]);
      if(elem >= eoa)
	elem -= SArray1dC<T>::Size();
      return *elem;
    }
    //: Access element in the ring from back from the first entry. 
    // 0 is first element in the queue (as accessed by First()), 1-second most etc..
    // Is is up the the user to ensure that no attempt is made to access beyond the last element.
    
    const T &operator[](IndexC i) const {
      RavlAssert(i < this->Size());
      T *elem = &(tail[i.V()]);
      if(elem >= eoa)
	elem -= SArray1dC<T>::Size();
      return *elem;
    }
    //: Access element in the ring from back from the first entry. 
    // 0 is first element in the queue (as accessed by First()), 1-second most etc..
    // Is is up the the user to ensure that no attempt is made to access beyond the last element.
    
    const T &IndexBackward(IndexC i) const {
      RavlAssert(i < this->Size());
      const T *l = &head[-i.V()-1];
      if(l < ArrayStart())
        l += SArray1dC<T>::Size();
      return *l;
    }
    // Access queue from most recent item backward.
    // 0=Last inserted, 1=one before that, and so on.
    // Is is up the the user to ensure that no attempt is made to access beyond the last element.

  protected:
    T *ArrayStart()
    { return this->DataStart(); }
    //: Get the start of the array.
    
    const T *ArrayStart() const
    { return this->DataStart(); }
    //: Get the start of the array.

    T *head; // Next free location.
    T *tail; // Last used location.
    T *eoa;  // Ptr to end of array.

    friend class FixedQueueIterC<T>;
    friend class FixedQueueRevIterC<T>;
  };
  
  //! userlevel=Normal
  //: Iterate through contents of the queue.
  // This goes from the oldest element in the list to the latest. <p>
  
  template<class T>
  class FixedQueueIterC
    : public SArray1dC<T>
  {
  public:
    FixedQueueIterC()
      : at(0),
	end(0),
	eoa(0)
    {}
    //: Default constructor.
    
    FixedQueueIterC(FixedQueueC<T> &queue)
      : SArray1dC<T>(queue)
    { First(queue); }
    //: Constructor from a queue.
    // Note: Changing the queue after the iterator is constructed
    // will not affect the indices iterated, though the data will
    // change.
    
    void First(FixedQueueC<T> &queue) {
      SArray1dC<T>::operator=(queue);
      at = queue.tail;
      end = queue.head;
      eoa = queue.eoa;
    }
    //: Goto first element in queue.
    
    const FixedQueueIterC<T> &operator=(FixedQueueC<T> &queue) {
      First(queue);
      return *this;
    }
    //: Assign to a queue.
    
    bool IsElm() const
    { return at != end; }
    //: At valid element ?
    
    operator bool() const
    { return at != end; }
    //: At a valid element ?
    
    void Next() {
      at++;
      if(at == eoa)
	at = ArrayStart();
    }
    //: Goto next element.
    
    void operator++(int)
    { Next(); }
    //: Goto next element.
    
    void operator++()
    { Next(); }
    //: Goto next element.
    
    T &Data() 
    { return *at; }
    //: Access data.
    
    const T &Data() const
    { return *at; }
    //: Access data.
    
    T &operator*()
    { return *at; }
    //: Access data.

    const T &operator*() const
    { return *at; }
    //: Access data.
    
    T *operator->()
    { return at; }
    //: Access data.

    const T *operator->() const
    { return at; }
    //: Access data.
    
  protected:
    T *ArrayStart()
    { return &SArray1dC<T>::operator[](0); }
    //: Get the start of the array.
    
    const T *ArrayStart() const
    { return &SArray1dC<T>::operator[](0); }
    //: Get the start of the array.
    
    T *at;
    T *end;
    T *eoa;
  };

  //-----------------------------------------------------
  //! userlevel=Normal
  //: Iterate through contents of the queue.
  // This goes from the latest element in the list to the oldest.
  
  template<class T>
  class FixedQueueRevIterC
    : public SArray1dC<T>
  {
  public:
    FixedQueueRevIterC()
      : at(0),
	end(0),
	eoa(0)
    {}
    //: Default constructor.
    
    FixedQueueRevIterC(FixedQueueC<T> &queue)
      : SArray1dC<T>(queue)
    { First(queue); }
    //: Constructor from a queue.
    // Note: Changing the queue after the iterator is constructed
    // will not affect the indices iterated, though the data will
    // change.
    
    void First(FixedQueueC<T> &queue) {
      SArray1dC<T>::operator=(queue);
      at = queue.head;
      end = queue.tail -1;
      eoa = ArrayStart() - 1;
      if(end == eoa)
	end = &(ArrayStart()[this->Size()-1]); 
      Next();
    }
    //: Goto first element in queue.
    
    const FixedQueueIterC<T> &operator=(FixedQueueC<T> &queue) {
      First(queue);
      return *this;
    }
    //: Assign to a queue.
    
    bool IsElm() const
    { return at != end; }
    //: At valid element ?
    
    operator bool() const
    { return at != end; }
    //: At a valid element ?
    
    void Next() {
      at--;
      if(at == eoa)
	at = &(ArrayStart()[this->Size()-1]);
    }
    //: Goto next element.
    
    void operator++(int)
    { Next(); }
    //: Goto next element.
    
    void operator++()
    { Next(); }
    //: Goto next element.
    
    T &Data() 
    { return *at; }
    //: Access data.
    
    const T &Data() const
    { return *at; }
    //: Access data.
    
    T &operator*()
    { return *at; }
    //: Access data.

    const T &operator*() const
    { return *at; }
    //: Access data.
    
    T *operator->()
    { return at; }
    //: Access data.

    const T *operator->() const
    { return at; }
    //: Access data.
    
  protected:
    T *ArrayStart()
    { return &SArray1dC<T>::operator[](0); }
    //: Get the start of the array.
    
    const T *ArrayStart() const
    { return &SArray1dC<T>::operator[](0); }
    //: Get the start of the array.
    
    T *at;
    T *end;
    T *eoa;
  };
  
  //-----------------------------------------------------

  //: Construct an empty queue, queue must be resized before use.
  template<class T>
  inline FixedQueueC<T>::FixedQueueC()
  {
    head = 0;
    tail = 0;
    eoa = 0;
  }

  template<class T>
  inline
  FixedQueueC<T>::FixedQueueC(SizeT size)
  // The queue needs to be bigger by one than the maximum size so we can distinguish between an empty and full queue
  // from the head and tail pointers.
    : SArray1dC<T>(size+1)
  { 
    head = ArrayStart();
    tail = head;
    eoa = &(head[SArray1dC<T>::Size()]);
  }

  //: Resize the queue.
  // The new size must be larger than the number of used elements.
  template<class T>
  void FixedQueueC<T>::Resize(SizeT newSize)
  {
    RavlAssert(newSize >= Size());
    if(newSize < Size())
      throw RavlN::ExceptionOperationFailedC("Resizing queue will delete elements.");
    SArray1dC<T> newArr(newSize+1);
    int at = 0;
    for(FixedQueueIterC<T> it(*this);it;it++)
      newArr[at++] = *it;
    static_cast<SArray1dC<T> &>(*this) = newArr;
    tail = ArrayStart();
    head = &tail[at];
    eoa = &(tail[SArray1dC<T>::Size()]);
  }


  
  template<class T>
  inline 
  bool FixedQueueC<T>::IsSpace() const {
    const T *nhead = head + 1;
    if(nhead >= eoa)
      nhead = ArrayStart();
    return (nhead != tail);
  }
  
  template<class T>
  inline 
  void FixedQueueC<T>::InsLast(const T &Obj)  {
    RavlAssert(IsSpace());
    *(head++) = Obj;
    if(head >= eoa)
      head = ArrayStart();
  }
  
  template<class T>
  inline 
  void FixedQueueC<T>::ForceInsLast(const T &Obj) {
    *(head++) = Obj;
    if(head >= eoa)
      head = ArrayStart();
    if(head == tail) { // Need to push tail ?
      if(++tail >= eoa)
	tail = ArrayStart();
    }
  }
  
  template<class T>
  inline 
  void FixedQueueC<T>::ShiftInsLast(const T &Obj) {
    DelFirst();
    InsLast(Obj);
  }
  
  template<class T>
  inline 
  T FixedQueueC<T>::GetFirst() {
    RavlAssert(!IsEmpty());
    T Ret = *(tail++);
    if(tail >= eoa)
      tail = ArrayStart();
    return Ret;
  }
  
  template<class T>
  inline 
  void  FixedQueueC<T>::DelFirst()  {
    RavlAssert(!IsEmpty());
    if(++tail >= eoa)
      tail = ArrayStart();
  }

  template<class T>
  inline 
  SizeT FixedQueueC<T>::Size() const {
    if(head >= tail)
      return (SizeT) (head - tail);
    return (SizeT) ((eoa - tail) + (head - ArrayStart()));
  }
  
  template<class T>
  inline 
  bool FixedQueueC<T>::IsInRing(UIntT at) const  {
    if(IsEmpty())
      return false;
    const T *p = &(ArrayStart()[at]);
    if(head > tail)
      return (p >= tail && p < head);
    return (p >= tail || p < head);
  }
  
  template<class T>
  inline 
  void FixedQueueC<T>::Empty() {
    tail = ArrayStart();
    head = tail;
  }
}


#endif
