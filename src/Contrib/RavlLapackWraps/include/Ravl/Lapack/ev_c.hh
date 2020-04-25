// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2007, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
// $Id$
//! lib=RavlLapackWraps

//This file contains C wrappers of LAPACK function for eigen vectors computation
#ifndef EV_C_H
#define EV_C_H

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
int ssyev_c(int Size, float *Matrix, float *EigValues, bool UpperTriangle,
            bool CalcVec, bool RowMajor, bool UseStack);

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
            bool CalcVec, bool RowMajor, bool UseStack);


#endif
