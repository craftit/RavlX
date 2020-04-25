// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2007, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_PLINEPV3D_HEADER
#define RAVL_PLINEPV3D_HEADER 1
///////////////////////////////////////////////////////////////////////////
//! file="Ravl/Math/Geometry/Projective/3D/PLinePV3d.hh"
//! lib=RavlMath
//! userlevel=Normal
//! author="Charles Galambos"
//! date="16/05/2007"
//! docentry="Ravl.API.Math.Projective Geometry.3D"

#include "Ravl/PPoint3d.hh"

namespace RavlN {
  class PPlane3dC;
  class PProjection3dC;
  
  //: Line in 3d space.
  // Defined by a point and a direction.
  
  class PLinePV3dC {
  public:
    PLinePV3dC()
    {}
    //: Default constructor.
    
    PLinePV3dC(const PPoint3dC &p,const PPoint3dC &v)
      : m_p(p),
        m_v(v)
    {}
    //: Constructor of a line containing point p in direction v
    // Note Is expected that v[4] == 0.
    
    const PPoint3dC &P() const
    { return m_p; }
    //: Position of first point.
    
    const PPoint3dC &V() const
    { return m_v; }
    //: Direction vector

    PPoint3dC &P()
    { return m_p; }
    //: Position of first point.
    
    PPoint3dC &V()
    { return m_v; }
    //: Direction vector
    
    PPoint3dC Value(RealT t) const
    { return m_p + t * m_v; }
    //: Get position of point t along line in direction d
    // Value = p + t * v
    
    RealT IntersectionParam(const PPlane3dC &plane);
    //: Point of intersection of a plane with this line.
    // Returns position of itersection as the position along the line
    // t, where the point of interection q is given by q = p + t * v
    
    PPoint3dC Intersection(const PPlane3dC &plane);
    //: Point of intersection of a plane with this line.
    
  protected:
    PPoint3dC m_p; // Origin of point.
    PPoint3dC m_v;
  };

  
  PLinePV3dC operator*(const PProjection3dC &projection,const PLinePV3dC &line);
  //: Transform a line
  
  
  ostream &operator<<(ostream &strm,const PLinePV3dC &line);
  //: Write to text stream.
  
  istream &operator>>(istream &strm,PLinePV3dC &line);
  //: Read from text stream.

  BinOStreamC &operator<<(BinOStreamC &strm,const PLinePV3dC &line);
  //: Write to binary stream.
  
  BinIStreamC &operator>>(BinIStreamC &strm,PLinePV3dC &line);
  //: Read from binary stream.
  
}


#endif
