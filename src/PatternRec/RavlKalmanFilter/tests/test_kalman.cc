// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2007-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! author="Rachel Gartshore"
//! lib=RavlKalmanFilter

// test_kalman.cc
//
// Simple program to test the kalman filter with our basic motion and measurement model
//
// Requires the user to input the odometry readings, and the measurement readings on demand
// odom_x1 odom_y1 odom_alpha1
// meas_x1 meas_y1 meas_x1
// odom_x2 odom_y2 odom_alpha2
// meas_x2 meas_y2 meas_x2
//
// Covariances are fixed within the program.... for simplicity
// If there is no measurement reading - enter 0 0 0

#include "Ravl/VectorMatrix.hh"
#include "Ravl/DList.hh"
#include "Ravl/DLIter.hh"

#include "Ravl/ExtendedKalmanFilter.hh"
#include "Ravl/KalmanTwoWheelDifferentialMotionModel.hh"
#include "Ravl/KalmanNullMeasurementModel.hh"

using namespace RavlN;

#include <iostream>

int main()
{
  DListC <VectorC> control_inputs;
  control_inputs += VectorC(1.0, 0.5); // k=1
  control_inputs += VectorC(3.0, 1.2); // k=2
  control_inputs += VectorC(1.0, 0.0); // k=3
  control_inputs += VectorC(3.0, 1.0); // k=4
  control_inputs += VectorC(2.0,-0.5); // k=5
  control_inputs += VectorC(2.0,-1.0); // k=6
  control_inputs += VectorC(1.0,-0.5); // k=7
  control_inputs += VectorC(3.0, 0.0); // k=8
  control_inputs += VectorC(2.0, 0.8); // k=9

  DListC <VectorC> measurements; // these were taken by Alberto as the controlled input + some random noise terms
  //measurements += VectorC( 0.00, 0.00, 0.52); // k=1
  measurements += VectorC( 0.87, 0.49, 0.6); // k=2
  measurements += VectorC( 2.02, 3.30, 1.1); // k=3
  measurements += VectorC( 3.06, 3.36, 0.4); // k=4
  measurements += VectorC( 4.75, 5.95, 1.2); // k=5
  measurements += VectorC( 6.51, 5.04,-0.8); // k=6
  measurements += VectorC( 7.64, 3.39,-1.05); // k=7
  measurements += VectorC( 8.53, 2.98,-0.4); // k=8
  measurements += VectorC(11.60, 3.05, 0.1); // k=9
  measurements += VectorC(12.90, 4.55, 0.7); // k=10

  DListC <MatrixC> measurement_covariances;
  measurement_covariances += MatrixC(.1,.0,.0, .0,.1,.0, .0,.0,.09); // k=1
  measurement_covariances += MatrixC(.5,.3,.0, .3,.5,.0, .0,.0,.30); // k=2
  measurement_covariances += MatrixC(.3,.2,.1, .2,.5,.2, .1,.2,.20); // k=3
  measurement_covariances += MatrixC(.2,.2,.2, .2,.2,.2, .2,.2,.09); // k=4
  measurement_covariances += MatrixC(.3,.2,.0, .2,.4,.1, .0,.1,.04); // k=5
  measurement_covariances += MatrixC(.5,.2,.0, .2,.1,.1, .0,.1,.00); // k=6
  measurement_covariances += MatrixC(.2,.0,.0, .0,.3,.1, .0,.1,.14); // k=7
  measurement_covariances += MatrixC(.3,.2,.0, .2,.3,.0, .0,.0,.04); // k=8
  measurement_covariances += MatrixC(.2,.2,.0, .2,.1,.0, .0,.1,.25); // k=9

  // Temporary storage! - just so I can read things properly!
  VectorC control_input(2);
  VectorC pose_meas(3);

  // Current State of the System - x(k), P(k)
  VectorMatrixC state(3);
  state.SetZero();

  // Set up the motion model - for the odometry measurement
  KalmanTwoWheelDifferentialMotionModelC odometry;
  
  // Set up the measurement model - for the vision measurement
  KalmanNullMeasurementModelC vision;
  
  // Set up the EKF with the two models
  ExtendedKalmanFilterC ekf(odometry,vision); // not that the '3' does anything - ?FIXME?
  
  // Set odometry error
  MatrixC Q(0.4,0.0,0.0, 0.0,0.4,0.0, 0.0,0.0,0.15 );
  MatrixC R(3,3); // initialise measurements error matrix

  DLIterC <VectorC> it_control(control_inputs);
  DLIterC <VectorC> it_measurements(measurements);
  DLIterC <MatrixC> it_measurement_covs(measurement_covariances);

  IntT count = 0;
  for( ; it_control.IsElm(), it_measurements.IsElm(), it_measurement_covs.IsElm();
		  it_control++, it_measurements++, it_measurement_covs++ )
  {
    //cerr << "The current state of the system: " << state << endl;
    cerr << "Iteration: " << count << endl;

    // Get odometry data (movement order given to the robot [r,theta])
    control_input = *it_control;
    //cerr << "Control Input from odometry: [" << control_input[0] << "," << control_input[1] << "]" << endl;
    //cerr << "With error: " << Q << endl;

    // Do the prediction stage of the EKF using this odometry data
    state = ekf.TimeUpdate( state, control_input, Q );

    cerr << "Predicted Position is: " << state << endl;

    // Get the measurement data
    pose_meas = *it_measurements;
    R = *it_measurement_covs;
    cerr << "Measurement data: [" << pose_meas[0] << "," << pose_meas[1] << "," << pose_meas[2] << "]" << endl;
    //cerr << "With error: " << R << endl;

    // Do the correction stage of the EKF using this vision data
    state = ekf.MeasurementUpdate( state, pose_meas, R );

    cerr << "Corrected Position is: " << state << endl;
    count++;
    cerr << "--------------------------------------------------\n";
#if 0
    char ch;
    cerr << "type a letter to continue:"; cin >> ch; cerr << endl;
#endif
  }
  
  return 0;
}
