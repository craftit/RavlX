// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2007, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_TFMATRIXDECOMPOSITION_HEADER
#define RAVL_TFMATRIXDECOMPOSITION_HEADER 1
//////////////////////////////////////////////////
//! rcsid=$Id$
//! file="Ravl/Math/LinearAlgebra/FixedSize/TFMatrixDecomposition.hh"
//! lib=RavlMath
//! userlevel=Develop
//! author="Charles Galambos"
//! date="17/06/2007"
//! docentry="Ravl.API.Math.Geometry"

#include "Ravl/TFMatrix.hh"

namespace RavlN {
  
  template<class DataT,unsigned int N>
  bool CholeskyDecomposition(const TFMatrixC<DataT,N,N> &rs,TFMatrixC<DataT,N,N> &ret) {
    ret.Fill(0);
    for(unsigned i = 0;i < N;i++) {
      SizeBufferAccessC<DataT> rowi = ret[i];
      for(unsigned j = i;j < N;j++) {
	DataT sum = rs[i][j];
	SizeBufferAccessC<DataT> rowj = ret[j];
        
	for(unsigned k = 0; k < i;k++)
	  sum -= rowi[k] * rowj[k];
        
	if(i == j) {
	  //RavlAssertMsg(sum > 0.0,"Matrix is not positive definite. ");
	  if(sum <= 0.0)
	    return false; // Matrix is not positive definite.
	  ret[i][i] = Sqrt(sum);
	} else
	  ret[j][i] = sum/ret[i][i];
      }
    }
    return true;
  }
  //: CholeskyDecomposition of matrix 'rs'.
  // Matrix 'rs' should be symetrix and positive definite.
  
}

#endif


