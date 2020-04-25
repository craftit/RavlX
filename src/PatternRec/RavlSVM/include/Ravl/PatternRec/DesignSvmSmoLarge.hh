// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here

#ifndef RAVL_DESIGN_SVM_SMO_LARGE_HEADER
#define RAVL_DESIGN_SVM_SMO_LARGE_HEADER 1

//////////////////////////////////////////////////////////////////////////////
//! file =      "Ravl/PatternRec/Svm/DesignClassifierSvmSmo.hh"
//! author =    "Alexey Kostin"
//! lib=RavlSVM
//! date =      "06/09/05"
//! rcsid =     "$Id: DesignClassifierSvmSmo.hh,v 1.2 2005/05/12 14:19:26 alex Exp $"
//! docentry =  "Ravl.API.Pattern_Recognition.Classifier.SVM"
//////////////////////////////////////////////////////////////////////////////

#include "Ravl/PatternRec/DesignClassifierSvmSmo.hh"
#include "Ravl/XMLFactory.hh"

namespace RavlN
{

//! userlevel = Develop
//: Class for designing SVM classifier with large training set
class DesignSvmSmoLargeBodyC : public DesignSvmSmoBodyC
{
public:
  DesignSvmSmoLargeBodyC(const KernelFunctionC &KernelFunction,
                         IntT MaxNumSv = 10000,
                         RealT Penalty1 = 1e3, RealT Penalty2 = 1e3,
                         RealT Tolerance = 1e-7, RealT Eps = 1e-9,
                         RealT LambdaThreshold = 1e-12);
  //: constructor.

  DesignSvmSmoLargeBodyC(const XMLFactoryContextC & factory);
  //: factory constructor

  DesignSvmSmoLargeBodyC(istream &strm);
  //: Load from stream.

  DesignSvmSmoLargeBodyC(BinIStreamC &strm);
  //: Load from binary stream.

  virtual bool Save(ostream &out) const;
  //: Writes object to stream

  virtual bool Save(BinOStreamC &out) const;
  //: Writes object to stream

  virtual ~DesignSvmSmoLargeBodyC();
  //: destructor

  Classifier2C Apply(const SampleC<VectorC> &TrainingSetVectors,
                     const SampleC<SByteT> &TrainingSetLabels);
  //: creates classifier

  ClassifierC Apply(const SampleC<VectorC> &TrainingSetVectors,
                    const SampleC<UIntT> &TrainingSetLabels);
  //: creates classifier

  IntT LeaveOneOutTest();
protected:
  int maxNumSv;
  int* objectsToUseLarge; //array of objects to use in large training set
  int objectsToUseLargeSize;
  int* kernelCacheIndices; //array for back projecting objectID into kernelCacheID


  void Prepare(const SampleC<VectorC> &TrainingSetVectors,
               const SampleC<SByteT> &TrainingSetLabels);
  void Prepare(const SampleC<VectorC> &TrainingSetVectors,
               const SampleC<UIntT> &TrainingSetLabels);

  void CalcLambdas(bool DoFinal = false);

  virtual RealT GetKernelValue(int I1, int I2);
};
//---------------------------------------------------------------------------
//! userlevel = Normal
//: Class for designing SVM classifier
class DesignSvmSmoLargeC : public DesignSvmSmoC
{
public:
  DesignSvmSmoLargeC()
  {};
  //: Default constructor.

  DesignSvmSmoLargeC(const KernelFunctionC &KernelFunction,
                     IntT MaxNumSv = 10000,
                     RealT Penalty1 = 1e3, RealT Penalty2 = 1e3,
                     RealT Tolerance = 1e-7, RealT Eps = 1e-9,
                     RealT LambdaThreshold = 1e-12)
    : DesignSvmSmoC(*new DesignSvmSmoLargeBodyC(KernelFunction, MaxNumSv,
                                                Penalty1, Penalty2,
                                                Tolerance, Eps, LambdaThreshold))
  {};
  //: constructor.

  DesignSvmSmoLargeC(const XMLFactoryContextC &factory)
    :  DesignSvmSmoC(*new DesignSvmSmoLargeBodyC(factory))
  {}
  //: Construct from XML factory
    
  DesignSvmSmoLargeC(istream &strm);
  //: Load from stream.

  DesignSvmSmoLargeC(BinIStreamC &strm);
  //: Load from binary stream.

protected:
  DesignSvmSmoLargeC(DesignSvmSmoBodyC &bod) : DesignSvmSmoC(bod)
  {}
  //: Body constructor.

  DesignSvmSmoLargeBodyC &Body()
  { return static_cast<DesignSvmSmoLargeBodyC &>(DesignSvmSmoC::Body()); }
  //: Access body.

  const DesignSvmSmoLargeBodyC &Body() const
  { return static_cast<const DesignSvmSmoLargeBodyC &>(DesignSvmSmoC::Body()); }
  //: Access body.
};

} // end of namespace RavlN

#endif
