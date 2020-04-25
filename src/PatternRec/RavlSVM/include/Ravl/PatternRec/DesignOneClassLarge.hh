// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
// $Id: DesignOneClassLarge.hh 12208 2008-02-07 12:54:07Z alex $
//! lib=RavlSVM

//! author="Alexey Kostin"

#ifndef RAVL_DESIGN_ONE_CLASS_LARGE_HEADER
#define RAVL_DESIGN_ONE_CLASS_LARGE_HEADER

#include "Ravl/PatternRec/DesignOneClass.hh"
#include "Ravl/XMLFactory.hh"

namespace RavlN
{
using namespace RavlN;

//! Class for designing SVM one class classifier with large training set
class DesignOneClassLargeBodyC : public DesignOneClassBodyC
{
public:
  //! constructor
  DesignOneClassLargeBodyC(const KernelFunctionC &KernelFunction,
                         IntT MaxNumSv = 10000,
                         RealT Penalty1 = 1e3, RealT Penalty2 = 1e3,
                         RealT Tolerance = 1e-7, RealT Eps = 1e-9,
                         RealT LambdaThreshold = 1e-12);


  DesignOneClassLargeBodyC(const XMLFactoryContextC & factory);
  //: factory constructor

  //! Load from stream.
  DesignOneClassLargeBodyC(istream &strm);

  //! Load from binary stream.
  DesignOneClassLargeBodyC(BinIStreamC &strm);

  //! Writes object to stream
  virtual bool Save(ostream &out) const;

  //! Writes object to stream
  virtual bool Save(BinOStreamC &out) const;

  //! destructor
  virtual ~DesignOneClassLargeBodyC();

  //! creates classifier
  Classifier2C Apply(const SampleC<VectorC> &TrainingSetVectors,
                     const SampleC<SByteT> &TrainingSetLabels);

  //! creates classifier
  ClassifierC Apply(const SampleC<VectorC> &TrainingSetVectors,
                    const SampleC<UIntT> &TrainingSetLabels);

  //! creates classifier
  Classifier2C Apply(const SampleC<VectorC> &TrainingSetVectors,
                     const SampleC<SByteT> &TrainingSetLabels,
                     const SArray1dC<IndexC> &ObjectsToUse);

  //! creates classifier
  ClassifierC Apply(const SampleC<VectorC> &TrainingSetVectors,
                    const SampleC<UIntT> &TrainingSetLabels,
                    const SArray1dC<IndexC> &ObjectsToUse);
protected:
  int maxNumSv;
  int* objectsToUseLarge; //array of objects to use in large training set
  int objectsToUseLargeSize;
  int* kernelCacheIndices; //array for back projecting objectID into kernelCacheID
  double* localSCache;
  double* localErrCache;
  double* localLambdas;
  int* changedIndices;

  void Prepare();

  void CalcLambdas(bool DoFinal = false);

  virtual RealT GetKernelValue(int I1, int I2) const;
  void InitMembers();
  void AllocateMemory(int NewTrainSubsetSize, int NewTrainSetSize);
  void ReleaseMemory();
};
//---------------------------------------------------------------------------
//! Class for designing SVM one  class classifier
class DesignOneClassLargeC : public DesignOneClassC
{
public:
  //! Default constructor.
  DesignOneClassLargeC()
    {};

  //! constructor
  DesignOneClassLargeC(const KernelFunctionC &KernelFunction,
                      IntT MaxNumSv = 10000,
                      RealT Penalty1 = 1e3, RealT Penalty2 = 1e3,
                      RealT Tolerance = 1e-7, RealT Eps = 1e-9,
                      RealT LambdaThreshold = 1e-12)
    : DesignOneClassC(*new DesignOneClassLargeBodyC(KernelFunction, MaxNumSv,
                                                    Penalty1, Penalty2,
                                                    Tolerance, Eps,
                                                    LambdaThreshold))
    {}

  DesignOneClassLargeC(const XMLFactoryContextC &factory)
    :  DesignOneClassC(*new DesignOneClassLargeBodyC(factory))
  {}
  //: Construct from XML factory

  //! Load from stream.
  DesignOneClassLargeC(istream &strm);

  //! Load from binary stream.
  DesignOneClassLargeC(BinIStreamC &strm);

protected:
  //! Body constructor.
  DesignOneClassLargeC(DesignOneClassBodyC &bod) : DesignOneClassC(bod)
    {}

  //! Access body.
  DesignOneClassLargeBodyC &Body()
    { return static_cast<DesignOneClassLargeBodyC &>(DesignOneClassC::Body()); }

  //! Access body.
  const DesignOneClassLargeBodyC &Body() const
    { return static_cast<const DesignOneClassLargeBodyC &>(DesignOneClassC::Body()); }
};

} // end of namespace RavlN

#endif
