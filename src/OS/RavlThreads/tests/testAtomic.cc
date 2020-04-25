// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlThreads
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Misc"
//! userlevel=Develop
//! file="Ravl/OS/Threads/Tools/testAtomic.cc"

#include "Ravl/Threads/Semaphore.hh"
#include "Ravl/Threads/LaunchThread.hh"
#include "Ravl/Atomic.hh"
#include "Ravl/Stream.hh"
#include "Ravl/Random.hh"

using namespace RavlN;

SemaphoreC testDone(0);
SemaphoreC testRunning(0);

#ifdef RAVL_ATOMIC_INIT
ravl_atomic_t testCount = RAVL_ATOMIC_INIT(0);
ravl_atomic_t counter = RAVL_ATOMIC_INIT(0);
#else
ravl_atomic_t testCount(0);
ravl_atomic_t counter(0);
#endif

bool TestAtomic(int loopVal) {
  //cerr << "TestAtomic(), Called. \n";
  OSYield();

  for(int i = 0;i < 500000;i++)
    ravl_atomic_inc(&counter);
  testRunning.Wait();
  for(int i = 0;i < 5000000 + loopVal;i++) {
    ravl_atomic_inc(&counter);
    ravl_atomic_dec(&counter);
  }
  for(int i = 0;i < 500000;i++)
    ravl_atomic_dec(&counter);
  
  //cerr << "TestAtomic(), Done. \n";
  if(ravl_atomic_dec_and_test(&testCount))
    testDone.Post();
  return true;
}

int main() {
  // Check the functions actualy do something.
  std::cerr << "Setting up memory test. \n";
  ravl_atomic_set(&testCount,0);
  ravl_atomic_set(&counter,0);
  ravl_atomic_t var;
  ravl_atomic_set(&var,0);
  std::cerr << "Init:" << ravl_atomic_read(&var) << ". \n";
  ravl_atomic_inc(&var);
  std::cerr << "Inc:" << ravl_atomic_read(&var) << ". \n";

  if(ravl_atomic_read(&var) != 1) {
    std::cerr << "ERROR: ravl_atomic_inc failed. \n";
    return 1;
  }
  ravl_atomic_dec(&var);
  if(ravl_atomic_read(&var) != 0) {
    std::cerr << "ERROR: ravl_atomic_dec failed. \n";
    return 1;
  }  
  ravl_atomic_inc(&var);
  ravl_atomic_inc(&var);
  if(ravl_atomic_dec_and_test(&var)) {
    std::cerr << "ERROR: ravl_atomic_dec_and_test failed. (false positive) \n";
    return 1;
  }
  if(!ravl_atomic_dec_and_test(&var)) {
    std::cerr << "ERROR: ravl_atomic_dec_and_test failed. (false negative) \n";
    return 1;
  }
  
  std::cerr << "Starting atomic test. \n";
  for(int tn = 0;tn < 10;tn++) {
    for(int i = 0;i < 6;i++) {
      ravl_atomic_inc(&testCount);
      LaunchThread(&TestAtomic,i*11);
    }
    for(int i = 0;i < 6;i++) {
      testRunning.Post();
    }
    std::cerr << "Running test " << tn <<". \n";

    testDone.Wait();

    if(ravl_atomic_read(&counter) != 0) {
      std::cerr << "Counter=" << ravl_atomic_read(&counter) << "\n";
      std::cerr << "FAILED!\n" ;
      return 1;
    }
  }
  std::cerr << "PASSED!\n" ;
  return 0;
}
