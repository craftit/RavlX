// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
//! lib=RavlSVM

//! author="Alexey Kostin"

#include "Ravl/PatternRec/SvmLinearClassifier.hh"
#include "Ravl/PatternRec/SvmClassifier.hh"

namespace RavlN
{
using namespace RavlN;

//---------------------------------------------------------------------------
//! Default constructor.
//! Creates empty classifier
SvmLinearClassifierBodyC::SvmLinearClassifierBodyC()
{
  InitMembers();
}
//---------------------------------------------------------------------------
//! Load from stream.
SvmLinearClassifierBodyC::SvmLinearClassifierBodyC(istream &Strm)
                         :Classifier2BodyC(Strm)
{
  InitMembers();

  IntT version;
  Strm >> version;
  if(version != 0)
    throw ExceptionOperationFailedC("SvmLinearClassifierBodyC::Load:"
                                    "Unrecognised version number in stream.");

  int numWeights;
  Strm >> numWeights;

  CreateBuffers(numWeights);

  // read weights
  for(SizeT i = 0; i < m_weights.Size(); i++)
  {
    Strm >> m_weights[i];
  }

  Strm >> m_threshold;
}
//---------------------------------------------------------------------------
//! Load from binary stream.
SvmLinearClassifierBodyC::SvmLinearClassifierBodyC(BinIStreamC &Strm)
                         :Classifier2BodyC(Strm)
{
  InitMembers();

  IntT version;
  Strm >> version;
  if(version != 0)
    throw ExceptionOperationFailedC("SvmLinearClassifierBodyC::Load:"
                                    "Unrecognised version number in stream.");

  SizeT numWeights;
  Strm >> numWeights;

  CreateBuffers(numWeights);

  // read weights
  for(SizeT i = 0; i < m_weights.Size(); i++)
  {
    Strm >> m_weights[i];
  }

  Strm >> m_threshold;
}
//---------------------------------------------------------------------------
// Destructor.
SvmLinearClassifierBodyC::~SvmLinearClassifierBodyC()
{
  DestroyBuffers();
}
//---------------------------------------------------------------------------
void SvmLinearClassifierBodyC::DestroyBuffers()
{
  m_weights = VectorC();
}
//---------------------------------------------------------------------------
//! allocate memory for weights
void SvmLinearClassifierBodyC::CreateBuffers(SizeT NumWeights)
{
  if(m_weights.Size() != NumWeights)
  {
    DestroyBuffers();
    m_weights = VectorC::ConstructAligned(NumWeights, 16);
    if(!m_weights.IsValid())
      throw ExceptionOperationFailedC("SvmLinearClassifierBodyC::CreateBuffers:"
                                      "Can't allocate memory for weights");
  }
}
//---------------------------------------------------------------------------
//! initialise member variables
void SvmLinearClassifierBodyC::InitMembers()
{
  m_threshold = 0.;
  m_weights = VectorC();
}
//---------------------------------------------------------------------------
// Writes object to stream
bool SvmLinearClassifierBodyC::Save(ostream &Out) const
{
  if(!ClassifierBodyC::Save(Out))
    throw ExceptionOperationFailedC("SvmLinearClassifierBodyC::Save:"
                                    "error in ClassifierBodyC::Save call.");
  const IntT version = 0;
  Out << ' ' << version << ' ' << m_weights.Size();
  for(SizeT i = 0; i < m_weights.Size(); i++)
  {
    Out << '\n' << m_weights[i];
  }
  Out << '\n' << m_threshold;
  return true;
}
//---------------------------------------------------------------------------
// Writes object to stream, can be loaded using constructor
bool SvmLinearClassifierBodyC::Save(BinOStreamC &Out) const
{
  if(!ClassifierBodyC::Save(Out))
    throw ExceptionOperationFailedC("SvmLinearClassifierBodyC::Save:"
                                    "error in ClassifierBodyC::Save call.");
  const IntT version = 0;
  Out << version << m_weights.Size();
  for(SizeT i = 0; i < m_weights.Size(); i++)
  {
    Out << m_weights[i];
  }
  Out << m_threshold;
  return true;
}
//---------------------------------------------------------------------------
// Classifier vector 'Data' return value of descriminant function
RealT SvmLinearClassifierBodyC::Classify2(const VectorC &data) const
{
  RealT retVal = m_threshold + m_weights.Dot(data);
  return retVal;
}
//---------------------------------------------------------------------------
// Get vector length of classifier
IntT SvmLinearClassifierBodyC::GetDataSize() const
{
  return m_weights.Size();
}
//---------------------------------------------------------------------------
//! Create classifier
/**
@param Sv support vectors
@param Lambdas Lagrangian multipliers
@param Scale global scale from kernel function
@param Threshold threshold
  */
void SvmLinearClassifierBodyC::Create(const SampleC<VectorC>& Sv,
                                      const RealT* Lambdas,
                                      RealT Scale, RealT Threshold)
{
  const int numSv = Sv.Size();

  if(numSv <= 0)
    throw ExceptionOperationFailedC("SvmLinearClassifierBodyC::Create:"
                                    "No support vectors!");

  CreateBuffers(Sv[0].Size());

  m_weights.Fill(0.);

  for(int i = 0; i < numSv; i++)
  {
    RealT lambda = Lambdas[i];
    const RealT* imPtr = Sv[i].DataStart();
    RealT* wPtr = m_weights.DataStart();
    const RealT* const ewPtr = wPtr + m_weights.Size();
    while(wPtr < ewPtr)
    {
      *wPtr++ += lambda * *imPtr++;
    }
  }

  m_weights *= Scale;

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
void SvmLinearClassifierBodyC::Create(const SampleC<VectorC>& Sv,
                                      const RealT* Lambdas,
                                      const RealT* Weights,
                                      RealT Scale, RealT Threshold)
{
  Create(Sv, Lambdas, Scale, Threshold);
  for(SizeT j = 0; j < m_weights.Size(); j++)
  {
    m_weights[j] *= Weights[j];
  }
}
//---------------------------------------------------------------------------
}
