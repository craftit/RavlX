// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2007, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
// $Id$
#include <stdlib.h>
#include <string.h>
#ifdef WIN32
#include <malloc.h>
#endif
//! lib=RavlLapackWraps

//This file contains C wrappers of LAPACK function for eigen vectors computation
extern "C"
{
  /* SUBROUTINE SGESV( N, NRHS, A, LDA, IPIV, B, LDB, INFO)

      INTEGER    INFO, LDA, LDB, N, NRHS
      INTEGER    IPIV( * )
      REAL       A( LDA, * ), B( LDB, * )
  *
  *  -- LAPACK driver routine (version 3.0) --
  *     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,
  *     Courant Institute, Argonne National Lab, and Rice University
  *     June 30, 1999
  *
  *     .. Scalar Arguments ..
        CHARACTER          JOBZ, UPLO
        INTEGER            INFO, LDA, LWORK, N
  *     ..
  *     .. Array Arguments ..
        REAL               A( LDA, * ), W( * ), WORK( * )
  *     ..
  *
  *  Purpose
  *  =======
  *
  *  SSYEV computes all eigenvalues and, optionally, eigenvectors of a
  *  real symmetric matrix A.
  *
  *  Arguments
  *  =========
  *
  *  JOBZ    (input) CHARACTER*1
  *          = 'N':  Compute eigenvalues only;
  *          = 'V':  Compute eigenvalues and eigenvectors.
  *
  *  UPLO    (input) CHARACTER*1
  *          = 'U':  Upper triangle of A is stored;
  *          = 'L':  Lower triangle of A is stored.
  *
  *  N       (input) INTEGER
  *          The order of the matrix A.  N >= 0.
  *
  *  A       (input/output) REAL array, dimension (LDA, N)
  *          On entry, the symmetric matrix A.  If UPLO = 'U', the
  *          leading N-by-N upper triangular part of A contains the
  *          upper triangular part of the matrix A.  If UPLO = 'L',
  *          the leading N-by-N lower triangular part of A contains
  *          the lower triangular part of the matrix A.
  *          On exit, if JOBZ = 'V', then if INFO = 0, A contains the
  *          orthonormal eigenvectors of the matrix A.
  *          If JOBZ = 'N', then on exit the lower triangle (if UPLO='L')
  *          or the upper triangle (if UPLO='U') of A, including the
  *          diagonal, is destroyed.
  *
  *  LDA     (input) INTEGER
  *          The leading dimension of the array A.  LDA >= max(1,N).
  *
  *  W       (output) REAL array, dimension (N)
  *          If INFO = 0, the eigenvalues in ascending order.
  *
  *  WORK    (workspace/output) REAL array, dimension (LWORK)
  *          On exit, if INFO = 0, WORK(1) returns the optimal LWORK.
  *
  *  LWORK   (input) INTEGER
  *          The length of the array WORK.  LWORK >= max(1,3*N-1).
  *          For optimal efficiency, LWORK >= (NB+2)*N,
  *          where NB is the blocksize for SSYTRD returned by ILAENV.
  *
  *          If LWORK = -1, then a workspace query is assumed; the routine
  *          only calculates the optimal size of the WORK array, returns
  *          this value as the first entry of the WORK array, and no error
  *          message related to LWORK is issued by XERBLA.
  *
  *  INFO    (output) INTEGER
  *          = 0:  successful exit
  *          < 0:  if INFO = -i, the i-th argument had an illegal value
  *          > 0:  if INFO = i, the algorithm failed to converge; i
  *                off-diagonal elements of an intermediate tridiagonal
  *                form did not converge to zero.
  *
  *  =====================================================================*/
#ifdef WIN32 
	void ssyev_(const char &jobz, const char &uplo, const int &n,
              float *a, const int &lda, float *w, float *work,
              const int &lwork, const int &info)
	{	
		abort();
		return;
	}
#else
	void ssyev_(const char &jobz, const char &uplo, const int &n,
              float *a, const int &lda, float *w, float *work,
              const int &lwork, const int &info);
#endif


  /*
  SUBROUTINE DSYEV( JOBZ, UPLO, N, A, LDA, W, WORK, LWORK, INFO )
  *
  *  -- LAPACK driver routine (version 3.1) --
  *     Univ. of Tennessee, Univ. of California Berkeley and NAG Ltd..
  *     November 2006
  *
  *     .. Scalar Arguments ..
  CHARACTER          JOBZ, UPLO
  INTEGER            INFO, LDA, LWORK, N
  *     ..
  *     .. Array Arguments ..
  DOUBLE PRECISION   A( LDA, * ), W( * ), WORK( * )
  *     ..
  *
  *  Purpose
  *  =======
  *
  *  DSYEV computes all eigenvalues and, optionally, eigenvectors of a
  *  real symmetric matrix A.
  *
  *  Arguments
  *  =========
  *
  *  JOBZ    (input) CHARACTER*1
  *          = 'N':  Compute eigenvalues only;
  *          = 'V':  Compute eigenvalues and eigenvectors.
  *
  *  UPLO    (input) CHARACTER*1
  *          = 'U':  Upper triangle of A is stored;
  *          = 'L':  Lower triangle of A is stored.
  *
  *  N       (input) INTEGER
  *          The order of the matrix A.  N >= 0.
  *
  *  A       (input/output) DOUBLE PRECISION array, dimension (LDA, N)
  *          On entry, the symmetric matrix A.  If UPLO = 'U', the
  *          leading N-by-N upper triangular part of A contains the
  *          upper triangular part of the matrix A.  If UPLO = 'L',
  *          the leading N-by-N lower triangular part of A contains
  *          the lower triangular part of the matrix A.
  *          On exit, if JOBZ = 'V', then if INFO = 0, A contains the
  *          orthonormal eigenvectors of the matrix A.
  *          If JOBZ = 'N', then on exit the lower triangle (if UPLO='L')
  *          or the upper triangle (if UPLO='U') of A, including the
  *          diagonal, is destroyed.
  *
  *  LDA     (input) INTEGER
  *          The leading dimension of the array A.  LDA >= max(1,N).
  *
  *  W       (output) DOUBLE PRECISION array, dimension (N)
  *          If INFO = 0, the eigenvalues in ascending order.
  *
  *  WORK    (workspace/output) DOUBLE PRECISION array, dimension (MAX(1,LWORK))
  *          On exit, if INFO = 0, WORK(1) returns the optimal LWORK.
  *
  *  LWORK   (input) INTEGER
  *          The length of the array WORK.  LWORK >= max(1,3*N-1).
  *          For optimal efficiency, LWORK >= (NB+2)*N,
  *          where NB is the blocksize for DSYTRD returned by ILAENV.
  *
  *          If LWORK = -1, then a workspace query is assumed; the routine
  *          only calculates the optimal size of the WORK array, returns
  *          this value as the first entry of the WORK array, and no error
  *          message related to LWORK is issued by XERBLA.
  *
  *  INFO    (output) INTEGER
  *          = 0:  successful exit
  *          < 0:  if INFO = -i, the i-th argument had an illegal value
  *          > 0:  if INFO = i, the algorithm failed to converge; i
  *                off-diagonal elements of an intermediate tridiagonal
  *                form did not converge to zero.
  *
  *  ===================================================================== */
#ifdef WIN32
void dsyev_(const char &jobz, const char &uplo, const int &n,
              double *a, const int &lda, double *w, double *work,
              const int &lwork, int &info)
{
	abort();
	return;
}
#else
	void dsyev_(const char &jobz, const char &uplo, const int &n,
              double *a, const int &lda, double *w, double *work,
              const int &lwork, int &info);
#endif
}


/*! computes all eigenvalues and, optionally, eigenvectors of a
 *  real symmetric matrix A.
 * @param Size - order of the matrix
 * @param Matrix - pointer to the first element of the matrix
 * @param EigValues - eigen values
 * @param UpperTriangle - use upper triangle (true) otherwise lower triangle (false)
 * @param CalcVec - whether to compute eigen vectors
 * @param RowMajor - matrix is stored in row-major order (C standard)
 * @param UseStack - use stack memory for caclulations
 *
 * Eigen vectors if computed will be put in the matrix
*/
int ssyev_c(int Size, float *Matrix, float *EigValues, bool UpperTriangle,
            bool CalcVec, bool RowMajor, bool UseStack)
{
  int lwork = 3 * Size - 1;
  float *work = UseStack ? (float*)alloca(lwork * sizeof(float)) :
                           (float*)malloc(lwork * sizeof(float));
  int ok = 0;
  /* to call a Fortran routine from C we have to transpose the matrix since
    matrix is symmetric we just change the sense of UpperTriangle to opposite */
  if(RowMajor)
    UpperTriangle = !UpperTriangle;
  ssyev_(CalcVec ? 'V' : 'N', UpperTriangle ? 'U' : 'L', Size, Matrix,
         Size, EigValues, work, lwork, ok);

  if(!UseStack)
    free(work);

  if(CalcVec && RowMajor) //need to transpose result
  {
    const unsigned int matSize = Size * Size * sizeof(float);
    float *a = UseStack ? (float*)alloca(matSize) : (float*)malloc(matSize);
    memcpy(a, Matrix, matSize);
    float *sPtr = a;

    for(int i = 0; i < Size; i++)
    {
      float *dPtr = Matrix + i;
      float *sPtrE = sPtr + Size;
      while(sPtr < sPtrE)
      {
        *dPtr = *sPtr++;
        dPtr += Size;
      }
    }

    if(!UseStack)
      free(a);
  }
  return ok;
}

/*! computes all eigenvalues and, optionally, eigenvectors of a
 *  real symmetric matrix A.
 * @param Size - order of the matrix
 * @param Matrix - pointer to the first element of the matrix
 * @param EigValues - eigen values
 * @param UpperTriangle - use upper triangle (true) otherwise lower triangle (false)
 * @param CalcVec - whether to compute eigen vectors
 * @param RowMajor - matrix is stored in row-major order (C standard)
 * @param UseStack - use stack memeory for caclulations
 *
 * Eigen vectors if computed will be put in the matrix
 */
int dsyev_c(int Size, double *Matrix, double *EigValues, bool UpperTriangle,
            bool CalcVec, bool RowMajor, bool UseStack)
{
  int lwork = 3 * Size - 1;
  double *work = UseStack ? (double*)alloca(lwork * sizeof(double)) :
                            (double*)malloc(lwork * sizeof(double));
  int ok;
  /* to call a Fortran routine from C we have to transpose the matrix since
  matrix is symmetric we just change the sense of UpperTriangle to opposite */
  if(RowMajor)
    UpperTriangle = !UpperTriangle;
  dsyev_(CalcVec ? 'V' : 'N', UpperTriangle ? 'U' : 'L', Size, Matrix,
         Size, EigValues, work, lwork, ok);

  if(!UseStack)
    free(work);

  if(CalcVec && RowMajor) //need to transpose result
  {
    const unsigned int matSize = Size * Size * sizeof(double);
    double *a = UseStack ? (double*)alloca(matSize) : (double*)malloc(matSize);
    memcpy(a, Matrix, matSize);
    double *sPtr = a;

    for(int i = 0; i < Size; i++)
    {
      double *dPtr = Matrix + i;
      double *sPtrE = sPtr + Size;
      while(sPtr < sPtrE)
      {
        *dPtr = *sPtr++;
        dPtr += Size;
      }
    }

    if(!UseStack)
      free(a);
  }
  return ok;
}


