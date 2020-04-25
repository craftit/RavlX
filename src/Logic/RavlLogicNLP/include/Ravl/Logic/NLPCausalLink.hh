// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_NLPCAUSALLINK_HEADER
#define RAVLLOGIC_NLPCAUSALLINK_HEADER 1
////////////////////////////////////////////////////////////
//! author="Charles Galambos"
//! date="12/12/1996"
//! docentry="Ravl.API.Logic.Planning"
//! rcsid="$Id$"
//! file="Ravl/Logic/NonLinearPlanner/NLPCausalLink.hh"
//! lib=RavlLogicNLP

#include "Ravl/Logic/MinTerm.hh"

namespace RavlLogicN {
  
  //! userlevel=Advanced
  //: Information stored in a causal link.
  // Links may be use to indicate either cause or be pure
  // ordering constraints.
  
  class NLPCausalLinkC 
  {
  public:
    inline NLPCausalLinkC()
    {}
    //: Default, Constructor.
    // If no condition is set the link is used for
    // ordering of actions only.
    
    inline NLPCausalLinkC(const MinTermC &cond)
      : aCond(cond) 
    {}
    //: Constructor.
    
    inline MinTermC &Cond()
    { return aCond;  }
    //: Access expression.
    
    inline const MinTermC &Cond() const
    { return aCond;  }
    //: Constant Access to an expression.
  
    bool IsStepOrder() const
    { return !aCond.IsValid(); }
    //: Is this an ordering link ?
    
    StringC Name() const;
    //: Get name of link.
    
    void Dump(void) const;
    //: Dump to stdout.
  
  private:
    MinTermC aCond;
  };
  
 
}
#endif
