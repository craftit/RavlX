// Copyright (C) 2009, OmniPerception Ltd.
// file-header-ends-here
//! lib=RavlPatternRec

#include "Ravl/UnitTest.hh"
#include "Ravl/PatternRec/SampleVector.hh"
#include "Ravl/PatternRec/DataSetVectorLabel.hh"
#include "Ravl/MeanCovariance.hh"

using namespace RavlN;

int testSampleVector(void);
int testDataSetVectorLabel(void);


int main() {
  int ln;

  if((ln = testSampleVector()) != 0) {
    std::cerr << "Test failed on line " << ln << "\n";
    return 1;
  }

  if((ln = testDataSetVectorLabel()) != 0) {
    std::cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  
  std::cout << "Test passed. \n";
  return 0;
}



int testSampleVector(void) {
  RavlN::StrIStreamC
    ss(
       "<?xml version='1.0' encoding='UTF-8' ?>\n"
       "<?RAVL class='RavlN::XMLTreeC' ?>\n"
       "<Config >\n"
       "<Sample typename=\"RavlN::SampleVectorC\" >"
       "<stats typename=\"RavlN::MeanCovarianceC\" samples=\"10000\" mean=\"3 4 5 -9\" covariance=\"3 3 4 3 0 3 5 0 0 0 10\" />"
       "</Sample>"
       "</Config>");
  RavlN::XMLTreeC xmlTree(true);
  if (!xmlTree.Read(ss))
    return __LINE__;
  RavlN::XMLFactoryHC mainFactory("test.xml", xmlTree);
  RavlN::XMLFactoryContextC context(mainFactory);
  SampleVectorC sampleVector;
  // get connection to database
  if(!context.UseComponent("Sample", sampleVector, true)) {
    return __LINE__;
  }
  
  // compute mean and covariance of generated data set
  MeanCovarianceC meanCovariance = sampleVector.MeanCovariance();
  
  RealT tolerance = 0.5;
  
  // do some tests
  if(!TestAlmostEquals(0.0, meanCovariance.Mean().Sum(), tolerance, __FILE__, __LINE__)) {
    return __LINE__;
  }  

  // do some tests
  if(!TestAlmostEquals(25.0, meanCovariance.Covariance().SumOfAbs(), tolerance, __FILE__, __LINE__)) {
    return __LINE__;
  }  
  
  return 0;
}

int testDataSetVectorLabel(void) {
  RavlN::StrIStreamC
    ss(
       "<?xml version='1.0' encoding='UTF-8' ?>\n"
       "<?RAVL class='RavlN::XMLTreeC' ?>\n"
       "<Config >\n"
       "<DataSet typename=\"RavlN::DataSetVectorLabelC\">"
       "<Sample1 typename=\"RavlN::SampleVectorC\" >"
       "<stats typename=\"RavlN::MeanCovarianceC\" samples=\"1000\" mean=\"2 1 5\" covariance=\"2 2 1 0 0 1\" />"
       "</Sample1>"
       "<Sample2 typename=\"RavlN::SampleVectorC\" >"
       "<stats typename=\"RavlN::MeanCovarianceC\" samples=\"1000\" mean=\"2 10 50\" covariance=\"2 2 1 0 0 1\" />"
       "</Sample2>"
       "</DataSet>"
       "</Config>");
  RavlN::XMLTreeC xmlTree(true);
  if (!xmlTree.Read(ss))
    return __LINE__;
  RavlN::XMLFactoryHC mainFactory("test.xml", xmlTree);
  RavlN::XMLFactoryContextC context(mainFactory);
  DataSetVectorLabelC dset;
  // get connection to database
  if(!context.UseComponent("DataSet", dset, true)) {
    return __LINE__;
  }
  return 0;
}

