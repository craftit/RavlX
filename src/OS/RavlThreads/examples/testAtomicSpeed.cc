// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2009, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////////////
//! author="Charles Galambos"
//! userlevel=Normal
//! docentry="Ravl.API.OS.Threads"
//! lib=RavlThreads
//! file="Ravl/OS/Threads/Tools/testAtomicSpeed.cc"

#include "Ravl/Atomic.hh"
#include "Ravl/Stream.hh"
#include "Ravl/OS/Date.hh"

// Short program to measure the speed of atomic operations.

int main(int nargs,char **argv) {
  
  ravl_atomic_t counter;
  ravl_atomic_set(&counter,0);
  RavlN::DateC start = RavlN::DateC::NowUTC();
  const int loopSize = 100000000;
  int sum = 0;
  for(int i= 0;i < loopSize;i++) {
    ravl_atomic_inc(&counter);
    ravl_atomic_inc(&counter);
    sum += ravl_atomic_dec_and_test(&counter);
    sum += ravl_atomic_dec_and_test(&counter);
  }
  RavlN::DateC done = RavlN::DateC::NowUTC();
  
  // We print the sums in the follow line to ensure they are not optimised away
  std::cerr << "Operation time=" << ((done - start).Double() * 1e9 / (4*loopSize)) << "ns  (Sums=" <<sum << ")\n";
  
  return 0;
}
