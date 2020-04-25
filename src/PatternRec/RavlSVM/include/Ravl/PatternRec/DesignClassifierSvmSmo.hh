// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here

#ifndef RAVL_DESIGN_CLASSIFIER_SVM_SMO_HEADER
#define RAVL_DESIGN_CLASSIFIER_SVM_SMO_HEADER 1

//////////////////////////////////////////////////////////////////////////////
//! file =      "Ravl/PatternRec/Svm/DesignClassifierSvmSmo.hh"
//! author =    "Alexey Kostin"
//! lib=RavlSVM
//! date =      "3/07/03"
//! rcsid =     "$Id: DesignClassifierSvmSmo.hh 12208 2008-02-07 12:54:07Z alex $"
//! docentry =  "Ravl.API.Pattern_Recognition.Classifier.SVM"
//////////////////////////////////////////////////////////////////////////////

#include "Ravl/PatternRec/DesignSvm.hh"
#include "Ravl/PatternRec/SvmClassifier.hh"
#include "Ravl/PatternRec/KernelFunc.hh"

namespace RavlN
{
using namespace RavlN;

//! userlevel = Develop
//: Class for designing SVM classifier

class DesignSvmSmoBodyC : public DesignSvmBodyC
{
public:
  typedef void CallbackFuncT(void *data, int Progr);

  DesignSvmSmoBodyC(const KernelFunctionC &KernelFunction,
                    RealT Penalty1 = 1e3, RealT Penalty2 = 1e3,
                    RealT Tolerance = 1e-7, RealT Eps = 1e-9,
                    RealT LambdaThreshold = 1e-12);
  //: constructor.

  DesignSvmSmoBodyC(const XMLFactoryContextC & factory);
  //: factory constructor  

  DesignSvmSmoBodyC(istream &strm);
  //: Load from stream.

  DesignSvmSmoBodyC(BinIStreamC &strm);
  //: Load from binary stream.

  virtual bool Save(ostream &out) const;
  //: Writes object to stream

  virtual bool Save(BinOStreamC &out) const;
  //: Writes object to stream

  virtual ~DesignSvmSmoBodyC();
  //: destructor

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

  SArray1dC<RealT> GetLambdas() const
  //: return Lagrangian multipliers
    { return SArray1dC<RealT>(BufferC<RealT>(trainSetSize,lambdas,true,true),
                              trainSetSize); }

  RealT GetThreshold() const
    { return b; }

  virtual IntT NumSupportVectors() const;
  //: returns number of support vectors

  virtual void SetInitialLambdas(const SampleC<RealT> &InitialLambdas)
    { initialLambdas = InitialLambdas; }

  IntT LeaveOneOutTest();
  SvmClassifierC GetClassifier() const;

  int& Verbosity()
    { return verbosity; }
protected:
  void Init(const KernelFunctionC &KernelFunction, RealT Penalty1,
            RealT Penalty2, RealT Tolerance, RealT Eps, RealT LambdaThreshold);



  void CalcLambdas();
  KernelFunctionC kernelFunction;
  RealT c1, c2;
  RealT tolerance;
  RealT eps;
  RealT lambdaThreshold;
  int verbosity;

  RealT *errorCache;
  RealT *lambdas;
  RealT b;
  int errorCacheSize;

  int trainSetSize;
  int numTrainObjects;

  SampleC<VectorC> trainSetVectors;
  double* *trSetVectorPtrs; //need for fast random access
  int numFeatures;
  char *trainingSetLabels;
  int *objectsToUse;

  void Prepare(const SampleC<VectorC> &TrainingSetVectors,
               const SampleC<SByteT> &TrainingSetLabels);
  void Prepare(const SampleC<VectorC> &TrainingSetVectors,
               const SampleC<UIntT> &TrainingSetLabels);
  void SetObjectsToUse(const SArray1dC<IndexC> &ObjectsToUse);

  void UpdateErrorCache();
  int ExamineExample(int I1);
  int TakeStep(int I1, int I2);
  double CalcFunc(int I);

  float *kernelCache;
  SizeT kernelCacheSize;

  virtual RealT GetKernelValue(int I1, int I2);

  SampleC<RealT> initialLambdas;
};
//---------------------------------------------------------------------------
//! userlevel = Normal
//: Class for designing SVM classifier
class DesignSvmSmoC : public DesignSvmC
{
public:
  DesignSvmSmoC()
  {};
  //: Default constructor.

  DesignSvmSmoC(const KernelFunctionC &KernelFunction,
                RealT Penalty1 = 1e3, RealT Penalty2 = 1e3,
                RealT Tolerance = 1e-7, RealT Eps = 1e-9,
                RealT LambdaThreshold = 1e-12)
                : DesignSvmC(*new DesignSvmSmoBodyC(KernelFunction,
                                                    Penalty1, Penalty2,
                                                    Tolerance, Eps,
                                                    LambdaThreshold))
    {}
  //: constructor.

  DesignSvmSmoC(const XMLFactoryContextC &factory)
    :  DesignSvmC(*new DesignSvmSmoBodyC(factory))
  {}
  //: Construct from XML factory
  
  DesignSvmSmoC(istream &strm);
  //: Load from stream.

  DesignSvmSmoC(BinIStreamC &strm);
  //: Load from binary stream.

  VectorC GetLambdas()
    { return Body().GetLambdas(); }

  RealT GetThreshold()
    { return Body().GetThreshold(); }

  void SetInitialLambdas(const SampleC<RealT> &InitialLambdas)
    { Body().SetInitialLambdas(InitialLambdas); }

  IntT LeaveOneOutTest()
    { return Body().LeaveOneOutTest(); }

  int& Verbosity()
    { return Body().Verbosity(); }
protected:
  DesignSvmSmoC(DesignSvmSmoBodyC &bod) : DesignSvmC(bod)
    {}
  //: Body constructor.

  DesignSvmSmoBodyC &Body()
   { return static_cast<DesignSvmSmoBodyC &>(DesignSvmC::Body()); }
  //: Access body.

  DesignSvmSmoBodyC* BodyPtr()
    { return (DesignSvmSmoBodyC*)&(DesignSvmC::Body()); }
  //: Access body.

  const DesignSvmSmoBodyC &Body() const
    { return static_cast<const DesignSvmSmoBodyC &>(DesignSvmC::Body()); }
  //: Access body.
};

} // end of namespace RavlN

#endif
