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
//! lib=RavlThreads
//! file="Ravl/OS/Threads/Posix/exThread.cc"

#include "Ravl/Threads/Thread.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/Stream.hh"

using namespace RavlN ; 

//: MyThread Body 
//: -------------------------------------------------------------
class MyThreadBodyC : public ThreadBodyC 
{
 public:
  
  MyThreadBodyC () {}
  //: Default Constructor 
  
  int Start(void) 
    {
      for ( IntT x = 1 ; x<= 5 ; ++x ) 
	{ RavlN::Sleep(1) ; std::cout << "\n MyThreadC counting " << x ; }
      return 1 ; 
    }
  //: Start Method
  // The code in this method gets excecuted when Execute is called. 

};


//: MyThreaC Handle 
//: -------------------------------------------------------------
class MyThreadC : public ThreadC 
{
 public: 
  
  MyThreadC (void) 
    : ThreadC (*new MyThreadBodyC () ) { }
  // : Defualt Constructor 

 protected:
  MyThreadBodyC & Body()
    { return static_cast<MyThreadBodyC &>(ThreadC::Body()); }
  //: Access body.
    
  const MyThreadBodyC & Body() const
    { return static_cast<const MyThreadBodyC &>(ThreadC::Body()); }
  //: Access body.
};






int main (void) 
{
  
  MyThreadC thread ;                  // an instance of the threaded class  
  thread.Execute() ;                  // start it !
  
  for ( IntT x = 1 ; x<= 10 ; ++x ) 
  {
    RavlN::Sleep(1) ; 
    std::cout << "\n main counting " << x ; 
  }


return 0 ; 
} 
