// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_ErrorBinaryClassifierC_HEADER
#define RAVL_ErrorBinaryClassifierC_HEADER 1
///////////////////////////////////////////////////////////
//! lib=RavlPatternRec
//! docentry="Ravl.API.Pattern Recognition.Performance Evaluation"
//! author="Kieron Messer"
//! file="Ravl/PatternRec/Performance/Error.hh"

#include "Ravl/PatternRec/Error.hh"
#include "Ravl/PatternRec/Classifier.hh"
#include "Ravl/PatternRec/Classifier2.hh"
#include "Ravl/PatternRec/OneClass.hh"
#include "Ravl/PatternRec/SampleVector.hh"
#include "Ravl/PatternRec/SampleLabel.hh"
#include "Ravl/PatternRec/DataSetVectorLabel.hh"


namespace RavlN {
  
  //! userlevel=Develop
  //: Error computation for binary classifiers
  
  class ErrorBinaryClassifierBodyC : public ErrorBodyC
  {
  public:
    ErrorBinaryClassifierBodyC(UIntT positiveLabel);
    //: Constructor.

    ErrorBinaryClassifierBodyC(const XMLFactoryContextC &factory);
    //: XML factory constructor

    ErrorBinaryClassifierBodyC(std::istream &strm);
    //: Load from stream.
    
    ErrorBinaryClassifierBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual RavlN::RCBodyVC &Copy() const;
    //: Copy me.

    virtual bool Save(std::ostream &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual bool Save(BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    using ErrorBodyC::Error;

    virtual RealT Error(const ClassifierC & classifier, const DataSetVectorLabelC & dset) const;
    //: Compute the error on the labelled dataset

    RealT FalseRejectRate(const ClassifierC & classifier, const DataSetVectorLabelC & dset, RealT falseAcceptRate, RealT & threshold) const;
    //: Return the false reject rate at a given false accept rate.  This assumes label 0 is the positive class.

    RealT FalseAcceptRate(const ClassifierC & classifier, const DataSetVectorLabelC & dset, RealT falseRejectRate, RealT & threshold) const;
    //: Return the false accept rate at a given false reject rate.  This assumes label 0 is the positive class.

    bool Plot(const ClassifierC & classifier, const DataSetVectorLabelC & dset) const;


  protected:
    RealT Error(const ClassifierC & classifier, const DataSetVectorLabelC & dset, bool falseAccept, RealT desiredLabelError, RealT & threshold) const;
    //: Return label 1 error at desired label 0 error and threshold which obtains this

    SArray1dC<Tuple2C<UIntT, RealT> > Scores(const ClassifierC & classifier, const DataSetVectorLabelC & dset) const;
    //: Compute the scores on the data set

    UIntT m_positiveLabel;

  };

  //! userlevel=Normal
  //: Simple error count for a classifier and dataset
  
  class ErrorBinaryClassifierC : public ErrorC
  {
  public:

    ErrorBinaryClassifierC(UIntT positiveLabel = 0) :
        ErrorC(*new ErrorBinaryClassifierBodyC(positiveLabel))
    {
    }
    //: Default constructor.
    
    ErrorBinaryClassifierC(const XMLFactoryContextC &factory);
    //: XML factory constructor

    ErrorBinaryClassifierC(istream &strm);
    //: Load from stream.
    
    ErrorBinaryClassifierC(BinIStreamC &strm);
    //: Load from binary stream.
    
  protected:
    ErrorBinaryClassifierC(ErrorBinaryClassifierBodyC &bod) :
        ErrorC(bod)
    {
    }
    //: Body constructor.

    ErrorBinaryClassifierC(ErrorBinaryClassifierBodyC *bod) :
        ErrorC(bod)
    {
    }
    //: Body ptr constructor.
    
    ErrorBinaryClassifierBodyC &Body()
    {
      return static_cast<ErrorBinaryClassifierBodyC &>(ErrorC::Body());
    }
    //: Access body.
    
    const ErrorBinaryClassifierBodyC &Body() const
    {
      return static_cast<const ErrorBinaryClassifierBodyC &>(ErrorC::Body());
    }
    //: Access body.
    
  public:
    
    RealT Error(const ClassifierC & classifier, const DataSetVectorLabelC & dset) const
    {
      return Body().Error(classifier, dset);
    }
    //: Return eer


    RealT FalseRejectRate(const ClassifierC & classifier, const DataSetVectorLabelC & dset, RealT falseAcceptRate, RealT & threshold) const
    {
      return Body().FalseRejectRate(classifier, dset, falseAcceptRate, threshold);
    }
    //: Return the false reject rate at a given false accept rate.  This assumes label 0 is the positive class.

    RealT FalseAcceptRate(const ClassifierC & classifier, const DataSetVectorLabelC & dset, RealT falseRejectRate, RealT & threshold) const
    {
      return Body().FalseAcceptRate(classifier, dset, falseRejectRate, threshold);
    }
    //: Return the false accept rate at a given false reject rate.  This assumes label 0 is the positive class.

    bool Plot(const ClassifierC & classifier, const DataSetVectorLabelC & dset) const {
      return Body().Plot(classifier, dset);
    }
    //: Make a plot of the errors


  };
  
  inline istream &operator>>(istream &strm, ErrorBinaryClassifierC &obj)
  {
    obj = ErrorBinaryClassifierC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline ostream &operator<<(ostream &out, const ErrorBinaryClassifierC &obj)
  {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm, ErrorBinaryClassifierC &obj)
  {
    obj = ErrorBinaryClassifierC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out, const ErrorBinaryClassifierC &obj)
  {
    obj.Save(out);
    return out;
  }
//: Save to a stream.
// Uses virtual constructor.

}

#endif
