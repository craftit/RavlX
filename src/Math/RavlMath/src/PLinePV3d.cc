// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2007, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlMath

#include "Ravl/PLinePV3d.hh"
#include "Ravl/PPlane3d.hh"
#include "Ravl/PProjection3d.hh"
#include "Ravl/LinePV3d.hh"
#include "Ravl/BinStream.hh"

namespace RavlN {

  //: Point of intersection of a plane with this line.
  
  RealT PLinePV3dC::IntersectionParam(const PPlane3dC &plane) 
  { return -plane.Dot(m_p) / plane.Dot(m_v); }
  
  //: Point of intersection of a plane with this line.
  
  PPoint3dC PLinePV3dC::Intersection(const PPlane3dC &plane) 
  { return Value(IntersectionParam(plane)); }
  
  //: Transform a line
  
  PLinePV3dC operator*(const PProjection3dC &projection,const PLinePV3dC &line) 
  { return PLinePV3dC(projection * line.P(),projection * line.V()); }
  
  //: Write to text stream.
  
  ostream &operator<<(ostream &strm,const PLinePV3dC &line) {
    strm << line.P() << ' ' << line.V();
    return strm;
  }
  
  //: Read from text stream.
  
  istream &operator>>(istream &strm,PLinePV3dC &line) {
    strm >> line.P() >> line.V();
    return strm;
  }
  
  //: Write to binary stream.
  
  BinOStreamC &operator<<(BinOStreamC &strm,const PLinePV3dC &line) {
    strm << line.P() << line.V();    
    return strm;
  }
  
  //: Read from binary stream.
  
  BinIStreamC &operator>>(BinIStreamC &strm,PLinePV3dC &line) {
    strm >> line.P() >> line.V();
    return strm;
  }

}
