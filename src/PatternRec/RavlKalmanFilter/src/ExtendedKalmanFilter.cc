// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2007-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! author="Rachel Gartshore"
// Kalman_Filter.cc
//! lib=RavlKalmanFilter

#include "Ravl/ExtendedKalmanFilter.hh"

#define debugEKF 1

namespace RavlN {
  
  ExtendedKalmanFilterBodyC::ExtendedKalmanFilterBodyC(const FunctionC &s_model ,const FunctionC &m_model)
  {
    SetSystemModel(s_model);
    SetMeasurementModel(m_model);
  }
  
  //: Constructor for new object

  // Set the state space model
  bool ExtendedKalmanFilterBodyC::SetSystemModel(const FunctionC &s_model )
  {
    f = s_model;
    return true;
  }

  // Set the measurement model
  bool ExtendedKalmanFilterBodyC::SetMeasurementModel(const FunctionC &H )
  {
    h = H;
    return true;
  }
  
  // Evaluate the EKF for the Time Update (predict) stage
  VectorMatrixC ExtendedKalmanFilterBodyC::TimeUpdate( const VectorMatrixC &Xk,
                                                       const VectorC &Uk, const MatrixC &Qk ) const
  {
#if debugEKF
    cerr << "Current State : " << Xk << endl;
    cerr << "New Control Input : " << Uk << endl;
#endif
    // State Prediction
    // xbar(k+1|k) = f[k,xbar(k|k),u(k)]
    VectorC Xkplus1(3);
#if debugEKF
    cerr <<" xk.vector: " << Xk.Vector() << endl;
    cerr <<" Size of input vector: " << f.InputSize() << endl;
#endif
    Xkplus1 = f.Apply(Xk.Vector(), Uk);
#if debugEKF
    cerr << "State Prediction: " << Xkplus1 << endl;
#endif

    // Evaluate Jacobian of system model
    MatrixC Fk = f.Jacobian(Xk.Vector());
#if debugEKF
    cerr << "Jacobian of system: " << Fk << endl;
#endif

    // State Prediction Covariance
    // P(k+1|k)=F(k)P(k|k)F(k)' + Q(k)
#if debugEKF
    cerr << "xk.matrix(): " << Xk.Matrix() << endl;
#endif
    MatrixC Pkplus1 = Fk * Xk.Matrix() * Fk.T() + Qk;
#if debugEKF
    cerr << "State Prediction Covariance: \n" << Pkplus1 << endl;
#endif

    return VectorMatrixC( Xkplus1, Pkplus1 );
  }

  // Evaluate the EKF for the Measurement Update (correct) stage
  VectorMatrixC ExtendedKalmanFilterBodyC::MeasurementUpdate( const VectorMatrixC &Xbar,
                                                              const VectorC &Zk, const MatrixC &R ) const 
  {
    // Evaluate Measurement Jacobians
    // H(k) = dh(k+1)/dx|x=xbar(k+1|k)
    MatrixC Hk = h.Jacobian(Xbar.Vector());
#if debugEKF
    cerr << "Measurement Jacobian: \n" << Hk << endl;
#endif

    // Measurement Prediction Covariance
    // S(k+1) = H(k+1) * P(k+1|k) * H(k+1)' + R(k+1)
    MatrixC Sk = Hk * Xbar.Matrix() * Hk.T() + R;
#if debugEKF
    cerr << "Measurement Prediction Covariance:\n" << Sk << endl;
#endif

    // Filter Gain
    // W(k+1) = P(k+1|k) * H(k+1)' * S(k+1)'
    MatrixC Wk = Xbar.Matrix();
    Wk *= Hk.T();
    Wk *= Sk.Inverse();
#if debugEKF
    cerr << "Filter Gain: \n" << Wk << endl;
#endif

    // Updated State Covariance
    // P(k+1|k+1) = P(k+1|k) - W(k+1)*S(k+1)*W(k+1)'
    MatrixC Pk = Xbar.Matrix() - Wk * Sk * Xbar.Matrix().T();
#if debugEKF
    cerr << "Updated State Covariance:\n" << Pk << endl;
#endif

    // Measurement Prediction
    // zbar(k+1|k) = h[k+1,xbar(k+1|k)]
    VectorC zbar = h.Apply( Xbar.Vector() );
#if debugEKF
    cerr << "Measurement Prediction: " << zbar << endl;
#endif

    // Measurement Residual
    // v(k+1) = z(k+1) - zbar(k+1|k)
    VectorC Vk = Zk - zbar;
#if debugEKF
    cerr << "Measurement Residual: " << Vk << endl;
#endif

    // Updated State Estimate
    // xbar(k+1|k+1) = xbar(k+1|k) + W(k+1)*v(k+1)
    VectorC Xk = Xbar.Vector() + Wk * Vk;
#if debugEKF
    cerr << "Updated State Estimate: " << Xk << endl;
#endif
    
    return VectorMatrixC( Xk, Pk );
  }

  // FIXME - needs re-writing once both TimeUpdate & MeasurementUpdate have been tested to make it "faster"
  VectorMatrixC ExtendedKalmanFilterBodyC::Update( const VectorMatrixC &Xk, const VectorC &Uk, const MatrixC &Qk,
                                                   const VectorC &Zk, const MatrixC &R ) const
  {
    return MeasurementUpdate( TimeUpdate(Xk,Uk,Qk), Zk, R );
  }


}
