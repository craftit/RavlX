// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/LinearAlgebra/FixedSize/Matrix2d.cc"

#include "Ravl/Matrix2d.hh"

namespace RavlN {
  
  //: Calculate the eigen values of a real symmetric matrix.
  // This matrix remains unchanged. A vector of the eigen
  // values is assigned to 'vec'.
  
  void EigenValues(const FMatrixC<2,2> &mat,FVectorC<2> &vec) {
    if (mat[0][1] == 0) { // The matrix is diagonal
      if (mat[0][0] > mat[1][1]) {
	vec[0] = mat[0][0];
	vec[1] = mat[1][1];
      } else {
	vec[0] = mat[1][1];
	vec[1] = mat[0][0];
      }
      return ;
    }
    const RealT d  = Sqr(mat[0][0]-mat[1][1]) + 4 * Sqr(mat[0][1]);
    const RealT sd = Sqrt(d);
    RealT sum00_11 = mat[0][0]+mat[1][1];
    vec[0] = (sum00_11 + sd)/2.0;
    vec[1] = (sum00_11 - sd)/2.0;
    if (Abs(vec[0]) < Abs(vec[1]))
      Swap(vec[0],vec[1]);
  }
  
  //: Calculate the eigen values and vectors of a real symmetric matrix.
  // A = E*D*E~ where D is the diagonal matrix of eigenvalues
  //   D[i,j] = ret[i] if i=j and 0 otherwise. 
  
  void EigenVectors(const FMatrixC<2,2> &mat,FMatrixC<2,2> &E,FVectorC<2> &D) {
    if (mat[0][1] == 0) { // it is a diagonal matrix
      if (mat[0][0] > mat[1][1]) {
	D[0] = mat[0][0];
	D[1] = mat[1][1];
	E = Matrix2dC(1.0,0.0,0.0,1.0);
      } else {
	D[0] = mat[1][1];
	D[1] = mat[0][0];
	E = Matrix2dC(0.0, 1.0, 1.0, 0.0);
      }
      return ;
    }
    
    // Compute eigenvalues.
    RealT d = Sqrt(Sqr(mat[0][0]-mat[1][1]) + 4 * Sqr(mat[0][1]));
    RealT sum00_11 = mat[0][0]+mat[1][1];
    RealT lambda1 = (sum00_11 + d)/2.0;
    RealT lambda2 = (sum00_11 - d)/2.0;
    
    // Change the order of the eigenvalues if necessary.
    if (Abs(lambda1) < Abs(lambda2))
      Swap(lambda1,lambda2);
    
    // Construct the first eigenvector.
    Vector2dC u1( mat[0][1], lambda1 - mat[0][0]);
    RealT size = u1.Modulus();
    // Change the direction of the vector to point to the I. or IV. quadrant.
    if (mat[0][1] < 0) 
      size = -size;
    // Make the vector to be unit.
    u1 /= size;
    
    D[0] = lambda1;
    D[1] = lambda2;
    
    E[0][0] = u1[0];
    E[1][1] = u1[0];
    E[0][1] = -u1[1];
    E[1][0] = u1[1];
  }
  
}
