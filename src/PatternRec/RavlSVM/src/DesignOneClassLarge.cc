// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
// $Id: DesignOneClassLarge.cc 10146 2007-02-01 11:17:56Z alex $
//! lib=RavlSVM

//! author="Alexey Kostin"

#include "Ravl/PatternRec/DesignOneClassLarge.hh"
#include <string.h>
#include "Ravl/XMLFactoryRegister.hh"

namespace RavlN
{
using namespace RavlN;

#define DOUBLE_QNAN 999e100
#define IS_NAN(_X) ((_X) == DOUBLE_QNAN)
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//! constructor.
DesignOneClassLargeBodyC::DesignOneClassLargeBodyC(const KernelFunctionC &KernelFunction,
                                                   IntT MaxNumSv,
                                                   RealT Penalty1, RealT Penalty2,
                                                   RealT Tolerance, RealT Eps,
                                                   RealT LambdaThreshold)
  : DesignOneClassBodyC(KernelFunction, Penalty1, Penalty2,
                        Tolerance, Eps, LambdaThreshold)
{
  InitMembers();

  maxNumSv = MaxNumSv;
  //cout << "c1:" << c1 << "   c2:" << c2 << endl;
  //cout << "tol:" << tolerance << "   eps:" << eps << endl;
  //cout << "lt:" << lambdaThreshold << endl;
}

//---------------------------------------------------------------------------
DesignOneClassLargeBodyC::DesignOneClassLargeBodyC(const XMLFactoryContextC & factory)
  : DesignOneClassBodyC(factory),
    maxNumSv(factory.AttributeReal("max_num_sv", 10000))
{
  // desired ones from INIT need to refactor
  objectsToUseLarge = NULL;
  objectsToUseLargeSize = 0;
  kernelCacheIndices = NULL;
  localSCache = NULL;
  localErrCache = NULL;
  localLambdas = NULL;
  changedIndices = NULL;
}

  //---------------------------------------------------------------------------
//: Load from stream.
DesignOneClassLargeBodyC::DesignOneClassLargeBodyC(istream& Strm)
                         :DesignOneClassBodyC(Strm)
{
  InitMembers();

  Strm >> maxNumSv;
}
//---------------------------------------------------------------------------
//: Load from binary stream.
DesignOneClassLargeBodyC::DesignOneClassLargeBodyC(BinIStreamC& Strm)
                         :DesignOneClassBodyC(Strm)
{
  InitMembers();

  Strm >> maxNumSv;
}
//---------------------------------------------------------------------------
//: Writes object to stream
bool DesignOneClassLargeBodyC::Save(ostream &Strm) const
{
  if(!DesignOneClassBodyC::Save(Strm))
    return false;
  Strm << ' ' << maxNumSv;
  return true;
}
//---------------------------------------------------------------------------
//: Writes object to stream
bool DesignOneClassLargeBodyC::Save(BinOStreamC& Strm) const
{
  if(!DesignOneClassBodyC::Save(Strm))
    return false;
  Strm << ' ' << maxNumSv;
  return true;
}
//---------------------------------------------------------------------------
// Destructor.
DesignOneClassLargeBodyC::~DesignOneClassLargeBodyC()
{
  //cout << "DesignOneClassLargeBodyC::~DesignOneClassLargeBodyC\n";
  ReleaseMemory();
  //cout << "ok\n";
}
//---------------------------------------------------------------------------
Classifier2C DesignOneClassLargeBodyC::Apply(const SampleC<VectorC> &TrainingSetVectors,
                                             const SampleC<SByteT> &TrainingSetLabels)
{
  //cout << "DesignOneClassLargeBodyC::Apply\n";
  AllocateMemory(TrainingSetVectors.Size(), TrainingSetVectors.Size());
  //cout << "ok\n";

  //copy training set vectors
  trainingSetVectors = TrainingSetVectors;

  //copy trainingSetLabels
  for(int i = 0; i < trainSetSize; i++)
  {
    trainingSetLabels[i] = TrainingSetLabels[i];
  }

  //cout << "aaa\n";
  /*for(int i = 0; i < trainSetSize; i++)
  {
    cout << "i:" << i << " " << TrainingSetVectors[i] << "  \tlab:" << int(trainingSetLabels[i]) << endl;
  }*/

  //create objectsToUse
  for(int i = 0; i < objectsToUseLargeSize; i++)
  {
    objectsToUseLarge[i] = i;
  }

  //clean buffers and vars
  Prepare();

  //find lagrangian multipliers
  CalcLambdas(false);

  return GetClassifier();
}
//---------------------------------------------------------------------------
ClassifierC DesignOneClassLargeBodyC::Apply(const SampleC<VectorC> &TrainingSetVectors,
                                          const SampleC<UIntT> &TrainingSetLabels)
{
  AllocateMemory(TrainingSetVectors.Size(), TrainingSetVectors.Size());

  //copy training set vectors
  trainingSetVectors = TrainingSetVectors;

  //copy trainingSetLabels
  for(int i = 0; i < trainSetSize; i++)
  {
    trainingSetLabels[i] = TrainingSetLabels[i] == 0 ? -1 : 1;
  }

  //create objectsToUse
  for(int i = 0; i < objectsToUseLargeSize; i++)
  {
    objectsToUseLarge[i] = i;
  }

  //clean buffers and vars
  Prepare();

  //find lagrangian multipliers
  CalcLambdas(false);

  return GetClassifier();
}
//---------------------------------------------------------------------------
Classifier2C DesignOneClassLargeBodyC::Apply(const SampleC<VectorC> &TrainingSetVectors,
                                             const SampleC<SByteT> &TrainingSetLabels,
                                             const SArray1dC<IndexC> &ObjectsToUse)
{
  //cout << "DesignOneClassLargeBodyC::Apply\n";
  AllocateMemory(ObjectsToUse.Size(), TrainingSetVectors.Size());
  //cout << "ok\n";

  //copy training set vectors
  trainingSetVectors = TrainingSetVectors;

  //copy trainingSetLabels
  for(int i = 0; i < trainSetSize; i++)
  {
    trainingSetLabels[i] = TrainingSetLabels[i];
  }

  //cout << "aaa\n";
  /*for(int i = 0; i < trainSetSize; i++)
  {
  cout << "i:" << i << " " << TrainingSetVectors[i] << "  \tlab:" << int(trainingSetLabels[i]) << endl;
}*/

  //create objectsToUse
  for(int i = 0; i < objectsToUseLargeSize; i++)
  {
    objectsToUseLarge[i] = ObjectsToUse[i].V();
  }

  //clean buffers and vars
  Prepare();

  //find lagrangian multipliers
  CalcLambdas(false);

  return GetClassifier();
}
//---------------------------------------------------------------------------
ClassifierC DesignOneClassLargeBodyC::Apply(const SampleC<VectorC> &TrainingSetVectors,
                                            const SampleC<UIntT> &TrainingSetLabels,
                                            const SArray1dC<IndexC> &ObjectsToUse)
{
  AllocateMemory(ObjectsToUse.Size(), TrainingSetVectors.Size());

  //copy training set vectors
  trainingSetVectors = TrainingSetVectors;

  //copy trainingSetLabels
  for(int i = 0; i < trainSetSize; i++)
  {
    trainingSetLabels[i] = TrainingSetLabels[i] == 0 ? -1 : 1;
  }

  //create objectsToUse
  for(int i = 0; i < objectsToUseLargeSize; i++)
  {
    objectsToUseLarge[i] = ObjectsToUse[i].V();
  }

  //clean buffers and vars
  Prepare();

  //find lagrangian multipliers
  CalcLambdas(false);

  return GetClassifier();
}
//---------------------------------------------------------------------------
void DesignOneClassLargeBodyC::Prepare()
{
  //cout << "DesignOneClassLargeBodyC::Prepare\n";

  //clear lambdas array
  memset(lambdas, 0, sizeof(double) * trainSetSize);
  memset(localLambdas, 0, sizeof(double) * trainSetSize);

  //find first inlier and sel lambda to 1
  bool lambdaSet = false;
  for(int i = 0; i < objectsToUseLargeSize; i++)
  {
    const int firstID = objectsToUseLarge[i];
    if(trainingSetLabels[firstID] > 0)
    {
      //cout << "firstID:" << firstID << endl;
      lambdas[firstID] = 1.;
      lambdaSet = true;
      break;
    }
  }
  if(!lambdaSet)
    throw ExceptionC("DesignOneClassLargeBodyC::Prepare:"
                     "No inliers in training set!\n");

  //clear kernel cache
  SizeT kernelCacheSize = (SizeT(trainSubsetSize + 1) *
                          SizeT(trainSubsetSize)) / 2;
  memset(kernelCache, 0, sizeof(float) * kernelCacheSize);
}
//---------------------------------------------------------------------------
RealT DesignOneClassLargeBodyC::GetKernelValue(int I1, int I2) const
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
    cacheVal = (float)kernelFunction(trainingSetVectors[I1], trainingSetVectors[I2]);
    *elAdr = cacheVal;
//    cout << "new kernel (" << I1 << ", " << I2 << ") = " << cacheVal << endl;
  }
  return cacheVal;
}
//---------------------------------------------------------------------------
void DesignOneClassLargeBodyC::CalcLambdas(bool DoFinal)
{
  //cout << "DesignOneClassLargeBodyC::CalcLambdas\n";
  //cout << "c1:" << c1 << "   c2:" << c2 << endl;

  //cout << "trainSetSize:" << trainSetSize << endl;
  for(int i = 0; i < trainSetSize; i++)
  {
    localSCache[i] = DOUBLE_QNAN;
    localErrCache[i] = DOUBLE_QNAN;
    localLambdas[i] = lambdas[i];
  }

  //****************** create training subset for the first iteration **********
  //initialise objectsToUse array, it must have objects of first class
  // empty subset
  for(int i = 0; i < trainSetSize; i++)
    kernelCacheIndices[i] = -1;

  // copy support vectors first (if start form known lambdas)
  int i = 0;
  bool needFirstClass = true;
  for(int j = 0; j < objectsToUseLargeSize && i < trainSubsetSize; j++)
  {
    const int objectID = objectsToUseLarge[j];
    if(lambdas[objectID] != 0.)
    {
      //cout << "j:" << j << "   lambda:" << lambdas[objectID] << endl;
      kernelCacheIndices[(objectsToUse[i] = objectID)] = i;
      i++;
      if(needFirstClass && trainingSetLabels[objectID] > 0)
      {
        needFirstClass = false;
      }
    }
  }
  //if(i == numTrainObjects)
  //{
  //  cerr << "Too many support vectors for first iteration\n";
  //  return;
  //}
  //make sure two classes in subset
  //cout << "ii:" << i << endl;
  if(needFirstClass)
  {
    for(int j = 0; j < objectsToUseLargeSize && i < trainSubsetSize; j++)
    {
      const int objectID = objectsToUseLarge[j];
      if(kernelCacheIndices[objectID] < 0) //object not in subset
      {
        if(needFirstClass && trainingSetLabels[objectID] > 0)
        {
          kernelCacheIndices[(objectsToUse[i] = objectID)] = i;
          i++;
          needFirstClass = false;
          break;
        }
      }
    }
  }
  if(needFirstClass)
  {
    cout << "No objects of first class\n";
    return;
  }
  //cout << "ii:" << i << endl;
  //fill the rest of training subset
  for(int j = 0; j < objectsToUseLargeSize && i < trainSubsetSize; j++)
  {
    const int objectID = objectsToUseLarge[j];
    if(kernelCacheIndices[objectID] < 0) //object not in subset
    {
      kernelCacheIndices[(objectsToUse[i] = objectID)] = i;
      i++;
    }
  }

  if(i < trainSubsetSize) //must never happen
  {
    cerr << "Internal error (i < numTrainObjects)!!!!!!\n";
    return;
  }

  /*cout << "trainSubsetSize:" << trainSubsetSize << endl;
  for(int i = 0; i < trainSubsetSize; i++)
  {
    cout << i << ":" << objectsToUse[i]
        << "  lab:" << int(trainingSetLabels[objectsToUse[i]])
        << "  ki:"  << kernelCacheIndices[objectsToUse[i]]
        << "   " << trainingSetVectors[objectsToUse[i]][0]
        << "   " << trainingSetVectors[objectsToUse[i]][1]
        << endl;
  }*/

  //main process
  long double localRadius, localNorm;
  localRadius = CalculateRadius(localNorm);

  RealT toleranceBU = toleranceR;
  RealT epsBU = eps;
  RealT toleranceR1 = toleranceR;
  bool firstRun = false;
  if(!DoFinal)
  {
    toleranceR = 0.1;
    toleranceR1 = 0.4;
    eps = 0.1;
    firstRun = true;
  }

  int maxErrID = 0;
  double maxError = -1e100;

  //main smo loop
  bool needToIterate;
  int errorRef = 0;
  //int errorID = objectsToUse[0] + 1; // make sure errorID-1 is used
  //if(errorID >= trainSetSize)
  //  errorID = 0;
  int freeRef = 0;
  do
  {
    //memcpy(errorCache, localErrCache, trainSetSize * sizeof(double));
    //radius = CalculateRadius(norm);
    //localRadius = radius;
    //localNorm = norm;
    radius = localRadius;
    norm = localNorm;

    if(maxError < -radius * toleranceR)
    {

      //cout << "radius:" << radius << endl;
      //cout << "toleranceA:" << toleranceA << endl;
      //cout << "toleranceR:" << toleranceR << endl;
      UpdateErrorCache();//to remove accumulated error of small numbers summation
      /*for(int i = 0; i < trainSubsetSize; i++)
      {
        cout << i << ":" << objectsToUse[i]
            << "  lab:" << int(trainingSetLabels[objectsToUse[i]])
            << "  ki:"  << kernelCacheIndices[objectsToUse[i]]
            << "   " << trainingSetVectors[objectsToUse[i]][0]
            << "   " << trainingSetVectors[objectsToUse[i]][1]
            << endl;
      }*/
      int numChanged = 0;
      int examineAll = 1;

      while(numChanged > 0 || examineAll)
      {
        //toleranceA = sqrt(radius) * toleranceR;
        toleranceA = radius * toleranceR;

        numChanged = 0;
        if(examineAll)
        {
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
      /*cout << "----------------------------------\n";
      for(int i = 0; i < trainSubsetSize; i++)
      {
        cout << i << ":" << objectsToUse[i]
            << "  lab:" << int(trainingSetLabels[objectsToUse[i]])
            << "  ki:" << kernelCacheIndices[objectsToUse[i]]
            << "  lm:" << lambdas[objectsToUse[i]] << endl;
      }*/

      //update local err cache
      int numChangedIndices = 0;
      int numSV = 0;
      for(int k = 0; k < trainSubsetSize; k++)
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
      //cout << "num changed lambdas:" << numChangedIndices << "  numSV:" << numSV << endl;

      //update local error cache and local s cache
      const double dErr = (radius - localRadius) - (norm - localNorm);
      //cout << "radius:" << radius << "  \t" << localRadius << "  \t" << radius - localRadius << endl;
      //cout << "norm  :" << norm   << "  \t" << localNorm   << "  \t" << norm - localNorm << endl;
      //cout << "dErr:" << dErr << endl;

      for(int i = 0; i < objectsToUseLargeSize; i++)
      {
        const int objectID1 = objectsToUseLarge[i];
        if(kernelCacheIndices[objectID1] == -1) //can not use errorCache
        {
          if(numSV <= numChangedIndices) //easy will be to calculate from scratch
          {
            localSCache[objectID1] = DOUBLE_QNAN;
            localErrCache[objectID1] = DOUBLE_QNAN;
          }
          else //update
          {
            double s1 = localSCache[objectID1];
            double e1 = localErrCache[objectID1];
            if(IS_NAN(s1) || IS_NAN(e1))
            {
              localSCache[objectID1] = DOUBLE_QNAN;
              localErrCache[objectID1] = DOUBLE_QNAN;
            }
            else
            {
              for(int k = 0; k < numChangedIndices; k++)
              {
                const int objectID2 = changedIndices[k];
                const double lambda = localLambdas[objectID2];
                if(lambda != 0.)
                {
                  const double ds1 = lambda * trainingSetLabels[objectID2] *
                                     kernelFunction(trainingSetVectors[objectID1],
                                                    trainingSetVectors[objectID2]);
                  s1 -= ds1;
                  e1 -= 2. * ds1;
                }
              }
              localSCache[objectID1] = s1;
              localErrCache[objectID1] = e1 + dErr;
            }
          }
        }
        else // take values from subset cache
        {
          localSCache[objectID1] = sCache[objectID1];
          localErrCache[objectID1] = errorCache[objectID1];
          /*if(errorCache[i] * trainingSetLabels[i] < -toleranceA)
          {
            cout << "i:" << i
                << " \terr:" << errorCache[i] * trainingSetLabels[i]
                << " \ttol:" << toleranceA
                << " \tlambda:" << lambdas[i] << endl;
          }*/
        }
      }

      /*cout << "prevMaxErrID:" << maxErrID
           << "  newErr:" << errorCache[maxErrID] * trainingSetLabels[maxErrID]
           << "  lambda:" << lambdas[maxErrID]
           << "  label:" << int(trainingSetLabels[maxErrID])
           << "  ki:" << kernelCacheIndices[maxErrID] << endl;

      {
        cout << "radius:" << radius << "   " << localRadius << endl;
        cout << "norm  :" << norm << "   " << localNorm << endl;
        double s1 = 0.;
        for(int k = 0; k < trainSubsetSize; k++)
        {
          const int objectID = objectsToUse[k];
          double lambda = lambdas[objectID];
          if(lambda > 0.)
            s1 += lambda * trainingSetLabels[objectID] *
                  kernelFunction(trainingSetVectors[maxErrID],
                                 trainingSetVectors[objectID]);
        }
        double e1 = radius + 2. * s1 - norm -
                    kernelFunction(trainingSetVectors[maxErrID],
                                   trainingSetVectors[maxErrID]);
        const double y1 = trainingSetLabels[maxErrID];
        const double r1 = y1 * e1;
        cout << "calculated new error:" << r1 << "  s1:" << s1 << endl;
      }*/


      //update local lambdas and vars
      memcpy(localLambdas, lambdas, trainSetSize * sizeof(double));
      //localRadius = CalculateRadius(localNorm);
      localRadius = radius;
      localNorm = norm;

      /*cout << "i  \tlambda  \tlocSCache  \tlocErrCache \t ki\n";
      for(int i = 0; i < trainSetSize; i++)
      {
        cout << i << "  \t" << localLambdas[i] << "  \t"
            << localSCache[i] << "  \t" << localErrCache[i] << "  \t"
            << kernelCacheIndices[i] << endl;
      }*/
    }

    //cout << "newRadius:" << localRadius << endl;
    //RealT toleranceA1 = toleranceR1 * sqrt(localRadius);
    RealT toleranceA1 = toleranceR1 * localRadius;
    //cout << "toleranceA1:" << toleranceA1 << endl;
    //cout << "toleranceR1:" << toleranceR1 << endl;

    int prevErrorRef = errorRef - 1;
    if(prevErrorRef < 0)
      prevErrorRef = objectsToUseLargeSize - 1;

    int lastFreeRef = freeRef;
    /*int lastFreeRef = freeRef - 1;
    if(lastFreeRef < 0)
      lastFreeRef = trainSubsetSize - 1;*/

    maxError = 0.;

    int numErrors = 0;
    needToIterate = false;

    do
    {
      //cout << "errorID:" << errorID << "    freeRef:" << freeRef << endl;
      //find object with big error
      bool errorFound = false;
      while(errorRef != prevErrorRef)
      {
        const int errorID = objectsToUseLarge[errorRef];
        //cout << "errorID:" << errorID << "   ki:" << kernelCacheIndices[errorID] << endl;
        if(kernelCacheIndices[errorID] == -1) //object not in use
        {
          double e1 = localErrCache[errorID];
          if(IS_NAN(e1))
          {
            long double s1 = localSCache[errorID];
            if(IS_NAN(s1))
            {
              s1 = 0.;
              for(int k = 0; k < trainSubsetSize; k++)
              {
                const int objectID = objectsToUse[k];
                double lambda = lambdas[objectID];
                if(lambda > 0.)
                  s1 += lambda * trainingSetLabels[objectID] *
                        kernelFunction(trainingSetVectors[errorID],
                                       trainingSetVectors[objectID]);
              }
              localSCache[errorID] = s1;
            }
            e1 = localRadius + 2. * s1 - localNorm -
                 kernelFunction(trainingSetVectors[errorID],
                                trainingSetVectors[errorID]);
            localErrCache[errorID] = e1;
          }
          const double y1 = trainingSetLabels[errorID];
          const double r1 = y1 * e1;
          //cout << "errorID:" << errorID << "  r1:" << r1 << endl;
          if(r1 < -toleranceA1)
          {
            errorFound = true;
            if(r1 < maxError)
            {
              maxError = r1;
              maxErrID = errorID;
              /*cout << "mErrId:" << maxErrID
                  << "  sCacheUsed:" << sCacheUsed
                  << "  eCacheUsed:" << eCacheUsed
                  << "  sCache:" << sCache[maxErrID]
                  << "  localSCache:" << localSCache[maxErrID] << endl
                  << "  eCache:" << errorCache[maxErrID]
                  << "  localECache:" << localErrCache[maxErrID]
                  << "  localNorm:" << localNorm
                  << "  localRadius:" << localRadius << endl;*/
            }

            //cout << "errId:" << errorID << " \tprevErrorID:" << prevErrorID
            //     << " \tr1:" << r1 << " \ttol:" << toleranceA1
            //     << " \tl:" << localLambdas[errorID] << endl;
            break;
          }
        }
        errorRef++;
        if(errorRef >= objectsToUseLargeSize)
          errorRef = 0;
      }
      //cout << "errorID:" << errorID << "    errorFound:" << errorFound << endl;

      if(errorFound)
      {
        needToIterate = true;

        //find free slot for object
        bool freeFound = false;
        //cout << "lastFreeRef:" << lastFreeRef << endl;
        do
        {
          //cout << "freeRef:" << freeRef << endl;
          const int objectID = objectsToUse[freeRef];
          const double lambda = lambdas[objectID];
          //cout << "lambda:" << lambda << endl;
          //cout << "error:" << trainingSetLabels[objectID] * errorCache[objectID] << endl;
          if(lambda == 0. &&
             (trainingSetLabels[objectID] * errorCache[objectID] > 0.))
          {
            freeFound = true;
            break;
          }
          freeRef++;
          if(freeRef >= trainSubsetSize)
            freeRef = 0;
        }while(freeRef != lastFreeRef);

        //cout << "freeRef:" << freeRef << "    freeFound:" << freeFound << endl;
        if(freeFound)
        {
          const int freeID  = objectsToUse[freeRef];
          const int errorID = objectsToUseLarge[errorRef];
          errorCache[freeID] = DOUBLE_QNAN;
          kernelCacheIndices[errorID] = kernelCacheIndices[freeID];
          kernelCacheIndices[freeID] = -1;
          objectsToUse[freeRef] = errorID;
          numErrors++;

          freeRef++;
          if(freeRef >= trainSubsetSize)
            freeRef = 0;

          //FIXME cleaning cache must be here!!!
        }

        if(errorFound)
        {
          errorRef++;
          if(errorRef >= objectsToUseLargeSize)
            errorRef = 0;
        }
      }
    } while((errorRef != prevErrorRef) && (freeRef != lastFreeRef));
    /*cout << "num changes:" << numErrors
         << "  last errRef:" << errorRef
         << "  trSetSize:" << objectsToUseLargeSize
         << "  lastFreeRef:" << freeRef
         << "  numTrObjects:" << trainSubsetSize
         << "  maxError:" << maxError
         << "  tolerance:" << radius * toleranceR << endl; */
    //cout << "maxErrID:" << maxErrID << endl;
    //slow but does the job
    SizeT kernelCacheSize = (SizeT(trainSubsetSize + 1) *
                            SizeT(trainSubsetSize)) / 2;
    memset(kernelCache, 0, sizeof(float) * kernelCacheSize);

    if((!needToIterate || numErrors == 0) && firstRun)
    {
      //cout << "Doing final processing\n";
      needToIterate = true;
      firstRun = false;
      toleranceR = toleranceBU;
      toleranceR1 = toleranceR;
      eps = epsBU;
    }
    else if(numErrors == 0 && needToIterate)
    {
      cout << "Not enough space for support vectors\n";
      cout << "Returning approximate solution\n";
      needToIterate = false;
    }

    /*if(!needToIterate) //do final check
    {
      for(int i = 0; i < trainSubsetSize; i++)
      {
        const int objectID = objectsToUse[i];
        const double s = CalcS(objectID);
        if(fabs(s - sCache[objectID]) > 1e-3)
        {
          cout << objectID << " error in sCache:" << s << " != "
               << sCache[objectID] << endl;
        }

        const double f = radius - GetKernelValue(objectID, objectID) + 2. * s - norm;
        if(fabs(f - errorCache[objectID]) > 1e-3)
        {
          cout << objectID << " error in eCache:" << f << " != "
               << errorCache[objectID] << endl;
        }

        if(trainingSetLabels[objectID] * f < -toleranceA)
        {
          cout << objectID << " error in classification:"
               << trainingSetLabels[objectID] * f << endl;
        }
      }
    }*/
  }while(needToIterate);
  //cout << "***************************************************************\n";
}
//---------------------------------------------------------------------------
void DesignOneClassLargeBodyC::InitMembers()
{
  //cout << "DesignOneClassLargeBodyC::InitMembers\n";
  maxNumSv = 0;
  objectsToUseLarge = NULL;
  objectsToUseLargeSize = 0;
  kernelCacheIndices = NULL;
  localSCache = NULL;
  localErrCache = NULL;
  localLambdas = NULL;
  changedIndices = NULL;
}
//---------------------------------------------------------------------------
void DesignOneClassLargeBodyC::AllocateMemory(int NewObjectsToUseSize,
                                              int NewTrainSetSize)
{
  int newTrainSubsetSize = min(NewObjectsToUseSize, maxNumSv);
  //cout << "newTrainSubsetSize:" << newTrainSubsetSize << endl;
  //cout << "NewTrainSetSize:" << NewTrainSetSize << endl;

  if((trainSetSize != NewTrainSetSize && trainSetSize != 0) ||
     (trainSubsetSize != newTrainSubsetSize && trainSubsetSize != 0) ||
     (objectsToUseLargeSize != NewObjectsToUseSize && objectsToUseLargeSize != 0))
  {
    ReleaseMemory();
  }

  if(trainSetSize == 0 || trainSubsetSize == 0 || objectsToUseLargeSize == 0)
  {
    errorCache         = new double[NewTrainSetSize];
    sCache             = new double[NewTrainSetSize];
    lambdas            = new double[NewTrainSetSize];
    trainingSetLabels  = new char[NewTrainSetSize];
    objectsToUse       = new int[newTrainSubsetSize];

    localSCache        = new double[NewTrainSetSize];
    localErrCache      = new double[NewTrainSetSize];
    localLambdas       = new double[NewTrainSetSize];
    changedIndices     = new int[newTrainSubsetSize];
    objectsToUseLarge  = new int[NewObjectsToUseSize];
    kernelCacheIndices = new int[NewTrainSetSize];

    SizeT kernelCacheSize = (SizeT(newTrainSubsetSize + 1) *
                            SizeT(newTrainSubsetSize)) / 2;
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
     trainingSetLabels == NULL || objectsToUse == NULL ||
     localSCache == NULL || localErrCache == NULL || localLambdas == NULL ||
     changedIndices == NULL || objectsToUseLarge == NULL ||
     kernelCacheIndices == NULL || kernelCache == NULL)
    throw ExceptionC("DesignOneClassLargeBodyC::AllocateMemory:"
                     "Can't allocate memory for internal buffers");

  //cout << "bbb\n";
  trainSubsetSize = newTrainSubsetSize;
  trainSetSize = NewTrainSetSize;
  objectsToUseLargeSize = NewObjectsToUseSize;
}
//---------------------------------------------------------------------------
void DesignOneClassLargeBodyC::ReleaseMemory()
{
  /*cout << "DesignOneClassLargeBodyC::ReleaseMemory\n";
  cout << "errorCache:" << SizeT(errorCache) << endl;
  cout << "sCache:" << SizeT(sCache) << endl;
  cout << "lambdas:" << SizeT(lambdas) << endl;
  cout << "trainingSetLabels:" << SizeT(trainingSetLabels) << endl;
  cout << "objectsToUse:" << SizeT(objectsToUse) << endl;

  cout << "localSCache:" << SizeT(localSCache) << endl;
  cout << "localErrCache:" << SizeT(localErrCache) << endl;
  cout << "localLambdas:" << SizeT(localLambdas) << endl;
  cout << "changedIndices:" << SizeT(changedIndices) << endl;
  cout << "objectsToUseLarge:" << SizeT(objectsToUseLarge) << endl;
  cout << "kernelCacheIndices:" << SizeT(kernelCacheIndices) << endl;

  cout << "kernelCache:" << SizeT(kernelCache) << endl;*/

  delete[] errorCache;         errorCache = NULL;
  delete[] sCache;             sCache = NULL;
  delete[] lambdas;            lambdas = NULL;
  delete[] trainingSetLabels;  trainingSetLabels = NULL;
  delete[] objectsToUse;       objectsToUse = NULL;

  delete[] localSCache;        localSCache = NULL;
  delete[] localErrCache;      localErrCache = NULL;
  delete[] localLambdas;       localLambdas = NULL;
  delete[] changedIndices;     changedIndices = NULL;
  delete[] objectsToUseLarge;  objectsToUseLarge = NULL;
  delete[] kernelCacheIndices; kernelCacheIndices = NULL;
  delete[] kernelCache;        kernelCache = NULL;
  trainSetSize = 0;
  trainSubsetSize = 0;
  objectsToUseLargeSize = 0;
  //cout << "DesignOneClassLargeBodyC::ReleaseMemory:OK\n";
}

  RavlN::XMLFactoryRegisterHandleConvertC<DesignOneClassLargeC, DesignOneClassC> g_registerXMLFactoryDesignOneClassLarge("RavlN::DesignOneClassLargeC");
  
  void linkDesignOneClassLarge()
{}


  //---------------------------------------------------------------------------
}
