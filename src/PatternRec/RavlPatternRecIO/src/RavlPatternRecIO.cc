// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRecIO
//! file="Ravl/PatternRec/IO/RavlPatternRecIO.cc"

namespace RavlN {
  
  extern void InitRavlClassifierLinearCombinationIO();
  extern void InitRavlClassifierWeakLinearIO();
  
  extern void InitRavlSampleIO();
  extern void InitRavlDataSetIO();
  extern void InitRavlClassifierIO();
  extern void InitRavlClassifier2IO();
  extern void InitRavlFunctionIO();
  extern void InitRavlGaussianMixtureIO();
  extern void InitRavlDistanceIO();
  
  void RavlPatternRecIO() {
    InitRavlSampleIO();
    InitRavlDataSetIO();
    InitRavlClassifierIO();
    InitRavlClassifier2IO();
    InitRavlFunctionIO();
    InitRavlGaussianMixtureIO();
    InitRavlDistanceIO();
    
    // Make sure these get linked.
    
    InitRavlClassifierLinearCombinationIO();
    InitRavlClassifierWeakLinearIO();
  }
}
