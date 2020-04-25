// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_THREADMSGQUEUE_HEADER 
#define RAVL_THREADMSGQUEUE_HEADER 1
/////////////////////////////////////////////////////////////////////
//! author="Charles Galambos"
//! date="25/11/1995"
//! docentry="Ravl.API.OS.Threads;Ravl.API.Core.Queues"
//! rcsid="$Id$"
//! file="Ravl/OS/Threads/Tools/MessageQueue.hh"
//! lib=RavlThreads

#include "Ravl/Threads/Mutex.hh"
#include "Ravl/Threads/Semaphore.hh"
#include "Ravl/Exception.hh"

#if RAVL_HAVE_ANSICPPHEADERS
#include <new>
#else
#include <new.h>
#endif

namespace RavlN {
  
  //////////////////////
  //! userlevel=Develop
  //: MessageQueue base class.
  // SMALL OBJECT. <p>
  
  class MessageQueueBaseC {
  public:
    MessageQueueBaseC(int nMaxSize = 10) 
      : putSema(nMaxSize), 
        ready(0),
        maxSize(nMaxSize + 2),
        head(0),
        tail(0)
    { RavlAssert(nMaxSize > 0); }
    //: Default constructor.
    
    MessageQueueBaseC(const MessageQueueBaseC &) 
      : putSema(0),
	ready(0),
	maxSize(0)
    { RavlAssert(0); } 
    // Not supported !
    
    void Dump(void) const; 
    //: Dump contents to stdout.
    
    IntT MaxSize() const { return maxSize-2; }
    //: Get maximum number of items that may be held in the queue.
    
  protected:
    inline bool IsSpace() const {
      IntT nHead = head + 1;
      if(nHead >= maxSize)
	nHead = 0;
      return (nHead != tail);
    }
    // Is space to data into ring ?
    
    inline bool IsEmptyBase() const
    { return head == tail; }
    // Is space to data into ring ?
    
    MutexC access;      // Access control.
    SemaphoreC putSema; // Used to limit the size of the queue.
    SemaphoreC ready;   // Used to signal queue is not empty.
    const IntT maxSize;
    volatile IntT head;
    volatile IntT tail;
  };
  
  //////////////////////
  //! userlevel=Normal
  //: Thread safe queue.
  // This is designed to be used in inter-thread communication.
  // And number of threads can get and put from the queue. The queue is
  // of a fixed length to provide some flow control in the program. 
  // SMALL OBJECT.
  
  template<class T>
  class MessageQueueC 
    : public MessageQueueBaseC
  {
  public:
    MessageQueueC(int nMaxSize = 10)
      : MessageQueueBaseC(nMaxSize),
	data((T *) new char[(nMaxSize + 2) * sizeof(T)])
    {}
    //: Default constructor.
    
    MessageQueueC(const MessageQueueC<T> &) 
     : data(0)
    { RavlAssert(0); }// Not supported !
    //: Copy constructor.
    
    ~MessageQueueC() { 
      // By now we should be the only thread running in this class, 
      // so the following should completely empty the queue.
      Empty();
      RavlAssert(!IsElm());
      delete [] (char *) data; 
    }
    //: Destructor.
    
    inline void Put(const T &Data);
    //: Put data into queue.
    // Will block until there is space in the queue.
    
    inline bool TryPut(const T &Data,RealT maxWait);
    //: Put data into queue, with maximum wait time in seconds.
    // If the data is added to the queue successfully the method returns true
    // if a timeout occurs false is returned.
    
    inline bool TryPut(const T &Data);
    //: Try an put data in queue if there's space.
    // Ret:true = Data in queue. Ret:false = data not in queue.
    
    inline T Get();
    //: Get data from queue.
    // Will block until there is data in the queue.
    
    inline bool Get(T &data,RealT maxWait);
    //: Get data from queue, with maximum wait time in seconds.
    // If successful the data is assigned to 'data' and true is returned.
    
    inline bool TryGet(T &Data);
    //: Try and get data from queue.
    // Ret:false = No data available.

    inline T First(RealT maxWait);
    //: Access the first element in the queue, with maximum wait time in seconds.
    //!throw: ExceptionOperationFailedC if the time out is exceeded.

    inline bool TryFirst(T& value);
    //: Access the first element in the queue.
    // If successful the data is assigned to 'value' and true is returned.

    inline bool Discard(RealT maxWait = -1);
    //: Discard element from queue.
    // If maxWait is negative it will block until data arrives otherwise the 
    // discard will time out after the given number of seconds.
    
    inline bool IsElm();
    //: Is there data in the queue ?

    bool IsEmpty()
    { return !IsElm(); }
    //: Is the queue empty ?
    
    inline int Size();
    //: How much data is there in the queue.
    
    void Empty();
    //: Empty pipe of all its contents.
    
  private:
    T *data;
  };
  
  //////////////////////////////////////////////////////////////
  
  // Put data into queue.
  
  template<class T>
  inline void MessageQueueC<T>::Put(const T &Data)  {
    putSema.Wait();
    MutexLockC lock(access);
    new((void *) &(data[head])) T(Data); // Possible exception.
    // If exception constructing object following code won't be called.
    head++;
    if(head >= maxSize)
      head = 0;
    lock.Unlock();
    ready.Post();
  }

  // Put data into queue.
  
  template<class T>
  inline bool MessageQueueC<T>::TryPut(const T &Data,RealT maxWait)  {
    if(!putSema.Wait(maxWait)) 
      return false;
    MutexLockC lock(access);
    new((void *) &(data[head])) T(Data); // Possible exception.
    // If exception constructing object following code won't be called.
    head++;
    if(head >= maxSize)
      head = 0;
    lock.Unlock();
    ready.Post();
    return true;
  }
  
  /////////////////////////
  // Try an put data in queue if there's space.
  // Ret:true = Data in queue. Ret:false = data not in queue.
  
  template<class T>
  inline bool MessageQueueC<T>::TryPut(const T &Data)  {
    if(!putSema.TryWait()) 
      return false;
    MutexLockC lock(access);
    new((void *) &(data[head])) T(Data); // Possible exception.
    // If exception constructing object following code won't be called.
    head++;
    if(head >= maxSize)
      head = 0;  
    lock.Unlock();
    ready.Post();
    return true;
  }

  /////////////////////////
  // Get data from queue.
  
  template<class T>
  inline T MessageQueueC<T>::Get()  {
    ready.Wait();
    MutexLockC lock(access);
    RavlAssert(!IsEmptyBase());
    T Ret = data[tail]; // Possible exception.
    // If copy failed we're in trouble anyway.
    data[tail].~T(); // Possible exception.
    tail++;
    if(tail >= maxSize)
      tail = 0;  
    lock.Unlock();
    putSema.Post();
    return Ret;
  }
  
  //: Get data from queue, with maximum wait.
  // If success full the data is assigned to 'data' and true is returned.

  template<class T>
  bool MessageQueueC<T>::Get(T &val,RealT maxWait) {
    if(!ready.Wait(maxWait))
      return false;
    MutexLockC lock(access);
    RavlAssert(!IsEmptyBase());
    val = data[tail]; // Possible exception.
    // If copy failed we're in trouble anyway.
    data[tail].~T(); // Possible exception.
    tail++;
    if(tail >= maxSize)
      tail = 0; 
    lock.Unlock();
    putSema.Post();
    return true;
  }
  
  ///////////////////////////
  // Try and get data from queue.
  // Ret:false = No data available.
  
  template<class T>
  inline 
  bool MessageQueueC<T>::TryGet(T &Data)  {
    if(!ready.TryWait())
      return false;
    MutexLockC lock(access);
    // If copy failed we're in trouble anyway.
    Data = data[tail]; // Possible exception.
    data[tail].~T();   // Possible exception.
    tail++;
    if(tail >= maxSize)
      tail = 0;  
    lock.Unlock();
    putSema.Post();
    return true;
  }

  //: Access first the first element in the queue.
  //!throw: ExceptionOperationFailedC if the time out is exceeded.

  template<class T>
  inline
  T MessageQueueC<T>::First(RealT maxWait)
  {
    ready.Wait(maxWait);
    MutexLockC lock(access);
    ready.Post();
    if (IsEmptyBase())
      throw ExceptionOperationFailedC("Timed out waiting for first queue element.");
    T Ret(data[tail]); // Possible exception.
    lock.Unlock();
    return Ret;
  }

  //: Access the first element in the queue.
  // If successful the data is assigned to 'value' and true is returned.

  template<class T>
  inline
  bool MessageQueueC<T>::TryFirst(T& value)
  {
    if (IsEmptyBase())
      return false;
    MutexLockC lock(access);
    // If copy failed we're in trouble anyway.
    value = data[tail]; // Possible exception.
    lock.Unlock();
    return true;
  };

  //: Discard element from queue.
  
  template<class T>
  inline 
  bool MessageQueueC<T>::Discard(RealT maxWait) {
    if(maxWait < 0) { // Wait forever ?
      ready.Wait();
    } else { // Just until the timeout.
      if(!ready.Wait(maxWait))
        return false;
    }
    MutexLockC lock(access);
    data[tail].~T();   // Possible exception.
    tail++;
    if(tail >= maxSize)
      tail = 0;  
    lock.Unlock();
    putSema.Post();
    return true;    
  }
  
  //: Empty pipe of all its contents.
  template<class T>
  void MessageQueueC<T>::Empty() {
    while(IsElm())
      Discard(0);
  }
  
  /////////////////////////
  // Is there data in the queue ?
  
  template<class T>
  inline 
  bool MessageQueueC<T>::IsElm()  { 
    MutexLockC lock(access);
    return !IsEmptyBase();
    // Unlock here.
  }
  
  /////////////////////////
  // How much data is there in the queue.
  
  template<class T>
  inline 
  int MessageQueueC<T>::Size()  {
    MutexLockC lock(access);
    if(head >= tail)
      return head-tail;
    return (maxSize - tail) + head;
    // Unlock here.
  }
  
};
#endif
