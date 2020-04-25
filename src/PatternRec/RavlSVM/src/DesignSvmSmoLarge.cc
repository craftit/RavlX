// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
//! lib=RavlSVM

//! author="Alexey Kostin"

#include "Ravl/PatternRec/DesignSvmSmoLarge.hh"
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
// Default constructor.
DesignSvmSmoLargeBodyC::DesignSvmSmoLargeBodyC(const KernelFunctionC &KernelFunction,
                                               IntT MaxNumSv,
                                               RealT Penalty1, RealT Penalty2,
                                               RealT Tolerance, RealT Eps,
                                               RealT LambdaThreshold)
  : DesignSvmSmoBodyC(KernelFunction, Penalty1, Penalty2,
                      Tolerance, Eps, LambdaThreshold)
{
  maxNumSv = MaxNumSv;
  objectsToUseLarge = NULL;
  kernelCacheIndices = NULL;
  objectsToUseLargeSize = 0;
  //cout << "c1:" << c1 << "   c2:" << c2 << endl;
  //cout << "tol:" << tolerance << "   eps:" << eps << endl;
  //cout << "lt:" << lambdaThreshold << endl;
}
//---------------------------------------------------------------------------

DesignSvmSmoLargeBodyC::DesignSvmSmoLargeBodyC(const XMLFactoryContextC & factory)
  : DesignSvmSmoBodyC(factory),
    maxNumSv(factory.AttributeUInt("max_num_sv", 10000))        
{
  objectsToUseLarge = NULL;
  kernelCacheIndices = NULL;
  objectsToUseLargeSize = 0;
}

  //-------------------------------------------------------------

//: Load from stream.
DesignSvmSmoLargeBodyC::DesignSvmSmoLargeBodyC(istream& Strm)
                       :DesignSvmSmoBodyC(Strm)
{
  Strm >> maxNumSv;
  objectsToUseLarge = NULL;
  objectsToUseLargeSize = 0;
}
//---------------------------------------------------------------------------
//: Load from binary stream.
DesignSvmSmoLargeBodyC::DesignSvmSmoLargeBodyC(BinIStreamC& Strm)
                       :DesignSvmSmoBodyC(Strm)
{
  Strm >> maxNumSv;
  objectsToUseLarge = NULL;
  objectsToUseLargeSize = 0;
}
//---------------------------------------------------------------------------
//: Writes object to stream
bool DesignSvmSmoLargeBodyC::Save(ostream &Strm) const
{
  if(!DesignSvmSmoBodyC::Save(Strm))
    return false;
  Strm << ' ' << maxNumSv;
  return true;
}
//---------------------------------------------------------------------------
//: Writes object to stream
bool DesignSvmSmoLargeBodyC::Save(BinOStreamC& Strm) const
{
  if(!DesignSvmSmoBodyC::Save(Strm))
    return false;
  Strm << ' ' << maxNumSv;
  return true;
}
//---------------------------------------------------------------------------
// Destructor.
DesignSvmSmoLargeBodyC::~DesignSvmSmoLargeBodyC()
{
  delete[] objectsToUseLarge;
  delete[] kernelCacheIndices;
}
//---------------------------------------------------------------------------
Classifier2C DesignSvmSmoLargeBodyC::Apply(const SampleC<VectorC> &TrainingSetVectors,
                                           const SampleC<SByteT> &TrainingSetLabels)
{
  Prepare(TrainingSetVectors, TrainingSetLabels);

  //prepare objects to use
  if(objectsToUseLarge == NULL || objectsToUseLargeSize != trainSetSize)
  {
    delete[] objectsToUseLarge;
    objectsToUseLarge = new int[trainSetSize];
    if(objectsToUseLarge == NULL)
      throw ExceptionOperationFailedC("Can't allocate memory for internal svm buffers");
    objectsToUseLargeSize = trainSetSize;
  }

  //copy objectsToUse
  for(int i = 0; i < objectsToUseLargeSize; i++)
  {
    objectsToUseLarge[i] = i;
  }

  //find Lagrangian multipliers
  CalcLambdas(false);

  return GetClassifier();
}
//---------------------------------------------------------------------------
ClassifierC DesignSvmSmoLargeBodyC::Apply(const SampleC<VectorC> &TrainingSetVectors,
                                          const SampleC<UIntT> &TrainingSetLabels)
{
  Prepare(TrainingSetVectors, TrainingSetLabels);

  if(objectsToUseLarge == NULL || objectsToUseLargeSize != trainSetSize)
  {
    delete[] objectsToUseLarge;
    objectsToUseLarge = new int[trainSetSize];
    if(objectsToUseLarge == NULL)
      throw ExceptionOperationFailedC("Can't allocate memory for internal svm buffers");
    objectsToUseLargeSize = trainSetSize;
  }

  //copy objectsToUse
  for(int i = 0; i < objectsToUseLargeSize; i++)
  {
    objectsToUseLarge[i] = i;
  }

  //find Lagrangian multipliers
  CalcLambdas(false);

  return GetClassifier();
}
//---------------------------------------------------------------------------
void DesignSvmSmoLargeBodyC::Prepare(const SampleC<VectorC> &TrainingSetVectors,
                                     const SampleC<SByteT> &TrainingSetLabels)
{
  trainSetVectors = TrainingSetVectors;
  trainSetSize = TrainingSetVectors.Size();
  numFeatures = TrainingSetVectors[0].Size();

  //additional actions
  if(errorCacheSize != trainSetSize || errorCache == NULL || lambdas == NULL
     || trainingSetLabels == NULL  || trSetVectorPtrs == NULL
     || kernelCacheIndices == NULL || objectsToUse == NULL)
  {
    delete[] errorCache;
    delete[] lambdas;
    delete[] trainingSetLabels;
    delete[] trSetVectorPtrs;
    delete[] kernelCacheIndices;
    delete[] objectsToUse;
    errorCacheSize = 0;
    errorCache = new double[trainSetSize];
    lambdas = new double[trainSetSize];
    trainingSetLabels = new char[trainSetSize];
    trSetVectorPtrs = new DoublePtrT[trainSetSize];
    kernelCacheIndices = new int[trainSetSize];
    objectsToUse = new int[maxNumSv];
    if(errorCache == NULL || lambdas == NULL || trainingSetLabels == NULL
       || trSetVectorPtrs == NULL
       || kernelCacheIndices == NULL || objectsToUse == NULL)
      throw ExceptionC("DesignSvmSmoLargeBodyC::Prepare:Can't allocate memory for internal svm buffers");
    errorCacheSize = trainSetSize;
  }
  for(int i = 0; i < trainSetSize; i++)
    kernelCacheIndices[i] = -1;

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
  SizeT newKernelCacheSize = (SizeT(maxNumSv + 1) * (SizeT)maxNumSv) / 2;
  //cout << "newKernelCacheSize:" << newKernelCacheSize << endl;
  if(kernelCache == NULL || kernelCacheSize != newKernelCacheSize)
  {
    delete[] kernelCache;
    kernelCacheSize = 0;
    kernelCache = new float[newKernelCacheSize];
    if(kernelCache == NULL)
      throw ExceptionC("DesignSvmSmoLargeBodyC::Prepare:Can't allocate memory for internal cache of svm kernel function");
    kernelCacheSize = newKernelCacheSize;
  }
  memset(kernelCache, 0, sizeof(float) * kernelCacheSize);
}
//---------------------------------------------------------------------------
void DesignSvmSmoLargeBodyC::Prepare(const SampleC<VectorC> &TrainingSetVectors,
                                     const SampleC<UIntT> &TrainingSetLabels)
{
  trainSetVectors = TrainingSetVectors;
  trainSetSize = TrainingSetVectors.Size();
  numFeatures = TrainingSetVectors[0].Size();

  //additional actions
  if(errorCacheSize != trainSetSize || errorCache == NULL || lambdas == NULL
     || trainingSetLabels == NULL || trSetVectorPtrs == NULL
     || kernelCacheIndices == NULL || objectsToUse == NULL)
  {
    delete[] errorCache;
    delete[] lambdas;
    delete[] trainingSetLabels;
    delete[] trSetVectorPtrs;
    delete[] kernelCacheIndices;
    delete[] objectsToUse;
    errorCacheSize = 0;
    errorCache = new double[trainSetSize];
    lambdas = new double[trainSetSize];
    trainingSetLabels = new char[trainSetSize];
    trSetVectorPtrs = new DoublePtrT[trainSetSize];
    kernelCacheIndices = new int[trainSetSize];
    objectsToUse = new int[maxNumSv];
    if(errorCache == NULL || lambdas == NULL || trainingSetLabels == NULL
       || trSetVectorPtrs == NULL || kernelCacheIndices == NULL || objectsToUse == NULL)
      throw ExceptionC("Can't allocate memory for internal svm buffers");
    errorCacheSize = trainSetSize;
  }
  for(int i = 0; i < trainSetSize; i++)
    kernelCacheIndices[i] = -1;

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
  SizeT newKernelCacheSize = (SizeT(maxNumSv + 1) * (SizeT)maxNumSv) / 2;
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
RealT DesignSvmSmoLargeBodyC::GetKernelValue(int I1, int I2)
{
  SizeT i1 = kernelCacheIndices[I1];
  SizeT i2 = kernelCacheIndices[I2];
  if(i1 < 0 || i2 < 0)
    cout << "err:" << " I1:" << I1 << "  i1:" << i1
        << "   I2:" << I2 << "  i2:" << i2 << endl;
  if(i1 < i2)
  {
    int t = i1;
    i1 = i2;
    i2 = t;
    t = I1;
    I1 = I2;
    I2 = t;
  }

  float* elAdr = kernelCache + ((SizeT(i1) * SizeT(i1 + 1)) / 2 + i2);
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
void DesignSvmSmoLargeBodyC::CalcLambdas(bool DoFinal)
{
  double* localErrCache = new double[trainSetSize];
  double* localLambdas = new double[trainSetSize];
  double localB = 0.;
  for(int i = 0; i < trainSetSize; i++)
  {
    localErrCache[i] = DOUBLE_QNAN;
    localLambdas[i] = lambdas[i];
  }

  //initialise objectsToUse array, it must have objects of both classes
  numTrainObjects = min(maxNumSv, trainSetSize);
  //cout << "numTrainObjects:" << numTrainObjects << endl;

  int* changedIndices = new int[numTrainObjects];

  //****************** create training subset for the first iteration **********
  // empty subset
  for(int i = 0; i < trainSetSize; i++)
    kernelCacheIndices[i] = -1;

  // copy support vectors first (if start form knows lambdas)
  int i = 0;
  bool needFirstClass = true, needSecondClass = true;
  for(int j = 0; j < trainSetSize && i < numTrainObjects; j++)
  {
    if(lambdas[j] != 0.)
    {
      kernelCacheIndices[(objectsToUse[i] = objectsToUseLarge[j])] = i;
      i++;
      if(needFirstClass && trainingSetLabels[objectsToUseLarge[j]] > 0)
      {
        needFirstClass = false;
      }
      if(needSecondClass && trainingSetLabels[objectsToUseLarge[j]] < 0)
      {
        needSecondClass = false;
      }
    }
  }
  //if(i == numTrainObjects)
  //{
  //  cerr << "Too many support vectors for first iteration\n";
  //  return;
  //}
  //make sure two classes in subset
  if(needFirstClass || needSecondClass)
  {
    for(int j = 0; j < trainSetSize && i < numTrainObjects; j++)
    {
      if(kernelCacheIndices[objectsToUseLarge[j]] < 0) //object not in subset
      {
        if(needFirstClass && trainingSetLabels[objectsToUseLarge[j]] > 0)
        {
          kernelCacheIndices[(objectsToUse[i] = objectsToUseLarge[j])] = i;
          i++;
          needFirstClass = false;
          if(!(needFirstClass || needSecondClass))
            break;
        }
        if(needSecondClass && trainingSetLabels[objectsToUseLarge[j]] < 0)
        {
          kernelCacheIndices[(objectsToUse[i] = objectsToUseLarge[j])] = i;
          i++;
          needSecondClass = false;
          if(!(needFirstClass || needSecondClass))
            break;
        }
      }
    }
  }
  if(needFirstClass || needSecondClass)
  {
    cerr << "All objects are of the same class\n";
    return;
  }
  //fill the rest of training subset
  for(int j = 0; j < trainSetSize && i < numTrainObjects; j++)
  {
    if(kernelCacheIndices[objectsToUseLarge[j]] < 0) //object not in subset
    {
      kernelCacheIndices[(objectsToUse[i] = objectsToUseLarge[j])] = i;
      i++;
    }
  }

  if(i < numTrainObjects) //must never happen
  {
    cerr << "Internal error (i < numTrainObjects)!!!!!!\n";
    return;
  }

  RealT toleranceBU = tolerance;
  RealT epsBU = eps;
  bool firstRun = false;
  if(!DoFinal && tolerance < 1e-2 && eps <= 1e-6)
  {
    tolerance = 0.3;
    eps = 1e-6;
    firstRun = true;
  }

  //main smo loop
  bool needToIterate;
  //UpdateErrorCache();
  int errorID = 0;
  int freeRef = 0;
  do
  {
    //memcpy(errorCache, localErrCache, trainSetSize * sizeof(double));
    UpdateErrorCache();//to remove accumulated error of summation of small numbers
    /*for(int i = 0; i < numTrainObjects; i++)
    {
      cout << i << ":" << objectsToUse[i] << "  lab:"
          << int(trainingSetLabels[objectsToUse[i]]) << "  ki:"
           << kernelCacheIndices[objectsToUse[i]]
          << "   " << trainingSetVectors[objectsToUse[i]][0]
          << "   " << trainingSetVectors[objectsToUse[i]][1]
          << endl;
    }*/
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

      //cout << "nc:" << numChanged << "   ea:" << examineAll << endl;
      if(callbackFunc != NULL)
        callbackFunc(callbackData, numChanged);

      if(examineAll)
        examineAll = 0;
      else if(!numChanged)
        examineAll = 1;
    }

    //check if we need an extra iteration and prepare for it
    //cout << "----------------------------------\n";
    /*for(int i = 0; i < numTrainObjects; i++)
    {
      cout << i << ":" << objectsToUse[i]
          << "  lab:" << int(trainingSetLabels[objectsToUse[i]])
          << "  ki:" << kernelCacheIndices[objectsToUse[i]]
          << "  lm:" << lambdas[objectsToUse[i]] << endl;
    } */

    //update local err cache
    int numChangedIndices = 0;
    int numSV = 0;
    for(int k = 0; k < numTrainObjects; k++)
    {
      const int objectID = objectsToUse[k];
      if(lambdas[objectID] > 0)
        numSV++;
      localLambdas[objectID] -= lambdas[objectID];
      if(localLambdas[objectID] != 0)
      {
        changedIndices[numChangedIndices] = objectID;
        numChangedIndices++;
      }
    }
    if(verbosity > 0)
      cout << "num changed lambdas:" << numChangedIndices
           << "  numSV:" << numSV << endl;
    double db = b - localB;
    for(int i = 0; i < trainSetSize; i++)
    {
      if(kernelCacheIndices[i] == -1) //can not use errorCache
      {
        if(numSV <= numChangedIndices)
          localErrCache[i] = DOUBLE_QNAN;
        else
        {
          double e1 = localErrCache[i];
          if(!IS_NAN(e1))
          {
            for(int k = 0; k < numChangedIndices; k++)
            {
              const int objectID = changedIndices[k];
              double lambda = localLambdas[objectID];
              if(lambda != 0.)
                e1 -= lambda * trainingSetLabels[objectID] *
                      kernelFunction.Apply(numFeatures, trSetVectorPtrs[i],
                                           trSetVectorPtrs[objectID]);
            }
          }
          localErrCache[i] = e1 + db;
        }
      }
      else
      {
        localErrCache[i] = errorCache[i];
      }
    }
    /*for(int i = 0; i < trainSetSize; i++)
    {
      if(IS_NAN(localErrCache[i]))
      {
        double f = b;
        for(int k = 0; k < numTrainObjects; k++)
        {
          const int objectID = objectsToUse[k];
          double lambda = lambdas[objectID];
          if(lambda > 0)
            f += lambda * trainingSetLabels[objectID] *
                kernelFunction(trainingSetVectors[i], trainingSetVectors[objectID]);
        }
        localErrCache[i] = f - trainingSetLabels[i];
      }
  } */
    localB = b;
    memcpy(localLambdas, lambdas, trainSetSize * sizeof(double));

    int prevErrorID = errorID - 1;
    if(prevErrorID < 0)
      prevErrorID = trainSetSize - 1;

    int lastFreeRef = freeRef - 1;
    if(lastFreeRef < 0)
      lastFreeRef = numTrainObjects - 1;

    int numErrors = 0;
    needToIterate = false;
    //int ccc = 0;
    double errTol = firstRun ? 2. * tolerance : tolerance;

    do
    {
      //cout << "errorID:" << errorID << "    freeRef:" << freeRef << endl;
      //find object with big error
      bool errorFound = false;
      while(errorID != prevErrorID)
      {
        //cout << "errorID:" << errorID << "   ki:" << kernelCacheIndices[errorID] << endl;
        if(kernelCacheIndices[errorID] == -1) //object not used before
        {
          double y1 = trainingSetLabels[errorID];
          double lambda1 = lambdas[errorID];
          double _c1 = y1 > 0 ? c1 : c2;
          double e1 = localErrCache[errorID];
          if(IS_NAN(e1))
          {
            /*double f = b;
            double *lPtr = lambdas;
            for(int i = 0; i < trainSetSize; i++, lPtr++)
            {
              if(*lPtr > 0)
                f += *lPtr * trainingSetLabels[i] *
                    kernelFunction(trainingSetVectors[errorID],
                                   trainingSetVectors[i]);
            } */

            long double f = b;
            for(int k = 0; k < numTrainObjects; k++)
            {
              const int objectID = objectsToUse[k];
              double lambda = lambdas[objectID];
              if(lambda > 0.)
                f += lambda * trainingSetLabels[objectID] *
                     kernelFunction.Apply(numFeatures, trSetVectorPtrs[errorID],
                                          trSetVectorPtrs[objectID]);
            }

            e1 = f - y1;
            localErrCache[errorID] = e1;
            //ccc++;
          }
          //if(fabs(e1 - localErrCache[errorID]) > 1e-5)
          //  cout << "err:" << errorID << "   v1:" << e1 << "  v2:" << localErrCache[errorID] << endl;;
          double r1 = y1 * e1;
          //cout << "r1:" << r1 << endl;
          if((r1 < -errTol && lambda1 < _c1) || (r1 > errTol && lambda1 > 0.))
          {
            errorFound = true;
            //cout << "errId:" << errorID << "   prevErrorID:" << prevErrorID << endl;
            //cout << "r1:" << r1 << "  tol:" << tolerance << "  l:" << lambda1 << endl;
            break;
          }
        }
        errorID++;
        if(errorID >= trainSetSize)
          errorID = 0;
      }
      //cout << "errorID:" << errorID << "    errorFound:" << errorFound << endl;

      if(errorFound)
      {
        needToIterate = true;

        //find free slot for object
        bool freeFound = false;
        while(freeRef != lastFreeRef)
        {
          int objectID = objectsToUse[freeRef];
          double lambda = lambdas[objectID];
          if(lambda == 0. &&
             (trainingSetLabels[objectID] * errorCache[objectID] > 0.))
          {
            freeFound = true;
            break;
          }
          freeRef++;
          if(freeRef >= numTrainObjects)
            freeRef = 0;
        }

        //cout << "freeRef:" << freeRef << "    freeFound:" << freeFound << endl;
        if(freeFound)
        {
          int freeID = objectsToUse[freeRef];
          errorCache[freeID] = DOUBLE_QNAN;
          kernelCacheIndices[errorID] = kernelCacheIndices[freeID];
          kernelCacheIndices[freeID] = -1;
          objectsToUse[freeRef] = errorID;
          numErrors++;

          freeRef++;
          if(freeRef >= numTrainObjects)
            freeRef = 0;

          //FIXME cleaning cache must be here!!!
        }

        if(errorFound)
        {
          errorID++;
          if(errorID >= trainSetSize)
            errorID = 0;
        }
      }
    } while((errorID != prevErrorID) && (freeRef != lastFreeRef));
    if(verbosity > 0)
      cout << "num changes:" << numErrors
         << "  last errID:" << errorID
         << "  trSetSize:" << trainSetSize
         << "  lastFreeRef:" << freeRef
         << "  numTrObjects:" << numTrainObjects << endl;
    //cout << "ccc:" << ccc << endl;
    //slow but does the job
    memset(kernelCache, 0, sizeof(float) * kernelCacheSize);

    if((!needToIterate || numErrors == 0) && firstRun)
    {
      if(verbosity > 0)
        cout << "Doing final processing\n";
      needToIterate = true;
      firstRun = false;
      tolerance = toleranceBU;
      eps = epsBU;
    }
    else if(numErrors == 0 && needToIterate)
    {
      cerr << "Not enough space for support vectors\n";
      cerr << "Returning approximate solution\n";
      needToIterate = false;
    }
  }while(needToIterate);
  //cout << "***************************************************************\n";
  delete[] localErrCache;
  delete[] localLambdas;
  delete[] changedIndices;
}



  RavlN::XMLFactoryRegisterHandleConvertC<DesignSvmSmoLargeC, DesignSvmSmoC> g_registerXMLFactoryDesignSvmSmoLarge("RavlN::DesignSvmSmoLargeC");
  
  void linkDesignSvmSmoLarge()
{}


  //---------------------------------------------------------------------------
}
