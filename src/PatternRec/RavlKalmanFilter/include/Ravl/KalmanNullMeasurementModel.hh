// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2007-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_KALMANNULLMEASUREMENT_MODEL__HH
#define RAVL_KALMANNULLMEASUREMENT_MODEL__HH
//! author="Rachel Gartshore"
//! lib=RavlKalmanFilter

#include "Ravl/PatternRec/Function.hh"

namespace RavlN {
  
  //!userlevel=Develop
  //: This is a null measurment model for use with ExtendedKalmanFilters.
  // The jacobian is a identity matrix, and the measurement values 
  // are passed straight through
  
  class KalmanNullMeasurementModelBodyC
    :public FunctionBodyC
  {
  public:
    KalmanNullMeasurementModelBodyC()
    { 
      outputSize=3; inputSize=3; 
      //cerr << "Default in MeasurementModelBodyC constructor\n"; 
    }
    //: Default Constructor
    
    KalmanNullMeasurementModelBodyC( UIntT insize, UIntT outsize )
      : FunctionBodyC(insize,outsize)
    { 
      //cerr << "insize,outsize constructor\n";
    }
    //: Constructor to set input and output size vectors
    // input vector is uK
    // output vector is xk and xkplus1

    virtual VectorC Apply( const VectorC &state ) const;
    //: Apply Motion Model function to state with control_input 
    // Predict the Next State
    
    virtual MatrixC Jacobian( const VectorC &X ) const;
    //: Calculate Jacobian matrix at X

  private:
    // For our motion model:
    // x(k+1) = x(k) + u(k) {+ v(k)}
    // We need these to calculate the jacobian
  };
  
  
  //!userlevel=Normal
  //: This is a null measurment model for use with ExtendedKalmanFilters.
  // The jacobian is a identity matrix, and the measurement values 
  // are passed straight through
  
  class KalmanNullMeasurementModelC
    : public FunctionC
  {
  public:
    // Creates an invalid handle
    KalmanNullMeasurementModelC()
      : FunctionC( *new KalmanNullMeasurementModelBodyC() )
    { 
      //cerr <<"default constructor in KalmanNullMeasurementmodelc\n"; 
    }
    //: Default Constructor
    
  protected:
    inline KalmanNullMeasurementModelC( KalmanNullMeasurementModelBodyC &bod ) : FunctionC(bod)
    {}
    //: Body constructor

    inline KalmanNullMeasurementModelC( KalmanNullMeasurementModelBodyC *bod ) : FunctionC(bod)
    {}
    //: Body ptr constructor
    inline KalmanNullMeasurementModelBodyC &Body()
    { return static_cast<KalmanNullMeasurementModelBodyC &>(FunctionC::Body()); }
    //: Access body

    inline const KalmanNullMeasurementModelBodyC& Body() const
    { return static_cast<const KalmanNullMeasurementModelBodyC &>(FunctionC::Body()); }
    //: Access body (for constant handle)
		  
  public:

    inline VectorC Apply( const VectorC &state ) const
    { return Body().Apply(state); }
    //: Apply Motion Model function to state with control_input 
    // Predict the Next State
    
    inline MatrixC Jacobian( const VectorC &X ) const
    { return Body().Jacobian(X); }
    //: Calculate Jacobian matrix at X
    
  };
  
#if 0
  inline istream &operator>>(istream &strm,KalmanNullMeasurementModelC &obj)
  {
    obj = KalmanNullMeasurementModelC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline ostream &operator<<(ostream &out,const KalmanNullMeasurementModelC &obj)
  {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
#endif
  
}
#endif
