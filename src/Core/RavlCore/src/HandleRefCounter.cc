// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Base/HandleRefCounter.cc"

#include "Ravl/HandleRefCounter.hh"
#include "Ravl/Assert.hh"
#include "Ravl/Calls.hh"

#define DODEBUG 0
#if DODEBUG
#include "Ravl/Stream.hh"
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  //: Destructor.
  
  HandleRefCounterBodyC::~HandleRefCounterBodyC() { 
    ONDEBUG(cerr << "HandleRefCounterBodyC::~HandleRefCounterBodyC(), Called. trig=" << trig.IsValid() << "\n");
    if(trig.IsValid())
      trig.Invoke();
  }
  
  static bool StackCalls(TriggerC &oldTrigger,TriggerC &newTrigger) {
    newTrigger.Invoke();
    oldTrigger.Invoke();
    return true;
  }
  
  //: Add a new trigger to call on delete.
  
  void HandleRefCounterBodyC::AddDestructionOp(const TriggerC &newTrigger) {
    ONDEBUG(cerr << "HandleRefCounterBodyC::AddDestructionOp(), Called. trig=" << trig.IsValid() << "\n");
    if(!trig.IsValid())
      trig = newTrigger;
    else
      trig = Trigger(&StackCalls,trig,newTrigger);
  }
  
}




