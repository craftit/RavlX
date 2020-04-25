// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! author="Charles Galambos"
//! file="Ravl/PatternRec/Cluster/DesignClassifierUnsupervised.cc"

#include "Ravl/PatternRec/DesignClassifierUnsupervised.hh"
#include "Ravl/PatternRec/SampleIter.hh"

namespace RavlN {

  //: Load from stream.
  
  DesignClassifierUnsupervisedBodyC::DesignClassifierUnsupervisedBodyC(istream &strm) 
    : DesignFunctionUnsupervisedBodyC(strm)
  {}
  
  //: Load from binary stream.

  DesignClassifierUnsupervisedBodyC::DesignClassifierUnsupervisedBodyC(BinIStreamC &strm)
    : DesignFunctionUnsupervisedBodyC(strm)
  {}
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DesignClassifierUnsupervisedBodyC::Save (ostream &out) const 
  { return DesignFunctionUnsupervisedBodyC::Save(out); }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DesignClassifierUnsupervisedBodyC::Save (BinOStreamC &out) const
  { return DesignFunctionUnsupervisedBodyC::Save(out); }
  
  //: Create a clasifier.
  
  FunctionC DesignClassifierUnsupervisedBodyC::Apply(const SampleC<VectorC> &in) {
    RavlAssertMsg(0,"DesignClassifierUnsupervisedBodyC::Apply(), Abstract method called. ");
    return FunctionC();
  }
  
  //: Create a clasifier with weights for the samples.
  
  FunctionC DesignClassifierUnsupervisedBodyC::Apply(const SampleC<VectorC> &in,const SampleC<RealT> &weight) {
    RavlAssertMsg(0,"DesignClassifierUnsupervisedBodyC::Apply(), Abstract method called. ");
    return FunctionC();
  }
  
  ///////////////////////////////////////////////////////////

  //: Load from stream.
  
  DesignClassifierUnsupervisedC::DesignClassifierUnsupervisedC(istream &strm)
    : DesignFunctionUnsupervisedC(RAVL_VIRTUALCONSTRUCTOR(strm,DesignClassifierUnsupervisedBodyC))    
  {}
  
  //: Load from binary stream.
  
  DesignClassifierUnsupervisedC::DesignClassifierUnsupervisedC(BinIStreamC &strm)
    : DesignFunctionUnsupervisedC(RAVL_VIRTUALCONSTRUCTOR(strm,DesignClassifierUnsupervisedBodyC))
  {}

}
