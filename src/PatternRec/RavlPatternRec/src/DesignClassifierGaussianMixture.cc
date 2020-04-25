// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Classify/DesignClassifierGaussianMixture.cc"

#include "Ravl/PatternRec/DesignClassifierGaussianMixture.hh"
#include "Ravl/PatternRec/ClassifierGaussianMixture.hh"
#include "Ravl/PatternRec/DesignGaussianMixture.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/PatternRec/DataSetVectorLabel.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/XMLFactoryRegister.hh"

namespace RavlN {
  
  //: Constructor.
  
  DesignClassifierGaussianMixtureBodyC::DesignClassifierGaussianMixtureBodyC(UIntT nmixtures)
    : mixtures(nmixtures)
  {}
  
  DesignClassifierGaussianMixtureBodyC::DesignClassifierGaussianMixtureBodyC(const XMLFactoryContextC & factory)
    : DesignClassifierSupervisedBodyC(factory),
      mixtures(factory.AttributeUInt("number_of_mixtures", 1))
  {
  }

  //: Load from stream.
  
  DesignClassifierGaussianMixtureBodyC::DesignClassifierGaussianMixtureBodyC(std::istream &strm)
    : DesignClassifierSupervisedBodyC(strm),
      mixtures(0)
  {
    int version;
    strm >> version;
    if(version != 0)
      throw ExceptionOutOfRangeC("DesignClassifierGaussianMixtureBodyC::DesignClassifierGaussianMixtureBodyC(std::istream &), Unrecognised version number in stream. ");
    strm >> mixtures;
  }
  
  //: Load from binary stream.
  
  DesignClassifierGaussianMixtureBodyC::DesignClassifierGaussianMixtureBodyC(BinIStreamC &strm)
    : DesignClassifierSupervisedBodyC(strm),
      mixtures(0)
  {
    int version;
    strm >> version;
    if(version != 0)
      throw ExceptionOutOfRangeC("DesignClassifierGaussianMixtureBodyC::DesignClassifierGaussianMixtureBodyC(BinIStreamC &), Unrecognised version number in stream. ");
    strm >> mixtures;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DesignClassifierGaussianMixtureBodyC::Save (std::ostream &out) const {
    if(!DesignClassifierSupervisedBodyC::Save(out))
      return false;
    int version = 0;
    out << ' ' << version << ' ' << mixtures;
    return true;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DesignClassifierGaussianMixtureBodyC::Save (BinOStreamC &out) const {
    if(!DesignClassifierSupervisedBodyC::Save(out))
      return false;
    int version = 0;
    out << version << mixtures;
    return true;
  }
  
  //: Create a classifier.
  
  ClassifierC DesignClassifierGaussianMixtureBodyC::Apply(const SampleC<VectorC> &in,const SampleC<UIntT> &out) {
    RavlAssertMsg(in.Size() == out.Size(),"DesignClassifierGaussianMixtureBodyC::Apply(), Sample of vector and labels should be the same size.");
    DataSetVectorLabelC vl(in,out);
    
    SArray1dC<SampleVectorC> sva = vl.SeperateLabels();
    SArray1dC<GaussianMixtureC> gma(sva.Size());
    DesignGaussianMixtureC designMixture(mixtures,true);
    for(SArray1dIter2C<GaussianMixtureC,SampleVectorC> it(gma,sva);it;it++) {
      if(it.Data2().Size() > 0)
	it.Data1() = designMixture.Apply(it.Data2());
    }
    return ClassifierGaussianMixtureC(gma);
  }
  
  //: Create a classifier with weights for the samples.
  
  ClassifierC DesignClassifierGaussianMixtureBodyC::Apply(const SampleC<VectorC> &in,
							  const SampleC<UIntT> &out,
							  const SampleC<RealT> &weight) {
    RavlAssertMsg(0,"DesignClassifierGaussianMixtureBodyC::Apply(in,out,weight), Not implemented. Send a feature request! ");
    return ClassifierC();
  }
 
  //////////////////////////////////////////////////////////
  static RavlN::XMLFactoryRegisterHandleConvertC<DesignClassifierGaussianMixtureC, DesignClassifierSupervisedC> g_registerXMLFactoryDesignClassifierGaussianMixture("RavlN::DesignClassifierGaussianMixtureC");

  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(DesignClassifierGaussianMixtureBodyC,DesignClassifierGaussianMixtureC,DesignClassifierSupervisedC);

  void linkDesignClassifierGaussianMixture()
  {}
  
}
