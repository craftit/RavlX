// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
// $Id: DesignSvm.hh 12208 2008-02-07 12:54:07Z alex $
//! docentry =  "Ravl.API.Pattern_Recognition.Classifier.SVM"
//! lib=RavlSVM

//! author="Alexey Kostin"

#ifndef RAVL_DESIGN_SVM_HEADER
#define RAVL_DESIGN_SVM_HEADER 1

#include "Ravl/PatternRec/DesignClassifierSupervised.hh"
#include "Ravl/PatternRec/SvmClassifier.hh"
#include "Ravl/PatternRec/KernelFunc.hh"

namespace RavlN
{
using namespace RavlN;

//! userlevel = Develop
//: Abstract Class for designing SVM classifier

class DesignSvmBodyC : public DesignClassifierSupervisedBodyC
{
public:
  typedef void CallbackFuncT(void *data, int Progr);

  DesignSvmBodyC();
  //: constructor.

  DesignSvmBodyC(const XMLFactoryContextC & factory);
  //: factory constructor
  
  DesignSvmBodyC(istream &strm);
  //: Load from stream.

  DesignSvmBodyC(BinIStreamC &strm);
  //: Load from binary stream.

  virtual bool Save(ostream &out) const;
  //: Writes object to stream

  virtual bool Save(BinOStreamC &out) const;
  //: Writes object to stream

  virtual ~DesignSvmBodyC();
  //: destructor

  void SetCallbackFunc(CallbackFuncT *Func, void *Data)
    { callbackFunc = Func; callbackData = Data; }
  //: Sets callback function

  using DesignClassifierSupervisedBodyC::Apply;

  virtual Classifier2C Apply(const SampleC<VectorC> &TrainingSetVectors,
                             const SampleC<SByteT> &TrainingSetLabels,
                             const SArray1dC<IndexC> &ObjectsToUse);
  //: creates classifier

  virtual ClassifierC Apply(const SampleC<VectorC> &TrainingSetVectors,
                            const SampleC<UIntT> &TrainingSetLabels,
                            const SArray1dC<IndexC> &ObjectsToUse);
  //: creates classifier
  virtual Classifier2C Apply(const SampleC<VectorC> &TrainingSetVectors,
                             const SampleC<SByteT> &TrainingSetLabels);
  //: creates classifier

  virtual ClassifierC Apply(const SampleC<VectorC> &TrainingSetVectors,
                            const SampleC<UIntT> &TrainingSetLabels);
  //: creates classifier
  
  virtual bool Reset();
  //: Reset the designer to an initial state
  
  virtual IntT NumSupportVectors() const;
  //: returns number of support vectors
protected:
  CallbackFuncT *callbackFunc;
  void *callbackData;
};
//---------------------------------------------------------------------------
//! userlevel = Normal
//: Class for designing SVM classifier
class DesignSvmC : public DesignClassifierSupervisedC
{
public:
  DesignSvmC()
  {};
  //: Default constructor.

  DesignSvmC(const XMLFactoryContextC &factory)
    :  DesignClassifierSupervisedC(*new DesignSvmBodyC(factory))
  {}
  //: Construct from XML factory

  DesignSvmC(istream &strm);
  //: Load from stream.

  DesignSvmC(BinIStreamC &strm);
  //: Load from binary stream.

  Classifier2C Apply(const SampleC<VectorC> &TrainingSetVectors,
                     const SampleC<SByteT> &TrainingSetLabels,
                     const SArray1dC<IndexC> &ObjectsToUse)
    { return BodyPtr()->Apply(TrainingSetVectors, TrainingSetLabels, ObjectsToUse); }
  //: creates classifier
  //!param: TrainingSetVectors - training set of feature vectors
  //!param: TrainingSetLabels - trainig set labels ('-1' for first class; '1' for second class)
  //!param: ObjectsToUse - indices of objects in training set which should be used for training

  ClassifierC Apply(const SampleC<VectorC> &TrainingSetVectors,
                    const SampleC<UIntT> &TrainingSetLabels,
                    const SArray1dC<IndexC> &ObjectsToUse)
    { return BodyPtr()->Apply(TrainingSetVectors, TrainingSetLabels, ObjectsToUse); }
  //: creates classifier
  //!param: TrainingSetVectors - training set of feature vectors
  //!param: TrainingSetLabels - trainig set labels ('0' for first class; '1' for second class)
  //!param: ObjectsToUse - indices of objects in training set which should be used for training

  Classifier2C Apply(const SampleC<VectorC> &TrainingSetVectors,
                     const SampleC<SByteT> &TrainingSetLabels)
    { return BodyPtr()->Apply(TrainingSetVectors, TrainingSetLabels); }
  //: creates classifier
  //!param: TrainingSetVectors - training set of feature vectors
  //!param: TrainingSetLabels - trainig set labels ('-1' for first class; '1' for second class)

  ClassifierC Apply(const SampleC<VectorC> &TrainingSetVectors,
                    const SampleC<UIntT> &TrainingSetLabels)
    { return BodyPtr()->Apply(TrainingSetVectors, TrainingSetLabels); }
  //: creates classifier
  //!param: TrainingSetVectors - training set of feature vectors
  //!param: TrainingSetLabels - trainig set labels ('0' for first class; '1' for second class)

  void SetCallbackFunc(DesignSvmBodyC::CallbackFuncT *Func, void *Data)
    { return Body().SetCallbackFunc(Func, Data); }
  //: Sets callback function

  IntT NumSupportVectors()
    { return BodyPtr()->NumSupportVectors(); }

protected:
  DesignSvmC(DesignSvmBodyC &Bod) : DesignClassifierSupervisedC(Bod)
    {}
  //: Body constructor.

  DesignSvmC(DesignSvmBodyC *Bod) : DesignClassifierSupervisedC(Bod)
    {}
  //: Body constructor.

  DesignSvmBodyC &Body()
  { return static_cast<DesignSvmBodyC &>(DesignClassifierSupervisedC::Body()); }
  //: Access body.

  DesignSvmBodyC* BodyPtr()
    { return (DesignSvmBodyC*)&(DesignClassifierSupervisedC::Body()); }
  //: Access body.

  const DesignSvmBodyC &Body() const
    { return static_cast<const DesignSvmBodyC &>(DesignClassifierSupervisedC::Body()); }
  //: Access body.
};

} // end of namespace RavlN

#endif
