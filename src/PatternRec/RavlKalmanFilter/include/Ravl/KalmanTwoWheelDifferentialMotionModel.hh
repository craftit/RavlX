// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2007-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_TWOWHEELDIFFERENTIALMOTIONMODEL__HH
#define RAVL_TWOWHEELDIFFERENTIALMOTIONMODEL__HH
//! author="Rachel Gartshore"
//! lib=RavlKalmanFilter

#include "Ravl/PatternRec/Function.hh"

namespace RavlN {
  
  //: This is the Motion Model for the mobile robot Holly
  // Motion Model
  // Using odometry measurements to tell us how the robot has moved
  // Will be given the new odometry reading (which includes the camera pan)
  // This is used as an example for ExtentedKalmanFilterC.
  
  class KalmanTwoWheelDifferentialMotionModelBodyC
    : public FunctionBodyC
  {
  public:
    KalmanTwoWheelDifferentialMotionModelBodyC()
    { 
      outputSize=3; inputSize=3; 
      //cerr << "Default MotionModelBodyC constructor\n"; 
    }
    //: Default Constructor
    
    KalmanTwoWheelDifferentialMotionModelBodyC( UIntT insize, UIntT outsize )
      : FunctionBodyC(insize,outsize)
    { 
      //cerr << "insize,outsize constructor\n"; 
    }
    //: Constructor to set input and output size vectors
    // input vector is uK
    // output vector is xk and xkplus1
    
    virtual VectorC Apply( const VectorC &state, const VectorC &control_input ) const;
    //: Apply Motion Model function to state with control_input 
    // Predict the Next State
    
    virtual MatrixC Jacobian( const VectorC &X ) const;
    //: Calculate Jacobian matrix at X
    
  private:
    // For our motion model:
    // x(k+1) = x(k) + u(k) {+ v(k)}
    // We need these to calculate the jacobian
  };
  
  //: This is the Motion Model for the mobile robot Holly
  // Motion Model
  // Using odometry measurements to tell us how the robot has moved
  // Will be given the new odometry reading (which includes the camera pan)
  // This is used as an example for ExtentedKalmanFilterC.
  
  class KalmanTwoWheelDifferentialMotionModelC
    :public FunctionC
  {
  public:
    // Creates an invalid handle
    KalmanTwoWheelDifferentialMotionModelC()
      : FunctionC( *new KalmanTwoWheelDifferentialMotionModelBodyC() )
    { //cerr <<"default constructor in motionmodelc\n"; 
    }
    //: Default Constructor
    
  protected:
    inline KalmanTwoWheelDifferentialMotionModelC( KalmanTwoWheelDifferentialMotionModelBodyC &bod ) 
      : FunctionC(bod)
    {}
    //: Body constructor

    inline KalmanTwoWheelDifferentialMotionModelC( KalmanTwoWheelDifferentialMotionModelBodyC *bod ) 
      : FunctionC(bod)
    {}
    //: Body ptr constructor
    
    inline KalmanTwoWheelDifferentialMotionModelBodyC &Body()
    { return dynamic_cast<KalmanTwoWheelDifferentialMotionModelBodyC &>(FunctionC::Body()); }
    //: Access body
    
    inline const KalmanTwoWheelDifferentialMotionModelBodyC& Body() const
    { return dynamic_cast<const KalmanTwoWheelDifferentialMotionModelBodyC &>(FunctionC::Body()); }
    //: Access body (for constant handle)
		  
  public:
    
    inline VectorC Apply( const VectorC &state, const VectorC &control_input ) const
    { return Body().Apply(state,control_input); }
    //: Apply Motion Model function to state with control_input 
    // Predict the Next State
    
    inline MatrixC Jacobian( const VectorC &X ) const
    { return Body().Jacobian(X); }
    //: Calculate Jacobian matrix at X

  };
  
#if 0
  inline istream &operator>>(istream &strm,MotionModelC &obj)
  {
    obj = MotionModelC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline ostream &operator<<(ostream &out,const MotionModelC &obj)
  {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
#endif
}  
#endif
