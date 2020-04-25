// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_MATRIX2D_HEADER
#define RAVL_MATRIX2D_HEADER 1
/////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Math/LinearAlgebra/FixedSize/Matrix2d.hh"
//! lib=RavlMath
//! userlevel=Normal
//! author="Charles Galambos"
//! date="24/01/2001"
//! docentry="Ravl.API.Math.Geometry.2D"

#include "Ravl/FMatrix.hh"
#include "Ravl/Vector2d.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: 2 by 2 matrix of real values.
  
  class Matrix2dC
    : public FMatrixC<2,2>
  {
  public:
    Matrix2dC()
      {}
    //: Default constructor.
    
    Matrix2dC(const TFMatrixC<RealT,2,2> &base)
      : FMatrixC<2,2>(base)
      {}
    //: Constructor.
    
    Matrix2dC(RealT v00,RealT v01,RealT v10,RealT v11) { 
      data[0][0] = v00;
      data[0][1] = v01;
      data[1][0] = v10;
      data[1][1] = v11;
    }
    //: Constructor.

    Matrix2dC(const FVectorC<2> &v0,const FVectorC<2> &v1)
    {
      data[0][0] = v0[0];
      data[0][1] = v1[0];
      data[1][0] = v0[1];
      data[1][1] = v1[1];
    }
    //: Compose the matrix from 2 column vectors.
    
    inline RealT Det() const 
    { return data[0][0]*data[1][1] - data[0][1]*data[1][0]; }
    //: Returns the value of the determinant of this matrix.
    
    bool Invert(TFMatrixC<RealT,2,2> &op) const {
      RealT det = Det();
      if(det == 0)
	return false;
      op[0][0] =  data[1][1]/det; 
      op[0][1] = -data[0][1]/det;
      op[1][0] = -data[1][0]/det; 
      op[1][1] =  data[0][0]/det;
      return true;
    }
    //: Invert this matrix put the result in 'op'
    // Returns false if matrix is singular.
    
    Matrix2dC Invert() const {
      Matrix2dC ret;
      Invert(ret);
      return ret;
    }
    //: Invert matrix and return result.
    
    bool InvertIP() {
      Matrix2dC org = (*this);
      return org.Invert(*this);
    }
    //: Invert matrix in place.
    
  };
  
  template<>
  inline
  TFVectorC<RealT,2> TFMatrixC<RealT,2,2>::operator*(const TFVectorC<RealT,2> & vec) const {
    TFVectorC<RealT,2> ret;
    ret[0] = data[0][0] * vec[0] + data[0][1] * vec[1];
    ret[1] = data[1][0] * vec[0] + data[1][1] * vec[1];
    return ret;
  }
  
  inline
  void MulAdd(const TFMatrixC<RealT,2,2> & R,const TFVectorC<RealT,2> & x, const TFVectorC<RealT,2> & t, TFVectorC<RealT,2> &result) {
    result[0] = R[0][0]*x[0] + R[0][1]*x[1] + t[0];
    result[1] = R[1][0]*x[0] + R[1][1]*x[1] + t[1];    
  }
  
  inline
  void Mul(const TFMatrixC<RealT,2,2> & R,const TFVectorC<RealT,2> & x, TFVectorC<RealT,2> &result) {
    result[0] = R[0][0]*x[0] + R[0][1]*x[1];
    result[1] = R[1][0]*x[0] + R[1][1]*x[1];
  }
  
  inline
  void MulM(const TFMatrixC<RealT,2,2> & R1, const TFMatrixC<RealT,2,2> &R2, TFMatrixC<RealT,2,2> & result) {
    result[0][0] = R1[0][0]*R2[0][0] + R1[0][1]*R2[1][0];
    result[0][1] = R1[0][0]*R2[0][1] + R1[0][1]*R2[1][1];
    result[1][0] = R1[1][0]*R2[0][0] + R1[1][1]*R2[1][0];
    result[1][1] = R1[1][0]*R2[0][1] + R1[1][1]*R2[1][1];
  }
  
  inline
  void MulT(const TFMatrixC<RealT,2,2> &R1,const TFMatrixC<RealT,2,2> & R2 ,TFMatrixC<RealT,2,2> &result) {
    result[0][0] = R1[0][0]*R2[0][0] + R1[0][1]*R2[0][1];
    result[0][1] = R1[0][0]*R2[1][0] + R1[0][1]*R2[1][1];
    result[1][0] = R1[1][0]*R2[0][0] + R1[1][1]*R2[0][1];
    result[1][1] = R1[1][0]*R2[1][0] + R1[1][1]*R2[1][1];
  }
  
  template<>
  inline
  TFMatrixC<RealT,2,2> &TFVectorC<RealT,2>::OuterProduct(TFMatrixC<RealT,2,2> &result) const {
    result[0][0] = data[0] * data[0];
    result[0][1] = data[0] * data[1];
    result[1][0] = data[1] * data[0];
    result[1][1] = data[1] * data[1];
    return result;
  }
  //: Calculate the outer product of this vector with itsself.
  // The output is assigned to 'result'. A reference to 'result' is
  // returned. <p>

  template<>
  inline
  TFMatrixC<RealT,2,2> &TFVectorC<RealT,2>::OuterProduct(const TFVectorC<RealT,2> &av,TFMatrixC<RealT,2,2> &result) const {
    result[0][0] = av[0] * data[0];
    result[0][1] = av[0] * data[1];
    result[1][0] = av[1] * data[0];
    result[1][1] = av[1] * data[1];
    return result;
  }
  //: Calculate the outer product of this vector with av.
  // The output is assigned to 'result'. A reference to 'result' is
  // returned. <p>
  
  template<>
  inline
  const TFMatrixC<RealT,2,2> &TFMatrixC<RealT,2,2>::operator+=(const TFMatrixC<RealT,2,2> & mat) {
    data[0][0] += mat.data[0][0];
    data[0][1] += mat.data[0][1];
    data[1][0] += mat.data[1][0];
    data[1][1] += mat.data[1][1];
    return *this;
  }
  
  template<>
  inline
  const TFMatrixC<RealT,2,2> &TFMatrixC<RealT,2,2>::operator-=(const TFMatrixC<RealT,2,2> & mat) {
    data[0][0] -= mat.data[0][0];
    data[0][1] -= mat.data[0][1];
    data[1][0] -= mat.data[1][0];
    data[1][1] -= mat.data[1][1];
    return *this;
  }
  
  void EigenValues(const FMatrixC<2,2> &mat,FVectorC<2> &vec);
  //: Calculate the eigen values of a real symmetric matrix.
  // This matrix remains unchanged. A vector of the eigen
  // values is assigned to 'vec'.
  
  inline
  void EigenValuesIP(FMatrixC<2,2> &mat,FVectorC<2> &vec) 
  { EigenValues(mat,vec); }
  //: Calculate the eigen values of a real symmetric matrix.
  // The contents of this matrix may be destroyed. The vector 
  // is placed in 'vec'.
  
  void EigenVectors(const FMatrixC<2,2> &mat,FMatrixC<2,2> &E,FVectorC<2> &D); 
  //: Calculate the eigen values and vectors of a real symmetric matrix.
  // A = E*D*E~ where D is the diagonal matrix of eigenvalues
  //   D[i,j] = ret[i] if i=j and 0 otherwise. 
  
  inline
  void EigenVectorsIP(FMatrixC<2,2> &mat,FVectorC<2> &D) {
    Matrix2dC tmp;
    EigenVectors(mat,tmp,D);
    mat = tmp;
  }
  //: Calculate the eigen values and vectors of a real symmetric matrix.
  // This matrix is filed with the eigen vectors
  // A = E*D*E~ where D is the diagonal matrix of eigenvalues
  //   D[i,j] = ret[i] if i=j and 0 otherwise. 
  
  inline
  RealT Determinant(const TFMatrixC<RealT,2,2> &mat) 
  { return ((const Matrix2dC &)mat).Det(); }
  //: Compute the determinant of a 2x2 matrix.
  
}

#endif
