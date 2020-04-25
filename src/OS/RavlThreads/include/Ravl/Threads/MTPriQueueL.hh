// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_MTPRIQUEUEL_HEADER
#define RAVL_MTPRIQUEUEL_HEADER 1
//! author="Charles Galambos"
//! date="10/6/2004"
//! docentry="Ravl.API.OS.Threads;Ravl.API.Core.Queues"
//! rcsid="$Id$"
//! lib=RavlThreads
//! file="Ravl/OS/Threads/Tools/MTPriQueueL.hh"

#include "Ravl/PriQueueL.hh"
#include "Ravl/Threads/RWLock.hh"
#include "Ravl/Threads/Semaphore.hh"

namespace RavlN {

  //! userlevel=Normal
  //: Thread safe priority queue.
  // Small object.
  
  template <class KeyT,class DataT>
  class MTPriQueueLC {
  public:
    MTPriQueueLC()
      : semaSize(0)
    {}
    //: Default constructor.
    
    bool IsElm() const  { 
      // Can use it directly as its a single compare.
      return queue.IsElm();
    }
    //: Does the queue contains any items ?
    
    bool IsEmpty() const { 
      // Can use it directly as its a single compare.
      return queue.IsEmpty(); 
    }
    //: Is the queue empty ?
    
    bool Top(DataT &buff) const {
      RWLockHoldC hold(access,RWLOCK_READONLY);
      if(queue.IsEmpty()) return false;
      buff = this->root->Data(); 
      return true;
    }
    //: Look at data on top of queue.
    // Returns false if queue is empty.

    bool Top(KeyT &key,DataT &buff) const {
      RWLockHoldC hold(access,RWLOCK_READONLY);
      if(queue.IsEmpty()) return false;
      key = queue.TopKey();
      buff = queue.Top(); 
      return true;
    }
    //: Look at data and key on top of queue.
    // Returns false if queue is empty.
    
    bool TopKey(KeyT &key) const { 
      RWLockHoldC hold(access,RWLOCK_READONLY);
      if(queue.IsEmpty()) return false;      
      key = this->root->Key(); 
      return true;
    }
    //: Look at key on top of queue.
    // Returns false if queue is empty.
    
    bool DelTop() { 
      if(!semaSize.TryWait())
	return false;
      RWLockHoldC hold(access,RWLOCK_WRITE);
      delete &GetTopPair(); 
      return true;
    }
    //: Delete item on top of queue.
    // This will return true if an item has been removed
    // and false otherwise.
    
    Tuple2C<KeyT,DataT> &GetTopPair() {
      semaSize.Wait();
      RWLockHoldC hold(access,RWLOCK_WRITE);
      return queue.GetTopPair();
    }
    //: Get Key/Data pair from queue.
    // Will block if queue is empty.
    // WARNING: For advanced users only.
    // !! Users responsability to delete returned info after use. !!
    
    DataT GetTop() {
      semaSize.Wait();
      RWLockHoldC hold(access,RWLOCK_WRITE);
      return queue.GetTop();
    }
    //: Get Data from top of queue.
    // Will block if queue is empty.
    
    void Insert(const KeyT &key,const DataT &data) {
      RWLockHoldC hold(access,RWLOCK_WRITE);
      queue.Insert(key,data); 
      hold.Unlock();
      semaSize.Post();
    }
    //: Insert Data/Key into queue.
    
    void Insert(Tuple2C<KeyT,DataT> &entry) {
      RWLockHoldC hold(access,RWLOCK_WRITE);
      queue.Insert(entry); 
      hold.Unlock();
      semaSize.Post();
    }
    //: Insert Data/Key into queue.
    
    int Size() const 
    { return semaSize.Count(); }
    //: Get number of items in queue. 
    
    void Empty() { 
      RWLockHoldC hold(access,RWLOCK_WRITE);
      queue.Empty();
      while(semaSize.TryWait()) ; // Set sema count to zero.
    }
    //: Empty the queue of all its contents.
    
  protected:
    RWLockC access;
    PriQueueLC<KeyT,DataT> queue;
    SemaphoreC semaSize; // Incremented for each item in the queue.
    
  private:
    MTPriQueueLC(const MTPriQueueLC<KeyT,DataT> &obj)
    { RavlAssert(0); }
    //: Copy constructor.
    // Hide so we can't seperate the lock and queue.
  };
  
}


#endif
