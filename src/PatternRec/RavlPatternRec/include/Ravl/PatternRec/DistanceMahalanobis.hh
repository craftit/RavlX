// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DISTANCEMAHALANOBIS_HEADER 
#define RAVL_DISTANCEMAHALANOBIS_HEADER 1
//////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! docentry="Ravl.API.Pattern Recognition.Distance"
//! file="Ravl/PatternRec/Distance/DistanceMahalanobis.hh"

#include "Ravl/PatternRec/Distance.hh"
#include "Ravl/Matrix.hh"
#include "Ravl/XMLFactoryRegister.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Mahalanobis Distance body.
  
  class DistanceMahalanobisBodyC
    : public DistanceBodyC
  {
  public:

    DistanceMahalanobisBodyC(const MatrixC & covVar);
    //: Constructor.
    
    DistanceMahalanobisBodyC(const XMLFactoryContextC &factory);
    //: Construct from XML factory
    
    DistanceMahalanobisBodyC(std::istream &strm);
    //: Load from stream.
    
    DistanceMahalanobisBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (std::ostream &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual RealT Measure(const VectorC &d1,const VectorC &d2) const;
    //: Measure the distance from d1 to d2.
    
    virtual RealT Magnitude(const VectorC &d1) const;
    //: Measure the magnitude of d1.

    virtual MatrixC Jacobian (const VectorC &X) const;
    //: Calculates Jacobian matrix at X
    
  protected:
    MatrixC iCovar; // Inverse covariance matrix.
  };
  
  //! userlevel=Normal
  //: Mahalanobis distance metric
  
  class DistanceMahalanobisC
    : public DistanceC
  {
  public:
    DistanceMahalanobisC(const MatrixC &covVar)
      : DistanceC(*new DistanceMahalanobisBodyC(covVar))
    {}
    //: Constructor.

    DistanceMahalanobisC(const XMLFactoryContextC &factory)
      :  DistanceC(*new DistanceMahalanobisBodyC(factory))
    {}
    //: Construct from XML factory
    
    DistanceMahalanobisC()
    {}
    //: Default constructor.
    // creates an invalid handle.
    
    DistanceMahalanobisC(std::istream &strm);
    //: Load from stream.
    
    DistanceMahalanobisC(BinIStreamC &strm);
    //: Load from binary stream.
    
  };
  
  inline std::istream &operator>>(std::istream &strm,DistanceMahalanobisC &obj) {
    obj = DistanceMahalanobisC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline std::ostream &operator<<(std::ostream &out,const DistanceMahalanobisC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,DistanceMahalanobisC &obj) {
    obj = DistanceMahalanobisC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const DistanceMahalanobisC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.

  
}



#endif
