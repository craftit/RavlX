// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlPatternRec

#include "Ravl/Option.hh"
#include "Ravl/XMLFactory.hh"
#include "Ravl/Resource.hh"
#include "Ravl/PatternRec/DesignClassifierSupervised.hh"
#include "Ravl/PatternRec/DataSetVectorLabel.hh"
#include "Ravl/PatternRec/Error.hh"
#include "Ravl/PatternRec/DataSet2Iter.hh"
#include "Ravl/Sums1d2.hh"
#include "Ravl/OS/SysLog.hh"
#include "Ravl/PatternRec/DataSetIO.hh"

using namespace RavlN;

// Simple program which performs leave one out

int main(int nargs, char **argv) {

  OptionC opts(nargs, argv);

  StringC installDir = opts.String("i", PROJECT_OUT, "Install directory.");
  RavlN::SetResourceRoot(installDir);
  StringC configFile = opts.String("c", RavlN::Resource("Ravl/PatternRec", "classifier.xml"),
      "Classifier config file.  Look here for setting-up classifier parameters.");
  StringC classifierType = opts.String("classifier", "KNN", "The type of classifier to use [KNN|GMM|SVM].");
  StringC dsetFile = opts.String("dset", "", "The dataset to perform leave one out on!");
  bool equaliseSamples = opts.Boolean("eq", false, "Make sure we have an equal number of samples per class");
  UIntT samplesPerClass = opts.Int("n", 0, "The number of samples per class");
  DListC<StringC>features = opts.List("features", "Use only these features");
  //bool verbose = opts.Boolean("v", false, "Verbose mode.");
  UIntT maxIter = opts.Int("maxIter", 0, "Set the maximum number of iterations (0 do all)");
  opts.Check();

  SysLogOpen("doLeaveOneOut");

  try {
    XMLFactoryC::RefT mainFactory = new XMLFactoryC(configFile);
    XMLFactoryContextC context(*mainFactory);

    // Get classifier designer
    RavlInfo( "Initialising classifier '%s'", classifierType.data());
    DesignClassifierSupervisedC design;
    if (!context.UseComponent(classifierType, design, true)) {
      RavlError( "No '%s' component in XML config", classifierType.data());
      return 1;
    }

    // Get dataset
    RavlInfo( "Loading dataset from file '%s'", dsetFile.data());
    DataSetVectorLabelC dset;
    if (!LoadDataSetVectorLabel(dsetFile, dset)) {
      RavlError( "Trouble loading dataset from file!");
      return 1;
    }

    // Modify dataset if requested
    dset.Shuffle(); // always good practice to shuffle (inplace)
    if (equaliseSamples) {
      UIntT min = dset.ClassNums()[dset.ClassNums().IndexOfMin()];
      RavlInfo( "Equalising number of samples per class to %d", min);
      dset = dset.ExtractPerLabel(min);
    }
    if (samplesPerClass > 0 && samplesPerClass <= dset.ClassNums()[dset.ClassNums().IndexOfMin()]) {
      RavlInfo( "Setting the samples per class to %d", samplesPerClass);
      dset = dset.ExtractPerLabel(samplesPerClass);
    }
    if (opts.IsOnCommandLine("features")) {
      RavlInfo( "Manually selecting features to use");
      SArray1dC<IndexC> keep(features.Size());
      UIntT c = 0;
      for (DLIterC<StringC> it(features); it; it++) {
        keep[c] = it.Data().IntValue();
        c++;
      }
      SampleVectorC vecs(dset.Sample1(), keep);
      dset = DataSetVectorLabelC(vecs, dset.Sample2());
    }

    // Lets compute mean and variance of dataset and normalise input
    RavlInfo( "Normalising sample!");
    MeanCovarianceC meanCovariance = dset.Sample1().MeanCovariance();
    dset.Sample1().Normalise(meanCovariance);

    UIntT outPos = 0;
    Sums1d2C sum;
    if (maxIter == 0 || maxIter > dset.Size()) {
      maxIter = dset.Size();
      RavlInfo( "Using maximum number of samples '%d' in leave-one-out tests", maxIter);
    } else {
      // We need to shuffle the dataset so we do no get bias if only doing a sub-set
      dset.Shuffle();
      RavlInfo( "Only using a sub-set of samples '%d' in leave one out test", maxIter);
    }

    RavlInfo( "Performing leave-one-out test");

    for (DataSet2IterC<SampleVectorC, SampleLabelC> outIt(dset); outIt; outIt++) {

      // check if we have reached max iterations set by user
      if (outPos == maxIter)
        break;

      DataSetVectorLabelC trainDataSet(dset.Size() - 1);
      DataSetVectorLabelC testDataSet(1);
      std::cerr << "\rProcessing (" << outPos << "/" << maxIter << ")...." << (RealT) outPos / (RealT) maxIter * 100.0
          << "\%";
      // Build the dataset
      UIntT inPos = 0;
      for (DataSet2IterC<SampleVectorC, SampleLabelC> inIt(dset); inIt; inIt++) {
        // we have out test vector
        if (inPos == outPos) {
          testDataSet.Append(inIt.Data1(), inIt.Data2());
        } else {
          trainDataSet.Append(inIt.Data1(), inIt.Data2());
        }
        inPos++;
      }

      // Train classifier
      ClassifierC classifier = design.Apply(trainDataSet.Sample1(), trainDataSet.Sample2());

      // Lets get error
      ErrorC error;
      RealT pmc = error.Error(classifier, testDataSet);
      sum += pmc;
      outPos++;

    }
    std::cerr << std::endl;
    RavlInfo( "The probability of miss-classification is %0.4f(%0.4f)", sum.MeanVariance().Mean(),
        sum.MeanVariance().Variance());

  } catch (const RavlN::ExceptionC &exc) {
    RavlError( "Exception:%s", exc.Text());
  } catch (...) {
    RavlError( "Unknown exception");
  }
  return 0;
}
