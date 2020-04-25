// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here

#ifndef RAVL_SVM_CLASSIFIER_HEADER
#define RAVL_SVM_CLASSIFIER_HEADER 1

//////////////////////////////////////////////////////////////////////////////
//! file =      "Ravl/API/PatternRec/Svm/SvmClassifier.hh"
//! author =    "Alexey Kostin"
//! lib=RavlSVM
//! date =      "03/10/05"
//! rcsid =     "$Id: SvmClassifier.hh 12208 2008-02-07 12:54:07Z alex $"
//! docentry =  "Ravl.API.Pattern_Recognition.Classifier.SVM"
//////////////////////////////////////////////////////////////////////////////

#include "Ravl/PatternRec/Classifier2.hh"
#include "Ravl/PatternRec/Sample.hh"
#include "Ravl/PatternRec/KernelFunc.hh"

namespace RavlN
{
using namespace RavlN;

class DesignSvmSmoBodyC;
//! userlevel = Develop
//: Support vector machines classifier
class SvmClassifierBodyC : public Classifier2BodyC
{
public:
  SvmClassifierBodyC();
  //: Creates empty classifier, which returns 0 as the value of descriminant function

  SvmClassifierBodyC(const KernelFunctionC &KernelFunction,
                     const SampleC<VectorC> &SupportVectors,
                     const SampleC<UIntT> &SupportVectorLabels,
                     const SampleC<RealT> &Lambdas, RealT Threshold);
  //: Constructor

  SvmClassifierBodyC(const KernelFunctionC &KernelFunction,
                     const SampleC<VectorC> &SupportVectors,
                     const char *SupportVectorLabels,
                     const RealT *Lambdas, RealT Threshold);
  //: Constructor

  SvmClassifierBodyC(istream &Strm);
  //: Load from stream.

  SvmClassifierBodyC(BinIStreamC &Strm);
  //: Load from binary stream.

  virtual ~SvmClassifierBodyC();
  //: Destructor

  virtual bool Save(ostream &Out) const;
  //: Writes object to stream
  // returns always true (compatibility issue), on error an exception is thrown

  virtual bool Save(BinOStreamC &Out) const;
  //: Writes object to stream
  // returns always true (compatibility issue), on error an exception is thrown

  virtual VectorC Apply(const VectorC &data) const;
  //: Estimate the confidence for each label.
  // The meaning of the confidence assigned to each label depends
  // on the classifier used. The higher the confidence the more likely
  // it is the label is correct.

  virtual RealT Classify2(const VectorC &Data) const;
  //: Classify vector 'data' and return value of descriminant function

  //virtual RealT Classify2(const RealT* Data) const;
  //: Classify vector 'data' and return value of descriminant function

  virtual IntT GetDataSize() const;
  //: Get vector length of classifier

  const KernelFunctionC& KernelFunction() const
    { return kernelFunction; }
  const SampleC<VectorC>& SupportVectors() const
    { return supportVectors; }
  const RealT* Lambdas() const
    { return lambdas; }
  RealT Threshold() const
    { return threshold; }
private:
  void DestroyBuffers();
  KernelFunctionC kernelFunction;
  SampleC<VectorC> supportVectors;
  double* *trSetVectorPtrs; //need for fast random access
  int numFeatures;
  RealT *lambdas;
  RealT threshold;
  SizeT numSv;
};
//---------------------------------------------------------------------------
//! userlevel = Normal
//: Support vector machines classifier
// The class stores inside support vectors and corresponding class labels and
// Lagrangian multipliers. Classifier function only calculates value of
// Descriminant function. For designing classifier use DesignSvmSmoC class.<p>
class SvmClassifierC : public Classifier2C
{
public:
  SvmClassifierC()
    : Classifier2C(*new SvmClassifierBodyC())
    {}
  //: Creates empty classifier, which returns 0 as the value of descriminant function

  SvmClassifierC(const KernelFunctionC &KernelFunction,
                 const SampleC<VectorC> &SupportVectors,
                 const SampleC<UIntT> &SupportVectorLabels,
                 const SampleC<RealT> &Lambdas, RealT Threshold)
    : Classifier2C(*new SvmClassifierBodyC(KernelFunction, SupportVectors,
                                       SupportVectorLabels, Lambdas, Threshold))
    {}
  //: Constructor.
  //!param: KernelFunction      - kernel function for classifier
  //!param: SupportVectors      - support vectors
  //!param: SupportVectorLabels - labels of support vectors (labels == 0 correspond to negatives class, all other to positives)
  //!param: Lambdas             - Lagrangian multipliers for each support vector
  //!param: Threshold           - threshold

  SvmClassifierC(const KernelFunctionC &KernelFunction,
                 const SampleC<VectorC> &SupportVectors,
                 const char *SupportVectorLabels,
                 const RealT *Lambdas, RealT Threshold)
    : Classifier2C(*new SvmClassifierBodyC(KernelFunction, SupportVectors,
                                      SupportVectorLabels, Lambdas, Threshold))
    {}
  //: Constructor.
  //!param: KernelFunction      - kernel function for classifier
  //!param: SupportVectors      - support vectors
  //!param: SupportVectorLabels - labels of support vectors (-1 correspond to negatives class, +1 to positives)
  //!param: Lambdas             - Lagrangian multipliers for each support vector
  //!param: Threshold           - threshold
  // Number of elements in SupportVectorLabels and Lambdas should be equal to
  // number of support vectors.

  SvmClassifierC(istream &Strm);
  //: Load from stream.

  SvmClassifierC(BinIStreamC &Strm);
  //: Load from binary stream.

  UIntT Classify(const VectorC &Data) const
    { return Body().Classify(Data); }
  //: Classify vector 'data' and return class (0 - negatives or 1 - positives)
  // Size of data should be equal to size of support vector.

  //UIntT Classify(const RealT* Data) const
  //  { return Body().Classify(Data); }
  //: Classify vector 'data' and return class (0 - negatives or 1 - positives)
  // Size of data should be equal to size of support vector.

  RealT Classify2(const VectorC &Data) const
  { return Body().Classify2(Data); }
  //: Classify vector 'data' and return value of descriminant function
  // Size of data should be equal to size of support vector.

  //RealT Classify2(const RealT* Data) const
  //  { return Body().Classify2(Data); }
  //: Classify vector 'data' and return value of descriminant function
  // Size of data should be equal to size of support vector.

  const KernelFunctionC& KernelFunction() const
    { return Body().KernelFunction(); }
  const SampleC<VectorC>& SupportVectors() const
    { return Body().SupportVectors(); }
  const RealT* Lambdas() const
    { return Body().Lambdas(); }
  RealT Threshold() const
    { return Body().Threshold(); }

protected:
  SvmClassifierC(SvmClassifierBodyC &Bod) : Classifier2C(Bod)
    {}
  //: Body constructor.

  SvmClassifierBodyC &Body()
    { return static_cast<SvmClassifierBodyC &>(Classifier2C::Body()); }
  //: Access body.

  const SvmClassifierBodyC &Body() const
    { return static_cast<const SvmClassifierBodyC &>(Classifier2C::Body()); }
  //: Access body.
};

//---------------------------------------------------------------------------
//: Stream load operators defined in RAVL_INITVIRTUALCONSTRUCTOR_FULL macro
// Implementation of 'load from stream' constructors defined there as well
//inline ostream &operator<<(ostream &out, SvmClassifierC &obj)
//inline BinOStreamC &operator<<(BinOStreamC &out, SvmClassifierC &obj)

// Stream read operators are defined in RCHandleVC
//and call virtual Save function of body class
//inline ostream &operator<<(ostream &out, const SvmClassifierC &obj)
//inline BinOStreamC &operator<<(BinOStreamC &out,const SvmClassifierC &obj)

} //end of namespace RavlN
#endif
