// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
#ifndef RAVL_DISTANCECHI2_HEADER
#define RAVL_DISTANCECHI2_HEADER 1

//////////////////////////////////////////////////////////////////////////////
//! file =      "Ravl/PatternRec/Distance/DistanceChi2.hh"
//! author =    "Teo de Campos"
//! lib=RavlPatternRec
//! date =      "12/10/2010"
//! rcsid =     "$Id: DistanceChi2.hh 12208 2010-10-12 12:54:07Z td0005 $"
//! docentry =  "Ravl.API.Pattern_Recognition.Distance"
//////////////////////////////////////////////////////////////////////////////

#include "Ravl/PatternRec/Distance.hh"

// For the function ComputeBestChi2Gamma:
#include "Ravl/PatternRec/Sample.hh"
#include "Ravl/PatternRec/SampleIter.hh"
#include "Ravl/Vector.hh"


namespace RavlN{
  // using namespace RavlN;

  //! userlevel=Develop
  //: Chi2 measure 
  // Note: Chi^2 is not a true metric, as the triangle rule doesn't apply

  class DistanceChi2BodyC
    : public DistanceBodyC
  {
  public:
    DistanceChi2BodyC()
    {}
    //: Default constructor.
    
    DistanceChi2BodyC(const XMLFactoryContextC &factory);
    //: Construct from XML factory

    DistanceChi2BodyC(std::istream &strm);
    //: Load from stream.
    
    DistanceChi2BodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (std::ostream &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual RealT Measure(const VectorC &d1,const VectorC &d2) const;
    //: Measure the distance from d1 to d2.
    
  };

  //! userlevel=Normal
  //: Chi2 measure 
  // Note: Chi^2 is not a true distance metric, as the triangle rule doesn't apply.
  
  class DistanceChi2C
    : public DistanceC
  {
  public:
    DistanceChi2C()
      : DistanceC(*new DistanceChi2BodyC())
    {}
    //: Default constructor.
    
    DistanceChi2C(const XMLFactoryContextC &factory)
      :  DistanceC(*new DistanceChi2BodyC(factory))
    {}
    //: Construct from XML factory

    DistanceChi2C(std::istream &strm);
    //: Load from stream.
    
    DistanceChi2C(BinIStreamC &strm);
    //: Load from binary stream.

    DistanceChi2C(DistanceC &base)
      : DistanceC(dynamic_cast<const DistanceChi2BodyC *>(BodyPtr(base)))
    {}
    //: Base class constructor.
    // Creates an invalid handle if 'base' is not a DistanceChi2C.
    
  };
  
  inline std::istream &operator>>(std::istream &strm,DistanceChi2C &obj) {
    obj = DistanceChi2C(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline std::ostream &operator<<(std::ostream &out,const DistanceChi2C &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,DistanceChi2C &obj) {
    obj = DistanceChi2C(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const DistanceChi2C &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  
  //----------------------------------------------------
  // Function to compute the best Gamma parameter
  RealT ComputeBestChi2Gamma(SampleC<VectorC> smplVec);

} // end of namespace Ravl
#endif

