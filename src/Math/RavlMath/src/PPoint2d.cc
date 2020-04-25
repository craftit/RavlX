// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! date="26.04.1995"
//! author="Radek Marik"
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Projective/2D/PPoint2d.cc"

#include "Ravl/PPoint2d.hh"
#include "Ravl/PLine2d.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/Matrix3d.hh"

namespace RavlN {

  //: Normalise an array of points.
  // This assumes there are no points at infinity.
  //!param: raw - Raw points to be normalised
  //!param: norm - Normalised points.
  //!param: normMat - Normalisation matrix 
  //!return: Normalisation found and applied.
  
  bool Normalise(const SArray1dC<PPoint2dC> &raw,SArray1dC<PPoint2dC> &norm,Matrix3dC &normMat) {
    if(raw.Size() == 0)
      return true;
    norm = SArray1dC<PPoint2dC>(raw.Size());
    Point2dC mean(0,0);
    for(SArray1dIter2C<PPoint2dC,PPoint2dC> it(raw,norm);it;it++) {
      Point2dC at = it.Data1().Point2d();
      it.Data2() = PPoint2dC(at[0],at[1],1);
      mean += at;
    }
    mean /= static_cast<RealT>(raw.Size());
    RealT d = 0;
    for(SArray1dIterC<PPoint2dC> it(norm);it;it++) {
      (*it)[0] -= mean[0];
      (*it)[1] -= mean[1];
      d += Sqrt(Sqr((*it)[0]) + Sqr((*it)[1]));
    }
    d = IsSmall(d) ? 1 : static_cast<RealT>(raw.Size())/d;
    for(SArray1dIterC<PPoint2dC> it(norm);it;it++) {
      (*it)[0] *= d;
      (*it)[1] *= d;
    }
    normMat = Matrix3dC(d,0,-mean[0] * d,
			0,d,-mean[1] * d,
			0,0,1);
    return true;
  }
  
  PLine2dC PPoint2dC::PLine(const PPoint2dC & p) const
  { return PPointLine2d(p); }
  
  ostream & operator<<(ostream & outS, const PPoint2dC & p) {
    outS << p.P1() << ' ' << p.P2() << ' ' << p.P3();
    return outS;
  }
  
  istream & operator>>(istream & inS, PPoint2dC & p) {
    inS >> p.P1() >> p.P2() >> p.P3();
    return inS;
  }

}
