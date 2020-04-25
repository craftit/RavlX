// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2007-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! author="Rachel Gartshore"
// KalmanFilter.cc
//! lib=RavlKalmanFilter

#include "Ravl/KalmanFilter.hh"

namespace RavlN {
  
  
  // Evaluate the EKF for the Time Update (predict) stage
  
  VectorMatrixC  KalmanFilterBodyC::TimeUpdate( const VectorMatrixC &Xk, const VectorC &Uk, const MatrixC &Qk ) const
  {
    RavlAssertMsg(0,"abstract method");
    return VectorMatrixC( );
  }
  
  // Evaluate the EKF for the Measurement Update (correct) stage
  
  VectorMatrixC KalmanFilterBodyC::MeasurementUpdate( const VectorMatrixC &Xbar, const VectorC &Zk,
                                                      const MatrixC &R ) const 
  {
    RavlAssertMsg(0,"abstract method");
    return VectorMatrixC( );
  }
  
  VectorMatrixC KalmanFilterBodyC::Update( const VectorMatrixC &Xk, const VectorC &Uk, const MatrixC &Qk,
                                           const VectorC &Zk, const MatrixC &R ) const
  {
    return MeasurementUpdate( TimeUpdate(Xk,Uk,Qk), Zk, R );
  }
  
  
}
