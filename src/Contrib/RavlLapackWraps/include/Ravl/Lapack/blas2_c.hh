// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2007, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
// $Id$
#ifndef BLAS2_HH
#define BLAS2_HH
//! lib=RavlLapackWraps


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
            const int &IncX, bool UpperTriangle, bool RowMajor);

#endif
