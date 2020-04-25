// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
// $Id: OneClass.hh 12208 2008-02-07 12:54:07Z alex $
//! lib=RavlSVM

#ifndef RAVL_ONE_CLASS_HEADER
#define RAVL_ONE_CLASS_HEADER

#include "Ravl/PatternRec/Classifier2.hh"
#include "Ravl/PatternRec/Sample.hh"
#include "Ravl/PatternRec/KernelFunc.hh"

namespace RavlN
{
using namespace RavlN;

/**
  @author Alexey Kostin
 */
//! One class classifier (body)
/*! The one class classifier has two parameters centre and radius of the
 *  hypersphere around objects of the class. This class implements
 *  the decision function f(x)=radius^2 - |x-centre|^2 in more generic way
 *  f(x)= radius^2 - K(x,x) - 2*sum(lambda_i*K(x,x_i)) +
 *        sum(sum(y_i*y_j*lambda_i*lambda_j*K(x_i,x_j)))
 */
class OneClassBodyC : public Classifier2BodyC
{
public:
  //! Creates empty classifier, which returns 0 as the value of descriminant function
  OneClassBodyC();

  //! Constructor
  OneClassBodyC(const KernelFunctionC &KernelFunction,
                const SampleC<VectorC> &SupportVectors,
                const SampleC<UIntT> &SupportVectorLabels,
                const SampleC<RealT> &Lambdas,
                RealT Radius);

  //! Constructor
  OneClassBodyC(const KernelFunctionC &KernelFunction,
                const SampleC<VectorC> &SupportVectors,
                const char *SupportVectorLabels,
                const RealT *Lambdas,
                RealT Radius);

  //! Load from stream.
  OneClassBodyC(istream &Strm);

  //! Load from binary stream.
  OneClassBodyC(BinIStreamC &Strm);

  //! Destructor
  virtual ~OneClassBodyC();

  //! Writes object to stream
  virtual bool Save(ostream &Out) const;

  //! Writes object to stream
  virtual bool Save(BinOStreamC &Out) const;

  //! Classify vector 'data' and return value of descriminant function
  virtual RealT Classify2(const VectorC &Data) const;

  //! Classify vector 'data' and return value of descriminant function
  //virtual RealT Classify2(const RealT* Data) const;

  //! Get vector length for classifier
  virtual IntT GetDataSize() const;

  const KernelFunctionC& KernelFunction() const
    { return kernelFunction; }

  const SampleC<VectorC>& SupportVectors() const
    { return supportVectors; }

  const double* Lambdas() const
    { return lambdas; }

  double& Radius()
    { return radius; }

  double Radius() const
    { return radius; }

private:
    void DestroyBuffers();

  //! Calculate radius
  double CalculateRadius(double &Sum) const;

  KernelFunctionC kernelFunction;
  SampleC<VectorC> supportVectors;
  double *lambdas;
  double radius, sum;
  SizeT numSv;
};
//---------------------------------------------------------------------------
//! One class classifier
class OneClassC : public Classifier2C
{
public:
  //! Creates empty classifier, which returns 0 as the value of descriminant function
  OneClassC() : Classifier2C(*new OneClassBodyC())
    {}

  //! Constructor.
  /*!
  @param[in] KernelFunction      kernel function for classifier
  @param[in] SupportVectors      support vectors
  @param[in] SupportVectorLabels labels of support vectors (labels == 0 correspond to outliers, all other to inliers)
  @param[in] Lambdas             Lagrangian multipliers for each support vector
   */
  OneClassC(const KernelFunctionC &KernelFunction,
            const SampleC<VectorC> &SupportVectors,
            const SampleC<UIntT> &SupportVectorLabels,
            const SampleC<RealT> &Lambdas,
            RealT Radius)
    : Classifier2C(*new OneClassBodyC(KernelFunction, SupportVectors,
                                      SupportVectorLabels, Lambdas, Radius))
    {}

  //! Constructor.
  /*!
  @param[in] KernelFunction      kernel function for classifier
  @param[in] SupportVectors      support vectors
  @param[in] SupportVectorLabels labels of support vectors (label = -1  correspond to outliers, label = 1 to inliers)
  @param[in] Lambdas             Lagrangian multipliers for each support vector
   */
  OneClassC(const KernelFunctionC &KernelFunction,
            const SampleC<VectorC> &SupportVectors,
            const char *SupportVectorLabels,
            const RealT *Lambdas,
            RealT Radius)
    : Classifier2C(*new OneClassBodyC(KernelFunction, SupportVectors,
                                      SupportVectorLabels, Lambdas, Radius))
    {}

  //! Load from stream.
  OneClassC(istream &Strm);

  //! Load from binary stream.
  OneClassC(BinIStreamC &Strm);

  //! Classify vector 'data' and return class (0 - outliers or 1 - inliers)
  UIntT Classify(const VectorC &Data) const
    { return Body().Classify(Data); }

  //! Classify vector 'data' and return class (0 - outliers or 1 - inliers)
  //UIntT Classify(const RealT* Data) const
  //  { return Body().Classify(Data); }

  //! Classify vector 'data' and return value of descriminant function
  RealT Classify2(const VectorC &Data) const
  { return Body().Classify2(Data); }

  //! Classify vector 'data' and return value of descriminant function
  //RealT Classify2(const RealT* Data) const
  //  { return Body().Classify2(Data); }

  const KernelFunctionC& KernelFunction() const
    { return Body().KernelFunction(); }

  const SampleC<VectorC>& SupportVectors() const
    { return Body().SupportVectors(); }

  const RealT* Lambdas() const
    { return Body().Lambdas(); }

  //! Access to squared radius
  double& Radius()
    { return Body().Radius(); }

  //! Access to squared radius
  double Radius() const
    { return Body().Radius(); }

protected:
  //! Body constructor.
  OneClassC(OneClassBodyC &Bod) : Classifier2C(Bod)
    {}

  //! Access body.
  OneClassBodyC &Body()
    { return static_cast<OneClassBodyC &>(Classifier2C::Body()); }

  //! Access body.
  const OneClassBodyC &Body() const
    { return static_cast<const OneClassBodyC &>(Classifier2C::Body()); }
};

//---------------------------------------------------------------------------
//: Stream load operators defined in RAVL_INITVIRTUALCONSTRUCTOR_FULL macro
// Implementation of 'load from stream' constructors defined there as well
//inline ostream &operator<<(ostream &out, OneClassC &obj)
//inline BinOStreamC &operator<<(BinOStreamC &out, OneClassC &obj)

// Stream read operators are defined in RCHandleVC
//and call virtual Save function of body class
//inline ostream &operator<<(ostream &out, const OneClassC &obj)
//inline BinOStreamC &operator<<(BinOStreamC &out,const OneClassC &obj)

} //end of namespace RavlN
#endif
