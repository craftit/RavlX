// $Id: OneClassLinear.hh 12208 2008-02-07 12:54:07Z alex $
// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
//! lib=RavlSVM

//! author="Alexey Kostin"

#ifndef RAVL_ONE_CLASS_LINEAR_HEADER
#define RAVL_ONE_CLASS_LINEAR_HEADER

#include "Ravl/PatternRec/OneClass.hh"
#include "Ravl/PatternRec/CommonKernels.hh"

namespace RavlN
{
using namespace RavlN;

//! One class linear classifier (body part)
class OneClassLinearBodyC : public Classifier2BodyC
{
public:
  //! Default constructor.
  //! Creates empty classifier
  OneClassLinearBodyC();

  //! Load from stream.
  OneClassLinearBodyC(istream &Strm);

  //! Load from binary stream.
  OneClassLinearBodyC(BinIStreamC &Strm);

  //! Destructor
  virtual ~OneClassLinearBodyC();

  //! Writes object to stream
  virtual bool Save(ostream &Out) const;

  //! Writes object to stream
  virtual bool Save(BinOStreamC &Out) const;

  //! Classify vector 'data' and return the most likely label (0 or 1)
  virtual UIntT Classify(const VectorC &Data) const;

  //! Classify vector 'data' and return the most likely label (0 or 1)
  virtual UIntT Classify(const RealT* Data) const;

  //! Classify vector 'data' and return value of descriminant function
  RealT Classify2(const VectorC &Data) const;

  //! Classify vector 'data' and return value of descriminant function
  RealT Classify2(const RealT* Data) const;

  //! Get vector length of classifier
  IntT GetDataSize() const;

  //! Get radius
  RealT& Radius()
    { return m_radius; }

  //! Get radius
  RealT Radius() const
    { return m_radius; }

  //! Get centre vector
  SArray1dC<RealT> GetCentreVector() const
    { return SArray1dC<RealT>(BufferC<RealT>(m_vecLength, m_centre, true, true),
                              m_vecLength); }

  //! Create classifier
  /**
  @param SupportVectors        support vectors
  @param SupportVectorLabels   support vectors labels
  @param Lambdas               Lagrangian multipliers
  @param Scale                 global scale from kernel function
  @param Radius                radius
   */
  void Create(const SampleC<VectorC> &SupportVectors,
              const SByteT *SupportVectorLabels, const RealT *Lambdas,
              RealT Scale, RealT Radius);

  void Create(const OneClassC& OneClass)
    { Create(OneClass.SupportVectors(), NULL, OneClass.Lambdas(),
             static_cast<const LinearKernelC *>(&(OneClass.KernelFunction()))->Scale(),
             OneClass.Radius()); }

private:
  void DestroyBuffers();              //!< free allocated memory
  void CreateBuffers(int VecLength);  //!< allocate memory for weights
  void InitMembers();                 //!< initialise member variables
  RealT m_radius;                     //!< radius
  RealT* m_centre;                    //!< centre vector
  int m_vecLength;                    //!< length of the center vector
};
//---------------------------------------------------------------------------
//! One class linear classifier
class OneClassLinearC : public Classifier2C
{
public:
  //! Default constructor creates empty classifier
  OneClassLinearC()
    : Classifier2C(*new OneClassLinearBodyC())
    {}

  //! Load from stream.
  OneClassLinearC(istream &Strm);

  //! Load from binary stream.
  OneClassLinearC(BinIStreamC &Strm);

  //! Classify vector 'data' and return the most likely label (0 - negatives or 1 - positives)
  //! Size of data must be equal to size of support vector.
  UIntT Classify(const VectorC &Data) const
    { return Body().Classify(Data); }

  //! Classify vector 'data' and return the most likely label (0 - negatives or 1 - positives)
  //! Size of data must be equal to size of support vector.
  UIntT Classify(const RealT* Data) const
    { return Body().Classify(Data); }

  //! Classify vector 'data' and return value of descriminant function
  //! Size of data must be equal to size of support vector.
  RealT Classify2(const VectorC &Data) const
    { return Body().Classify2(Data); }

  //! Classify vector 'data' and return value of descriminant function
  //! Size of data must be equal to size of support vector.
  RealT Classify2(const RealT* Data) const
    { return Body().Classify2(Data); }

  //! Get vector length of classifier
  IntT GetDataSize() const
    { return Body().GetDataSize(); }

  //! Get radius
  RealT& Radius()
    { return Body().Radius(); }

  //! Get radius
  RealT Radius() const
    { return Body().Radius(); }

  //! Get weights
  SArray1dC<RealT> GetCentreVector() const
    { return Body().GetCentreVector(); }

  //! Create classifier
  /**
  @param SupportVectors        support vectors
  @param SupportVectorLabels   support vectors labels
  @param Lambdas               lagrangian multipliers
  @param Scale                 global scale from kernel function
  @param Radius                radius
   */
  void Create(const SampleC<VectorC> &SupportVectors,
              const SByteT* SupportVectorLabels, const RealT* Lambdas,
              RealT Scale, RealT Radius)
    { Body().Create(SupportVectors, SupportVectorLabels, Lambdas, Scale, Radius); }

  void Create(const OneClassC& OneClass)
    { Body().Create(OneClass); }

protected:
  //! Body constructor.
  OneClassLinearC(OneClassLinearBodyC &Bod) : Classifier2C(Bod)
    {}

  //! Access body.
  OneClassLinearBodyC &Body()
    { return static_cast<OneClassLinearBodyC &>(Classifier2C::Body()); }

  //! Access body.
  const OneClassLinearBodyC &Body() const
    { return static_cast<const OneClassLinearBodyC &>(Classifier2C::Body()); }
};

} //end of namespace RavlN
#endif
