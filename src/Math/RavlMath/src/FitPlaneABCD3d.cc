// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, OmniPerception Ltd 
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlMath

#include "Ravl/PlaneABCD3d.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/Matrix3d.hh"

namespace RavlN {
  
  //: Least squares fit of a plane to a set of points in 3d
  // At least 3 points are needed.
  
  bool FitPlane(const SArray1dC<Point3dC> &points,PlaneABCD3dC &plane) {
    if(points.Size() < 3)
      return false;
    
    // Compute the mean of the points
    
    Point3dC mean(0,0,0);
    for(SArray1dIterC<Point3dC> it(points);it;it++) {
      mean += *it;
    }
    mean /= points.Size();
    
    // Compute the covariance matrix.
    
    Matrix3dC covar(0,0,0,
		    0,0,0,
		    0,0,0);
    
    for(SArray1dIterC<Point3dC> it(points);it;it++) {
      Point3dC p = *it - mean;
      for(int i = 0;i < 3;i++) {
	for(int j = i;j < 3;j++)
	  covar[i][j] += p[i] * p[j];
      }
    }
    
    // Make it symetric.
    covar[1][0] = covar[0][1];
    covar[2][0] = covar[0][2];
    covar[2][1] = covar[1][2];
    
    Matrix3dC u,v;
    Vector3dC s = SVD_IP(covar,u,v);

#if 0
    std::cerr << "Singular values= " << s << "\n";
    std::cerr << "U= " << u << "\n";
    std::cerr << "V= " << v << "\n";
#endif
    
    // Find smallest singular value,
    // they are normally sorted, but lets be paranoid.
    RealT min = s[0];
    IntT minI = 0;
    for(int i = 1;i < 3;i++) {
      if(s[i] < min) {
	minI = i;
	min = s[i];
      }
    }
    
    RealT a = v[0][minI];
    RealT b = v[1][minI];
    RealT c = v[2][minI];
    RealT d = -(a*mean[0] + b*mean[1] + c*mean[2]);
    
    plane = PlaneABCD3dC(a, b, c, d);
    
    return true;
  }
  
}
