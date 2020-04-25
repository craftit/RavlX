// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_CLASSIFY_HEADER
#define RAVL_CLASSIFY_HEADER 1
///////////////////////////////////////////////////////////
//! lib=RavlPatternRec
//! docentry="Ravl.API.Pattern Recognition.Classifier"
//! author="Charles Galambos"
//! file="Ravl/PatternRec/Cluster/Classifier.hh"

#include "Ravl/PatternRec/Function.hh"
#include "Ravl/Vector.hh"
#include "Ravl/PatternRec/Sample.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Generic classifier body - Used as a base class for specific classifiers. 
  
  class ClassifierBodyC
    : public FunctionBodyC
  {
  public:
    ClassifierBodyC(UIntT nmaxLabels = 0);
    //: Constructor.
    
    ClassifierBodyC(std::istream &strm);
    //: Load from stream.
    
    ClassifierBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (std::ostream &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual UIntT Classify(const VectorC &data) const;
    //: Classifier vector 'data' return the most likely label.
    
    using FunctionBodyC::Apply;

    virtual VectorC Apply(const VectorC &data) const;
    //: Estimate the confidence for each label.
    // The meaning of the confidence assigned to each label depends
    // on the classifier used. The higher the confidence the more likely
    // it is the label is correct.
    
    virtual SampleC<UIntT> Classify(const SampleC<VectorC> &data) const;
    //: Classify each vector in the set.

    virtual UIntT Classify(const VectorC &data,const SArray1dC<IndexC> &featureSet) const;
    //: Classifier vector 'data' return the most likely label.
    
    virtual VectorC Apply(const VectorC &data,const SArray1dC<IndexC> &featureSet) const;
    //: Estimate the confidence for each label.
    // The meaning of the confidence assigned to each label depends
    // on the classifier used. The higher the confidence the more likely
    // it is the label is correct.
    
    virtual RealT LabelProbability(const VectorC & data, UIntT label) const;
    //: Compute the 'posterior' probability of a given class

    virtual RealT LabelProbability(const VectorC & data, const SArray1dC<IndexC> &featureSet, UIntT label) const;
    //: Compute the 'posterior' probability of a given class

    UIntT NoLabels() const
    { return outputSize; }
    //: Returns the maximum number of output labels.
    // Identical to OutputSize().
    //!deprecated: Use NumLabels()
    
    UIntT NoLabels(UIntT labels)
    { return OutputSize(labels); }
    //: Returns the maximum number of output labels.
    // Identical to OutputSize().
    //!deprecated: Use NumLabels(UIntT numLabels)
    
    UIntT NumLabels() const
    { return outputSize; }
    //: Returns the maximum number of output labels.
    // Identical to OutputSize().
    
    UIntT NumLabels(UIntT labels)
    { return OutputSize(labels); }
    //: Returns the maximum number of output labels.
    // Identical to OutputSize().
    
  protected:
  };

  //! userlevel=Normal
  //: Generic classifier.
  
  class ClassifierC 
    : public FunctionC
  {
  public:
    ClassifierC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    ClassifierC(std::istream &strm);
    //: Load from stream.
    
    ClassifierC(BinIStreamC &strm);
    //: Load from binary stream.
    
    ClassifierC(const FunctionC &func)
      : FunctionC(dynamic_cast<const ClassifierBodyC *>(BodyPtr(func)))
    {}
    //: Create from base class.
    // Creates an invalid handle if 'func' is not a classifier.
    
  protected:
    ClassifierC(ClassifierBodyC &bod)
      : FunctionC(bod)
    {}
    //: Body constructor.

    ClassifierC(const ClassifierBodyC *bod)
      : FunctionC(bod)
    {}
    //: Body ptr constructor.
    
    ClassifierBodyC &Body()
    { return static_cast<ClassifierBodyC &>(FunctionC::Body()); }
    //: Access body.
    
    const ClassifierBodyC &Body() const
    { return static_cast<const ClassifierBodyC &>(FunctionC::Body()); }
    //: Access body.
    
  public:
    UIntT Classify(const VectorC &data) const
    { return Body().Classify(data); }
    //: Classifier vector 'data' return the most likely label.

    SampleC<UIntT> Classify(const SampleC<VectorC> &data) const
    { return Body().Classify(data); }
    //: Classify each vector in the set.

    VectorC Confidence(const VectorC &data) const
    { return Body().Apply(data); }
    //: Estimate the confidence for each label.
    // The meaning of the confidence assigned to each label depends
    // on the classifier used. The higher the confidence the more likely
    // it is the label is correct. <p>
    // The result is identical to that of the apply method.
    
    UIntT Classify(const VectorC &data,const SArray1dC<IndexC> &featureSet) const
    { return Body().Classify(data,featureSet); }
    //: Classifier vector 'data' return the most likely label.
    
    VectorC Confidence(const VectorC &data,const SArray1dC<IndexC> &featureSet) const
    { return Body().Apply(data,featureSet); }
    //: Estimate the confidence for each label.
    // The meaning of the confidence assigned to each label depends
    // on the classifier used. The higher the confidence the more likely
    // it is the label is correct. <p>
    // The result is identical to that of the apply method.
    
    RealT LabelProbability(const VectorC & data, UIntT label) const {
      return Body().LabelProbability(data, label);
    }
    //: Compute the 'posterior' probability of a given class

    RealT LabelProbability(const VectorC & data, const SArray1dC<IndexC> &featureSet, UIntT label) const {
      return Body().LabelProbability(data, featureSet, label);
    }
    //: Compute the 'posterior' probability of a given class using only features specified.

    UIntT NoLabels() const
    { return Body().NoLabels(); }
    //: Returns the maximum number of output labels.
    //!deprecated: Use NumLabels(UIntT numLabels)
    
    UIntT NoLabels(UIntT labels)
    { return Body().NoLabels(labels); }
    //: Set the number of labels
    //!deprecated: Use NumLabels(UIntT numLabels)
    
    UIntT NumLabels() const
    { return Body().NumLabels(); }
    //: Returns the maximum number of output labels.
    // Identical to OutputSize().
    
    UIntT NumLabels(UIntT labels)
    { return Body().NumLabels(labels); }
    //: Returns the maximum number of output labels.
    // Identical to OutputSize().
    
  };
  
  inline std::istream &operator>>(std::istream &strm,ClassifierC &obj) {
    obj = ClassifierC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline std::ostream &operator<<(std::ostream &out,const ClassifierC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,ClassifierC &obj) {
    obj = ClassifierC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const ClassifierC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
}

#endif
