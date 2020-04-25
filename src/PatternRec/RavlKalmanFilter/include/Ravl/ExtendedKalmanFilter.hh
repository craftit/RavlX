// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2007-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_EXTENDEDKALMANFILTER_HH
#define RAVL_EXTENDEDKALMANFILTER_HH
//! author="Rachel Gartshore"
//! lib=RavlKalmanFilter

#include "Ravl/KalmanFilter.hh"

namespace RavlN {
  
  //!userlevel=Develop
  //: Extended Kalman Filter Class
  
  class ExtendedKalmanFilterBodyC
    : public KalmanFilterBodyC
  {
  public:
    ExtendedKalmanFilterBodyC()
    {}
    //: Default Constructor
    
    ExtendedKalmanFilterBodyC(const FunctionC &s_model ,const FunctionC &m_model);
    //: Constructor for new object
    
    virtual bool SetSystemModel(const FunctionC &s_model );
    //: Attempt to set the system model
    
    virtual bool SetMeasurementModel(const FunctionC &m_model );
    //: Attempt to set the measurement model
    
    virtual VectorMatrixC TimeUpdate( const VectorMatrixC &Xk, const VectorC &Uk, const MatrixC &Qk  ) const;
    //: Evaluate the EKF for the Time-Update stage
    // Given the system model f with the current state and covariance Xk and error Qk,
    // calculate the Predicted State and Covariance
    // Also known as the 'Predict Stage'

    virtual VectorMatrixC MeasurementUpdate( const VectorMatrixC &Xbar, const VectorC &Zk, const MatrixC &R ) const;
    //: Evaluate EKF for the Measurement-Update stage
    // Given the measurement model h, the new measurement Zk,
    // the error Qk and the Predicted State & Covariance Xbar (calculated by TimeUpdate)
    // calculate the Updated State and Covariance
    // Also known as the 'Update Stage'
    
    virtual VectorMatrixC Update( const VectorMatrixC &Xk, const VectorC &Uk, const MatrixC &Qk,
				  const VectorC &Zk, const MatrixC &R ) const;
    //: Evaluate the EKF for the Predict and Update Stage
    // Given a system model, with measurement model h. For the previous state Xk with error Qk,
    // along with the measurement Zk with error R, calculate the updated state
    
  protected:
    FunctionC	f;	// Non-linear State Space Model
    FunctionC	h;	// Non-linear Measurement Model
  };


  //!userlevel=Normal
  //: Extended Kalman Filter Class
  
  class ExtendedKalmanFilterC
    : public KalmanFilterC
  {
  public:
    ExtendedKalmanFilterC()
    {}
    //: Default Constructor
    // Creates an invalid handle
    
    ExtendedKalmanFilterC(const FunctionC &s_model ,const FunctionC &m_model )
      : KalmanFilterC( *new ExtendedKalmanFilterBodyC(s_model,m_model) )
    {}
    //: Create a function with the given number of inputs and outputs.
    
  protected:
    ExtendedKalmanFilterC( ExtendedKalmanFilterBodyC &bod )
      : KalmanFilterC(bod)
    {}
    //: Body ptr constructor

    inline ExtendedKalmanFilterBodyC & Body()
    { return static_cast<ExtendedKalmanFilterBodyC &>(RCHandleC<KalmanFilterBodyC>::Body()); }
    //: Access body

    inline const ExtendedKalmanFilterBodyC & Body() const
    { return static_cast<const ExtendedKalmanFilterBodyC &>(RCHandleC<KalmanFilterBodyC>::Body()); }
    //: Access body (for constant handle
    
  public:

    bool SetSystemModel(const FunctionC &s_model )
    { return Body().SetSystemModel( s_model ); }
    //: Attempt to set the System Model
    
    bool SetMeasurementModel(const FunctionC &m_model )
    { return Body().SetMeasurementModel( m_model ); }
    //: Attempt to set the Measurement Model


    VectorMatrixC TimeUpdate( const VectorMatrixC &Xk,
                              const VectorC &Uk, const MatrixC &Qk  ) const
    { return Body().TimeUpdate( Xk, Uk, Qk ); }
    //: Evaluate the EKF for the Time-Update stage
    // Given the system model f with the current state and covariance Xk and error Qk,
    // calculate the Predicted State and Covariance
    // Also known as the 'Predict Stage'

    VectorMatrixC MeasurementUpdate( const VectorMatrixC &Xbar,
                                     const VectorC &Zk, const MatrixC &R ) const
    { return Body().MeasurementUpdate( Xbar, Zk, R ); }
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
