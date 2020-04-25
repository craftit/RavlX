// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
// $Id: DesignOneClass.hh 12208 2008-02-07 12:54:07Z alex $
//! lib=RavlSVM

//! author="Alexey Kostin"

#ifndef RAVL_DESIGN_ONE_CLASS_HEADER
#define RAVL_DESIGN_ONE_CLASS_HEADER

#include "Ravl/PatternRec/DesignSvm.hh"
#include "Ravl/PatternRec/OneClass.hh"
#include "Ravl/PatternRec/KernelFunc.hh"
#include "Ravl/XMLFactory.hh"

namespace RavlN
{
using namespace RavlN;

//! Class for designing one class classifier (body)
class DesignOneClassBodyC : public DesignSvmBodyC
{
public:
  //! constructor.
  DesignOneClassBodyC(const KernelFunctionC &KernelFunction,
                      double Penalty1 = 1e3, double Penalty2 = 1e3,
                      double Tolerance = 1e-7, double Eps = 1e-9,
                      double LambdaThreshold = 1e-12);


  DesignOneClassBodyC(const XMLFactoryContextC & factory);
  //: factory constructor

  //! Load from stream.
  DesignOneClassBodyC(istream &strm);

  //! Load from binary stream.
  DesignOneClassBodyC(BinIStreamC &strm);

  //! Writes object to stream
  virtual bool Save(ostream &out) const;

  //! Writes object to stream
  virtual bool Save(BinOStreamC &out) const;

  //! Destructor
  virtual ~DesignOneClassBodyC();

  //! creates classifier
  virtual Classifier2C Apply(const SampleC<VectorC> &TrainingSetVectors,
                             const SampleC<SByteT> &TrainingSetLabels,
                             const SArray1dC<IndexC> &ObjectsToUse);

  //! creates classifier
  virtual ClassifierC Apply(const SampleC<VectorC> &TrainingSetVectors,
                            const SampleC<UIntT> &TrainingSetLabels,
                            const SArray1dC<IndexC> &ObjectsToUse);

  //! creates classifier
  virtual Classifier2C Apply(const SampleC<VectorC> &TrainingSetVectors,
                             const SampleC<SByteT> &TrainingSetLabels);

  //! creates classifier
  virtual ClassifierC Apply(const SampleC<VectorC> &TrainingSetVectors,
                            const SampleC<UIntT> &TrainingSetLabels);

  //! return Lagrangian multipliers
  SArray1dC<double> GetLambdas() const
    { return SArray1dC<double>(BufferC<double>(trainSetSize, lambdas, true, true),
                              trainSetSize); }

  RealT GetRadius() const
    { return (double)radius; }

  //! returns number of support vectors
  virtual IntT NumSupportVectors() const;

  OneClassC GetClassifier() const;

  int& Debug()
    { return debug; }
protected:
  void CalcLambdas();
  KernelFunctionC kernelFunction;
  double c1, c2;
  double toleranceR; //!< tolerance in fractions of radius
  double toleranceA; //!< absolute value of tolerance
  double eps;
  double lambdaThreshold;

  long double radius, norm;

  double *errorCache;
  double *sCache;
  double *lambdas;
  char   *trainingSetLabels;
  double* *trSetVectorPtrs; //need for fast random access
  int numFeatures;
  int    *objectsToUse; //
  float  *kernelCache;
  int debug;

  int trainSubsetSize;  //!< number of objects in subset used for training
  int trainSetSize;     //!< total number of training objects

  SampleC<VectorC> trainingSetVectors;

  void Prepare();

  void UpdateErrorCache();
  int ExamineExample(int I1);
  int TakeStep(int I1, int I2);

  //! Calculate discriminant function value
  double CalcFunc(int I);

  //! Calculate dot product of center vector and one of the training vectors
  double CalcS(int I) const;

  //! Calculate radius
  long double CalculateRadius(long double &Norm) const;

  virtual double GetKernelValue(int I1, int I2) const;

  void InitMembers();
  void AllocateMemory(int NewTrainSubsetSize, int NewTrainSetSize);
  void ReleaseMemory();
};
//---------------------------------------------------------------------------
//! Class for designing one class classifier
class DesignOneClassC : public DesignSvmC
{
public:
  //! Default constructor.
  DesignOneClassC()
    {};

  //! constructor.
  DesignOneClassC(const KernelFunctionC &KernelFunction,
                  RealT Penalty1 = 1e3, RealT Penalty2 = 1e3,
                  RealT Tolerance = 1e-7, RealT Eps = 1e-9,
                  RealT LambdaThreshold = 1e-12)
   : DesignSvmC(*new DesignOneClassBodyC(KernelFunction, Penalty1, Penalty2,
                                         Tolerance, Eps, LambdaThreshold))
    {}

  
  DesignOneClassC(const XMLFactoryContextC &factory)
    :  DesignSvmC(*new DesignOneClassBodyC(factory))
  {}
  //: Construct from XML factory
  
  //! Load from stream.
  DesignOneClassC(istream &strm);

  //! Load from binary stream.
  DesignOneClassC(BinIStreamC &strm);

  VectorC GetLambdas()
    { return Body().GetLambdas(); }

  RealT GetRadius() const
    { return Body().GetRadius(); }

  int& Debug()
    { return Body().Debug(); }
protected:
  //! Body constructor.
  DesignOneClassC(DesignOneClassBodyC &bod) : DesignSvmC(bod)
    {}

  //! Access body.
  DesignOneClassBodyC &Body()
   { return static_cast<DesignOneClassBodyC &>(DesignSvmC::Body()); }

  //! Access body.
  DesignOneClassBodyC* BodyPtr()
    { return (DesignOneClassBodyC*)&(DesignSvmC::Body()); }

  //! Access body.
  const DesignOneClassBodyC &Body() const
    { return static_cast<const DesignOneClassBodyC &>(DesignSvmC::Body()); }
};

} // end of namespace RavlN

#endif
