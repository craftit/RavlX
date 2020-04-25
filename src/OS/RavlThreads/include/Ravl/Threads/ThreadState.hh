// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_THREADSTATE_HEADER
#define RAVL_THREADSTATE_HEADER 1
/////////////////////////////////////////////////
//! file="Ravl/OS/Threads/Posix/ThreadState.hh"
//! lib=RavlThreads
//! userlevel=Normal
//! docentry="Ravl.API.OS.Threads"
//! author="Charles Galambos"
//! date="25/05/2011"

#include "Ravl/config.h"
#include "Ravl/Threads/ConditionalVariable.hh"
#include "Ravl/Stream.hh"
#include "Ravl/Threads/Signal1.hh"
#include "Ravl/OS/Date.hh"

#define DODEBUG_TH 0
#if DODEBUG_TH
#include <Ravl/SysLog.hh>
#define ONDEBUG_TH(x) x
#else
#define ONDEBUG_TH(x)
#endif


namespace RavlN
{

  //! userlevel=Devel
  //: Base class for ThreadStateC

  class ThreadStateBaseC
  {
  public:
    ThreadStateBaseC();
    //: Constructor

    ~ThreadStateBaseC();
    //: Destructor.

    bool WaitForFree();
    //: Wait for lock to be free of all waiters.

    IntT ThreadsWaiting() const
    { return m_waiting; }
    //: Get approximation of number of threads waiting.

  protected:
    mutable ConditionalVariableC m_cond;
    mutable MutexC m_access;
    mutable volatile IntT m_waiting; // Count of number of threads waiting on this...
  };

  //! userlevel=Normal
  //: Thread safe state machine.
  // <p>This class enables a thread to sleep until a specific state or set of states has been entered.</p>
  // It is assumed that 'StateT' may be copied without a lock, as is the case with built and enumerated types.

  template<typename StateT>
  class ThreadStateC
   : public ThreadStateBaseC
  {
  public:
    ThreadStateC(const StateT &initialState)
      : m_state(initialState),
        m_sigState(initialState)
    {}
    //: Constructor

    bool Update(const StateT &newState) {
      ONDEBUG_TH(RavlDebug("Updating state (current %i) to %i", int(m_state), int(newState)));
      m_access.Lock();
      if(newState == m_state) {
        // No change.
        m_access.Unlock();
        return false;
      }
      m_state = newState;
      m_access.Unlock();
      m_cond.Broadcast();
      m_sigState(newState);
      ONDEBUG_TH(RavlDebug("New state %i", int(m_state)));
      return true;
    }
    //: Set the current state.
    // Returns true if new state has changed.  false it its at the given state already

    bool SetState(const StateT &newState)
    { return Update(newState); }
    //: Set the current state, used for connecting signals as update is ambiguous.

    bool Update(const StateT &expectedCurrentState,const StateT &newState) {
      ONDEBUG_TH(RavlDebug("Updating state (current %i) expected current %i  to %i", int(m_state), int(expectedCurrentState), int(newState)));
      m_access.Lock();
      if(m_state != expectedCurrentState) {
        // No change.
        m_access.Unlock();
        return false;
      }
      m_state = newState;
      m_access.Unlock();
      m_cond.Broadcast();
      m_sigState(newState);
      ONDEBUG_TH(RavlDebug("New state %i", int(m_state)));
      return true;
    }
    //: Change from a given existing state to a new state
    // Returns true if event transition was achieved.
    // The state of class isn't 'expectedCurrentState' return false.

    bool Update(const StateT &expectedCurrentState1,const StateT &expectedCurrentState2,const StateT &newState) {
      ONDEBUG_TH(RavlDebug("Updating state (current %i) expected current %i or %i  to %i", int(m_state), int(expectedCurrentState1), int(expectedCurrentState2), int(newState)));
      m_access.Lock();
      if(m_state != expectedCurrentState1 && m_state != expectedCurrentState2) {
        // No change.
        m_access.Unlock();
        return false;
      }
      m_state = newState;
      m_access.Unlock();
      m_cond.Broadcast();
      m_sigState(newState);
      ONDEBUG_TH(RavlDebug("New state %i", int(m_state)));
      return true;
    }

    bool Update(const StateT &expectedCurrentState1,
                const StateT &expectedCurrentState2,
                const StateT &expectedCurrentState3,
                const StateT &newState)
    {
      ONDEBUG_TH(RavlDebug("Updating state (current %i) expected current %i %i or %i  to %i", int(m_state), int(expectedCurrentState1), int(expectedCurrentState2), int(expectedCurrentState3),int(newState)));
      m_access.Lock();
      if(m_state != expectedCurrentState1 &&
          m_state != expectedCurrentState2 &&
          m_state != expectedCurrentState3) {
        // No change.
        m_access.Unlock();
        return false;
      }
      m_state = newState;
      m_access.Unlock();
      m_cond.Broadcast();
      m_sigState(newState);
      ONDEBUG_TH(RavlDebug("New state %i", int(m_state)));
      return true;
    }

    bool Update(const StateT &expectedCurrentState1,
                const StateT &expectedCurrentState2,
                const StateT &expectedCurrentState3,
                const StateT &expectedCurrentState4,
                const StateT &newState)
    {
      ONDEBUG_TH(RavlDebug("Updating state (current %i) expected current %i %i or %i  to %i", int(m_state), int(expectedCurrentState1), int(expectedCurrentState2), int(expectedCurrentState3),int(newState)));
      m_access.Lock();
      if(m_state != expectedCurrentState1 &&
          m_state != expectedCurrentState2 &&
          m_state != expectedCurrentState3 &&
          m_state != expectedCurrentState4) {
        // No change.
        m_access.Unlock();
        return false;
      }
      m_state = newState;
      m_access.Unlock();
      m_cond.Broadcast();
      m_sigState(newState);
      ONDEBUG_TH(RavlDebug("New state %i", int(m_state)));
      return true;
    }

    bool Update(const StateT &expectedCurrentState1,
                const StateT &expectedCurrentState2,
                const StateT &expectedCurrentState3,
                const StateT &expectedCurrentState4,
                const StateT &expectedCurrentState5,
                const StateT &newState)
    {
      ONDEBUG_TH(RavlDebug("Updating state (current %i) expected current %i %i or %i  to %i", int(m_state), int(expectedCurrentState1), int(expectedCurrentState2), int(expectedCurrentState3),int(newState)));
      m_access.Lock();
      if(m_state != expectedCurrentState1 &&
          m_state != expectedCurrentState2 &&
          m_state != expectedCurrentState3 &&
          m_state != expectedCurrentState4 &&
          m_state != expectedCurrentState5) {
        // No change.
        m_access.Unlock();
        return false;
      }
      m_state = newState;
      m_access.Unlock();
      m_cond.Broadcast();
      m_sigState(newState);
      ONDEBUG_TH(RavlDebug("New state %i", int(m_state)));
      return true;
    }

    template<typename TransitionMapT>
    bool Transition(TransitionMapT tmap) {
      ONDEBUG_TH(RavlDebug("Transition"));
      m_access.Lock();
      StateT newState = m_state; // Default to do nothing.
      const StateT oldState = m_state;
      bool ret = tmap(oldState,newState);
      if(m_state == newState) {
        // No change.
        m_access.Unlock();
        return ret;
      }
      m_state = newState;
      m_access.Unlock();
      m_cond.Broadcast();
      m_sigState(newState);
      ONDEBUG_TH(RavlDebug("New state %i", int(m_state)));
      return ret;
    }

    //: Change from one of two given existing states to a new state
    // Returns true if event transition was achieved.
    // The state of class isn't 'expectedCurrentState' return false.

    template<typename TransitionMapT,typename InputT>
    bool Transition(TransitionMapT tmap,InputT input) {
      ONDEBUG_TH(RavlDebug("Transition"));
      m_access.Lock();
      StateT newState = m_state; // Default to do nothing.
      const StateT oldState = m_state;
      bool ret = tmap(oldState,input,newState);
      if(m_state == newState) {
        // No change.
        m_access.Unlock();
        return ret;
      }
      m_state = newState;
      m_access.Unlock();
      m_cond.Broadcast();
      m_sigState(newState);
      ONDEBUG_TH(RavlDebug("New state %i", int(m_state)));
      return ret;
    }

    void Wait(const StateT &desiredState) const {
      ONDEBUG_TH(RavlDebug("Wait for %i (current %i)", int(desiredState), int(m_state)));
      if(m_state == desiredState) // Check before we bother with locking.
        return ;
      m_access.Lock();
      m_waiting++;
      while(!(m_state == desiredState))
        m_cond.Wait(m_access);
      int value = --m_waiting;
      m_access.Unlock();
      if(value == 0)
        m_cond.Broadcast(); // If something is waiting for it to be free...
    }
    //: Wait indefinitely for an event to be posted.

    void Wait(const StateT &desiredState1, const StateT &desiredState2) const {
      ONDEBUG_TH(RavlDebug("Wait for %i or %i (current %i)", int(desiredState1), int(desiredState2), int(m_state)));
      if(m_state == desiredState1 || m_state == desiredState2) // Check before we bother with locking.
        return ;
      m_access.Lock();
      m_waiting++;
      while(!(m_state == desiredState1 || m_state == desiredState2))
        m_cond.Wait(m_access);
      int value = --m_waiting;
      m_access.Unlock();
      if(value == 0)
        m_cond.Broadcast(); // If something is waiting for it to be free...
    }
    //: Wait indefinitely until either one or another state is reached


    bool Wait(RealT maxTime,const StateT &desiredState) const {
      ONDEBUG_TH(RavlDebug("Timed wait for %i  timeout %g (current %i)", int(desiredState), maxTime, int(m_state)));
      if(m_state == desiredState) // Check before we bother with locking.
        return true;
      bool ret = true;
      m_access.Lock();
      m_waiting++;
      if(maxTime >= 0) {
        DateC deadline = DateC::NowUTC() + maxTime;
        while(m_state != desiredState && ret)
          ret = m_cond.WaitUntil(m_access,deadline);
      } else {
        while(m_state != desiredState)
          m_cond.Wait(m_access);
      }
      int value = --m_waiting;
      m_access.Unlock();
      if(value == 0)
        m_cond.Broadcast(); // If something is waiting for it to be free...
      return ret;
    }
    //: Wait for a given state.
    // Returns false if timed out.

    bool Wait(RealT maxTime,
               const StateT &desiredState1,
               const StateT &desiredState2,
               StateT &stateAchieved
               ) const {
      ONDEBUG_TH(RavlDebug("Timed wait for %i or %i  timeout %g (current %i)", int(desiredState1), int(desiredState2), maxTime, int(m_state)));
      if(m_state == desiredState1 || m_state == desiredState2) { // Check before we bother with locking.
        stateAchieved = m_state;
        return true;
      }
      bool ret = true;
      m_access.Lock();
      m_waiting++;
      if(maxTime >= 0) {
        DateC deadline = DateC::NowUTC() + maxTime;
        while(m_state != desiredState1 && m_state != desiredState2 && ret)
          ret = m_cond.WaitUntil(m_access,deadline);
      } else {
        while(m_state != desiredState1 && m_state != desiredState2)
          m_cond.Wait(m_access);
      }
      int value = --m_waiting;
      stateAchieved = m_state;
      m_access.Unlock();
      if(value == 0)
        m_cond.Broadcast(); // If something is waiting for it to be free...
      return ret;
    }
    //: Wait for one of two states to be reached.
    // Returns false if timed out.

    bool Wait(RealT maxTime,
               const StateT &desiredState1,
               const StateT &desiredState2,
               const StateT &desiredState3,
               StateT &stateAchieved
               ) const {
      ONDEBUG_TH(RavlDebug("Timed wait for %i or %i or %i  timeout %g (current %i)", int(desiredState1), int(desiredState2), int(desiredState3), maxTime, int(m_state)));
      if(m_state == desiredState1 || m_state == desiredState2 || m_state == desiredState3) {// Check before we bother with locking.
        stateAchieved = m_state;
        return true;
      }
      bool ret = true;
      m_access.Lock();
      m_waiting++;
      if(maxTime >= 0) {
        DateC deadline = DateC::NowUTC() + maxTime;
        while(m_state != desiredState1 && m_state != desiredState2 && m_state != desiredState3 && ret)
          ret = m_cond.WaitUntil(m_access,deadline);
      } else {
        while(m_state != desiredState1 && m_state != desiredState2 && m_state != desiredState3)
          m_cond.Wait(m_access);
      }
      int value = --m_waiting;
      stateAchieved = m_state;
      m_access.Unlock();
      if(value == 0)
        m_cond.Broadcast(); // If something is waiting for it to be free...
      return ret;
    }
    //: Wait for one of two states to be reached.
    // Returns false if timed out.

    bool Wait(RealT maxTime,
               const StateT &desiredState1,
               const StateT &desiredState2,
               const StateT &desiredState3,
               const StateT &desiredState4,
               StateT &stateAchieved
               ) const {
      ONDEBUG_TH(RavlDebug("Timed wait for %i or %i or %i or %i  timeout %g (current %i)", int(desiredState1), int(desiredState2), int(desiredState3), int(desiredState4), maxTime, int(m_state)));
      if(m_state == desiredState1 || m_state == desiredState2 || m_state == desiredState3|| m_state == desiredState4) {// Check before we bother with locking.
        stateAchieved = m_state;
        return true;
      }
      bool ret = true;
      m_access.Lock();
      m_waiting++;
      if(maxTime >= 0) {
        DateC deadline = DateC::NowUTC() + maxTime;
        while(m_state != desiredState1 && m_state != desiredState2 && m_state != desiredState3 && m_state != desiredState4 && ret)
          ret = m_cond.WaitUntil(m_access,deadline);
      } else {
        while(m_state != desiredState1 && m_state != desiredState2 && m_state != desiredState3 && m_state != desiredState4)
          m_cond.Wait(m_access);
      }
      int value = --m_waiting;
      stateAchieved = m_state;
      m_access.Unlock();
      if(value == 0)
        m_cond.Broadcast(); // If something is waiting for it to be free...
      return ret;
    }
    //: Wait for one of two states to be reached.
    // Returns false if timed out.


    bool WaitNot(RealT maxTime,const StateT &theState) const {
      ONDEBUG_TH(RavlDebug("Timed wait not for %i   timeout %g (current %i)", int(theState), maxTime, int(m_state)));
      if(m_state != theState) {// Check before we bother with locking.
        return true;
      }
      bool ret = true;
      m_access.Lock();
      m_waiting++;
      if(maxTime >= 0) {
        DateC deadline = DateC::NowUTC() + maxTime;
        while(m_state == theState && ret)
          ret = m_cond.WaitUntil(m_access,deadline);
      } else {
        while(m_state == theState)
          m_cond.Wait(m_access);
      }
      int value = --m_waiting;
      m_access.Unlock();
      if(value == 0)
        m_cond.Broadcast(); // If something is waiting for it to be free...
      return ret;

    }
    //: Wait for a state to be left.

    operator StateT () const
    { return m_state; }
    //: Access state

    const StateT State() const
    { return m_state; }
    //: Access the current state.

    RavlN::Signal1C<StateT> &SigState()
    { return m_sigState; }
    //: Access state change signal.

  protected:

    volatile StateT m_state;
    RavlN::Signal1C<StateT> m_sigState;
  };
};

#endif
