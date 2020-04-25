// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlPatternRec
//! author="Charles Galambos"
//! file="Ravl/PatternRec/Classify/DesignClassifierSupervised.cc"

#include "Ravl/PatternRec/DesignClassifierSupervised.hh"
#include "Ravl/PatternRec/SampleIter.hh"
#include "Ravl/config.h"
#include "Ravl/PatternRec/SampleVector.hh"
#include "Ravl/XMLFactoryRegister.hh"


namespace RavlN {
  

  //: Load from XML factory

  DesignClassifierSupervisedBodyC::DesignClassifierSupervisedBodyC(const XMLFactoryContextC & factory)
    : DesignFunctionSupervisedBodyC(factory)
  {}

  //: Load from stream.
  
  DesignClassifierSupervisedBodyC::DesignClassifierSupervisedBodyC(std::istream &strm)
    : DesignFunctionSupervisedBodyC(strm)
  {}
  
  //: Load from binary stream.
  
  DesignClassifierSupervisedBodyC::DesignClassifierSupervisedBodyC(BinIStreamC &strm)
    : DesignFunctionSupervisedBodyC(strm)
  {}
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DesignClassifierSupervisedBodyC::Save (std::ostream &out) const 
  { return DesignFunctionSupervisedBodyC::Save(out); }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DesignClassifierSupervisedBodyC::Save (BinOStreamC &out) const
  { return DesignFunctionSupervisedBodyC::Save(out); }
  
  //: Create a clasifier.
  
  ClassifierC DesignClassifierSupervisedBodyC::Apply(const SampleC<VectorC> &in,const SampleC<UIntT> &out) {
    RavlAssertMsg(0,"DesignClassifierSupervisedBodyC::Apply(), Abstract method called. ");
    return ClassifierC();
  }
  
  //: Create a clasifier with weights for the samples.
  
  ClassifierC DesignClassifierSupervisedBodyC::Apply(const SampleC<VectorC> &in,const SampleC<UIntT> &out,const SampleC<RealT> &weight) {
    RavlAssertMsg(0,"DesignClassifierSupervisedBodyC::Apply(), Abstract method called. ");
    return ClassifierC();
  }

  ClassifierC DesignClassifierSupervisedBodyC::Apply(const SampleC<VectorC> &in,const SampleC<UIntT> &out,const SArray1dC<IndexC> &featureSet) {
    return Apply(SampleVectorC(in,featureSet),out);
  }
  
  ClassifierC DesignClassifierSupervisedBodyC::Apply(const SampleC<VectorC> &in,const SampleC<UIntT> &out,const SArray1dC<IndexC> &featureSet,const SampleC<RealT> &weight) {
    return Apply(SampleVectorC(in,featureSet),out,weight);
  }
  
  //: Load from stream.
  
  DesignClassifierSupervisedC::DesignClassifierSupervisedC(std::istream &strm)
    : DesignFunctionSupervisedC(RAVL_VIRTUALCONSTRUCTOR(strm,DesignClassifierSupervisedBodyC))    
  {}
  
  //: Load from binary stream.
  
  DesignClassifierSupervisedC::DesignClassifierSupervisedC(BinIStreamC &strm)
    : DesignFunctionSupervisedC(RAVL_VIRTUALCONSTRUCTOR(strm,DesignClassifierSupervisedBodyC))
  {}


  RavlN::XMLFactoryRegisterHandleConvertC<DesignClassifierSupervisedC, DesignFunctionSupervisedC> g_registerXMLFactoryDesignClassifierSupervised("RavlN::DesignClassifierSupervisedC");

  void linkDesignClassifierSupervised()
  {}

  
}

#if RAVL_COMPILER_MIPSPRO
#include "Ravl/SArray1dIter3.hh"
#include "Ravl/SArray1dIter4.hh"
#include "Ravl/MeanCovariance.hh"
#include "Ravl/MatrixRS.hh"
#include "Ravl/Vector.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/RCHandleV.hh"

#pragma instantiate RavlN::SArray1dIter3C<RavlN::MeanCovarianceC,RavlN::VectorC,RavlN::MatrixRSC>
#pragma instantiate std::istream & RavlN::operator>>(std::istream&,RavlN::SArray1dC<RavlN::MeanCovarianceC>&)
#pragma instantiate RavlN::BinIStreamC & RavlN::operator>>(RavlN::BinIStreamC&,RavlN::SArray1dC<RavlN::MeanCovarianceC>&)
#pragma instantiate std::ostream & RavlN::operator<<(std::ostream&,const RavlN::SArray1dC<RavlN::MeanCovarianceC>&)
#pragma instantiate std::ostream & RavlN::operator<<(std::ostream&,const RavlN::SArray1dC<double>&) 
#pragma instantiate RavlN::BinOStreamC & RavlN::operator<<(RavlN::BinOStreamC&,const RavlN::SArray1dC<RavlN::MeanCovarianceC>&)
#pragma instantiate RavlN::SArray1dIter4C<RavlN::MeanCovarianceC,double,RavlN::MatrixRSC,double>
#pragma instantiate RavlN::DesignClassifierSupervisedBodyC* ::RavlN::VCLoad(std::istream&, RavlN::DesignClassifierSupervisedBodyC*)
#pragma instantiate RavlN::DesignClassifierSupervisedBodyC* ::RavlN::VCLoad(RavlN::BinIStreamC & , RavlN::DesignClassifierSupervisedBodyC*)

#endif 
