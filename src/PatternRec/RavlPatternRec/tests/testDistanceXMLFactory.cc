// Copyright (C) 2009, OmniPerception Ltd.
// file-header-ends-here
//! lib=RavlPatternRec

#include "Ravl/UnitTest.hh"
#include "Ravl/PatternRec/Distance.hh"

using namespace RavlN;

int testDistanceCityBlock(void);
int testDistanceEuclidean(void);
int testDistanceMahalanobis(void);
int testDistanceMax(void);
int testDistanceRobust(void);
int testDistanceSqrEuclidean(void);
int testDistanceChi2(void);

int main() {
  int ln;

  if((ln = testDistanceCityBlock()) != 0) {
    std::cerr << "Test failed on line " << ln << "\n";
    return 1;
  }

  if((ln = testDistanceEuclidean()) != 0) {
    std::cerr << "Test failed on line " << ln << "\n";
    return 1;
  }

  if((ln = testDistanceMahalanobis()) != 0) {
    std::cerr << "Test failed on line " << ln << "\n";
    return 1;
  }

  if((ln = testDistanceMax()) != 0) {
    std::cerr << "Test failed on line " << ln << "\n";
    return 1;
  }

  if((ln = testDistanceRobust()) != 0) {
    std::cerr << "Test failed on line " << ln << "\n";
    return 1;
  }

  if((ln = testDistanceSqrEuclidean()) != 0) {
    std::cerr << "Test failed on line " << ln << "\n";
    return 1;
  }

  if((ln = testDistanceChi2()) != 0) {
    std::cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  
  std::cout << "Test passed. \n";
  return 0;
}



int testDistanceCityBlock(void) {
  RavlN::StrIStreamC
    ss(
       "<?xml version='1.0' encoding='UTF-8' ?>\n"
       "<?RAVL class='RavlN::XMLTreeC' ?>\n"
       "<Config >\n"
       "<Distance typename=\"RavlN::DistanceCityBlockC\"  />"         
       "</Config>");
  RavlN::XMLTreeC xmlTree(true);
  if (!xmlTree.Read(ss))
    return __LINE__;
  RavlN::XMLFactoryHC mainFactory("test.xml", xmlTree);
  RavlN::XMLFactoryContextC context(mainFactory);
  DistanceC distance;
  // get connection to database
  if(!context.UseComponent("Distance", distance, true)) {
    return __LINE__;
  }  
  return 0;
}


int testDistanceEuclidean(void) {
  RavlN::StrIStreamC
    ss(
       "<?xml version='1.0' encoding='UTF-8' ?>\n"
       "<?RAVL class='RavlN::XMLTreeC' ?>\n"
       "<Config >\n"
       "<Distance typename=\"RavlN::DistanceEuclideanC\"  />"         
       "</Config>");
  RavlN::XMLTreeC xmlTree(true);
  if (!xmlTree.Read(ss))
    return __LINE__;
  RavlN::XMLFactoryHC mainFactory("test.xml", xmlTree);
  RavlN::XMLFactoryContextC context(mainFactory);
  DistanceC distance;
  // get connection to database
  if(!context.UseComponent("Distance", distance, true)) {
    return __LINE__;
  }  
  return 0;
}


int testDistanceMax(void) {
  RavlN::StrIStreamC
    ss(
       "<?xml version='1.0' encoding='UTF-8' ?>\n"
       "<?RAVL class='RavlN::XMLTreeC' ?>\n"
       "<Config >\n"
       "<Distance typename=\"RavlN::DistanceMaxC\"  />"         
       "</Config>");
  RavlN::XMLTreeC xmlTree(true);
  if (!xmlTree.Read(ss))
    return __LINE__;
  RavlN::XMLFactoryHC mainFactory("test.xml", xmlTree);
  RavlN::XMLFactoryContextC context(mainFactory);
  DistanceC distance;
  // get connection to database
  if(!context.UseComponent("Distance", distance, true)) {
    return __LINE__;
  }  
  return 0;
}

int testDistanceMahalanobis(void) {
  RavlN::StrIStreamC
    ss(
       "<?xml version='1.0' encoding='UTF-8' ?>\n"
       "<?RAVL class='RavlN::XMLTreeC' ?>\n"
       "<Config >\n"
       "<Distance typename=\"RavlN::DistanceMahalanobisC\"  >"
       "<covariance_matrix typename=\"RavlN::MatrixC\" values=\"2 2 1.0 0.0 0.0 1.0\" />"
       "</Distance>"
       "</Config>");
  RavlN::XMLTreeC xmlTree(true);
  if (!xmlTree.Read(ss))
    return __LINE__;
  RavlN::XMLFactoryHC mainFactory("test.xml", xmlTree);
  RavlN::XMLFactoryContextC context(mainFactory);
  DistanceC distance;
  // get connection to database
  if(!context.UseComponent("Distance", distance, true)) {
    return __LINE__;
  }  
  return 0;
}

int testDistanceRobust(void) {
  RavlN::StrIStreamC
    ss(
       "<?xml version='1.0' encoding='UTF-8' ?>\n"
       "<?RAVL class='RavlN::XMLTreeC' ?>\n"
       "<Config >\n"
       "<Distance typename=\"RavlN::DistanceRobustC\" clip_limit=\"8.0\">"
       "<metric typename=\"RavlN::DistanceMaxC\" />"
       "</Distance>"
       "</Config>");
  RavlN::XMLTreeC xmlTree(true);
  if (!xmlTree.Read(ss))
    return __LINE__;
  RavlN::XMLFactoryHC mainFactory("test.xml", xmlTree);
  RavlN::XMLFactoryContextC context(mainFactory);
  DistanceC distance;
  // get connection to database
  if(!context.UseComponent("Distance", distance, true)) {
    return __LINE__;
  }  
  return 0;
}

int testDistanceSqrEuclidean(void) {
  RavlN::StrIStreamC
    ss(
       "<?xml version='1.0' encoding='UTF-8' ?>\n"
       "<?RAVL class='RavlN::XMLTreeC' ?>\n"
       "<Config >\n"
       "<Distance typename=\"RavlN::DistanceSqrEuclideanC\" />"
       "</Config>");
  RavlN::XMLTreeC xmlTree(true);
  if (!xmlTree.Read(ss))
    return __LINE__;
  RavlN::XMLFactoryHC mainFactory("test.xml", xmlTree);
  RavlN::XMLFactoryContextC context(mainFactory);
  DistanceC distance;
  // get connection to database
  if(!context.UseComponent("Distance", distance, true)) {
    return __LINE__;
  }  
  return 0;
}

int testDistanceChi2(void) {
  RavlN::StrIStreamC
    ss(
       "<?xml version='1.0' encoding='UTF-8' ?>\n"
       "<?RAVL class='RavlN::XMLTreeC' ?>\n"
       "<Config >\n"
       "<Distance typename=\"RavlN::DistanceChi2C\" />"
       "</Config>");
  RavlN::XMLTreeC xmlTree(true);
  if (!xmlTree.Read(ss))
    return __LINE__;
  RavlN::XMLFactoryHC mainFactory("test.xml", xmlTree);
  RavlN::XMLFactoryContextC context(mainFactory);
  DistanceC distance;
  // get connection to database
  if(!context.UseComponent("Distance", distance, true)) {
    return __LINE__;
  }  
  return 0;
}

