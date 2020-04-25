// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2007-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_KALMANFILTER_HH
#define RAVL_KALMANFILTER_HH
//! author="Rachel Gartshore"
//! lib=RavlKalmanFilter

#include "Ravl/RefCounter.hh"
#include "Ravl/PatternRec/Function.hh"
#include "Ravl/Stream.hh"

namespace RavlN {

  //!userlevel=Develop
  //: Abstract Kalman Filter Class
  
  class KalmanFilterBodyC
    : public RCBodyVC
  {
  public:
    KalmanFilterBodyC()
    {}
    //: Default Constructor
    
    virtual VectorMatrixC TimeUpdate( const VectorMatrixC &Xk, const VectorC &Uk, const MatrixC &Qk  ) const = 0;
    //: Evaluate the EKF for the Time-Update stage
    // Given the system model f with the current state and covariance Xk, control input Uk, and error Qk,
    // calculate the Predicted State and Covariance
    // Also known as the 'Predict Stage'
    
    virtual VectorMatrixC MeasurementUpdate( const VectorMatrixC &Xbar,
                                             const VectorC &Zk,
                                             const MatrixC &R ) const = 0;
    //: Evaluate EKF for the Measurement-Update stage
    // Given the measurement model h, the new measurement Zk,
    // the error Qk and the Predicted State & Covariance Xbar (calculated by TimeUpdate)
    // calculate the Updated State and Covariance
    // Also known as the 'Update Stage'
    // R is the measurement noise covariance
    
    virtual VectorMatrixC Update( const VectorMatrixC &Xk, const VectorC &Uk, const MatrixC &Qk,
				  const VectorC &Zk, const MatrixC &R ) const;
    //: Evaluate the EKF for the Predict and Update Stage
    // Given a system model, with measurement model h. For the previous state Xk with error Qk,
    // along with the measurement Zk with error R, calculate the updated state
    // Qk is process noise covariance
    
  protected:
  };

  //!userlevel=Normal
  //: Abstract Kalman Filter Class
  
  class KalmanFilterC
    :public RCHandleVC<KalmanFilterBodyC>
  {
  public:
    KalmanFilterC()
    {}
    //: Default Constructor
    // Creates an invalid handle
    
  protected:
    KalmanFilterC( KalmanFilterBodyC &bod )
      : RCHandleVC<KalmanFilterBodyC>(bod)
    {}
    //: Body ref constructor

    KalmanFilterC( KalmanFilterBodyC *bod )
      : RCHandleVC<KalmanFilterBodyC>(bod)
    {}
    //: Body ptr constructor
    
    inline KalmanFilterBodyC & Body()
    { return static_cast<KalmanFilterBodyC &>(RCHandleC<KalmanFilterBodyC>::Body()); }
    //: Access body

    inline const KalmanFilterBodyC & Body() const
    { return static_cast<const KalmanFilterBodyC &>(RCHandleC<KalmanFilterBodyC>::Body()); }
    //: Access body (for constant handle
    
  public:
    
    VectorMatrixC TimeUpdate( const VectorMatrixC &Xk, const VectorC &Uk, const MatrixC &Qk  ) const
    { return Body().TimeUpdate( Xk, Uk, Qk ); }
    //: Evaluate the EKF for the Time-Update stage
    // Given the system model f with the current state and covariance Xk and error Qk,
    // calculate the Predicted State and Covariance
    // Also known as the 'Predict Stage'

    VectorMatrixC MeasurementUpdate( const VectorMatrixC &Xbar, const VectorC &Zk, const MatrixC &R ) const
    { return Body().MeasurementUpdate( Xbar, Zk,  R ); }
    //: Evaluate EKF for the Measurement-Update stage
    // Given the measurement model h, the new measurement Zk,
    // the error Qk and the Predicted State & Covariance Xbar (calculated by TimeUpdate)
    // calculate the Updated State and Covariance
    // Also known as the 'Update Stage'
		  
    VectorMatrixC Update( const VectorMatrixC &Xk, const VectorC &Uk, const MatrixC &Qk,
                          const VectorC &Zk, const MatrixC &R ) const
    { return Body().Update( Xk, Uk, Qk, Zk, R ); }
    //: Evaluate the EKF for the Predict and Update Stage
    // Given a system model, with measurement model h. For the previous state Xk with error Qk,
    // along with the measurement Zk with error R, calculate the updated state
  };

}

#endif
