// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2007, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
// $Id$
//! lib=RavlLapackWraps

#include<stdlib.h>

extern "C"
{
  /*
  SUBROUTINE DSYR(UPLO,N,ALPHA,X,INCX,A,LDA)
  *     .. Scalar Arguments ..
  DOUBLE PRECISION ALPHA
  INTEGER INCX,LDA,N
  CHARACTER UPLO
  *     ..
  *     .. Array Arguments ..
  DOUBLE PRECISION A(LDA,*),X(*)
  *     ..
  *
  *  Purpose
  *  =======
  *
  *  DSYR   performs the symmetric rank 1 operation
  *
  *     A := alpha*x*x' + A,
  *
  *  where alpha is a real scalar, x is an n element vector and A is an
  *  n by n symmetric matrix.
  *
  *  Arguments
  *  ==========
  *
  *  UPLO   - CHARACTER*1.
  *           On entry, UPLO specifies whether the upper or lower
  *           triangular part of the array A is to be referenced as
*           follows:
  *
  *              UPLO = 'U' or 'u'   Only the upper triangular part of A
  *                                  is to be referenced.
  *
  *              UPLO = 'L' or 'l'   Only the lower triangular part of A
  *                                  is to be referenced.
  *
  *           Unchanged on exit.
  *
  *  N      - INTEGER.
  *           On entry, N specifies the order of the matrix A.
  *           N must be at least zero.
  *           Unchanged on exit.
  *
  *  ALPHA  - DOUBLE PRECISION.
  *           On entry, ALPHA specifies the scalar alpha.
  *           Unchanged on exit.
  *
  *  X      - DOUBLE PRECISION array of dimension at least
  *           ( 1 + ( n - 1 )*abs( INCX ) ).
  *           Before entry, the incremented array X must contain the n
  *           element vector x.
  *           Unchanged on exit.
  *
  *  INCX   - INTEGER.
  *           On entry, INCX specifies the increment for the elements of
  *           X. INCX must not be zero.
  *           Unchanged on exit.
  *
  *  A      - DOUBLE PRECISION array of DIMENSION ( LDA, n ).
  *           Before entry with  UPLO = 'U' or 'u', the leading n by n
  *           upper triangular part of the array A must contain the upper
  *           triangular part of the symmetric matrix and the strictly
  *           lower triangular part of A is not referenced. On exit, the
  *           upper triangular part of the array A is overwritten by the
  *           upper triangular part of the updated matrix.
  *           Before entry with UPLO = 'L' or 'l', the leading n by n
  *           lower triangular part of the array A must contain the lower
  *           triangular part of the symmetric matrix and the strictly
  *           upper triangular part of A is not referenced. On exit, the
  *           lower triangular part of the array A is overwritten by the
  *           lower triangular part of the updated matrix.
  *
  *  LDA    - INTEGER.
  *           On entry, LDA specifies the first dimension of A as declared
  *           in the calling (sub) program. LDA must be at least
  *           max( 1, n ).
  *           Unchanged on exit.
  */
#if WIN32
void dsyr_(const char &uplo, const int &n, const double &alpha,
             const double *x, const int &incx, double *a, const int &lda)
{
	abort();
	return;
}
#else
  void dsyr_(const char &uplo, const int &n, const double &alpha,
             const double *x, const int &incx, double *a, const int &lda);
#endif
}

/*! Adds outer product to the matrix
 * @param Size - size of matrix
 * @param Matrix - matrix itself
 * @param Vector - vector
 * @param Aplfa - scale for the outer product
 * @param IncX - increment for the index in Vector
 * @param UpperTriangle - use upper triangle (true) otherwise lower triangle (false)
 * @param RowMajor - matrix is stored in row-major order (C standard)
 */
void dsyr_c(int Size, double *Matrix, const double *Vector, double Alpha,
            const int &IncX, bool UpperTriangle, bool RowMajor)
{
  if(RowMajor)
    UpperTriangle = !UpperTriangle;
  dsyr_(UpperTriangle ? 'U' : 'L', Size, Alpha, Vector, IncX, Matrix, Size);
}
