// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2013, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DISTANCECORRELATION_HEADER
#define RAVL_DISTANCECORRELATION_HEADER 1
//////////////////////////////////////////////////////////////
//! lib=RavlPatternRec
//! docentry="Ravl.API.Pattern Recognition.Distance"
//! file="Ravl/PatternRec/Distance/DistanceCorrelation.hh"

#include "Ravl/PatternRec/Distance.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: This returns a 1 - correlation (or dot product) of the two vectors.
  //: This is not strictly a distance.
  //: Identical vectors return 0, anti correlated vectors 2.
  // Note if normalisation isn't used, this can produce negative distances.
  
  class DistanceCorrelationBodyC
    : public DistanceBodyC
  {
  public:
    DistanceCorrelationBodyC(bool normalise = true);
    //: Constructor.
    // If normalise is true the vectors are normalised to unit length as
    // part of the computation
    
    DistanceCorrelationBodyC(const XMLFactoryContextC &factory);
    //: Construct from XML factory

    DistanceCorrelationBodyC(std::istream &strm);
    //: Load from stream.
    
    DistanceCorrelationBodyC(BinIStreamC &strm);
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
    
    bool UseNormalise() const
    { return m_normalise; }
    //: Test if we're doing normalisation

  protected:
    bool m_normalise;
  };

  //! userlevel=Normal
  //: This returns a 1 - correlation (or dot product) of the two vectors.
  //: This is not strictly a distance.
  //: Identical vectors return 0, anti correlated vectors 2.
  // Note if normalisation isn't used, this can produce negative distances.

  class DistanceCorrelationC
    : public DistanceC
  {
  public:
    DistanceCorrelationC(bool normalise = true)
      : DistanceC(*new DistanceCorrelationBodyC(normalise))
    {}
    //: Constructor.
    // If normalise is true the vectors are normalised to unit length as
    // part of the computation
    
    DistanceCorrelationC(const XMLFactoryContextC &factory)
      :  DistanceC(*new DistanceCorrelationBodyC(factory))
    {}
    //: Construct from XML factory

    DistanceCorrelationC(std::istream &strm);
    //: Load from stream.
    
    DistanceCorrelationC(BinIStreamC &strm);
    //: Load from binary stream.

    DistanceCorrelationC(const DistanceC &base)
      : DistanceC(dynamic_cast<const DistanceCorrelationBodyC *>(BodyPtr(base)))
    {}
    //: Base class constructor.
    // Creates an invalid handle if 'base' is not a DistanceCorrelationC.

  protected:

    DistanceCorrelationBodyC &Body()
    { return static_cast<DistanceCorrelationBodyC &>(FunctionC::Body()); }
    //: Access body.

    const DistanceCorrelationBodyC &Body() const
    { return static_cast<const DistanceCorrelationBodyC &>(FunctionC::Body()); }
    //: Access body.

  public:

    bool UseNormalise() const
    { return Body().UseNormalise(); }
    //: Test if we're doing normalisation

  };
  
  inline std::istream &operator>>(std::istream &strm,DistanceCorrelationC &obj) {
    obj = DistanceCorrelationC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline std::ostream &operator<<(std::ostream &out,const DistanceCorrelationC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,DistanceCorrelationC &obj) {
    obj = DistanceCorrelationC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const DistanceCorrelationC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
}



#endif
