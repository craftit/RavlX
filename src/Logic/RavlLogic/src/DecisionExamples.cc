// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlLogic
//! file="Ravl/Logic/DecisionTree/DecisionExamples.cc"

#include "Ravl/StdMath.hh"
#include "Ravl/Logic/DecisionExamples.hh"
#include "Ravl/HashIter.hh"
#include "Ravl/PatternRec/DataSet2Iter.hh"
#include "Ravl/PointerManager.hh"
#include "Ravl/BinStream.hh"

#define DODEBUG	0
#if DODEBUG
#define ONDEBUG(x) 
#else
#define ONDEBUG(x) x
#endif

namespace RavlLogicN {

  //: Construct from a stream.
  
  DecisionExamplesBodyC::DecisionExamplesBodyC(istream &strm) 
  { RavlAssertMsg(0,"Not implemented. "); }
  
  //: Construct from a binary stream.
  
  DecisionExamplesBodyC::DecisionExamplesBodyC(BinIStreamC &strm) 
  {
    IntT version;
    UIntT lsize,esize;
    strm >> version;
    if(version != 0) 
      throw ExceptionOutOfRangeC("DecisionExamplesBodyC::DecisionExamplesBodyC(BinIStreamC &), Unrecognised version number in stream. ");
    strm >> lsize;
    for(UIntT i = 0;i < lsize;i++) {
      LiteralC lit;
      strm >> ObjIO(lit) >> esize;
      if(esize == 0) {
	examples[lit]; // Create an empty entry.
      } else {
	for(UIntT j = 0;j < esize;j++) {
	  StateC state;
	  strm >> ObjIO(state);
	  AddExample(state,lit);
	}
      }
    }
  }
  
  //: Save to stream 'out'.
  
  bool DecisionExamplesBodyC::Save(ostream &out) const {
    RavlAssertMsg(0,"Not implemented. ");
    return false;
  }
  
  //: Save to binary stream 'out'.
  
  bool DecisionExamplesBodyC::Save(BinOStreamC &out) const {
    IntT version = 0;
    out << version;
    UIntT size = examples.Size();
    out << size;
    for(HashIterC<LiteralC,HSetC<StateC> > it(examples);it;it++) {
      out << ObjIO(it.Key());
      size = it.Data().Size();
      out << size;
      for(HSetIterC<StateC> sit(it.Data());sit;sit++)
	out << ObjIO(*sit);
    }
    return true;
  }
  
  //: Construct from a dataset.
  
  DecisionExamplesBodyC::DecisionExamplesBodyC(const DataSet2C<SampleStateC,SampleLiteralC> &data) {
    for(DataSet2IterC<SampleStateC,SampleLiteralC> it(data);it;it++)
      AddExample(it.Data1(),it.Data2());
  }
  
  //: Add an example to the node.
  // Return's true if example is a new one.
  
  bool DecisionExamplesBodyC::AddExample(const StateC &state,const LiteralC &decision) {
    examples[decision] += state;
    return histogram.Vote(Tuple2C<StateC,LiteralC>(state,decision)) == 1;
  }
  
  //: Remove examples from this node.
  
  bool DecisionExamplesBodyC::RemoveExamples(DecisionExamplesC &ex) {
    for(HashIterC<Tuple2C<StateC,LiteralC>,UIntC> it(ex.Histogram());it;it++) {
      UIntC &c = histogram[it.Key()];
      RavlAssert(examples[it.Key().Data2()].IsMember(it.Key().Data1()));
      if(c == it.Data()) {
	histogram.Unvote(it.Key());
	examples[it.Key().Data2()] -= it.Key().Data1();
      } else
	c -= it.Data(); // Just subtract the appropriate number of examples.
    }
    return true;
  }
  
  //: Get an estimate of the information in decisions.
  
  RealT DecisionExamplesBodyC::DecisionInformation() {
    HistogramC<LiteralC> hist;
    for(HashIterC<Tuple2C<StateC,LiteralC>,UIntC> it(Histogram());it;it++) 
      hist.Vote(it.Key().Data2(),it.Data().v);
    return hist.Information();
  }
  
  //: Dump examples to a stream.
  
  void DecisionExamplesBodyC::Dump(ostream &out) const {
    for(HashIterC<LiteralC,HSetC<StateC> > it(examples);it;it++)
      out << " " << it.Key() << " -> " << it.Data() << "\n";
  }

  //: Split this set into to, ones where the test is true (ttrue) and ones where its false (tfalse).
  
  bool DecisionExamplesBodyC::Seperate(const LiteralC &test,DecisionExamplesC &ttrue,DecisionExamplesC &tfalse) const {
    if(ttrue.IsValid())
      ttrue = DecisionExamplesC(true);
    if(tfalse.IsValid())
      tfalse = DecisionExamplesC(true);
    for(HashIterC<LiteralC,HSetC<StateC> > it(examples);it;it++) {
      for(HSetIterC<StateC> sit(it.Data());sit;sit++) {
	if(sit->Ask(test))
	  ttrue.AddExample(*sit,it.Key());
	else
	  tfalse.AddExample(*sit,it.Key());
      }
    }
    return true;
  }
  
  //: Make a list of decisions.
  
  DListC<LiteralC> DecisionExamplesBodyC::ListDecisions() const {
    DListC<LiteralC> ret;
    for(HashIterC<LiteralC,HSetC<StateC> > it(examples);it;it++)
      ret += it.Key();
    return ret;
  }
  
  //: Get the most likely decision given the examples.
  
  LiteralC DecisionExamplesBodyC::ProbableDecision() const {
    ONDEBUG(cerr << "DecisionExamplesBodyC::ProbableDecision(), Called. \n");
    LiteralC ret;
    IntT maxScore = -1;
    for(HashIterC<LiteralC,HSetC<StateC> > it(examples);it;it++) {
      IntT score = 0;
      for(HSetIterC<StateC> sit(it.Data());sit;sit++)
	score += histogram[Tuple2C<StateC,LiteralC>(*sit,it.Key())];
      if(maxScore < score) {
	ret = it.Key();
	maxScore = score;
      }
    }
    ONDEBUG(cerr << "DecisionExamplesBodyC::ProbableDecision(), Decision=" << ret << ". \n");
    return ret;
  }

}
