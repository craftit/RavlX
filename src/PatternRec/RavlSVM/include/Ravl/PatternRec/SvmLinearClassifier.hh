// $Id: KernelFuncFS.hh 7751 2006-01-31 11:59:21Z alex $
// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
//! lib=RavlSVM

//! author="Alexey Kostin"

#ifndef RAVL_SVM_LINEAR_CLASSIFIER_HEADER
#define RAVL_SVM_LINEAR_CLASSIFIER_HEADER 1

#include "Ravl/PatternRec/Classifier2.hh"

namespace RavlN
{
using namespace RavlN;

//! Support vector machines linear classifier (body part)
class SvmLinearClassifierBodyC : public Classifier2BodyC
{
public:
  //! Default constructor.
  //! Creates empty classifier
  SvmLinearClassifierBodyC();

  //! Load from stream.
  SvmLinearClassifierBodyC(istream &Strm);

  //! Load from binary stream.
  SvmLinearClassifierBodyC(BinIStreamC &Strm);

  //! Destructor
  virtual ~SvmLinearClassifierBodyC();

  //! Writes object to stream
  //! returns always true (compatibility issue), on error an exception is thrown
  virtual bool Save(ostream &Out) const;

  //! Writes object to stream
  //! returns always true (compatibility issue), on error an exception is thrown
  virtual bool Save(BinOStreamC &Out) const;

  //! Classify vector 'data' and return value of descriminant function
  RealT Classify2(const VectorC &data) const;

  //! Classify vector 'data' and return value of descriminant function
  RealT Classify2(const RealT &data) const;

  //! Get vector length of classifier
  IntT GetDataSize() const;

  //! Get threshold
  RealT GetThreshold() const
    { return m_threshold; }

  //! Get weights
  const SArray1dC<RealT>& GetWeights() const
    { return m_weights; }

  //! Create classifier
  /**
  @param Sv support vectors
  @param Lambdas Lagrangian multipliers
  @param Scale global scale from kernel function
  @param Threshold threshold
   */
  void Create(const SampleC<VectorC>& Sv, const RealT* Lambdas,
              RealT Scale, RealT Threshold);

  //! Create classifier
  /**
  @param Sv support vectors
  @param Lambdas Lagrangian multipliers
  @param Weights weights for features
  @param Scale global scale from kernel function
  @param Threshold threshold
   */
  void Create(const SampleC<VectorC>& Sv, const RealT* Lambdas,
              const RealT* Weights, RealT Scale, RealT Threshold);
private:
  void DestroyBuffers();              //!< free allocated memory
  void CreateBuffers(SizeT NumWeights); //!< allocate memory for weights
  void InitMembers();                 //!< initialise member variables
  RealT m_threshold;                  //!< threshold
  VectorC m_weights;                  //!< weights
};
//---------------------------------------------------------------------------
//! Support vector machines linear classifier
class SvmLinearClassifierC
  : public Classifier2C
{
public:
  //! Default constructor creates empty classifier
  SvmLinearClassifierC()
   : Classifier2C(*new SvmLinearClassifierBodyC())
  {}

  SvmLinearClassifierC(Classifier2C &base)
   : Classifier2C(dynamic_cast<SvmLinearClassifierBodyC *>(BodyPtr(base)))
  {}

  //! Load from stream.
  SvmLinearClassifierC(istream &Strm);

  //! Load from binary stream.
  SvmLinearClassifierC(BinIStreamC &Strm);

  //! Classify vector 'data' and return value of descriminant function
  //! Size of data should be equal to size of support vector.
  RealT Classify2(const VectorC &data) const
    { return Body().Classify2(data); }

  //! Get vector length of classifier
  IntT GetDataSize() const
    { return Body().GetDataSize(); }

  //! Get threshold
  RealT GetThreshold() const
    { return Body().GetThreshold(); }

  //! Get weights
  const SArray1dC<RealT>& GetWeights() const
    { return Body().GetWeights(); }

  //! Create classifier
  /**
  @param Sv support vectors
  @param Lambdas Lagrangian multipliers
  @param Scale global scale from kernel function
  @param Threshold threshold
   */
  void Create(const SampleC<VectorC>& Sv, const RealT* Lambdas,
              RealT Scale, RealT Threshold)
    { Body().Create(Sv, Lambdas, Scale, Threshold); }

  //! Create classifier
  /**
  @param Sv support vectors
  @param Lambdas Lagrangian multipliers
  @param Weights weights for features
  @param Scale global scale from kernel function
  @param Threshold threshold
   */
  void Create(const SampleC<VectorC>& Sv, const RealT* Lambdas,
              const RealT* Weights, RealT Scale, RealT Threshold)
    { Body().Create(Sv, Lambdas, Weights, Scale, Threshold); }

protected:
  //! Body constructor.
  SvmLinearClassifierC(SvmLinearClassifierBodyC &Bod)
    : Classifier2C(Bod)
  {}

  //! Access body.
  SvmLinearClassifierBodyC &Body()
    { return static_cast<SvmLinearClassifierBodyC &>(Classifier2C::Body()); }

  //! Access body.
  const SvmLinearClassifierBodyC &Body() const
    { return static_cast<const SvmLinearClassifierBodyC &>(Classifier2C::Body()); }
};

//---------------------------------------------------------------------------
//: Stream load operators defined in RAVL_INITVIRTUALCONSTRUCTOR_FULL macro
// Implementation of 'load from stream' constructors defined there as well
//
//inline ostream &operator<<(ostream &out, SvmLinearClassifierC &obj)
//inline BinOStreamC &operator<<(BinOStreamC &out, SvmLinearClassifierC &obj)

// Stream read operators are defined in RCHandleVC
//and call virtual Save function of body class
//inline ostream &operator<<(ostream &out, const SvmLinearClassifierC &obj)
//inline BinOStreamC &operator<<(BinOStreamC &out, const SvmLinearClassifierC &obj)

} //end of namespace RavlN
#endif
