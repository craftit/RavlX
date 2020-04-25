// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
//! lib=RavlSVM

//! author="Alexey Kostin"

#include "Ravl/PatternRec/OneClassLinear.hh"
#include "Ravl/PatternRec/CommonKernels.hh"

#if RAVL_OS_MACOSX
#include <alloca.h>
#endif 

namespace RavlN
{
using namespace RavlN;

//---------------------------------------------------------------------------
//! Default constructor.
//! Creates empty classifier
OneClassLinearBodyC::OneClassLinearBodyC()
{
  InitMembers();
}
//---------------------------------------------------------------------------
//! Load from stream.
OneClassLinearBodyC::OneClassLinearBodyC(istream &Strm)
                    :Classifier2BodyC(Strm)
{
  InitMembers();

  IntT version;
  Strm >> version;
  if(version != 0)
    throw ExceptionC("OneClassLinearBodyC::Load:"
                     "Unrecognised version number in stream.");

  int vecLength;
  Strm >> vecLength;

  CreateBuffers(vecLength);

  // read weights
  for(int i = 0; i < m_vecLength; i++)
  {
    Strm >> m_centre[i];
  }

  Strm >> m_radius;
}
//---------------------------------------------------------------------------
//! Load from binary stream.
OneClassLinearBodyC::OneClassLinearBodyC(BinIStreamC &Strm)
                    :Classifier2BodyC(Strm)
{
  InitMembers();

  IntT version;
  Strm >> version;
  if(version != 0)
    throw ExceptionC("OneClassLinearBodyC::Load:"
                     "Unrecognised version number in stream.");

  int vecLength;
  Strm >> vecLength;

  CreateBuffers(vecLength);

  // read weights
  for(int i = 0; i < m_vecLength; i++)
  {
    Strm >> m_centre[i];
  }

  Strm >> m_radius;
}
//---------------------------------------------------------------------------
// Destructor.
OneClassLinearBodyC::~OneClassLinearBodyC()
{
  DestroyBuffers();
}
//---------------------------------------------------------------------------
void OneClassLinearBodyC::DestroyBuffers()
{
  if(m_centre != NULL)
    delete[] m_centre;
  m_centre = NULL;
  m_vecLength = 0;
}
//---------------------------------------------------------------------------
//! allocate memory for weights
void OneClassLinearBodyC::CreateBuffers(int VecLength)
{
  if(m_vecLength != VecLength)
  {
    DestroyBuffers();
    m_centre = new RealT[VecLength];
    if(m_centre == NULL)
      throw ExceptionC("OneClassLinearBodyC::CreateBuffers:"
                       "Can't allocate memory for the centre vector");
    m_vecLength = VecLength;
  }
}
//---------------------------------------------------------------------------
//! initialise member variables
void OneClassLinearBodyC::InitMembers()
{
  m_radius = 0.;
  m_centre = NULL;
  m_vecLength = 0;
}
//---------------------------------------------------------------------------
// Writes object to stream
bool OneClassLinearBodyC::Save(ostream &Out) const
{
  if(!ClassifierBodyC::Save(Out))
    throw ExceptionC("OneClassLinearBodyC::Save:"
                     "error in ClassifierBodyC::Save call.");
  const IntT version = 0;
  Out << ' ' << version << ' ' << m_vecLength;
  for(int i = 0; i < m_vecLength; i++)
  {
    Out << '\n' << m_centre[i];
  }
  Out << '\n' << m_radius;
  return true;
}
//---------------------------------------------------------------------------
// Writes object to stream, can be loaded using constructor
bool OneClassLinearBodyC::Save(BinOStreamC &Out) const
{
  if(!ClassifierBodyC::Save(Out))
    throw ExceptionC("OneClassLinearBodyC::Save:"
                     "error in ClassifierBodyC::Save call.");
  const IntT version = 0;
  Out << version << m_vecLength;
  for(int i = 0; i < m_vecLength; i++)
  {
    Out << m_centre[i];
  }
  Out << m_radius;
  return true;
}
//---------------------------------------------------------------------------
// Classifier vector 'data' return the most likely label.
UIntT OneClassLinearBodyC::Classify(const VectorC &Data) const
{
  return Classify2(Data) > 0;
}
//---------------------------------------------------------------------------
// Classifier vector 'data' return the most likely label.
UIntT OneClassLinearBodyC::Classify(const RealT* Data) const
{
  return Classify2(Data) > 0;
}
//---------------------------------------------------------------------------
// Classifier vector 'Data' return value of descriminant function
RealT OneClassLinearBodyC::Classify2(const VectorC &Data) const
{
  return Classify2(Data.DataStart());
}
//---------------------------------------------------------------------------
// Classifier vector 'Data' return value of descriminant function
RealT OneClassLinearBodyC::Classify2(const RealT* Data) const
{
  long double retVal = m_radius;
  const RealT* wPtr = m_centre;
  const RealT* const ewPtr = m_centre + m_vecLength;
  const RealT* dPtr = Data;
  while(wPtr < ewPtr)
  {
    const long double val = (long double)(*wPtr++) - (long double)(*dPtr++);
    retVal -= val * val;
  }
  return retVal;
}
//---------------------------------------------------------------------------
// Get vector length of classifier
IntT OneClassLinearBodyC::GetDataSize() const
{
  return m_vecLength;
}
//---------------------------------------------------------------------------
//! Create classifier
/**
@param SupportVectors        support vectors
@param SupportVectorLabels   support vectors labels
@param Lambdas               lagrangian multipliers
@param Scale                 global scale from kernel function
@param Radius                radius
 */
void OneClassLinearBodyC::Create(const SampleC<VectorC> &SupportVectors,
                                 const SByteT *SupportVectorLabels,
                                 const RealT *Lambdas, RealT Scale, RealT Radius)
{
  const int numSv = SupportVectors.Size();

  if(numSv <= 0)
    throw ExceptionC("OneClassLinearBodyC::Create:No support vectors!");

  CreateBuffers(SupportVectors[0].Size());

  long double* t_centre = (long double *)alloca(m_vecLength * sizeof(long double));

  for(int i = 0; i < m_vecLength; t_centre[i++] = 0.);

  for(int i = 0; i < numSv; i++)
  {
    RealT lambda = Lambdas[i];
    if(SupportVectorLabels != NULL)
      lambda *= SupportVectorLabels[i];
    const RealT* imPtr = SupportVectors[i].DataStart();
    long double* wPtr = t_centre;
    const long double* const ewPtr = t_centre + m_vecLength;
    while(wPtr < ewPtr)
    {
      *wPtr++ += lambda * *imPtr++;
    }
  }

  for(int i = 0; i < m_vecLength; i++)
  {
    m_centre[i] = t_centre[i] * Scale;
  }

  m_radius = Radius;
}
//---------------------------------------------------------------------------
}
