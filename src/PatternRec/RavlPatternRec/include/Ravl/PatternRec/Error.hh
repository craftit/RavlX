// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_ERROR_HEADER
#define RAVL_ERROR_HEADER 1
///////////////////////////////////////////////////////////
//! lib=RavlPatternRec
//! docentry="Ravl.API.Pattern Recognition.Performance Evaluation"
//! author="Kieron Messer"
//! file="Ravl/PatternRec/Performance/Error.hh"

#include "Ravl/PatternRec/Classifier.hh"
#include "Ravl/PatternRec/Classifier2.hh"
#include "Ravl/PatternRec/OneClass.hh"
#include "Ravl/PatternRec/SampleVector.hh"
#include "Ravl/PatternRec/SampleLabel.hh"
#include "Ravl/PatternRec/DataSetVectorLabel.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Simple error count for a classifier and dataset
  
  class ErrorBodyC
    : public RCBodyVC
  {
  public:
    ErrorBodyC();
    //: Constructor.

    ErrorBodyC(const XMLFactoryContextC &factory);
    //: XML factory constructor

    ErrorBodyC(std::istream &strm);
    //: Load from stream.
    
    ErrorBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual RavlN::RCBodyVC &Copy() const;
    //: Copy me.

    virtual bool Save (std::ostream &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual RealT Error(const ClassifierC & classifier, const DataSetVectorLabelC & dset) const;
    //: Compute the error on the labelled dataset

    virtual SArray1dC<RealT> ErrorByLabel(const ClassifierC & classifier, const DataSetVectorLabelC & dset) const;
    //: Compute errors on the data set by label

    virtual RealT Error(const ClassifierC & classifier, const SampleVectorC & x, const SampleLabelC & labels) const;
    //: Compute the error on the labelled dataset

    virtual RealT Error(const ClassifierC & classifier, const DataSetVectorLabelC & dset, const SArray1dC<IndexC> & features) const;
    //: Compute the error on the labelled dataset

    virtual RealT Error(const Classifier2C & classifier, const DataSetVectorLabelC & dset, UIntT label, RealT threshold) const;
    //: Compute the error on the labelled dataset and a 2 class classifier at a specified threshold level
        
  };

  //! userlevel=Normal
  //: Simple error count for a classifier and dataset
  
  class ErrorC 
    : public RCHandleVC<ErrorBodyC>
  {
  public:
    //! Construct from a base handle
    ErrorC(const RCHandleVC<ErrorBodyC> &other)
     : RCHandleVC<ErrorBodyC>(other)
    {}

    ErrorC()
      : RCHandleVC<ErrorBodyC>(*new ErrorBodyC())
    {}
    //: Default constructor.
    
    ErrorC(const XMLFactoryContextC &factory);
    //: XML factory constructor

    ErrorC(istream &strm);
    //: Load from stream.
    
    ErrorC(BinIStreamC &strm);
    //: Load from binary stream.
    
  protected:
    ErrorC(ErrorBodyC &bod)
      : RCHandleVC<ErrorBodyC>(bod)
    {}
    //: Body constructor.

    ErrorC(ErrorBodyC *bod)
      : RCHandleVC<ErrorBodyC>(bod)
    {}
    //: Body ptr constructor.
    
    ErrorBodyC &Body()
    { return RCHandleC<ErrorBodyC>::Body(); }
    //: Access body.
    
    const ErrorBodyC &Body() const
    { return RCHandleC<ErrorBodyC>::Body(); }
    //: Access body.
    
  public:
    RealT Error(const ClassifierC & classifier, const SampleVectorC & x, const SampleLabelC & labels) const
    { return Body().Error(classifier, x, labels); }
    //: Compute the error of a classifier on a sample set
    //!param: classifier - the classifier
    //!param: x - the input vectors
    //!param: labels - the corresponding labels
    //!return: the probability of mis-classification

    RealT Error(const ClassifierC & classifier, const DataSetVectorLabelC & dset) const
    { return Body().Error(classifier, dset); }
    //: Compute the error of a classifier on a sample set
    //!param: classifier - the classifier
    //!param: dset - the labelled dataset
    //!return: the probability of mis-classification

    SArray1dC<RealT> ErrorByLabel(const ClassifierC & classifier, const DataSetVectorLabelC & dset) const
    { return Body().ErrorByLabel(classifier, dset); }
    //: Compute errors on the data set by label

    RealT Error(const Classifier2C & classifier, const DataSetVectorLabelC & dset, UIntT label, RealT threshold=0) const
    { return Body().Error(classifier, dset, label, threshold); }
    //: Compute the error on the labelled data set and a 2 class classifier if label at or below specified threshold
    
    RealT Error(const ClassifierC & classifier, const DataSetVectorLabelC & dset, const SArray1dC<IndexC> & features) const
    { return Body().Error(classifier, dset, features); }
    //: Compute the error on the data set only using features specified
    
  };
  
  inline istream &operator>>(istream &strm,ErrorC &obj) {
    obj = ErrorC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline ostream &operator<<(ostream &out,const ErrorC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,ErrorC &obj) {
    obj = ErrorC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const ErrorC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
}

#endif
