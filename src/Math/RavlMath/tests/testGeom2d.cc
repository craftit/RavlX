// This file is part of RAVL, Recognition And Vision Library 
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Euclidean/2D/testGeom2d.cc"
//! docentry="Ravl.API.Math.Geometry.2D"
//! author="Charles Galambos"
//! userlevel=Develop

#include "Ravl/Moments2d2.hh"
#include "Ravl/Stream.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/StrStream.hh"
#include "Ravl/Matrix2d.hh"
#include "Ravl/Polygon2d.hh"
#include "Ravl/Circle2d.hh"
#include "Ravl/Array1d.hh"
#include "Ravl/Affine2d.hh"
#include "Ravl/Random.hh"
#include "Ravl/DelaunayTriangulation2d.hh"
#include "Ravl/HEMesh2d.hh"
#include "Ravl/TriMesh2d.hh"
#include "Ravl/Projection2d.hh"
#include "Ravl/LineABC2d.hh"
#include "Ravl/Conic2d.hh"
#include "Ravl/Ellipse2d.hh"
#include "Ravl/Triangulate2d.hh"
#include "Ravl/Angle.hh"
#include "Ravl/ScanPolygon2d.hh"
#include "Ravl/Collection.hh"
#include "Ravl/DListExtra.hh"
#include "Ravl/SArray1dIter2.hh"

using namespace RavlN;

int testMoments();
int testBinIO();
int testCircle2d();
int testConvexHull2d();
int testDelaunayTriangulation2d();
int testCompoundAffine();
int testFitAffine();
int testFitSimilarity();
int testAffine();
int testHEMesh2d();
int testProjective2d();
int testLineProjective2d();
int testLines2d();
int testConic2d();
int testEllipse2dA();
int testEllipse2dB();
int testEllipse2dC();
int testEllipse2dD();
int testScanPolygon();
int testOverlap();
int testPolygonClip();
int testTriangulate2d();

#define TEST(f) \
  if((ln = f()) != 0) {\
    cerr << "Test failed at " << ln << "\n";\
    return 1;\
  }

int main() {
  int ln;
  
#if 0
  TEST(testMoments);
  TEST(testBinIO);
  TEST(testCircle2d);
  TEST(testConvexHull2d);
  TEST(testDelaunayTriangulation2d);
  TEST(testCompoundAffine);
  TEST(testFitAffine);
  TEST(testFitSimilarity);
  TEST(testHEMesh2d);
  TEST(testProjective2d);
  TEST(testLineProjective2d);
  TEST(testLines2d);
  TEST(testConic2d);
  TEST(testEllipse2dA);
  TEST(testEllipse2dB);
  TEST(testEllipse2dC);
  TEST(testEllipse2dD);
  TEST(testOverlap);
  TEST(testPolygonClip);
  TEST(testAffine);
  TEST(testTriangulate2d);
#endif
  TEST(testScanPolygon);
  cout << "Test passed. \n";
  return 0;
}


int testMoments() {
  cerr << "testMoments Called.\n";
  Moments2d2C mom;
  mom.AddPixel(Index2dC(9,19));
  mom.AddPixel(Index2dC(10,20));
  mom.AddPixel(Index2dC(11,22));
  RealT cx = mom.CentroidX();
  RealT cy = mom.CentroidY();
  //cerr << "Centroid=" << cx << " " << cy <<"\n";
  if((Abs(cx - 10) > 0.001) || (Abs(cy - 20.3333) > 0.001))
     return __LINE__;
  Vector2dC principleAxis = mom.PrincipalAxisSize();
  //cerr << "PrincipleAxis=" << principleAxis  <<"\n"; 
  RealT elong = mom.Elongatedness(principleAxis);
  //cerr <<"Elong=" << elong << "\n";
  if(Abs(elong - 0.984886) > 0.001) return __LINE__;
  return 0;
}

int testBinIO() {
  cerr << "testBinIO Called.\n";
  StrOStreamC ostr;
  Point2dC p1(0.12,0.34);
  Point2dC p2(4.5,6.7);
  Point2dC p3(8.9,10.11);
  {
    BinOStreamC bos(ostr);
    bos << p1 << p2 << p3;
  }
  Point2dC ip1,ip2,ip3;
  {
    StrIStreamC istr(ostr.String());
    BinIStreamC bis(istr);
    bis >> ip1 >> ip2 >> ip3;
  }
  if((p1 - ip1).SumOfSqr() > 0.000000001)
    return __LINE__;
  if((p2 - ip2).SumOfSqr() > 0.000000001)
    return __LINE__;
  if((p3 - ip3).SumOfSqr() > 0.000000001)
    return __LINE__;
  return 0;
}

int testCircle2d() {
  cerr << "testCircle2d Called. \n";
  Array1dC<Point2dC> pnts(5);
  pnts[0] = Point2dC(1,0);
  pnts[1] = Point2dC(-1,2);
  pnts[2] = Point2dC(3,2);
  pnts[3] = Point2dC(1,4);
  pnts[4] = Point2dC(1,4.01); // Just to add a slight error.
  
  Circle2dC circle;
  
  RealT residual;
  if(!circle.FitLSQ(pnts,residual))
    return __LINE__;
  //cerr << "Residual=" << residual << "\n";
  //cerr << "Center=" << circle.Centre() << " Radius=" << circle.Radius() << "\n";
  if(Point2dC(circle.Centre() - Point2dC(1,2)).SumOfSqr() > 0.01)
    return __LINE__;
  if(Abs(circle.Radius() - 2) > 0.01) 
    return __LINE__;

  Circle2dC circle2;
  if(!circle2.Fit(pnts[0],pnts[1],pnts[2]))
    return __LINE__;
  //cerr << "Center=" << circle2.Centre() << " Radius=" << circle2.Radius() << "\n";
  if(Point2dC(circle2.Centre() - Point2dC(1,2)).SumOfSqr() > 0.01)
    return __LINE__;
  if(Abs(circle2.Radius() - 2) > 0.01) 
    return __LINE__;
  
  return 0;
}

int testConvexHull2d() {
  cerr << "testConvexHull2d Called. \n";
  for(int j = 0;j < 10;j++) {
    DListC<Point2dC> pnts;
    
    // Generate a point set.
    for(int i = 0;i < (10 + j * 10);i++)
      pnts.InsLast(Point2dC(Random1() * 100,Random1() * 100));
    
    DListC<Point2dC> tmp = pnts.Copy();
    Polygon2dC poly = ConvexHull(tmp);
    //cout << "Hull=" << poly << "\n";
    
    for(DLIterC<Point2dC> it(pnts);it;it++) {
      if(!poly.Contains(*it)) {
	cerr << *it << " not in polygon. \n";
	return __LINE__;
      }
    }
  }
  
  return 0;
}

int testDelaunayTriangulation2d() {
  cerr << "testDelaunayTriangulation2d Called. \n";
  
  // Try a mesh with a exactly aligned points.
  
  SArray1dC<Point2dC> pnts(5);
  pnts[0] = Point2dC(10,10);
  pnts[1] = Point2dC(10,90);
  pnts[2] = Point2dC(10,50);
  pnts[3] = Point2dC(90,50);
  pnts[4] = Point2dC(50,50);
  
  HEMesh2dC mesh = DelaunayTriangulation(pnts);
  if(!mesh.CheckMesh(true)) return __LINE__;
  
  for(int j = 0;j < 10;j++) {
    SArray1dC<Point2dC> pnts(10 + j * 10);
    
    // Generate a point set.
    for(UIntT i = 0;i < pnts.Size();i++)
      pnts[i] = Point2dC(Random1() * 100,Random1() * 100);
    
    HEMesh2dC mesh = DelaunayTriangulation(pnts);
    
    if(!mesh.CheckMesh(true)) return __LINE__;
    
    if(!IsDelaunayTriangulation(mesh)) return __LINE__;
    
    // Convert it into a tri mesh.
    
    TriMesh2dC triMesh = mesh.TriMesh();
    
    // Check faces are all oriented correctly.
    
    if(!triMesh.IsFlat()) return __LINE__;
    
  }
  
  return 0;
}

int testCompoundAffine() {
  cerr << "testCompoundAffine Called. \n";
  Affine2dC a1(Vector2dC(2,2), RavlConstN::pi_2, Vector2dC(0,0));
  Affine2dC a2(Vector2dC(1,1), 0, Vector2dC(10,20));
  Point2dC p(0,0);
  RealT diff = (a2*a1*p - Point2dC(10,20)).SumOfSqr();
  if (diff > 0.001) {
    cout << a1*p << " " << a2*a1*p << " " << diff << endl;
    return __LINE__;
  }
  diff = (a1*a2*p - Point2dC(-40,20)).SumOfSqr();
  if (diff > 0.001) {
    cout << a1*p << " " << a1*a2*p << " " << diff << endl;
    return __LINE__;
  }
  Point2dC q(5,4);
  diff = (a2*(a1*q) - (a2*a1)*q).SumOfSqr();
  if (diff > 0.001) {
    cout << diff << endl;
    return __LINE__;
  }
  return 0;
}

int testFitAffine() {
  cerr << "testFitAffine Called. \n";

  // Try a simple case.
  
  SArray1dC<Point2dC> ipnt(3);
  ipnt[0] = Point2dC(1,1);
  ipnt[1] = Point2dC(2,1);
  ipnt[2] = Point2dC(1,3);
  
  SArray1dC<Point2dC> opnt(3);
  opnt[0] = Point2dC(2,2);
  opnt[1] = Point2dC(3,2);
  opnt[2] = Point2dC(2,3);
  
  Affine2dC aff = FitAffine(ipnt,opnt);
  int i;
  for(i=0;i < 3;i++)
    if(((aff * ipnt[i]) - opnt[i]).SumOfSqr() > 0.001)
      return __LINE__;
  
  
  // Check 3 point fit.
  
  Affine2dC aff3;
  if(!FitAffine(ipnt[0],opnt[0],
		ipnt[1],opnt[1],
		ipnt[2],opnt[2],
		aff3)) {
    return __LINE__;
  }
  
  for(i=0;i < 3;i++)
    if(((aff * ipnt[i]) - opnt[i]).SumOfSqr() > 0.001)
      return __LINE__;
  
  // Check polygon transform.
  
  Polygon2dC poly;
  
  for(i = 0;i < 3;i++)
    poly.InsLast(ipnt[i]);
  Polygon2dC tpoly = aff * poly;
  i = 0;
  for(DLIterC<Point2dC> it(tpoly);it;it++,i++)
    if((*it - opnt[i]).SumOfAbs() > 0.001) return __LINE__;
  
  // Try some random cases.
  
  for(int j = 0;j < 100;j++) {
    for(i = 0;i < 3;i++) {
      ipnt[i] = Point2dC(Random1() * 100,Random1() * 100);
      opnt[i] = Point2dC(Random1() * 100,Random1() * 100);
    }
    Affine2dC aff = FitAffine(ipnt,opnt);
    int i;
    for(i=0;i < 3;i++) {
      if(((aff * ipnt[i]) - opnt[i]).SumOfSqr() > 0.001) {
	cerr << "Fit failed " << j << " ipnt=" << ipnt[i] << " opnt=" << opnt[i] << "\n";
	return __LINE__;
      }
    }
  }

  
  return 0;
}

int testAffine() {
  
  Affine2dC aff(Matrix2dC(0.6,0.5,
                          0.2,0.4),Vector2dC(22,34));
  
  cerr << "Original=" << aff << "\n";
  Point2dC translation;
  Vector2dC scaling;
  RealT skew;
  RealT rotation;
  
  aff.Decompose(translation,scaling,skew,rotation);
  
  Affine2dC recom = Affine2dC::Compose(translation,scaling,skew,rotation);
  cerr << "Recomposed=" << recom << "\n";
  return 0;
}

int testFitSimilarity() {
  cerr << "testFitSimilarity Called. \n";
  
  CollectionC<Point2dC> points(16);
  points.Append(Point2dC(1,4));
  points.Append(Point2dC(3,2));
  points.Append(Point2dC(7,3));
  points.Append(Point2dC(9,7));
  points.Append(Point2dC(5,3));
  
  // Generate a random rotation.
  
  RealT rotAngle(Random1() * RavlConstN::pi * 2.0 - RavlConstN::pi);
  
  Matrix2dC rot(Cos(rotAngle), Sin(rotAngle),
                -Sin(rotAngle), Cos(rotAngle));
  
  Vector2dC offset(Random1() * 10 - 5,
                   Random1() * 10 - 5);
  
  RealT scale = 0.75;
  
  SArray1dC<Point2dC> transformedPoints(points.Size());  
  for(SArray1dIter2C<Point2dC,Point2dC> it(transformedPoints,points.Array());it;it++) {
    it.Data1() = rot * it.Data2() * scale + offset;
  }
  
  
  //! Fit a rigid transform between the two point sets.

  Vector2dC fittedTranslation;
  Matrix2dC fittedRotation;
  RealT fittedScaling;
  
  if(!FitSimilarity(points.Array(),
                    transformedPoints,
                    fittedRotation,
                    fittedTranslation,
                    fittedScaling
                    )) return __LINE__;
    
  // Check known transform
  SArray1dC<Point2dC> a(2), b(2);
  a[0] = Point2dC(0,0);
  a[1] = Point2dC(1,-1);
  b[0] = Point2dC(4,6);
  b[1] = Point2dC(2,4);
  if(!FitSimilarity(a,
                    b,
                    fittedRotation,
                    fittedTranslation,
                    fittedScaling
                    )) return __LINE__;
  /*
  cerr << "  R= " << fittedRotation 
       << "   = " << ATan2(fittedRotation[0][1],fittedRotation[0][0])/RavlConstN::pi*180 << " deg"
       << ", t= " << fittedTranslation
       << ", s= " << fittedScaling
       << endl;
  */
  if (   (Abs(Matrix2dC(fittedRotation-Matrix2dC(0,1,-1,0)).Det()) > 1.0e-10)
      || (Abs(Vector2dC(fittedTranslation-Vector2dC(4,6)).Modulus()) > 1.0e-10)
      || (Abs(fittedScaling-2) > 1.0e-10)
         ) return __LINE__;

  return 0;
}


int testHEMesh2d() {
  HEMesh2dC mesh(true);
  SArray1dC<HEMeshBaseVertexC > points(3);
  
  points[0] = mesh.InsertVertex(Point2dC(0,0));
  points[1] = mesh.InsertVertex(Point2dC(0,1));
  points[2] = mesh.InsertVertex(Point2dC(1,0));
  
  
  HashC<Tuple2C<HEMeshBaseVertexC,HEMeshBaseVertexC> , HEMeshBaseEdgeC> edgeTab;
  mesh.InsertFace(points,edgeTab);
  
  ;  
  THEMeshFaceC<Point2dC> face = mesh.FindFace(Point2dC(0.1,0.1));
  if(!face.IsValid()) return __LINE__;
  
  face = mesh.FindFace(Point2dC(-1,-1));
  if(face.IsValid()) return __LINE__;
  
  face = mesh.FindFace(Point2dC(-1,0.5));
  if(face.IsValid()) return __LINE__;
  
  face = mesh.FindFace(Point2dC(0.5,-1));
  if(face.IsValid()) return __LINE__;
  
  face = mesh.FindFace(Point2dC(1,1));
  if(face.IsValid()) return __LINE__;
  
  if(!IsDelaunayTriangulation(mesh)) return __LINE__;
  
  return 0;
}

int testProjective2d() {
  cerr << "testProjective2d Called. \n";
  IntT i;
  Projection2dC proj(Projection2dC::I(2,10)); // Create a unit projection with arbitrary iz & oz.
  
  // Check homography 
  if ((proj.Homography() - Matrix3dC::I()).SumOfAbs() > 0.001)  return __LINE__;
  
  SArray1dC<Point2dC> ipnt(3);
  ipnt[0] = Point2dC(1,1);
  ipnt[1] = Point2dC(2,1);
  ipnt[2] = Point2dC(1,3);
  
  SArray1dC<Point2dC> opnt = ipnt;
  
  // Check polygon transform.
  Polygon2dC poly;
  for(i = 0;i < 3;i++)
    poly.InsLast(ipnt[i]);
  Polygon2dC tpoly = proj * poly;
  i = 0;
  for(DLIterC<Point2dC> it(tpoly);it;it++,i++)
    if((*it - opnt[i]).SumOfAbs() > 0.001) return __LINE__;

  // Check projection multiplication
  Projection2dC proj2(Projection2dC::I(14,3)); // Another arbitrary unit projection
  if (((proj*proj2)*ipnt[1] - ipnt[1]).SumOfAbs() > 0.001) return __LINE__;

  // Check affine constructor, using arbitrary projective scale values
  Projection2dC proj3(Affine2dC::Compose(Point2dC(3,1),Vector2dC(2,2),0,0),2,7);
  Point2dC result = proj3 * Point2dC(1,0);
  if ((Abs(result[0]-5) > 0.001) || (Abs(result[1]-1) > 0.001)) return __LINE__;
  return 0;
}

int testLineProjective2d() {
  cerr << "testLineProjective2d Called. \n";
  // Draw a line through 2 points
  Point2dC p1(2,1);
  Point2dC p2(1,3);
  LineABC2dC inLine(p1, p2);
  // See if projected line still passes through projected points
  Projection2dC proj(Matrix3dC(0,.5,0,.5,0,0,0,0,0.4), 2, 5);
  LineABC2dC outLine(proj*inLine);
  if (   (outLine.Distance(proj*p1) > 0.001)
      || (outLine.Distance(proj*p2) > 0.001)) return __LINE__;  
  LinePP2dC inPPLine(p1, p2);
  // See if projected line still passes through projected points
  LinePP2dC outPPLine(proj*inPPLine);
  if (   (outPPLine.Distance(proj*p1) > 0.001)
      || (outPPLine.Distance(proj*p2) > 0.001)) return __LINE__;  
  return 0;
}

int testLines2d() {
  cerr << "testLinePP2d Called. \n";
  Point2dC p1(0,0);
  Point2dC p2(2,0);
  Point2dC p3(6,3);
  {
    LineABC2dC l[4];
    l[0] = LineABC2dC(p2,p1);  // Note incorrect arg ordering
    l[1] = LineABC2dC(Vector2dC(0,1),p2);
    l[2] = LineABC2dC(p1, Vector2dC(1,0));
    l[3] = LineABC2dC(0,1,0);
    for (int i=0; i<4; ++i) {
      cout << "Line " << i << endl;
      if (Vector2dC(l[i].UnitNormal()-Vector2dC(0,1)).Modulus() > 0.001) return __LINE__;  
      if (Abs(l[i].SignedDistance(p3)-3) > 0.001) return __LINE__; 
      if (Vector2dC(l[i].Projection(p3)-Point2dC(6,0)).Modulus() > 0.001) return __LINE__;  
    }
  }
  {
    LinePP2dC l[2];
    l[0] =  LinePP2dC(p1,p2);
    l[1] =  LinePP2dC(p1, Vector2dC(1,0));
    for (int i=0; i<2; ++i) {
      cout << "Line " << i << endl;
      if (Vector2dC(l[i].UnitNormal()-Vector2dC(0,1)).Modulus() > 0.001) return __LINE__;  
      if (Abs(l[i].SignedDistance(p3)-3) > 0.001) return __LINE__; 
      if (Vector2dC(l[i].Projection(p3)-Point2dC(6,0)).Modulus() > 0.001) return __LINE__;  
    }
  }
  return 0;
}


int testConic2d() {
  cerr << "testConic2d() Called. \n";
  SArray1dC<Point2dC> pnts(5);
  pnts[0] = Point2dC(1, 0);
  pnts[1] = Point2dC(2,-1);
  pnts[2] = Point2dC(3, 0);
  pnts[3] = Point2dC(3, 1);
  pnts[4] = Point2dC(2, 4);
  Conic2dC conic = FitConic(pnts);
  for(UIntT i = 0;i < pnts.Size();i++) {
    //cerr << i << " Residue=" << conic.Residue(pnts[i]) << "\n";
    if(!conic.IsOnCurve(pnts[i]))
      return __LINE__;
  }
  return 0;
}

int testEllipse2dA() {
  cerr << "testEllipse2dA Called. \n";
  SArray1dC<Point2dC> pnts(5);
  pnts[0] = Point2dC(1, 0);
  pnts[1] = Point2dC(2,-1);
  pnts[2] = Point2dC(3, 0);
  pnts[3] = Point2dC(3, 1);
  pnts[4] = Point2dC(2, 4);
  
  Conic2dC conic;
  if(!FitEllipse(pnts,conic)) {
    cerr << "Failed to fit ellipse.\n";
    return __LINE__;
  }
  
  Ellipse2dC ellipse;
  if(!conic.AsEllipse(ellipse)) {
    cerr << "Not an ellipse!\n";
    return __LINE__;
  }
  
  for(UIntT i = 0;i <pnts.Size();i++) {
    //cerr << i << " Residue=" << conic.Residue(pnts[i]) << "\n";
    if(!conic.IsOnCurve(pnts[i])) {
      cerr << "Conic fit failed on point " << i << " \n";
      return __LINE__;
    }
    if(!ellipse.IsOnCurve(pnts[i])) {
      cerr << "Ellipse fit failed on point " << i << " \n";
      return __LINE__;
    }
  }  
  return 0;
}

int testEllipse2dB() {
  cerr << "testEllipse2dB Called. \n";
  // Generates series of ellipses with orientations from 0 to pi
  for(RealT tangle = 0;tangle < RavlConstN::pi;tangle += RavlConstN::pi/10) {
    Point2dC gtc(50,50);
    // Generate ellispe
    Ellipse2dC ellipse(gtc,40,20,tangle);
    // Generate set of points on ellipse
    RealT step = RavlConstN::pi/5;
    SArray1dC<Point2dC> points(10);
    IntT i = 0;
    for(RealT a = 0;i < 10;a += step,i++) {
      points[i] = ellipse.Point(a);
    }
    // Fit set of points to ellipse as conic
    Conic2dC conic;
    if(!FitEllipse(points,conic)) 
      return __LINE__;
    //cerr << "Conic=" << conic.C() << "\n";
    Point2dC centre;
    RealT min,maj,ang;
    conic.EllipseParameters(centre,maj,min,ang);
    //cerr << "Conic representation parameters=" << centre << " " << maj << " " << min << " " << ang << "   Diff=" << AngleC(ang,RavlConstN::pi).Diff(AngleC(tangle,RavlConstN::pi)) << "\n";
    if((centre - gtc).SumOfAbs() > 0.00000001) return __LINE__;
    if(Abs(maj - 40) > 0.000000001) return __LINE__;
    if(Abs(min - 20) > 0.000000001) return __LINE__;
    if(Abs(AngleC(ang,RavlConstN::pi).Diff(AngleC(tangle,RavlConstN::pi))) > 0.000001) return __LINE__;
    // Fit same set of points to ellipse as Ellipse2dC
    Ellipse2dC ellipse2;
    if(!FitEllipse(points,ellipse2)) {
      cerr << "Failed to fit ellipse. \n";
      return __LINE__;
    }
    // Check that fitted ellipse has same params as original
    ellipse2.EllipseParameters(centre,maj,min,ang);
    //cerr << "Ellipse representation parameters=" << centre << " " << maj << " " << min << " " << ang << "  Diff=" << AngleC(ang,RavlConstN::pi).Diff(AngleC(tangle,RavlConstN::pi)) << "\n";
    if((centre - gtc).SumOfAbs() > 0.00000001) return __LINE__;
    if(Abs(maj - 40) > 0.000000001) return __LINE__;
    if(Abs(min - 20) > 0.000000001) return __LINE__;
    //cerr << "param angle vs orig: " << ang << " " << tangle << endl;
    if(Abs(AngleC(ang,RavlConstN::pi).Diff(AngleC(tangle,RavlConstN::pi))) > 0.000001) return __LINE__;
  }
  //cerr << "Ellipse2=" << ellipse2 << "\n";
  return 0;
}

int testEllipse2dC() {
  cerr << "testEllipse2dC Called. \n";
  Matrix2dC covar(4,0,
		  0,1);
  Vector2dC mean(50,50);
  
  Ellipse2dC ellipse = EllipseMeanCovariance(covar,mean,1.0);
  Point2dC centre;
  RealT min,maj,ang;
  ellipse.EllipseParameters(centre,maj,min,ang);
  //cerr << "Parameters=" << centre << " " << maj << " " << min << " " << ang << " \n";
  if(Abs(maj - 2) > 0.0000001)
    return __LINE__;
  if(Abs(min - 1) > 0.0000001)
    return __LINE__;
  if(Abs(AngleC(ang,RavlConstN::pi).Diff(AngleC(0,RavlConstN::pi))) > 0.000001) return __LINE__;
  return 0;
}

int testEllipse2dD() { 
  // Create an ellipse that passes through 5 points and check that it is the same (more or less) as the corresponding ellipse constructed from ellipse parameters. 
  cerr << "testEllipse2dD Called. \n";

  // Check that EllipseParameters works ok.
  RealT major=Sqrt(8), minor=Sqrt(0.5), angle=RavlConstN::pi_4;
  Ellipse2dC e(Point2dC(0,0), major, minor, angle);
  Point2dC e_centre;
  RealT e_major, e_minor, e_angle;
  e.EllipseParameters(e_centre, e_major, e_minor, e_angle);
  //cout << "e : " << e_centre <<"; "<<e_major <<"; "<< e_minor <<"; "<< e_angle << endl;
  //cout << "e as stream:\n" << e << endl;
  if((e_centre).SumOfAbs() > 0.00000001) return __LINE__;
  if(Abs(e_major - major) > 0.000000001) return __LINE__;
  if(Abs(e_minor - minor) > 0.000000001) return __LINE__;
  if(Abs(e_angle - angle) > 0.000001) return __LINE__;

  // Create diamond of points + another point along (1,1) direction: should fit to ellipse centred on 0,0; major axis 10 root 2; minor axis a tad more than 2 root 2; angle pi/4
  SArray1dC<Point2dC> pt(5);
  pt[0] = Point2dC(1,0);
  pt[1] = Point2dC(0,1);
  pt[2] = Point2dC(-1,0);
  pt[3] = Point2dC(0,-1);
  pt[4] = Point2dC(2,2);
  //  fit ellipse
  Ellipse2dC f;
  bool ans = FitEllipse(pt, f);	
  if (!ans) return  __LINE__;
  Point2dC f_centre;
  RealT f_major, f_minor, f_angle;
  f.EllipseParameters(f_centre, f_major, f_minor, f_angle);
  //cerr << "f: " << f_centre <<"; "<<f_major <<"; "<< f_minor <<"; "<< f_angle << endl;
  //cerr << "f as stream:\n" << f << endl;
  if((e_centre - f_centre).SumOfAbs() > 0.00000001) return __LINE__;
  if(Abs(e_major - f_major) > 0.000000001) return __LINE__;
  // The following test is deliberately loose, as ellipse "e" is only an
  // approximation to ellipse "f"
  if(Abs(e_minor - f_minor) > 0.03) return __LINE__;
  //cerr << "param angle vs orig: " << f_angle << " " << e_angle << endl;
  if(Abs(e_angle - f_angle) > 0.000001) return __LINE__;
  
  return 0;
}

#define DODISPLAY 0

#if DODISPLAY
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/DrawLine.hh"
#include "Ravl/Image/DrawPolygon.hh"
#include "Ravl/IO.hh"
using namespace RavlImageN;
#endif


int testScanPolygon() {
  cerr << "testScanPolygon Called. \n";
  UIntT count = 0;
#if DODISPLAY
  ByteT drawVal = 255;
  ImageC<ByteT> img(105,105);
  img.Fill(0);
#endif
  Polygon2dC poly;
#if 1
  poly.InsLast(Point2dC(5,10));
  poly.InsLast(Point2dC(40,50));
  poly.InsLast(Point2dC(100,20));
  
  for(ScanPolygon2dC it(poly);it;it++) {
#if DODISPLAY
    DrawLine(img,drawVal,Index2dC(it.Row(),it.Data().Min()),Index2dC(it.Row(),it.Data().Max()));
#endif
    //cerr << " " << it.Row() << " " << it.Data() << "\n";
    if(it.Data().Size() > 0.001 && 
       !poly.Contains(Point2dC(it.Row(),it.Data().Center()))) 
      return __LINE__;
    count++;
  }
#if DODISPLAY
  Save("@X:1",img);
  cerr << " ---------- Test 2 --------------------------- \n";
#endif
  //cerr << "Entries=" << count <<"\n";
  
  count = 0;
#if DODISPLAY
  img = ImageC<ByteT>(105,105);
  img.Fill(0);
#endif
  poly.Empty();
  poly.InsLast(Point2dC(10,10));
  poly.InsLast(Point2dC(30,20));
  poly.InsLast(Point2dC(20,40));
  poly.InsLast(Point2dC(40,20));
  for(ScanPolygon2dC it(poly,1);it;it++) {
#if DODISPLAY
    DrawLine(img,drawVal,Index2dC(it.Row(),it.Data().Min()),Index2dC(it.Row(),it.Data().Max()));
#endif
    //cerr << " " << it.Row() << " " << it.Data() << "\n";
    if(it.Data().Size() > 0.001 &&
       !poly.Contains(Point2dC(it.Row(),it.Data().Center()))) 
      return __LINE__;
    count++;
  }
#if DODISPLAY
  Save("@X:2",img);
  cerr << " ---------- Test 3 --------------------------- \n";
  img = ImageC<ByteT>(105,105);
  img.Fill(0);
#endif
  poly.Empty();
  poly.InsLast(Point2dC(10,30));
  poly.InsLast(Point2dC(40,40));
  poly.InsLast(Point2dC(20,30));
  poly.InsLast(Point2dC(30,10));
  for(ScanPolygon2dC it(poly,1);it;it++) {
#if DODISPLAY
    DrawLine(img,drawVal,Index2dC(it.Row(),it.Data().Min()),Index2dC(it.Row(),it.Data().Max()));
#endif
    //cerr << " " << it.Row() << " " << it.Data() << "\n";
    if(it.Data().Size() > 0.001 &&
       !poly.Contains(Point2dC(it.Row(),it.Data().Center()))) 
      return __LINE__;
    count++;
  }
#if DODISPLAY
  Save("@X:3",img);
  cerr << " ---------- Test 4 --------------------------- \n";
  img = ImageC<ByteT>(105,105);
  img.Fill(0);
#endif
  poly.Empty();
  poly.InsLast(Point2dC(10,10));
  poly.InsLast(Point2dC(10,90));
  poly.InsLast(Point2dC(90,90));
  poly.InsLast(Point2dC(90,10));
  for(ScanPolygon2dC it(poly,1);it;it++) {
#if DODISPLAY
    DrawLine(img,drawVal,Index2dC(it.Row(),it.Data().Min()),Index2dC(it.Row(),it.Data().Max()));
#endif
    //cerr << " " << it.Row() << " " << it.Data() << "\n";
    if(it.Data().Size() > 0.001 &&
       !poly.Contains(Point2dC(it.Row(),it.Data().Center()))) 
      return __LINE__;
    count++;
  }
#if DODISPLAY
  Save("@X:4",img);
#endif
  
#if DODISPLAY
  cerr << " ---------- Test 5 --------------------------- \n";
  img = ImageC<ByteT>(105,105);
  img.Fill(0);
#endif
  poly.Empty();
  poly.InsLast(Point2dC(10,10));
  poly.InsLast(Point2dC(10,20));
  poly.InsLast(Point2dC(20,20));
  poly.InsLast(Point2dC(20,30)); 
  poly.InsLast(Point2dC(30,30));
  poly.InsLast(Point2dC(30,10));
  for(ScanPolygon2dC it(poly,1);it;it++) {
#if DODISPLAY
    DrawLine(img,drawVal,Index2dC(it.Row(),it.Data().Min()),Index2dC(it.Row(),it.Data().Max()));
#endif
    //cerr << " " << it.Row() << " " << it.Data() << "\n";
#if 0
    if(it.Data().Size() > 0.001 &&
       !poly.Contains(Point2dC(it.Row(),it.Data().Center()))) 
      return __LINE__;
#endif
    count++;
  }
#if DODISPLAY
  Save("@X:5",img);
#endif

#if DODISPLAY
  cerr << " ---------- Test 6 --------------------------- \n";
  img = ImageC<ByteT>(105,105);
  img.Fill(0);
#endif
  poly.Empty();
  poly.InsLast(Point2dC(10,10));
  poly.InsLast(Point2dC(10,20));
  poly.InsLast(Point2dC(20,20));
  poly.InsLast(Point2dC(20,30)); 
  poly.InsLast(Point2dC(10,30));
  poly.InsLast(Point2dC(10,40));
  poly.InsLast(Point2dC(40,40));
  poly.InsLast(Point2dC(40,30));
  poly.InsLast(Point2dC(30,30));
  poly.InsLast(Point2dC(30,20));
  poly.InsLast(Point2dC(40,20));
  poly.InsLast(Point2dC(40,10));
  for(ScanPolygon2dC it(poly,1);it;it++) {
#if DODISPLAY
    DrawLine(img,drawVal,Index2dC(it.Row(),it.Data().Min()),Index2dC(it.Row(),it.Data().Max()));
#endif
    //cerr << " " << it.Row() << " " << it.Data() << "\n";
    if(it.Data().Size() > 0.001 &&
       !poly.Contains(Point2dC(it.Row(),it.Data().Center()))) 
      return __LINE__;
    count++;
  }
#if DODISPLAY
  Save("@X:6",img);
#endif

#if DODISPLAY
  cerr << " ---------- Test 7 --------------------------- \n";
  img = ImageC<ByteT>(105,105);
  img.Fill(0);
#endif
  poly.Empty();
  poly.InsLast(Point2dC(10,20));
  poly.InsLast(Point2dC(10,30));
  poly.InsLast(Point2dC(20,30));
  poly.InsLast(Point2dC(20,40)); 
  poly.InsLast(Point2dC(30,40));  
  poly.InsLast(Point2dC(30,30));
  poly.InsLast(Point2dC(40,30));
  poly.InsLast(Point2dC(40,20));
  poly.InsLast(Point2dC(30,20));
  poly.InsLast(Point2dC(30,10));
  poly.InsLast(Point2dC(20,10));
  poly.InsLast(Point2dC(20,20));
  for(ScanPolygon2dC it(poly,1);it;it++) {
#if DODISPLAY
    DrawLine(img,drawVal,Index2dC(it.Row(),it.Data().Min()),Index2dC(it.Row(),it.Data().Max()));
#endif
    //cerr << " " << it.Row() << " " << it.Data() << "\n";
    if(it.Data().Size() > 0.001 &&
       !poly.Contains(Point2dC(it.Row(),it.Data().Center())))  {
      //Save("@X:7",img);
      return __LINE__;
    }
    count++;
  }
#if DODISPLAY
  Save("@X:7",img);
#endif
  
#if DODISPLAY
  cerr << " ---------- Test 8 --------------------------- \n";
  img = ImageC<ByteT>(105,105);
  img.Fill(0);
#endif
  poly.Empty();
  poly.InsLast(Point2dC(10,10));
  poly.InsLast(Point2dC(10,40));
  poly.InsLast(Point2dC(20,40));
  poly.InsLast(Point2dC(20,30)); 
  //poly.InsLast(Point2dC(30,30));
  poly.InsLast(Point2dC(30,30));
  poly.InsLast(Point2dC(30,40));
  poly.InsLast(Point2dC(40,40));
  poly.InsLast(Point2dC(40,10));
  poly.InsLast(Point2dC(30,10));
  poly.InsLast(Point2dC(30,20));
  poly.InsLast(Point2dC(20,20));
  poly.InsLast(Point2dC(20,10));
  for(ScanPolygon2dC it(poly,1);it;it++) {
#if DODISPLAY
    DrawLine(img,drawVal,Index2dC(it.Row(),it.Data().Min()),Index2dC(it.Row(),it.Data().Max()));
#endif
    if(it.Data().Size() > 0.001 &&
       !poly.Contains(Point2dC(it.Row(),it.Data().Center()))) {
      std::cerr << " Row:" << it.Row() << " Span:" << it.Data() << "\n";
      //  Save("@X:8",img);
      return __LINE__;
    }
    count++;
  }
#if DODISPLAY
  Save("@X:8",img);
#endif

#if DODISPLAY
  cerr << " ---------- Test 9 --------------------------- \n";
  img = ImageC<ByteT>(105,105);
  img.Fill(0);
#endif
  poly.Empty();
  poly.InsLast(Point2dC(10,10));
  poly.InsLast(Point2dC(30,20));
  poly.InsLast(Point2dC(10,30));
  poly.InsLast(Point2dC(30,40)); 
  poly.InsLast(Point2dC(10,50));  
  poly.InsLast(Point2dC(40,40));
  poly.InsLast(Point2dC(20,30));
  poly.InsLast(Point2dC(40,20));
  for(ScanPolygon2dC it(poly,1);it;it++) {
#if DODISPLAY
    DrawLine(img,drawVal,Index2dC(it.Row(),it.Data().Min()),Index2dC(it.Row(),it.Data().Max()));
#endif
    //cerr << " " << it.Row() << " " << it.Data() << "\n";
    if(it.Data().Size() > 0.001 &&
       !poly.Contains(Point2dC(it.Row(),it.Data().Center()))) 
      return __LINE__;
    count++;
  }
#if DODISPLAY
  Save("@X:9",img);
#endif

#if DODISPLAY
  cerr << " ---------- Test 10 --------------------------- \n";
  img = ImageC<ByteT>(105,105);
  img.Fill(0);
#endif
  poly.Empty();

  poly.InsLast(Point2dC(20,40));
  poly.InsLast(Point2dC(30,20));
  poly.InsLast(Point2dC(40,40));
  poly.InsLast(Point2dC(50,10));
  poly.InsLast(Point2dC(40,30));
  poly.InsLast(Point2dC(30,10));
  poly.InsLast(Point2dC(20,30));
  poly.InsLast(Point2dC(10,10));
  
  for(ScanPolygon2dC it(poly,1);it;it++) {
#if DODISPLAY
    DrawLine(img,drawVal,Index2dC(it.Row(),it.Data().Min()),Index2dC(it.Row(),it.Data().Max()));
#endif
    //cerr << " " << it.Row() << " " << it.Data() << "\n";
    if(it.Data().Size() > 0.001 &&
       !poly.Contains(Point2dC(it.Row(),it.Data().Center()))) 
      return __LINE__;
    count++;
  }
#if DODISPLAY
  Save("@X:10",img);
#endif
#if DODISPLAY
  cerr << " ---------- Test 11 --------------------------- \n";
  img = ImageC<ByteT>(105,105);
  img.Fill(0);
#endif
  poly.Empty();
  poly.InsLast(Point2dC(10,10));
  poly.InsLast(Point2dC(10,60));
  poly.InsLast(Point2dC(50,60));
  poly.InsLast(Point2dC(50,40)); 
  poly.InsLast(Point2dC(40,40));  
  poly.InsLast(Point2dC(40,50));
  poly.InsLast(Point2dC(30,50));
  poly.InsLast(Point2dC(30,40));
  poly.InsLast(Point2dC(20,40));
  poly.InsLast(Point2dC(20,30));
  poly.InsLast(Point2dC(30,30));
  poly.InsLast(Point2dC(30,20));
  poly.InsLast(Point2dC(40,20));
  poly.InsLast(Point2dC(40,30));
  poly.InsLast(Point2dC(50,30));
  poly.InsLast(Point2dC(50,10));
  
  for(ScanPolygon2dC it(poly,1);it;it++) {
#if DODISPLAY
    DrawLine(img,drawVal,Index2dC(it.Row(),it.Data().Min()),Index2dC(it.Row(),it.Data().Max()));
#endif
    //cerr << " " << it.Row() << " " << it.Data() << "\n";
    if(it.Data().Size() > 0.001 &&
       !poly.Contains(Point2dC(it.Row(),it.Data().Center()))) 
      return __LINE__;
    count++;
  }
#if DODISPLAY
  Save("@X:11",img);
#endif
#endif
#if DODISPLAY
  cerr << " ---------- Test 12 --------------------------- \n";
  img = ImageC<ByteT>(105,105);
  img.Fill(0);
#endif
  poly.Empty();
  int stop = 1000;


  poly.InsLast(Point2dC(10,50));
  poly.InsLast(Point2dC(30,50));
  poly.InsLast(Point2dC(30,40));
  poly.InsLast(Point2dC(20,40));
  poly.InsLast(Point2dC(20,30));
  poly.InsLast(Point2dC(30,30));
  poly.InsLast(Point2dC(30,20));
  poly.InsLast(Point2dC(40,20));
  poly.InsLast(Point2dC(40,30));
  poly.InsLast(Point2dC(50,30));
  poly.InsLast(Point2dC(50,40));
  poly.InsLast(Point2dC(40,40));
  poly.InsLast(Point2dC(40,50));
  poly.InsLast(Point2dC(60,50));
  poly.InsLast(Point2dC(60,10));
  poly.InsLast(Point2dC(10,10));

  

  for(ScanPolygon2dC it(poly,1);it && (stop-- > 0);it++) {
#if DODISPLAY
    DrawLine(img,drawVal,Index2dC(it.Row(),it.Data().Min()),Index2dC(it.Row(),it.Data().Max()));
#endif
    //cerr << " " << it.Row() << " " << it.Data() << "\n";
#if 1
    if(it.Data().Size() > 0.001 &&
       !poly.Contains(Point2dC(it.Row(),it.Data().Center()))) {
#if DODISPLAY
    Save("@X:12",img);
#endif
      return __LINE__;
    }
#endif
    count++;
  }
#if DODISPLAY
  Save("@X:12",img);
#endif
  if(stop <= 0) return __LINE__;
  return 0;
}

int testOverlap() {
  cerr << "testOverlap, Called. \n";
  Polygon2dC poly;
  poly.InsLast(Point2dC(0,0));
  poly.InsLast(Point2dC(0,10));
  poly.InsLast(Point2dC(10,10));
  poly.InsLast(Point2dC(10,0));
  
  RealT score = poly.Overlap(poly);
  if(Abs(score - 1.0) > 0.000001) return __LINE__;
  
  poly.Reverse();
  
  score = poly.Overlap(poly);
  if(Abs(score - 1.0) > 0.000001) return __LINE__;
  
  Polygon2dC poly2 = poly.Copy();
  poly2 += Point2dC(100,100);
  
  score = poly.Overlap(poly2);
  if(Abs(score) > 0.000001) return __LINE__;
  
  score = poly2.Overlap(poly);
  if(Abs(score) > 0.000001) return __LINE__;
  
  
  return 0;
}

int testPolygonClip() {
  cerr << "testPolygonClip, Called. \n";
  Polygon2dC poly;
  poly.InsLast(Point2dC(-10,0));
  poly.InsLast(Point2dC(0,20));
  poly.InsLast(Point2dC(10,10));
  poly.InsLast(Point2dC(10,0));

  RealRange2dC range1(10,10);
  RealRange2dC range2(10,15);
  Polygon2dC clippedConvex = poly.ClipByConvex(Polygon2dC(range1));
  Polygon2dC clippedRange = poly.ClipByRange(range1);

  RealT score = Abs(clippedConvex.Area());
  if(Abs(score - 100) > 1e-6) return __LINE__;

  score = Abs(clippedRange.Area());
  if (Abs(score - 100) > 1e-6) return __LINE__;

  score = clippedConvex.Overlap(clippedRange);
  if (Abs(score - 1) > 1e-6) return __LINE__;

  clippedConvex = poly.ClipByConvex(Polygon2dC(range2));
  clippedRange = poly.ClipByRange(range2);

  score = clippedConvex.Overlap(clippedRange);
  if (Abs(score - 1) > 1e-6) return __LINE__;

  return 0;
}

int testTriangulate2d() {
  CollectionC<Point2dC> poly(20);
  poly.Append( Vector2dC(0,6));  
  poly.Append( Vector2dC(0,0));
  poly.Append( Vector2dC(3,0));
  poly.Append( Vector2dC(4,1));
  poly.Append( Vector2dC(6,1));
  poly.Append( Vector2dC(8,0));
  poly.Append( Vector2dC(12,0));
  poly.Append( Vector2dC(13,2));
  poly.Append( Vector2dC(8,2));
  poly.Append( Vector2dC(8,4));
  poly.Append( Vector2dC(11,4));
  poly.Append( Vector2dC(11,6));
  poly.Append( Vector2dC(6,6));
  poly.Append( Vector2dC(4,3));
  poly.Append( Vector2dC(2,6));
  
  SArray1dC<Index3dC> triangles;
  if(!PolygonTriangulation(poly.Array(),triangles))
    return __LINE__;
  if(triangles.Size() != poly.Size()-2) return __LINE__;
  
#if DODISPLAY
  ByteT drawVal = 128;
  ImageC<ByteT> img(128,128);
  img.Fill(0);
  for(unsigned i = 0;i < triangles.Size();i++) {
    Index3dC tri = triangles[i];
    DrawLine(img,drawVal,Index2dC(poly[tri[0]]*10),Index2dC(poly[tri[1]]*10));
    DrawLine(img,drawVal,Index2dC(poly[tri[1]]*10),Index2dC(poly[tri[2]]*10));
    DrawLine(img,drawVal,Index2dC(poly[tri[2]]*10),Index2dC(poly[tri[0]]*10));
  }
  Polygon2dC dpoly = DListOf(poly.Array());
  dpoly *= 10;
  drawVal = 255;
  DrawPolygon(img,drawVal,dpoly);
  RavlN::Save("@X:Triangulate",img);
#endif
  std::cerr << "Points=" << poly.Size() << " Triangles=" << triangles.Size() << "\n";
  
  return 0;
}
