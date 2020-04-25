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
//! author="Radek Marik"
//! date="26.04.1995"
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Projective/3D/PPoint3d.cc"

#include "Ravl/PPoint3d.hh"
#include "Ravl/Matrix4d.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/SArray1dIter.hh"

namespace RavlN {

  //: Normalise an array of points.
  // This assumes there are no points at infinity.
  //!param: raw - Raw points to be normalised
  //!param: norm - Normalised points.
  //!param: normMat - Normalisation matrix 
  //!return: Normalisation found and applied.
  
  bool Normalise(const SArray1dC<PPoint3dC> &raw,SArray1dC<PPoint3dC> &norm,Matrix4dC &normMat) {
    if(raw.Size() == 0)
      return true;
    norm = SArray1dC<PPoint3dC>(raw.Size());
    Point3dC mean(0,0,0);
    for(SArray1dIter2C<PPoint3dC,PPoint3dC> it(raw,norm);it;it++) {
      Point3dC at = it.Data1().Point3d();
      it.Data2() = PPoint3dC(at[0],at[1],at[2],1);
      mean += at;
    }
    mean /= static_cast<RealT>(raw.Size());
    RealT d = 0;
    for(SArray1dIterC<PPoint3dC> it(norm);it;it++) {
      (*it)[0] -= mean[0];
      (*it)[1] -= mean[1];
      (*it)[2] -= mean[2];
      d += Sqrt(Sqr((*it)[0]) + Sqr((*it)[1]) + Sqr((*it)[2]));
    }
    d = IsSmall(d) ? 1 : static_cast<RealT>(raw.Size())/d;
    for(SArray1dIterC<PPoint3dC> it(norm);it;it++) {
      (*it)[0] *= d;
      (*it)[1] *= d;
      (*it)[2] *= d;
    }
    normMat = Matrix4dC(d,0,0,-mean[0] * d,
			0,d,0,-mean[1] * d,
                        0,0,d,-mean[2] * d,
			0,0,0, 1);
    return true;
  }


  ostream & 
  operator<<(ostream & outS, const PPoint3dC & p)
  {
    outS << p.P1() << ' ' << p.P2() << ' ' << p.P3() << ' ' << p.P4();
    return outS;
  }
  
  istream & 
  operator>>(istream & inS, PPoint3dC & p)
  {
    inS >> p.P1() >> p.P2() >> p.P3() >> p.P4();
    return inS;
  }

}
