// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
//! file="Ravl/Math/Geometry/Projective/3D/PPointPlane3d.cc"

//! author="Radek Marik"
//! date="26.06.1995"
//! rcsid="$Id$"
//! lib=RavlMath

#include "Ravl/Matrix3d.hh"
#include "Ravl/PlaneABCD3d.hh"
#include "Ravl/PPointPlane3d.hh"
#include "Ravl/BinStream.hh"

namespace RavlN {
  
  PPointPlane3dC::PPointPlane3dC(const PPointPlane3dC & p0,
				 const PPointPlane3dC & p1,
				 const PPointPlane3dC & p2)
    // Ref.: pp. 80 - 81
    : TFVectorC<RealT,4>(Point4d(AdjointDet(p0.P2(), p0.P3(), p0.P4(), 
                                            p1.P2(), p1.P3(), p1.P4(), 
                                            p2.P2(), p2.P3(), p2.P4()), 
                                 AdjointDet(p0.P1(), p0.P4(), p0.P3(),  
                                            p1.P1(), p1.P4(), p1.P3(),  
                                            p2.P1(), p2.P4(), p2.P3()), 
                                 AdjointDet(p0.P1(), p0.P2(), p0.P4(),  
                                            p1.P1(), p1.P2(), p1.P4(),  
                                            p2.P1(), p2.P2(), p2.P4()), 
                                 AdjointDet(p0.P1(), p0.P3(), p0.P2(),  
                                            p1.P1(), p1.P3(), p1.P2(),  
                                            p2.P1(), p2.P3(), p2.P2()))
                         )
  {}
  
  
  PPointPlane3dC::PPointPlane3dC(const PlaneABCD3dC & p)
    : TFVectorC<RealT,4>(Point4d(p.A(), p.B(), p.C(), p.D()))
  {}
  
  ostream & 
  operator<<(ostream & outS, const PPointPlane3dC & p)
  {
    outS << p.P1() << ' ' << p.P2() << ' ' << p.P3() << ' ' << p.P4();
    return outS;
  }
  
  istream & 
  operator>>(istream & inS, PPointPlane3dC & p)
  {
    inS >> p.P1() >> p.P2() >> p.P3() >> p.P4();
    return inS;
  }
  
  BinOStreamC & operator<<(BinOStreamC & strm, const PPointPlane3dC & p) {
    strm << p.P1() << p.P2() << p.P3() << p.P4();
    return strm;
  }
  // Writes the 'point' into the output stream.
  
  BinIStreamC & operator>>(BinIStreamC & strm, PPointPlane3dC & p) {
    strm >> p.P1() >> p.P2() >> p.P3() >> p.P4();
    return strm;
  }
  // Reads the point parameters from the input stream.
  
}
