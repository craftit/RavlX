// Copyright (C) 2009, OmniPerception Ltd.
// file-header-ends-here
//! lib=RavlSVM

#include "Ravl/UnitTest.hh"
#include "Ravl/PatternRec/DataSetVectorLabel.hh"
#include "Ravl/PatternRec/DesignSvm.hh"
#include "Ravl/PatternRec/Error.hh"

using namespace RavlN;

int testDesignOneClass(void);
int testDesignOneClassLarge(void);
int testDesignSvmSmo(void);
int testDesignSvmSmoLarge(void);
int setup(void);

DataSetVectorLabelC g_train;
DataSetVectorLabelC g_testOverlap;
DataSetVectorLabelC g_testNonOverlap;

int main() {
  int ln;

  if((ln = setup()) != 0) {
    std::cerr << "Test failed on line " << ln << "\n";
    return 1;
  }

  if((ln = testDesignOneClass()) != 0) {
    std::cerr << "Test failed on line " << ln << "\n";
    return 1;
  }

  if((ln = testDesignOneClassLarge()) != 0) {
    std::cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  
  if((ln = testDesignSvmSmo()) != 0) {
    std::cerr << "Test failed on line " << ln << "\n";
    return 1;
  }

  if((ln = testDesignSvmSmoLarge()) != 0) {
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
       "<stats typename=\"RavlN::MeanCovarianceC\" samples=\"1000\" mean=\"2 0 0\" covariance=\"2 2 1 0 0 1\" />"
       "</Sample1>"
       "<Sample2 typename=\"RavlN::SampleVectorC\" >"
       "<stats typename=\"RavlN::MeanCovarianceC\" samples=\"1000\" mean=\"2 5 5\" covariance=\"2 2 1 0 0 1\" />"
       "</Sample2>"
       "</TrainDataSet>"

       "<TestNonOverlap typename=\"RavlN::DataSetVectorLabelC\">"
       "<Sample1 typename=\"RavlN::SampleVectorC\" >"
       "<stats typename=\"RavlN::MeanCovarianceC\" samples=\"1000\" mean=\"2 0 0\" covariance=\"2 2 1 0 0 1\" />"
       "</Sample1>"
       "<Sample2 typename=\"RavlN::SampleVectorC\" >"
       "<stats typename=\"RavlN::MeanCovarianceC\" samples=\"1000\" mean=\"2 5 5\" covariance=\"2 2 1 0 0 1\" />"
       "</Sample2>"
       "</TestNonOverlap>"
       
       "<TestOverlap typename=\"RavlN::DataSetVectorLabelC\">"
       "<Sample1 typename=\"RavlN::SampleVectorC\" >"
       "<stats typename=\"RavlN::MeanCovarianceC\" samples=\"1000\" mean=\"2 0 0\" covariance=\"2 2 1 0 0 1\" />"
       "</Sample1>"
       "<Sample2 typename=\"RavlN::SampleVectorC\" >"
       "<stats typename=\"RavlN::MeanCovarianceC\" samples=\"1000\" mean=\"2 0 0\" covariance=\"2 2 1 0 0 1\" />"
       "</Sample2>"
       "</TestOverlap>"

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
  if(!context.UseComponent("TestNonOverlap", g_testNonOverlap, true)) {
    return __LINE__;
  }

  // Get test dataset that doesnt overlap
  if(!context.UseComponent("TestOverlap", g_testOverlap, true)) {
    return __LINE__;
  }

  return 0;
  
}

int testDesignOneClass(void) {
  RavlN::StrIStreamC
    ss(
       "<?xml version='1.0' encoding='UTF-8' ?>\n"
       "<?RAVL class='RavlN::XMLTreeC' ?>\n"
       "<Config >\n"
       
       "<Design  typename=\"RavlN::DesignOneClassC\" >"
       "<KernelFunction typename=\"RavlN::LinearKernelC\" scale=\"1.0\" />"
       "</Design>"
       
       "</Config>");
  RavlN::XMLTreeC xmlTree(true);
  if (!xmlTree.Read(ss))
    return __LINE__;
  RavlN::XMLFactoryHC mainFactory("test.xml", xmlTree);
  RavlN::XMLFactoryContextC context(mainFactory);

  // Get classifier designer
  DesignSvmC design;
  if(!context.UseComponent("Design", design, true)) {
    return __LINE__;
  }
  Classifier2C classifier = design.Apply(g_train.Sample1(), g_train.Sample2());
  
  ErrorC performance;
  if(!TestAlmostEquals(0.0, performance.Error(classifier, g_testNonOverlap), 1e-2, __FILE__, __LINE__)) {
    return __LINE__;
  }

  if(!TestAlmostEquals(0.5, performance.Error(classifier, g_testOverlap), 1e-2, __FILE__, __LINE__)) {
    return __LINE__;
  }

  RealT threshold=0.5;
  UIntT label=0;
  if(!TestAlmostEquals(0.0, performance.Error(classifier, g_testNonOverlap, threshold, label), 1e-2, __FILE__, __LINE__)) {
    return __LINE__;
  }

  
  return 0;
}

int testDesignOneClassLarge(void) {
  RavlN::StrIStreamC
    ss(
       "<?xml version='1.0' encoding='UTF-8' ?>\n"
       "<?RAVL class='RavlN::XMLTreeC' ?>\n"
       "<Config >\n"
       
       "<Design  typename=\"RavlN::DesignOneClassLargeC\" max_num_sv=\"100\" >"
       "<KernelFunction typename=\"RavlN::LinearKernelC\" scale=\"1.0\" />"
       "</Design>"
       
       "</Config>");
  RavlN::XMLTreeC xmlTree(true);
  if (!xmlTree.Read(ss))
    return __LINE__;
  RavlN::XMLFactoryHC mainFactory("test.xml", xmlTree);
  RavlN::XMLFactoryContextC context(mainFactory);

  // Get classifier designer
  DesignSvmC design;
  if(!context.UseComponent("Design", design, true)) {
    return __LINE__;
  }
  Classifier2C classifier = design.Apply(g_train.Sample1(), g_train.Sample2());
  
  ErrorC performance;
  if(!TestAlmostEquals(0.0, performance.Error(classifier, g_testNonOverlap), 1e-2, __FILE__, __LINE__)) {
    return __LINE__;
  }

  if(!TestAlmostEquals(0.5, performance.Error(classifier, g_testOverlap), 1e-2, __FILE__, __LINE__)) {
    return __LINE__;
  }

  RealT threshold=0.5;
  UIntT label=0;
  if(!TestAlmostEquals(0.0, performance.Error(classifier, g_testNonOverlap, threshold, label), 1e-2, __FILE__, __LINE__)) {
    return __LINE__;
  }

  
  return 0;
}


int testDesignSvmSmo(void) {
  RavlN::StrIStreamC
    ss(
       "<?xml version='1.0' encoding='UTF-8' ?>\n"
       "<?RAVL class='RavlN::XMLTreeC' ?>\n"
       "<Config >\n"
       
       "<Design  typename=\"RavlN::DesignSvmSmoC\" >"
       "<KernelFunction typename=\"RavlN::LinearKernelC\" scale=\"1.0\" />"
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
  if(!TestAlmostEquals(0.0, performance.Error(classifier, g_testNonOverlap), 1e-10, __FILE__, __LINE__)) {
    return __LINE__;
  }

  if(!TestAlmostEquals(0.5, performance.Error(classifier, g_testOverlap), 1e-10, __FILE__, __LINE__)) {
    return __LINE__;
  }
  
  return 0;
}

int testDesignSvmSmoLarge(void) {
  RavlN::StrIStreamC
    ss(
       "<?xml version='1.0' encoding='UTF-8' ?>\n"
       "<?RAVL class='RavlN::XMLTreeC' ?>\n"
       "<Config >\n"
       
       "<Design  typename=\"RavlN::DesignSvmSmoLargeC\" >"
       "<KernelFunction typename=\"RavlN::LinearKernelC\" scale=\"1.0\" />"
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
  if(!TestAlmostEquals(0.0, performance.Error(classifier, g_testNonOverlap), 1e-10, __FILE__, __LINE__)) {
    return __LINE__;
  }

  if(!TestAlmostEquals(0.5, performance.Error(classifier, g_testOverlap), 1e-10, __FILE__, __LINE__)) {
    return __LINE__;
  }
  
  return 0;
}


