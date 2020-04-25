// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/LinearAlgebra/General/MatrixRandom.cc"

#include "Ravl/Matrix.hh"
#include "Ravl/Random.hh"
#include "Ravl/MatrixRUT.hh"
#include "Ravl/Vector.hh"

namespace RavlN {
  
  //: Create a random matrix of values between -scale and scale with the given size.
  
  MatrixC RandomMatrix(int n,int m,RealT scale) {
    MatrixC x(n,m);
    for(SArray2dIterC<RealT> it(x);it;it++)
      *it = (Random1() -0.5) * 2 * scale;
    return x;
  }
  
  //: Create a random symmetric matrix of values between -scale and scale with the given size.
  
  MatrixC RandomSymmetricMatrix(int n,RealT scale) {
    MatrixRUTC x(n);
    for(SArray2dIterC<RealT> it(x);it;it++)
      *it = (Random1() - 0.5) * 2 * scale;
    x.MakeSymmetric();
    return x;
  }
  
  //: Create a random positive definite matrix.
  
  MatrixC RandomPositiveDefiniteMatrix(int n) {
    MatrixRUTC x(n);
    VectorC vec(n);
    x.Fill(0);
    int k = Max(n,n) * 2;
    for(int i = 0;i < k;i++) {
      for(SArray1dIterC<RealT> it1(vec);it1;it1++)
	*it1 = (Random1() - 0.5) * 2;
      x.AddOuterProduct(vec);
    }
    x.MakeSymmetric();
    return x;
  }

  //: Create a random vector of values between -scale and scale with the given size.
  
  VectorC RandomVector(int n,RealT scale) {
    VectorC ret(n);
    for(SArray1dIterC<RealT> it(ret);it;it++)
      *it = (Random1() - 0.5) * 2 * scale;
    return ret;
  }

}
