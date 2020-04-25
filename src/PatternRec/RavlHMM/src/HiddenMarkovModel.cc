// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlHMM
//! file="Ravl/PatternRec/HiddenMarkovModels/HiddenMarkovModel.cc"

#include "Ravl/HiddenMarkovModel.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/SArray1dIter3.hh"
#include "Ravl/SArray1dIterR.hh"

namespace RavlN {
  
  //: Constructor.
  
  HiddenMarkovModelC::HiddenMarkovModelC(const MatrixC &nTransition,const MatrixC &nObservation)
    : transition(nTransition),
      observation(nObservation)
  {
    RavlAssert(transition.Rows() == observation.Rows());
    for(UIntT i = 0;i < transition.Rows();i++) {
      if((Abs(transition.SliceRow(i).Sum() - 1) > 0.00000001)) {
	cerr << "WARNING: Transition matrix row " << i << " sums to " << transition.SliceRow(i).Sum() << "\n";
      }
    }
    for(UIntT i = 0;i < observation.Rows();i++) {
      if(Abs(observation.SliceRow(i).Sum() - 1) > 0.00000001) {
	cerr << "WARNING: Observation matrix row " << i << " sums to " << observation.SliceRow(i).Sum() << "\n";
      }
    }
  }
  
  //: Compute the probabilty of next state, given the current state and an observation.
  
  VectorC HiddenMarkovModelC::Forward(const VectorC &state,const VectorC &nObservation) {
    VectorC stateProb = (observation * nObservation);
    //cerr << "StateProb=" << stateProb <<"\n";
    return (transition.T() * state) * stateProb;
  }
  
  VectorC HiddenMarkovModelC::Forward( const VectorC &state, const UIntT &obsIndex )
  {
	  VectorC obs = ObsIndexToVector(obsIndex);
	  return Forward(state, obs);
  }

  class HMMViterbiStateC {
  public:
    HMMViterbiStateC()
    {}
    
    HMMViterbiStateC(UIntT n)
      : source(n),
	prob(n)
    {}
    //: Constructor.
    
    SArray1dC<UIntT> &Source()
    { return source; }
    //: Access likeliest source.
    
    SArray1dC<RealT> &Prob()
    { return prob; }
    //: Access probability of source.
    
    VectorC &StateProbability()
    { return stateProb; }
    //: Access probability of states a this time.
    
  protected:
    SArray1dC<UIntT> source;
    SArray1dC<RealT> prob;
    VectorC stateProb;
  };

  //: Compute the probability of observing a given sequence of observations.
  
  RealT HiddenMarkovModelC::ObsSeqProbability(const SArray1dC<VectorC> &observations, const VectorC &initProb)
  {
	  UIntT T = observations.Size();
	  VectorC stateprob = observation * observations[0] * initProb;
	  for ( IndexC i = 1; i < T; ++i )
	  {
		  stateprob = Forward(stateprob, observations[i]);
	  }
	  return stateprob.Sum();
  }
  
  //: Compute the probability of observing a given sequence of observations.
  
  RealT HiddenMarkovModelC::ObsSeqProbability(const SArray1dC<UIntT> &obsIndices, const VectorC &initProb)
  {
	  UIntT T = obsIndices.Size();
	  VectorC stateprob = initProb * observation.SliceColumn(obsIndices[0]);
	  for ( IndexC i = 1; i < T; ++i )
	  {
		  stateprob = Forward( stateprob, obsIndices[i]);
	  }
	  return stateprob.Sum();
  }
  
  //: Given a sequence of observations find the most likely sequence of states.

  RealT HiddenMarkovModelC::Viterbi(const SArray1dC<UIntT> &obsIndices,const VectorC &initState,SArray1dC<UIntT>&path)
  {
	  SArray1dC<VectorC> observations(obsIndices.Size());
	  for (SArray1dIter2C<UIntT,VectorC> it(obsIndices, observations); it; ++it)
		  it.Data2() = ObsIndexToVector(it.Data1());
	  
	  return Viterbi( observations, initState, path);
}
  
  //: Given a sequence of observations find the most likely sequence of states.
  
  RealT HiddenMarkovModelC::Viterbi(const SArray1dC<VectorC> &observations,const VectorC &initState,SArray1dC<UIntT> &path) {
    path = SArray1dC<UIntT>(observations.Size());
    SArray1dC<HMMViterbiStateC> stateProb(observations.Size());
    
    // Compute forward probabilities.
    VectorC last = initState;
    for(SArray1dIter2C<HMMViterbiStateC,VectorC> oit(stateProb,observations);oit;oit++) {
      HMMViterbiStateC &here = oit.Data1();
      VectorC tranObs = observation * oit.Data2();
      //cerr << "Tran=" << tranObs << "\n";
      VectorC prob(last.Size());
      here.Source() = SArray1dC<UIntT> (last.Size());
      for(SArray1dIter3C<UIntT,RealT,RealT> it(here.Source(),prob,tranObs);it;it++) {
	UIntT i = it.Index().V();
	SArray1dC<RealT> slice = SArray1dC<RealT>(transition.SliceColumn(i));
	SArray1dIter2C<RealT,RealT> sit(slice,last);
	IndexC max = sit.Index();
	RealT maxval = sit.Data1() * sit.Data2();
	for(;sit;sit++) {
	  RealT val = sit.Data1() * sit.Data2();
	  if(val > maxval) {
	    max = sit.Index();
	    maxval = val;
	  }
	}
	//cerr << " " << maxval << "\n";
	it.Data1() = max.V();
	it.Data2() = maxval * it.Data3();
      }
      here.Prob() = prob;
      last = prob;
      //cerr << "Prob=" << prob <<"\n";
      //cerr << here.Source() << "\n";
    }
    // Trace path.
    IndexC max = stateProb[stateProb.Size()-1].Prob().IndexOfMax();
    //cerr << "INit=" << max << "\n";
    for(int i = stateProb.Size()-1;i >= 0;i--) {
      //cerr << i << "=" << max << "\n";
      path[i] = max.V();
      max = stateProb[i].Source()[max];
    }
    //cerr << "Path=" << path << "\n";
    return true;
  }

  
  VectorC HiddenMarkovModelC::ObsIndexToVector(const UIntT &obsIndex)
  {
	  VectorC obs(observation.Cols());
	  obs.Fill(0);
	  obs[obsIndex] = 1;
	  return obs;
  }
  
  UIntT HiddenMarkovModelC::ObsVectorToIndex(const VectorC &obsVec)
  {
	  RealT Index = RealT(obsVec.IndexOfMax());
	  return static_cast<UIntT>(Index);
  }
}
