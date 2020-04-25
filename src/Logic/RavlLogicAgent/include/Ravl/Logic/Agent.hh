// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_AGENT_HEADER
#define RAVLLOGIC_AGENT_HEADER 1
//! author="Charles Galambos"
//! docentry="Ravl.API.Logic.Agent"
//! rcsid="$Id$"
//! lib=RavlLogicAgent
//! file="Ravl/Logic/Agent/Agent.hh"

#include "Ravl/Logic/LiteralIndex.hh"
#include "Ravl/DP/Port.hh"

namespace RavlLogicN {
  
  //! userlevel=Develop
  //: Abstract agent.
  
  class AgentBodyC
    : public RCBodyVC
  {
  public:
    AgentBodyC(DPOPortC<LiteralC> &actionStream);
    //: Constructor.
    //!param: actionStream - Stream to which actions are sent
    
    virtual bool HandleEvent(const LiteralC &lit);
    //: Handle an event from the enviroment.
    
    virtual bool DoAction(const LiteralC &action);
    //: Peform an action.
    
  protected:
    
    //: Action output.
    DPOPortC<LiteralC> actionStream;
  };
  
  //! userlevel=Normal
  //: Abstract agent.
  
  class AgentC
    : public RCHandleVC<AgentBodyC>
  {
  public:
    AgentC()
    {}
    //: Default constructor.
    
  protected:
    AgentC(AgentBodyC &bod)
      : RCHandleVC<AgentBodyC>(bod)
    {}
    //: Body constructor.
    
    AgentBodyC &Body()
    { return RCHandleVC<AgentBodyC>::Body(); }
    //: Access body.
    
    const AgentBodyC &Body() const
    { return RCHandleVC<AgentBodyC>::Body(); }
    //: Access body.
    
  public:
    
    bool HandleEvent(const LiteralC &lit)
    { return Body().HandleEvent(lit); }
    //: Handle an event from the enviroment.
    
    bool DoAction(const LiteralC &action)
    { return Body().DoAction(action); }
    //: Peform an action.
    
  };
}


#endif
