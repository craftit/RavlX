// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlThreads
//! file="Ravl/OS/Threads/Tools/Signal.cc"

#include "Ravl/Threads/Signal.hh"
#include "Ravl/Threads/Signal1.hh"
#include "Ravl/Threads/Signal2.hh"
#include "Ravl/Threads/Signal3.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/IntrDLIter.hh"
#include "Ravl/Assert.hh"

namespace RavlN {
  //// Signal0BodyC //////////////////////////////////////////////
  
  Signal0BodyC::~Signal0BodyC()
  { DisconnectAll(); };
  
  //: Connect new output.
  
  void Signal0BodyC::Connect(SignalConnector0BodyC &con) {
    con.sigAccess = access;     // Take a reference to the rwlock
    
    RWLockHoldC hold(access,RWLOCK_WRITE); //
    // Make new array 1 bigger than old one.
    SArray1dC<SignalConnectorC> newouts(outputs.Size() + 1);
    for(SArray1dIter2C<SignalConnectorC,SignalConnectorC> it(outputs,newouts);it;it++)
      it.Data2() = it.Data1();   // Copy existing connections
    con.ind = outputs.Size();  // Put in new index.
    con.sigbod = this;          // Make sure we've got the right signal.
    newouts[con.ind] = SignalConnectorC(con);
    outputs = newouts;
  }
  
  //: Disonnect old output.
  
  void Signal0BodyC::Disconnect(SignalConnector0BodyC &con,bool waitThreadsExit,RCRWLockC &sigRWLock) {
    RWLockHoldC hold(sigRWLock,RWLOCK_WRITE); // Obtain write lock.
    int ind = con.ind;
    if(ind < 0)
      return ; // Disconnected already.
    // Make new array 1 smaller than old one.
    SArray1dC<SignalConnectorC> newouts(outputs.Size() - 1);
    for(int j = 0;j < ind;j++)
      newouts[j] = outputs[j];
    for(int i = ind;i < ((IntT) newouts.Size());i++) {
      SignalConnectorC &sc = outputs[i+1];
      sc.Body().ind = i; // Update id.
      newouts[i] = sc;
    }
    con.ind = -1; // Flag as disconnected.
    outputs = newouts; // Replace outputs.
    
    // Make sure we're hold a reference to the exe lock, otherwise
    // another thread may destroy it before we're finished with it.
    RCRWLockC refExecLock = execLock;
    hold.Unlock();
    
    if(waitThreadsExit) {
      // This ensures all threads have left this signal on exit.
      // Unfortunatly if this disconnect is being called from this signal
      // it will deadlock. 
      RWLockHoldC holdExec(refExecLock,RWLOCK_WRITE);
    }
  }
  
  //: Disconnect an input.
  
  void Signal0BodyC::DisconnectInput(SignalInterConnect0BodyC &in) {
    RWLockHoldC hold(access,false);
    if(in.IsSelfPointing())
      return ; // Done already.
    in.Unlink(); 
    in.SetSelfPointing(); // Flag as deleted.
    hold.Unlock();
  }
  
  //: Connect an input.
  
  void Signal0BodyC::ConnectInput(SignalInterConnect0BodyC &in) {
    RWLockHoldC hold(access,false);
    in.sigTargetAccess = access;
    inputs.InsFirst(in);
  }
  
  
  //: Send default signal.
  
  bool Signal0BodyC::Invoke() {
    // Lock output list while we obtain a handle to it.
    RWLockHoldC hold(access,RWLOCK_READONLY); 
    SArray1dIterC<SignalConnectorC> it(outputs);
    // Make sure we're hold a reference to the exe lock, otherwise
    // another thread may destroy it before we're finished with it.
    RCRWLockC refExecLock = execLock;
    hold.Unlock();
    // Flag that we're executing signal code.
    // This is used to ensure all threads have left the signal handlers
    // before they are disconnected.
    RWLockHoldC holdExec(refExecLock,RWLOCK_READONLY);
    bool ret = true;
    for(;it;it++) 
      ret &= it.Data().Invoke();
    return ret;
  }
  
  //: Find interconnection between this and 'targ'.
  // Assumes there's only one connect!!
  
  SignalInterConnect0C Signal0BodyC::FindInterConnect(const Signal0C &targ)  {
    if(!targ.IsValid())
      return SignalInterConnect0C();
    RWLockHoldC hold(access,RWLOCK_READONLY); // Make access to the output array thread safe.
    SArray1dIterC<SignalConnectorC> it(outputs);
    hold.Unlock(); // Modifications are always made by on a copy of 'outputs', so don't need the lock anymore.
    for(;it;it++) {
      SignalInterConnect0BodyC *inter = dynamic_cast<SignalInterConnect0BodyC *>(&it.Data().Body());
      if(inter == 0)
	continue; // Not an interconnector.
      if(&inter->Target() == &targ.Body())
	return SignalInterConnect0C(*inter); // Found !
    }
    return SignalInterConnect0C();
  }

  //: Access the number of connections to the signal.
  
  IntT Signal0BodyC::NumOutputs() const {
    RWLockHoldC hold(access,RWLOCK_READONLY); 
    return outputs.Size();
  }
  
  //: Access the number of inputs to the signal.
  
  IntT Signal0BodyC::NumInputs() const {
    RWLockHoldC hold(access,RWLOCK_READONLY); 
    return inputs.Size();
  }
  
  //: Disconnect other signal from this one.
  // 'targ' must be the target of the signal.
  
  bool Signal0BodyC::Disconnect(Signal0C &targ) {
    SignalInterConnect0C  inter = FindInterConnect(targ);
    if(!inter.IsValid())
      return false; // Not found.
    inter.Disconnect();
    return true;
  }

  //: Disconnect all inputs to this signal.
  
  void Signal0BodyC::DisconnectInputs(bool waitThreadsExit) {
    RWLockHoldC hold(access,false); 
    while(!inputs.IsEmpty()) {
      SignalInterConnect0C con(inputs.First());
      hold.Unlock();
      con.Disconnect(waitThreadsExit);
      con.Invalidate(); // Make sure its released here..
      hold.LockWr();
    }
  }
  
  //: Disconnect all outputs from this signal.
  
  // This method is always called with a reference count held
  // so we don't have to worry about the destructor being called
  // on another thread.
  
  void Signal0BodyC::DisconnectOutputs(bool waitThreadsExit) {
    
    RWLockHoldC hold(access,false); 
    hold.Unlock();
    do {
      hold.LockRd();
      SArray1dIterC<SignalConnectorC> it(outputs);
      hold.Unlock();
      // Prevent threads from running while we disconnect the outputs.
      // Note this may create a deadlock if the thread is being disconnect
      // from by a currently running signal.
      for(;it;it++) 
	it.Data().Disconnect();
    } while(outputs.Size() > 0) ;
    if(waitThreadsExit) {
      // This ensures all threads have left this signal on exit.
      // Unfortunatly if this disconnect is being called from this signal
      // it will deadlock. 
      RWLockHoldC holdExec(execLock,RWLOCK_WRITE);
    }
  }
  
  //: Disconnect all signals.
  
  void Signal0BodyC::DisconnectAll(bool waitThreadsExit) {
    DisconnectInputs();
    // Only have to wait for threads to exit once, so do it at the end of DisconnectOutputs, 
    // if we want to at all.
    DisconnectOutputs(waitThreadsExit);
  }
  
  //: Called when owner handles drop to zero.
  
  void Signal0BodyC::ZeroOwners() {
    DisconnectAll(false);
  }
  
  //// SignalIterConnect0 ////////////////////////////////////////////////
  
  SignalInterConnect0C::SignalInterConnect0C(Signal0C &from,Signal0C &targ)
    : SignalConnectorC(*new SignalInterConnect0BodyC())
  { 
    // Need to do this after we've got a reference to the object.
    Body().Connect(from,targ); 
  }
  
  
  SignalConnector0BodyC::~SignalConnector0BodyC() 
  { Disconnect(false); }
  
  //: Pass signal on.
  
  bool SignalConnector0BodyC::Invoke() {
    std::cerr << "SignalConnector0BodyC::Invoke(), ABSTRACT method Called. \n";
    RavlAssert(0);
    return true;
  }
  
  //: Disconnect from input list.
  
  void SignalConnector0BodyC::Disconnect(bool waitThreadsExit) {    
    if(ind < 0) // Not connected ?
      return; 
    RavlAssert(sigbod != 0);
    sigbod->Disconnect(*this,waitThreadsExit,sigAccess); 
  }
  
  std::ostream &operator<<(std::ostream &os,const SignalConnectorC &sc) {
    RavlAssertMsg(0,"not implemented");
    return os;
  }

  std::istream &operator>>(std::istream &os,SignalConnectorC &sc) {
    RavlAssertMsg(0,"not implemented");
    return os;
  }

  BinOStreamC &operator<<(BinOStreamC &strm,const SignalConnectorC  &vertex) {
    RavlAssertMsg(0,"not implemented");
    return strm;
  }
  //: Binary stream output.
  // Not implemented
  
  BinIStreamC &operator>>(BinIStreamC &strm,SignalConnectorC  &vertex) {
    RavlAssertMsg(0,"not implemented");
    return strm;
  }
  //: Binary stream input.
  // Not implemented
  
  ///////// SignalInterConnect0BodyC //////////////////////////////////////////////////
  
  //: Constructor.
  
  void SignalInterConnect0BodyC::Connect(Signal0BodyC &from,Signal0BodyC &targ) {
    // Setup desination for signal first.
    targetHandle = RCLayerC<RCLayerBodyC>(targ,RCLH_CALLBACK);
    target = &targ;
    targ.ConnectInput(*this);
    
    // Now setup source.
    SignalConnector0BodyC::Connect(from);
  }
  
  //: Constructor.
  
  void SignalInterConnect0BodyC::Connect(Signal0C &from,Signal0C &targ) {
    // Setup desination for signal first.
    targetHandle = RCLayerC<RCLayerBodyC>(targ.Body(),RCLH_CALLBACK);
    target = &targ.Body();
    targ.Body().ConnectInput(*this);
    
    // Now setup source.
    SignalConnector0BodyC::Connect(from);
  }
 
  //: Destructor.
  
  SignalInterConnect0BodyC::~SignalInterConnect0BodyC() { 
    Disconnect(false); 
  }
  
  //: Invoke signal.
  
  bool SignalInterConnect0BodyC::Invoke() {
    RWLockHoldC hold(sigTargetAccess,RWLOCK_READONLY);
    if(target == 0) // Has it been deleted?
      return false;
    
    // Copy pointer in case its zero'd
    Signal0BodyC *tmp = target;
    // Make handle to target signal to ensure its not deleted before we're finished.
    RCLayerC<RCLayerBodyC> localHandle = targetHandle;
    hold.Unlock();
    return tmp->Invoke();
  }
  
  void SignalInterConnect0BodyC::Disconnect(bool waitThreadsExit)  { 
    RWLockHoldC hold(sigTargetAccess,RWLOCK_WRITE);
    target = 0;
    if(!IsSelfPointing()) {
      Unlink(); 
      SetSelfPointing(); // Flag as deleted.
    }
    // Calling 'Invalidate' on a handle is NOT thread safe, so to avoid problems invalidate the handle
    // with a lock held.  To avoid calling the destructor of the targetHandle with a lock held
    // (which could cause a deadlock) we make a local copy of the the handle which will
    // be free'd after we've released the lock. This ensures only 1
    RCLayerC<RCLayerBodyC> localHandle = targetHandle;
    targetHandle.Invalidate();
    hold.Unlock();
    SignalConnector0BodyC::Disconnect(waitThreadsExit); 
    
  }
}

