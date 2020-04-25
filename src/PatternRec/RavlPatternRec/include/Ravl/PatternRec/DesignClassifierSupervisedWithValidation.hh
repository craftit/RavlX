// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DesignClassifierSupervisedWithValidation_HEADER
#define RAVL_DesignClassifierSupervisedWithValidation_HEADER 1
///////////////////////////////////////////////////////////
//! userlevel=Normal
//! author="Kieron Messer"
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! docentry="Ravl.API.Pattern Recognition.Classifier.DesignClassifier"
//! file="Ravl/PatternRec/Classify/DesignClassifierSupervised.hh"
//! example=exKNearestNeighbour.cc

#include "Ravl/RCHandleV.hh"
#include "Ravl/Vector.hh"
#include "Ravl/PatternRec/Sample.hh"
#include "Ravl/PatternRec/Classifier.hh"
#include "Ravl/PatternRec/DesignClassifierSupervised.hh"
#include "Ravl/PatternRec/DataSetVectorLabel.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Abstract supervised classifer designer with validation
  
  class DesignClassifierSupervisedWithValidationBodyC : public DesignClassifierSupervisedBodyC
  {
  public:
    DesignClassifierSupervisedWithValidationBodyC()
    {
    }
    //: Default constructor.
    
    DesignClassifierSupervisedWithValidationBodyC(const XMLFactoryContextC &factory);
    //: Construct from XML factory

    DesignClassifierSupervisedWithValidationBodyC(std::istream &strm);
    //: Load from stream.
    
    DesignClassifierSupervisedWithValidationBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save(std::ostream &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual bool Save(BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual ClassifierC Apply(const DataSetVectorLabelC & trainingSet,
        const DataSetVectorLabelC & validationSet);
    //: Create a classifier using a training set and a validation set.  Not necessarily implemented by most classifiers.
    //!param: trainingSet - the data to use for training
    //!param: validationSet - the data to use for validation
    //!return: The trained classifier

    virtual ClassifierC Apply(const SampleC<VectorC> &in, const SampleC<UIntT> &out);
    //: Create a classifier.
    
    virtual ClassifierC Apply(const SampleC<VectorC> &in, const SampleC<UIntT> &out, const SampleC<RealT> &weight);
    //: Create a classifier with weights for the samples.

    virtual ClassifierC Apply(const SampleC<VectorC> &in, const SampleC<UIntT> &out, const SArray1dC<IndexC> &featureSet);
    //: Create a classifier using feature subset
    //!param: in         - sample set of feature vectors
    //!param: out        - sample set of labels
    //!param: featureSet - array of feature indexes to use from sample set when designing classifier
    
    virtual ClassifierC Apply(const SampleC<VectorC> &in,
        const SampleC<UIntT> &out,
        const SArray1dC<IndexC> &featureSet,
        const SampleC<RealT> &weight);
    //: Create a classifier using feature subset
    //!param: in         - sample set of feature vectors
    //!param: out        - sample set of labels
    //!param: featureSet - array of feature indexes to use from sample set when designing classifier
    //!param: weight     - relative weights attached to each feature vector
    
  };
  
  //! userlevel=Normal
  //: Abstract supervised classifer designer.
  
  class DesignClassifierSupervisedWithValidationC : public DesignClassifierSupervisedC
  {
  public:
    DesignClassifierSupervisedWithValidationC()
    {
    }
    //: Default constructor.
    // Creates an invalid handle.
    
    DesignClassifierSupervisedWithValidationC(const XMLFactoryContextC &factory) :
        DesignClassifierSupervisedC(*new DesignClassifierSupervisedWithValidationBodyC(factory))
    {
    }
    //: Construct from XML factory

    DesignClassifierSupervisedWithValidationC(std::istream &strm);
    //: Load from stream.
    
    DesignClassifierSupervisedWithValidationC(BinIStreamC &strm);
    //: Load from binary stream.

  protected:
    DesignClassifierSupervisedWithValidationC(DesignClassifierSupervisedWithValidationBodyC &bod) :
        DesignClassifierSupervisedC(bod)
    {
    }
    //: Body constructor.
    
    DesignClassifierSupervisedWithValidationC(DesignClassifierSupervisedWithValidationBodyC *bod) :
        DesignClassifierSupervisedC(bod)
    {
    }
    //: Body ptr constructor.
    
    DesignClassifierSupervisedWithValidationBodyC &Body()
    {
      return static_cast<DesignClassifierSupervisedWithValidationBodyC &>(DesignClassifierSupervisedC::Body());
    }
    //: Access body.
    
    const DesignClassifierSupervisedWithValidationBodyC &Body() const
    {
      return static_cast<const DesignClassifierSupervisedWithValidationBodyC &>(DesignClassifierSupervisedC::Body());
    }
    //: Access body.
    
  public:
    
    ClassifierC Apply(const DataSetVectorLabelC & trainingSet,
        const DataSetVectorLabelC & validationSet)
    {
      return Body().Apply(trainingSet, validationSet);
    }
    //: Create a classifier using a training set and a validation set.  Not necessarily implemented by most classifiers.
    //!param: trainingSet - the data to use for training
    //!param: validationSet - the data to use for validation
    //!return: The trained classifier

  };

  inline std::istream &operator>>(std::istream &strm, DesignClassifierSupervisedWithValidationC &obj)
  {
    obj = DesignClassifierSupervisedWithValidationC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline std::ostream &operator<<(std::ostream &out, const DesignClassifierSupervisedWithValidationC &obj)
  {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm, DesignClassifierSupervisedWithValidationC &obj)
  {
    obj = DesignClassifierSupervisedWithValidationC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out, const DesignClassifierSupervisedWithValidationC &obj)
  {
    obj.Save(out);
    return out;
  }
//: Save to a stream.
// Uses virtual constructor.

}

#endif
