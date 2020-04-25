// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
//! lib=RavlSVM

//! author="Alexey Kostin"

//#define RAVL_HAVE_EXCEPTIONS
#include "Ravl/String.hh"
#include "Ravl/PatternRec/CommonKernels.hh"
#include "Ravl/PatternRec/DesignClassifierSvmSmo.hh"
#include "Ravl/PatternRec/SvmClassifier.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/BinStream.hh"

using namespace RavlN;
using namespace RavlN;

void MyCallbackFunc(void *, int N)
{
  static int n = 0;
  if(N != n)
  {
    n = N;
    cout << n << "\t\t\r";
  }
}

void GenerateTrainingSet(SampleC<VectorC> &TrVec, SampleC<UIntT> &TrLab,
                         IntT NumPoints, IntT Dimension)
{
  int numPos;
  do
  {
    SArray1dC<RealT> norm(Dimension);
    for(int j = 0; j < Dimension; j++)
    {
      norm[j] = RealT(rand()) / RAND_MAX * 2. - 1.;
    }
    RealT b = RealT(rand()) / RAND_MAX * 2. - 1.;
    numPos = 0;
    
    SArray1dC<RealT> vec(Dimension);
    for(int i = 0; i < NumPoints; i++)
    {
      RealT d;
      do
      {
        d = b;
        for(int j = 0; j < Dimension; j++)
        {
          d += norm[j] * (vec[j] = RealT(rand()) / RAND_MAX * 2. - 1.);          
        }
      } while (d < 2e-1 && d > -2e-1);
      char label;
      if(d > 0)
      {
        label = 1;
        numPos++;
      }
      else
        label = 0;

      for(int j = 0; j < Dimension; j++)
      {
        TrVec[i][j] = vec[j];
      }
      TrLab[i] = label;
    }
  } while (numPos == 0 || numPos == NumPoints);
//  cout << "numpos:" << numPos << endl;
}

int main()
{
  try
  {
    DesignSvmSmoC dsvm(LinearKernelC(1), 1e3, 1e3);
    dsvm.SetCallbackFunc(MyCallbackFunc, NULL);

    //run test
    const IntT numPoints = 1000;
    const IntT dimension = 50;
    SampleC<VectorC> trVec(numPoints);
    SampleC<UIntT> trLab(numPoints);
    SArray1dC<IndexC> vecIDs(numPoints);
    for(int i = 0; i < numPoints; i++)
    {
      trVec.Append(VectorC(dimension));
      trLab.Append(0);
      vecIDs[i] = i;
    }
    IntT errors = 0;
    cout << endl;
    DateC startTime(true);
    cout << "Current time:" << startTime.Double() << endl;
    for(int i = 0; i < 10; i++)
    {
      cout << "i:" << i << endl;
      
      //generate new set
      GenerateTrainingSet(trVec, trLab, numPoints, dimension);

      //build rule
      cout << "creating rule\n";
      ClassifierC svm = dsvm.Apply(trVec, trLab);

      //check rule
      cout << "checking rule\n";
      for(int i = 0; i < numPoints; i++)
      {
        RealT d = svm.Classify(trVec[i]) == 0 ? -1 : 1;
        float s = d * trLab[i];
        if(s < 0)
        {
          errors++;
          cout << " Error, s=" << s << endl;
        }
      }
    }
    cout << endl;
    cout << "End time:" << DateC(true).Double() << endl;
    cout << "time:" << DateC(true).Double() - startTime.Double() << endl;
    cout << "Errors:" << errors << endl;
  }
  catch( ExceptionOperationFailedC &exc )
  {
    cout << "Exception!\n" << exc.Text() << endl;
  }
  catch( ... )
  {
    cout << "Unknown exception\n";
  }
  return 0;
}


