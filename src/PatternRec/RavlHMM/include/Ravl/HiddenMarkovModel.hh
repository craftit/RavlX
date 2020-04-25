// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_HIDDENMARKOVMODEL_HEADER
#define RAVL_HIDDENMARKOVMODEL_HEADER
//! author="Charles Galambos, Akiel Khan"
//! rcsid="$Id$"
//! lib=RavlHMM
//! docentry="Ravl.API.Pattern Recognition.Markov Processes"
//! file="Ravl/PatternRec/HiddenMarkovModels/HiddenMarkovModel.hh"
//! example="exHiddenMarkovModel.cc"

#include "Ravl/Matrix.hh"
#include "Ravl/Vector.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: Hidden Markov Model
  // Basic functionality for 1st-order HMM.  Does not contain training algorithm.
  
  class HiddenMarkovModelC {
  public:
    HiddenMarkovModelC()
    {}
    //: Default contructor.
    
    HiddenMarkovModelC(const MatrixC &nTransition,const MatrixC &nObservation);
    //: Contructor.
    
    MatrixC &Transition()
    { return transition; }
    //: Access transition matrix.
    
    MatrixC &Observation()
    { return observation; }
    //: Accesss observation matrix
    
    VectorC Forward(const VectorC &state,const VectorC &observation);
    //: Compute the probability of next state, given the current state and an observation.
    
    VectorC Forward(const VectorC &state, const UIntT &obsIndex);
	//: Compute the probability of next state, given the current state and an observation.
	
    RealT ObsSeqProbability(const SArray1dC<VectorC> &observations, const VectorC &initProb);
    //: Compute the probability of observing a given sequence of observations.
    
    RealT ObsSeqProbability( const SArray1dC<UIntT> &obsIndices, const VectorC &initProb);
	//: Compute the probability of observing a given sequence of observations.

    RealT Viterbi(const SArray1dC<VectorC> &observations,const VectorC &initState,SArray1dC<UIntT> &path);
    //: Given a sequence of observations find the most likely path through the states.
    
    RealT Viterbi(const SArray1dC<UIntT> &observations, const VectorC &initState, SArray1dC<UIntT> &path);
    //: Given sequence of observations find the most likely path through the states.

  protected:
    MatrixC transition;
    MatrixC observation;
	
  private:
  	VectorC ObsIndexToVector( const UIntT &obsIndex );
  	//: Convert observation symbol from integer to VectorC form.
  	
  	UIntT ObsVectorToIndex( const VectorC &obsVec );
  	//: Convert observation specified in VectorC form to integer.
  };
}

#endif
