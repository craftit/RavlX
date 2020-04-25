// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
//! lib=RavlSVM

//! author="Alexey Kostin"

#include <Ravl/config.h>

#if !RAVL_OS_FREEBSD
#define _XOPEN_SOURCE 600
#endif


#include "Ravl/PatternRec/SvmQuadraticClassifier.hh"
#include "Ravl/PatternRec/SvmClassifier.hh"

namespace RavlN
{
using namespace RavlN;


//---------------------------------------------------------------------------
//! Default constructor
//! Creates empty classifier
SvmQuadraticClassifierBodyC::SvmQuadraticClassifierBodyC()
{
  InitMembers();
}

//---------------------------------------------------------------------------
//! Load from stream.
SvmQuadraticClassifierBodyC::SvmQuadraticClassifierBodyC(istream &Strm)
                           : Classifier2BodyC(Strm)
{
  InitMembers();

  IntT version;
  Strm >> version;
  if(version != 0)
    throw ExceptionOperationFailedC("SvmQuadraticClassifierBodyC::Load:"
                                    "Unrecognised version number in stream.");

  int numWeights;
  Strm >> numWeights;
  RavlAssert((numWeights & 1) == 0); // Should always be even!

  CreateBuffers(numWeights / 2);

  // read weights
  for(int i = 0; i < m_halfWeights; i++)
  {
    Strm >> m_weights1[i];
  }

  for(int i = 0; i < m_halfWeights; i++)
  {
    Strm >> m_weights2[i];
  }

  Strm >> m_threshold;
}
//---------------------------------------------------------------------------
//! Load from binary stream.
SvmQuadraticClassifierBodyC::SvmQuadraticClassifierBodyC(BinIStreamC &Strm)
                           :Classifier2BodyC(Strm)
{
  InitMembers();

  IntT version;
  Strm >> version;
  if(version != 0)
    throw ExceptionOperationFailedC("SvmQuadraticClassifierBodyC::Load:"
                                    "Unrecognised version number in stream.");

  int numWeights;
  Strm >> numWeights;
  RavlAssert((numWeights & 1) == 0); // Should always be even!

  CreateBuffers(numWeights / 2);

  // read weights
  for(int i = 0; i < m_halfWeights; i++)
    Strm >> m_weights1[i];

  for(int i = 0; i < m_halfWeights; i++)
    Strm >> m_weights2[i];

  Strm >> m_threshold;
}

//! Construct from raw components
SvmQuadraticClassifierBodyC::SvmQuadraticClassifierBodyC(const VectorC &w1,
                                                                 const VectorC &w2,
                                                                 RealT threshold)
 : m_threshold(threshold),
   m_weights1(w1),
   m_weights2(w2),
   m_halfWeights(w1.Size().V())
{
  RavlAssert(w1.Size() == w2.Size());
}

//---------------------------------------------------------------------------
//! Destructor.
SvmQuadraticClassifierBodyC::~SvmQuadraticClassifierBodyC()
{
  DestroyBuffers();
}
//---------------------------------------------------------------------------
//! free allocated memory
void SvmQuadraticClassifierBodyC::DestroyBuffers()
{
  m_weights1 = VectorC();
  m_weights2 = VectorC();
  m_halfWeights = 0;
}
//---------------------------------------------------------------------------
//! allocate memory for weights
void SvmQuadraticClassifierBodyC::CreateBuffers(int halfWeights)
{
  m_weights1 = VectorC::ConstructAligned(halfWeights, 16);
  m_weights2 = VectorC::ConstructAligned(halfWeights, 16);

  if(!m_weights1.IsValid() || !m_weights2.IsValid())
  {
    DestroyBuffers();
    throw ExceptionOperationFailedC("SvmQuadraticClassifierBodyC::CreateBuffers:"
                                    "Can't allocate memory for weights");
  }
  m_halfWeights = halfWeights;
}
//---------------------------------------------------------------------------
//! initialise member variables
void SvmQuadraticClassifierBodyC::InitMembers()
{
  m_weights1 = VectorC();
  m_weights2 = VectorC();
  m_halfWeights = 0;
  m_threshold = 0.;
}
//---------------------------------------------------------------------------
//! Writes object to stream
bool SvmQuadraticClassifierBodyC::Save(ostream &Out) const
{
  if(!ClassifierBodyC::Save(Out))
    throw ExceptionOperationFailedC("SvmQuadraticClassifierBodyC::Save:"
                                    "error in ClassifierBodyC::Save call.");
  const IntT version = 0;
  Out << ' ' << version << ' ' << m_halfWeights * 2;
  for(int i = 0; i < m_halfWeights; i++)
  {
    Out << '\n' << m_weights1[i];
  }
  for(int i = 0; i < m_halfWeights; i++)
  {
    Out << '\n' << m_weights2[i];
  }
  Out << '\n' << m_threshold;
  return true;
}
//---------------------------------------------------------------------------
//! Writes object to stream, can be loaded using constructor
bool SvmQuadraticClassifierBodyC::Save(BinOStreamC &Out) const
{
  if(!ClassifierBodyC::Save(Out))
    throw ExceptionOperationFailedC("SvmQuadraticClassifierBodyC::Save:"
                                    "error in ClassifierBodyC::Save call.");
  const IntT version = 0;
  Out << version << m_halfWeights * 2;
  for(int i = 0; i < m_halfWeights; i++)
  {
    Out << m_weights1[i];
  }
  for(int i = 0; i < m_halfWeights; i++)
  {
    Out << m_weights2[i];
  }
  Out << m_threshold;
  return true;
}
//---------------------------------------------------------------------------
//! Classifier vector 'Data' return value of descriminant function
RealT SvmQuadraticClassifierBodyC::Classify2(const VectorC & Data) const
{
  RealT retVal = m_threshold + Data.Dot2(m_weights1, m_weights2);
  return retVal;
}
//---------------------------------------------------------------------------
//! Get vector length of classifier
IntT SvmQuadraticClassifierBodyC::GetDataSize() const
{
  return m_halfWeights;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//! Create classifier
/**
@param Sv support vectors
@param Lambdas Lagrangian multipliers
@param Scale global scale from kernel function
@param Threshold threshold
 */
void SvmQuadraticClassifierBodyC::Create(const SampleC<VectorC>& Sv,
                                         const RealT* Lambdas,
                                         RealT Scale, RealT Threshold)
{
  const SizeT numSv = Sv.Size();
  if(numSv <= 0)
    throw ExceptionOperationFailedC("SvmQuadraticClassifierBodyC::Create:"
                                    "No support vectors!");
  CreateBuffers(Sv[0].Size());

  m_weights1.Fill(0.);
  m_weights2.Fill(0.);

  for(SizeT i = 0; i < numSv; i++)
  {
    RealT lambda = Lambdas[i];
    const RealT* imPtr = Sv[i].DataStart();
    RealT* wPtr = m_weights1.DataStart();
    RealT* w2Ptr = m_weights2.DataStart();
    const RealT* const wePtr = wPtr + m_halfWeights;
    while(wPtr < wePtr)
    {
      RealT val = *imPtr++;
      *wPtr++ += lambda * val;
      *w2Ptr++ += lambda * val * val;
    }
  }

  m_weights1 *= Scale;
  m_weights2 *= Scale;

  m_threshold = Threshold;
}

//---------------------------------------------------------------------------
//! Create classifier
/**
@param Sv support vectors
@param Lambdas Lagrangian multipliers
@param Weights weights for features
@param Scale global scale from kernel function
@param Threshold threshold
 */
void SvmQuadraticClassifierBodyC::Create(const SampleC<VectorC>& Sv,
                                         const RealT* Lambdas,
                                         const RealT* Weights,
                                         RealT Scale, RealT Threshold)
{
  Create(Sv, Lambdas, Scale, Threshold);
  for(int j = 0; j < m_halfWeights; j++)
  {
    const RealT w = Weights[j];
    m_weights1[j] *= w;
    m_weights2[j] *= w * w;
  }
}

//---------------------------------------------------------------------------
}
