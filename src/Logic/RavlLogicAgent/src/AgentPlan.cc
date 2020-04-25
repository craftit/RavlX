// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlLogicAgent
//! file="Ravl/Logic/Agent/AgentPlan.cc"

//! author="Charles Galambos"

#include "Ravl/Logic/AgentPlan.hh"
#include "Ravl/Logic/State.hh"
#include "Ravl/Logic/NonLinearPlan.hh"
#include "Ravl/OS/Date.hh"

namespace RavlLogicN {
  
  //: Default constructor.
  
  AgentPlanBodyC::AgentPlanBodyC(DPOPortC<LiteralC> &actionStream) 
    : AgentBodyC(actionStream)
  {}
  
  //: Handle an event from the enviroment.
  
  bool AgentPlanBodyC::HandleEvent(const LiteralC &lit) {
    RWLockHoldC hold(access,RWLOCK_WRITE);
    worldState.Tell(lit);
    hold.Unlock();
    stateUpdate.Post();
    return true;
  }
  
  //: Setup a new goal.
  
  bool AgentPlanBodyC::SetGoal(const LiteralC &newGoal) {
    RWLockHoldC hold(access,RWLOCK_WRITE);
    goal = newGoal;
    hold.Unlock();
    goalUpdate.Post();
    stateUpdate.Post();
    return true;
  }
  
  //: Main thread.
  
  bool AgentPlanBodyC::Run() {
    RWLockHoldC hold(access,RWLOCK_READONLY);
    NLPlannerC planner(model);
    hold.Unlock();
    
    while(1) {
      // Build plan.
      RWLockHoldC hold(access,RWLOCK_READONLY);
      LiteralC currentGoal = goal;
      StateC currentState = worldState.Copy();
      goalUpdate.Reset();
      hold.Unlock();
      DListC<NLPStepC> plan = planner.Apply(currentState,MinTermC(currentGoal,true));
      if(goalUpdate)
	continue;
      if(plan.IsEmpty()) {
	stateUpdate.Wait(); // Wait for something to happen.
	continue;
      }
      
      // Do steps from plan.
      
      for(DLIterC<NLPStepC> it(plan);it;it++) {
	
	// Wait for step precondition to become true.
	LiteralC pre = it->PreCondition();
	hold.LockRd();
	stateUpdate.Reset();
	DateC start(true);
	while(!goalUpdate && !worldState.Ask(pre)) {
	  hold.Unlock();
	  // Wait a limited amount of time for change.
	  if(!stateUpdate.Wait(0.2)) {
	    DateC now(true);
	    if((now - start).Double() > 0.5) {
	      //This isn't working, force a replan.
	      goalUpdate.Post(); 
	      break;
	    }
	  }
	}
	hold.Unlock();
	if(goalUpdate) // Have our goals change ?
	  break;
	
	// Do action.
	DoAction(it->Action());
      }
    }
    
    return true;
  }

}
