/*****************************************************************************
 * This file is used in conjunction with RAVL, Recognition And Vision Library
 * Copyright (C) 2006, OmniPerception Ltd.
 * This code may be redistributed under the terms of the GNU Lesser
 * General Public License (LGPL). See the gpl.licence file for details or
 * see http://www.gnu.org/copyleft/lesser.html
 *****************************************************************************/
//! lib=RavlSVM
// file-header-ends-here

//! author="Alexey Kostin"

#include "Ravl/String.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/Option.hh"
#include "Ravl/IO.hh"
#include "Ravl/Image/ImgIO.hh"

#include "Ravl/PatternRec/CommonKernels.hh"
#include "Ravl/PatternRec/DesignClassifierSvmSmo.hh"
#include "Ravl/PatternRec/DesignSvmSmoLarge.hh"
#include "Ravl/PatternRec/SvmLinearClassifier.hh"
#include "Ravl/PatternRec/SvmQuadraticClassifier.hh"
#include "Ravl/PatternRec/ArffIO.hh"
#include "Ravl/PatternRec/SampleIter.hh"
#include "Ravl/OS/Filename.hh"

#include <cstdio>

using namespace RavlN;
using namespace RavlN;

//---------------------------------------------------------------------------
static void MyCallbackFunc(void *, int N)
{
  static int n = 0;
  if(N != n)
  {
    n = N;
    fprintf(stdout, "%i   \r", n);
    fflush(stdout);
  }
}

static void LoadTxt(const StringC &FileName, SampleC<VectorC> &TrSet,
                    SampleC<SByteT> &TrSetLabels)
{
  cout << "LoadTxt: Loading from:" << FileName << endl;
  IStreamC file(FileName);
  if(!file)
  {
    cerr << "LoadTxt: Failed to open file:" << FileName << endl;
    throw ExceptionOperationFailedC("Can't open file.");
  }
  int numObj, numFeatures;
  file >> numFeatures;
  file >> numObj;
  cout << "LoadTxt: Num features:" << numFeatures << endl;
  cout << "LoadTxt: Num objects :" << numObj << endl;
  for(int i = 0; i < numObj; i++)
  {
    VectorC vec(numFeatures);
    for(int j = 0; j < numFeatures; j++)
      file >> vec[j];

    int objClass;
    file >> objClass;
    //cout << "Object " << i << "  label " << objClass << endl;

    TrSet.Append(vec);
    TrSetLabels.Append(SByteT(objClass));
  }
}
//---------------------------------------------------------------------------
static void LoadFilesSet(StringC &ListFileName, int MaxNumObjects,
                         SampleC<VectorC> &Set, SampleC<StringC> &FileNames)
{
  cout << "LoadFilesSet: Loading list of files from:" << ListFileName << endl;
  IStreamC filesList(ListFileName);
  if(!filesList)
  {
    cerr << "LoadFilesSet: Failed to open file:" << ListFileName << endl;
    throw ExceptionOperationFailedC("Can't open file.");
  }

  // Load file names list
  DListC<StringC> namesList;
  StringC curFileName;
  while(!filesList.eof())
  {
    filesList >> curFileName;
    if(!curFileName.IsEmpty() && curFileName[0] != '#')
      namesList += curFileName;
  }
  cout << "LoadFilesSet: " << namesList.Size() << " filenames read\n";

  // Determine object ratio
  int listSize = namesList.Size();
  int numVectors = listSize > MaxNumObjects ? MaxNumObjects : listSize;

  // Allocate memory for array
  cout << "Allocating memory for " << numVectors << " objects\n";
  Set = SampleC<VectorC>(numVectors);
  FileNames = SampleC<StringC>(numVectors);

  // Read data
  cout << "LoadFilesSet: Loading data...\n";
  int vectorNumber = 0, namesProcessed = 0;

  for(DLIterC<StringC> curName(namesList); curName; curName++, namesProcessed++)
  {
    RealT threshold = (numVectors - vectorNumber) / (listSize - namesProcessed);
    if(Random1() > threshold || vectorNumber >= numVectors)
      continue;

    cout << (*curName) << " ";
    IStreamC inStrm(*curName);
    if(!inStrm)
    {
      cerr << "LoadFilesSet: Failed to open file:" << *curName << endl;
      throw ExceptionOperationFailedC("Can't open file.");
    }
    VectorC vec;
    inStrm >> vec;

    // preprocess data
    /*for(int i = 0; i < (int)(vec.Size()); i += 3)
    {
      if(vec[i] < 3.)
      {
        vec[i+1] = vec[i+2] = 0.;
      }
    }*/

    Set.Append(vec);
    FileNames.Append(*curName);
    vectorNumber++;
    cout << "OK   \r";
  }

  cout << "\nLoadFilesSet: " << vectorNumber << " images loaded\n";
}

//---------------------------------------------------------------------------
static void SaveSupportVectors(const StringC &ObjFileName,
                               const StringC &NonObjFileName,
                               const SampleC<SByteT> &TrSetLabels,
                               const SampleC<StringC> &TrSetNames,
                               const RealT* Lambdas)
{
  bool useArff = TrSetNames.Size() == 0;
  cout << "Saving support vectors..." << endl;
  int numSV1 = 0, numSV2 = 0;
  OStreamC svObj(ObjFileName);
  if(!svObj)
  {
    cerr << "SaveSupportVectors: Failed to open file:" << ObjFileName << endl;
    throw ExceptionOperationFailedC("Can't open file.");
  }

  OStreamC svNonObj(NonObjFileName);
  if(!svNonObj)
  {
    cerr << "SaveSupportVectors: Failed to open file:" << NonObjFileName << endl;
    throw ExceptionOperationFailedC("Can't open file.");
  }

  const RealT* labmdasPtr = Lambdas;
  const SizeT trSetSize = TrSetLabels.Size();
  for(SizeT i = 0; i < trSetSize; i++)
  {
    if(*labmdasPtr != 0)
    {
      StringC objName = useArff ? StringC(i) : TrSetNames[i];
      if(TrSetLabels[i] > 0)
      {
        svObj << objName << "  " << *labmdasPtr << endl;
        numSV1++;
      }
      else
      {
        svNonObj << objName << "  " << *labmdasPtr << endl;
        numSV2++;
      }
    }
    labmdasPtr++;
  }
  cout << "Class 1: " << numSV1 << "    Class 2: " << numSV2
       << "     total:" << numSV1 + numSV2 << endl;
}

//---------------------------------------------------------------------------
void LoadLambdas(SampleC<RealT> &Lambdas, const FilenameC &LambdasFile,
                 const SampleC<StringC> &Names)
{
  if(!LambdasFile.IsReadable())
    return;

  cout << "LoadLambdas: Loading lambdas from:" << LambdasFile << endl;
  IStreamC svListStrm(LambdasFile);
  if(!svListStrm)
  {
    cerr << "LoadLambdas: Failed to open file:" << LambdasFile << endl;
    throw ExceptionOperationFailedC("Can't open file.");
  }

  SArray1dC<RealT> lambdas = SArray1dC<RealT>(Names.Size());
  lambdas.Fill(0.);

  SampleIterC<StringC> namesIt(Names);
  UIntT index = 0;

  // Load file names list
  while(!svListStrm.eof())
  {
    StringC curFileName;
    RealT lambda = 0.;
    svListStrm >> curFileName >> lambda;
    if(!curFileName.IsEmpty() && curFileName[0] != '#')
    {
      UIntT lastIndex = index;

      //search for name
      while(namesIt)
      {
        if(*namesIt == curFileName)
        {
          lambdas[index] = lambda;
          break;
        }

        namesIt++;
        index++;
        if(!namesIt)
        {
          namesIt.First();
          index = 0;
        }
        if(index == lastIndex)
        {
          cout << "Skipping:" << curFileName << "  " << lambda << endl;
          break;
        }
      }
    }
  }
  Lambdas = SampleC<RealT>(lambdas);
}

//---------------------------------------------------------------------------
int main(int argc, char **argv)
{
  try
  {
    //parse command line parameters
    OptionC option(argc, argv);

    StringC objListName      = option.String("obj", "objList.txt",
                                      "List of objects for training");
    StringC nonObjListName   = option.String("nonObj", "nonObjList.txt",
                                      "List of non-objects for training");
    StringC svObjListName    = option.String("objSV", "objSVList.txt",
                                      "Output list of object support vectors");
    StringC svNonObjListName = option.String("nonObjSV", "nonObjSVList.txt",
                                      "Output list of non-object support vectors");
    StringC classifierName   = option.String("cName", "test.cls",
                                      "Name of file for saving classifier");
    IntT kernelType          = option.Int("kType", 0,
                                          "Kernel type (0 - linear, 1 - quadratic, 2 - polynomial, 3 - rbf)");
//    IntT classifierType      = option.Int("cType", 0,
//                                          "Classifier type (0 - linear, 1 - square)");
    RealT scale            = option.Real("scale", 1.,
                                         "Scale for kernel value");
    RealT c1               = option.Real("c1", 1e3,
                                         "Penalty for objects of 1 class");
    RealT c2               = option.Real("c2", 1e3,
                                         "Penalty for objects of 2 class");
    RealT tolerance        = option.Real("tol", 1e-7,
                                         "Tolerance of svm designer");
    RealT eps              = option.Real("eps", 1e-9,
                                         "Epsilon in svm designer");
    int maxNumSV           = option.Int("maxNumSV", 1000, "Maximum number of SVs");

    UIntT maxNoObjects     = option.Int("mno", 10000, "Maximum number of objects");
    UIntT maxNoNonObjects  = option.Int("mnn", 10000, "Maximum number of non-objects");
    StringC arffFileName   = option.String("arff", "", "Arff file");
    StringC txtFileName    = option.String("txt", "", "Txt file");
    StringC trimFeatures   = option.String("trFeat", "", "Features to use (0 - do not use, 1 - use)");
    int verbosity          = option.Int("v", 0, "verbosity");

    option.Check();

    //print parameters
    cout << "Object list file:" << objListName << endl;
    cout << "Non-object list file:" << nonObjListName << endl;

    switch(kernelType)
    {
    case 0:
      cout << "Linear kernel" << endl;
      break;
    case 1:
      cout << "Quadratic kernel" << endl;
      break;
    case 2:
      cout << "Polynomial kernel" << endl;
      break;
    case 3:
      cout << "RBF kernel" << endl;
      break;
    default:
      cout << "Classifier type:" << kernelType << endl;
      throw ExceptionOperationFailedC("Bad kernel type");
    }

    //read training set data
    SampleC<VectorC> trSet;
    SampleC<SByteT> trSetLabels;
    SampleC<StringC> trSetObjNames;
    SampleC<RealT> initLambdas;
    SizeT trSetSize = 0;
    SizeT numObj = 0;
    bool useArff = !arffFileName.IsEmpty() || !txtFileName.IsEmpty();
    if(!useArff)
    {
      SampleC<VectorC> objectsSet;
      SampleC<VectorC> nonObjectsSet;
      SampleC<StringC> objectNames;
      SampleC<StringC> nonObjectNames;
      SampleC<RealT> objectLambdas;
      SampleC<RealT> nonObjectLambdas;

      LoadFilesSet(objListName, maxNoObjects, objectsSet, objectNames);
      LoadFilesSet(nonObjListName, maxNoNonObjects, nonObjectsSet, nonObjectNames);

      LoadLambdas(objectLambdas, svObjListName, objectNames);
      LoadLambdas(nonObjectLambdas, svNonObjListName, nonObjectNames);

      //prepare data for training
      trSetSize = objectsSet.Size() + nonObjectsSet.Size();
      trSet = SampleC<VectorC>(trSetSize);
      trSetLabels = SampleC<SByteT>(trSetSize);
      trSetObjNames = SampleC<StringC>(trSetSize);
      UIntT vecLen = objectsSet[0].Size();

      for(int i = 0; i < int(objectsSet.Size()); i++)
      {
        if(vecLen != objectsSet[i].Size())
        {
          cerr << "Wrong length of training vector:" << objectNames[i] << endl;
          throw ExceptionOperationFailedC("Error in training set");
        }
        trSet.Append(objectsSet[i]);
        trSetLabels.Append(1);
        trSetObjNames.Append(objectNames[i]);
      }

      for(int i = 0; i < int(nonObjectsSet.Size()); i++)
      {
        if(vecLen != nonObjectsSet[i].Size())
        {
          cerr << "Wrong length of training vector:" << nonObjectNames[i] << endl;
          throw ExceptionOperationFailedC("Error in training set");
        }
        trSet.Append(nonObjectsSet[i]);
        trSetLabels.Append(-1);
        trSetObjNames.Append(nonObjectNames[i]);
      }
      numObj = objectsSet.Size();

      if(objectLambdas.Size() == objectsSet.Size() &&
         nonObjectLambdas.Size() == nonObjectsSet.Size())
      {
        initLambdas.Append(objectLambdas);
        initLambdas.Append(nonObjectLambdas);
      }
    }
    else
    {
      if(!txtFileName.IsEmpty())
      {
        LoadTxt(txtFileName, trSet, trSetLabels);
        if(!arffFileName.IsEmpty())
          SaveArff(arffFileName, trSet, trSetLabels);
      }
      else
      {
        LoadArff(arffFileName, trSet, trSetLabels);
      }
      trSetSize = trSet.Size();
      for(SampleIterC<SByteT> itLab(trSetLabels); itLab; itLab++)
      {
        if(*itLab > 0)
          numObj++;
      }
    }
    cout << "objects in 1 class:" << numObj << endl;
    cout << "objects in 2 class:" << trSetSize - numObj << endl;

    if(!trimFeatures.IsEmpty())
    {
      cout << "Trimming features\n";
      TrimFeatures(trSet, trimFeatures.chars());
    }

    //create classifier
    //define kernel function
    KernelFunctionC kernelFunc;
    switch(kernelType)
    {
    case 0:
      kernelFunc = LinearKernelC(scale);
      break;
    case 1:
      kernelFunc = QuadraticKernelC(scale);
      break;
    case 2:
      kernelFunc = PolynomialKernelC(2, scale, 1);
      break;
    case 3:
      kernelFunc = RBFKernelC(scale);
      break;
    }

    //create design svm object
    DesignSvmSmoC dsvm;
    if(trSetSize > 8000)
      dsvm = DesignSvmSmoLargeC(kernelFunc, maxNumSV, c1, c2, tolerance, eps);
    else
      dsvm = DesignSvmSmoC(kernelFunc, c1, c2, tolerance, eps);

    dsvm.SetCallbackFunc(MyCallbackFunc, NULL);
    dsvm.Verbosity() = verbosity;

    if(initLambdas.Size() == trSetSize)
    {
      dsvm.SetInitialLambdas(initLambdas);
    }

    //run training
    DateC startTime(true);
    //cout << "Current time:" << startTime << endl;
    cout << "Running designer\n";
    Classifier2C cl2 = dsvm.Apply(trSet, trSetLabels);
    SvmClassifierC svmClassifier = (SvmClassifierC &)cl2;
    cout << "Done\n";
    DateC endTime(true);
    cout << "Desing time:" << endTime.Double() - startTime.Double() << endl;

    //make simple classifier
    Classifier2C classifier;
    switch(kernelType)
    {
      case 0:
      {
        SvmLinearClassifierC linCl;
        linCl.Create(svmClassifier.SupportVectors(), svmClassifier.Lambdas(),
                     scale, svmClassifier.Threshold());
        classifier = linCl;
        break;
      }
      case 1:
      {
        SvmQuadraticClassifierC qCl;
        qCl.Create(svmClassifier.SupportVectors(), svmClassifier.Lambdas(),
                   scale, svmClassifier.Threshold());
        classifier = qCl;
        break;
      }
      default:
        classifier = svmClassifier;
        break;
    }

    //save classifier
    cout << "Saving classifier\n";
    OStreamC os(classifierName);
    if(!os)
      throw ExceptionOperationFailedC("Failed to open file for storing classifier.");
    os << classifier;

    //check training set
    cout << "Checking trainig set\n";
    int numErr = 0, numZeros = 0;
    for(SizeT i = 0; i < trSetSize; i++)
    {
      RealT val = classifier.Classify2(trSet[i]);
      StringC objName = useArff ? StringC(i) : trSetObjNames[i];
      if(trSetLabels[i] > 0) //objects
      {
        if(val < 0.)
        {
          cout << "c1 " << objName << " : " << val << endl;
          numErr++;
        }
        else if(val == 0.)
        {
          cout << "c1 " << objName << " : " << val << endl;
          numZeros++;
        }
      }
      else //non objects
      {
        if(val > 0.)
        {
          cout << "c2 " << objName << " : " << val << endl;
          numErr++;
        }
        else if(val == 0.)
        {
          cout << "c2 " << objName << " : " << val << endl;
          numZeros++;
        }
      }
    }
    cout << "Number of errors:" << numErr << endl;
    cout << "Number of zeros:" << numZeros << endl;

    //save support vectors
    VectorC lambdas = dsvm.GetLambdas();
    SaveSupportVectors(svObjListName, svNonObjListName,
                       trSetLabels, trSetObjNames, lambdas.DataStart());
    cout << "Finished." << endl;
  }
  catch( ExceptionC &exc )
  {
    cout << "Exception!\n" << exc.Text() << endl;
  }
  catch( const char* Str )
  {
    cout << "Exception!\n" << Str << endl;
  }
  catch( ... )
  {
    cout << "Unknown exception.\n";
  }
  return 0;
}


