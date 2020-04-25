// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
//! lib=RavlSVM

#include "Ravl/PatternRec/OneClass.hh"

namespace RavlN
{
using namespace RavlN;

//---------------------------------------------------------------------------
// Creates empty classifier, which returns 0 as the value ofdiscriminant function
OneClassBodyC::OneClassBodyC()
{
  lambdas = NULL;
  radius = 0.;
  sum = 0.;
  numSv = 0;
}
//---------------------------------------------------------------------------
// Constructor.
OneClassBodyC::OneClassBodyC(const KernelFunctionC &KernelFunction,
                             const SampleC<VectorC> &SupportVectors,
                             const SampleC<UIntT> &SupportVectorLabels,
                             const SampleC<RealT> &Lambdas,
                             RealT Radius)
{
  lambdas = NULL;
  numSv = SupportVectors.Size();
  if(numSv != SupportVectorLabels.Size() || numSv != Lambdas.Size())
    throw ExceptionC("OneClassBodyC constructor: "
                      "parameters must have the same number of elements");

  lambdas = new RealT[numSv];

  if(lambdas == NULL)
    throw ExceptionC("Can't allocate memory for internal buffers");

  // copy labels and lambdas
  for(UIntT i = 0; i < numSv; i++)
  {
    lambdas[i] = Lambdas[i] * SupportVectorLabels[i] == 0 ? -1 : 1;
  }
  supportVectors = SupportVectors;
  kernelFunction = KernelFunction;
  CalculateRadius(sum);
  radius = Radius;
}
//---------------------------------------------------------------------------
// Constructor.
OneClassBodyC::OneClassBodyC(const KernelFunctionC &KernelFunction,
                             const SampleC<VectorC> &SupportVectors,
                             const char *SupportVectorLabels,
                             const RealT *Lambdas,
                             RealT Radius)
{
  supportVectors = SupportVectors;
  numSv = SupportVectors.Size();

  lambdas = new RealT[numSv];

  if(lambdas == NULL)
    throw ExceptionC("Can't allocate memory for internal buffers");

  // copy labels and lambdas
  for(UIntT i = 0; i < numSv; i++)
  {
    lambdas[i] = Lambdas[i] * SupportVectorLabels[i];
  }

  kernelFunction = KernelFunction;
  CalculateRadius(sum);
  radius = Radius;
}
//---------------------------------------------------------------------------
// Load from stream.
OneClassBodyC::OneClassBodyC(istream &Strm)
              :Classifier2BodyC(Strm)
{
  lambdas = NULL;
  IntT version;
  Strm >> version;
  if(version != 0)
    throw ExceptionC("OneClassBodyC::Load:Unrecognised version number in stream.");

  Strm >> kernelFunction;
  Strm >> supportVectors;
  Strm >> numSv;
  if(numSv != supportVectors.Size())
    throw ExceptionC("OneClassBodyC::Load:numSv != supportVectors.Size().");

  lambdas = new RealT[numSv];

  if(lambdas == NULL)
    throw ExceptionC("OneClassBodyC::Load:"
                     "can't allocate memory for internal buffers");

  // read labels and lambdas
  for(UIntT i = 0; i < numSv; i++)
  {
    Strm >> lambdas[i];
  }

  Strm >> sum;
  Strm >> radius;
}
//---------------------------------------------------------------------------
// Load from binary stream.
OneClassBodyC::OneClassBodyC(BinIStreamC &Strm)
              :Classifier2BodyC(Strm)
{
  lambdas = NULL;
  IntT version;
  Strm >> version;
  if(version != 0)
    throw ExceptionC("OneClassBodyC::Load:"
                     "Unrecognised version number in stream.");

  Strm >> kernelFunction;
  Strm >> supportVectors;
  Strm >> numSv;
  if(numSv != supportVectors.Size())
    throw ExceptionC("OneClassBodyC::Load:"
                     "numSv != supportVectors.Size().");

  lambdas = new RealT[numSv];

  if(lambdas == NULL)
    throw ExceptionC("OneClassBodyC::Load:"
                     "can't allocate memory for internal buffers");

  // read labels and lambdas
  for(UIntT i = 0; i < numSv; i++)
  {
    Strm >> lambdas[i];
  }

  Strm >> sum;
  Strm >> radius;
}
//---------------------------------------------------------------------------
// Destructor.
OneClassBodyC::~OneClassBodyC()
{
  DestroyBuffers();
}
//---------------------------------------------------------------------------
void OneClassBodyC::DestroyBuffers()
{
  if(lambdas != NULL)
    delete[] lambdas;
  lambdas = NULL;
}
//---------------------------------------------------------------------------
// Writes object to stream
bool OneClassBodyC::Save(ostream &Out) const
{
  if(!Classifier2BodyC::Save(Out))
    throw ExceptionC("OneClassBodyC::Save:"
                     "error in ClassifierBodyC::Save call.");
  const IntT version = 0;
  Out << ' ' << version << ' ' << kernelFunction << ' ' << supportVectors
      << ' ' << numSv;
  for(SizeT i = 0; i < numSv; i++)
  {
    Out << '\n' << lambdas[i];
  }
  Out << '\n' << sum;
  Out << '\n' << radius;
  return true;
}
//---------------------------------------------------------------------------
// Writes object to stream, can be loaded using constructor
bool OneClassBodyC::Save(BinOStreamC &Out) const
{
  if(!Classifier2BodyC::Save(Out))
    throw ExceptionC("OneClassBodyC::Save:"
                     "error in ClassifierBodyC::Save call.");
  const IntT version = 0;
  Out << version << kernelFunction << supportVectors << numSv;
  for(SizeT i = 0; i < numSv; i++)
  {
    Out << lambdas[i];
  }
  Out << sum;
  Out << radius;
  return true;
}
//---------------------------------------------------------------------------
// Classifier vector 'Data' return value of descriminant function
RealT OneClassBodyC::Classify2(const VectorC &Data) const
{
  SizeT vecLen = supportVectors[0].Size();
  long double accum = radius - sum - kernelFunction.Apply(vecLen, Data.DataStart(), Data.DataStart());
  for(SizeT i = 0; i < numSv; i++)
  {
    const RealT lambda = lambdas[i];
    if(lambda != 0.)
    {
      const RealT *sv = supportVectors[i].ReferenceElm();
      accum += 2. * lambda * kernelFunction.Apply(vecLen, Data.DataStart(), sv);
    }
  }
  return (RealT)accum;
}
//---------------------------------------------------------------------------
//! Get vector length
IntT OneClassBodyC::GetDataSize() const
{
  return supportVectors[0].Size();
}
//---------------------------------------------------------------------------
//! Calculate radius
RealT OneClassBodyC::CalculateRadius(RealT &Sum) const
{
  if(numSv == 0)
    return 0;

  SizeT vecLen = supportVectors[0].Size();

  //calculate nested sum
  long double accum = 0.;
  for(SizeT i = 0; i < numSv; i++)
  {
    const RealT lambda1 = lambdas[i];
    if(lambda1 != 0.)
    {
      const RealT *sv1 = supportVectors[i].ReferenceElm();
      accum += lambda1 * lambda1 * kernelFunction.Apply(vecLen, sv1, sv1);

      for(SizeT j = i + 1; j < numSv; j++)
      {
        const RealT lambda2 = lambdas[j];
        if(lambda2 != 0.)
        {
          const RealT *sv2 = supportVectors[j].ReferenceElm();
          accum += 2. * lambda1 * lambda2 * kernelFunction.Apply(vecLen, sv1, sv2);
        }
      }
    }
  }
  Sum = (RealT)accum;

  //calculate radius
  const RealT *sv1 = supportVectors[0].ReferenceElm();
  accum += kernelFunction.Apply(vecLen, sv1, sv1);
  for(SizeT i = 0; i < numSv; i++)
  {
    const RealT lambda2 = lambdas[i];
    if(lambda2 != 0.)
    {
      const RealT *sv2 = supportVectors[i].ReferenceElm();
      accum -= 2. * lambda2 * kernelFunction.Apply(vecLen, sv1, sv2);
    }
  }

  return (RealT)accum;
}
//---------------------------------------------------------------------------
}
