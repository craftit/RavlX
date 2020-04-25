// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_AGENTPLAN_HEADER
#define RAVLLOGIC_AGENTPLAN_HEADER 1
//! author="Charles Galambos"
//! docentry="Ravl.API.Logic.Agent"
//! rcsid="$Id$"
//! lib=RavlLogicAgent
//! file="Ravl/Logic/Agent/AgentPlan.hh"

#include "Ravl/Logic/Agent.hh"
#include "Ravl/Logic/NLPlanner.hh"
#include "Ravl/Threads/RWLock.hh"
#include "Ravl/Threads/ThreadEvent.hh"

namespace RavlLogicN {
  
  class AgentPlanC;
  
  //! userlevel=Develop
  //: Planning agent.
  
  class AgentPlanBodyC
    : public AgentBodyC
  {
  public:
    AgentPlanBodyC(DPOPortC<LiteralC> &actionStream);
    //: Constructor.
    //!param: actionStream - Stream to which actions are sent

    virtual bool HandleEvent(const LiteralC &lit);
    //: Handle an event from the enviroment.
    
    virtual bool Run();
    //: Main thread.
    
    virtual bool SetGoal(const LiteralC &newGoal); 
    //: Setup a new goal.
    
  protected:
    friend class AgentPlanC;
    RWLockC access;
    DListC<NLPStepC> model; // Rules in world model.
    LiteralC goal;
    StateC worldState;
    ThreadEventC stateUpdate;
    ThreadEventC goalUpdate;
  };
  
  //! userlevel=Normal
  //: Planning agent.
  
  class AgentPlanC
    : public AgentC
  {
  public:
    AgentPlanC()
    {}
    //: Default constructor.
    
  protected:
    AgentPlanC(AgentPlanBodyC &bod)
      : AgentC(bod)
    {}
    //: Body constructor.
    
    AgentPlanBodyC &Body()
    { return static_cast<AgentPlanBodyC &>(AgentC::Body()); }
    //: Access body.
    
    const AgentPlanBodyC &Body() const
    { return static_cast<const AgentPlanBodyC &>(AgentC::Body()); }
    //: Access body.
    
    bool Run()
    { return Body().Run(); }
    //: Main thread.
    
  public:
    
    bool SetGoal(const LiteralC &newGoal)
    { return Body().SetGoal(newGoal); }
    //: Setup a new goal.
    
    friend class AgentPlanBodyC;
  };

  
}


#endif 
