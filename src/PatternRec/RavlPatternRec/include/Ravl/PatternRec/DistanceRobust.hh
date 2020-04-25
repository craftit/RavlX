// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DISTANCEROBUST_HEADER 
#define RAVL_DISTANCEROBUST_HEADER 1
//////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! docentry="Ravl.API.Pattern Recognition.Distance"
//! author="Robert Crida"
//! date="22/4/1998"
//! file="Ravl/PatternRec/Distance/DistanceRobust.hh"

#include "Ravl/PatternRec/Distance.hh"

namespace RavlN {

  //! userlevel=Develop
  //: Robust magnitude metric.
  // Class for calculating robust distance to a metric using any of the other
  // distance metrics. A threshold is set and all elements of X greater than it
  // are clipped. The other metric is specified in the constructor. <p>
  // If you need separate clip limits for each component of the vector
  // let me know and I'll change it.
  
  class DistanceRobustBodyC
    : public DistanceBodyC
  {
  public:
    DistanceRobustBodyC(RealT nClipLimit,const DistanceC &metric);
    //: Constructor.

    DistanceRobustBodyC(const XMLFactoryContextC &factory);
    //: Construct from XML factory
    
    DistanceRobustBodyC(std::istream &strm);
    //: Load from stream.
    
    DistanceRobustBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (std::ostream &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor

    virtual RealT Measure(const VectorC &d1,const VectorC &d2) const;
    //: Measure the distance from d1 to d2.
    // Each value of d2 is limited to be within 'clipLimit' of d1.
    
    virtual RealT Magnitude(const VectorC &d1) const;
    //: Measure the magnitude of d1.
    
    virtual MatrixC Jacobian (const VectorC &X) const;
    //: Calculates Jacobian matrix at X

  protected:
    RealT clipLimit;
    DistanceC metric;
  };

  //! userlevel=Normal
  //: Robust magnitude metric.
  //
  // Class for calculating robust distance to a metric using any of the other
  // distance metrics. A threshold is set and all elements of X greater than it
  // are clipped. The other metric is specified in the constructor. <p>
  // If you need seperate clip limits for each component of the vector
  // let me know and I'll change it.
  
  class DistanceRobustC
    : public DistanceC
  {
  public:
    DistanceRobustC(RealT nClipLimit,const DistanceC &metric)
      : DistanceC(*new DistanceRobustBodyC(nClipLimit,metric))
    {}
    //: Constructor.
    // nClipLimit is maximum difference in one dimension, metric
    // is the metric to use on the limited values.
    
    DistanceRobustC(const XMLFactoryContextC &factory)
      : DistanceC(*new DistanceRobustBodyC(factory))
    {}
    //: Construct from XML factory

    DistanceRobustC()
    {}
    //: Default constructor.
    // creates an invalid handle.
    
    DistanceRobustC(std::istream &strm);
    //: Load from stream.
    
    DistanceRobustC(BinIStreamC &strm);
    //: Load from binary stream.

  };
  
  inline std::istream &operator>>(std::istream &strm,DistanceRobustC &obj) {
    obj = DistanceRobustC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline std::ostream &operator<<(std::ostream &out,const DistanceRobustC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,DistanceRobustC &obj) {
    obj = DistanceRobustC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const DistanceRobustC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.

}
#endif
