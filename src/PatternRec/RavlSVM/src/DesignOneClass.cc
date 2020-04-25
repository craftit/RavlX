// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
// $Id: DesignOneClass.cc 10146 2007-02-01 11:17:56Z alex $
//! lib=RavlSVM

//! author="Alexey Kostin"

#include "Ravl/PatternRec/DesignOneClass.hh"
#include "Ravl/PatternRec/SampleIter.hh"
#include <string.h>
#include "Ravl/XMLFactoryRegister.hh"

namespace RavlN
{
using namespace RavlN;

typedef double *DoublePtrT;
#define DOUBLE_QNAN 999e100
#define IS_NAN(_X) ((_X) == DOUBLE_QNAN)

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//! Default constructor.
DesignOneClassBodyC::DesignOneClassBodyC(const KernelFunctionC &KernelFunction,
                                         double Penalty1, double Penalty2,
                                         double Tolerance, double Eps,
                                         double LambdaThreshold)
                    :DesignSvmBodyC()
{
  InitMembers();

  kernelFunction = KernelFunction;
  c1 = Penalty1;
  c2 = Penalty2;
  toleranceR = Tolerance;
  toleranceA = 0.;
  eps = Eps;
  lambdaThreshold = LambdaThreshold;
}


//---------------------------------------------------------------------------
DesignOneClassBodyC::DesignOneClassBodyC(const XMLFactoryContextC & factory)
  : DesignSvmBodyC(factory),
    c1(factory.AttributeReal("penalty1", 1e3)),
    c2(factory.AttributeReal("penalty2", 1e3)),
    toleranceR(factory.AttributeReal("tolerance", 1e-7)),
    toleranceA(0),
    eps(factory.AttributeReal("eps", 1e-9)),
    lambdaThreshold(factory.AttributeReal("lambdaThreshold", 1e-12))
        
{

  // from InitMembers....
  lambdas           = NULL;
  errorCache        = NULL;
  sCache            = NULL;
  trainingSetLabels = NULL;
  trSetVectorPtrs   = NULL;
  objectsToUse      = NULL;
  kernelCache       = NULL;

  trainSubsetSize = 0;
  trainSetSize = 0;
  numFeatures = 0;

  callbackFunc = NULL;
  callbackData = NULL;

  debug = 0;

  if(!factory.UseComponent("KernelFunction", kernelFunction))
    RavlIssueError("No kernel function specified in XML factory");
  
}


  //---------------------------------------------------------------------------
//! Load from stream.
DesignOneClassBodyC::DesignOneClassBodyC(istream &strm)
                    :DesignSvmBodyC(strm)
{
  InitMembers();

  IntT version;
  strm >> version;
  if(version != 0)
    throw ExceptionC("DesignOneClassBodyC::DesignOneClassBodyC(istream &strm):"
                     "Unrecognised version number in stream.");
  strm >> kernelFunction;
  strm >> c1;
  strm >> c2;
  strm >> toleranceR;
  toleranceA = 0.;
  strm >> eps;
  strm >> lambdaThreshold;
}
//---------------------------------------------------------------------------
//! Load from binary stream.
DesignOneClassBodyC::DesignOneClassBodyC(BinIStreamC &strm)
                    :DesignSvmBodyC(strm)
{
  InitMembers();

  IntT version;
  strm >> version;
  if(version != 0)
    throw ExceptionC("DesignOneClassBodyC::DesignOneClassBodyC(BinIStreamC &strm),"
                     "Unrecognised version number in stream.");
  strm >> kernelFunction;
  strm >> c1;
  strm >> c2;
  strm >> toleranceR;
  toleranceA = 0.;
  strm >> eps;
  strm >> lambdaThreshold;
}
//---------------------------------------------------------------------------
//! Writes object to stream
bool DesignOneClassBodyC::Save(ostream &out) const
{
  if(!DesignSvmBodyC::Save(out))
    return false;
  const IntT version = 0;
  out << ' ' << version << '\n' <<  kernelFunction << '\n'
      << c1 << ' ' << c2 << '\n' << toleranceR << ' ' << eps << '\n'
      << lambdaThreshold;
  return true;
}
//---------------------------------------------------------------------------
//! Writes object to stream
bool DesignOneClassBodyC::Save(BinOStreamC &out) const
{
  if(!DesignSvmBodyC::Save(out))
    return false;
  const IntT version = 0;
  out << version << kernelFunction << c1 << c2 << toleranceR << eps
      << lambdaThreshold;
  return true;
}
//---------------------------------------------------------------------------
//! Destructor.
DesignOneClassBodyC::~DesignOneClassBodyC()
{
  ReleaseMemory();
}
//---------------------------------------------------------------------------
Classifier2C DesignOneClassBodyC::Apply(const SampleC<VectorC> &TrainingSetVectors,
                                        const SampleC<SByteT> &TrainingSetLabels,
                                        const SArray1dC<IndexC> &ObjectsToUse)
{
  AllocateMemory(ObjectsToUse.Size(), TrainingSetVectors.Size());

  //copy training set vectors
  trainingSetVectors = TrainingSetVectors;
  numFeatures = TrainingSetVectors[0].Size();

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
    throw ExceptionC("Can't copy data to internal svm beffers");

  //copy array of indices of objects to use for training
  for(int i = 0; i < trainSubsetSize; i++)
  {
    objectsToUse[i] = ObjectsToUse[i].V();
  }

  //clean buffers and vars
  Prepare();

  //find lagrangian multipliers
  CalcLambdas();

  return GetClassifier();
}
//---------------------------------------------------------------------------
ClassifierC DesignOneClassBodyC::Apply(const SampleC<VectorC> &TrainingSetVectors,
                                       const SampleC<UIntT> &TrainingSetLabels,
                                       const SArray1dC<IndexC> &ObjectsToUse)
{
  AllocateMemory(ObjectsToUse.Size(), TrainingSetVectors.Size());

  //copy training set vectors
  trainingSetVectors = TrainingSetVectors;
  numFeatures = TrainingSetVectors[0].Size();

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
    throw ExceptionC("Can't copy data to internal svm beffers");

  //copy array of indices of objects to use for training
  for(int i = 0; i < trainSubsetSize; i++)
  {
    objectsToUse[i] = ObjectsToUse[i].V();
  }

  //clean buffers and vars
  Prepare();

  //find lagrangian multipliers
  CalcLambdas();

  return GetClassifier();
}
//---------------------------------------------------------------------------
Classifier2C DesignOneClassBodyC::Apply(const SampleC<VectorC> &TrainingSetVectors,
                                        const SampleC<SByteT> &TrainingSetLabels)
{
  if(debug > 0)
    for(SizeT i = 0; i < TrainingSetVectors.Size(); i++)
      cout << i << " \t" << TrainingSetVectors[i][0] << " \t"
          << TrainingSetVectors[i][1] << " \t" << int(TrainingSetLabels[i]) << endl;

  AllocateMemory(TrainingSetVectors.Size(), TrainingSetVectors.Size());

  //copy training set vectors
  trainingSetVectors = TrainingSetVectors;
  numFeatures = TrainingSetVectors[0].Size();

  //copy trainingSetLabels
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
    throw ExceptionC("Can't copy data to internal svm beffers");

  //set objectsToUse
  for(int i = 0; i < trainSubsetSize; i++)
  {
    objectsToUse[i] = i;
  }

  //clean buffers and vars
  Prepare();

  //find lagrangian multipliers
  CalcLambdas();

  return GetClassifier();
}
//---------------------------------------------------------------------------
ClassifierC DesignOneClassBodyC::Apply(const SampleC<VectorC> &TrainingSetVectors,
                                       const SampleC<UIntT> &TrainingSetLabels)
{
  AllocateMemory(TrainingSetVectors.Size(), TrainingSetVectors.Size());

  //for(int i = 0; i < trainSetSize; i++)
  //  cout << i << " \t" << TrainingSetVectors[i][0] << " \t"
  //       << TrainingSetVectors[i][1] << " \t" << TrainingSetLabels[i] << endl;

  //copy training set vectors
  trainingSetVectors = TrainingSetVectors;
  numFeatures = TrainingSetVectors[0].Size();

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
    throw ExceptionC("Can't copy data to internal svm beffers");

  //set objectsToUse
  for(int i = 0; i < trainSubsetSize; i++)
  {
    objectsToUse[i] = i;
  }

  //clean buffers and vars
  Prepare();

  //find lagrangian multipliers
  CalcLambdas();

  return GetClassifier();
}
//---------------------------------------------------------------------------
void DesignOneClassBodyC::Prepare()
{
  //clear lambdas array
  memset(lambdas, 0, sizeof(double) * trainSetSize);

  //find first inlier and sel lambda to 1
  bool lambdaSet = false;
  for(int i = 0; i < trainSubsetSize; i++)
  {
    const int firstID = objectsToUse[i];
    if(trainingSetLabels[firstID] > 0)
    {
      lambdas[firstID] = 1.;
      lambdaSet = true;
      break;
    }
  }
  if(!lambdaSet)
    throw ExceptionC("DesignOneClassBodyC::Prepare:"
                     "No inliers in training set!\n");

  //clear kernel cache
  SizeT kernelCacheSize = (SizeT(trainSetSize + 1) * SizeT(trainSetSize)) / 2;
  memset(kernelCache, 0, sizeof(float) * kernelCacheSize);

  //update sCache
  for(int i = 0; i < trainSetSize; i++)
  {
    sCache[i] = DOUBLE_QNAN;
  }
}
//---------------------------------------------------------------------------
OneClassC DesignOneClassBodyC::GetClassifier() const
{
  //create classifier
  int numSV = NumSupportVectors();

  SampleC<VectorC> sv(numSV);
  char *svl = new char[numSV];
  RealT *lmbd = new RealT[numSV];
  if(svl == NULL || lmbd == NULL)
    throw ExceptionC("DesignOneClassBodyC::GetClassifier:"
                     "Can't allocate memory for internal svm buffers");
  try
  {
    int j = 0;
    for(int i = 0; i < trainSubsetSize; i++)
    {
      const int objectID = objectsToUse[i];
      const double lambda = lambdas[objectID];
      if(lambda > 0.)
      {
        if(j >= numSV)
          throw ExceptionC("DesignOneClassBodyC::GetClassifier:"
                          "Internal error in number of SV");
        sv.Append(trainingSetVectors[objectID]);
        svl[j] = trainingSetLabels[objectID];
        lmbd[j] = lambda;
        //cout << j << ": lambda[" << objectID << "] = " << lambda << " label = " << int(svl[j]) << endl;
        j++;
      }
    }
    //cout << "j:" << j << "   numSV:" << numSV << endl;
    if(j != numSV)
      throw ExceptionC("DesignOneClassBodyC::GetClassifier:"
                      "Internal error in number of SV");

    OneClassC rv(kernelFunction, sv, svl, lmbd, radius + 2. * toleranceA);
    delete[] svl;
    delete[] lmbd;
    return rv;
  }
  catch( ... )
  {
    delete[] svl;
    delete[] lmbd;
    throw;
  }
  return OneClassC();
}
//---------------------------------------------------------------------------
RealT DesignOneClassBodyC::GetKernelValue(int I1, int I2) const
{
  //cout << "DesignOneClassBodyC::GetKernelValue\n";
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
  return cacheVal;
}
//---------------------------------------------------------------------------
void DesignOneClassBodyC::CalcLambdas()
{
  //radius = CalculateRadius(norm);
  if(debug > 0)
  {
    cout << "DesignOneClassBodyC::CalcLambdas\n";
    //calculate sum and radius
    cout << "radius:" << radius << endl;
    cout << "norm:" << norm << endl;
    cout << "lambdas\n";
    for(int i = 0; i < trainSetSize; i++)
      cout << i << ":" << lambdas[i] << endl;
  }

  //UpdateErrorCache();

  int numChanged = 0;
  int examineAll = 1;

  while(numChanged > 0 || examineAll)
  {
    //toleranceA = sqrt(radius) * toleranceR;
    //toleranceA = radius * toleranceR;
    toleranceA = toleranceR;

    numChanged = 0;
    if(examineAll)
    {
      radius = CalculateRadius(norm);
      UpdateErrorCache();

      int step = rand() % trainSubsetSize;
      if(step == 0)
        step = 1;
      for(int i = 0; i < step; i++)
      {
        for(int k = i; k < trainSubsetSize; k += step)
        {
          int objectID = objectsToUse[k];
          numChanged += ExamineExample(objectID);
        }
      }
    }
    else
    {
      for(int k = 0; k < trainSubsetSize; k++)
      {
        const int objectID = objectsToUse[k];
        const double lambda = lambdas[objectID];
        if(lambda != 0 &&
           (trainingSetLabels[objectID] > 0 ? lambda != c1 : lambda != c2))
          numChanged += ExamineExample(objectID);
      }
    }

    if(callbackFunc != NULL)
      callbackFunc(callbackData, numChanged);

    if(examineAll)
      examineAll = 0;
    else if(!numChanged)
      examineAll = 1;
  }
  if(debug > 0)
  {
    cout << "lambdas\n";
    for(int i = 0; i < trainSetSize; i++)
      cout << i << ":" << lambdas[i] << endl;
    cout << "sCache errorCache\n";
    for(int i = 0; i < trainSetSize; i++)
      cout << i << ":" << sCache[i] << "\t " << errorCache[i] << "\t "
           << int(trainingSetLabels[i]) << endl;
    cout << "norm:" << norm << endl;
    cout << "radius:" << radius << endl;
  }
}
//---------------------------------------------------------------------------
void DesignOneClassBodyC::UpdateErrorCache()
{
  for(int i = 0; i < trainSetSize; i++)
  {
    errorCache[i] = DOUBLE_QNAN;
    sCache[i] = DOUBLE_QNAN;
  }
}
//---------------------------------------------------------------------------
int DesignOneClassBodyC::ExamineExample(int I1)
{
  if(debug > 0)
    cout << "ExamineExample:" << I1 << endl;
  double y1 = trainingSetLabels[I1];
  double lambda1 = lambdas[I1];
  double _c1 = y1 > 0 ? c1 : c2;
  double e1 = errorCache[I1];
  if(IS_NAN(e1))
    e1 = errorCache[I1] = CalcFunc(I1);
  double r1 = y1 * e1;

  if((r1 < -toleranceA && lambda1 < _c1) || (r1 > toleranceA && lambda1 > 0.))
  //if((r1 < -toleranceA && lambda1 < _c1) || (r1 > 0. && lambda1 > 0.))
  {
    // 1st Attempt
    double tmax = 0.;
    int i2 = -1;
    for(int k = 0; k < trainSubsetSize; k++)
    {
      int objectID = objectsToUse[k];
      double tl = lambdas[objectID];
      if(tl > 0. && tl < (trainingSetLabels[objectID] > 0. ? c1 : c2))
      {
        double e2 = errorCache[objectID];
        if(IS_NAN(e2))
          e2 = errorCache[objectID] = CalcFunc(objectID);
        const double temp = fabs(e1 - e2);
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
    int k0 = rand() % trainSubsetSize;
    for(int k = k0; k < trainSubsetSize + k0; k++)
    {
      int i2 = k % trainSubsetSize;
      int objectID = objectsToUse[i2];
      double tl = lambdas[objectID];
      if(tl > 0. && tl < (trainingSetLabels[objectID] > 0. ? c1 : c2))
      {
        if(TakeStep(I1, objectID))
          return 1;
      }
    }

    // 3rd Attempt
    k0 = rand() % trainSubsetSize;
    for(int k = k0; k < trainSubsetSize + k0; k++)
    {
      int i2 = k % trainSubsetSize;
      int objectID = objectsToUse[i2];
      if(TakeStep(I1, objectID))
        return 1;
    }
  }
  else
  {
    if(debug > 0 && r1 < 0.)
      cout << "Ignoring error:" << r1 << "  toleranceA:" << toleranceA
          << "  lambda1:" << lambda1 << "  _c1:" << _c1 << endl;
  }
  return 0;
}
//---------------------------------------------------------------------------
int DesignOneClassBodyC::TakeStep(int I1, int I2)
{
  if(I1 == I2)
    return 0;

  if(debug > 0)
  {
    cout << "TakeStep " << I1 << ", " << I2 << endl;
    cout << "lambdas\n";
    for(int i = 0; i < trainSetSize; i++)
      if(lambdas[i] != 0.)
        cout << i << ":" << lambdas[i] << endl;
  }

  double lambda1 = lambdas[I1];
  double y1 = trainingSetLabels[I1];
  double _c1 = y1 > 0 ? c1 : c2;
  double e1 = errorCache[I1];
  if(IS_NAN(e1))
    e1 = errorCache[I1] = CalcFunc(I1);
  if(debug > 0)
    cout << "e1:" << e1 << endl;

  double lambda2 = lambdas[I2];
  double y2 = trainingSetLabels[I2];
  double _c2 = y2 > 0 ? c1 : c2;
  double e2 = errorCache[I2];
  if(IS_NAN(e2))
    e2 = errorCache[I2] = CalcFunc(I2);
  if(debug > 0)
    cout << "e2:" << e2 << endl;

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

  if(debug > 0)
    cout << "L:" << L << endl << "H:" << H << endl;

  if(L == H)
    return 0;

  double k12 = GetKernelValue(I1, I2);
  double k11 = GetKernelValue(I1, I1);
  double k22 = GetKernelValue(I2, I2);
  double eta = k11 + k22 - 2. * k12;

  if(debug > 0)
    cout << "k11:" << k11 << endl
         << "k12:" << k12 << endl
         << "k22:" << k22 << endl
         << "eta:" << eta << endl;

  double l2, delta_l2;
  if(eta > 0.)
  {
    l2 = lambda2 + (delta_l2 = y2 * (e1 - e2) / (2. * eta));
    //cout << "l2:" << l2 << endl;
    if(l2 < L)
    {
      delta_l2 = (l2 = L) - lambda2;
    }
    else if(l2 > H)
    {
      delta_l2 = (l2 = H) - lambda2;
    }
  }
  else
  {
    return 0;
  }
  //cout << "l2:" << l2 << endl;

  if(l2 != 0. && l2 < lambdaThreshold)
  {
    l2 = 0.;
    delta_l2 = -lambda2;
  }
  //cout << "l2:" << l2 << endl;

  if((fabs(delta_l2) - lambdaThreshold) < eps * (l2 + lambda2)) // Step too small
  {
    return 0;
  }

  double delta_l1;
  double l1 = lambda1 + (delta_l1 = -s * delta_l2);
  if(l1 != 0. && l1 < lambdaThreshold)
  {
    l1 = 0.;
    delta_l1 = -lambda1;
    l2 = lambda2 + (delta_l2 = -s * delta_l1);
  }
  if(debug > 0)
    cout << "l1:" << l1 << "   dl1:" << delta_l1 << endl
         << "l2:" << l2 << "   dl2:" << delta_l2 << endl;

  //store new lambdas
  lambdas[I1] = l1;
  lambdas[I2] = l2;

  // Update caches and vars
  const double t1 = y1 * delta_l1;
  const double t2 = y2 * delta_l2;

  double s1 = sCache[I1];
  if(IS_NAN(s1))
    s1 = sCache[I1] = CalcS(I1);

  double s2 = sCache[I2];
  if(IS_NAN(s2))
    s2 = sCache[I2] = CalcS(I2);

  if(debug > 0)
    cout << "s1:" << s1 << endl << "s2:" << s2 << endl;

  //norm
  double delta_norm;
  norm += (delta_norm = 2. * t1 * s1 + 2. * t2 * s2 +
                        t1 * t1 * k11 + 2. * t1 * t2 * k12 + t2 * t2 * k22);
  if(debug > 0)
    cout << "delta_norm:" << delta_norm << endl << "norm:" << norm << endl;

  //radius
  double delta_radius;
  if(l1 > 0. && l1 < c1)
    radius += (delta_radius = delta_norm - e1 - 2. * t1 * k11 - 2. * t2 * k12);
  else if(l2 > 0. && l2 < c2)
    radius += (delta_radius = delta_norm - e2 - 2. * t1 * k12 - 2. * t2 * k22);
  else
    radius += (delta_radius = delta_norm - (e1 + e2) / 2. -
                              t1 * k11 - 2. * (t1 + t2) * k12 - t2 * k22);
  if(debug > 0)
    cout << "radius:" << radius << endl << "delta_radius:" << delta_radius << endl;

  // Update Error Cache and s cache
  for(int k = 0; k < trainSubsetSize; k++)
  {
    int objectID = objectsToUse[k];

    if(IS_NAN(sCache[objectID]))
      continue;

    const double part1 = t1 * GetKernelValue(I1, objectID) +
                         t2 * GetKernelValue(I2, objectID);
    if(debug > 0)
      cout << "part1:" << part1 << endl;

    if(debug > 0)
      cout << "old sCache[objectID]:" << sCache[objectID] << endl;
    sCache[objectID] += part1;

    if(IS_NAN(errorCache[objectID]))
      continue;

    if(debug > 0)
      cout << "old errorCache[objectID]:" << errorCache[objectID] << endl;
    errorCache[objectID] += delta_radius + 2. * part1 - delta_norm;

    if(debug > 0)
      cout << k << "\t " << sCache[objectID] << "\t " << errorCache[objectID] << endl;
  }

  return 1;
}
//---------------------------------------------------------------------------
double DesignOneClassBodyC::CalcFunc(int I)
{
  if(debug > 0)
    cout << "CalcFunc:" << I << endl;
  double s = sCache[I];
  if(s == DOUBLE_QNAN)
    sCache[I] = s = CalcS(I);
  if(debug > 0)
    cout << "s:" << s << endl;

  const double rv = radius - GetKernelValue(I, I) + 2. * s - norm;
  if(debug > 0)
    cout << "rv:" << rv << endl;
  return rv;
}
//---------------------------------------------------------------------------
double DesignOneClassBodyC::CalcS(int I) const
{
  if(debug > 0)
    cout << "CalcS:" << I << endl;
  long double accum = 0.;
  for(int k = 0; k < trainSubsetSize; k++)
  {
    const int objectID = objectsToUse[k];
    const double lambda = lambdas[objectID];
    if(lambda != 0.)
    {
      const double y = trainingSetLabels[objectID];
      accum += lambda * y * GetKernelValue(objectID, I);
    }
  }
  if(debug > 0)
    cout << "accum:" << accum << endl;
  return (double)accum;
}
//---------------------------------------------------------------------------
//! Calculate radius
long double DesignOneClassBodyC::CalculateRadius(long double &Norm) const
{
  if(debug > 0)
    cout << "DesignOneClassBodyC::CalculateRadius\n";
  if(trainSubsetSize == 0)
    return 0;

  //calculate squared norm of center vector
  long double accum = 0.;
  for(int i = 0; i < trainSubsetSize; i++)
  {
    //cout << "i:" << i << endl;
    const int objectID1 = objectsToUse[i];
    //cout << "objectID1:" << objectID1 << endl;
    const double lambda1 = lambdas[objectID1];
    //cout << "lambda1:" << lambda1 << endl;
    if(lambda1 != 0.)
    {
      double y1 = trainingSetLabels[objectID1];
      accum += lambda1 * lambda1 * GetKernelValue(objectID1, objectID1);

      for(int j = i + 1; j < trainSubsetSize; j++)
      {
        const int objectID2 = objectsToUse[j];
        const double lambda2 = lambdas[objectID2];
        if(lambda2 != 0.)
        {
          double y2 = trainingSetLabels[objectID2];
          accum += 2. * lambda1 * lambda2 * y1 * y2 *
                   GetKernelValue(objectID1, objectID2);
        }
      }
    }
  }
  Norm = accum;
  if(debug > 0)
    cout << "norm:" << Norm << endl;

  //find first sv
  int firstSvID = 0;
  for(int i = 0; i < trainSubsetSize; i++)
  {
    const int objectID = objectsToUse[i];
    const double lambda = lambdas[objectID];
    if(lambda != 0.)
    {
      firstSvID = objectID;
      break;
    }
  }
  if(debug > 0)
    cout << "first sv:" << firstSvID << endl;

  //calculate radius
  accum += GetKernelValue(firstSvID, firstSvID);
  if(debug > 0)
    cout << "norm sv:" << GetKernelValue(firstSvID, firstSvID) << endl;
  for(int i = 0; i < trainSubsetSize; i++)
  {
    const int objectID2 = objectsToUse[i];
    const double lambda2 = lambdas[objectID2];
    if(lambda2 != 0.)
    {
      double y2 = trainingSetLabels[objectID2];
      if(debug > 0)
        cout << "i:" << i << " \tlambda2:" << lambda2 << " \ty2:" << y2
             << " \tkern:" << GetKernelValue(firstSvID, objectID2) << endl;
      accum -= 2. * lambda2 * y2 * GetKernelValue(firstSvID, objectID2);
    }
  }
  if(debug > 0)
    cout << "radius:" << accum << endl;

  return accum;
}
//---------------------------------------------------------------------------
IntT DesignOneClassBodyC::NumSupportVectors() const
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
void DesignOneClassBodyC::InitMembers()
{
  c1 = 0.;
  c2 = 0.;
  toleranceR = 0.;
  toleranceA = 0.;
  eps = 0.;
  lambdaThreshold = 0.;
  trainSetSize = 0;

  lambdas           = NULL;
  errorCache        = NULL;
  sCache            = NULL;
  trainingSetLabels = NULL;
  trSetVectorPtrs   = NULL;
  objectsToUse      = NULL;
  kernelCache       = NULL;

  trainSubsetSize = 0;
  trainSetSize = 0;
  numFeatures = 0;

  callbackFunc = NULL;
  callbackData = NULL;

  debug = 0;
}
//---------------------------------------------------------------------------
void DesignOneClassBodyC::AllocateMemory(int NewTrainSubsetSize,
                                         int NewTrainSetSize)
{
  //cout << "NewTrainSubsetSize:" << NewTrainSubsetSize << endl;
  //cout << "NewTrainSetSize:" << NewTrainSetSize << endl;

  if((trainSetSize != NewTrainSetSize && trainSetSize != 0) ||
     (trainSubsetSize != NewTrainSubsetSize && trainSubsetSize != 0))
  {
    ReleaseMemory();
  }

  if(trainSetSize == 0 || trainSubsetSize == 0)
  {
    errorCache        = new double[NewTrainSetSize];
    sCache            = new double[NewTrainSetSize];
    lambdas           = new double[NewTrainSetSize];
    trainingSetLabels = new char[NewTrainSetSize];
    trSetVectorPtrs   = new DoublePtrT[NewTrainSetSize];
    objectsToUse      = new int[NewTrainSubsetSize];

    SizeT kernelCacheSize = (SizeT(NewTrainSetSize + 1) *
                             SizeT(NewTrainSetSize)) / 2;
    //cout << "kernelCacheSize:" << kernelCacheSize << endl;
    kernelCache = new float[kernelCacheSize];

    //cout << "errorCache:" << SizeT(errorCache) << endl;
    //cout << "sCache:" << SizeT(sCache) << endl;
    //cout << "lambdas:" << SizeT(lambdas) << endl;
    //cout << "trainingSetLabels:" << SizeT(trainingSetLabels) << endl;
    //cout << "objectsToUse:" << SizeT(objectsToUse) << endl;
    //cout << "kernelCache:" << SizeT(kernelCache) << endl;
  }

  if(errorCache == NULL || sCache == NULL || lambdas == NULL ||
     trainingSetLabels == NULL || trSetVectorPtrs == NULL ||
     objectsToUse == NULL || kernelCache == NULL)
    throw ExceptionC("DesignOneClassBodyC::AllocateMemory:"
                     "Can't allocate memory for internal buffers");

  trainSubsetSize = NewTrainSubsetSize;
  trainSetSize = NewTrainSetSize;
}
//---------------------------------------------------------------------------
void DesignOneClassBodyC::ReleaseMemory()
{
  /*cout << "DesignOneClassBodyC::ReleaseMemory\n";
  cout << "errorCache:" << SizeT(errorCache) << endl;
  cout << "sCache:" << SizeT(sCache) << endl;
  cout << "lambdas:" << SizeT(lambdas) << endl;
  cout << "trainingSetLabels:" << SizeT(trainingSetLabels) << endl;
  cout << "objectsToUse:" << SizeT(objectsToUse) << endl;
  cout << "kernelCache:" << SizeT(kernelCache) << endl;*/

  delete[] errorCache;        errorCache = NULL;
  delete[] sCache;            sCache = NULL;
  delete[] lambdas;           lambdas = NULL;
  delete[] trainingSetLabels; trainingSetLabels = NULL;
  delete[] trSetVectorPtrs;   trSetVectorPtrs = NULL;
  delete[] objectsToUse;      objectsToUse = NULL;
  delete[] kernelCache;       kernelCache = NULL;
  trainSetSize = 0;
  trainSubsetSize = 0;
  //cout << "DesignOneClassBodyC::ReleaseMemory:ok\n";
}
//---------------------------------------------------------------------------

  RavlN::XMLFactoryRegisterHandleConvertC<DesignOneClassC, DesignSvmC> g_registerXMLFactoryDesignOneClass("RavlN::DesignOneClassC");
  
  void linkDesignOneClass()
  {}


}

