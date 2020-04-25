// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2005-12, University of Surrey.
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
#include "Ravl/Face/Roc.hh"

using namespace RavlN;

// Simple program which performs leave one out

int main(int nargs, char **argv) {

  OptionC opts(nargs, argv);

  StringC installDir = opts.String("i", PROJECT_OUT, "Install directory.");
  RavlN::SetResourceRoot(installDir);
  StringC classifierFile = opts.String("classifier", "", "Load the trained classifier from this file.");
  StringC testDataSetFile = opts.String("dset", "", "The dataset to perform the test on!");
  bool equaliseSamples = opts.Boolean("eq", false, "Make sure we have an equal number of samples per class");
  UIntT samplesPerClass = opts.Int("n", 0, "The number of samples per class");
  DListC<StringC> features = opts.List("features", "Use only these features");
  DirectoryC resultDir = opts.String("dir", "results", "Output directory to place results");
  //bool verbose = opts.Boolean("v", false, "Verbose mode.");
  opts.Check();

  try {

    SysLogOpen("doTestClassifier");

    if(!resultDir.Exists()) {
      if(!resultDir.MakeDir()) {
        RavlError("Failed to make output directory");
        return 1;
      }
    }

    // And save the classifier
    ClassifierC classifier;
    RavlInfo( "Loading classifier from '%s'", classifierFile.data());
    if (!Load(classifierFile, classifier)) {
      SysLog(SYSLOG_ERR, "Trouble loading classifier");
      return 1;
    }

    // Get dataset
    RavlInfo( "Loading dataset from file '%s'", testDataSetFile.data());
    // FIXME: Still want to use Load/Save instead
    DataSetVectorLabelC testDataSet;
    if (!LoadDataSetVectorLabel(testDataSetFile, testDataSet)) {
      SysLog(SYSLOG_ERR, "Trouble loading dataset from file '%s'", testDataSetFile.data());
      return 1;
    }

    // Modify data set if requested
    testDataSet.Shuffle(); // always good practice to shuffle (in place)
    if (equaliseSamples) {
      UIntT min = testDataSet.ClassNums()[testDataSet.ClassNums().IndexOfMin()];
      RavlInfo( "Equalising number of samples per class to %d", min);
      testDataSet = testDataSet.ExtractPerLabel(min);
    }
    if (samplesPerClass > 0
        && samplesPerClass <= testDataSet.ClassNums()[testDataSet.ClassNums().IndexOfMin()]) {
      RavlInfo( "Setting the samples per class to %d", samplesPerClass);
      testDataSet = testDataSet.ExtractPerLabel(samplesPerClass);
    }
    if (opts.IsOnCommandLine("features")) {
      RavlInfo( "Manually selecting features to use");
      SArray1dC<IndexC> keep(features.Size());
      UIntT c = 0;
      for (DLIterC<StringC> it(features); it; it++) {
        keep[c] = it.Data().IntValue();
        c++;
      }
      SampleVectorC vecs(testDataSet.Sample1(), keep);
      testDataSet = DataSetVectorLabelC(vecs, testDataSet.Sample2());
    }

    FaceN::RocC roc(true);

    for(DataSet2IterC<SampleVectorC, SampleLabelC>it(testDataSet);it;it++) {
      VectorC confidence = classifier.Apply(it.Data1());
      RealT label0 = classifier.LabelProbability(it.Data1(), 0);
      roc.Insert(label0, it.Data2() == 0);
    }

    roc.Report(resultDir);


  } catch (const RavlN::ExceptionC &exc) {
    SysLog(SYSLOG_ERR, "Exception:%s", exc.Text());
  } catch (...) {
    SysLog(SYSLOG_ERR, "Unknown exception");
  }
}
