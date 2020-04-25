// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlThreads
//! file="Ravl/OS/Threads/Tools/SignalConnectionSet.cc"

#include "Ravl/Threads/SignalConnectionSet.hh"
#include "Ravl/HSetExtra.hh"

namespace RavlN {

  //: Add a connection.
  
  void SignalConnectionSetBodyC::operator+=(const SignalConnectorC &c) { 
    MutexLockC lock(access);
    cons += c; 
  }
  
  //: Remove a connection.
  
  void SignalConnectionSetBodyC::operator-=(const SignalConnectorC &c) {
    MutexLockC lock(access);
    cons -= c; 
  }
  
  //: Test if the connection set is empty.
  
  bool SignalConnectionSetBodyC::IsEmpty() const {
    MutexLockC lock(access);
    return cons.IsEmpty();
  }

  //: Disconnect everything.
  
  void SignalConnectionSetBodyC::DisconnectAll(bool waitThreadsExit) {
    MutexLockC lock(access);
    DListC<SignalConnectorC> sigs = HSet2DList(cons);
    cons.Empty();
    lock.Unlock();
    
    for(DLIterC<SignalConnectorC> it(sigs);it;it++)
      const_cast<SignalConnectorC &>(*it).Disconnect(waitThreadsExit);
  }
}



