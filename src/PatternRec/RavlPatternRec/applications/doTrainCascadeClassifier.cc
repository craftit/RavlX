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
#include "Ravl/PatternRec/DesignCascade.hh"
#include "Ravl/PatternRec/DataSetVectorLabel.hh"
#include "Ravl/PatternRec/ClassifierPreprocess.hh"
#include "Ravl/PatternRec/Error.hh"
#include "Ravl/PatternRec/DataSet2Iter.hh"
#include "Ravl/IO.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/PatternRec/DataSetIO.hh"
#include "Ravl/PatternRec/OptimiseClassifierDesign.hh"
#include "Ravl/PatternRec/FuncSubset.hh"
#include "Ravl/PatternRec/ErrorBinaryClassifier.hh"
#include "Ravl/SArray1dIter2.hh"

using namespace RavlN;

#define USE_EXCEPTIONS 0

// Simple program which performs leave one out

int main(int nargs, char **argv)
{

  OptionC opts(nargs, argv);

  StringC installDir = opts.String("i", PROJECT_OUT, "Install directory.");
  RavlN::SetResourceRoot(installDir);
  StringC configFile = opts.String("c", RavlN::Resource("Ravl/PatternRec", "classifier.xml"), "Classifier config file.");
  StringC classifierType = opts.String("classifier", "Cascade", "The type of classifier to train [KNN|GMM|SVM|SVMOneClass].");
  StringC trainingDataSetFile = opts.String("train", "", "The data set to train on");
  StringC validationDataSetFile = opts.String("validation", "", "If supplied, use this data set to perform validation whilst training.");
  bool equaliseSamples = opts.Boolean("eq", "Make sure we have an equal number of samples per class");
  bool noshuffle = opts.Boolean("noshuffle", "Do not shuffle, i.e. default is not to shuffle.");
  UIntT samplesPerClass = opts.Int("n", 0, "The number of samples per class");
  DListC<StringC> featuresList = opts.List("features", "Use only these features");
  DataSetNormaliseT normaliseType = (DataSetNormaliseT) opts.Int("normalise", 0, "Normalise sample (0 - none, 1 - mean, 2- scale)");
  FilenameC classifierOutFile = opts.String("o", "classifier.abs", "Save classifier to this file.");
  bool saveDataSet = opts.Boolean("saveDataSet", "Save the training data set.");

  //bool verbose = opts.Boolean("v", false, "Verbose mode.");
  opts.Compulsory("train");
  opts.Compulsory("validation");
  opts.Check();

  SysLogOpen("doTrainCascadeClassifier", false, true, true, -1, true);

#if USE_EXCEPTIONS
  try {
#endif
  XMLFactoryContextC context(configFile);

  // Get classifier designer
  RavlInfo("Initialising classifier '%s'", classifierType.data());
  DesignCascadeC design;
  if (!context.UseComponent(classifierType, design, true)) {
    RavlError("No '%s' component in XML config", classifierType.data());
    return 1;
  }

  /*
   * Are we using a sub-set of features
   */
  SArray1dC<IndexC> features;
  if (opts.IsOnCommandLine("features")) {
    RavlInfo( "Manually selecting '%d' features to use", featuresList.Size().V());
    features = SArray1dC<IndexC>(featuresList.Size());
    UIntT c = 0;
    for (DLIterC<StringC> it(featuresList); it; it++) {
      features[c] = IndexC(it.Data().IntValue());
      c++;
    }
  }

  /*
   * Training data set
   */
  DataSetVectorLabelC trainingDataSet;
  FunctionC normaliseFunc;
  if (!LoadDataSetVectorLabel(trainingDataSetFile,
      !noshuffle,
      equaliseSamples,
      samplesPerClass,
      features,
      normaliseType,
      normaliseFunc,
      trainingDataSet)) {
    RavlError("Failed to load data set '%s'", trainingDataSetFile.data());
    return 1;
  }

  // Do we want to save the modified data set
  if (saveDataSet) {
    if (!SaveDataSetVectorLabel("training.abs", trainingDataSet)) {
      RavlError("Trouble saving 'training.abs");
    }
  }

  /*
   * Load validation data set if requested
   */
  DataSetVectorLabelC validationDataSet;
  // it will be normalised using the stats from the training data set
  if (!LoadDataSetVectorLabel(validationDataSetFile,
      !noshuffle,
      equaliseSamples,
      samplesPerClass,
      features,
      normaliseFunc,
      validationDataSet)) {
    RavlError("Failed to load data set '%s'", trainingDataSetFile.data());
    return 1;
  }
  // Do we want to save the modified data set
  if (saveDataSet) {
    if (!SaveDataSetVectorLabel("validation.abs", validationDataSet)) {
      RavlError("Trouble saving 'validation.abs");
    }
  }

  /*
   * Lets sort out the classifier
   */
  ClassifierC classifier = design.Apply(trainingDataSet, validationDataSet);

  ErrorC error;
  SArray1dC<RealT>errorByLabel = error.ErrorByLabel(classifier, validationDataSet);
  RavlInfo("Validation Data FA = %0.4f FR = %0.4f", errorByLabel[0], errorByLabel[1]);

  // If we have normalised the sample we need to make sure
  // all input data to classifier is normalised by same statistics
  if (normaliseFunc.IsValid()) {
    RavlInfo( "Making classifier with pre-processing step!");
    classifier = ClassifierPreprocessC(normaliseFunc, classifier);
  }

  // And save the classifier
  RavlInfo( "Saving classifier to '%s'", classifierOutFile.data());
  if (!Save(classifierOutFile, classifier)) {
    RavlError( "Trouble saving classifier");
    return 1;
  }

  RavlInfo("All OK!");

#if USE_EXCEPTIONS
} catch (const RavlN::ExceptionC &exc) {
  RavlError( "Exception:%s", exc.Text());
} catch (...) {
  RavlError( "Unknown exception");
}
#endif
}
