// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DISTANCESQREUCLIDEAN_HEADER 
#define RAVL_DISTANCESQREUCLIDEAN_HEADER 1
//////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! docentry="Ravl.API.Pattern Recognition.Distance"
//! file="Ravl/PatternRec/Distance/DistanceSqrEuclidean.hh"

#include "Ravl/PatternRec/Distance.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Euclidean distance body.
  
  class DistanceSqrEuclideanBodyC
    : public DistanceBodyC
  {
  public:
    DistanceSqrEuclideanBodyC();
    //: Default constructor.
    
    DistanceSqrEuclideanBodyC(const XMLFactoryContextC &factory);
    //: Construct from XML factory

    DistanceSqrEuclideanBodyC(std::istream &strm);
    //: Load from stream.
    
    DistanceSqrEuclideanBodyC(BinIStreamC &strm);
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
  //: SqrEuclidean distance metric
  
  class DistanceSqrEuclideanC
    : public DistanceC
  {
  public:
    DistanceSqrEuclideanC()
      : DistanceC(*new DistanceSqrEuclideanBodyC())
      {}
    //: Default constructor.
    
    DistanceSqrEuclideanC(const XMLFactoryContextC &factory)
      : DistanceC(*new DistanceSqrEuclideanBodyC(factory))
    {}
    //: Construct from XML factory

    DistanceSqrEuclideanC(std::istream &strm);
    //: Load from stream.
    
    DistanceSqrEuclideanC(BinIStreamC &strm);
    //: Load from binary stream.
    
  };
  
  inline std::istream &operator>>(std::istream &strm,DistanceSqrEuclideanC &obj) {
    obj = DistanceSqrEuclideanC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline std::ostream &operator<<(std::ostream &out,const DistanceSqrEuclideanC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,DistanceSqrEuclideanC &obj) {
    obj = DistanceSqrEuclideanC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const DistanceSqrEuclideanC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
 
}



#endif
