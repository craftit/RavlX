// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
//! lib=RavlSVM

//! author="Alexey Kostin"

#include "Ravl/PatternRec/SvmClassifier.hh"
#include "Ravl/PatternRec/SampleIter.hh"

typedef double *DoublePtrT;

namespace RavlN
{
using namespace RavlN;

//---------------------------------------------------------------------------
// Creates empty classifier, which returns 0 as the value of descriminant function
SvmClassifierBodyC::SvmClassifierBodyC()
{
  lambdas = NULL;
  threshold = 0.;
  numSv = 0;
  trSetVectorPtrs = NULL;
  numFeatures = 0;
}
//---------------------------------------------------------------------------
// Constructor.
SvmClassifierBodyC::SvmClassifierBodyC(const KernelFunctionC &KernelFunction,
                                       const SampleC<VectorC> &SupportVectors,
                                       const SampleC<UIntT> &SupportVectorLabels,
                                       const SampleC<RealT> &Lambdas,
                                       RealT Threshold)
{
  lambdas = NULL;
  trSetVectorPtrs = NULL;
  numSv = SupportVectors.Size();
  if(numSv != SupportVectorLabels.Size() || numSv != Lambdas.Size())
    throw ExceptionOperationFailedC("SvmClassifierBodyC constructor: "
                                    "parameters should have the same size");

  lambdas = new RealT[numSv];
  trSetVectorPtrs = new DoublePtrT[numSv];

  if(lambdas == NULL || trSetVectorPtrs == NULL)
    throw ExceptionOperationFailedC("Can't allocate memory for internal svm"
                                    " classifier buffers");

  numFeatures = SupportVectors[0].Size();

  // copy labels, lambdas and support vectors
  for(UIntT i = 0; i < numSv; i++)
  {
    lambdas[i] = Lambdas[i] * SupportVectorLabels[i] == 0 ? -1 : 1;
    trSetVectorPtrs[i] = SupportVectors[i].ReferenceElm();
  }
  supportVectors = SupportVectors;
  threshold = Threshold;
  kernelFunction = KernelFunction;
}
//---------------------------------------------------------------------------
// Constructor.
SvmClassifierBodyC::SvmClassifierBodyC(const KernelFunctionC &KernelFunction,
                                       const SampleC<VectorC> &SupportVectors,
                                       const char *SupportVectorLabels,
                                       const RealT *Lambdas, RealT Threshold)
{
  lambdas = NULL;
  trSetVectorPtrs = NULL;
  numSv = SupportVectors.Size();
  if(numSv < 1)
    throw ExceptionOperationFailedC("Number of support vectors is zero. "
                                    "Can't create SVM classifier without support vectors");

  lambdas = new RealT[numSv];
  trSetVectorPtrs = new DoublePtrT[numSv];

  if(lambdas == NULL || trSetVectorPtrs == NULL)
    throw ExceptionOperationFailedC("Can't allocate memory for internal svm"
                                    " classifier buffers");

  numFeatures = SupportVectors[0].Size();

  // copy labels, lambdas and support vectors
  for(UIntT i = 0; i < numSv; i++)
  {
    lambdas[i] = Lambdas[i] * SupportVectorLabels[i];
    trSetVectorPtrs[i] = SupportVectors[i].ReferenceElm();
  }
  supportVectors = SupportVectors;
  threshold = Threshold;
  kernelFunction = KernelFunction;
}
//---------------------------------------------------------------------------
// Load from stream.
SvmClassifierBodyC::SvmClassifierBodyC(istream &Strm)
                   :Classifier2BodyC(Strm)
{
  lambdas = NULL;
  IntT version;
  Strm >> version;
  if(version != 0)
    throw ExceptionOperationFailedC("SvmClassifierBodyC::Load:"
                                    "Unrecognised version number in stream.");

  Strm >> kernelFunction;
  Strm >> supportVectors;
  Strm >> numSv;
  if(numSv != supportVectors.Size())
    throw ExceptionOperationFailedC("SvmClassifierBodyC::Load:"
                                    "numSv != supportVectors.Size().");

  numFeatures = supportVectors[0].Size();

  lambdas = new RealT[numSv];
  trSetVectorPtrs = new DoublePtrT[numSv];

  if(lambdas == NULL || trSetVectorPtrs == NULL)
    throw ExceptionOperationFailedC("SvmClassifierBodyC::Load:"
                                    "can't allocate memory for internal buffers");

  // read labels and lambdas
  for(UIntT i = 0; i < numSv; i++)
  {
    Strm >> lambdas[i];
  }

  Strm >> threshold;

  for(UIntT i = 0; i < numSv; i++)
  {
    trSetVectorPtrs[i] = supportVectors[i].ReferenceElm();
  }
}
//---------------------------------------------------------------------------
// Load from binary stream.
SvmClassifierBodyC::SvmClassifierBodyC(BinIStreamC &Strm)
                   :Classifier2BodyC(Strm)
{
  lambdas = NULL;
  IntT version;
  Strm >> version;
  if(version != 0)
    throw ExceptionOperationFailedC("SvmClassifierBodyC::Load:"
                                    "Unrecognised version number in stream.");

  Strm >> kernelFunction;
  Strm >> supportVectors;
  Strm >> numSv;
  if(numSv != supportVectors.Size())
    throw ExceptionOperationFailedC("SvmClassifierBodyC::Load:"
                                    "numSv != supportVectors.Size().");

  numFeatures = supportVectors[0].Size();

  lambdas = new RealT[numSv];
  trSetVectorPtrs = new DoublePtrT[numSv];

  if(lambdas == NULL || trSetVectorPtrs == NULL)
    throw ExceptionOperationFailedC("SvmClassifierBodyC::Load:"
                                    "can't allocate memory for internal buffers");

  // read labels and lambdas
  for(UIntT i = 0; i < numSv; i++)
  {
    Strm >> lambdas[i];
  }

  Strm >> threshold;

  for(UIntT i = 0; i < numSv; i++)
  {
    trSetVectorPtrs[i] = supportVectors[i].ReferenceElm();
  }
}
//---------------------------------------------------------------------------
// Destructor.
SvmClassifierBodyC::~SvmClassifierBodyC()
{
  DestroyBuffers();
}
//---------------------------------------------------------------------------
void SvmClassifierBodyC::DestroyBuffers()
{
  if(lambdas != NULL)
    delete[] lambdas;
  lambdas = NULL;

  if(trSetVectorPtrs != NULL)
    delete[] trSetVectorPtrs;
  trSetVectorPtrs = NULL;
}
//---------------------------------------------------------------------------
// Writes object to stream
bool SvmClassifierBodyC::Save(ostream &Out) const
{
  if(!Classifier2BodyC::Save(Out))
    throw ExceptionOperationFailedC("SvmClassifierBodyC::Save:"
                                    "error in ClassifierBodyC::Save call.");
  const IntT version = 0;
  Out << ' ' << version << ' ' << kernelFunction << ' ' << supportVectors
      << ' ' << numSv;
  for(SizeT i = 0; i < numSv; i++)
  {
    Out << '\n' << lambdas[i];
  }
  Out << '\n' << threshold;
  return true;
}
//---------------------------------------------------------------------------
// Writes object to stream, can be loaded using constructor
bool SvmClassifierBodyC::Save(BinOStreamC &Out) const
{
  if(!Classifier2BodyC::Save(Out))
    throw ExceptionOperationFailedC("SvmClassifierBodyC::Save:"
                                    "error in ClassifierBodyC::Save call.");
  const IntT version = 0;
  Out << version << kernelFunction << supportVectors << numSv;
  for(SizeT i = 0; i < numSv; i++)
  {
    Out << lambdas[i];
  }
  Out << threshold;
  return true;
}

VectorC SvmClassifierBodyC::Apply(const VectorC &data) const {
  // scale using sigmoid
  RealT s = Classify2(data);
  RealT sig = 1.0 / (1.0 + Exp(-0.25 * s)); // A sigmoid but with less slope
  return VectorC(1-sig, sig); // label 0 is negative in SVM...
}

//---------------------------------------------------------------------------
// Classifier vector 'Data' return value of descriminant function
RealT SvmClassifierBodyC::Classify2(const VectorC &Data) const
{
  RealT retVal = threshold;
  for(UIntT j = 0; j < numSv; j++)
  {
    retVal += lambdas[j] *
              kernelFunction.Apply(numFeatures, trSetVectorPtrs[j], Data.DataStart());
  }
  return retVal;
}
//---------------------------------------------------------------------------
// Get vector length of classifier
IntT SvmClassifierBodyC::GetDataSize() const
{
  return numFeatures;
}
//---------------------------------------------------------------------------
}
