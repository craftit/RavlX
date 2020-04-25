// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DISTANCE_HEADER 
#define RAVL_DISTANCE_HEADER 1
//////////////////////////////////////////////////////////////
//! lib=RavlPatternRec
//! docentry="Ravl.API.Pattern Recognition.Distance"
//! file="Ravl/PatternRec/Distance/Distance.hh"

#include "Ravl/RefCounter.hh"
#include "Ravl/Vector.hh"
#include "Ravl/PatternRec/Function1.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Distance body.
  
  class DistanceBodyC
    : public Function1BodyC
  {
  public:
    DistanceBodyC();
    //: Default constructor.

    DistanceBodyC(const XMLFactoryContextC &factory);
    //: Construct from XML factory
    
    DistanceBodyC(std::istream &strm);
    //: Load from stream.
    
    DistanceBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (std::ostream &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor

    virtual RealT Measure(const VectorC &d1,const VectorC &d2) const;
    //: Measure the distance from d1 to d2.
    
    virtual RealT Magnitude(const VectorC &d1) const;
    //: Measure the magnitude of d1.
    
    virtual RealT Apply1(const VectorC &data) const;
    //: Apply function to 'data'
  };
  
  //! userlevel=Normal
  //: Distance Metric
  
  class DistanceC
    : public Function1C
  {
  public:
    DistanceC()
    {}
    //: Default constructor.
    // Creates an invalid handle.

    DistanceC(const XMLFactoryContextC &factory)
      :  Function1C(*new DistanceBodyC(factory))
    {}
    //: Construct from XML factory
    
    DistanceC(std::istream &strm);
    //: Load from stream.
    
    DistanceC(BinIStreamC &strm);
    //: Load from binary stream.
    
  protected:
    DistanceC(DistanceBodyC &bod)
      : Function1C(bod)
    {}
    //: Body constructor.
    
    DistanceC(const DistanceBodyC *bod)
      : Function1C(bod)
    {}
    //: Body ptr constructor.
    
    DistanceBodyC &Body()
    { return static_cast<DistanceBodyC &>(FunctionC::Body()); }
    //: Access body.
    
    const DistanceBodyC &Body() const
    { return static_cast<const DistanceBodyC &>(FunctionC::Body()); }
    //: Access body.
    
  public:
    
    RealT Measure(const VectorC &d1,const VectorC &d2) const
    { return Body().Measure(d1,d2); }
    //: Measure the distance from d1 to d2.
    
    RealT Magnitude(const VectorC &d1) const 
    { return Body().Magnitude(d1); }
    //: Measure the magnitude of d1.
    
  };
  
  inline std::istream &operator>>(std::istream &strm,DistanceC &obj) {
    obj = DistanceC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline std::ostream &operator<<(std::ostream &out,const DistanceC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,DistanceC &obj) {
    obj = DistanceC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const DistanceC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
}


#endif
