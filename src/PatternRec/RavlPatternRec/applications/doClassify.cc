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

using namespace RavlN;

// Simple program which performs leave one out

int main(int nargs, char **argv) {

  OptionC opts(nargs, argv);

  StringC installDir = opts.String("i", PROJECT_OUT, "Install directory.");
  RavlN::SetResourceRoot(installDir);
  StringC classifierFile = opts.String("c", "", "Load the trained classifier from this file.");
  StringC inputDataSetFile = opts.String("di", "", "Data to classify");
  StringC outputDataSetFile = opts.String("do", "", "Where to save dataset with labels attached.");
  //bool verbose = opts.Boolean("v", false, "Verbose mode.");
  opts.Check();

  try {

    SysLogOpen("doClassify");

    // And save the classifier
    ClassifierC classifier;

    RavlInfo( "Loading classifier from '%s'", classifierFile.c_str());
    if (!Load(classifierFile, classifier)) {
      RavlError("Failed to loading classifier '%s' ",classifierFile.c_str());
      return 1;
    }

    if(!inputDataSetFile.IsEmpty()) {
      // Get dataset
      RavlInfo( "Loading dataset from file '%s'", inputDataSetFile.data());

      SampleVectorC data;
      if (!RavlN::Load(inputDataSetFile,data)) {
        RavlError("Failed to load dataset from file '%s'", inputDataSetFile.data());
        return 1;
      }

      DataSetVectorLabelC outputDataSet(data,classifier.Classify(data));

      RavlInfo("Saving data to '%s'", outputDataSetFile.c_str());
      if (!RavlN::Save(outputDataSetFile,outputDataSet)) {
        RavlError("Failed to save dataset to file '%s'", outputDataSetFile.data());
        return 1;
      }
    } else {
      RavlInfo("No data set provided for classification.");
    }

  } catch (const RavlN::ExceptionC &exc) {
    RavlError("Exception:%s", exc.Text());
  } catch (...) {
    RavlError("Unknown exception");
  }
}
