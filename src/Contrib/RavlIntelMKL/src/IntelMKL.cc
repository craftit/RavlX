// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, OmniPerception Ltd and University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlIntelMKL

#include "Ravl/IntelMKL.hh"
#include "Ravl/LAHooks.hh"
#include "Ravl/SArray1d.hh"
#include "mkl_lapacke.h"
#include "mkl_lapack.h"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  //: Invert a matrix in-place using Intel MKL functions
  bool InverseIP_IntelMKL(MatrixC & m)
  {
    ONDEBUG(cerr << "Intel MKL Inverse" << endl);
    // this tries to find inverse of general matrix
    IntT pivotSize = min(m.Rows(), m.Cols());
    SArray1dC<IntT> ipiv(pivotSize);
    SArray1dC<RealT> work(m.Rows() * m.Cols());
    IntT info = LAPACKE_dgetrf(LAPACK_ROW_MAJOR, m.Rows().V(), m.Cols().V(), &m[0][0], pivotSize, &ipiv[0]);
    if (info != 0) {
      cerr << "trouble computing pivot";
      return false;
    }

    // OK lets compute the inverse
    info = LAPACKE_dgetri(LAPACK_ROW_MAJOR, m.Cols(), &m[0][0], pivotSize, &ipiv[0]);
    if (info != 0) {
      cerr << "trouble computing inverse" << endl;
      return false;
    }

    return true;
  }

  //: Invert a matrix in-place using Intel MKL functions
  bool InverseIP_IntelMKL_hook(MatrixC & m, RealT & det)
  {
    det = -1.0;
    return InverseIP_IntelMKL(m);
  }

  //! compute Eigen-values and Eigen-vectors for real symmetric matrices
  bool EigenVectors_IntelMKL(const MatrixC &mat, VectorMatrixC & vm)
  {
    ONDEBUG(cerr << "Intel MKL EigenVectors" << endl);
    VectorC eigenValues(mat.Rows());
    MatrixC eigenVectors = mat.Copy();
    IntT info = LAPACKE_dsyev(LAPACK_ROW_MAJOR, 'V', 'U', mat.Rows(), &(eigenVectors[0][0]), mat.Rows(),
        &(eigenValues[0]));
    if (info != 0) {
      cerr << "trouble computing eigen vectors" << endl;
      return false;
    }
    vm = VectorMatrixC(eigenValues, eigenVectors);
    return true;
  }

  //! Eigenvectors hook function for Ravl
  bool EigenVectors_IntelMKL_hook(VectorC & evalues, MatrixC & evector, const MatrixC & m)
  {
    VectorMatrixC vm;
    if(!EigenVectors_IntelMKL(m, vm)) {
      return false;
    }
    evalues = vm.Vector();
    evector = vm.Matrix();
    return true;
  }

  //! Compute SVD of matrix
  bool SVD_IP_IntelMKL(MatrixC & m, MatrixC & u, VectorC & s, MatrixC & v)
  {

    ONDEBUG(cerr << "Intel SVD" << endl);
    // both of these work, no difference between them
#if 1
    MatrixC t = m.T();
    IntT rows = m.Rows();
    IntT cols = m.Cols();
    IntT min = Min(rows, cols);
    IntT lda = rows;
    IntT ldu = rows;
    IntT ldvt = min;
    s = VectorC(min);
    u = MatrixC(min, ldu);
    MatrixC vt(cols, ldvt);
    VectorC superb(min - 1);
    IntT info = LAPACKE_dgesvd(LAPACK_COL_MAJOR, 'S', 'S', rows, cols, &t[0][0], lda, &s[0], &u[0][0], ldu, &vt[0][0],
        ldvt, &superb[0]);
    if (info != 0) {
      cerr << "trouble computing SVD" << endl;
      return false;
    }
    u = u.T(); // argh!
    v = vt;
#else
    MatrixC t = m.T(); // fortran code so have to transpose, argh!
    IntT rows = m.Rows();
    IntT cols = m.Cols();
    IntT min = Min(rows, cols);
    IntT lda = rows;
    IntT ldu = rows;
    IntT ldvt = min;
    IntT info;
    s = VectorC(min);
    u = MatrixC(min, ldu); // OK this seems a little screwed but because matrices are stored column wise by Lapack we need to swap
    MatrixC vt(cols, ldvt); // ditto
    int lwork = -1;
    // Work out optimal work-space size
    double wkopt;
    char A = 'S';
    char S = 'S';
    dgesvd_(&A, &S, &rows, &cols, &t[0][0], &lda, &s[0], &u[0][0], &ldu, &vt[0][0], &ldvt, &wkopt, &lwork, &info);
    lwork = (int) wkopt;
    VectorC work(lwork);
    // do the actual SVD
    dgesvd_(&A, &S, &rows, &cols, &t[0][0], &lda, &s[0], &u[0][0], &ldu, &vt[0][0], &ldvt, &work[0], &lwork, &info);
    if (info != 0) {
      cerr << "trouble computing SVD" << endl;
      return false;
    }

    u = u.T(); // argh!
    v = vt;
#endif
    return true;
  }

  int IntelMKLHooksInit()
  {
    cerr << "using IntelMKL\n";
    g_EigenVectorsSymmetric = &EigenVectors_IntelMKL_hook;
    g_SVD_IP_hook = &SVD_IP_IntelMKL;
    g_Inverse_IP_hook = &InverseIP_IntelMKL_hook;
    return 0;
  }

  static int a = IntelMKLHooksInit();

}
