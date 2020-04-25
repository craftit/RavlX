// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlThreads
//! file="Ravl/OS/Threads/Tools/testSignal.cc"
//! author="Charles Galambos"
//! docentry="Ravl.API.OS.Signals"
//! userlevel=Develop

#include "Ravl/Threads/Signal.hh"
#include "Ravl/Threads/Signal1.hh"
#include "Ravl/Threads/Signal2.hh"
#include "Ravl/Threads/Signal3.hh"
#include "Ravl/Threads/SignalConnectionSet.hh"
#include "Ravl/Option.hh"
#include "Ravl/RandomMersenneTwister.hh"
#include "Ravl/Threads/LaunchThread.hh"
#include "Ravl/Threads/Semaphore.hh"
#include "Ravl/Collection.hh"
#include "Ravl/Random.hh"
#include "Ravl/Calls.hh"
#include "Ravl/SmartPtr.hh"
#include "Ravl/OS/Date.hh"

using namespace RavlN;

int callcount = 0;

bool test1() {
  callcount++;
  return true;
}

bool test2(int x) {
  callcount += 2;
  return true;
}

bool test3(bool x) {
  callcount += 2;
  return true;
}

bool test_2(bool x,int z) {
  callcount += 2;
  return true;
}

bool test_3(bool x,int z,float y) {
  callcount += 2;
  return true;
}

class TestClassC {
public:
  bool test1() {
    callcount++;
    return true;
  }
  
  bool test2(int x) {
    callcount += 2;
    return true;
  }
  
  bool test3(bool x) {
    callcount += 2;
    return true;
  }

  bool test_2(bool x,int n) {
    callcount += 2;
    return true;
  }

  bool test_3(bool x,int n,RealT y) {
    callcount += 2;
    return true;
  }

}; 

class TestClassBodyC 
  : public RCBodyVC
{
public:
  bool test0() {
    callcount++;
    return true;
  }
  
  bool test1(int x) {
    callcount++;
    return true;
  }
  
  bool test2(int x,float y) {
    callcount++;
    return true;
  }
  
  bool test3(int x,float y,bool z) {
    callcount++;
    return true;
  }
}
;

int testBasic() {
  
  Signal0C sig1(true);
  
  
  SignalConnectorC c1 = Connect(sig1,&test1);
  SignalConnectorC c2 = Connect(sig1,&test2);
  
  sig1();
  if(callcount != 3) {
    std::cerr<< "Signal test failed \n";
    return 1;
  }
  c1.Disconnect();
  sig1();
  if(callcount != 5) {
    std::cerr<< "Signal test failed \n";
    return 1;
  }
  SignalConnectorC c3 = Connect(sig1,&test2);
  sig1();
  if(callcount != 9) {
    std::cerr<< "Signal test failed \n";
    return 1;
  }
  Signal0C sig2(true);
  Connect(sig1,sig2);
  Connect(sig2,&test2);
  sig1();
  if(callcount != 15) {
    std::cerr<< "Signal test failed  " << callcount << "\n";
    return 1;
  }

  Signal0C sig6(true);
  
  SmartPtrC<TestClassBodyC> handle(new TestClassBodyC);
  ConnectPtr(sig6,handle,&TestClassBodyC::test3);
  ConnectPtr(sig6,handle,&TestClassBodyC::test2);
  ConnectPtr(sig6,handle,&TestClassBodyC::test1);
  ConnectPtr(sig6,handle,&TestClassBodyC::test0);
  sig6();
  if(callcount != 19) 
    return __LINE__;
  
  // Some extra checks.
  Signal1C<bool> sig3(true);
  Signal2C<bool,int> sig4(true,1);
  Signal3C<bool,int,RealT> sig5(true,1,2.2);

  SignalConnectorC c4 = Connect(sig3,&test3);
  SignalConnectorC c5 = Connect(sig4,&test3);

  TestClassC aclass;
  SignalConnectorC c4a = Connect(sig3,aclass,&TestClassC::test3);
  SignalConnectorC c5a = Connect(sig4,aclass,&TestClassC::test_2);
  SignalConnectorC c6a = Connect(sig5,aclass,&TestClassC::test_3);

  SignalConnectorC c4b = ConnectR(sig3,aclass,&TestClassC::test3);
  SignalConnectorC c5b = ConnectR(sig4,aclass,&TestClassC::test_2);
  SignalConnectorC c6b = ConnectR(sig5,aclass,&TestClassC::test_3);
  
  Connect(sig2,sig4);
  Connect(sig3,sig4);
  Connect(sig5,sig3);
  
  return 0;
}

volatile bool done = false;
SemaphoreC semaThreadDone(0);

bool invokeThread(SArray1dC<Signal0C> signals,RCRWLockC rwLock) {
  // We want to avoid using global locks so use a local random number generator 
  // seeded from the global one.
  RandomMersenneTwisterC rmt(RandomInt());
  CollectionC<SignalConnectorC> connections(40);
  CollectionC<SignalConnectorC> invokes(40);
  TestClassC aclass;
  
  while(!done) {
    switch(rmt.Int() % 7) {
    case 0: 
    case 1: {
      // Invoke a signal
      RWLockHoldC hold(rwLock,RWLOCK_READONLY);
      IntT id = Abs(rmt.Int()) % signals.Size();
      Signal0C sig = signals[id];
      hold.Unlock();
      sig.Invoke();
    } break;
    case 2:
    case 3: {
      // Change a connection
      if(connections.Size() > signals.Size()) {
	IntT nc = connections.Size();
	IntT cid = Abs(rmt.Int()) % nc;
	connections[cid].Disconnect();
	connections.Delete(cid);
      }
      IntT id1 = Abs(rmt.Int()) % (signals.Size()-1);
      // Generate an id with a larger value so we never create a loop
      IntT id2 = (Abs(rmt.Int()) % (signals.Size()-(id1+1))) + id1 + 1;
      RWLockHoldC hold(rwLock,RWLOCK_READONLY);
      Signal0C sig1 = signals[id1];
      Signal0C sig2 = signals[id2];
      hold.Unlock();
      connections.Insert(Connect(sig1,sig2));
    } break;
    case 4: {
      // Replace a signal
      RWLockHoldC hold(rwLock,RWLOCK_WRITE); 
      IntT id = Abs(rmt.Int()) % signals.Size();
      signals[id] = Signal0C(true);
    } break;
    case 5: {
      // Disconnect everything from a signal
      RWLockHoldC hold(rwLock,RWLOCK_READONLY); 
      IntT id = Abs(rmt.Int()) % signals.Size();
      Signal0C sig = signals[id];
      hold.Unlock();
      sig.DisconnectAll();
    } break;
    case 6: {
      // Change a connection
      if(invokes.Size() > signals.Size()) {
	IntT nc = invokes.Size();
	IntT cid = Abs(rmt.Int()) % nc;
	invokes[cid].Disconnect();
	invokes.Delete(cid);
      }
      IntT id1 = Abs(rmt.Int()) % (signals.Size());
      RWLockHoldC hold(rwLock,RWLOCK_READONLY);
      Signal0C sig1 = signals[id1];
      hold.Unlock();
      SignalConnectorC nc;
      switch(Abs(rmt.Int()) % 15) {
      case 0: nc = Connect(sig1,&test1); break;
      case 1: nc = Connect(sig1,&test2); break;
      case 2: nc = Connect(sig1,&test3); break; 
      case 3: nc = Connect(sig1,&test_2); break;
      case 4: nc = Connect(sig1,&test_3); break;
      case 5: nc = Connect(sig1,aclass,&TestClassC::test1); break;
      case 6: nc = Connect(sig1,aclass,&TestClassC::test2); break;
      case 7: nc = Connect(sig1,aclass,&TestClassC::test3); break;
      case 8: nc = Connect(sig1,aclass,&TestClassC::test_2); break;
      case 9: nc = Connect(sig1,aclass,&TestClassC::test_3); break;
      case 10: nc = ConnectR(sig1,aclass,&TestClassC::test1); break;
      case 11: nc = ConnectR(sig1,aclass,&TestClassC::test2); break;
      case 12: nc = ConnectR(sig1,aclass,&TestClassC::test3); break;
      case 13: nc = ConnectR(sig1,aclass,&TestClassC::test_2); break;
      case 14: nc = ConnectR(sig1,aclass,&TestClassC::test_3); break;
      }
      
      invokes.Insert(nc);
    } break;
    }
  }
  semaThreadDone.Post();
  return true;
}


int testInterconnect() {

  // Test various dissconect situations
  {
    SignalConnectionSetC cs;
    Signal0C sig1(true);
    Signal0C sig2(true);
    SignalConnectorC sc = Connect(sig1,sig2);
    sc.Disconnect();
    if(sc.IsConnected()) return __LINE__;
  }
  
  {
    SignalConnectionSetC cs;
    Signal0C sig1(true);
    Signal0C sig2(true);
    SignalConnectorC sc = Connect(sig1,sig2);
    sig1.Invalidate();
    if(sc.IsConnected()) return __LINE__;
    sc.Disconnect();
  }
  
  {
    SignalConnectionSetC cs;
    Signal0C sig1(true);
    Signal0C sig2(true);
    SignalConnectorC sc = Connect(sig1,sig2);
    sig2.Invalidate();
    if(sc.IsConnected()) return __LINE__;
    sc.Disconnect();
  }

  // Now really hammer the connect dissconnect stuff.
  
  SArray1dC<Signal0C> signals(40);
  RCRWLockC rwLock(true);
  for(SArray1dIterC<Signal0C> it(signals);it;it++) {
    *it = Signal0C(true);
  }
  
  std::cerr << "Starting thread test. \n";
  IntT numberOfThreads = 10;
  for(int i = 0;i < numberOfThreads;i++)
    LaunchThread(Trigger(&invokeThread,signals,rwLock));
  RavlN::Sleep(20);
  std::cerr << "Shutting down. \n";
  done = true;
  for(int i = 0;i < numberOfThreads;i++)
    semaThreadDone.Wait();
  std::cerr << "Test done. Passed ok. \n";
  return 0;
}


int main(int argc,char **argv)
{
  int ln;
  if((ln = testBasic()) != 0) {
    std::cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  if((ln = testInterconnect()) != 0) {
    std::cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  
  //  Launch(ae,&ExampleC::DoItArg,1);
  std::cerr << "testSignal(): Passed. \n";
  return 0;
}


template class RavlN::Signal1C<IntT>;
template class RavlN::Signal1BodyC<IntT>;

template class RavlN::Signal2C<IntT,RealT>;
template class RavlN::Signal2BodyC<IntT,RealT>;
template class RavlN::Signal2FuncBodyC<IntT,RealT>;
template class RavlN::Signal2FuncC<IntT,RealT>;
template class RavlN::SignalConnector2BodyC<IntT,RealT>;
template class RavlN::Signal2MethodC<IntT,RealT,Signal0C>;
template class RavlN::SignalInterConnect2C<IntT,RealT>;

template class RavlN::Signal3C<IntT,RealT,UIntT>;
template class RavlN::Signal3BodyC<IntT,RealT,UIntT>;
template class RavlN::Signal3FuncBodyC<IntT,RealT,UIntT>;
template class RavlN::Signal3FuncC<IntT,RealT,UIntT>;
template class RavlN::SignalConnector3BodyC<IntT,RealT,UIntT>;
template class RavlN::Signal3MethodC<IntT,RealT,UIntT,Signal0C>;
template class RavlN::SignalInterConnect3C<IntT,RealT,UIntT>;
