// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlPatternRec

#include "Ravl/Option.hh"
#include "Ravl/XMLFactory.hh"
#include "Ravl/OS/SysLog.hh"
#include "Ravl/Resource.hh"
#include "Ravl/PatternRec/DesignClassifierSupervised.hh"
#include "Ravl/PatternRec/DataSetVectorLabel.hh"
#include "Ravl/PatternRec/ClassifierPreprocess.hh"
#include "Ravl/PatternRec/Error.hh"
#include "Ravl/PatternRec/DataSet2Iter.hh"
#include "Ravl/IO.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/PatternRec/DataSetIO.hh"
#include "Ravl/PatternRec/OptimiseClassifierDesign.hh"
#include "Ravl/PatternRec/FeatureSelectAsymmetricAdaBoost.hh"
#include "Ravl/PatternRec/DesignWeakLinear.hh"
#include "Ravl/PatternRec/FuncSubset.hh"
#include "Ravl/PatternRec/SampleIter.hh"

using namespace RavlN;

#define USE_EXCEPTIONS 0

// Simple program which performs leave one out

int main(int nargs, char **argv)
{

  OptionC opts(nargs, argv);

  StringC installDir = opts.String("i", PROJECT_OUT, "Install directory.");
  RavlN::SetResourceRoot(installDir);
  StringC trainingDataSetFile = opts.String("train", "", "The dataset to train on");
  StringC testDataSetFile = opts.String("test", "", "The dataset to train on");
  bool equaliseSamples = opts.Boolean("eq", false, "Make sure we have an equal number of samples per class");
  UIntT samplesPerClass = opts.Int("n", 0, "The number of samples per class");
  StringC NormaliseSample = opts.String("normalise", "none", "Normalise sample (mean, none, scale)");
  FilenameC classifierOutFile = opts.String("o", "classifier.abs", "Save classifier to this file.");
  RealT weight = opts.Real("weight", 0.5, "Prioiri prob for labels.  Greater than 0.5 favours label1, less than 0.5 favours label0.");
  UIntT numberOfFeatures = opts.Int("f", 100, "Number of features to keep");
  //bool verbose = opts.Boolean("v", false, "Verbose mode.");
  opts.Check();

  SysLogOpen("doTrainClassifier", false, true, true, -1, true);

#if USE_EXCEPTIONS
  try {
#endif


  // Get training dataset
  RavlInfo("Loading dataset from file '%s'", trainingDataSetFile.data());
  // FIXME: Still want to use Load/Save instead
  DataSetVectorLabelC trainingDataSet;
  if (!LoadDataSetVectorLabel(trainingDataSetFile, trainingDataSet)) {
    RavlError("Trouble loading dataset from file '%s'", trainingDataSetFile.data());
    return 1;
  }
  SizeT dim = trainingDataSet.Sample1().First().Size();
  RavlInfo("Training input dimension %s with samples %s", StringOf(dim).data(), StringOf(trainingDataSet.Size()).data());

  // Get testing dataset
  RavlInfo("Loading dataset from file '%s'", testDataSetFile.data());
  // FIXME: Still want to use Load/Save instead
  DataSetVectorLabelC testDataSet;
  if (!LoadDataSetVectorLabel(testDataSetFile, testDataSet)) {
    RavlError("Trouble loading dataset from file '%s'", testDataSetFile.data());
    return 1;
  }

  // Modify data set if requested
  trainingDataSet.Shuffle(); // always good practice to shuffle (inplace)
  if (equaliseSamples) {
    UIntT min = trainingDataSet.ClassNums()[trainingDataSet.ClassNums().IndexOfMin()];
    RavlInfo( "Equalising number of training samples per class to %d", min);
    trainingDataSet = trainingDataSet.ExtractPerLabel(min);

    UIntT minTest = testDataSet.ClassNums()[testDataSet.ClassNums().IndexOfMin()];
    RavlInfo( "Equalising number of test samples per class to %d", min);
    testDataSet = testDataSet.ExtractPerLabel(minTest);

  }
  if (samplesPerClass > 0 && samplesPerClass <= trainingDataSet.ClassNums()[trainingDataSet.ClassNums().IndexOfMin()]) {
    RavlInfo( "Setting the samples per class to %d", samplesPerClass);
    trainingDataSet = trainingDataSet.ExtractPerLabel(samplesPerClass);
    testDataSet = testDataSet.ExtractPerLabel(samplesPerClass);
  }

  FeatureSelectAsymmetricAdaBoostC adaBoost(weight, numberOfFeatures);
  DesignWeakLinearC designClassifier;
  ClassifierC classifier;

  SArray1dC<IndexC> features = adaBoost.SelectFeatures(designClassifier, trainingDataSet, testDataSet, classifier);

  FuncSubsetC funcSubset(features, dim);
  classifier = ClassifierPreprocessC(funcSubset, classifier);

  // Lets get error on training data set - even though highly biased
  ErrorC error;
  RealT pmc = error.Error(classifier, trainingDataSet);
  RavlInfo( "The (biased) probability of miss-classification on training data is %0.4f ", pmc);

  pmc = error.Error(classifier, testDataSet);
  RavlInfo( "The (biased) probability of miss-classification on validation data is %0.4f ", pmc);

  // And save the classifier
  RavlInfo( "Saving classifier to '%s'", classifierOutFile.data());
  if (!Save(classifierOutFile, classifier)) {
    RavlError( "Trouble saving classifier");
    return 1;
  }

#if 0
  // Lets compute mean and variance of data set and normalise input
  FunctionC normaliseFunc;
  if (NormaliseSample == "none") {
    if (!doMetaDesign)
    RavlInfo( "You are not normalising your sample!  I hope you know what you are doing.");
  } else if (NormaliseSample == "mean") {
    // FIXME: Sometimes you want to normalise on a class, rather than the whole sample
    RavlInfo( "Normalising the whole sample using sample mean and variance!");
    MeanCovarianceC meanCovariance = trainingDataSet.Sample1().MeanCovariance();
    normaliseFunc = trainingDataSet.Sample1().NormalisationFunction(meanCovariance);
    trainingDataSet.Sample1().Normalise(meanCovariance);
  } else if (NormaliseSample == "scale") {
    RavlInfo( "Scaling the whole sample!");
    FuncLinearC lfunc;
    trainingDataSet.Sample1().Scale(lfunc);
    normaliseFunc = lfunc;
  } else {
    RavlError( "Normalisation method not known!");
    return 1;
  }

  ClassifierC classifier;

  if (doMetaDesign) {
    OptimiseClassifierDesignC ocd;
    context.UseComponent("OptimiseClassifierDesign", ocd);
    VectorC bestClassifierParams;
    RealT finalResult;
    ocd.Apply(design, trainingDataSet.Sample1(), trainingDataSet.Sample2(), classifier, bestClassifierParams, finalResult);

    RavlInfo( "The (biased) probability of miss-classification is %0.4f  @ %s ", finalResult, RavlN::StringOf(bestClassifierParams).c_str());
  } else {
    // Train classifier
    RavlInfo( "Training the classifier");
    classifier = design.Apply(trainingDataSet.Sample1(), trainingDataSet.Sample2());
    RavlInfo( " - finished");
    // Lets get error on training data set - even though highly biased
    ErrorC error;
    RealT pmc = error.Error(classifier, trainingDataSet);
    RavlInfo( "The (biased) probability of miss-classification is %0.4f ", pmc);
  }

  // If we have normalised the sample we need to make sure
  // all input data to classifier is normalised by same statistics
  if (NormaliseSample != "none") {
    RavlInfo( "Making classifier with pre-processing step!");
    classifier = ClassifierPreprocessC(normaliseFunc, classifier);
  }

#endif

#if USE_EXCEPTIONS
} catch (const RavlN::ExceptionC &exc) {
  RavlError( "Exception:%s", exc.Text());
} catch (...) {
  RavlError( "Unknown exception");
}
#endif
}
