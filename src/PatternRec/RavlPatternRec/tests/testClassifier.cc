// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Classify/testClassifier.cc"
//! docentry="Ravl.API.Pattern Recognition.Classifier"
//! userlevel=Develop

#include "Ravl/PatternRec/ClassifierKNearestNeighbour.hh"
#include "Ravl/PatternRec/ClassifierAverageNearestNeighbour.hh"
#include "Ravl/PatternRec/DesignClassifierGaussianMixture.hh"
#include "Ravl/PatternRec/DesignClassifierLogisticRegression.hh"
#include "Ravl/PatternRec/DesignClassifierNeuralNetwork2.hh"
#include "Ravl/PatternRec/DesignWeakLinear.hh"
#include "Ravl/PatternRec/DesignKMeans.hh"
#include "Ravl/PatternRec/SampleIter.hh"
#include "Ravl/PatternRec/DataSet2Iter.hh"
#include "Ravl/PatternRec/DesignBayesNormalLinear.hh"
#include "Ravl/PatternRec/DesignBayesNormalQuadratic.hh"
#include "Ravl/PatternRec/FuncQuadratic.hh"

#include "Ravl/HSet.hh"
#include "Ravl/UnitTest.hh"
#include "Ravl/SysLog.hh"

using namespace RavlN;

int GenerateDataSet();
int testKNearestNeighbour();
int testAverageNearestNeighbour();
int testDesignKMeans();
int testDesignClassifierGaussianMixture();
int testDesignClassifierLogisticRegression();
int testDesignClassifierNeuralNetwork2();
int testDesignClassifierLogisticRegressionQuadratic();
int testDesignClassifierWeakLinear();
int testDesignClassifierBayesNormalLinear();
int testDesignClassifierBayesNormalQuadratic();


int main() {
  SysLogOpen("testClassifier",false,true,true);
  RAVL_RUN_TEST(GenerateDataSet());
#if 0
  RAVL_RUN_TEST(testKNearestNeighbour());
  RAVL_RUN_TEST(testAverageNearestNeighbour());
  RAVL_RUN_TEST(testDesignKMeans());
  RAVL_RUN_TEST(testDesignClassifierGaussianMixture());
  RAVL_RUN_TEST(testDesignClassifierLogisticRegressionQuadratic());
  RAVL_RUN_TEST(testDesignClassifierWeakLinear());
  RAVL_RUN_TEST(testDesignClassifierBayesNormalLinear());
  RAVL_RUN_TEST(testDesignClassifierBayesNormalQuadratic());
  RAVL_RUN_TEST(testDesignClassifierNeuralNetwork2());
#endif
  RAVL_RUN_TEST(testDesignClassifierLogisticRegression());
  RavlInfo("Test passed ok. ");
  return 0;
}

DataSet2C<SampleVectorC,SampleLabelC> dataset(true);

int GenerateDataSet() {
  
  dataset.Append(VectorC(0.10,0.10),1);
  dataset.Append(VectorC(0.11,0.11),1);
  dataset.Append(VectorC(0.30,0.20),1);
  dataset.Append(VectorC(0.20,0.10),1);
  dataset.Append(VectorC(0.23,0.12),1);
  dataset.Append(VectorC(0.40,0.90),2);
  dataset.Append(VectorC(0.30,0.80),2);
  dataset.Append(VectorC(0.32,0.81),2);
  dataset.Append(VectorC(0.50,0.90),2);
  dataset.Append(VectorC(0.51,0.92),2);
  dataset.Append(VectorC(0.90,0.30),0);
  dataset.Append(VectorC(0.90,0.20),0);
  dataset.Append(VectorC(0.93,0.24),0);
  dataset.Append(VectorC(0.90,0.40),0);
  dataset.Append(VectorC(0.95,0.42),0);
  RAVL_TEST_TRUE(dataset.Size() == 15);
  return 0;
}


int testKNearestNeighbour() {
  //std::cerr << "testKNearestNeighbour(), Called. \n";
  ClassifierKNearestNeighbourC knn(dataset,3);
  int c = knn.Classify(VectorC(0.3,0.2));
  RAVL_TEST_TRUE(c == 1);
  VectorC vec =  knn.Confidence(VectorC(0.3,0.2));
  RAVL_TEST_TRUE(vec.Size() == 3);
  UIntT right = 0,wrong = 0;
  for(DataSet2IterC<SampleVectorC,SampleLabelC> it(dataset);it;it++) {
    UIntT label = knn.Classify(it.Data1());
    if(label == it.Data2()) {
      right ++;
    } else {
      wrong ++;
    }
  }
  RAVL_TEST_TRUE(right == dataset.Size());
  //RavlDebug("Average KNearest Right=%u Wrong=%u ",right,wrong);
  return 0;
}

int testAverageNearestNeighbour() {
  //std::cerr << "testAverageNearestNeighbour(), Called. \n";
  ClassifierAverageNearestNeighbourC knn(dataset,3);
  int c = knn.Classify(VectorC(0.3,0.2));
  if(c != 1) return __LINE__;
  VectorC vec =  knn.Confidence(VectorC(0.3,0.2));
  UIntT right = 0,wrong = 0;
  for(DataSet2IterC<SampleVectorC,SampleLabelC> it(dataset);it;it++) {
    UIntT label = knn.Classify(it.Data1());
    if(label == it.Data2()) {
      right ++;
    } else {
      wrong ++;
    }
  }
  //RavlDebug("Average KNearest Right=%u Wrong=%u ",right,wrong);
  RAVL_TEST_TRUE(right == dataset.Size());
  return 0;
}

int testDesignKMeans() {
  //std::cerr << "testDesignKMeans(), Called. \n";
  DesignKMeansC kmeans(3);
  ClassifierC cv = kmeans.Apply(dataset.Sample1());
  RAVL_TEST_TRUE(cv.IsValid());
  HSetC<UIntT> labels;
  for(DataSet2IterC<SampleVectorC,SampleLabelC> it(dataset);it;it++) {
    UIntT label = cv.Classify(it.Data1());
    labels += label;
    //std::cerr << "Label=" << label << "\n";
  }
  //RavlDebug("Right=%u Wrong=%u ",right,wrong);
  RAVL_TEST_TRUE(labels.Size() == 3);
  return 0;
}

int testDesignClassifierGaussianMixture() {
  //std::cerr << "testDesignClassifierGaussianMixture(), Called. \n";
  DesignClassifierGaussianMixtureC gm(1);
  ClassifierC cv = gm.Apply(dataset.Sample1(),dataset.Sample2());
  RAVL_TEST_TRUE(cv.IsValid());
  UIntT right = 0,wrong = 0;
  for(DataSet2IterC<SampleVectorC,SampleLabelC> it(dataset);it;it++) {
    UIntT label = cv.Classify(it.Data1());
    if(label == it.Data2()) {
      right ++;
    } else {
      wrong ++;
    }
    //std::cerr << "Label=" << label << "\n";
  }
  //RavlDebug("Right=%u Wrong=%u ",right,wrong);
  RAVL_TEST_TRUE(right == dataset.Size());
  return 0;
}

int testDesignClassifierLogisticRegression()
{
  DesignClassifierLogisticRegressionC lr(0.0);
  ClassifierC cv = lr.Apply(dataset.Sample1(),dataset.Sample2());
  RAVL_TEST_TRUE(cv.IsValid());
  UIntT right = 0,wrong = 0;
  for(DataSet2IterC<SampleVectorC,SampleLabelC> it(dataset);it;it++) {
    UIntT label = cv.Classify(it.Data1());
    VectorC conf = cv.Confidence(it.Data1());
    //RavlDebug(" %u -> %s ",label,RavlN::StringOf(conf).c_str());
    if(label == it.Data2()) {
      right ++;
    } else {
      wrong ++;
    }
  }
  //RavlDebug("Right=%u Wrong=%u ",right,wrong);
  RAVL_TEST_TRUE(right == dataset.Size());
  return 0;
}

int testDesignClassifierLogisticRegressionQuadratic()
{
  DesignClassifierLogisticRegressionC lr(100,FuncQuadraticC(2));
  ClassifierC cv = lr.Apply(dataset.Sample1(),dataset.Sample2());
  RAVL_TEST_TRUE(cv.IsValid());
  UIntT right = 0,wrong = 0;
  for(DataSet2IterC<SampleVectorC,SampleLabelC> it(dataset);it;it++) {
    UIntT label = cv.Classify(it.Data1());
    VectorC conf = cv.Confidence(it.Data1());
    //RavlDebug(" %u -> %s ",label,RavlN::StringOf(conf).c_str());
    if(label == it.Data2()) {
      right ++;
    } else {
      wrong ++;
    }
  }
  //RavlDebug("Right=%u Wrong=%u ",right,wrong);
  RAVL_TEST_TRUE(right == dataset.Size());
  return 0;
}

int testDesignClassifierWeakLinear() {
#if 0
  DesignWeakLinearC lr;
  ClassifierC cv = lr.Apply(dataset.Sample1(),dataset.Sample2());
  RAVL_TEST_TRUE(cv.IsValid());
  UIntT right = 0,wrong = 0;
  for(DataSet2IterC<SampleVectorC,SampleLabelC> it(dataset);it;it++) {
    UIntT label = cv.Classify(it.Data1());
    VectorC conf = cv.Confidence(it.Data1());
    //RavlDebug(" %u -> %s ",label,RavlN::StringOf(conf).c_str());
    if(label == it.Data2()) {
      right ++;
    } else {
      wrong ++;
    }
  }
  //RavlDebug("Right=%u Wrong=%u ",right,wrong);
  RAVL_TEST_TRUE(right == dataset.Size());
#endif
  return 0;
}

int testDesignClassifierBayesNormalLinear() {
  DesignBayesNormalLinearC lr(true);
  ClassifierC cv = lr.Apply(dataset.Sample1(),dataset.Sample2());
  RAVL_TEST_TRUE(cv.IsValid());
  UIntT right = 0,wrong = 0;
  for(DataSet2IterC<SampleVectorC,SampleLabelC> it(dataset);it;it++) {
    UIntT label = cv.Classify(it.Data1());
    VectorC conf = cv.Confidence(it.Data1());
    //RavlDebug(" %u -> %s ",label,RavlN::StringOf(conf).c_str());
    if(label == it.Data2()) {
      right ++;
    } else {
      wrong ++;
    }
  }
  //RavlDebug("Right=%u Wrong=%u ",right,wrong);
  RAVL_TEST_TRUE(right == dataset.Size());
  return 0;
}

int testDesignClassifierBayesNormalQuadratic() {
  DesignBayesNormalQuadraticC lr(true);
  ClassifierC cv = lr.Apply(dataset.Sample1(),dataset.Sample2());
  RAVL_TEST_TRUE(cv.IsValid());
  UIntT right = 0,wrong = 0;
  for(DataSet2IterC<SampleVectorC,SampleLabelC> it(dataset);it;it++) {
    UIntT label = cv.Classify(it.Data1());
    VectorC conf = cv.Confidence(it.Data1());
    //RavlDebug(" %u -> %s ",label,RavlN::StringOf(conf).c_str());
    if(label == it.Data2()) {
      right ++;
    } else {
      wrong ++;
    }
  }
  //RavlDebug("Right=%u Wrong=%u ",right,wrong);
  RAVL_TEST_TRUE(right == dataset.Size());
  return 0;
}


int testDesignClassifierNeuralNetwork2() {
  DesignClassifierNeuralNetwork2C lr(3,4);
  ClassifierC cv = lr.Apply(dataset.Sample1(),dataset.Sample2());
  RAVL_TEST_TRUE(cv.IsValid());
  UIntT right = 0,wrong = 0;
  for(DataSet2IterC<SampleVectorC,SampleLabelC> it(dataset);it;it++) {
    UIntT label = cv.Classify(it.Data1());
    VectorC conf = cv.Confidence(it.Data1());
    //RavlDebug(" %u -> %s ",label,RavlN::StringOf(conf).c_str());
    if(label == it.Data2()) {
      right ++;
    } else {
      wrong ++;
    }
  }
  //RavlDebug("Right=%u Wrong=%u ",right,wrong);
  RAVL_TEST_TRUE(right == dataset.Size());
  ClassifierC cvrl;
  if(!TestBinStreamIO(cv,cvrl))
    return __LINE__;
  right = 0;
  wrong = 0;
  // Check the reloaded one still works.
  for(DataSet2IterC<SampleVectorC,SampleLabelC> it(dataset);it;it++) {
    UIntT label = cvrl.Classify(it.Data1());
    VectorC conf = cvrl.Confidence(it.Data1());
    //RavlDebug(" %u -> %s ",label,RavlN::StringOf(conf).c_str());
    if(label == it.Data2()) {
      right ++;
    } else {
      wrong ++;
    }
  }
  RAVL_TEST_TRUE(right == dataset.Size());

  return 0;
}
