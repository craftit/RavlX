// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2007-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! author="Rachel Gartshore"
//! lib=RavlKalmanFilter

// MotionModel.cc

#include "Ravl/KalmanNullMeasurementModel.hh"

namespace RavlN {
  
  //: Apply Motion Model function to 'data' == predict the next state
  
  VectorC KalmanNullMeasurementModelBodyC::Apply( const VectorC &data ) const
  {
    // z(k+1) = h[k+1,x(k+1)]+w(k+1)
    
    // the state vector x(k+1) is the same as the measurement vector z(k+1)
    return data;
  }
  
  //: Calculate Jacobian matrix of model at X
  //
  // F(k) = df(k)|
  //        -----|
  //         d(x)|
  //             |x=X
  MatrixC KalmanNullMeasurementModelBodyC::Jacobian(const VectorC &X) const
  {
    return MatrixC( 1, 0, 0, 0, 1, 0, 0, 0, 1 );
  }
  
}
