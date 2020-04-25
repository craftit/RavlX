// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlMath
//! file="Ravl/Math/LinearAlgebra/General/MatrixDeterminant.cc"

#include "Ravl/MatrixDecomposition.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  //: Compute determinant of a n by n matrix.
  //:-
  
  RealT Determinant(const RealT *data,UIntT n) 
  { return MatrixC(n,n,const_cast<RealT*>(data)).Det(); }
  
  
  RealT MatrixC::Det() const {
    RavlAssertMsg(Rows() == Cols(),"Can only find the determinant of square matrices.\n");
    switch(static_cast<size_t>(Rows())) {
    case 0: return 0;
    case 1: return (*this)[0][0];
    case 2: return (*this)[0][0] * (*this)[1][1] - (*this)[1][0] * (*this)[0][1];
    case 3: return (*this)[0][0] * ((*this)[1][1]*(*this)[2][2] - (*this)[1][2]*(*this)[2][1])
	      + (*this)[1][0] * ((*this)[2][1]*(*this)[0][2] - (*this)[2][2]*(*this)[0][1])
	      + (*this)[2][0] * ((*this)[0][1]*(*this)[1][2] - (*this)[0][2]*(*this)[1][1]);
    }
    RealT det;
    MatrixC mc = Copy();
    LUDecomposition(mc,det);
    return det;
  }
  
}
