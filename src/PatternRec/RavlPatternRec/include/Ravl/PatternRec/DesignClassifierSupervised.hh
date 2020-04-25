// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DESIGNCLASSIFYVECTORSUPERVISED_HEADER
#define RAVL_DESIGNCLASSIFYVECTORSUPERVISED_HEADER 1
///////////////////////////////////////////////////////////
//! userlevel=Normal
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! docentry="Ravl.API.Pattern Recognition.Classifier.DesignClassifier"
//! file="Ravl/PatternRec/Classify/DesignClassifierSupervised.hh"
//! example=exKNearestNeighbour.cc

#include "Ravl/RCHandleV.hh"
#include "Ravl/Vector.hh"
#include "Ravl/PatternRec/Sample.hh"
#include "Ravl/PatternRec/DataSetVectorLabel.hh"
#include "Ravl/PatternRec/Classifier.hh"
#include "Ravl/PatternRec/DesignFunctionSupervised.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Abstract supervised classifier designer.
  
  class DesignClassifierSupervisedBodyC
    : public DesignFunctionSupervisedBodyC
  {
  public:
    DesignClassifierSupervisedBodyC()
    {}
    //: Default constructor.
    
    DesignClassifierSupervisedBodyC(const XMLFactoryContextC &factory);
    //: Construct from XML factory

    DesignClassifierSupervisedBodyC(std::istream &strm);
    //: Load from stream.
    
    DesignClassifierSupervisedBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (std::ostream &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    using DesignFunctionSupervisedBodyC::Apply;

    virtual ClassifierC Apply(const SampleC<VectorC> &in,const SampleC<UIntT> &out);
    //: Create a classifier.
    
    virtual ClassifierC Apply(const SampleC<VectorC> &in,const SampleC<UIntT> &out,const SampleC<RealT> &weight);
    //: Create a classifier with weights for the samples.

    virtual ClassifierC Apply(const SampleC<VectorC> &in,const SampleC<UIntT> &out,const SArray1dC<IndexC> &featureSet);
    //: Create a classifier using feature subset
    //!param: in         - sample set of feature vectors
    //!param: out        - sample set of labels
    //!param: featureSet - array of feature indexes to use from sample set when designing classifier
    
    virtual ClassifierC Apply(const SampleC<VectorC> &in,const SampleC<UIntT> &out,const SArray1dC<IndexC> &featureSet,const SampleC<RealT> &weight);
    //: Create a classifier using feature subset
    //!param: in         - sample set of feature vectors
    //!param: out        - sample set of labels
    //!param: featureSet - array of feature indexes to use from sample set when designing classifier
    //!param: weight     - relative weights attached to each feature vector
    
    ClassifierC Apply(const DataSetVectorLabelC & dset) {
      return Apply(dset.Sample1(), dset.Sample2());
    }
    //: Create a classifier from a data set

  };
  
  //! userlevel=Normal
  //: Abstract supervised classifier designer.
  
  class DesignClassifierSupervisedC
    : public DesignFunctionSupervisedC
  {
  public:
    DesignClassifierSupervisedC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    DesignClassifierSupervisedC(const XMLFactoryContextC &factory)
      :  DesignFunctionSupervisedC(*new DesignClassifierSupervisedBodyC(factory))
    {}
    //: Construct from XML factory

    DesignClassifierSupervisedC(std::istream &strm);
    //: Load from stream.
    
    DesignClassifierSupervisedC(BinIStreamC &strm);
    //: Load from binary stream.

  protected:
    DesignClassifierSupervisedC(DesignClassifierSupervisedBodyC &bod)
      : DesignFunctionSupervisedC(bod)
    {}
    //: Body constructor.
    
    DesignClassifierSupervisedC(DesignClassifierSupervisedBodyC *bod)
      : DesignFunctionSupervisedC(bod)
    {}
    //: Body ptr constructor.
    
    DesignClassifierSupervisedBodyC &Body()
    { return static_cast<DesignClassifierSupervisedBodyC &>(DesignFunctionSupervisedC::Body()); }
    //: Access body.
    
    const DesignClassifierSupervisedBodyC &Body() const
    { return static_cast<const DesignClassifierSupervisedBodyC &>(DesignFunctionSupervisedC::Body()); }
    //: Access body.
    
  public:
    DesignClassifierSupervisedC Copy() const
    { return DesignClassifierSupervisedC(dynamic_cast<DesignClassifierSupervisedBodyC &>(Body().Copy())); }
     //: Make copy of body.

    ClassifierC Apply(const SampleC<VectorC> &in,const SampleC<UIntT> &out)
    { return Body().Apply(in,out); }
    //: Create a classifier.
    
    ClassifierC Apply(const SampleC<VectorC> &in,const SampleC<UIntT> &out,const SampleC<RealT> &weight) 
    { return Body().Apply(in,out,weight); }
    //: Create a classifier with weights for the samples.

    ClassifierC Apply(const SampleC<VectorC> &in,const SampleC<UIntT> &out,const SArray1dC<IndexC> &featureSet)
    { return Body().Apply(in,out,featureSet); }
    //: Create a classifier using feature subset
    //!param: in         - sample set of feature vectors
    //!param: out        - sample set of labels
    //!param: featureSet - array of feature indexes to use from sample set when designing classifier
    
    ClassifierC Apply(const SampleC<VectorC> &in,const SampleC<UIntT> &out,const SArray1dC<IndexC> &featureSet,const SampleC<RealT> &weight)
    { return Body().Apply(in,out,featureSet,weight); }
    //: Create a classifier using feature subset
    //!param: in         - sample set of feature vectors
    //!param: out        - sample set of labels
    //!param: featureSet - array of feature indexes to use from sample set when designing classifier
    //!param: weight     - weight associated with each feature vector
    
    ClassifierC Apply(const DataSetVectorLabelC & dset) {
       return Body().Apply(dset);
     }
     //: Create a classifier from a data set

  };

  inline std::istream &operator>>(std::istream &strm,DesignClassifierSupervisedC &obj) {
    obj = DesignClassifierSupervisedC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline std::ostream &operator<<(std::ostream &out,const DesignClassifierSupervisedC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,DesignClassifierSupervisedC &obj) {
    obj = DesignClassifierSupervisedC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const DesignClassifierSupervisedC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
}


#endif
