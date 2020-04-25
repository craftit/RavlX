// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2013, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlPatternRec

#include "Ravl/Option.hh"
#include "Ravl/XMLFactory.hh"
#include "Ravl/OS/SysLog.hh"
#include "Ravl/Resource.hh"
#include "Ravl/PatternRec/DataSetVectorLabel.hh"
#include "Ravl/PatternRec/DataSet2Iter.hh"
#include "Ravl/IO.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/PatternRec/DataSetIO.hh"
#include "Ravl/PatternRec/DesignCluster.hh"
#include "Ravl/Plot/Plot2d.hh"
#include "Ravl/PatternRec/DesignFuzzyCMeansCluster.hh"
#include "Ravl/PatternRec/DesignKMeans.hh"
#include "Ravl/PatternRec/DistanceEuclidean.hh"
#include "Ravl/PatternRec/DistanceCorrelation.hh"
#include "Ravl/Random.hh"

using namespace RavlN;

#define USE_EXCEPTIONS 0

// Simple program which performs leave one out

int main(int nargs, char **argv) {

  OptionC opts(nargs, argv);

  StringC installDir = opts.String("i", PROJECT_OUT, "Install directory.");
  RavlN::SetResourceRoot(installDir);
  //StringC configFile = opts.String("c", RavlN::Resource("Ravl/PatternRec", "doDimensionReduction.xml"),"Dimension r config file.");
  //StringC classifierType = opts.String("classifier", "KNN", "The type of classifier to train [KNN|GMM|SVM|NN2|LR|SVMOneClass].");

  StringC dataSetFile = opts.String("d", "", "The data set to processor");
  StringC method = opts.String("m","KMeans","Method to use for clustering ");
  StringC distanceMetricName = opts.String("dm","SqrEuclidean","Distance metric (SqrEuclidean|Euclidean|Correlation)");

  bool plotData = opts.Boolean("pl",true,"Plot statistics. ");
  bool normaliseVectors = opts.Boolean("nv",false,"Normalise vector lengths when using correlation.");
  unsigned targetClusters = opts.Int("tc",0,"Target number of  clusters ");
  unsigned maxVecPlot = opts.Int("mp",6,"Maximum number of eigen vector plots to display.");
  RandomSeedDefault(opts.Int("rs",42,"Random number seed. "));

  bool verbose = opts.Boolean("v",true,"Verbose ");
  FilenameC classifierOutFile = opts.String("o", "cluster.abs", "Classification function.");
  FilenameC dataOutFile = opts.String("do", "", "Labeled data output");
  //bool verbose = opts.Boolean("v", false, "Verbose mode.");
  opts.Check();

  SysLogOpen("doCluster",false,true,true,-1,true);

#if USE_EXCEPTIONS
  try {
#endif
    //XMLFactoryContextC context(configFile);
    DistanceC distanceMetric;
    if(distanceMetricName == "SqrEuclidean") {
      distanceMetric = DistanceSqrEuclideanC();
    } else if(distanceMetricName == "Euclidean") {
      distanceMetric = DistanceEuclideanC();
    } else if(distanceMetricName == "Correlation") {
      distanceMetric = DistanceCorrelationC(normaliseVectors);
    } else {
      RavlError("Distance metric '%s' unknown ",distanceMetricName.c_str());
      return 1;
    }

    DesignClusterC designCluster;

    if(method == "KMeans") {
      designCluster = DesignKMeansC(targetClusters,distanceMetric);
    } else if(method == "FuzzyCMeans") {
      designCluster = DesignFuzzyCMeansClusterC(targetClusters,distanceMetric);
    } else {
      RavlError("Clustering method '%s' unknown ",method.c_str());
      return 1;
    }

    designCluster.SetVerbose(verbose);

    DataSetVectorLabelC dataSet; // the normalised data set
    if(!LoadDataSetVectorLabel(dataSetFile,dataSet)) {
      RavlError("Failed to load data set '%s'", dataSetFile.data());
      return 1;
    }

    ClassifierC classifier = designCluster.Apply(dataSet.Sample1());

    // And save the classifier
    RavlInfo( "Saving function to '%s'", classifierOutFile.data());
    if (!Save(classifierOutFile, classifier)) {
      RavlError("Failed to save classifier");
      return 1;
    }

    if(!dataOutFile.IsEmpty()) {
      SampleLabelC labels = classifier.Classify(dataSet.Sample1());
      DataSetVectorLabelC clusterDataSet(dataSet.Sample1(),labels); // the normalised data set
      if(!SaveDataSetVectorLabel(dataOutFile,clusterDataSet)) {
        RavlError("Failed to save labelled data to '%s' ",dataOutFile.c_str());
      }
    }

#if USE_EXCEPTIONS
  } catch (const RavlN::ExceptionC &exc) {
    RavlError( "Exception:%s", exc.Text());
  } catch (...) {
    RavlError( "Unknown exception");
  }
#endif
}
