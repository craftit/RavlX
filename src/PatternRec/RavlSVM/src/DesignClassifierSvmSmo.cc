// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
//! lib=RavlSVM

//! author="Alexey Kostin"

#include "Ravl/PatternRec/DesignClassifierSvmSmo.hh"
#include "Ravl/PatternRec/SampleIter.hh"
#include <string.h>
#include <stdio.h>
#include "Ravl/XMLFactoryRegister.hh"

namespace RavlN
{
using namespace RavlN;

typedef double *DoublePtrT;
#define DOUBLE_QNAN 999e100
#define IS_NAN(_X) ((_X) == DOUBLE_QNAN)
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void DesignSvmSmoBodyC::Init(const KernelFunctionC &KernelFunction,
                             RealT Penalty1, RealT Penalty2,
                             RealT Tolerance, RealT Eps,
                             RealT LambdaThreshold)
{
  kernelFunction = KernelFunction;
  c1 = Penalty1;
  c2 = Penalty2;
  tolerance = Tolerance;
  eps = Eps;
  lambdaThreshold = LambdaThreshold;
  trainSetSize = 0;

  lambdas = NULL;
  errorCache = NULL;
  trainingSetLabels = NULL;
  trSetVectorPtrs = NULL;
  objectsToUse = NULL;
  kernelCache = NULL;

  numFeatures = 0;

  numTrainObjects = 0;
  errorCacheSize = 0;
  kernelCacheSize = 0;

  callbackFunc = NULL;
  callbackData = NULL;

  verbosity = 0;
}
//---------------------------------------------------------------------------
// Default constructor.
DesignSvmSmoBodyC::DesignSvmSmoBodyC(const KernelFunctionC &KernelFunction,
                                     RealT Penalty1, RealT Penalty2,
                                     RealT Tolerance, RealT Eps,
                                     RealT LambdaThreshold)
  : DesignSvmBodyC(),
    verbosity(0)
{
  Init(KernelFunction, Penalty1, Penalty2, Tolerance, Eps, LambdaThreshold);
}


DesignSvmSmoBodyC::DesignSvmSmoBodyC(const XMLFactoryContextC & factory)
  : DesignSvmBodyC(factory),
    c1(factory.AttributeReal("penalty1", 1e3)),
    c2(factory.AttributeReal("penalty2", 1e3)),
    tolerance(factory.AttributeReal("tolerance", 1e-7)),
    eps(factory.AttributeReal("eps", 1e-9)),
    lambdaThreshold(factory.AttributeReal("lambdaThreshold", 1e-12)),
    verbosity(factory.AttributeBool("verbosity", 0))
        
{
  if(!factory.UseComponent("KernelFunction", kernelFunction))
    RavlIssueError("No kernel function specified in XML factory");

  // Lets get to a good initial state
  Reset();
}


//---------------------------------------------------------------------------
//: Load from stream.
DesignSvmSmoBodyC::DesignSvmSmoBodyC(istream &strm)
                  :DesignSvmBodyC(strm)
{
  IntT version;
  strm >> version;
  if(version != 0)
    throw ExceptionC("DesignSvmSmoBodyC::DesignSvmSmoBodyC(istream &strm):"
                     "Unrecognised version number in stream.");
  KernelFunctionC kFunction;
  strm >> kFunction;
  RealT _c1, _c2, _tolerance, _eps, _lambdaThreshold;
  strm >> _c1;
  strm >> _c2;
  strm >> _tolerance;
  strm >> _eps;
  strm >> _lambdaThreshold;

  Init(kFunction, _c1, _c2, _tolerance, _eps, _lambdaThreshold);
  verbosity = 0;
}
//---------------------------------------------------------------------------
//: Load from binary stream.
DesignSvmSmoBodyC::DesignSvmSmoBodyC(BinIStreamC &strm)
                  :DesignSvmBodyC(strm)
{
  IntT version;
  strm >> version;
  if(version != 0)
    throw ExceptionC("DesignSvmSmoBodyC::DesignSvmSmoBodyC(istream &strm):"
                     "Unrecognised version number in stream.");
  KernelFunctionC kFunction;
  strm >> kFunction;
  RealT _c1, _c2, _tolerance, _eps, _lambdaThreshold;
  strm >> _c1;
  strm >> _c2;
  strm >> _tolerance;
  strm >> _eps;
  strm >> _lambdaThreshold;

  Init(kFunction, _c1, _c2, _tolerance, _eps, _lambdaThreshold);
  verbosity = 0;
}
//---------------------------------------------------------------------------
//: Writes object to stream
bool DesignSvmSmoBodyC::Save(ostream &out) const
{
  if(!DesignSvmBodyC::Save(out))
    return false;
  const IntT version = 0;
  out << ' ' << version << '\n' <<  kernelFunction << '\n'
      << c1 << ' ' << c2 << '\n' << tolerance << ' ' << eps
      << '\n' << lambdaThreshold;
  return true;
}
//---------------------------------------------------------------------------
//: Writes object to stream
bool DesignSvmSmoBodyC::Save(BinOStreamC &out) const
{
  if(!DesignSvmBodyC::Save(out))
    return false;
  const IntT version = 0;
  out << version << kernelFunction << c1 << c2 << tolerance
      << eps << lambdaThreshold;
  return true;
}
//---------------------------------------------------------------------------
// Destructor.
DesignSvmSmoBodyC::~DesignSvmSmoBodyC()
{
  delete[] errorCache;
  delete[] lambdas;
  delete[] trainingSetLabels;
  delete[] trSetVectorPtrs;
  delete[] objectsToUse;
  delete[] kernelCache;
  errorCache = NULL;
  errorCacheSize = 0;
  trainSetSize = 0;
  numTrainObjects = 0;
  kernelCacheSize = 0;
}
//---------------------------------------------------------------------------
Classifier2C DesignSvmSmoBodyC::Apply(const SampleC<VectorC> &TrainingSetVectors,
                                      const SampleC<SByteT> &TrainingSetLabels,
                                      const SArray1dC<IndexC> &ObjectsToUse)
{
  Prepare(TrainingSetVectors, TrainingSetLabels);

  SetObjectsToUse(ObjectsToUse);

  //find Lagrangian multipliers
  CalcLambdas();

  return GetClassifier();
}
//---------------------------------------------------------------------------
ClassifierC DesignSvmSmoBodyC::Apply(const SampleC<VectorC> &TrainingSetVectors,
                                     const SampleC<UIntT> &TrainingSetLabels,
                                     const SArray1dC<IndexC> &ObjectsToUse)
{
  Prepare(TrainingSetVectors, TrainingSetLabels);

  SetObjectsToUse(ObjectsToUse);

  //find Lagrangian multipliers
  CalcLambdas();

  return GetClassifier();
}
//---------------------------------------------------------------------------
Classifier2C DesignSvmSmoBodyC::Apply(const SampleC<VectorC> &TrainingSetVectors,
                                      const SampleC<SByteT> &TrainingSetLabels)
{
  Prepare(TrainingSetVectors, TrainingSetLabels);

  if(objectsToUse == NULL || numTrainObjects != trainSetSize)
  {
    delete[] objectsToUse;
    objectsToUse = new int[trainSetSize];
    if(objectsToUse == NULL)
      throw ExceptionOperationFailedC("Can't allocate memory for internal svm buffers");
    numTrainObjects = trainSetSize;
  }

  //copy objectsToUse
  for(int i = 0; i < numTrainObjects; i++)
  {
    objectsToUse[i] = i;
  }

  //find Lagrangian multipliers
  CalcLambdas();

  return GetClassifier();
}
//---------------------------------------------------------------------------
ClassifierC DesignSvmSmoBodyC::Apply(const SampleC<VectorC> &TrainingSetVectors,
                                     const SampleC<UIntT> &TrainingSetLabels)
{
  Prepare(TrainingSetVectors, TrainingSetLabels);

  if(objectsToUse == NULL || numTrainObjects != trainSetSize)
  {
    delete[] objectsToUse;
    objectsToUse = new int[trainSetSize];
    if(objectsToUse == NULL)
      throw ExceptionOperationFailedC("Can't allocate memory for internal svm buffers");
    numTrainObjects = trainSetSize;
  }

  //copy objectsToUse
  for(int i = 0; i < numTrainObjects; i++)
  {
    objectsToUse[i] = i;
  }

  //find Lagrangian multipliers
  CalcLambdas();

  return GetClassifier();
}

// Reset designer to initial state
bool DesignSvmSmoBodyC::Reset() {
  
  trainSetSize = 0;

  lambdas = NULL;
  errorCache = NULL;
  trainingSetLabels = NULL;
  trSetVectorPtrs = NULL;
  objectsToUse = NULL;
  kernelCache = NULL;

  numFeatures = 0;

  numTrainObjects = 0;
  errorCacheSize = 0;
  kernelCacheSize = 0;

  callbackFunc = NULL;
  callbackData = NULL;


  return true;
}


//---------------------------------------------------------------------------
void DesignSvmSmoBodyC::Prepare(const SampleC<VectorC> &TrainingSetVectors,
                                const SampleC<SByteT> &TrainingSetLabels)
{
  trainSetVectors = TrainingSetVectors;
  trainSetSize = TrainingSetVectors.Size();
  numFeatures = TrainingSetVectors[0].Size();

  //additional actions
  if(errorCacheSize != trainSetSize || errorCache == NULL || lambdas == NULL
     || trainingSetLabels == NULL || trSetVectorPtrs == NULL)
  {
    delete[] errorCache;
    delete[] lambdas;
    delete[] trainingSetLabels;
    delete[] trSetVectorPtrs;
    errorCacheSize = 0;
    errorCache = new double[trainSetSize];
    lambdas = new double[trainSetSize];
    trainingSetLabels = new char[trainSetSize];
    trSetVectorPtrs = new DoublePtrT[trainSetSize];
    if(errorCache == NULL || lambdas == NULL || trainingSetLabels == NULL
       || trSetVectorPtrs == NULL)
      throw ExceptionC("Can't allocate memory for internal svm beffers");
    errorCacheSize = trainSetSize;
  }
  //clear lambdas array
  memset(lambdas, 0, sizeof(double) * trainSetSize);
  b = 0;

  //set initial values for lambdas
  //if known initial values for lambdas copy them
  if(initialLambdas.Size() == UIntT(trainSetSize))
  {
    double *lambdasPtr = lambdas;
    for(SampleIterC<RealT> it(initialLambdas); it; it++, lambdasPtr++)
    {
      RealT l = *it;
      if(l < 0) l = 0;
      *lambdasPtr = l;
    }
  }

  //copy trainingSetLabels and vectors
  SampleIterC<SByteT> itLab(TrainingSetLabels);
  SampleIterC<VectorC> itVec(TrainingSetVectors);
  int i = 0;
  for(; i < trainSetSize && itLab && itVec; i++, itLab++, itVec++)
  {
    trainingSetLabels[i] = *itLab;
    trSetVectorPtrs[i] = itVec->ReferenceElm();
  }
  if(i != trainSetSize)
    throw ExceptionC("Can't copy data to internal svm buffers");

  //create kernel cache
  SizeT newKernelCacheSize = (SizeT(trainSetSize + 1) * SizeT(trainSetSize)) / 2;
  if(kernelCache == NULL || kernelCacheSize != newKernelCacheSize)
  {
    delete[] kernelCache;
    kernelCacheSize = 0;
    kernelCache = new float[newKernelCacheSize];
    if(kernelCache == NULL)
      throw ExceptionOperationFailedC("Can't allocate memory for internal cache of svm kernel function");
    kernelCacheSize = newKernelCacheSize;
  }
  memset(kernelCache, 0, sizeof(float) * kernelCacheSize);
}
//---------------------------------------------------------------------------
void DesignSvmSmoBodyC::Prepare(const SampleC<VectorC> &TrainingSetVectors,
                                const SampleC<UIntT> &TrainingSetLabels)
{
  trainSetVectors = TrainingSetVectors;
  trainSetSize = TrainingSetVectors.Size();
  numFeatures = TrainingSetVectors[0].Size();

  //additional actions
  if(errorCacheSize != trainSetSize || errorCache == NULL || lambdas == NULL
     || trainingSetLabels == NULL || trSetVectorPtrs == NULL)
  {
    delete[] errorCache;
    delete[] lambdas;
    delete[] trainingSetLabels;
    delete[] trSetVectorPtrs;
    errorCacheSize = 0;
    errorCache = new double[trainSetSize];
    lambdas = new double[trainSetSize];
    trainingSetLabels = new char[trainSetSize];
    trSetVectorPtrs = new DoublePtrT[trainSetSize];
    if(errorCache == NULL || lambdas == NULL || trainingSetLabels == NULL
       || trSetVectorPtrs == NULL)
      throw ExceptionC("Can't allocate memory for internal svm beffers");
    errorCacheSize = trainSetSize;
  }

  //clear lambdas array
  memset(lambdas, 0, sizeof(double) * trainSetSize);
  b = 0;

  //if known initial values for lambdas copy them
  if(initialLambdas.Size() == UIntT(trainSetSize))
  {
    double *lambdasPtr = lambdas;
    for(SampleIterC<RealT> it(initialLambdas); it; it++, lambdasPtr++)
    {
      RealT l = *it;
      if(l < 0) l = 0;
      *lambdasPtr = l;
    }
  }

  //copy trainingSetLabels and vectors
  SampleIterC<UIntT> itLab(TrainingSetLabels);
  SampleIterC<VectorC> itVec(TrainingSetVectors);
  int i = 0;
  for(; i < trainSetSize && itLab && itVec; i++, itLab++, itVec++)
  {
    trainingSetLabels[i] = (*itLab == 0) ? -1 : 1;
    trSetVectorPtrs[i] = itVec->ReferenceElm();
  }
  if(i != trainSetSize)
    throw ExceptionC("Can't copy data to internal svm buffers");

  //create kernel cache
  SizeT newKernelCacheSize = ((trainSetSize + 1) * trainSetSize) / 2;
  if(kernelCache == NULL || kernelCacheSize != newKernelCacheSize)
  {
    delete[] kernelCache;
    kernelCacheSize = 0;
    kernelCache = new float[newKernelCacheSize];
    if(kernelCache == NULL)
      throw ExceptionOperationFailedC("Can't allocate memory for internal cache of svm kernel function");
    kernelCacheSize = newKernelCacheSize;
  }
  memset(kernelCache, 0, sizeof(float) * kernelCacheSize);
}
//---------------------------------------------------------------------------
void DesignSvmSmoBodyC::SetObjectsToUse(const SArray1dC<IndexC> &ObjectsToUse)
{
  if(objectsToUse == NULL || numTrainObjects != (int)(ObjectsToUse.Size()))
  {
    delete[] objectsToUse;
    objectsToUse = new int[ObjectsToUse.Size()];
    if(objectsToUse == NULL)
      throw ExceptionC("Can't allocate memory for internal svm beffers");
    numTrainObjects = ObjectsToUse.Size();
  }

  //copy objectsToUse
  for(int i = 0; i < numTrainObjects; i++)
  {
    objectsToUse[i] = ObjectsToUse[i].V();
  }
}
//---------------------------------------------------------------------------
SvmClassifierC DesignSvmSmoBodyC::GetClassifier() const
{
  //create classifier
  int numSV = NumSupportVectors();

  SampleC<VectorC> sv(numSV);
  char *svl = new char[numSV];
  RealT *lmbd = new RealT[numSV];
  if(svl == NULL || lmbd == NULL)
    throw ExceptionC("Can't allocate memory for internal svm buffers");

  int j = 0;
  for(int i = 0; i < numTrainObjects; i++)
  {
    const int objectID = objectsToUse[i];
    const double lambda = lambdas[objectID];
    if(lambda > 0)
    {
      if(j >= numSV) {
        delete[] svl;
        delete[] lmbd;
        throw ExceptionC("Internal error in number of SV");
      }
      sv.Append(trainSetVectors[objectID]);
      svl[j] = trainingSetLabels[objectID];
      lmbd[j] = lambda;
      //cout << j << ": lambda[" << objectID << "] = " << lambda << " label = " << int(svl[j]) << endl;
      j++;
    }
  }
  //cout << "j:" << j << "   numSV:" << numSV << endl;
  if(j != numSV) {
    delete[] svl;
    delete[] lmbd;
    throw ExceptionC("Internal error in number of SV");
  }

  SvmClassifierC rv(kernelFunction, sv, svl, lmbd, b);
  delete[] svl;
  delete[] lmbd;
  return rv;
}
//---------------------------------------------------------------------------
RealT DesignSvmSmoBodyC::GetKernelValue(int I1, int I2)
{
  if(I1 < I2)
  {
    int t = I1;
    I1 = I2;
    I2 = t;
  }

  float* elAdr = kernelCache + ((SizeT(I1) * SizeT(I1 + 1)) / 2 + I2);
  float cacheVal = *elAdr;

  if(cacheVal == 0)
  {
    cacheVal = (float)kernelFunction.Apply(numFeatures, trSetVectorPtrs[I1], trSetVectorPtrs[I2]);
    *elAdr = cacheVal;
//    cout << "new kernel (" << I1 << ", " << I2 << ") = " << cacheVal << endl;
  }
  if(cacheVal > 1e25)
  {
    printf("Warning: Possible overflow kernelfunc: %lg", cacheVal);
  }
  return cacheVal;
}
//---------------------------------------------------------------------------
void DesignSvmSmoBodyC::CalcLambdas()
{
  UpdateErrorCache();

  int numChanged = 0;
  int examineAll = 1;

  while(numChanged > 0 || examineAll)
  {
    numChanged = 0;
    if(examineAll)
    {
      int step = rand() % numTrainObjects;
      if(step == 0)
        step = 1;
      for(int i = 0; i < step; i++)
      {
        for(int k = i; k < numTrainObjects; k += step)
        {
          int objectID = objectsToUse[k];
          numChanged += ExamineExample(objectID);
        }
      }
      //for(int k = 0; k < numTrainObjects; k++)
      //numChanged += ExamineExample(k);
    }
    else
    {
     for(int k = 0; k < numTrainObjects; k++)
     {
       int objectID = objectsToUse[k];
       if(lambdas[objectID] != 0 &&
          (trainingSetLabels[objectID] > 0 ? lambdas[objectID] != c1 :
                                             lambdas[objectID] != c2))
         numChanged += ExamineExample(objectID);
     }
    }

    //cout << "threshold:" << b << endl;
    if(callbackFunc != NULL)
      callbackFunc(callbackData, numChanged);

    if(examineAll)
      examineAll = 0;
    else if(!numChanged)
      examineAll = 1;
  }

}
//---------------------------------------------------------------------------
void DesignSvmSmoBodyC::UpdateErrorCache()
{
  for(int i = 0; i < trainSetSize; i++)
    errorCache[i] = DOUBLE_QNAN;
}
//---------------------------------------------------------------------------
int DesignSvmSmoBodyC::ExamineExample(int I1)
{
  double y1 = trainingSetLabels[I1];
  double lambda1 = lambdas[I1];
  double _c1 = y1 > 0 ? c1 : c2;
  double e1 = errorCache[I1];
  if(IS_NAN(e1))
    e1 = errorCache[I1] = CalcFunc(I1) - y1;
  double r1 = y1 * e1;

  if((r1 < -tolerance && lambda1 < _c1) || (r1 > tolerance && lambda1 > 0.))
  {
    // 1st Attempt
    double tmax = 0.;
    int i2 = -1;
    for(int k = 0; k < numTrainObjects; k++)
    {
      int objectID = objectsToUse[k];
      double tl = lambdas[objectID];
      if(tl > 0. && tl < (trainingSetLabels[objectID] > 0. ? c1 : c2))
      {
        double e2 = errorCache[objectID];
        double temp = fabs(e1 - e2);
        if(temp > tmax)
        {
          tmax = temp;
          i2 = objectID;
        }
      }
    }
    if(i2 >= 0)
      if(TakeStep (I1, i2))
        return 1;

    // 2nd Attempt
    int k0 = rand() % numTrainObjects;
    for(int k = k0; k < numTrainObjects + k0; k++)
    {
      int i2 = k % numTrainObjects;
      int objectID = objectsToUse[i2];
      double tl = lambdas[objectID];
      if(tl > 0. && tl < (trainingSetLabels[objectID] > 0. ? c1 : c2))
      {
        if(TakeStep(I1, objectID))
          return 1;
      }
    }

    // 3rd Attempt
    k0 = rand() % numTrainObjects;
    for(int k = k0; k < numTrainObjects + k0; k++)
    {
      int i2 = k % numTrainObjects;
      int objectID = objectsToUse[i2];
      if(TakeStep(I1, objectID))
        return 1;
    }
  }
  return 0;
}
//---------------------------------------------------------------------------
int DesignSvmSmoBodyC::TakeStep(int I1, int I2)
{
  if(I1 == I2)
    return 0;

  double lambda1 = lambdas[I1];
  double y1 = trainingSetLabels[I1];
  double _c1 = y1 > 0 ? c1 : c2;
  double e1 = errorCache[I1];
  if(IS_NAN(e1))
    e1 = errorCache[I1] = CalcFunc(I1) - y1;

  double lambda2 = lambdas[I2];
  double y2 = trainingSetLabels[I2];
  double _c2 = y2 > 0 ? c1 : c2;
  double e2 = errorCache[I2];
  if(IS_NAN(e2))
    e2 = errorCache[I2] = CalcFunc(I2) - y2;

  double s = y1 * y2;
  double L, H;
  if(s < 0.)
  {
    double gamma = lambda2 - lambda1;
    if(gamma < 0.)
    {
      L = 0.;
      H = _c1 + gamma;
      if(H > _c2)
        H = _c2;
    }
    else
    {
      L = gamma;
      if(L > _c2)
        L = _c2;
      H = _c2;
    }
  }
  else
  {
    double gamma = lambda2 + lambda1;
    if(gamma > _c2)
    {
      L = gamma - _c1;
      if(L > _c2)
        L = _c2;
      H = _c2;
    }
    else
    {
      L = 0.;
      H = gamma;
      if(H > _c2)
        H = _c2;
    }
  }

  if(L == H)
    return 0;

  double k12 = GetKernelValue(I1, I2);
  double k11 = GetKernelValue(I1, I1);
  double k22 = GetKernelValue(I2, I2);
  double eta = 2. * k12 - k11 - k22;

  double l2, delta_l2;
  //bool force = false;
  if(eta < 0.)
  { // Normal case
    l2 = lambda2 + (delta_l2 = y2 * (e2 - e1) / eta);
    if(l2 < L)
    {
      delta_l2 = (l2 = L) - lambda2;
      //force = L == 0;
    }
    else if(l2 > H)
    {
      delta_l2 = (l2 = H) - lambda2;
      //force = H == _c2;
    }
  }
  else
  {
    double cn1 = eta / 2.;
    double cn2 = y2 * (e1 - e2) - eta *  lambda2;
    double Lobj = (cn1 * L + cn2) * L;
    double Hobj = (cn1 * H + cn2) * H;

    if(Lobj > Hobj + eps)
    {
      l2 = L;
    }
    else if(Lobj < Hobj - eps)
    {
      l2 = H;
    }
    else
      l2 = lambda2;

    delta_l2 = l2 - lambda2;
  }
  if(l2 != 0. && l2 < lambdaThreshold)
  {
    l2 = 0.;
    delta_l2 = -lambda2;
    //force = true;
  }

  if((fabs(delta_l2) - lambdaThreshold) < eps * (l2 + lambda2/* + eps*/)) // Step too small
  {
    //if(!force || delta_l2 == 0.)
      return 0;
    /*else
    cout << "forcing\n";*/
  }

  double delta_l1;
  double l1 = lambda1 + (delta_l1 = -s * delta_l2);
  if(l1 != 0. && l1 < lambdaThreshold)
  {
    l1 = 0.;
    delta_l1 = -lambda1;
    l2 = lambda2 + (delta_l2 = -s * delta_l1);
  }

  double t1 = y1 * delta_l1;
  double t2 = y2 * delta_l2;

  // Update b
  double delta_b;
  if(l1 > 0. && l1 < c1)
    b -= (delta_b = e1 + t1 * k11 + t2 * k12);
  else if(l2 > 0. && l2 < c2)
    b -= (delta_b = e2 + t1 * k12 + t2 * k22);
  else
    b -= (delta_b = (e1 + e2 + t1 * (k11 + k12) + t2 * (k12 + k22)) / 2.);

  lambdas[I1] = l1;
  lambdas[I2] = l2;

  // Update Error Cache
  for(int k = 0; k < numTrainObjects; k++)
  {
    int objectID = objectsToUse[k];
    //double tl = lambdas[objectID];
    if(/*0 < tl &&*/ /*tl < (trainingSetLabels[objectID] > 0 ? c1 : c2) &&*/
       !IS_NAN(errorCache[objectID]))
    {
      errorCache[objectID] +=  t1 * GetKernelValue(I1, objectID) +
                               t2 * GetKernelValue(I2, objectID) - delta_b;
    }
    else
    {
      errorCache[objectID] = DOUBLE_QNAN;
    }
  }

  return 1;
}
//---------------------------------------------------------------------------
double DesignSvmSmoBodyC::CalcFunc(int I)
{
  //static int nn = 0;
  //cout << "DesignSvmSmoBodyC::CalcFunc:" << nn++ << endl;
  /*double f = b;
  double *lPtr = lambdas;
  for(int i = 0; i < trainSetSize; i++, lPtr++)
  {
    if(*lPtr > 0)
      f += *lPtr * trainingSetLabels[i] * GetKernelValue(I, i);
  }
  return f;*/
  long double f = b;
  for(int k = 0; k < numTrainObjects; k++)
  {
    const int objectID = objectsToUse[k];
    double lambda = lambdas[objectID];
    if(lambda > 0.)
      f += lambda * trainingSetLabels[objectID] * GetKernelValue(I, objectID);
  }
  return f;
}
//---------------------------------------------------------------------------
IntT DesignSvmSmoBodyC::NumSupportVectors() const
{
  IntT numSV = 0;
  double *lPtr = lambdas;
  for(int i = trainSetSize; i > 0 ; i--, lPtr++)
  {
    if(*lPtr > 0.)
      numSV++;
  }
  return numSV;
}
//---------------------------------------------------------------------------
IntT DesignSvmSmoBodyC::LeaveOneOutTest()
{
  //store lambdas
  double* lambdaStore = new double[trainSetSize];
  memcpy(lambdaStore, lambdas, trainSetSize * sizeof(double));

  //store objectsToUse
  int* objectsToUseStore = objectsToUse;
  objectsToUse = new int[numTrainObjects-1];
  numTrainObjects--;

  int numErrors = 0;
  //loop over SV
  for(int i = 0; i < trainSetSize; i++)
  {
    if(lambdas[i] == 0)
      continue;

    //cout << "checking:" << i << endl;
    //set objects to use
    memcpy(objectsToUse, objectsToUseStore, numTrainObjects * sizeof(int));
    for(int j = 0; j < numTrainObjects; j++)
    {
      if(objectsToUse[j] == i)
      {
        objectsToUse[j] = objectsToUseStore[numTrainObjects];
        break;
      }
    }

    //set lambda for the out object to 0
    lambdas[i] = 0;

    //run SMO
    CalcLambdas();

    //calc descriminant function
    double dv = CalcFunc(i) * trainingSetLabels[i];
    //cout << "dv:" << dv << endl;

    //update statistics
    if(dv <= 0)
    {
      numErrors++;
    }

    //restore lambdas from store
    memcpy(lambdas, lambdaStore, trainSetSize * sizeof(double));
  }

  //restore object to use
  delete[] objectsToUse;
  objectsToUse = objectsToUseStore;
  numTrainObjects++;

  delete[] lambdaStore;

  //cout << "numErrors:" << numErrors << endl;
  return numErrors;
}


RavlN::XMLFactoryRegisterHandleConvertC<DesignSvmSmoC, DesignSvmC> g_registerXMLFactoryDesignSvmSmo("RavlN::DesignSvmSmoC");
  
void linkDesignSvmSmo()
{}



//---------------------------------------------------------------------------
}

