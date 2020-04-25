// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlPatternRec
//! author="Charles Galambos"
//! file="Ravl/PatternRec/Classify/DesignClassifierSupervised.cc"

#include "Ravl/PatternRec/DesignClassifierSupervisedWithValidation.hh"
#include "Ravl/PatternRec/SampleIter.hh"
#include "Ravl/config.h"
#include "Ravl/PatternRec/SampleVector.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include "Ravl/PatternRec/DataSetVectorLabel.hh"
#include "Ravl/PatternRec/FuncMeanProjection.hh"

namespace RavlN {
  
  //: Load from XML factory

  DesignClassifierSupervisedWithValidationBodyC::DesignClassifierSupervisedWithValidationBodyC(const XMLFactoryContextC & factory) :
      DesignClassifierSupervisedBodyC(factory)
  {
  }

  //: Load from stream.
  
  DesignClassifierSupervisedWithValidationBodyC::DesignClassifierSupervisedWithValidationBodyC(std::istream &strm) :
      DesignClassifierSupervisedBodyC(strm)
  {
  }
  
  //: Load from binary stream.
  
  DesignClassifierSupervisedWithValidationBodyC::DesignClassifierSupervisedWithValidationBodyC(BinIStreamC &strm) :
      DesignClassifierSupervisedBodyC(strm)
  {
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DesignClassifierSupervisedWithValidationBodyC::Save(std::ostream &out) const
  {
    return DesignClassifierSupervisedBodyC::Save(out);
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DesignClassifierSupervisedWithValidationBodyC::Save(BinOStreamC &out) const
  {
    return DesignClassifierSupervisedBodyC::Save(out);
  }
  
  ClassifierC DesignClassifierSupervisedWithValidationBodyC::Apply(const DataSetVectorLabelC & trainingSet,
      const DataSetVectorLabelC & validationSet)
  {
    RavlAssertMsg(0, "Base class function called.");
    return ClassifierC();
  }

  //: Create a clasifier.
  
  ClassifierC DesignClassifierSupervisedWithValidationBodyC::Apply(const SampleC<VectorC> &in, const SampleC<UIntT> &out)
  {
    DataSetVectorLabelC trainingDataSet(in, out);
    DataSetVectorLabelC validationDataSet = trainingDataSet.ExtractSample(0.5);
    return Apply(trainingDataSet, validationDataSet);
  }
  
  //: Create a clasifier with weights for the samples.
  
  ClassifierC DesignClassifierSupervisedWithValidationBodyC::Apply(const SampleC<VectorC> &in,
      const SampleC<UIntT> &out,
      const SampleC<RealT> &weight)
  {
    RavlAssertMsg(0, "DesignClassifierSupervisedWithValidationBodyC::Apply(), Abstract method called. ");
    return ClassifierC();
  }

  ClassifierC DesignClassifierSupervisedWithValidationBodyC::Apply(const SampleC<VectorC> &in,
      const SampleC<UIntT> &out,
      const SArray1dC<IndexC> &featureSet)
  {
    DataSetVectorLabelC trainingDataSet(SampleVectorC(in, featureSet), out);
    DataSetVectorLabelC validationDataSet = trainingDataSet.ExtractSample(0.5);
    return Apply(trainingDataSet, validationDataSet);
  }
  
  ClassifierC DesignClassifierSupervisedWithValidationBodyC::Apply(const SampleC<VectorC> &in,
      const SampleC<UIntT> &out,
      const SArray1dC<IndexC> &featureSet,
      const SampleC<RealT> &weight)
  {
    RavlAssertMsg(0, "Not supported");
    return ClassifierC();
  }
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(DesignClassifierSupervisedWithValidationBodyC,
      DesignClassifierSupervisedWithValidationC,
      DesignClassifierSupervisedC);
  
  RavlN::XMLFactoryRegisterHandleConvertC<DesignClassifierSupervisedWithValidationC, DesignClassifierSupervisedC> g_registerXMLFactoryDesignClassifierSupervisedWithValidation("RavlN::DesignClassifierSupervisedWithValidationC");
  
  void linkDesignClassifierSupervisedWithValidation()
  {
  }

}
