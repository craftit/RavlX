// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Classify/ClassifierGaussianMixture.cc"

#include "Ravl/PatternRec/ClassifierGaussianMixture.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/SArray1dIter2.hh"

namespace RavlN {
  
  //: Create classifier from function.
  
  ClassifierGaussianMixtureBodyC::ClassifierGaussianMixtureBodyC(const SArray1dC<GaussianMixtureC> &nmodels)
    : ClassifierBodyC(nmodels.Size()),
      models(nmodels)
  {}
  
  //: Load from stream.
  
  ClassifierGaussianMixtureBodyC::ClassifierGaussianMixtureBodyC(std::istream &strm)
    : ClassifierBodyC(strm)
  { 
    IntT version;
    strm >> version;
    if(version != 0)
      throw ExceptionOutOfRangeC("ClassifierGaussianMixtureBodyC::ClassifierGaussianMixtureBodyC(std::istream &), Unrecognised version number in stream. ");
    strm >> models;
  }
  
  //: Load from binary stream.
  
  ClassifierGaussianMixtureBodyC::ClassifierGaussianMixtureBodyC(BinIStreamC &strm)
    : ClassifierBodyC(strm)
  {
    IntT version;
    strm >> version;
    if(version != 0)
      throw ExceptionOutOfRangeC("ClassifierGaussianMixtureBodyC::ClassifierGaussianMixtureBodyC(BinIStreamC &), Unrecognised version number in stream. ");
    strm >> models; 
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool ClassifierGaussianMixtureBodyC::Save(std::ostream &out) const {
    if(!ClassifierBodyC::Save(out))
      return false;
    IntT version = 0;
    out << version << ' ' << models;
    return true;
  }
  

  //: Writes object to stream, can be loaded using constructor
  
  bool ClassifierGaussianMixtureBodyC::Save(BinOStreamC &out) const {
    if(!ClassifierBodyC::Save(out))
      return false;
    IntT version = 0;
    out << version << models;
    return true;    
  }
  
  //: Classifier vector 'data' return the most likely label.
  
  UIntT ClassifierGaussianMixtureBodyC::Classify(const VectorC &data) const {
    UIntT ret = 0;
    SArray1dIterC<GaussianMixtureC > lit(models);
    if(!lit)
      return ret;
    RealT max;
    if(lit->IsValid())
      max = lit->DensityValue(data);
    else max = 0;
    for(lit++;lit;lit++) {
      if(!lit->IsValid())
        continue;
      RealT v = lit->DensityValue(data);
      if(v > max) {
        max = v;
        ret = lit.Index().V();
      }
    }
    return ret;
  }
  
  //: Estimate the confidence for each label.
  // The meaning of the confidence assigned to each label depends
  // on the classifier used. The higher the confidence the more likely
  // it is the label is correct.
  
  VectorC ClassifierGaussianMixtureBodyC::Confidence(const VectorC &data) const {
    VectorC ret(models.Size());
    for(SArray1dIter2C<RealT,GaussianMixtureC > lit(ret,models);lit;lit++) {
      if(lit.Data2().IsValid())
	lit.Data1() = lit.Data2().DensityValue(data);
      else
	lit.Data1() = 0;
    }
    return ret;
  }
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(ClassifierGaussianMixtureBodyC,ClassifierGaussianMixtureC,ClassifierC);
  
  
}
