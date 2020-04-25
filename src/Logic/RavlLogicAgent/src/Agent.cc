// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlLogicAgent
//! file="Ravl/Logic/Agent/Agent.cc"

#include "Ravl/Logic/Agent.hh"

namespace RavlLogicN {
   
  //: Default constructor.
  
  AgentBodyC::AgentBodyC(DPOPortC<LiteralC> &nactionStream)
    : actionStream(nactionStream)
  {}
  
  //: Handle an event from the enviroment.
  
  bool AgentBodyC::HandleEvent(const LiteralC &lit) {
    
    return true;
  }
  
  //: Peform an action.
  
  bool AgentBodyC::DoAction(const LiteralC &action) {
    if(actionStream.IsValid())
      return actionStream.Put(action);
    cerr << "AgentBodyC::DoAction(), Warning: No action stream. \n";
    return false;
  }

}
