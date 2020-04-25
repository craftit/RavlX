// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_NLPSTEP_HEADER
#define RAVLLOGIC_NLPSTEP_HEADER 1
//! rcsid="$Id$"
//! docentry="Ravl.API.Logic.Planning"
//! author="Charles Galambos"
//! lib=RavlLogicNLP
//! file="Ravl/Logic/NonLinearPlanner/NLPStep.hh"

#include "Ravl/Logic/MinTerm.hh"

namespace RavlLogicN {
  
  //! userlevel=Normal
  //: Step in nonlinear plan.
  
  class NLPStepC 
  {
  public:
    NLPStepC()
      : confidence(0)
    {}
    //: Default constructor.
    
    NLPStepC(const LiteralC &nact,const MinTermC &npre,const MinTermC &npost,RealT nConfidence = 1)
      : act(nact),
	pre(npre),
	post(npost),
	confidence(nConfidence)
    {}
    //: Constructor.

    NLPStepC(const LiteralC &nact,const LiteralC &npre,const LiteralC &npost,RealT nConfidence = 1)
      : act(nact),
	pre(npre,false),
	post(npost,false),
	confidence(nConfidence)
    {}
    //: Constructor.
    
    NLPStepC ReplaceVars(HashC<LiteralC,LiteralC> &subs) const;
    //: Replace variables in step with new ones.
    // The mapping between old and new variables is in subs.
    
    bool Substitute(const BindSetC &subs,NLPStepC &newStep) const;
    //: Replace variables in step with values from 'subs.
    
    StringC Name() const;
    //: Name of step.
    
    const LiteralC &Action() const
    { return act; }
    //: Access action
    
    const MinTermC &PreCondition() const
    { return pre; }
    //: Access pre-condition.
    
    const MinTermC &PostCondition() const
    { return post; }
    //: Access post-condition.
    
    RealT Confidence() const
    { return confidence; }
    //: Access confidence in step being correct.
    
  protected:
    LiteralC act;  // Action id.
    MinTermC pre;  // Precondition for step.
    MinTermC post; // Post condition for step.
    RealT confidence; // Confidence in step working.
  };
  
  ostream &operator<<(ostream &strm,const NLPStepC &step);
  //: Write out step.
  
  BinOStreamC &operator<<(BinOStreamC &strm,const NLPStepC &step);
  //: Save to binary stream.
  
  BinIStreamC &operator>>(BinIStreamC &strm,NLPStepC &step);
  //: Load from binary stream.
  
}

#endif
