// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here

#ifndef RAVL_SVM_QUADRATIC_CLASSIFIER_HEADER
#define RAVL_SVM_QUADRATIC_CLASSIFIER_HEADER 1

//////////////////////////////////////////////////////////////////////////////
//! file =      "Ravl/PatternRec/Svm/SvmQuadraticClassifier.hh"
//! author =    "Alexey Kostin"
//! lib=RavlSVM
//! date =      "11/11/05"
//! rcsid =     "$Id: SvmQuadraticClassifier.hh,v 1.2 2005/05/12 14:19:26 alex Exp $"
//! docentry =  "Ravl.API.Pattern_Recognition.Classifier.SVM"
//////////////////////////////////////////////////////////////////////////////

#include "Ravl/PatternRec/Classifier2.hh"

namespace RavlN
{
using namespace RavlN;

//! Support vector machines quadratic classifier
class SvmQuadraticClassifierBodyC : public Classifier2BodyC
{
public:
  //! Default constructor
  //! Creates empty classifier
  SvmQuadraticClassifierBodyC();

  //! Load from stream.
  SvmQuadraticClassifierBodyC(istream &Strm);

  //! Load from binary stream.
  SvmQuadraticClassifierBodyC(BinIStreamC &Strm);

  //! Construct from raw components
  SvmQuadraticClassifierBodyC(const VectorC &w1,const VectorC &w2,RealT threshold);

  //! Destructor
  virtual ~SvmQuadraticClassifierBodyC();

  //! Writes object to stream
  //! returns always true (compatibility issue), on error an exception is thrown
  virtual bool Save(ostream &Out) const;

  //! Writes object to stream
  //! returns always true (compatibility issue), on error an exception is thrown
  virtual bool Save(BinOStreamC &Out) const;

  //! Classify vector 'data' and return value of descriminant function
  RealT Classify2(const VectorC & Data) const;

  //! Get vector length of classifier
  IntT GetDataSize() const;

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
  
  RealT Threshold() const
  { return m_threshold; }
  //: Threshold for classifier.
  
  const VectorC & Weights1() const
  { return m_weights1; }
  //: Linear terms.
  
  const VectorC & Weights2() const
  { return m_weights2; }
  //: Square terms.
  
private:
  void DestroyBuffers();               //!< free allocated memory
  void CreateBuffers(int halfWeights); //!< allocate memory for weights
  void InitMembers();                  //!< initialise member variables
  RealT m_threshold;                   //!< threshold
  VectorC m_weights1;                  //!< weights linear
  VectorC m_weights2;                   //!< weights quadratic
  int m_halfWeights;                   //!< dimensionality of original space
};
//---------------------------------------------------------------------------
//! Support vector machines quadratic classifier
class SvmQuadraticClassifierC  : public Classifier2C
{
public:
  //! Default constructor creates wrong object
  SvmQuadraticClassifierC()
    : Classifier2C(*new SvmQuadraticClassifierBodyC())
  {}

  //! Load from stream.
  SvmQuadraticClassifierC(istream &Strm);

  //! Load from binary stream.
  SvmQuadraticClassifierC(BinIStreamC &Strm);

  //! Base class constructor.
  SvmQuadraticClassifierC(Classifier2C &base)
    : Classifier2C(dynamic_cast<SvmQuadraticClassifierBodyC *>(BodyPtr(base)))
  {}

  //! Construct from raw components
  SvmQuadraticClassifierC(const VectorC &w1,const VectorC &w2,RealT threshold)
   : Classifier2C(new SvmQuadraticClassifierBodyC(w1,w2,threshold))
  {}

  //! Classify vector 'data' and return value of descriminant function
  //! Size of data should be equal to size of support vector.
  RealT Classify2(const VectorC & Data) const
    { return Body().Classify2(Data); }

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

  RealT Threshold() const
  { return Body().Threshold(); }
  //: Threshold for classifier.
  
  const VectorC & Weights1() const
  { return Body().Weights1(); }
  //: Linear terms.
  
  const VectorC & Weights2() const
  { return Body().Weights2(); }
  //: Square terms.

protected:
  //! Body constructor.
  SvmQuadraticClassifierC(SvmQuadraticClassifierBodyC &Bod) : Classifier2C(Bod)
    {}

  //! Access body.
  SvmQuadraticClassifierBodyC &Body()
    { return static_cast<SvmQuadraticClassifierBodyC &>(Classifier2C::Body()); }

  //! Access body.
  const SvmQuadraticClassifierBodyC &Body() const
    { return static_cast<const SvmQuadraticClassifierBodyC &>(Classifier2C::Body()); }
};

//---------------------------------------------------------------------------
//: Stream load operators defined in RAVL_INITVIRTUALCONSTRUCTOR_FULL macro
// Implementation of 'load from stream' constructors defined there as well
//
//inline ostream &operator<<(ostream &out, SvmQuadraticClassifierC &obj)
//inline BinOStreamC &operator<<(BinOStreamC &out, SvmQuadraticClassifierC &obj)

// Stream read operators are defined in RCHandleVC
//and call virtual Save function of body class
//inline ostream &operator<<(ostream &out, const SvmQuadraticClassifierC &obj)
//inline BinOStreamC &operator<<(BinOStreamC &out, const SvmQuadraticClassifierC &obj)

} //end of namespace RavlN
#endif
