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
#include "Ravl/Option.hh"

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

void ReadData(StringC &FileName, SampleC<VectorC> &Vectors, 
              SampleC<UIntT> &Labels)
{
  IStreamC is(FileName);
  if(!is)
  {
    cerr << "Failed to open file:" << FileName << endl;
    throw ExceptionOperationFailedC("Can't open file.");
  }

  is >> Vectors;
  is >> Labels;

  cout << "Num vectors:" << Vectors.Size() << endl;
  cout << "Num labels :" << Labels.Size() << endl;

//  cout << Vectors;
//  cout << Labels;
}


int main(int argc, char **argv)
{
  try
  {
    //parse command line parameters
    OptionC option(argc, argv);
    StringC trainingSetFileName = option.String("tr", "train.dset", 
                                                "Training set file name");
    StringC testSetFileName     = option.String("te", "test.dset", 
                                                "Test set file name");
    IntT kernelType = option.Int("kt", 0, "Kernel type (0 - linear; "
                                 "1 - pilynimial; 2 - RBF)");
    RealT par1 = option.Real("par1", 1, "Kernel parameter 1 (Scale for "
                             "linear and polynomial kernels; Gamma for RBF)");
    RealT par2 = option.Real("par2", 0, "Kernel parameter 2 (B for polynomial "
                             "kernel)");
    RealT par3 = option.Real("par3", 0, "Kernel parameter 3 (Power for "
                             "polynomial kernel)");
    RealT c1 = option.Real("c1", 1000, "Penalty for misclassifieation of "
                           "1st class objects");
    RealT c2 = option.Real("c2", 1000, "Penalty for misclassifieation of "
                           "2nd class objects");
    option.Check();

    //print parameters
    cout << "c1 = " << c1 << "\t  c2 = " << c2 << endl;
    cout << "Training set file:" << trainingSetFileName << endl;
    cout << "Test set file    :" << testSetFileName << endl;
    switch(kernelType)
    {
    case 0:
      cout << "Linear kernel: K(X1, X2) = <X1, X2> * " << par1 << endl;
      break;
    case 1:
      cout << "Polynomial kernel: K(X1, X2) = (<X1, X2> * " << par1 << 
              " + " << par2 << ") ^ " << par3 << endl;
      break;
    case 2:
      cout << "RBF kernel: K(X1, X2) = exp(- {X1 - X2} ^ 2 / " << par1 << 
              ")" << endl;
      break;
    default:
      cout << "Kernel type:" << kernelType << endl;
      throw ExceptionOperationFailedC("Bad kernel type");
    }

    //read training set data
    SampleC<VectorC> trainingSet;
    SampleC<UIntT> trainingSetLabels;

    cout << "Reading training set\n";
    ReadData(trainingSetFileName, trainingSet, trainingSetLabels);
    cout << "Done\n";

    //read test set data
    SampleC<VectorC> testSet;
    SampleC<UIntT> testSetLabels;

    cout << "Reading test set\n";
    ReadData(testSetFileName, testSet, testSetLabels);
    cout << "Done\n";

    //create kernel
    KernelFunctionC kernel;

    switch(kernelType)
    {
    case 0:
      kernel = LinearKernelC(par1);
      break;
    case 1:
      kernel = PolynomialKernelC(par3, par1, par2);
      break;
    case 2:
      kernel = RBFKernelC(par1);
      break;
    }
    
    //create classifier
    DateC startTime(true);
    cout << "Current time:" << startTime.Double() << endl;

    DesignSvmSmoC dsvm(kernel, c1, c2);
    dsvm.SetCallbackFunc(MyCallbackFunc, NULL);

    cout << "Training...\n";
    ClassifierC svm = dsvm.Apply(trainingSet, trainingSetLabels);

    cout << "End time:" << DateC(true).Double() << endl;
    cout << "time:" << DateC(true).Double() - startTime.Double() << endl;

    cout << "Number of suppot vectors: " << dsvm.NumSupportVectors() << endl;
//    cout << svm << endl;
    
    //test
    cout << "Testing...\n";
    UIntT fa = 0, fr = 0;
    UIntT numTests = testSet.Size();
    UIntT numPos = 0, numNeg = 0;
    for(UIntT i = 0; i < numTests; i++)
    {
      UIntT cls = svm.Classify(testSet[i]);
      UIntT label = testSetLabels[i];
      if(label == 0)
      {
        numNeg++;
        if(cls != 0)
        {
          fa++;
        }
      }
      else
      {
        numPos++;
        if(cls == 0)
        {
          fr++;
        }
      }
    }
    cout << "Total test objects  : " << numTests << endl;
    cout << "False acceptances   : " << fa << endl;
    cout << "False rejections    : " << fr << endl;
    cout << "Total errors        : " << fa + fr << endl;
    cout << "Number of clients   : " << numPos << endl;    
    cout << "Number of impostors : " << numNeg << endl;    

    cout << "FAR : " << float(fa) / numNeg << endl;
    cout << "FRR : " << float(fr) / numPos << endl;
    cout << "TER : " << float(fa) / numNeg + float(fr) / numPos << endl;
  }
  catch( ExceptionOperationFailedC &exc )
  {
    cout << "Exception!\n" << exc.Text() << endl;
  }
  catch( ExceptionC &exc )
  {
    cout << "Exception!\n" << exc.Text() << endl;
  }
  catch( ... )
  {
    cout << "Unknown exception\n";
  }
  return 0;
}


