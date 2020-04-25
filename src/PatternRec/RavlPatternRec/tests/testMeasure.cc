// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Distance/testMeasure.cc"
//! docentry="Ravl.API.Pattern Recognition.Distance"
//! author="Charles Galambos"

#include "Ravl/PatternRec/DistanceEuclidean.hh"
#include "Ravl/PatternRec/DistanceCorrelation.hh"
#include "Ravl/Stream.hh"
#include "Ravl/StrStream.hh"
#include "Ravl/EntryPnt.hh"
#include "Ravl/UnitTest.hh"

using namespace RavlN;

int testEuclidian();
int testEuclidianIO();
int testCorrelationIO();

int testMeasure(int args,char **argv) {
  int ln;
  RAVL_RUN_TEST(testEuclidian());
  RAVL_RUN_TEST(testEuclidianIO());
  RAVL_RUN_TEST(testCorrelationIO());
  RavlInfo("Test passed. ");
  return 0;
}

RAVL_ENTRY_POINT(testMeasure);

int testEuclidian()
{
  std::cerr << "testEuclidian(). Started. \n";  
  DistanceEuclideanC de;
  if(Abs(de.Measure(VectorC(1,0),VectorC(2,0)) - 1) > 0.000000001)
    return __LINE__;
  return 0;
}

int testEuclidianIO()
{
  DistanceEuclideanC de;
  StrOStreamC os;
  os << de;
  StrIStreamC is(os.String());
  DistanceC de2;
  is >> de2;
  if(!de2.IsValid()) return __LINE__;
  DistanceEuclideanC tst(de2);
  if(!tst.IsValid()) return __LINE__;

  DistanceEuclideanC deRL;
  RAVL_TEST_TRUE(TestBinStreamIO(de,deRL));

  return 0;
}

int testCorrelationIO()
{
  DistanceCorrelationC dc(false);
  DistanceCorrelationC dcRL;

  RAVL_TEST_TRUE(TestBinStreamIO(dc,dcRL));
  RAVL_TEST_EQUALS(dc.UseNormalise(),dcRL.UseNormalise());

  return 0;
}
