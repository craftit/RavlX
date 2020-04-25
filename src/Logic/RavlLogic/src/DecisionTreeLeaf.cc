// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlLogic
//! file="Ravl/Logic/DecisionTree/DecisionTreeLeaf.cc"

#include "Ravl/Logic/DecisionTreeLeaf.hh"
#include "Ravl/Logic/Tuple.hh"
#include "Ravl/Logic/NamedLiteral.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/PointerManager.hh"

namespace RavlLogicN {

  DecisionTreeLeafBodyC::DecisionTreeLeafBodyC()
  {}

  //: Constructor.
  
  DecisionTreeLeafBodyC::DecisionTreeLeafBodyC(const LiteralC &ndecision,const DecisionExamplesC  &nexamples) 
    : DecisionTreeElementBodyC(nexamples),
      decision(ndecision)
  {}
  
  //: Construct from a stream.
  
  DecisionTreeLeafBodyC::DecisionTreeLeafBodyC(istream &strm)
    : DecisionTreeElementBodyC(strm)
  { RavlAssertMsg(0,"Not implemented. "); }
  
  //: Construct from a binary stream.
  
  DecisionTreeLeafBodyC::DecisionTreeLeafBodyC(BinIStreamC &strm)
    : DecisionTreeElementBodyC(strm)
  { strm >> ObjIO(decision); }
  
  //: Save to stream 'out'.
  
  bool DecisionTreeLeafBodyC::Save(ostream &out) const { 
    RavlAssertMsg(0,"Not implemented. ");
    return false;
  }
  
  //: Save to binary stream 'out'.
  
  bool DecisionTreeLeafBodyC::Save(BinOStreamC &out) const { 
    if(!DecisionTreeElementBodyC::Save(out)) return false;
    out << ObjIO(decision);
    return true;
  }
  
  //: Go through the tree building a rule set.
  
  void DecisionTreeLeafBodyC::BuildRuleSet(const LiteralC &preCond,StateC &ruleSet) const {
    ruleSet.Tell(Tuple(Literal("Rule"),preCond,decision));
  }
  
  //: Dump node in human readable form,
  
  void DecisionTreeLeafBodyC::Dump(ostream &out,IntT level) const {
    Pad(out,level) << "Leaf: " << decision << "\n";
  }
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(DecisionTreeLeafBodyC,DecisionTreeLeafC,DecisionTreeElementC);
  
}
