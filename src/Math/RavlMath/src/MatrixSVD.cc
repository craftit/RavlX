// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/LinearAlgebra/General/MatrixSVD.cc"

#include "Ravl/Matrix.hh"
#include "Ravl/Vector.hh"
#include "Ravl/StdMath.hh"
#include "Ravl/Exception.hh"
#include "Ravl/SVD.hh"
#include "Ravl/LAHooks.hh"

#define RAVL_USE_CCMATH_SVD 0

#if RAVL_USE_CCMATH_SVD
#include "Ravl/CCMath.hh"
#endif

// General matrix functions.

namespace RavlN {
  // n*m row*col
  
  //: Singular value decomposition, eg. M = U * D * V.T(). 
  // The diagonal matrix D is returned as a vector. Values for the
  // other matrixes are not computed.
  // If the operation fails the returned vector is invalid.
  
  VectorC SVD(const MatrixC &mat) {
    MatrixC ret = mat.Copy();
    return SVD_IP(ret);
  }
  
  //: Singular value decomposition, eg. M = U * D * V.T(). 
  // The diagonal matrix D is returned as a vector. Values for the
  // other matrixes are not computed.
  // If the operation failes the returned vector is invalid. <p>
  // NB. This function destory's the contents of this matrix!
  
#if RAVL_USE_CCMATH_SVD
  
  VectorC SVD_IP(MatrixC &mat) {
    RavlAlwaysAssert(mat.IsContinuous()); // Should we cope with this silently ?
    if(mat.Rows() == 0)
      return VectorC(0);
    VectorC ret(mat.Cols());
    if(mat.Rows() > mat.Cols() * 2) { // Pick the best routine.
      if(sv2val(&ret[0],&mat[0][0],mat.Rows(),mat.Cols()) != 0)
	return VectorC();
    } else {
      if(svdval(&ret[0],&mat[0][0],mat.Rows(),mat.Cols()) != 0)
	return VectorC();
    }
    return ret;
  }
  
#else
  VectorC SVD_IP(MatrixC &mat) {
   // This hook function will run standard SVD routine
   // However, if you link in the Intel MKL library then
   // it will use the parallel Intel MKL libraries - which
   // are super, super fast.
   VectorC s;
   MatrixC u, v;
   SVD_IP_hook(mat, u, s, v);
   return s;
  }
#endif
  
  //: Singular value decomposition, eg. M = U * D * V.T(). 
  // The diagonal matrix D is returned as a vector.
  // This also returns the matrix u and v matrixes, the passed
  // matrixes will be used to store the results if they are
  // of the correct size.
  // If the operation failes the returned vector is invalid.
  
  VectorC SVD(const MatrixC &mat,MatrixC & u, MatrixC & v) {

    MatrixC ret = mat.Copy();
    return SVD_IP(ret,u,v);
  }
  
  //: Singular value decomposition, eg. M = U * D * V.T(). 
  // The diagonal matrix D is returned as a vector.
  // This also returns the matrix u and v matrixes, the passed
  // matrixes will be used to store the results if they are
  // of the correct size.
  // If the operation fails the returned vector is invalid.
  // NB. This function destroys the contents of this matrix!
#if RAVL_USE_CCMATH_SVD
  
  VectorC SVD_IP(MatrixC &mat,MatrixC & u, MatrixC & v) {
    RavlAlwaysAssert(mat.IsContinuous()); // Should we cope with this silently ?
    // Check for trivial cases.
    if(mat.Rows() == 0)
      return VectorC(0);
    // Make sure the output matrixes are right.
    if(!u.IsContinuous() || mat.Rows() != u.Rows() || mat.Rows() != u.Cols()) 
      u = MatrixC(mat.Rows(),mat.Rows());
    if(!v.IsContinuous() || mat.Cols() != v.Rows() || mat.Cols() != v.Cols()) 
      v = MatrixC(mat.Cols(),mat.Cols());
    VectorC ret(mat.Cols());
    if(mat.Rows() > mat.Cols() * 2) { // Pick the best routine.
      if(sv2uv(&ret[0],&mat[0][0],&u[0][0],mat.Rows(),&v[0][0],mat.Cols()) != 0) {
	cerr << "SVD_IP(), ERROR: mat.Rows() < mat.Cols(). Returning an empty vector. \n";
	return VectorC();
      }
    } else {
      if(svduv(&ret[0],&mat[0][0],&u[0][0],mat.Rows(),&v[0][0],mat.Cols()) != 0) {
	cerr << "SVD_IP(), ERROR: mat.Rows() < mat.Cols(). Returning an empty vector. \n";
	return VectorC();
      }
    }
    return ret;
  }
#else
  VectorC SVD_IP(MatrixC &mat,MatrixC & u, MatrixC & v) {
#if RAVL_SVDC_M_LT_N_WORKAROUND    
    if(mat.Rows() < mat.Cols()) {
      // Hack to get around bug in SVD.
      MatrixC matt = mat.T();
      MatrixC ut;
      MatrixC vt;
      VectorC x = SVD_IP(matt,ut,vt);
      v = ut.T();
      u = vt.T();
      return x;
    }
#endif
#if 0
    SVDC<RealT> svd(mat);
    v = svd.GetV();
    u = svd.GetU();
    return svd.SingularValues();
#endif
    // This hook function will run standard SVD routine
    // However, if you link in the Intel MKL library then
    // it will use the parallel Intel MKL libraries - which
    // are super, super fast.
    VectorC s;
    SVD_IP_hook(mat, u, s, v);
    return s;
  }  
#endif
}
