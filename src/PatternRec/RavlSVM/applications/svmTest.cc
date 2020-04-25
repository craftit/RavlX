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

void GenerateTrainingSet(SampleC<VectorC> &TrVec, SampleC<UIntT> &TrLab,
                         IntT numPoints)
{
  int numPos;
  do
  {
    RealT nx = RealT(rand()) / RAND_MAX * 2. - 1.;
    RealT ny = RealT(rand()) / RAND_MAX * 2. - 1.;
    RealT b = RealT(rand()) / RAND_MAX * 2. - 1.;
    numPos = 0;
    for(int i = 0; i < numPoints; i++)
    {
      RealT x, y, d;
      do
      {
        x = RealT(rand()) / RAND_MAX * 2. - 1.;
        y = RealT(rand()) / RAND_MAX * 2. - 1.;
        d = nx * x + ny * y + b;
      } while (d < 1e-2 && d > -1e-2);
      char label;
      if(d > 0)
      {
        label = 1;
        numPos++;
      }
      else
        label = 0;

      TrVec[i][0] = x;
      TrVec[i][1] = y;
      TrLab[i] = label;
    }
  } while (numPos == 0 || numPos == numPoints);
//  cout << "numpos:" << numPos << endl;
}

int main()
{
  try
  {
    //check linear kernel class
    cout << "Test of linear kernel\n";
    LinearKernelC linKern(1);
    RealT res = linKern(VectorC(1, 2), VectorC(1, 3));
    cout << "result:" << res << endl;

    //create training set
    SampleC<VectorC> tsv1(3);
    SampleC<UIntT> tsl1(3);
    SArray1dC<IndexC> otu1(3);

    tsv1.Append(VectorC(0, 1)); tsl1.Append(0); otu1[0] = IndexC(0);
    tsv1.Append(VectorC(0, 2)); tsl1.Append(0); otu1[1] = IndexC(1);
    tsv1.Append(VectorC(1, 0)); tsl1.Append(1); otu1[2] = IndexC(2);

    //create classifier
    DesignSvmSmoC dsvm(linKern,1e5,1e5);
    //DesignSvmSmoC dsvm(RBFKernelC(1), 1e5, 1e5);
    //DesignSvmSmoC dsvm(LinearKernelC(1));

    //test stream operators for design classifier
    cout << "Design classifier\n" << dsvm << endl;
    {
      BinOStreamC os("testDsvm.dat");
      if(!os.Stream())
      {
        cerr << "Failed to open output. \n";
        return __LINE__;
      }
      os << dsvm;
    }
    {
      BinIStreamC is("testDsvm.dat");
      if(!is.Stream())
      {
        cerr << "Failed to open output. \n";
        return __LINE__;
      }
      DesignSvmSmoC test(is);
      cout << "Test design classifier\n" << test << endl;
    }
    //test stream
    {
      OStreamC os("testDsvm.txt");
      if(!os)
      {
        cerr << "Failed to open output. \n";
        return __LINE__;
      }
      os << dsvm;
      if(!os)
      {
        cerr << "Failed output stream bad after write. \n";
        return __LINE__;
      }
    }
    {
      IStreamC is("testDsvm.txt");
      if(!is)
      {
        cerr << "Failed to open output. \n";
        return __LINE__;
      }
      DesignSvmSmoC test(is);
      cout << "Test design classifier\n" << test << endl;
    }

//    ClassifierC svm = dsvm.Apply(tsv1, tsl1, otu1);
    ClassifierC svm = dsvm.Apply(tsv1, tsl1);

    cout << svm.Classify(tsv1[0]) << endl;

    cout << "Classifier\n" << svm << endl;

    //test operations with binary stream
    {
      BinOStreamC os("test.dat");
      if(!os.Stream())
      {
        cerr << "Failed to open output. \n";
        return __LINE__;
      }
      os << svm;
    }
    {
      BinIStreamC is("test.dat");
      if(!is.Stream())
      {
        cerr << "Failed to open output. \n";
        return __LINE__;
      }
      ClassifierC testClassifier(is);
      cout << "Test classifier\n" << testClassifier << endl;
    }
    //test stream
    {
      OStreamC os("test.txt");
      if(!os)
      {
        cerr << "Failed to open output. \n";
        return __LINE__;
      }
      os << svm;
      if(!os)
      {
        cerr << "Failed output stream bad after write. \n";
        return __LINE__;
      }
    }
    {
      IStreamC is("test.txt");
      if(!is)
      {
        cerr << "Failed to open output. \n";
        return __LINE__;
      }
      ClassifierC testClassifier(is);
      cout << "Test classifier\n" << testClassifier << endl;
    }

    //run 2D test
    IntT numPoints = 20;
    SampleC<VectorC> trVec(numPoints);
    SampleC<UIntT> trLab(numPoints);
    SArray1dC<IndexC> vecIDs(numPoints);
    for(int i = 0; i < numPoints; i++)
    {
      trVec.Append(VectorC(2));
      trLab.Append(0);
      vecIDs[i] = i;
    }
    IntT errors = 0;
    cout << endl;
    DateC startTime(true);
    cout << "Current time:" << startTime.Double() << endl;
    for(int i = 0; i < 100; i++)
    {
//      cout << "\ri:" << i;

      //generate new set
      GenerateTrainingSet(trVec, trLab, numPoints);

      //build rule
//      cout << trLab << endl;
//      SvmClassifierC svm = dsvm.Apply(trVec, trLab, vecIDs);
      ClassifierC svm = dsvm.Apply(trVec, trLab);

      //check rule
      for(int i = 0; i < numPoints; i++)
      {
//        RealT d = svm.ClassifySvm(trVec[i]);
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


