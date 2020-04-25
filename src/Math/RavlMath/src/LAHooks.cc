// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/*
 * LAHooks.cc
 *
 *  Created on: 2 Mar 2010
 *      Author: Alexey Kostin
 */
//! lib=RavlMath

#include "Ravl/LAHooks.hh"

#include "Ravl/Vector.hh"
#include "Ravl/Matrix.hh"
#include "Ravl/MatrixRUT.hh"
#include "Ravl/VectorMatrix.hh"
#include "Ravl/Eigen.hh"
#include "Ravl/SVD.hh"

namespace RavlN {


static bool EigenVectorsSymmetric_default(VectorC &resVals, MatrixC &resVecs, const MatrixC &mat) {
  EigenValueC<RealT> ev(mat);
  resVals = ev.EigenValues();
  resVecs = ev.EigenVectors();
  return true;
}


//: Add outer product of 'vec' with itself to this matrix.
static void AddOuterProduct_default(MatrixRUTC &matr, const VectorC &vec) {
  SArray2dIterC<RealT> mit(matr);
  int off = 0;
  for(BufferAccessIterC<RealT> it(vec);it;it++) {
    const RealT v1 = (*it);
    for(BufferAccessIterC<RealT> it2 = it;it2;it2++,mit.NextCol())
      *mit += v1 * (*it2);
    mit.NextRow(++off);
  }
}

//: Add outer product of 'vec' with itself to this matrix.
static void AddOuterProduct1_default(MatrixRUTC &matr, const VectorC &vec,RealT a) {
  SArray2dIterC<RealT> mit(matr);
  int off = 0;
  for(BufferAccessIterC<RealT> it(vec);it;it++) {
    const RealT v1 = *it * a;
    for(BufferAccessIterC<RealT> it2 = it;it2;it2++,mit.NextCol())
      *mit += v1 * (*it2);
    mit.NextRow(++off);
  }
}

//: Subtract outer product of 'vec' with itself to this matrix.
static void SubtractOuterProduct_default(MatrixRUTC &matr, const VectorC &vec) {
  SArray2dIterC<RealT> mit(matr);
  int off = 0;
  for(BufferAccessIterC<RealT> it(vec);it;it++) {
    const RealT v1 = (*it);
    for(BufferAccessIterC<RealT> it2 = it;it2;it2++,mit.NextCol())
      *mit -= v1 * (*it2);
    mit.NextRow(++off);
  }
}

//: Subtract outer product of 'vec' with itself to this matrix.
static void SubtractOuterProduct1_default(MatrixRUTC &matr, const VectorC &vec,RealT a) {
  SArray2dIterC<RealT> mit(matr);
  int off = 0;
  for(BufferAccessIterC<RealT> it(vec);it;it++) {
    const RealT v1 = *it * a;
    for(BufferAccessIterC<RealT> it2 = it;it2;it2++,mit.NextCol())
      *mit -= v1 * (*it2);
    mit.NextRow(++off);
  }
}

//: Compute SVD of matrix
static bool SVD_IP_default(MatrixC & m, MatrixC & u, VectorC & s, MatrixC & v) {
  SVDC<RealT> svd(m);
  v = svd.GetV();
  u = svd.GetU();
  s = svd.SingularValues();
  return true;
}

//: Compute inverse of matrix using standard Ravl algorithm
static bool Inverse_IP_default(MatrixC & m, RealT & det) {
  return InverseIP_GaussJordan(m, det);
}



bool (*g_EigenVectorsSymmetric)(VectorC &resVals, MatrixC &resVecs, const MatrixC &M) = &EigenVectorsSymmetric_default;
void (*g_AddOuterProduct)(MatrixRUTC &matr, const VectorC &vec) = &AddOuterProduct_default;
void (*g_AddOuterProduct1)(MatrixRUTC &matr, const VectorC &vec, RealT a) = &AddOuterProduct1_default;
void (*g_SubtractOuterProduct)(MatrixRUTC &matr, const VectorC &vec) = &SubtractOuterProduct_default;
void (*g_SubtractOuterProduct1)(MatrixRUTC &matr, const VectorC &vec, RealT a) = &SubtractOuterProduct1_default;
bool (*g_SVD_IP_hook)(MatrixC & m, MatrixC & u, VectorC & s, MatrixC & v) = &SVD_IP_default;
bool (*g_Inverse_IP_hook)(MatrixC & m, RealT & det) = &Inverse_IP_default;


}
