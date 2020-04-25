// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_HANDLEREFCOUNTER_HEADER
#define RAVL_HANDLEREFCOUNTER_HEADER 1
////////////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Core.Reference Counting"
//! file="Ravl/Core/Base/HandleRefCounter.hh"
//! lib=RavlCore
//! userlevel=Normal
//! author="Charles Galambos"

#include "Ravl/RefCounter.hh"
#include "Ravl/Trigger.hh"

//: Ravl library namespace.

namespace RavlN {  
  
  class HandleRefCounterC;
  
  //! userlevel=Advanced
  //: Handle based reference counter.
  
  class HandleRefCounterBodyC
    : public RCBodyVC
  {
  public:
    HandleRefCounterBodyC()
    {}
    //: Default constructor.
    
    HandleRefCounterBodyC(const TriggerC &ntrig)
      : trig(ntrig)
    {}
    //: Constructor.
    
    virtual ~HandleRefCounterBodyC();
    //: Destructor.
    
    void AddDestructionOp(const TriggerC &ntrig);
    //: Add a new trigger to call on delete.
    
  protected:
    TriggerC trig;
    friend class HandleRefCounterC;
  };
  
  //! userlevel=Advanced
  //: Handle based reference counter.
  // The class RefCounterC is a reference counter which should be always
  // inherited if a class contains pointers to allocated memory and the 
  // memory is shared by several objects. All constructors, a destructor,
  // and an assigment operator must be modified in such class. The destructor
  // is not virtual, therefore the class should be inherited as private
  // or protected.
  // <p>
  // The RefCounterC class allocates its own memory separately from 
  // the memory of the object.
  
  class HandleRefCounterC 
    :  public RCHandleC<HandleRefCounterBodyC>
  {
  public:
    HandleRefCounterC()
      : RCHandleC<HandleRefCounterBodyC>(*new HandleRefCounterBodyC())
    {}
    //: Creates the reference counter.
    
    HandleRefCounterC(const TriggerC &ntrig)
      : RCHandleC<HandleRefCounterBodyC>(*new HandleRefCounterBodyC(ntrig))
    {}
    //: Creates the reference counter and initializes it.
    // 'func' is triggered when references of the object reach 0.
    
    inline
    void SetDestructionOp(const TriggerC &newTrigger) 
    { Body().trig = newTrigger; }
    //: Set trigger to call on delete.
    // Note: This will replace any existing triggers with 
    // the 'newTrigger'
    
    void AddDestructionOp(const TriggerC &newTrigger)
    { Body().AddDestructionOp(newTrigger); }
    //: Add a new trigger to call on delete.
    // This adds an extra operation 'newTrigger' to be executed when the object
    // is deleted, it is called before any already existing trigger is 
    // run
    
    inline UIntT References() const
    { return Body().References(); }
    //: Returns the number of references to this object.  

  };
  
  
   
}


#endif




