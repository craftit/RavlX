// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SAMPLELITERAL_HEADER
#define RAVL_SAMPLELITERAL_HEADER 1
//! rcsid="$Id$"
//! lib=RavlLogic
//! file="Ravl/Logic/DecisionTree/SampleLiteral.hh"
//! author="Charles Galambos"
//! docentry="Ravl.API.Logic.Decision Tree"

#include "Ravl/PatternRec/SampleDiscrete.hh"
#include "Ravl/Logic/Literal.hh"
#include "Ravl/HSet.hh"

namespace RavlLogicN {
  using namespace RavlN;
  
  //! userlevel=Normal
  //: Set of example literals.
  
  class SampleLiteralC
    : public SampleDiscreteC<LiteralC>
  {
  public:
    SampleLiteralC()
    {}
    //: Default constructor.
    
    SampleLiteralC(SizeT sizeEst)
      : SampleDiscreteC<LiteralC>(sizeEst)
    {}
    //: Constructor.

    HSetC<LiteralC> Components() const;
    //: Make a list of all LiteralC and sub LiteralC's in the sample.
    
  };
  
}


#endif
