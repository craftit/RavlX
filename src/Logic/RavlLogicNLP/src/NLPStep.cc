// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlLogicNLP
//! file="Ravl/Logic/NonLinearPlanner/NLPStep.cc"

#include "Ravl/Logic/NLPStep.hh"
#include "Ravl/PointerManager.hh"
#include "Ravl/BinStream.hh"

namespace RavlLogicN {

  //: Name of step.
  
  StringC NLPStepC::Name() const {
    return act.Name() + "{" + pre.Name() + " -> " + post.Name() + "}";
  }  
  
  NLPStepC NLPStepC::ReplaceVars(HashC<LiteralC,LiteralC> &subs) const {
    LiteralC xAct;  // Action id.
    MinTermC xPre;  // Precondition for step.
    MinTermC xPost; // Post condition for step.
    bool change = act.ReplaceVars(subs,xAct);
    change |= pre.ReplaceVars(subs,xPre);
    change |= post.ReplaceVars(subs,xPost);
    return NLPStepC(xAct,xPre,xPost,confidence);
  }
  
  //: Replace variables in step with values from 'subs.
  
  bool NLPStepC::Substitute(const BindSetC &subs,NLPStepC &newStep) const {
    act.Substitute(subs,newStep.act);
    pre.Substitute(subs,newStep.pre);
    post.Substitute(subs,newStep.post);
    newStep.confidence = confidence;
    return true;
  }
  
  ostream &operator<<(ostream &strm,const NLPStepC &step) {
    strm << step.Action() << "{" << step.PreCondition() << " -> " << step.PostCondition() << "}";
    return strm;
  }
  //: Write out step.
  
  //: Save to binary stream.
  
  BinOStreamC &operator<<(BinOStreamC &strm,const NLPStepC &step) {
    strm << ObjIO(step.Action()) << ObjIO(step.PreCondition()) << ObjIO(step.PostCondition()) << step.Confidence();
    return strm;
  }
  
  //: Load from binary stream.
  
  BinIStreamC &operator>>(BinIStreamC &strm,NLPStepC &step) {
    LiteralC act;
    MinTermC pre,post;
    RealT conf;
    strm >> ObjIO(act) >> ObjIO(pre) >> ObjIO(post) >> conf;
    step = NLPStepC(act,pre,post,conf);
    return strm;
  }
  
}
