// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/*
 * LAHooksLAPACK.cc
 *
 *  Created on: 2 Mar 2010
 *      Author: Alexey Kostin
 */
//! lib=RavlLapack

#include "Ravl/LAHooks.hh"

#include "Ravl/Vector.hh"
#include "Ravl/Matrix.hh"
#include "Ravl/Lapack/lapack.hh"
#include "Ravl/Lapack/blas2.hh"
#

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

//! compute eigen values and eigen vectors of symmetric matrix
static bool EigenVectorsSymmetric_LAPACK(VectorC &resVals, MatrixC &resVecs, const MatrixC &mat) {
  VectorMatrixC vm;
  EigenVectors_Lapack(mat, vm);
  resVals = vm.Vector();
  resVecs = vm.Matrix();
  return true;
}

//: Add outer product of 'vec' with itself to this matrix.
static void AddOuterProduct_LAPACK(MatrixRUTC &matr, const VectorC &vec) {
  AddOuterProduct(matr, vec, 1.);
}

//: Add outer product of 'vec' with itself to this matrix.
static void AddOuterProduct1_LAPACK(MatrixRUTC &matr, const VectorC &vec,RealT a) {
  AddOuterProduct(matr, vec, a);
}

//: Subtract outer product of 'vec' with itself to this matrix.
static void SubtractOuterProduct_LAPACK(MatrixRUTC &matr, const VectorC &vec) {
  AddOuterProduct(matr, vec, -1.);
}

//: Subtract outer product of 'vec' with itself to this matrix.
static void SubtractOuterProduct1_LAPACK(MatrixRUTC &matr, const VectorC &vec,RealT a) {
  AddOuterProduct(matr, vec, a);
}





int LAHooksLAPACKInit() {
  ONDEBUG(cerr << "using LAPACK\n");
  g_EigenVectorsSymmetric = &EigenVectorsSymmetric_LAPACK;
  g_AddOuterProduct = &AddOuterProduct_LAPACK;
  g_AddOuterProduct1 = &AddOuterProduct1_LAPACK;
  g_SubtractOuterProduct = &SubtractOuterProduct_LAPACK;
  g_SubtractOuterProduct1 = &SubtractOuterProduct1_LAPACK;
  g_SVD_IP_hook = &SVD_IP_Lapack;
  return 0;
}

static int a = LAHooksLAPACKInit();

}
