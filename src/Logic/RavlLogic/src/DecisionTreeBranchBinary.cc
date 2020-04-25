// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlLogic
//! file="Ravl/Logic/DecisionTree/DecisionTreeBranchBinary.cc"

#include "Ravl/Logic/DecisionTreeBranchBinary.hh"
#include "Ravl/Logic/State.hh"
#include "Ravl/Logic/And.hh"
#include "Ravl/Logic/Not.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/PointerManager.hh"

namespace RavlLogicN {

  DecisionTreeBranchBinaryBodyC::DecisionTreeBranchBinaryBodyC()
  {}

  //: Construct with a test.
  
  DecisionTreeBranchBinaryBodyC::DecisionTreeBranchBinaryBodyC(const LiteralC &ntest)
    : test(ntest)
  {}

  //: Construct from a stream.
  
  DecisionTreeBranchBinaryBodyC::DecisionTreeBranchBinaryBodyC(istream &strm)
    : DecisionTreeBranchBodyC(strm)
  { RavlAssertMsg(0,"Not implemented. "); }
  
  //: Construct from a binary stream.
  
  DecisionTreeBranchBinaryBodyC::DecisionTreeBranchBinaryBodyC(BinIStreamC &strm)
    : DecisionTreeBranchBodyC(strm)
  { strm >> ObjIO(test) >> children[0] >> children[1]; }
  
  //: Save to stream 'out'.
  
  bool DecisionTreeBranchBinaryBodyC::Save(ostream &out) const  { 
    RavlAssertMsg(0,"Not implemented. ");
    return false;
  }
  
  //: Save to binary stream 'out'.
  
  bool DecisionTreeBranchBinaryBodyC::Save(BinOStreamC &out) const {
    if(!DecisionTreeBranchBodyC::Save(out)) return false;
    out << ObjIO(test) << children[0] << children[1];
    return true;
  }
  
  //: Go through the tree building a rule set.
  
  void DecisionTreeBranchBinaryBodyC::BuildRuleSet(const LiteralC &preCond,StateC &ruleSet) const {
    RavlAssert(test.IsValid());
    if(preCond.IsValid()) {
      children[1].BuildRuleSet(preCond * test,ruleSet);
      children[0].BuildRuleSet(preCond * !test,ruleSet);
    } else {
      children[1].BuildRuleSet(test,ruleSet);
      children[0].BuildRuleSet(!test,ruleSet);
    }
  }
  
  //: Dump node in human readable form,
  
  void DecisionTreeBranchBinaryBodyC::Dump(ostream &out,IntT level) const {
    Pad(out,level) << test << "\n";
    Pad(out,level) << "true:\n";
    children[0].Dump(out,level+1);
    Pad(out,level) << "false:\n";
    children[1].Dump(out,level+1);
  }
  
  //: Find next level in the decision tree for given 'state'.
  
  DecisionTreeElementC DecisionTreeBranchBinaryBodyC::Find(const StateC &state) {
    return children[state.Ask(test)];
  }

  RAVL_INITVIRTUALCONSTRUCTOR_FULL(DecisionTreeBranchBinaryBodyC,DecisionTreeBranchBinaryC,DecisionTreeBranchC);
  
}
