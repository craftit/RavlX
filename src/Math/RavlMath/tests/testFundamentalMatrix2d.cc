// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Projective/2D/testFundamentalMatrix2d.cc"

#include "Ravl/FundamentalMatrix2d.hh"
#include "Ravl/PProjection2d.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/Random.hh"
#include "Ravl/Vector3d.hh"
#include "Ravl/LineABC2d.hh"

using namespace RavlN;

int testBasic();
int testPProjection();
int testCorrectCorrespondence();

int main() {
  int ln;
#if 1
  if((ln = testBasic()) != 0) {
    cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
#endif
  if((ln = testPProjection()) != 0) {
    cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  if((ln = testCorrectCorrespondence()) != 0) {
    cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  cerr << "Test passed. \n";
  return 0;
}

int testBasic() {
  cerr << "Testing fundanmental matrix estimation.\n";
  int n = 8;
  SArray1dC<PPoint2dC> pnts1(n);
  SArray1dC<PPoint2dC> pnts2(n);

  FundamentalMatrix2dC fm1;

  for(int i = 0;i < 3;i++)
    for(int j = 0;j < 3;j++)
      fm1[i][j] = Random1();
  fm1.MakeRank2();
  fm1.NormaliseScale();
  cerr << "Fm1=" << fm1 << "\n";

  // Check epipoles.
  PPoint2dC ep1 = fm1.Epipole1();
  PPoint2dC tep1 = fm1 * ep1;
  cerr << "Ep1=" << ep1 << " tep1=" << tep1 << "\n";
  if(tep1.SumOfAbs() > 0.00000001) return __LINE__;
  PPoint2dC ep2 = fm1.Epipole2();
  PPoint2dC tep2 = fm1.T() * ep2;
  cerr << "Ep2=" << ep2 << " tep2=" << tep2 << "\n";
  if(tep2.SumOfAbs() > 0.00000001) return __LINE__;

  for(int i =0;i < n;i++) {
    PPoint2dC p1(Random1() * 10,Random1() * 10,1);
    pnts1[i] = p1;
    LineABC2dC l2 = fm1.EpipolarLine(p1);
    Point2dC xp2 = l2.Projection(Point2dC(Random1() * 10 ,Random1() * 10));
    PPoint2dC p2(xp2);
    pnts2[i] = p2;
    cerr << i << ": " << pnts1[i] << " <->  " << pnts2[i] << "    " << fm1.Error(p1,p2) << "\n";
  }
  cerr << "\n";
  SArray1dC<PPoint2dC> cps1 = pnts1.Copy();
  SArray1dC<PPoint2dC> cps2 = pnts2.Copy();

  FundamentalMatrix2dC fm2 = FundamentalMatrix2dC::FitLinear(cps1,cps2);
  cerr << "fm2=" << fm2 << "\n";
  for(int i = 0;i < n;i++) {
    RealT err1 = fm1.Error(pnts1[i],pnts2[i]);
    RealT err2 = fm2.Error(pnts1[i],pnts2[i]);

    cerr << i << ": Err1=" << err1 << " Err2=" << err2 << "\n";
    //RealT x = fm2(pnts1[i]).Distance(pnts2[i]);
    //cerr << "x=" << x <<" \n";
    if(err1 > 0.00001 || err2 > 0.000001) return __LINE__;

  }

  cerr << "\nFm2=" << fm2 << "\n";
  return 0;
}

int testPProjection() {
  cerr << "Testing projective fitting. \n";
  SArray1dC<PPoint2dC> from(5);
  SArray1dC<PPoint2dC> to(5);

  // Do a simple test.

  from[0] = PPoint2dC(1,1,1); to[0] = PPoint2dC(2,2,1);
  from[1] = PPoint2dC(1,2,1); to[1] = PPoint2dC(2,4,1);
  from[2] = PPoint2dC(2,1,1); to[2] = PPoint2dC(3,2,1);
  from[3] = PPoint2dC(2,2,1); to[3] = PPoint2dC(3,4,1);
  from[4] = PPoint2dC(1,1,1); to[4] = PPoint2dC(2,2,1);
  PProjection2dC pp;
  if(!FitPProjection(from,to,pp))
    return __LINE__;

  for(SArray1dIter2C<PPoint2dC,PPoint2dC> it(from,to);it;it++) {
    PPoint2dC x = pp * it.Data1();
    //cerr << "x=" << x <<"  " << x.Point2d() << "\n";
    if((x.Point2d() - it.Data2().Point2d()).SumOfAbs() > 0.000001)
      return __LINE__;
  }
  //cerr << "Pp=" << pp << "\n";

  // Try a more complex transform.

  for(SArray1dIter2C<PPoint2dC,PPoint2dC> it(from,to);it;it++) {
    it.Data1() = PPoint2dC(Random1(),Random1(),1);
    it.Data2() = PPoint2dC(Random1(),Random1(),1);
  }
  from[4] = from[3]; to[4] = to[3]; // Duplicate last entry to ensure unique transform

  if(!FitPProjection(from,to,pp))
    return __LINE__;

  for(SArray1dIter2C<PPoint2dC,PPoint2dC> it(from,to);it;it++) {
    PPoint2dC x = pp * it.Data1();
    //cerr << "x=" << x <<"  " << x.Point2d() << "\n";
    if((x.Point2d() - it.Data2().Point2d()).SumOfAbs() > 0.000001)
      return __LINE__;
  }


  return 0;
}

int testCorrectCorrespondence() {
  cerr << "Testing correct correspondence. \n";

  PPoint2dC ipnt1(0, 0, 1);
  PPoint2dC ipnt2(0, 0, 1);
  PPoint2dC opnt1;
  PPoint2dC opnt2;
  FundamentalMatrix2dC F1(0, -1, 0,
                         1, 2, -1,
                        0, 1, 0);
  F1.CorrectCorrespondence(ipnt1, ipnt2, opnt1, opnt2);
  // test exact match
  if (opnt1.EuclidDistance(PPoint2dC(0,0,1)) > 1e-6 || opnt2.EuclidDistance(PPoint2dC(0,0,1)) > 1e-6)
    return __LINE__;
  FundamentalMatrix2dC F2(4, -3, -4,
                         -3, 2, 3,
                         -4, 3, 4);
  F2.CorrectCorrespondence(ipnt1, ipnt2, opnt1, opnt2);
  PPoint2dC opnta(1.77184, -1.33111, 2.77184);
  PPoint2dC opntb(4.4658e-5, -0.00225733, 0.114146);
  RealT dist1 = opnt2.EuclidDistance(opnta);
  RealT dist2 = opnt1.EuclidDistance(opntb);
  RealT dist3 = opnt2.EuclidDistance(opntb);
  RealT dist4 = opnt1.EuclidDistance(opnta);
  // The solution can change between these two due to numerical instabilities
  if (!((dist1 < 1e-5 && dist2 < 1e-5) ||
      (dist3 < 1e-5 && dist4 < 1e-5)))
    return __LINE__;
  return 0;
}
