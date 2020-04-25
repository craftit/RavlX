// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLSIGCONNECTIONSET_HEADER
#define RAVLSIGCONNECTIONSET_HEADER 1
/////////////////////////////////////////////////////
//! docentry="Ravl.API.OS.Signals"
//! lib=RavlThreads
//! author="Charles Galambos"
//! date="13/12/2000"
//! rcsid="$Id$"
//! file="Ravl/OS/Threads/Tools/SignalConnectionSet.hh"

#include "Ravl/Threads/Signal.hh"
#include "Ravl/Threads/Mutex.hh"
#include "Ravl/HSet.hh"

namespace RavlN {

  //! userlevel=Develop
  //: Signal connection set body.
  
  class SignalConnectionSetBodyC 
    : public RCBodyC
  {
  public:
    SignalConnectionSetBodyC()
    {}
    //: Constructor.
    
    ~SignalConnectionSetBodyC()
    { DisconnectAll(); }
    //: Destructor.
    
    void DisconnectAll(bool waitThreadsExit = false);
    //: Disconnect everything.
    // If waitThreadExit is true, the routine won't return
    // till all threads have left the connection.
    
    void operator+=(const SignalConnectorC &c);
    //: Add a connection.
    
    void operator-=(const SignalConnectorC &c);
    //: Remove a connection.
    
    bool IsEmpty() const;
    //: Test if the connection set is empty.
    
  protected:
    MutexC access;
    HSetC<SignalConnectorC> cons;
  };
  
  //! userlevel=Normal
  //: Signal connection set body.
  // This class is used to construct a list of
  // connections that should be disconnected when the class
  // is destroyed or at some other point in processing.
  
  class SignalConnectionSetC 
    : public RCHandleC<SignalConnectionSetBodyC>
  {
  public:
    //SignalConnectionSetC()
    //{}
    //: Default constructor.
    // creates an invalid handle.
    
    SignalConnectionSetC()
      : RCHandleC<SignalConnectionSetBodyC>(*new SignalConnectionSetBodyC())
    {}
    //: Constructor.
    
    void DisconnectAll(bool waitThreadsExit = false)
    { Body().DisconnectAll(waitThreadsExit); }
    //: Disconnect everything.
    // If waitThreadExit is true, the routine won't return
    // till all threads have left the connection. NOTE: If this routine
    // is itself being called on the signal it will deadlock. 
    
    void operator+=(const SignalConnectorC &c)
    { Body().operator+=(c); }
    //: Add a connection.
   
    void operator-=(const SignalConnectorC &c)
    { Body().operator-=(c); }
    //: Remove a connection.
    
    bool IsEmpty() const
    { return Body().IsEmpty(); }
    //: Test if the connection set is empty.
    
  };
}

#endif
