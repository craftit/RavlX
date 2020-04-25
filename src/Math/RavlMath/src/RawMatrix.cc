// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/LinearAlgebra/General/RawMatrix.cc"

#include "Ravl/RawMatrix.hh"
#include "Ravl/TMatrix.hh"

#define RAVL_USE_CCMATH_SVD 0

#if RAVL_USE_CCMATH_SVD
#include "Ravl/CCMath.hh"
#else
#include "Ravl/SVD.hh"
#endif

namespace RavlN {
  
#if RAVL_USE_CCMATH_SVD
  //: Compute matrix D from SVD.
  
  bool RawSVD(RealT *vec,RealT *mat,UIntT N,UIntT M) {
    if(N > M * 2) // Pick the best routine.
      return sv2val(&vec[0],&mat[0][0],N,M) == 0;
    return svdval(&vec[0],&mat[0][0],N,M) == 0;
  }
  
  //: Compute full SVD.
  
  bool RawSVD(RealT *vec,RealT *mat,RealT *u,UIntT N,RealT *v,UIntT M) {
    if(N > M * 2) // Pick the best routine.
      return sv2uv(vec,mat,u,N,v,M) == 0;
    return svduv(vec,mat,u,N,v,M) == 0;
  }
#else
  //: Compute matrix D from SVD.
  
  bool RawSVD(RealT *vec,RealT *rmat,UIntT N,UIntT M) {
    TMatrixC<RealT> mat(N,M,rmat,false,false);
    SVDC<RealT> svd(mat);
    
    // Copy singular values.
    RealT *at1 = vec;
    RealT *at2 = &(svd.SingularValues()[0]);
    RealT *end = &vec[N];
    for(;at1 != end;at1++,at2++)
      *at1 = *at2;
    
    return false;
  }
  
  //: Compute full SVD.
  
  bool RawSVD(RealT *rvec,RealT *rmat,RealT *ru,UIntT N,RealT *rv,UIntT M) {
    TMatrixC<RealT> mat(N,M,rmat,false,false);
    TVectorC<RealT> vec(N,rvec,false,false);
    vec.Fill(0);
    TMatrixC<RealT> nU(M,N,ru,false,false);
    TMatrixC<RealT> nV(N,N,rv,false,false);
    SVDC<RealT> svd(mat,vec,nU,nV);
    
    return true;
  }
  
#endif

}

