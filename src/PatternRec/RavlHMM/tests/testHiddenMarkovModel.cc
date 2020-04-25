// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd. and University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! author="Charles Galambos, Akiel Khan"
//! rcsid="$Id$"
//! lib=RavlHMM
//! file="Ravl/PatternRec/HiddenMarkovModels/testHiddenMarkovModel.cc"
//! docentry="Ravl.API.Pattern Recognition.Markov Processes"

#include "Ravl/HiddenMarkovModel.hh"
#include "Ravl/Option.hh"


using namespace RavlN;
int main()
{
  RealT initTrans[] = {
    0.5, 0.2, 0.3,
    0.4, 0.2, 0.4,
    0.1, 0.6, 0.3
  };
  
  RealT initObs[] = {
    0.6, 0.2, 0.15, 0.05,
    0.25, 0.25, 0.25, 0.25,
    0.05, 0.1, 0.35, 0.5,
  };
  
  HiddenMarkovModelC hmm(MatrixC(3, 3, initTrans), MatrixC(3, 4, initObs));
  std::cout << "HMM has the transition matrix: " << hmm.Transition() << "\n"
      << "and observation matrix: " << hmm.Observation() << std::endl;
  
  VectorC istate(1.0/3, 1.0/3, 1.0/3); //Initial state probabilities
  
// Observations specified as vectors:
  VectorC observations[] = {
    VectorC(0,0,1,0),
    VectorC(0,1,0,0),
    VectorC(1,0,0,0), 
    VectorC(0,0,0,1),
    VectorC(0,0,1,0),
    VectorC(1,0,0,0),
    VectorC(1,0,0,0),
    VectorC(1,0,0,0),
    VectorC(1,0,0,0),
    VectorC(0,1,0,0),
    VectorC(0,1,0,0),
    VectorC(0,1,0,0),
  };
  
  UIntT T = sizeof(observations)/sizeof(observations[0]);
  SArray1dC<VectorC> obsvecarray(observations, T, false);
  std::cout << "Observed sequence (as simple array of vectors): " << obsvecarray;
  RealT prob1 = hmm.ObsSeqProbability( obsvecarray, istate );
  std::cout << "Probability of observing the sequence is " << prob1 << std::endl;
  
  SArray1dC<UIntT> path1;
  hmm.Viterbi( obsvecarray, istate, path1);
  std::cout << "Most likely path through states is " << path1 << std::endl;
  
  // Same observation as above, but specified as indices between 0 and observationM.size()-1
  
  UIntT obsseq[] = { 2, 1, 0, 3, 2, 0, 0, 0, 0, 1, 1, 1 };
  SArray1dC<UIntT> obsindices(obsseq, T, false); 
  std::cout << "**************************************************\n";
  std::cout << "Observed sequence (as simple array of indices): \n" << obsindices;
  RealT prob2 = hmm.ObsSeqProbability( obsindices, istate );
  std::cout << "Probability of observing the sequence is " << prob2 << std::endl;
  SArray1dC<UIntT> path2;
  hmm.Viterbi( obsindices, istate, path2);
  std::cout << "Most likely path through states is \n" << path2 << std::endl;
  return 0;
}
