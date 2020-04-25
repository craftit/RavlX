// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2007-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_LINEARKALMANFILTER_HH
#define RAVL_LINEARKALMANFILTER_HH
//! author="Rachel Gartshore"
//! lib=RavlKalmanFilter

#include "Ravl/KalmanFilter.hh"

namespace RavlN {

  //!userlevel=Develop
  //: Linear Kalman Filter Class
  // 
  // WARNING: This class is incomplete, it doesn't correctly integrate control information (Uk)
  //
  
  class LinearKalmanFilterBodyC
    : public KalmanFilterBodyC
  {
  public:
    LinearKalmanFilterBodyC()
    {}
    //: Default Constructor
    
    LinearKalmanFilterBodyC(const MatrixC &a, const MatrixC &b ,const MatrixC &h);
    //: Setup kalman filter.
    // Matrices a,b form the system model and h is the measurment model.
    
    virtual VectorMatrixC TimeUpdate( const VectorMatrixC &Xk, const VectorC &Uk, const MatrixC &Qk  ) const;
    //: Evaluate the EKF for the Time-Update stage
    // Given the system model f with the current state and covariance Xk, control input Uk, and error Qk,
    // calculate the Predicted State and Covariance
    // Also known as the 'Predict Stage'
    
    virtual VectorMatrixC MeasurementUpdate( const VectorMatrixC &Xbar,
                                             const VectorC &Zk,
                                             const MatrixC &R ) const;
    //: Evaluate EKF for the Measurement-Update stage
    // Given the measurement model h, the new measurement Zk,
    // the error Qk and the Predicted State & Covariance Xbar (calculated by TimeUpdate)
    // calculate the Updated State and Covariance
    // Also known as the 'Update Stage'
    // R is the measurement noise covariance
    
    virtual bool SetSystemModel(const MatrixC &a, const MatrixC &b );
    //: Set the system model
    
    virtual bool SetMeasurementModel(const MatrixC &h );
    //: Set the measurment model.
    
  protected:
    MatrixC A;	// State Transition Matrix
    MatrixC B;	// Optional Control to State Relation
    MatrixC H;	// State to Measurement Relation
  };

  //!userlevel=Normal
  //: Linear Kalman Filter Class
  // 
  // WARNING: This class is incomplete, it doesn't correctly integrate control information (Uk)
  //
  
  class LinearKalmanFilterC
    : public KalmanFilterC
  {
  public:
    LinearKalmanFilterC()
    {}
    //: Default Constructor
    // Creates an invalid handle
    
    LinearKalmanFilterC(const MatrixC &a, const MatrixC &b ,const MatrixC &h)
      : KalmanFilterC( *new LinearKalmanFilterBodyC(a,b,h) )
    {}
    
  protected:
    LinearKalmanFilterC( LinearKalmanFilterBodyC &bod )
      : KalmanFilterC(bod)
    {}
    //: Body ptr constructor

    inline LinearKalmanFilterBodyC & Body()
    { return static_cast<LinearKalmanFilterBodyC &>(RCHandleC<KalmanFilterBodyC>::Body()); }
    //: Access body
    
    inline const LinearKalmanFilterBodyC & Body() const
    { return static_cast<const LinearKalmanFilterBodyC &>(RCHandleC<KalmanFilterBodyC>::Body()); }
    //: Access body (for constant handle
    
  public:

    bool SetSystemModel(const MatrixC &a,const MatrixC &b )
    { return Body().SetSystemModel( a, b ); }
    //: Attempt to set the System Model

    bool SetMeasurementModel(const MatrixC &h )
    { return Body().SetMeasurementModel( h ); }
    //: Attempt to set the Measurement Model
    
  };

}

#endif
