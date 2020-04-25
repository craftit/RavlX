// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlHMM
//! file="Ravl/PatternRec/HiddenMarkovModels/exHiddenMarkovModel.cc"

#include "Ravl/HiddenMarkovModel.hh"
#include "Ravl/Option.hh"

using namespace RavlN;

RealT initTrans2[] = {
  0.0, 0.0, 0.0 , 0.0,
  1.0, 0.0, 0.0 , 0.0,
  0.0, 1.0, 0.5 , 0.0,
  0.0, 0.0, 0.5 , 1.0
};

RealT initObs2[] = {
  0.25, 0.1,
  0.25, 0.1,
  0.25, 0.7,
  0.25, 0.1,
};

VectorC observations2[] = {
  VectorC(1,0),
  VectorC(1,0),
  VectorC(0,1),
  VectorC(1,0), 
  VectorC(1,0)
};

unsigned int expected2[] = {
  1,
  2,
  2,
  3,
  3
};


int main() {
  MatrixC trans(4,4,initTrans2);
  MatrixC obs(4,2,initObs2);
  
  VectorC sv(4);
  sv.Fill(0);
  for(int i = 0;i < 4; i++) {
    sv[i] = 1;
    //cerr << "Value=" << (trans * sv).Sum() << "\n";
    sv[i] = 0;
  }
  
  HiddenMarkovModelC mm(trans,obs);
  VectorC state(1,0,0,0);
  SArray1dC<VectorC> vec(5);
  for(int i = 0;i < (int)vec.Size();i++) {
    state = mm.Forward(state,observations2[i]);
    vec[i] = observations2[i];
    std::cerr << "State=" << state <<"\n";
  }
  VectorC istate(1,0,0,0);
  SArray1dC<UIntT> path;
  
  std::cerr << "Viterbi:\n";
  mm.Viterbi(vec,istate,path);
  
  std::cerr << "Path=" << path << "\n";
  for(int i = 0;i < (int)vec.Size();i++) {
    if(expected2[i] != path[i])
      return 1;
  }
  
  return 0;
}


