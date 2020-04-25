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
#include "Ravl/PatternRec/FuncSubset.hh"
#include "Ravl/PatternRec/FeatureSelectPlusLMinusR.hh"

using namespace RavlN;

#define USE_EXCEPTIONS 0

// Simple program which performs leave one out

int main(int nargs, char **argv) {

  OptionC opts(nargs, argv);

  StringC installDir = opts.String("i", PROJECT_OUT, "Install directory.");
  RavlN::SetResourceRoot(installDir);
  StringC configFile = opts.String("c", RavlN::Resource("Ravl/PatternRec", "classifier.xml"),
      "Classifier config file.");
  StringC classifierType = opts.String("classifier", "KNN", "The type of classifier to train [KNN|GMM|SVM|NN2|LR|SVMOneClass].");
  bool doMetaDesign = opts.Boolean("md",false,"Do meta design, optimise the design parameters ");
  StringC trainingDataSetFile = opts.String("dset", "", "The data set to train on");
  StringC validationDataSetFile = opts.String("validation", "", "If supplied, use this data set to perform validation whilst training.");
  bool equaliseSamples = opts.Boolean("eq", "Make sure we have an equal number of samples per class");
  bool noshuffle = opts.Boolean("noshuffle", "Do not shuffle, i.e. default is to shuffle.");
  UIntT samplesPerClass = opts.Int("n", 0, "The number of samples per class");
  DListC<StringC> featuresList = opts.List("features", "Use only these features");
  DataSetNormaliseT normaliseType = (DataSetNormaliseT)opts.Int("normalise",doMetaDesign ? 0 : 1, "Normalise sample (0 - none, 1 - mean, 2- scale)");
  FilenameC classifierOutFile = opts.String("o", "classifier.abs", "Save classifier to this file.");
  UIntT numberOfFeatures = opts.Int("fs", 0, "Use +L-R feature selection to select this many features.  Requires validation set.");
  bool saveDataSet = opts.Boolean("saveDataSet", "Save the training data set.");
  //bool verbose = opts.Boolean("v", false, "Verbose mode.");
  opts.Check();

  SysLogOpen("doTrainClassifier",false,true,true,-1,true);

#if USE_EXCEPTIONS
  try {
#endif
    XMLFactoryContextC context(configFile);

    // Get classifier designer
    RavlInfo("Initialising classifier '%s'", classifierType.data());
    DesignClassifierSupervisedC design;
    if (!context.UseComponent(classifierType, design, true)) {
      RavlError("No '%s' component in XML config", classifierType.data());
      return 1;
    }

    /*
     * Are we using a sub-set of features
     */
    SArray1dC<IndexC>features;
    if(opts.IsOnCommandLine("features")) {
      RavlInfo( "Manually selecting '%d' features to use", featuresList.Size().V());
      features = SArray1dC<IndexC>(featuresList.Size());
      UIntT c=0;
      for(DLIterC<StringC>it(featuresList);it;it++) {
        features[c] = IndexC(it.Data().IntValue());
        c++;
      }
    }

    /*
     * Load the training set and normalise if requested
     */
    DataSetVectorLabelC trainingDataSet; // the normalised data set
    FunctionC normaliseFunc; // the function used to normalise the data set
    if(!LoadDataSetVectorLabel(trainingDataSetFile, !noshuffle, equaliseSamples, samplesPerClass, features, normaliseType, normaliseFunc, trainingDataSet)) {
      RavlError("Failed to load data set '%s'", trainingDataSetFile.data());
      return 1;
    }

    // Do we want to save the modified data set??
    if(saveDataSet) {
      if(!SaveDataSetVectorLabel("training.abs", trainingDataSet)) {
        RavlError("Trouble saving 'training.abs");
      }
    }

    /*
     * Load validation data set if requested.  We normalise using the function obtained loading the training data set
     */
    DataSetVectorLabelC validationDataSet;
    if (opts.IsOnCommandLine("validation")) {
      // it will be normalised using the stats from the training data set
      if(!LoadDataSetVectorLabel(validationDataSetFile, !noshuffle, equaliseSamples, samplesPerClass, features, normaliseFunc, validationDataSet)) {
        RavlError("Failed to load data set '%s'", trainingDataSetFile.data());
        return 1;
      }
      // Do we want to save the modified data set
      if(saveDataSet) {
        if(!SaveDataSetVectorLabel("validation.abs", validationDataSet)) {
          RavlError("Trouble saving 'validation.abs");
        }
      }
    }

    /*
     * Lets sort out the classifier
     */
    ClassifierC classifier;

    if(doMetaDesign) {
      OptimiseClassifierDesignC ocd;
      context.UseComponent("OptimiseClassifierDesign",ocd);
      VectorC bestClassifierParams;
      RealT finalResult;
      ocd.Apply(design,
                trainingDataSet.Sample1(),
                trainingDataSet.Sample2(),
                classifier,
                bestClassifierParams,
                finalResult
                ) ;

      RavlInfo( "The (biased) probability of miss-classification is %0.4f  @ %s ", finalResult,RavlN::StringOf(bestClassifierParams).c_str());
    } else {
      // Train classifier
      RavlInfo( "Training the classifier");
      /*
       * The user may of supplied a validation set and want to do feature selection?
       */
      if (opts.IsOnCommandLine("validation") && numberOfFeatures > 0) {
        FeatureSelectPlusLMinusRC featureSelection(2, 1, 0.001, numberOfFeatures);
        SArray1dC<IndexC> selectedFeatures = featureSelection.SelectFeatures(design, trainingDataSet, validationDataSet, classifier);
        FuncSubsetC funcSubset(selectedFeatures, trainingDataSet.Sample1().First().Size());
        classifier = ClassifierPreprocessC(funcSubset, classifier);
      }
      /*
       * Or perhaps just train a normal classifier
       */
      else {
        classifier = design.Apply(trainingDataSet.Sample1(), trainingDataSet.Sample2());
      }

      /*
       * Finally lets test the trained classifier.  If we have a validation set we use that as well.
       */
      ErrorC error;
      RealT pmc = error.Error(classifier, trainingDataSet);
      RavlInfo( "The (biased) probability of miss-classification is %0.4f ", pmc);
      if (opts.IsOnCommandLine("validation")) {
        RealT pmc2 = error.Error(classifier, validationDataSet);
        RavlInfo( "The probability of miss-classification on validation set is %0.4f ", pmc2);
      }
      RavlInfo( " - finished");
    }

    /*
     * Now we need to save the classifier
     */
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

#if USE_EXCEPTIONS
  } catch (const RavlN::ExceptionC &exc) {
    RavlError( "Exception:%s", exc.Text());
  } catch (...) {
    RavlError( "Unknown exception");
  }
#endif
}
