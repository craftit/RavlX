// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/DataSet/DesignFunctionSupervised.cc"

#include "Ravl/PatternRec/DesignFunctionSupervised.hh"
#include "Ravl/PatternRec/SampleVector.hh"
#include "Ravl/PatternRec/SampleReal.hh"
#include "Ravl/PatternRec/SampleStreamVector.hh"
#include "Ravl/DP/Compose.hh"
#include "Ravl/DP/SPortAttach.hh"
#include "Ravl/XMLFactoryRegister.hh"


namespace RavlN {

  //: Load from XML factory
  
  DesignFunctionSupervisedBodyC::DesignFunctionSupervisedBodyC(const XMLFactoryContextC & factory)
    : DesignerBodyC(factory)
  {}

  //: Load from stream.
  
  DesignFunctionSupervisedBodyC::DesignFunctionSupervisedBodyC(std::istream &strm)
    : DesignerBodyC(strm)
  {}
  
  //: Load from binary stream.
  
  DesignFunctionSupervisedBodyC::DesignFunctionSupervisedBodyC(BinIStreamC &strm) 
    : DesignerBodyC(strm)
  {}
  
  //: Writes object to stream.
  
  bool DesignFunctionSupervisedBodyC::Save (std::ostream &out) const {
    return DesignerBodyC::Save(out);
  }
  
  //: Writes object to binary stream.
  
  bool DesignFunctionSupervisedBodyC::Save (BinOStreamC &out) const {
    return DesignerBodyC::Save(out);
  }
  
  //: Create function from the given data.
  
  FunctionC DesignFunctionSupervisedBodyC::Apply(const SampleC<VectorC> &in,const SampleC<VectorC> &out) {
    RavlAssertMsg(0,"DesignSupervisedFunctionBodyC::Apply(const SampleC<VectorC> &,const SampleC<VectorC> &), Abstract method called. \n");
    return FunctionC();
  }
  
  //: Create function from the given data.
  
  FunctionC DesignFunctionSupervisedBodyC::Apply(const SampleC<VectorC> &in,const SampleC<VectorC> &out,const SampleC<RealT> &weights) {
    RavlAssertMsg(0,"DesignFunctionSupervisedBodyC::Apply(const SampleC<VectorC> &,const SampleC<VectorC> &out,const SampleC<RealT> &), Abstract method called. \n");
    return FunctionC();
  }
  
  //: Create function from the given data.
  
  FunctionC DesignFunctionSupervisedBodyC::Apply(SampleStream2C<VectorC,VectorC > &in) {
    RavlAssertMsg(0,"DesignFunctionSupervisedBodyC::Apply(const SampleStream2C<VectorC,VectorC> &), Abstract method called. \n");
    return FunctionC();
    
  }
  
  //: Create function from the given data, and sample weights.
  
  FunctionC DesignFunctionSupervisedBodyC::Apply(SampleStream3C<VectorC,VectorC,RealT> &in) {
    RavlAssertMsg(0,"DesignFunctionSupervisedBodyC::Apply(const SampleStream2C<VectorC,VectorC,RealT> &), Abstract method called. \n");
    return FunctionC();
  }
  
  FunctionC DesignFunctionSupervisedBodyC::Apply(const SampleC<TVectorC<float> > &in,const SampleC<TVectorC<float> > &out) {
    SampleVectorC newIn(in);
    SampleVectorC newOut(out);
    return Apply(newIn,newOut);
  }
  //: Create function from the given data.

  FunctionC DesignFunctionSupervisedBodyC::Apply(const SampleC<TVectorC<float> > &in,const SampleC<TVectorC<float> > &out,const SampleC<float> &weight) {
    SampleVectorC newIn(in);
    SampleVectorC newOut(out);
    SampleRealC realWeights(weight);
    return Apply(newIn,newOut,realWeights);
  }
  //: Create function from the given data, and sample weights.

  static Tuple2C<VectorC,VectorC> float22realTup(const Tuple2C<TVectorC<float>,TVectorC<float> > &in) {
    return Tuple2C<VectorC,VectorC> (in.Data1(),in.Data2());
  }

  FunctionC DesignFunctionSupervisedBodyC::Apply(SampleStream2C<TVectorC<float>,TVectorC<float> > &in) {
    SampleStream2C<VectorC,VectorC> ssv = SPort(in >> Process(float22realTup));
    return Apply(ssv);
  }

  static Tuple3C<VectorC,VectorC,RealT> float2weight2realTup(const Tuple3C<TVectorC<float>,TVectorC<float>,float> &in) {
    return Tuple3C<VectorC,VectorC,RealT> (in.Data1(),in.Data2(),in.Data3());
  }

  //: Create function from the given data.
  // Note: Construction from a sample stream may not be implemented by all designers.

  FunctionC DesignFunctionSupervisedBodyC::Apply(SampleStream3C<TVectorC<float>,TVectorC<float>,float> &in) {
    SampleStream3C<VectorC,VectorC,RealT> ssvw = SPort(in >> Process(float2weight2realTup));
    return Apply(ssvw);
  }
  //: Create function from the given data, and sample weights.
  // Note: Construction from a sample stream may not be implemented by all designers.

  // Reset designer to intial state
  bool DesignFunctionSupervisedBodyC::Reset() {
    RavlAssertMsg(0,"DesignFunctionSupervisedBodyC::Reset(), Abstract method called. \n");
    return false;
  }

  
  ////////////////////////////////////////////////////////////////////
  
  //: Load from stream.
  
  DesignFunctionSupervisedC::DesignFunctionSupervisedC(std::istream &strm)
    : DesignerC(RAVL_VIRTUALCONSTRUCTOR(strm,DesignFunctionSupervisedBodyC))
  {}
  
  //: Load from binary stream.
  
  DesignFunctionSupervisedC::DesignFunctionSupervisedC(BinIStreamC &strm)
    : DesignerC(RAVL_VIRTUALCONSTRUCTOR(strm,DesignFunctionSupervisedBodyC))
  {}


  RavlN::XMLFactoryRegisterHandleC<DesignFunctionSupervisedC> g_registerXMLFactoryDesignFunctionSupervised("RavlN::DesignFunctionSupervisedC");

  
}
