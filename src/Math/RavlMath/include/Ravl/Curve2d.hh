// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_CURVE2D_HEADER 
#define RAVL_CURVE2D_HEADER 1
///////////////////////////////////////////////////////////
//! docentry="Ravl.API.Math.Geometry.2D"
//! rcsid="$Id$"
//! author="Charles Galambos"
//! date="26/08/1998"
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Euclidean/2D/Curve2d.hh"

#include "Ravl/Point2d.hh"
#include "Ravl/Vector2d.hh"
#include "Ravl/RCHandleV.hh"

namespace RavlN {
  class IndexRange2dC;
  template <class DataT> class SArray1dC;
  
  //! userlevel=Develop
  //: 2d Parametric curve base class
  
  class Curve2dBodyC 
    :  public RCBodyVC 
  {
  public:
    inline Curve2dBodyC() {}
    //: Default constuctor.
    
    inline Curve2dBodyC(const Curve2dBodyC &oth) 
    {}
    //: Copy constructor.
    
    virtual Point2dC Point(RealT p) const;
    //: Localion of curve at p
    
    virtual Vector2dC Tangent(RealT p) const;
    //: Tangent to curve at p
    
    virtual Vector2dC UnitTangent(RealT p) const;
    //: Tangent to curve at p
    
    virtual RealT Closest(Point2dC Pnt) const;
    //: Parametric value of closest point on curve to Pnt.
    
    virtual void GetLimits(RealT &Low,RealT &High,const IndexRange2dC &Rect) const;
    //: Get limits of paramiter if shape is within Rectangle 'Rect'.
    
    virtual IntT CodeSize() const { return 0; }
    //: Estimated bits to code primitive.
  
    virtual RealT Fit(const SArray1dC<Point2dC> &Pnts);
    //: Fit the current curve to the pnts.
    
    virtual bool WriteGF(ostream &Out) const;
    //: Write GF file.
    
    virtual bool WriteGFHeader(ostream &Out) const;
    //: Write GF file.
    
    virtual void Print(ostream &out) const;
    //: Print to ostream.
  };
  
  /////////////////////////////////
  
  //! userlevel=Normal
  //: 2d Parametric curve
  
  class Curve2dC 
    : public RCHandleC<Curve2dBodyC> 
  {
  public:
    inline Curve2dC() 
    {}
    //: Default Constuctor.
    // Creates an invalid handle.
    
    inline Curve2dC(const Curve2dC &oth) 
      : RCHandleC<Curve2dBodyC>(oth)
    {}
    //: Copy constructor.
    
    inline Curve2dC(Curve2dBodyC &oth)
      : RCHandleC<Curve2dBodyC>(oth)
    {}
    //: Body constructor.
    
    inline Point2dC Point(RealT p) const
    { return Body().Point(p); }
    
    //: Localion of curve at p
    
    inline Vector2dC Tangent(RealT p) const
    { return Body().Tangent(p); }
    //: Tangent to curve at p
    
    inline Vector2dC UnitTangent(RealT p) const
    { return Body().UnitTangent(p); }
    //: Tangent to curve at p
    
    inline RealT Closest(Point2dC Pnt) const
    { return Body().Closest(Pnt); }
    //: Parametric value of closest point on curve to Pnt.
    
    inline void GetLimits(RealT &Low,RealT &High,const IndexRange2dC &Rect) const
    { Body().GetLimits(Low,High,Rect); }
    //: Get limits of paramiter if shape is within Rectangle 'Rect'.
    
    inline IntT CodeSize() const 
    { return Body().CodeSize(); }
    //: Estimated bits to code primitive.
    
    inline RealT Fit(const SArray1dC<Point2dC> &Pnts)
    { return Body().Fit(Pnts); }
    //: Fit the current curve to the pnts.
    
    inline bool WriteGF(ostream &Out) const
    { return Body().WriteGF(Out); }
    //: Write GF file.
    
    inline bool WriteGFHeader(ostream &Out) const
    { return Body().WriteGFHeader(Out); }
    //: Write GF file.
    
    inline void Print(ostream &out) const
    { Body().Print(out); }
    //: Print to ostream.
  };
  
  ostream & operator<<(ostream & outS, const Curve2dC & cv);
  // Sends the Curve to output stream.
  
  //////////////////////////////
  
  //! userlevel=Normal
  //: 2d Parametric Curve Body Template
  
  template<class Curve>
  class Curve2dIC : public Curve2dBodyC, public Curve {
  public:
    Curve2dIC(const Curve &Oth) 
      : Curve(Oth) {}
    //: Construct from curve.
    
    Curve2dIC(const Curve2dIC<Curve> &Oth) 
    : Curve(Oth) {}
    //: Copy constructer.
    
    virtual Point2dC Point(RealT p) const 
    { return Curve::Point(p); }
    //: Localion of curve at p
    
    virtual Vector2dC Tangent(RealT p) const 
    { return Curve::Tangent(p); }
    //: Tangent to curve at p
    
    virtual Vector2dC UnitTangent(RealT p) const 
    { return Curve::UnitTangent(p); }
    //: Tangent to curve at p
    
    virtual RealT Closest(Point2dC Pnt) const 
    { return Curve::Closest(Pnt); }
    //: Parametric value of closest point on curve to Pnt.
    
    virtual void GetLimits(RealT &Low,RealT &High,const IndexRange2dC &Rect) const
    { Curve::GetLimits(Low,High,Rect); }
    //: Get limits of paramiter if shape is within Rect.
    
    virtual IntT CodeSize() const 
    { return Curve::CodeSize(); }
    //: Estimated bits to code primitive.
    
    virtual RealT Fit(const SArray1dC<Point2dC> &Pnts) 
    { return Curve::Fit(Pnts); }
    //: Fit the current curve to the pnts.
    
    virtual bool WriteGF(ostream &Out) const 
    { return Curve::WriteGF(Out); }
    //: Write GF file.
    
    virtual bool WriteGFHeader(ostream &Out) const 
    { return Curve::WriteGFHeader(Out); }
    //: Write GF header for this object type.
    
    virtual void Print(ostream &out) const 
    { Curve::Print(out); }
    //: Print to ostream.
    
  };
}


#endif
