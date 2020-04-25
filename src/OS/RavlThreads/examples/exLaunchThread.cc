// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////////////
//! author="Lee Gregory"
//! userlevel=Normal
//! docentry="Ravl.API.OS.Threads"
//! rcsid="$Id$"
//! lib=RavlThreads
//! file="Ravl/OS/Threads/Tools/exLaunchThread.cc"

#include "Ravl/Stream.hh"
#include "Ravl/Calls.hh"
#include "Ravl/CallMethodRefs.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/Threads/LaunchThread.hh"
#include "Ravl/Array1d.hh"
using namespace RavlN ; 



// Some Test Functions 
// --------------------
 
bool Func1 (void) 
{
  for (UIntT i = 1 ; i <= 5 ; ++i ) 
    {
      Sleep(1) ; 
      std::cout << "\n Func1 counting:" << i ; 
    }
  return true ; 
}


bool Func2 (IntT & dummy) 
{
  std::cout << "\n Func2 const argument is: " << dummy;
  for ( UIntT i = 1 ; i <= 10 ; i++ ) 
    { 
      Sleep(1) ; 
      std::cout << "\n Func2 counting:" << i ; 
    }
  return true ;
}


// This function shows how a big object can be modified, so that the modified results
// are available in the main thread (object "a").  This doesn't happen for "b", because
// the handle for "b" is reassigned, and the link with the "b" in the main thread is
// lost.
bool Func3 (Array1dC<UIntT> &a, Array1dC<UIntT> &b) 
{
  for ( UIntT i = 1 ; i <= 15 ; i++ ) 
    { 
      Sleep(1) ; 
      std::cout << "\n Func3 counting:" << i ; 
    }
  a[0] = 6;
  Array1dC<UIntT> c(1);
  c[0] = 8;
  b = c;
  return true ;
}



// Main Program - demonstrate simple use of Launchthread

int main (void) 
{
  Array1dC<UIntT> a(1); a[0] = 0;
  Array1dC<UIntT> b(1); b[0] = 3;

  TriggerC 
    trig1 = Trigger (&Func1),         // thread function with no args
    trig2 = Trigger (&Func2, 0),      // thread function with one const arg
    trig3 = Trigger (&Func3, a, b) ;  // thread function with two big object args
  
  LaunchThreadC thread1 = LaunchThread ( trig1 ) ; 
  LaunchThreadC thread2 = LaunchThread ( trig2 ) ; 
  LaunchThreadC thread3 = LaunchThread ( trig3 ) ; 
  
  for ( UIntT i = 1 ; i <= 5 ; ++i ) 
    { Sleep(1) ; std::cout << "\n Main counting " ; } 

  thread1.WaitForExit() ; 
  std::cout << "\n\n **** Thread 1 Finished " ; 
  thread2.WaitForExit() ; 
  std::cout << "\n\n **** Thread 2 Finished " ; 
  thread3.WaitForExit() ; 
  std::cout << "\n\n **** Thread 3 Finished " ; 

  // Because "a" is a big object, the value set in the thread can be retrieved
  // in the main thread (or any other thread).  On the other hand, because the
  // handle of "b" was reassigned, the link with the body is lost
  std::cout << "\n\n ****** Thread 3 arg values: "<< a[0] << " " << b[0] << std::endl;
  return 0;
}
