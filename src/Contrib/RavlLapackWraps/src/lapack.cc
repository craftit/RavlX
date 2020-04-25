// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2007, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
// $Id$
//! lib=RavlLapackWraps

#include "Ravl/Lapack/lapack.hh"
#include "Ravl/Lapack/ev_c.hh"

// This file contains C wrappers of LAPACK function for eigen vectors computation
extern "C" {

#ifdef WIN32
  void dgesvd_(char *jobu, char *jobvt, int *m, int *n, double *a, int *lda, double *s, double *u, int * ldu,
      double *vt, int *ldvt, double *work, int *lwork, int *info)
  {
    abort();
    return;
  }

  void dgetrf_(int *m, int *n, double *a, int *lda, int *ipiv, int *info) {
    abort();
    return;
  }

  void dgetri_(int *n, double *a, int *lda, int *ipiv, double *work, int *lwork, int *info) {
    abort();
    return;
  }
#else
  void dgesvd_(char *jobu, char *jobvt, int *m, int *n, double *a, int *lda, double *s, double *u, int * ldu,
      double *vt, int *ldvt, double *work, int *lwork, int *info);

  void dgetrf_(int *m, int *n, double *a, int *lda, int *ipiv, int *info);

  void dgetri_(int *n, double *a, int *lda, int *ipiv, double *work, int *lwork, int *info);

#endif
}

namespace RavlN {

  //: Invert a matrix in-place using Intel MKL functions
  bool InverseIP_Lapack(MatrixC & m)
  {
    if(m.Rows() != m.Cols())
      return false;

    IntT n = m.Rows().V();
    m = m.T(); // things stored column wise
    // this tries to find inverse of general matrix
    SArray1dC<IntT> ipiv(n);
    IntT info;

    dgetrf_(&n, &n, &m[0][0], &n, &ipiv[0], &info);
    if (info != 0) {
      cerr << "trouble computing pivot";
      return false;
    }

    // OK lets compute the optimal workspace
    int lwork = -1;
    RealT workSize;
    dgetri_(&n, &m[0][0], &n, &ipiv[0], &workSize, &lwork, &info);
    lwork = (IntT)workSize;

    // Allocate work space and compute inverse
    SArray1dC<RealT> work(lwork);
    dgetri_(&n, &m[0][0], &n, &ipiv[0], &work[0], &lwork, &info);
    if (info != 0) {
      cerr << "trouble computing inverse" << endl;
      return false;
    }

    // Transpose back result to get row major
    m = m.T();

    return true;
  }

  bool EigenVectors_Lapack(const MatrixC & mat, VectorMatrixC & vm)
  {
    const SizeT mSize = mat.Size1();
    VectorC resVals(mSize);
    MatrixC resVecs(mat.Copy());
    dsyev_c(mSize, &(resVecs[0][0]), &(resVals[0]), true, true, true, false);
    vm.Vector() = resVals;
    vm.Matrix() = resVecs;
    return true;
  }

  //! compute singular value decomposition of matrix using Lapack

  bool SVD_IP_Lapack(MatrixC & m, MatrixC & u, VectorC & s, MatrixC & v)
  {
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

    return true;
  }

} //end of namespace
