// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/LinearAlgebra/General/MatrixEigen.cc"

#include "Ravl/StdConst.hh"
#include "Ravl/Vector.hh"
#include "Ravl/Matrix.hh"
#include "Ravl/VectorMatrix.hh"
#include "Ravl/Eigen.hh"
#include "Ravl/LAHooks.hh"


// matrix functions base on JAMA method.

namespace RavlN {



  // n*m row*col
  
  VectorC EigenValues(const MatrixC &mat) {
    MatrixC ret = mat.Copy();
    return EigenValuesIP(ret);
  }
  
  VectorC EigenValuesIP(MatrixC &mat) {
    EigenValueC<RealT> ev(mat);
    return ev.EigenValues();
  }
  
  VectorC EigenVectors(const MatrixC &mat,MatrixC &E)
#if 1
  {
    VectorC vals;
    EigenVectorsSymmetric(vals, E, mat);
    return vals;
  }
#else
  {
    EigenValueC<RealT> ev(mat);
    E = ev.EigenVectors();
    return ev.EigenValues();
  }
#endif

  VectorMatrixC EigenVectors(const MatrixC &mat)
#if 1
  {
    VectorC v;
    MatrixC e;
    EigenVectorsSymmetric(v, e, mat);
    return VectorMatrixC(v, e);
  }
#else
  {
    EigenValueC<RealT> ev(mat);
    MatrixC ret = ev.EigenVectors();
    VectorC vec = ev.EigenValues();
    return VectorMatrixC(vec,ret);
  }
#endif
  
  VectorC EigenVectorsIP(MatrixC &mat) {
    EigenValueC<RealT> ev(mat);
    mat = ev.EigenVectors();
    return ev.EigenValues();
  }
  

#if RAVL_COMPILER_GNU
  template<class NumT> class EigenC;
#endif
}
