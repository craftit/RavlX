// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
// $Id: oneClassLargeTest.cc 9564 2006-09-28 13:19:30Z alex $
//! lib=RavlSVM

//! author="Alexey Kostin"

#include "Ravl/String.hh"
#include "Ravl/PatternRec/CommonKernels.hh"
#include "Ravl/PatternRec/DesignOneClassLarge.hh"
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

void GenerateTrainingSet(SampleC<VectorC> &TrVec, SampleC<SByteT> &TrLab,
                         IntT numPoints)
{
  int numPos;
  do
  {
    RealT cx = RealT(rand()) / RAND_MAX * 2. - 1.;
    RealT cy = RealT(rand()) / RAND_MAX * 2. - 1.;
    RealT r = 0.8;//RealT(rand()) / RAND_MAX * 1.;
    //cout << "center:" << cx << "   " << cy << endl;
    //cout << "norm:" << cx * cx + cy * cy << endl;
    //cout << "radius:" << r << endl;
    numPos = 0;
    for(int i = 0; i < numPoints; i++)
    {
      RealT x, y, d;
      do
      {
        x = RealT(rand()) / RAND_MAX * 4. - 2.;
        y = RealT(rand()) / RAND_MAX * 4. - 2.;
        d = r - (cx - x) * (cx - x) - (cy - y) * (cy - y);
        //cout << "x:" << x << " \ty:" << y << " \td:" << d << endl;
      } while (d < 1e-1 && d > -1e-1);
      char label;
      if(d > 0)
      {
        label = 1;
        numPos++;
      }
      else
        label = -1;

      TrVec[i][0] = x;
      TrVec[i][1] = y;
      TrLab[i] = label;
    }
  } while (numPos == 0 /*|| numPos == numPoints*/);
  //cout << "numpos:" << numPos << endl;
}

int main()
{
  try
  {
    //create training set
    SampleC<VectorC> tsv1(3);
    SampleC<UIntT> tsl1(3);
    SArray1dC<IndexC> otu1(3);

    tsv1.Append(VectorC(0, 1)); tsl1.Append(1); otu1[0] = IndexC(0);
    tsv1.Append(VectorC(0, 2)); tsl1.Append(1); otu1[1] = IndexC(1);
    tsv1.Append(VectorC(1, 0)); tsl1.Append(1); otu1[2] = IndexC(2);

    //create classifier
    LinearKernelC linKern(1);
    DesignOneClassLargeC doc(linKern, 10, 1e5, 1e5, 1e-5, 1e-7, 1e-9);
    //DesignSvmSmoC dsvm(RBFKernelC(1), 1e5, 1e5);
    //DesignSvmSmoC dsvm(LinearKernelC(1));
    doc.SetCallbackFunc(MyCallbackFunc, NULL);


    cout << "Test of Apply method\n";
    ClassifierC oc = doc.Apply(tsv1, tsl1);

    cout << "Classifier\n" << oc << endl;


    cout << "run 2D test\n";
    IntT numPoints = 20000;
    SampleC<VectorC> trVec(numPoints);
    SampleC<SByteT> trLab(numPoints);
    SArray1dC<IndexC> vecIDs(numPoints);
    for(int i = 0; i < numPoints; i++)
    {
      trVec.Append(VectorC(2));
      trLab.Append(1);
      vecIDs[i] = i;
    }
    IntT errors = 0;
    cout << endl;
    DateC startTime(true);
    cout << "Current time:" << startTime.Double() << endl;
    for(int i = 0; i < 10; i++)
    {
//      cout << "\ri:" << i;

      //generate new set
      cout << "Generating test:" << i << endl;
      GenerateTrainingSet(trVec, trLab, numPoints);

      //build rule
      cout << "Training\n";
      if(i == -10)
        doc.Debug() = 1;
      else
        doc.Debug() = 0;
      ClassifierC cl = doc.Apply(trVec, trLab);
      OneClassC oc = *(static_cast<OneClassC*>(&cl));
      cout << "classifier radius:" << oc.Radius() << endl;
      if(oc.Radius() > 1.)
        cerr << "Classifier created radius bigger than expected\n";

      //check rule
      for(int i = 0; i < numPoints; i++)
      {
//        RealT d = svm.ClassifySvm(trVec[i]);
        RealT d = oc.Classify2(trVec[i]);
        float s = d * trLab[i];
        if(s < -0.001)
        {
          errors++;
          cerr << "Error:" << i << "  s=" << s << endl;
        }
      }
    }
    cout << endl;
    cout << "End time:" << DateC(true).Double() << endl;
    cout << "time:" << DateC(true).Double() - startTime.Double() << endl;
    cout << "Errors:" << errors << endl;
  }
  catch( ExceptionC &Exc )
  {
    cout << "Exception!\n" << Exc.Text() << endl;
  }
  catch( ... )
  {
    cout << "Unknown exception\n";
  }
  return 0;
}


