// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! author="Charles Galambos"
//! lib=RavlThreads
//! file="Ravl/OS/Threads/Tools/exSignal.cc"
//! docentry="Ravl.API.OS.Signals"
//! date="23/09/1999"
//! userlevel=Normal

#include "Ravl/Threads/Signal1.hh"

using namespace RavlN;

bool PrintA() {
  std::cout << "PrintA called. \n";
  return true;
}

bool PrintB() {
  std::cout << "PrintB called.\n";
  return true;
}

int main() {
  
  // Create two signals with a single integer argument.
  
  Signal0C sig1(true);
  Signal0C sig2(true);
  
  // Connect sig1 to a function which prints message A.
  
  Connect(sig1,PrintA); 

  // Connect sig2 to a function which prints message B
  
  Connect(sig2,PrintB); 

  // Connect signal 1 to signal 2.  
  // If sig1 is invoked, sig2 will also be triggered
  
  Connect(sig1,sig2);
  
  // Trigger signal 1.  Both PrintA and PrintB will be called.
  
  std::cout << "Triggering signal 'sig1' \n";
  sig1(); // Send a signal.
  
  // Trigger signal 2.  Only PrintB will be called.
  
  std::cout << "Triggering signal 'sig2' \n";
  sig2();
  
  return 0;
}
