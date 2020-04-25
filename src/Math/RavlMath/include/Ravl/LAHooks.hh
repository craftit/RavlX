// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/*
 * LAHooks.hh
 *
 *  Created on: 2 Mar 2010
 *      Author: Alexey Kostin
 */
//! lib=RavlMath

#ifndef LAHOOKS_HH_
#define LAHOOKS_HH_

namespace RavlN {
  class VectorC;
  class MatrixC;
  class MatrixRUTC;

  extern bool (*g_EigenVectorsSymmetric)(VectorC &resVals, MatrixC &resVecs, const MatrixC &M);
  inline bool EigenVectorsSymmetric(VectorC &resVals, MatrixC &resVecs, const MatrixC &M)
  { return (*g_EigenVectorsSymmetric)(resVals, resVecs, M); }

  extern void (*g_AddOuterProduct)(MatrixRUTC &matr, const VectorC &vec);
  inline void AddOuterProduct(MatrixRUTC &matr, const VectorC &vec)
  { (*g_AddOuterProduct)(matr, vec); }

  extern void (*g_AddOuterProduct1)(MatrixRUTC &matr, const VectorC &vec, double a);
  inline void AddOuterProduct(MatrixRUTC &matr, const VectorC &vec, double a)
  { (*g_AddOuterProduct1)(matr, vec, a); }

  extern void (*g_SubtractOuterProduct)(MatrixRUTC &matr, const VectorC &vec);
  inline void SubtractOuterProduct(MatrixRUTC &matr, const VectorC &vec)
  { (*g_SubtractOuterProduct)(matr, vec); }

  extern void (*g_SubtractOuterProduct1)(MatrixRUTC &matr, const VectorC &vec, double a);
  inline void SubtractOuterProduct(MatrixRUTC &matr, const VectorC &vec, double a)
  { (*g_SubtractOuterProduct1)(matr, vec, a); }

  extern bool (*g_SVD_IP_hook)(MatrixC & m, MatrixC & u, VectorC & s, MatrixC & v);
  inline bool SVD_IP_hook(MatrixC & m, MatrixC & u, VectorC & s, MatrixC & v)
  { return (*g_SVD_IP_hook)(m, u, s, v); }

  extern bool (*g_Inverse_IP_hook)(MatrixC & m, double & det);
  inline bool Inverse_IP_hook(MatrixC & m, double & det) {
    return (*g_Inverse_IP_hook)(m, det);
  }



}

#endif /* LAHOOKS_HH_ */
