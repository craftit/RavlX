// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BLKQUEUE_HEADER
#define RAVL_BLKQUEUE_HEADER 1
///////////////////////////////////////////////////////////////////////////
//! date="15/11/1995"
//! file="Ravl/Core/Container/Queue/BlkQueue.hh"
//! lib=RavlCore
//! author="George (Jiri) Matas"
//! docentry="Ravl.API.Core.Queues"
//! example=exQueue.cc

#include "Ravl/Assert.hh"
#include "Ravl/SysLog.hh"

#if RAVL_HAVE_TEMPLATEREQUIREALLDEFINITIONS 
#include "Ravl/Stream.hh"
#endif

#if RAVL_HAVE_ANSICPPHEADERS
#include <new>
#else
#include <new.h>
#endif

namespace RavlN {

  template <class T> class BlkQueueIterC;

  //: Basic Queue.
  //! userlevel=Normal
  
  template <class T>
  class BlkQueueC {
  public:
    BlkQueueC(int blkSize = 20);
    //: Construct empty queue.

    BlkQueueC(const BlkQueueC<T> &other);
    //: Copy constructor
    
    ~BlkQueueC();
    //: Destructor
    
    const BlkQueueC<T>& operator=(const BlkQueueC<T> &other);
    //: Assignment to another queue.
    //: This creates a new copy.

    inline T GetFirst();
    //: Get the first element in the queue.
    
    inline void InsLast(const T& data);
    //: Put an item at the end of the queue.
    
    inline T& First()
    { return (first!=blkSize) ? firstBlk->d[first] : firstBlk->link->d[0]; }
    //: Access the first element in the queue.
    
    inline const T& First() const
    { return (first!=blkSize) ? firstBlk->d[first] : firstBlk->link->d[0]; }
    //: Access the first element in the queue.
    
    inline T& Last()
    { return lastBlk->d[last-1]; }
    //: Access the last element in the queue.
    
    inline const T& Last()  const
    { return lastBlk->d[last-1]; }
    //: Access the last element in the queue.
    
    inline bool IsEmpty() const
    { return last==first && lastBlk==firstBlk; }
    //: Is queue empty ?
    
    inline SizeT Size() const
    { return last-first + blkTotal; }
    //: Returns the number of items in the queue.
    
    inline void DelFirst(); 
    //: Delete the first element in the queue.
    
    inline void  Empty();
    //: Empty the queue of all items.

    inline void InsFirst(const T& data);
    //: Insert an item at the front of the queue.
    
    inline T& Top()
    { return First(); }
    //: Alias for First().
    
    inline void Push(const T& d)
    { InsFirst(d); }
    //: Push an item onto the front of the queue.
    
    inline T Pop()
    { return GetFirst();}
    //: Pop an item off the front on the queue.
    
    const T &operator[](int index) const;
    //: Index queue

    void DbPrint() { RavlDebug("Queue: %d -> %d   Size:%d BlkTotal:%d",last,first,Size(),blkTotal); }
    //: Debug print.


  protected:
    inline void Init();
    inline void FixFirst();
    //! userlevel=Develop
    struct q_Blk {q_Blk* link; T d[0];};
    q_Blk * AllocBlk()
    {return (q_Blk*) new char[sizeof(q_Blk) + sizeof(T)*(blkSize)];}
    void FirstBlk();
    void DelBlk();
    
    q_Blk* firstBlk;                   
    q_Blk* lastBlk;                   
    
    int first;
    int last;
    int blkSize;
    int blkTotal;

    friend class BlkQueueIterC<T>;
  };
  

  //! Block queue iterator.
  //! Note: The underling queue must not be modified while the iterator is in use.

  template <class T>
  class BlkQueueIterC {
  public:
    BlkQueueIterC()
     : m_atBlk(0),
       m_index(0),
       m_last(0),
       m_blkSize(1),
       m_blkTotal(0)
    {}
    //! Default constructor

    BlkQueueIterC(const BlkQueueC<T> &blkQueue)
     : m_atBlk(blkQueue.firstBlk),
       m_index(blkQueue.first),
       m_last(blkQueue.last),
       m_blkSize(blkQueue.blkSize),
       m_blkTotal(blkQueue.blkTotal)
    {
      if(m_index >= m_blkSize && m_blkTotal > 0) {
        m_atBlk = m_atBlk->link;
        m_index = 0;
        m_blkTotal -= m_blkSize;
      }
    }
    //! Constructor.

    bool IsElm() const
    { return m_index < m_last || m_blkTotal > 0; }
    //! Is iterator on a valid element.

    operator bool() const
    { return IsElm(); }
    //! Is iterator on a valid element.

    const T &Data() const
    {
      RavlAssert(m_atBlk != 0 && m_index < m_blkSize);
      return m_atBlk->d[m_index];
    }
    //! Access current data element.

    T &Data()
    {
      RavlAssert(m_atBlk != 0 && m_index < m_blkSize);
      return m_atBlk->d[m_index];
    }
    //! Access current data element.

    T &operator*()
    { return Data(); }
    //! Access current data element.

    const T &operator*() const
    { return Data(); }
    //! Access current data element.

    bool Next()
    {
      RavlAssert(m_atBlk != 0);
      m_index++;
      if(m_blkTotal > 0) {
        if(m_index >= m_blkSize) {
          m_atBlk = m_atBlk->link;
          m_index = 0;
          m_blkTotal -= m_blkSize;
        }
      }
      return IsElm();
    }
    //! Goto next element.
    //! Return true if left on a valid data element.

    bool operator++()
    { return Next(); }
    //! Goto next element.
    //! Return true if left on a valid data element.

    bool operator++(int)
    { return Next(); }
    //! Goto next element.
    //! Return true if left on a valid data element.

    bool operator==(const BlkQueueIterC<T> &other) const
    { return m_atBlk == other.m_atBlk && m_index == other.m_index; }
    //! Does the iterator point to the same element as another?

  protected:
    typedef typename BlkQueueC<T>::q_Blk BlkT;

    BlkT *m_atBlk;
    int m_index;
    int m_last;
    int m_blkSize;
    int m_blkTotal;
  };


  //--------------------------------------------------------------------------
  
  template<class T>
  inline void BlkQueueC<T>::Init() {
    RavlAssert(blkSize > 0);
    blkSize    =  -blkSize;
    first      =  last    = blkSize; 
    firstBlk   =  lastBlk = 0;
    blkTotal   =  0;
  }
  
  template<class T>
  BlkQueueC<T>::BlkQueueC(int size)
    : blkSize(size) 
  { Init(); }

  template<class T>
  BlkQueueC<T>::BlkQueueC(const BlkQueueC<T> &other)
    : blkSize(other.blkSize < 0 ? - other.blkSize: other.blkSize)
  {
    Init();
    for(BlkQueueIterC<T> it(other);it;it.Next())
      InsLast(*it);
  }

  template<class T>
  const BlkQueueC<T>& BlkQueueC<T>::operator=(const BlkQueueC<T> &other)
  {
    Empty();
    for(BlkQueueIterC<T> it(other);it;it.Next())
      InsLast(*it);
    return *this;
  }

  template <class T>
  void BlkQueueC<T>::DelBlk() {
    q_Blk * next = firstBlk->link;
    delete [] ((char *) firstBlk);
    firstBlk = next;
    blkTotal -=   blkSize;
  }
  
  template<class T>
  inline void BlkQueueC<T>::FirstBlk() {
    blkSize  = -blkSize;
    first    = last   = 0;
    firstBlk = lastBlk= AllocBlk();
  }
  
  template<class T> 
  inline void BlkQueueC<T>::Empty() {
    while(!IsEmpty())
      DelFirst();
  }
  
  template<class T>
  BlkQueueC<T>::~BlkQueueC()  { 
    Empty(); 
    if(firstBlk != 0)
      DelBlk(); 
  }
  
  template<class T>
  inline  void BlkQueueC<T>::FixFirst() {
    if (IsEmpty()) 
      IssueError(__FILE__,__LINE__,"attempting to read from and empty queue.");
    if (first == blkSize)  {
      DelBlk();
      first = 0;
    }
  }
  
  template<class T>
  inline  void BlkQueueC<T>::DelFirst() {
    FixFirst();
    (&firstBlk->d[first++])->~T();
  }
  
  template<class T>
  inline  T BlkQueueC<T>::GetFirst() {
    FixFirst();
    T tmp=firstBlk->d[first];
    (&firstBlk->d[first++])->~T();
    return tmp;
  }
    
  template<class T>
  inline void BlkQueueC<T>::InsLast(const T& data) {
    if (last == blkSize) {
      if(blkSize<0)
	FirstBlk();
      else {
	lastBlk->link = AllocBlk();
	lastBlk = lastBlk->link;
	blkTotal +=   blkSize;
	last = 0;
      }
      lastBlk->link = NULL;
    }
    new(&lastBlk->d[last++]) T(data);   // like a straightforward array impl.
  }
  
  template<class T>
  inline 
  void BlkQueueC<T>::InsFirst(const T& data) {
    --first;
    if(first<0) {
      if (blkSize<0) {
	FirstBlk();
	new(&(firstBlk->d[last++])) T(data);
	return;
      } else {
	q_Blk* f = AllocBlk();
	f->link  = firstBlk;
	firstBlk = f;
	first    = blkSize - 1;
	blkTotal += blkSize;
      }
    }
    new(&firstBlk->d[first]) T(data);
  }

  template<class T>
  const T &BlkQueueC<T>::operator[](int index) const
  {
    RavlAssert(index >= 0 && index < Size());
    int inFirstBlk=(blkSize - first);
    if(inFirstBlk > index) {
      return firstBlk->d[first + index];
    }
    index -= inFirstBlk;
    q_Blk *at = firstBlk->link;
    while(index >= blkSize) {
      index -= blkSize;
      at = at->link;
      RavlAssert(at != 0);
    }
    return at->d[index];
  }

}

#endif
