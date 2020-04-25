// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2007-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! author="Rachel Gartshore"
// KalmanFilter.cc
//! lib=RavlKalmanFilter

#include "Ravl/LinearKalmanFilter.hh"

namespace RavlN {
  
  //: Setup kalman filter.
  // Matrices a,b form the system model and h is the measurment model.
  
  LinearKalmanFilterBodyC::LinearKalmanFilterBodyC(const MatrixC &a, const MatrixC &b ,const MatrixC &h) {
    SetSystemModel(a,b);
    SetMeasurementModel(h);
  }
  
  //: Set the System Model
  
  bool LinearKalmanFilterBodyC::SetSystemModel(const MatrixC &a,const MatrixC &b )
  {
    A = a;
    B = b;
    return true;
  }

  //: Set the Measurement Model
  bool LinearKalmanFilterBodyC::SetMeasurementModel(const MatrixC &h )
  {
    H = h;
    return true;
  }
  
  // Evaluate the EKF for the Time Update (predict) stage
  
  VectorMatrixC  LinearKalmanFilterBodyC::TimeUpdate( const VectorMatrixC &Xk, const VectorC &Uk, const MatrixC &Qk ) const
  {
    // State Prediction - FIXME - Need to add optional B*uk
    
    VectorC Xkplus1 = A * Xk.Vector();
    // xbar(k+1|k) = f[k,xbar(k|k),u(k)]

    // State Prediction Covariance
    // P(k+1|k)=F(k)P(k|k)F(k)' + Q(k)
    MatrixC Pkplus1 = A * Xk.Matrix() * A.T() + Qk;

    return VectorMatrixC( Xkplus1, Pkplus1 );
  }
  
  // Evaluate the EKF for the Measurement Update (correct) stage
  
  VectorMatrixC LinearKalmanFilterBodyC::MeasurementUpdate( const VectorMatrixC &Xbar, 
                                                            const VectorC &Zk,
                                                            const MatrixC &R ) const 
  {
    // Measurement Prediction Covariance
    // S(k+1) = H(k+1) * P(k+1|k) * H(k+1)' + R(k+1)
    MatrixC Sk = H * Xbar.Matrix() * H.T() + R;

    // Filter Gain
    // W(k+1) = P(k+1|k) * H(k+1)' * S(k+1)'
    MatrixC Wk = Xbar.Matrix() * H.T() * Sk.Inverse();

    // Updated State Covariance
    // P(k+1|k+1) = P(k+1|k) - W(k+1)*S(k+1)*W(k+1)'
    MatrixC Pk = Xbar.Matrix() - Wk * Sk * Xbar.Matrix().Inverse();

    // Measurement Prediction
    // zbar(k+1|k) = h[k+1,xbar(k+1|k)]
    VectorC zbar = H * Xbar.Vector();

    // Measurement Residual
    // v(k+1) = z(k+1) - zbar(k+1|k)
    VectorC Vk = Zk - zbar;

    // Updated State Estimate
    // xbar(k+1|k+1) = xbar(k+1|k) + W(k+1)*v(k+1)
    VectorC Xk = Xbar.Vector() + Wk * Vk;
    
    return VectorMatrixC( Xk, Pk );
  }
  
  
}
