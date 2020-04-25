// Copyright (C) 2009, OmniPerception Ltd.
// file-header-ends-here
//! lib=RavlPatternRec

#include "Ravl/UnitTest.hh"
#include "Ravl/PatternRec/DataSetVectorLabel.hh"
#include "Ravl/PatternRec/DesignClassifierSupervised.hh"
#include "Ravl/PatternRec/Error.hh"

using namespace RavlN;

int setup(void);
int testOneAgainstAll(void);

DataSetVectorLabelC g_train;
DataSetVectorLabelC g_test;

int main() {
  int ln;

  if((ln = setup()) != 0) {
    std::cerr << "Test failed on line " << ln << "\n";
    return 1;
  }

  if((ln = testOneAgainstAll()) != 0) {
    std::cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  
  std::cout << "Test passed. \n";
  return 0;
}



int setup(void) {
  RavlN::StrIStreamC
    ss(
       "<?xml version='1.0' encoding='UTF-8' ?>\n"
       "<?RAVL class='RavlN::XMLTreeC' ?>\n"
       "<Config >\n"

       "<TrainDataSet typename=\"RavlN::DataSetVectorLabelC\">"
       "<Sample1 typename=\"RavlN::SampleVectorC\" >"
       "<stats typename=\"RavlN::MeanCovarianceC\" samples=\"50\" mean=\"2 0 0\" covariance=\"2 2 1 0 0 1\" />"
       "</Sample1>"
       "<Sample2 typename=\"RavlN::SampleVectorC\" >"
       "<stats typename=\"RavlN::MeanCovarianceC\" samples=\"50\" mean=\"2 6 -2\" covariance=\"2 2 1 0 0 2\" />"
       "</Sample2>"
       "<Sample3 typename=\"RavlN::SampleVectorC\" >"
       "<stats typename=\"RavlN::MeanCovarianceC\" samples=\"50\" mean=\"2 12 0\" covariance=\"2 2 1 0 0 2\" />"
       "</Sample3>"
       "<Sample4 typename=\"RavlN::SampleVectorC\" >"
       "<stats typename=\"RavlN::MeanCovarianceC\" samples=\"50\" mean=\"2 18 2\" covariance=\"2 2 2 0 0 0.5\" />"
       "</Sample4>"
       "<Sample5 typename=\"RavlN::SampleVectorC\" >"
       "<stats typename=\"RavlN::MeanCovarianceC\" samples=\"50\" mean=\"2 24 0\" covariance=\"2 2 1 0 0 1\" />"
       "</Sample5>"
       "</TrainDataSet>"

       "<TestDataSet typename=\"RavlN::DataSetVectorLabelC\">"
       "<Sample1 typename=\"RavlN::SampleVectorC\" >"
       "<stats typename=\"RavlN::MeanCovarianceC\" samples=\"50\" mean=\"2 0 0\" covariance=\"2 2 1 0 0 1\" />"
       "</Sample1>"
       "<Sample2 typename=\"RavlN::SampleVectorC\" >"
       "<stats typename=\"RavlN::MeanCovarianceC\" samples=\"50\" mean=\"2 6 -2\" covariance=\"2 2 1 0 0 2\" />"
       "</Sample2>"
       "<Sample3 typename=\"RavlN::SampleVectorC\" >"
       "<stats typename=\"RavlN::MeanCovarianceC\" samples=\"50\" mean=\"2 12 0\" covariance=\"2 2 1 0 0 2\" />"
       "</Sample3>"
       "<Sample4 typename=\"RavlN::SampleVectorC\" >"
       "<stats typename=\"RavlN::MeanCovarianceC\" samples=\"50\" mean=\"2 18 2\" covariance=\"2 2 2 0 0 0.5\" />"
       "</Sample4>"
       "<Sample5 typename=\"RavlN::SampleVectorC\" >"
       "<stats typename=\"RavlN::MeanCovarianceC\" samples=\"50\" mean=\"2 24 0\" covariance=\"2 2 1 0 0 1\" />"
       "</Sample5>"
       "</TestDataSet>"

       "</Config>");
  RavlN::XMLTreeC xmlTree(true);
  if (!xmlTree.Read(ss))
    return __LINE__;
  RavlN::XMLFactoryHC mainFactory("test.xml", xmlTree);
  RavlN::XMLFactoryContextC context(mainFactory);

  // Get training dataset
  if(!context.UseComponent("TrainDataSet", g_train, true)) {
    return __LINE__;
  }

  // Get test dataset that doesnt overlap
  if(!context.UseComponent("TestDataSet", g_test, true)) {
    return __LINE__;
  }

  return 0;
  
}

int testOneAgainstAll(void) {
  RavlN::StrIStreamC
    ss(
       "<?xml version='1.0' encoding='UTF-8' ?>\n"
       "<?RAVL class='RavlN::XMLTreeC' ?>\n"
       "<Config >\n"

       "<Design typename=\"RavlN::DesignOneAgainstAllC\">"
       "<Classifier  typename=\"RavlN::DesignSvmSmoC\" lambdaThreshold=\"1e-7\" tolerance=\"1e-2\" eps=\"1e-5\" >"
       "<KernelFunction typename=\"RavlN::QuadraticKernelC\" scale=\"1.0\" />"
       "</Classifier>"
       "</Design>"
       
       "</Config>");
  RavlN::XMLTreeC xmlTree(true);
  if (!xmlTree.Read(ss))
    return __LINE__;
  RavlN::XMLFactoryHC mainFactory("test.xml", xmlTree);
  RavlN::XMLFactoryContextC context(mainFactory);

  // Get classifier designer
  DesignClassifierSupervisedC design;
  if(!context.UseComponent("Design", design, true)) {
    return __LINE__;
  }

  
  ClassifierC classifier = design.Apply(g_train.Sample1(), g_train.Sample2());
  
  ErrorC performance;
  std::cerr << performance.Error(classifier, g_test);
#if 0
  if(!TestAlmostEquals(0.0, performance.Error(classifier, g_test), 1e-10, __FILE__, __LINE__)) {
    return __LINE__;
  }
#endif

  
  return 0;
}
