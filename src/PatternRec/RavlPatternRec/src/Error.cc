// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Performance/Error.cc"

#include "Ravl/PatternRec/Error.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/PatternRec/DataSet2Iter.hh"
#include "Ravl/PatternRec/FuncSubset.hh"
#include "Ravl/SArray1dIter2.hh"

namespace RavlN {

  //: Constructor.
  ErrorBodyC::ErrorBodyC()
  {}

  //: XML factory constructor
  ErrorBodyC::ErrorBodyC(const XMLFactoryContextC &factory)
  {}

  //: Load from stream.
  
  ErrorBodyC::ErrorBodyC(istream &strm)
    : RCBodyVC(strm)
  {}
  
  //: Load from binary stream.
  
  ErrorBodyC::ErrorBodyC(BinIStreamC &strm)
    : RCBodyVC(strm)
  {}

  //: Copy me.
  RavlN::RCBodyVC &ErrorBodyC::Copy() const
  { return *new ErrorBodyC(*this); }

  //: Writes object to stream, can be loaded using constructor
  
  bool ErrorBodyC::Save (std::ostream &out) const {
    if(!RCBodyVC::Save(out))
      return false;
    return true;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool ErrorBodyC::Save (BinOStreamC &out) const {
    if(!RCBodyVC::Save(out))
      return false;
    return true;
  }
  
  RealT ErrorBodyC::Error(const ClassifierC & classifier, const SampleVectorC & x, const SampleLabelC & labels) const {
    return Error(classifier, DataSetVectorLabelC(x, labels));
  }

  RealT ErrorBodyC::Error(const ClassifierC & classifier, const DataSetVectorLabelC & dset) const {
    UIntT correct = 0;
    UIntT wrong = 0;    
    for(DataSet2IterC<SampleVectorC, SampleLabelC>it(dset);it;it++) {
      UIntT label = classifier.Classify(it.Data1());
      if(label==it.Data2()) correct++;
      else wrong++;
    }
    RealT pmc = (RealT)wrong/(RealT)dset.Sample1().Size();
    return pmc;
  }

  
  SArray1dC<RealT> ErrorBodyC::ErrorByLabel(const ClassifierC & classifier, const DataSetVectorLabelC & dset) const {

    SArray1dC<RealT> errors(classifier.NumLabels());
    errors.Fill(0.0);
    for(DataSet2IterC<SampleVectorC, SampleLabelC>it(dset);it;it++) {
      UIntT label = classifier.Classify(it.Data1());
      if(label!=it.Data2()) {
        errors[it.Data2()]++;
      }
    }
    for(SArray1dIter2C<RealT, UIntT>it(errors, dset.Sample2().LabelSums());it;it++) {
      it.Data1() /= (RealT)it.Data2();
    }
    return errors;
  }
     //: Compute errors on the data set by label

  RealT ErrorBodyC::Error(const Classifier2C & classifier, const DataSetVectorLabelC & dset, UIntT label, RealT threshold) const {
    UIntT correct = 0;
    UIntT wrong = 0;    
    
    for(DataSet2IterC<SampleVectorC, SampleLabelC>it(dset);it;it++) {
      RealT score = classifier.Classify2(it.Data1());
      if(it.Data2() == label && score <= threshold) 
        correct++;
      else if(it.Data2() != label && score > threshold)
        correct++;
      else wrong++;
    }
    RealT pmc = (RealT)wrong/(RealT)dset.Sample1().Size();
    return pmc;
  }
  
  RealT ErrorBodyC::Error(const ClassifierC & classifier, const DataSetVectorLabelC & dset, const SArray1dC<IndexC> & features) const
  {
#if 0
    SizeT dim = dset.Sample1().First().Size();
    FuncSubsetC funcSubset(features, dim);
    UIntT correct = 0;
    UIntT wrong = 0;
    for (DataSet2IterC<SampleVectorC, SampleLabelC> it(dset); it; it++) {
      UIntT label = classifier.Classify(funcSubset(it.Data1()));
      if (label == it.Data2())
        correct++;
      else
        wrong++;
    }
    RealT pmc = (RealT) wrong / (RealT) dset.Sample1().Size();
    return pmc;
#else
    return Error(classifier, SampleVectorC(dset.Sample1(), features), dset.Sample2());
#endif
  }
  
  //: Load from stream.
  
  ErrorC::ErrorC(istream &strm) 
    : RCHandleVC<ErrorBodyC>(RAVL_VIRTUALCONSTRUCTOR(strm,ErrorBodyC))
  {}
    
  //: Load from binary stream.
  
  ErrorC::ErrorC(BinIStreamC &strm) 
    : RCHandleVC<ErrorBodyC>(RAVL_VIRTUALCONSTRUCTOR(strm,ErrorBodyC))    
  {}

  //: Setup virtual constructor.
  // As this is an abstract class its not really needed, but its usefull to
  // have an example of how it should work here.
  
  //RAVL_INITVIRTUALCONSTRUCTOR(ErrorBodyC);
  
}
