// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DISTANCEEUCLIDEAN_HEADER 
#define RAVL_DISTANCEEUCLIDEAN_HEADER 1
//////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! docentry="Ravl.API.Pattern Recognition.Distance"
//! file="Ravl/PatternRec/Distance/DistanceEuclidean.hh"

#include "Ravl/PatternRec/Distance.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Euclidean distance body.
  
  class DistanceEuclideanBodyC
    : public DistanceBodyC
  {
  public:
    DistanceEuclideanBodyC()
    {}
    //: Default constructor.
    
    DistanceEuclideanBodyC(const XMLFactoryContextC &factory);
    //: Construct from XML factory

    DistanceEuclideanBodyC(std::istream &strm);
    //: Load from stream.
    
    DistanceEuclideanBodyC(BinIStreamC &strm);
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
    
  };

  //! userlevel=Normal
  //: Euclidean distance metric
  
  class DistanceEuclideanC
    : public DistanceC
  {
  public:
    DistanceEuclideanC()
      : DistanceC(*new DistanceEuclideanBodyC())
    {}
    //: Default constructor.
    
    DistanceEuclideanC(const XMLFactoryContextC &factory)
      :  DistanceC(*new DistanceEuclideanBodyC(factory))
    {}
    //: Construct from XML factory

    DistanceEuclideanC(std::istream &strm);
    //: Load from stream.
    
    DistanceEuclideanC(BinIStreamC &strm);
    //: Load from binary stream.

    DistanceEuclideanC(DistanceC &base)
      : DistanceC(dynamic_cast<const DistanceEuclideanBodyC *>(BodyPtr(base)))
    {}
    //: Base class constructor.
    // Creates an invalid handle if 'base' is not a DistanceEuclideanC.
    
  };
  
  inline std::istream &operator>>(std::istream &strm,DistanceEuclideanC &obj) {
    obj = DistanceEuclideanC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline std::ostream &operator<<(std::ostream &out,const DistanceEuclideanC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,DistanceEuclideanC &obj) {
    obj = DistanceEuclideanC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const DistanceEuclideanC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
}



#endif
