// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////////////
//! author="Lee Gregory"
//! docentry="Ravl.API.Core.Calls"
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Base/exTrigger.cc"

#include "Ravl/Stream.hh"
#include "Ravl/DList.hh" 
#include "Ravl/DLIter.hh" 
//#include "Ravl/Trigger.hh"
#include "Ravl/Calls.hh"
#include "Ravl/CallMethodRefs.hh"

using namespace RavlN ; 



// Some Test Functions 
// --------------------
 
bool Func1 (void) 
{
  std::cerr << "\n Func1 Called - \t with no arguments" ; 
  return true ; 
}


bool Func2 (RealT a) 
{
  std::cerr << "\n Func2 Called - \t with arguments " << a ; 
  return true ; 
}


bool Func3 (UIntT * pa, const IntT & b ) 
{
  std::cerr << "\n Func3 Called - \t with arguments " << pa << " and " << b ;
  return true ; 
}


 
// A Class with some test Methods 
// ------------------------------
class MethodCallsC 
{
  
 public: 
  MethodCallsC () 
    {} 
  //: Default Constructor 
  
  MethodCallsC (const MethodCallsC & other) 
    { std::cerr << "\n Copy constructor called MethodCallsC " ; }
  //: Copy Constructor 

  void Method1 (void) 
    { std::cerr << "\n Method1 Called - \t with no arguments"  ; }
  //: Method 1 
  
  RealT Method2 (IntT & a, RealT & b ) 
    { std::cerr << "\n Method2 Called - \t with arguments " << a << " and " << b  ; return 0 ;  }
  // Method 2
}; 



// Main Program - demonstrate how triggers work 

int main (void) 
{ 



  MethodCallsC methods ;                                 // a class with some methods ..
  DListC<TriggerC> trigList ;                            // a list of triggers to be invoked later 
 
  // add some triggers to the list 
  trigList.InsLast( Trigger  ( &Func1) ) ; 
  trigList.InsLast( Trigger  ( &Func2, 2.0) ) ; 
  trigList.InsLast( Trigger  ( &Func3, NULL, 3 ) ) ; 
  trigList.InsLast( Trigger  ( methods, & MethodCallsC::Method1 )) ;      // this will make a copy of object
  trigList.InsLast( TriggerR ( methods, & MethodCallsC::Method2, 4, 5.0 )) ;   // uses a reference to a method, ie copy constructor wont be called 
  
  // now invoke all triggers in the list 
  for ( DLIterC<TriggerC> iter ( trigList) ; iter ; ++iter ) 
      iter.Data().Invoke() ; 


  return 0 ; 
}
