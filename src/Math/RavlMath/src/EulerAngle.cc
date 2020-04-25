// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! date="7/12/2002"
//! author="Joel Mitchelson"
//! file="Ravl/Math/Geometry/Euclidean/3D/EulerAngle.cc"

#include "Ravl/EulerAngle.hh"
#include "Ravl/StdConst.hh"

namespace RavlN
{
  // evaluation of trig functions of euler angles 
#define DO_EULER_TRIG_FUNCTIONS \
    RealT sa = sin(a[0]); \
    RealT sb = sin(a[1]); \
    RealT sc = sin(a[2]); \
    RealT ca = cos(a[0]); \
    RealT cb = cos(a[1]); \
    RealT cc = cos(a[2]);

  // epsilon
#define SMALL_ANGLE (RavlConstN::minReal*16)

  //---------------------------------------------
  // C-style euler angle -> matrix conversions

#if 0
  // template 
  void EulerAAAToMatrix(const RealT* a, RealT* R)
  {
    DO_EULER_TRIG_FUNCTIONS;
    R[0] = ;
    R[1] = ;
    R[2] = ;
    R[3] = ;
    R[4] = ;
    R[5] = ;
    R[6] = ;
    R[7] = ;
    R[8] = ;
  }
#endif


  void EulerXYXToMatrix(const RealT* a, RealT* R)
  {
    DO_EULER_TRIG_FUNCTIONS;
    R[0] = cb;
    R[1] = sb*sc;
    R[2] = sb*cc;
    R[3] = sa*sb;
    R[4] = -sa*cb*sc+ca*cc;
    R[5] = -sa*cb*cc-ca*sc;
    R[6] = -ca*sb;
    R[7] = ca*cb*sc+sa*cc;
    R[8] = ca*cb*cc-sa*sc;
  }

  void EulerXYZToMatrix(const RealT* a, RealT* R)
  {
    DO_EULER_TRIG_FUNCTIONS;
    R[0] = cb*cc;
    R[1] = -cb*sc;
    R[2] = sb;
    R[3] = sa*sb*cc+ca*sc;
    R[4] = -sa*sb*sc+ca*cc;
    R[5] = -sa*cb;
    R[6] = -ca*sb*cc+sa*sc;
    R[7] = ca*sb*sc+sa*cc;
    R[8] = ca*cb;
  }

  void EulerXZXToMatrix(const RealT* a, RealT* R)
  {
    DO_EULER_TRIG_FUNCTIONS;
    R[0] = cb;
    R[1] = -sb*cc;
    R[2] = sb*sc;

    R[3] = ca*sb;
    R[4] = ca*cb*cc-sa*sc;
    R[5] = -ca*cb*sc-sa*cc;

    R[6] = sa*sb;
    R[7] = sa*cb*cc+ca*sc;
    R[8] = -sa*cb*sc+ca*cc;
  }

  //---------------------------------------------
  // C-style euler angle -> matrix conversions

#if 0
  // template
  void MatrixToEulerAAA(const RealT* R, RealT* a, const RealT a0_default /*=0.0*/)
  {
    a[1] = atan2(,);
    RealT  = ;
    if ( > SMALL_ANGLE ||  < -SMALL_ANGLE)
    {
      RealT r_ = ;
      a[0] = atan2(,);
      a[2] = atan2(,);
    }
    else
    {
      a[0] = a0_default;
      a[2] = atan2(,);
    }
  }
#endif

  void MatrixToEulerXYX(const RealT* R, RealT* a, const RealT a0_default /*=0.0*/)
  {
    a[1] = atan2(sqrt(R[1]*R[1]+R[2]*R[2]), R[0]);
    RealT sb = sin(a[1]);
    if (sb > SMALL_ANGLE || sb < -SMALL_ANGLE)
    {
      RealT r_sb = 1.0 / sb;
      a[0] = atan2(R[3]*r_sb,-R[6]*r_sb);
      a[2] = atan2(R[1]*r_sb, R[2]*r_sb);
    }
    else
    {
      a[0] = a0_default;
      a[2] = atan2(-R[5],R[4]);
    }
  }

  void MatrixToEulerXYZ(const RealT* R, RealT* a, const RealT a0_default /*=0.0*/)
  {
    a[1] = atan2(R[2], sqrt(R[0]*R[0]+R[1]*R[1]));
    RealT cb = cos(a[1]);
    if (cb > SMALL_ANGLE || cb < -SMALL_ANGLE)
    {
      RealT r_cb = 1.0 / cb;
      a[0] = atan2(-R[5]*r_cb, R[8]*r_cb);
      a[2] = atan2(-R[1]*r_cb, R[0]*r_cb);
    }
    else
    {
      a[0] = a0_default;
      a[2] = atan2(R[3],R[4]);
    }
  }

  void MatrixToEulerXZX(const RealT* R, RealT* a, const RealT a0_default /*=0.0*/)
  {
    a[1] = atan2(sqrt(R[3]*R[3]+R[6]*R[6]), R[0]);
    RealT sb = sin(a[1]);
    if (sb > SMALL_ANGLE || sb < -SMALL_ANGLE)
    {
      RealT r_sb = 1.0 / sb;
      a[0] = atan2(R[6]*r_sb,R[3]*r_sb);
      a[2] = atan2(R[2]*r_sb,-R[1]*r_sb);
    }
    else
    {
      a[0] = a0_default;
      a[2] = (fmod(fabs(a[1]), 2.0*RavlConstN::pi) < SMALL_ANGLE) ? atan2(R[7],R[4]) : atan2(R[7],-R[4]);
    }
  }

  //---------------------------------------------


  //---------------------------------------------
  // generic conversion

  void EulerToMatrix(const Vector3dC& a, 
		     Matrix3dC& R, 
		     const EulerSequenceC sequence)
  {
    switch (sequence)
    {
    case EulerSequenceC::XYX:
      EulerXYXToMatrix(a,R);
      break;

    case EulerSequenceC::XYZ:
      EulerXYZToMatrix(a,R);
      break;

    case EulerSequenceC::XZX:
      EulerXZXToMatrix(a,R);
      break;

    default:
      RavlAlwaysAssert(false);  // unknown euler sequence
      break;
    }
  }

  void MatrixToEuler(const Matrix3dC& R, 
		     Vector3dC& a, 
		     const EulerSequenceC sequence,
		     const RealT a0_default /*=0.0*/)
  {
    switch (sequence)
    {
    case EulerSequenceC::XYX:
      MatrixToEulerXYX(R,a,a0_default);
      break;

    case EulerSequenceC::XYZ:
      MatrixToEulerXYZ(R,a,a0_default);
      break;

    case EulerSequenceC::XZX:
      MatrixToEulerXZX(R,a,a0_default);
      break;

    default:
      RavlAlwaysAssert(false);  // unknown euler sequence
      break;
    }

  }

}




