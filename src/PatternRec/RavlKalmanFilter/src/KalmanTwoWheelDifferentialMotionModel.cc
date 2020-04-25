// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2007-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! author="Rachel Gartshore"
//! lib=RavlKalmanFilter

#include "Ravl/KalmanTwoWheelDifferentialMotionModel.hh"

namespace RavlN {
  
  //: Apply Motion Model function to 'data' == predict the next state
  
  VectorC KalmanTwoWheelDifferentialMotionModelBodyC::Apply( const VectorC &state, const VectorC &control_input ) const
  {
    // The control input u(k) is
    // 		d (Distance travelled, in mm)
    // 		theta (Robot rotation, in degrees)

    // The state x(k) is
    // 		x(k)
    // 		y(k)
    // 		theta(k)

    //cerr << "Applying motion model to: " << state << " with " << control_input << endl;
    // x(k+1) = Ax(k) + Bu(k) + v(k) - if it was a linear model! - we have f[x(k),u(k),k]
    RealT angle = Radians(control_input[1]);
    RealT delta_x = control_input[0] * cos(angle);
    RealT delta_y = control_input[0] * sin(angle);
    RealT delta_th = control_input[1] - state[2];
    VectorC delta(delta_x,delta_y,delta_th);
    //cerr << "The change in position components is: " << delta << endl;
    return state + delta;
    
    /*
    // Set the previously found position to the now old position (!)
    position_k = position_kplus1;

    // Assuming that we have been given new odometry information previously in position
    // x(k+1|k) = f[k,x(k|k),u(k)]

    // Actually we are being given the new odometry information
    // therefore this is the predicted state
    // ( if this was a proper model we would predict next position based on velocity
    // or the line model - which we'll add in at a later stage )
    // I'm going to assume (that I know what I'm doing) and therefore
    // data = delta_odometry
    // we know position_k, therefore position_kplus1...
    // x(k+1) = x(k) + u(k) + v(k)
    delta_position = data;				// 1. Input Driving Force
    position_kplus1 = position_k + delta_position; 	// 2. State Prediction
    return position_kplus1;
    */
  }
  
  //: Calculate Jacobian matrix of model at X
  //
  // F(k) = df(k)|
  //        -----|
  //         d(x)|
  //             |x=X
  MatrixC KalmanTwoWheelDifferentialMotionModelBodyC::Jacobian(const VectorC &X) const
  {
    // Our model is:
    // x(k+1) = x(k) + u(k) + v(k)
    // Estimates the error covariance matrix for the prediciton model
    return MatrixC( 1, 0, 0, 0, 1, 0, 0, 0, 0 );
    
    //for(UIntT i = 0;i < X.Size();i++)
    //ret.SetColumn(i,a*MakeJacobianInput(X,i));
  }


}
