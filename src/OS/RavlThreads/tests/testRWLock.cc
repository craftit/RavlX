// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////////
//! lib=RavlThreads
//! file="Ravl/OS/Threads/Posix/testRWLock.cc"

#include "Ravl/Threads/RWLock.hh"
#include "Ravl/Threads/Thread.hh"
#include "Ravl/Threads/Semaphore.hh"
#include "Ravl/Stream.hh"
#include "Ravl/AMutex.hh"
#include "Ravl/OS/Date.hh"

#include <stdlib.h>

using namespace RavlN;

RWLockC *ARWLock;
SemaphoreC *Done;

int Val1 = 0;
int Val2 = 0;
int Val3 = 0;

bool pass = true;

class RWLTestBodyC 
  : public ThreadBodyC 
{
public:
  RWLTestBodyC(int nNum) { Num = nNum; }

  virtual int Start(void);
private:
  int Num;
};

class RWLTestC 
  : public ThreadC 
{
public:
  RWLTestC(int nNum)
    : ThreadC(*new RWLTestBodyC(nNum))
    {}
};

int RWLTestBodyC::Start(void) {  
  for(;Num > 0;Num--) {
    if((rand() % 10) == 0) {      // Check old values.
#if 1
      if((rand() % 5) == 0) {
        if(ARWLock->TryRdLock()) { // Write new values.
          if(Val1 != Val2)
            pass = false;
          if(Val2 != Val3)
            pass = false;
          RavlN::Sleep(0);
          if(Val1 != Val3)
            pass = false;
          RavlN::Sleep(0);
          if(Val1 != Val3)
            pass = false;
          ARWLock->UnlockRd();
        }
      }
#endif
#if 1
      if((rand() % 5) == 0) {
        if(ARWLock->RdLock(0.1)) { // Write new values.
          if(Val1 != Val2)
            pass = false;
          if(Val2 != Val3)
            pass = false;
          RavlN::Sleep(0);
          if(Val1 != Val3)
            pass = false;
          RavlN::Sleep(0);
          if(Val1 != Val3)
            pass = false;
          ARWLock->UnlockRd();
        }
      }
#endif
      ARWLock->RdLock();
      if(Val1 != Val2)
	pass = false;
      if(Val2 != Val3)
	pass = false;
      RavlN::Sleep(0);
      if(Val1 != Val3)
	pass = false;
      RavlN::Sleep(0);
      if(Val1 != Val3)
	pass = false;
      ARWLock->UnlockRd();
      
    } else {
      
      if((rand() % 5) == 0) {
#if 1
        if(ARWLock->TryWrLock()) { // Write new values.
          Val1 = rand();
          RavlN::Sleep(0);
          Val2 = Val1;
          RavlN::Sleep(0);
          Val3 = Val2;
          RavlN::Sleep(0);
          ARWLock->UnlockWr();
        }
#endif
      }

      if((rand() % 5) == 0) {
#if 1
        if(ARWLock->WrLock(0.1)) { // Write new values.
          Val1 = rand();
          RavlN::Sleep(0);
          Val2 = Val1;
          RavlN::Sleep(0);
          Val3 = Val2;
          RavlN::Sleep(0);
          ARWLock->UnlockWr();
        }
#endif
      }

      ARWLock->WrLock();  // Write new values.
      Val1 = rand();
      RavlN::Sleep(0);
      Val2 = Val1;
      RavlN::Sleep(0);
      Val3 = Val2;
      RavlN::Sleep(0);
      ARWLock->UnlockWr();
      
    }
    RavlN::Sleep(0);
  }
  Done->Post();
  return 0;
}

static const int TestSize = 16;

int main() {
  int i;
  //AMutexC testMutex(true);
  for(int j = 0;j < 2;j++) {
    std::cerr << "Starting RWLock test. WriterPref:" << j << "\n";
    Done = new SemaphoreC(0);
    ARWLock = new RWLockC(j == 0 ? RavlN::RWLOCK_PREFER_READERS : RavlN::RWLOCK_PREFER_WRITERS);

     // Boil the RWLock for a bit.
    for(i = 0;i < TestSize;i++)
      RWLTestC(20000).Execute();
  
    for(i = 0;i < TestSize;i++) {
      Done->Wait();
      std::cerr << i << " \n";
    }
    if(!pass) {
      std::cerr << "ERROR in test.. \n";
      exit(1);
    }
    delete Done;
    delete ARWLock;
  }
  std::cout << "Test passed. \n";
  exit(0);
}
