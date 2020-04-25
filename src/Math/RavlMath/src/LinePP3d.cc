// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Euclidean/3D/LinePP3d.cc"

#include "Ravl/Stream.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/StdMath.hh"     //::Abs()
#include "Ravl/Point3d.hh"
#include "Ravl/Vector3d.hh"
#include "Ravl/LinePP3d.hh"

namespace RavlN {
  
  RealT LinePP3dC::Distance(const LinePP3dC & line) { 
    // more information in Rektorys: 
    // Prehled uzite matematiky, SNTL, Praha 1988, p. 205
    
    Vector3dC axb(Vector().Cross(line.Vector()));
    RealT     modul = axb.Magnitude();
    if (modul == 0)  // the straight lines are parallel
      return RavlN::Distance(FirstPoint(),line);
    // the straight lines are not parallel
    return RavlN::Abs(Vector3dC(line.FirstPoint() - FirstPoint()).Dot(axb))/modul;
  }
  
  LinePP3dC LinePP3dC::ShortestLine(const LinePP3dC & line) {
    Vector3dC axb(Vector().Cross(line.Vector()));
    RealT  axbNorm = axb.Dot(axb);
    Vector3dC pmr(FirstPoint() - line.FirstPoint());
    return LinePP3dC(Point3dC(FirstPoint() + Vector() * ((axb.Dot(line.Vector().Cross(pmr))) / axbNorm)),
		     Point3dC(line.FirstPoint() + line.Vector() * ((axb.Dot(Vector().Cross(pmr))) / axbNorm)));
  }
  
  RealT LinePP3dC::Distance(const Point3dC & p) const {
    Vector3dC lineVector(Vector());
    return  lineVector.Cross(Vector3dC(FirstPoint() - p)).Magnitude() / lineVector.Magnitude();
  }
  
}

