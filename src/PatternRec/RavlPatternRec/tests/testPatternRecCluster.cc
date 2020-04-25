

#include "Ravl/UnitTest.hh"
#include "Ravl/SysLog.hh"
#include "Ravl/PatternRec/ClassifierNearestNeighbour.hh"

int testClassifierNearestNeighbourIO();

using namespace RavlN;

int main(int nargs,char **argv)
{
  SysLogOpen(argv[0],false,true,true,-1,true);

  RAVL_RUN_TEST(testClassifierNearestNeighbourIO());

  return 0;
}


int testClassifierNearestNeighbourIO()
{
  SampleVectorC vectors(2);
  vectors.Append(VectorC(1,2,3));
  vectors.Append(VectorC(3,2,1));
  SampleLabelC labels(2);
  labels.Append(1);
  labels.Append(2);
  ClassifierNearestNeighbourC value(vectors,
                                    labels);
  ClassifierNearestNeighbourC valueRL;
  if(!TestBinStreamIO(value,valueRL))
    return __LINE__;
  RAVL_TEST_EQUALS(value.Vectors().Size(),valueRL.Vectors().Size());
  RAVL_TEST_EQUALS(value.Labels().Size(),valueRL.Labels().Size());
  return 0;
}
