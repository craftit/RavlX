// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlLogic
//! file="Ravl/Logic/DecisionTree/DecisionTreeBase.cc"

#include "Ravl/Logic/DecisionTreeBase.hh"
#include "Ravl/Logic/DecisionTreeLeaf.hh"
#include "Ravl/Logic/DecisionTreeBranchBinary.hh"
#include "Ravl/PatternRec/DataSet2Iter.hh"
#include "Ravl/BlkQueue.hh"
#include "Ravl/PointerManager.hh"
#include "Ravl/BinStream.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlLogicN {
  
  //: Default constructor.
  // Use the default discriminator.
  
  DecisionTreeBaseBodyC::DecisionTreeBaseBodyC() 
    : discriminator(true)
  {}
  
  //: Constructor.
  DecisionTreeBaseBodyC::DecisionTreeBaseBodyC(const DiscriminatorC &desc)
    : discriminator(desc)
  {}

  //: Construct from a stream.
  
  DecisionTreeBaseBodyC::DecisionTreeBaseBodyC(istream &strm) 
  { RavlAssertMsg(0,"Not Implemented."); }
  
  //: Construct from a binary stream.
  
  DecisionTreeBaseBodyC::DecisionTreeBaseBodyC(BinIStreamC &strm) 
  { 
    IntT version;
    strm >> version;
    if(version != 0)
      throw ExceptionOutOfRangeC("DecisionTreeBaseBodyC::DecisionTreeBaseBodyC(BinIStreamC &), Unrecognised version number in stream. ");
    strm >> ObjIO(discriminator) >> root;
  }
  
  //: Save to stream 'out'.
  
  bool DecisionTreeBaseBodyC::Save(ostream &out) const {
    RavlAssertMsg(0,"Not Implemented.");
    return false;
  }
  
  //: Save to binary stream 'out'.
  
  bool DecisionTreeBaseBodyC::Save(BinOStreamC &out) const {
    IntT version = 0;
    out << version << ObjIO(discriminator) << root;
    return true;
  }
  
  //: Find the decision for given 'state'.
  
  DecisionTreeLeafC DecisionTreeBaseBodyC::Find(const StateC &state) {
    DecisionTreeElementC at = root;
    while(at.IsValid()) {
      at = at.Find(state);
      if(at.IsLeaf())
	return DecisionTreeLeafC(at);
    }
    RavlAssert(0); // Should never fail.
    return DecisionTreeLeafC(); // Failed.
  }
  
  //: Add a new example to the tree.
  
  bool DecisionTreeBaseBodyC::Add(const StateC &state,const LiteralC &decision) {
    ONDEBUG(cerr << "DecisionTreeBaseBodyC::Add(state,decision), Called \n");
    DecisionTreeElementC last,at = root;
    while(at.IsValid()) {	
      if(at.IsLeaf()) {
	DecisionTreeLeafC leaf(at);
	if(leaf.Decision() == decision) {
	  RavlAssert(leaf.Examples().IsValid());
	  leaf.Examples().AddExample(state,decision);
	  return true; // Already 
	}
	// Need to replace the leaf.
	//discriminator.Distinguish( 
	RavlAssertMsg(0,"Need some work here. ");
	return true;
      }
      last = at;
      at = last.Find(state);
      RavlAssert(at.IsValid()); // Shouldn't have any dangling branches in tree.
    }
    DecisionExamplesC initalExamples(true);
    initalExamples.AddExample(state,decision);
    root = DecisionTreeLeafC(decision,initalExamples);
    return false;
  }


  //: Train the decision tree with the new data.
  
  void DecisionTreeBaseBodyC::Train(const DataSet2C<SampleStateC,SampleLiteralC> &data) {
    ONDEBUG(cerr << "DecisionTreeBaseBodyC::Train(), Called \n");
    BlkQueueC<Tuple2C<DecisionTreeElementC *,DecisionExamplesC> > todo;
    DecisionExamplesC initalExamples(data);
    todo.InsLast(Tuple2C<DecisionTreeElementC *,DecisionExamplesC>(&root,initalExamples));
    
    while(!todo.IsEmpty()) {
      Tuple2C<DecisionTreeElementC *,DecisionExamplesC> place = todo.GetFirst();
      RavlAssert(place.Data2().Decisions() != 0);
      if(place.Data2().Decisions() == 1) {
	// We're at a leaf.
	*place.Data1() = DecisionTreeLeafC(place.Data2().ProbableDecision(),place.Data2());
	continue;
      }
      LiteralC test = discriminator.BestDiscriminator(place.Data2());
      
      DecisionExamplesC tset(true),fset(true);
      place.Data2().Seperate(test,tset,fset);
      DecisionTreeBranchBinaryC branch(test);
      branch.Examples() = place.Data2();
      *place.Data1() = branch; // Setup new branch.
      
      todo.InsLast(Tuple2C<DecisionTreeElementC *,DecisionExamplesC>(&branch.TrueChild(),tset));
      todo.InsLast(Tuple2C<DecisionTreeElementC *,DecisionExamplesC>(&branch.FalseChild(),fset));
    }
    //for(DataSet2IterC<SampleStateC,SampleLiteralC> it(data);it;it++)
    //  Add(it.Data1(),it.Data2());
  }

  //: Dump the tree in a human readable form to stream out.
  
  void DecisionTreeBaseBodyC::Dump(ostream &out) const {
    ONDEBUG(cerr << "DecisionTreeBaseBodyC::Dump(), Called \n");
    out << "Decision tree:\n";
    if(!root.IsValid()) {
      out << " Empty\n";
      return ;
    }
    root.Dump(out,0);
  }
  
  //: Build a rule set from the decision tree.
  
  StateC DecisionTreeBaseBodyC::BuildRuleSet() const {
    StateC ret(true);
    if(!root.IsValid())
      return ret;
    LiteralC x;
    root.BuildRuleSet(x,ret);
    return ret;
  }

  //------------------------------------------------------------------
  
  void DecisionTreeBaseC::Dump(ostream &out) const { 
    if(!IsValid()) {
      cerr << "(NIL TREE)\n";
      return ;
    }
    Body().Dump(out); 
  }
  //: Dump the tree in a human readable form to stream out.

}
