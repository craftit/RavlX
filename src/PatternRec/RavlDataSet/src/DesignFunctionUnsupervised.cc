// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/DataSet/DesignFunctionUnsupervised.cc"

#include "Ravl/PatternRec/DesignFunctionUnsupervised.hh"
#include "Ravl/PatternRec/SampleVector.hh"
#include "Ravl/PatternRec/SampleReal.hh"
#include "Ravl/PatternRec/SampleStreamVector.hh"
#include "Ravl/DP/Compose.hh"
#include "Ravl/DP/SPortAttach.hh"

namespace RavlN {

  //: Default constructor.
  DesignFunctionUnsupervisedBodyC::DesignFunctionUnsupervisedBodyC()
  {}

  //: Load from stream.
  
  DesignFunctionUnsupervisedBodyC::DesignFunctionUnsupervisedBodyC(std::istream &strm)
    : DesignerBodyC(strm)
  {}
  //: Load from binary stream.
  
  DesignFunctionUnsupervisedBodyC::DesignFunctionUnsupervisedBodyC(BinIStreamC &strm)
    : DesignerBodyC(strm)
  {}
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DesignFunctionUnsupervisedBodyC::Save (std::ostream &out) const 
  { return DesignerBodyC::Save(out); }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DesignFunctionUnsupervisedBodyC::Save (BinOStreamC &out) const 
  { return DesignerBodyC::Save(out); }
  
  //: Create function from the given data.
  
  FunctionC DesignFunctionUnsupervisedBodyC::Apply(const SampleC<VectorC> &in) {
    RavlAssertMsg(0,"DesignUnsupervisedFunctionBodyC::Apply(const SampleC<VectorC> &,const SampleC<VectorC> &), Abstract method called. \n");
    return FunctionC();
  }
  
  //: Create function from the given data.
  
  FunctionC DesignFunctionUnsupervisedBodyC::Apply(const SampleC<VectorC> &in,const SampleC<RealT> &weights) {
    RavlAssertMsg(0,"DesignFunctionUnsupervisedBodyC::Apply(const SampleC<VectorC> &,const SampleC<RealT> &), Abstract method called. \n");
    return FunctionC();
  }
  
  //: Create function from the given data.
  
  FunctionC DesignFunctionUnsupervisedBodyC::Apply(SampleStreamC<VectorC> &in) {
    RavlAssertMsg(0,"DesignFunctionUnsupervisedBodyC::Apply(const SampleStreamC<VectorC> &), Abstract method called. \n");
    return FunctionC();
  }
  
  //: Create function from the given data, and sample weights.
  
  FunctionC DesignFunctionUnsupervisedBodyC::Apply(SampleStream2C<VectorC,RealT> &in) {
    RavlAssertMsg(0,"DesignFunctionUnsupervisedBodyC::Apply(const SampleStreamC<VectorC,RealT> &), Abstract method called. \n");
    return FunctionC();
  }

  //: Create function from the given data.
  FunctionC DesignFunctionUnsupervisedBodyC::Apply(const SampleC<TVectorC<float> > &in) {
    SampleVectorC newIn(in);
    return Apply(newIn);
  }

  //: Create function from the given data, and sample weights.
  FunctionC DesignFunctionUnsupervisedBodyC::Apply(const SampleC<TVectorC<float> > &in,const SampleC<float> &weight) {
    SampleVectorC newIn(in);
    SampleRealC realWeights(weight);
    return Apply(newIn,realWeights);
  }

  //: Create function from the given data.
  // Note: Construction from a sample stream may not be implemented by all designers.

  FunctionC DesignFunctionUnsupervisedBodyC::Apply(SampleStreamC<TVectorC<float> > &in) {
    SampleStreamVectorC ssv(in);
    return Apply(ssv);
  }

  static Tuple2C<VectorC,RealT> float2realTup(const Tuple2C<TVectorC<float>,float> &in) {
    return Tuple2C<VectorC,RealT> (in.Data1(),in.Data2());
  }

  //: Create function from the given data, and sample weights.
  // Note: Construction from a sample stream may not be implemented by all designers.
  FunctionC DesignFunctionUnsupervisedBodyC::Apply(SampleStream2C<TVectorC<float>,float> &in) {
    SampleStream2C<VectorC,RealT> ssvw = SPort(in >> Process(float2realTup));
    return Apply(ssvw);
  }


  ////////////////////////////////////////////////////////////////
  
  //: Load from stream.
  
  DesignFunctionUnsupervisedC::DesignFunctionUnsupervisedC(std::istream &strm)
    : DesignerC(RAVL_VIRTUALCONSTRUCTOR(strm,DesignFunctionUnsupervisedBodyC))
  {}
  
  //: Load from binary stream.
  
  DesignFunctionUnsupervisedC::DesignFunctionUnsupervisedC(BinIStreamC &strm)
    : DesignerC(RAVL_VIRTUALCONSTRUCTOR(strm,DesignFunctionUnsupervisedBodyC))
  {}

  
}
