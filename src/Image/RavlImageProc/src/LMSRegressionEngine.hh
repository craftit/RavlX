// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_LMSREGRESSIONENGINE_HEADER
#define RAVLIMAGE_LMSREGRESSIONENGINE_HEADER 1
//! author="Bill Christmas"
//! date="07/12/1999"
//! rcsid="$Id$"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Motion/LMSGradient/LMSRegressionEngine.hh"

#include "Ravl/Image/Image.hh"
#include "Ravl/Vector2d.hh"
#include "Ravl/Matrix2d.hh"
#include "Ravl/Pair.hh"

namespace RavlImageN {

  //! docentry="Ravl.Images.Motion"
  
  inline
  Vector2dC LMSRegressionEngine(const Matrix2dC &A, 
				const Vector2dC &b, 
				RealT dt_sq, // inputs
				RealT noise, 
				IntT N, // inputs
				Vector2dC &lambda, 
				RealT &sig_sq, 
				Matrix2dC &cov)  // outputs
  {
    // calculate Z as inverted averaged "A" matrix    
    Matrix2dC E;
    EigenVectors(A,E,lambda);
    const RealT sqr_n_noise = Sqr(N*noise);
    RealT l_inv0 = lambda[0] / (Sqr(lambda[0]) + sqr_n_noise); // (pseudo) inverse
    RealT l_inv1 = lambda[1] / (Sqr(lambda[1]) + sqr_n_noise); // of eigenvalues
    //Matrix2dC A_inv = E * Matrix2dC(l_inv0, 0.0, 0.0, l_inv1).MulT(E);
    // We now assign A_inv directly to cov to avoid creating another variable.
    
    Matrix2dC mtmp(l_inv0 * E[0][0] ,l_inv1 * E[0][1],
		   l_inv0 * E[1][0] ,l_inv1 * E[1][1]);
    //MulT<RealT,2,2,2>(mtmp,E,cov);
    MulT(mtmp,E,cov);
    
    // hence compute motion vector
    // Vector2dC v (cov * b);
    Vector2dC v;
    Mul(cov,b,v);
    
    // and statistics
    sig_sq = (dt_sq + v.Dot(b)) / (N-2);
    if (sig_sq < 0.0)  
      sig_sq = 0.0;
    
    cov *= sig_sq;
    //cov *= (sig_sq + 0.1);  //: Frank's Solution
    return v;
  }
  //! userlevel=Advanced
  //: Computes the LMS fit from the various sums of squares
  
}

#endif
