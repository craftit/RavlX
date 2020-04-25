// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! author="Joel Mitchelson"
//! docentry="Ravl.API.Math.Geometry.3D"
//! userlevel=Develop
//! file="Ravl/Math/Geometry/Euclidean/3D/testGeom3d.cc"

#include "Ravl/EulerAngle.hh"
#include "Ravl/AxisAngle.hh"
#include "Ravl/StdConst.hh"
#include "Ravl/LinePP3d.hh"
#include "Ravl/LinePV3d.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/Affine3d.hh"
#include "Ravl/Point2d.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/Collection.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/PlaneABCD3d.hh"
#include "Ravl/PlanePVV3d.hh"
#include "Ravl/GPSCoordinate.hh"

using namespace RavlN;

int testAngles();
int testLine();
int testPlane();
int testPlaneFit();
int testLineDist();
int testFitAffine();
int testFitAffineDirections();
int testSimilarity();
int testGPS();

int main(int nargs,char **argv) {
  int ln;
#if 1
  if((ln = testLine()) != 0) {
    cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  if((ln = testLineDist()) != 0) {
    cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  if((ln = testFitAffine()) != 0) {
    cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  if((ln = testFitAffineDirections()) != 0) {
    cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  if((ln = testAngles()) != 0) {
    cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  if((ln = testSimilarity()) != 0) {
    cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  if((ln = testPlane()) != 0) {
    cerr << "Test failed on line " << ln << "\n";
    return 1;    
  }
#endif
  if((ln = testPlaneFit()) != 0) {
    cerr << "Test failed on line " << ln << "\n";
    return 1;    
  }
  if((ln = testGPS()) != 0) {
    cerr << "Test failed on line " << ln << "\n";
    return 1;    
  }

  cerr << "Test passed ok. \n";
  return 0;
}

int testLine() {
  cerr << "Testing lines. \n";
  Point3dC p0(1,1,1);
  Point3dC p1(3,5,7);
  Point3dC p2(2,3,4);
  Point3dC p3(5,9,13);
  
  LinePP3dC line(p0,p1);
  
  if(line.Distance(p2) > 0.0000001) return __LINE__;
  RealT val;
  val = line.ParClosest(p0);
  if(Abs(val) > 0.0000001) return __LINE__;
  val = line.ParClosest(p1);  
  if(Abs(val-1) > 0.0000001) return __LINE__;
  val = line.ParClosest(p2);
  if(Abs(val-0.5) > 0.0000001) return __LINE__;
  val = line.ParClosest(p3);
  if(Abs(val-2) > 0.0000001) return __LINE__;
  return 0;
}

int testLineDist() {
  
  LinePV3dC line1(Point3dC(0,0,0),Vector3dC(1,0,0));
  LinePV3dC line2(Point3dC(0,1,0),Vector3dC(0,0,1));
  if(Abs(line1.Distance(line2) - 1) > 0.0000001) return __LINE__;
  if((line1.Intersection(line2) - Point3dC(0,0.5,0)).SumOfSqr() > 0.000001) return __LINE__;
  
  LinePV3dC line3(Point3dC(0.5,0.5,0.5),Vector3dC(0.5,0.5,0.5));
  if(Abs(line1.Distance(line3)) > 0.000001) return __LINE__;
  
  
  return 0;
}

int testFitAffine() {
  cerr << "testFitAffine Called. \n";

  // Try 3d -> 3d case.
  
  SArray1dC<Point3dC> ipnt(4);
  ipnt[0] = Point3dC(1,1,3);
  ipnt[1] = Point3dC(2,1,4);
  ipnt[2] = Point3dC(1,3,5);
  ipnt[3] = Point3dC(2,4,6);
  
  SArray1dC<Point3dC> opnt(4);
  opnt[0] = Point3dC(2,2,6);
  opnt[1] = Point3dC(3,2,5);
  opnt[2] = Point3dC(2,4,4);
  opnt[3] = Point3dC(3,3,3);
  
  Affine3dC aff = FitAffine(ipnt,opnt);
  int i;
  for(i=0;i < 4;i++) {
    //std::cerr << "i=" << i << " " << ipnt[i] << " -> " << opnt[i] << " " << (aff * ipnt[i]) << "\n";
    if(((aff * ipnt[i]) - opnt[i]).SumOfSqr() > 0.001)
      return __LINE__;
  }
  
  // Test 2d -> 3d stuff.
  
  SArray1dC<Point2dC> ipnt2(3);
  ipnt2[0] = Point2dC(1,1);
  ipnt2[1] = Point2dC(2,1);
  ipnt2[2] = Point2dC(1,3);
  
  SArray1dC<Point3dC> opnt2(3);
  opnt2[0] = Point3dC(2,2,6);
  opnt2[1] = Point3dC(3,2,5);
  opnt2[2] = Point3dC(2,4,4);
  
  aff = FitAffine(ipnt2,opnt2);
  for(i=0;i < 3;i++) {
    //std::cerr << "i=" << i << " " << ipnt[i] << " -> " << opnt[i] << " " << (aff * ipnt[i]) << "\n";
    if(((aff * Point3dC(ipnt2[i][0],ipnt2[i][1],0)) - opnt2[i]).SumOfSqr() > 0.001)
      return __LINE__;
  }
  return 0;
}

int testFitAffineDirections() {
  
  cerr << "testFitAffineDirections Called. \n";
  
  // Setup some test points.
  
  SArray1dC<Point3dC> ipnt(6);
  ipnt[0] = Point3dC(1,1,3);
  ipnt[1] = Point3dC(2,1,4);
  ipnt[2] = Point3dC(1,3,5);
  ipnt[3] = Point3dC(2,4,6);
  ipnt[4] = Point3dC(3,2,5);
  ipnt[5] = Point3dC(4,2,3);
  
  SArray1dC<Point3dC> opnt(ipnt.Size());
  
  
  Point3dC center(2.2,3.3,4.4);
  
  SArray1dC<Vector3dC> odir(ipnt.Size());
  
  Affine3dC testTransform(Matrix3dC(2,0,0,
                                    0,3,0,
                                    0,0,2),Vector3dC(1,2,3));
  
  // Create direction vectors.
  
  for(UIntT i = 0;i < ipnt.Size();i++) {
    opnt[i] = testTransform * ipnt[i];
    odir[i] = opnt[i] - testTransform * center;
  }
  
  // Scale is ambiguous.
  Affine3dC aff = FitAffineDirection(ipnt,odir);
  // ipnt -> odir
  
  Point3dC fittedCenter = aff.Inverse() * Vector3dC(0,0,0);
  //std::cerr << "Fitted center=" << fittedCenter << "\n";
  if((fittedCenter - center).SumOfSqr() > 0.000001) return __LINE__;
  
  
  UIntT i;
  for(i=0;i < ipnt.Size();i++) {
    //std::cerr << "i=" << i << "  " << ipnt[i] << " -> " << opnt[i] << " Dir=" << odir[i].Unit()  << " " << Vector3dC(aff * ipnt[i]).Unit() << " @ " << aff * ipnt[i] << "\n";
    
    if(((aff * ipnt[i]).Unit() - odir[i].Unit()).SumOfSqr() > 0.001)
      return __LINE__;
  }
  
  
  return 0;
}


//! date="7/12/2002"
//! author="Joel Mitchelson"

int testAngles() {
  
  cerr << "Testing Euler angles...";
  
  for (int sequence = 0; sequence < EulerSequenceC::NumSequences; sequence++)
  {
    for (int a = -32; a <= 32; a++)
      for (int b = -32; b <= 32; b++)
	for (int c = -32; c <= 32; c++)
    {
      // make euler angle
      EulerAngleC e1(a*RavlConstN::pi/8.0, b*RavlConstN::pi/8.0, c*RavlConstN::pi/8.0, sequence);
      
      // extract rotation matrix
      Matrix3dC R1;
      e1.Matrix(R1);
      
      // check determinant
      if (fabs(R1.Det() - 1.0) > 1E-12)
	{
	  cerr << "ERROR: euler sequence " << sequence << " caused non-orthonormal matrix" << endl;
	  cerr << "Angles: " << Vector3dC(e1) << endl;
	  cerr << "Matrix: " << endl << R1 << endl;
	  return __LINE__;
	}
      
      // make new angle
      EulerAngleC e2(R1, sequence);
      
      // convert to new matrix
      Matrix3dC R2;
      e2.Matrix(R2);
      
      // check they coincide
      if ((R2-R1).SumOfAbs() > 1E-6)
      {
	cerr << "ERROR: euler sequence " << sequence << " conversion problem." << endl;
	cerr << "Angles: " << Vector3dC(e1) << endl;
	cerr << "Matrix: " << endl << R1 << endl;
	cerr << "Convertex to angles: " << Vector3dC(e2) << endl;
	cerr << "Converted to matrix: " << endl << R2 << endl;
	return __LINE__;
      }
    }
  }

  cerr << "Euler angles OK" << endl;


  cerr << "Testing axis-angles...";
  for (int a = -15; a <= 15; a++)
    for (int b = -15; b <= 15; b++)
      for (int c = -15; c <= 15; c++)
      {
	// make euler angle
	AxisAngleC e1(a*RavlConstN::pi/8.0, b*RavlConstN::pi/8.0, c*RavlConstN::pi/8.0);
      
	// extract rotation matrix
	Matrix3dC R1;
	e1.Matrix(R1);
      
	// check determinant
	if (fabs(R1.Det() - 1.0) > 1E-12)
	{
	  cerr << "ERROR: axis-angle sequence caused non-orthonormal matrix" << endl;
	  cerr << "Angles: " << Vector3dC(e1) << endl;
	  cerr << "Matrix: " << endl << R1 << endl;
	  return __LINE__;
	}
      
	// make new angle
	AxisAngleC e2(R1);
      
	// convert to new matrix
	Matrix3dC R2;
	e2.Matrix(R2);
      
	// check they coincide
	if ((R2-R1).SumOfAbs() > 1E-10)
	{
	  cerr << "ERROR: axis angle conversion problem." << endl;
	  cerr << "Angles: " << Vector3dC(e1) << endl;
	  cerr << "Matrix: " << endl << R1 << endl;
	  cerr << "Convertex to angles: " << Vector3dC(e2) << endl;
	  cerr << "Converted to matrix: " << endl << R2 << endl;
	  return __LINE__;
	}
      }

  cerr << "axis angles OK" << endl;
  
  return 0;

}


int testSimilarity() {
  
  CollectionC<Point3dC> points(16);
  points.Append(Point3dC(1,4,6));
  points.Append(Point3dC(3,2,9));
  points.Append(Point3dC(7,3,3));
  points.Append(Point3dC(9,7,2));
  points.Append(Point3dC(5,3,2));
  
  // Generate a random rotation.
  
  Vector3dC rotAngle(Random1() * RavlConstN::pi * 2.0 - RavlConstN::pi,
                     Random1() * RavlConstN::pi * 2.0 - RavlConstN::pi,
                     Random1() * RavlConstN::pi * 2.0 - RavlConstN::pi);
  
  Matrix3dC rot;
  EulerXYZToMatrix(rotAngle,rot);
  
  Vector3dC offset(Random1() * 10 - 5,
                   Random1() * 10 - 5,
                   Random1() * 10 - 5);
  
  RealT scale = 0.75;
  
  SArray1dC<Point3dC> transformedPoints(points.Size());  
  for(SArray1dIter2C<Point3dC,Point3dC> it(transformedPoints,points.Array());it;it++) {
    it.Data1() = rot * it.Data2() * scale + offset;
  }
  
  
  //! Fit a rigid transform between the two point sets.

  Vector3dC fittedTranslation;
  Matrix3dC fittedRotation;
  RealT fittedScaling;
  
  if(!FitSimilarity(points.Array(),
                    transformedPoints,
                    fittedRotation,
                    fittedTranslation,
                    fittedScaling
                    )) return __LINE__;
  

#if 0
  std::cerr << "Translation=" << offset << "\n";
  std::cerr << "Rotation=" << rot << "\n";
  std::cerr << "Scaling=" << scale << "\n";

  std::cerr << "Fitted Translation=" << fittedTranslation << "\n";
  std::cerr << "Fitted Rotation=" << fittedRotation << "\n";
  std::cerr << "Fitted Scaling=" << fittedScaling << "\n";
#endif  
  
  if(Sqr(fittedScaling - scale) > 0.0001) return __LINE__;
  if((fittedRotation - rot).SumOfSqr() > 0.0001) return __LINE__;
  if((fittedTranslation - offset).SumOfSqr() > 0.0001) return __LINE__;
  
  // Check the affine version.
  
  Affine3dC aff;
  if(!FitSimilarity(points.Array(),
                    transformedPoints,
                    aff
                    )) return __LINE__;
  
  for(SArray1dIter2C<Point3dC,Point3dC> it(transformedPoints,points.Array());it;it++) {
    if((it.Data1() - aff * it.Data2()).SumOfSqr() > 0.0001) return __LINE__;
  }
  

  return 0;
}

RealT RandomValue(RealT scale) 
{ return (Random1() - 0.5) * scale * 2.0; }

int testPlane() {
  cerr << "Testing planes.";
  
  for(int i =0 ;i < 100;i++) {
    
    PlaneABCD3dC plane(Vector3dC(RandomValue(10),RandomValue(10),RandomValue(10)), 
                       Point3dC(RandomValue(10),RandomValue(10),RandomValue(10))
                       );
    
    Point3dC testPoint(RandomValue(10),RandomValue(10),RandomValue(10));
    
    Point3dC closestPoint = plane.ClosestPoint(testPoint);
    RealT distance = closestPoint.EuclideanDistance(testPoint);
    if(Abs(distance - plane.EuclideanDistance(testPoint)) > 0.000000001)
      return __LINE__;
  }
  
  cerr << ".";
  for(int i =0 ;i < 100;i++) {
    
    PlanePVV3dC plane(Point3dC(RandomValue(10),RandomValue(10),RandomValue(10)),
                       Vector3dC(RandomValue(10),RandomValue(10),RandomValue(10)), 
                       Vector3dC(RandomValue(10),RandomValue(10),RandomValue(10))
                       );
    
    Point3dC testPoint(RandomValue(10),RandomValue(10),RandomValue(10));
    
    Point3dC closestPoint = plane.ClosestPoint(testPoint);
    RealT distance = closestPoint.EuclideanDistance(testPoint);
    if(Abs(distance - plane.EuclideanDistance(testPoint)) > 0.000000001)
      return __LINE__;
    
    // Check 'ProjectionOnto'
    Point2dC pCloesestPoint = plane.Projection(testPoint);
    if(closestPoint.EuclideanDistance(plane.Point(pCloesestPoint)) > 0.00000001) return __LINE__;
    
  }
  
  
  cerr << " OK \n";
  return 0;
}


int testPlaneFit() {
  
  SArray1dC<Point3dC> points(3);
  points[0] = Point3dC(1,1,1);
  points[1] = Point3dC(1,2,1);
  points[2] = Point3dC(2,1,2);
  
  {
    PlaneABCD3dC plane;
    if(!FitPlane(points,plane))
      return __LINE__;
    
    for(int i =0;i < 3;i++) {
      if(plane.EuclideanDistance(points[i]) > 0.001)
	return __LINE__;
    }
  }

  {
    PlanePVV3dC plane;
    if(!FitPlane(points,plane))
      return __LINE__;
    
    for(int i =0;i < 3;i++) {
      if(plane.EuclideanDistance(points[i]) > 0.001)
	return __LINE__;
    }
  }
  
  //std::cerr << "Plane= " << plane << "\n";
  
  return 0;
}

int testGPS()
{
  {
    StringC strTestCoord = "52 39' 27.2531\",1 43' 4.5177\",24.7m";
    GPSCoordinateC testCoord(strTestCoord);
    //std::cout << " DMS=[" << testCoord.TextDMS() << "]\n";

    GPSCoordinateC restoredCoord(testCoord.TextDMS());
    //std::cout << " RestoredDMS=[" << restoredCoord.TextDMS() << "]\n";

    if((testCoord - restoredCoord).SumOfSqr() > 0.0001) return __LINE__;

    RavlN::Point3dC cartesianPlace = testCoord.Cartesian();
    //std::cout << " Metric=" << cartesianPlace << "\n";

    GPSCoordinateC recoveredGps = GPSCoordinateC::Cartesian2GPS(cartesianPlace,1e-12);
    //std::cout << " Restored DMS=[" << recoveredGps.TextDMS() << "]\n";
  }

  const UIntT ntestdata = 7;
  const GPSCoordinateC testdata[ntestdata] = {
    GPSCoordinateC(-26.2025543,28.032913,1730),
    GPSCoordinateC( 120,60,-20 ),
    GPSCoordinateC( -10,-10,0 ),
    GPSCoordinateC( -10,15,10 ),
    GPSCoordinateC( -80,34.3,123 ),
    GPSCoordinateC( 170,170,100 ),
    GPSCoordinateC(" 51.240322 N, 0.614352 W" )
  };

  for(UIntT i = 0;i < ntestdata;i++) {
    const GPSCoordinateC &gps = testdata[i];

    // Check text conversion.
    StringC stdCoordDMS = gps.TextDMS();
    //std::cout << "gps=" << stdCoordDMS << "\n";

    GPSCoordinateC restoredTextCoord(stdCoordDMS);
    if((gps - restoredTextCoord).SumOfSqr() > 0.0001) return __LINE__;

    // Check cartersian conversion.
    RavlN::Point3dC cart = gps.Cartesian();
    GPSCoordinateC recoveredCartGps = GPSCoordinateC::Cartesian2GPS(cart,1e-12);
    if(cart.EuclidDistance(recoveredCartGps.Cartesian()) > 0.01) return __LINE__;

    // Check differentials.
    RavlN::Vector3dC dlat,dlong,dheight;
    gps.Differential(dlat,dlong,dheight);

#if 0
    std::cout << "DiffLat=" << dlat << "\n";
    std::cout << "DiffLong=" << dlong << "\n";
    std::cout << "DiffHeight=" << dheight << "\n";

    std::cout << "lat.long=" << dlat.Dot(dlong) << "\n";
    std::cout << "lat.vert=" << dlat.Dot(dheight) << "\n";
    std::cout << "long.vert=" << dlong.Dot(dheight) << "\n";
#endif
  }
  return 0;
}

