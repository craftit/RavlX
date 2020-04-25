// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////////////////////////////
//! author =    "Jean-Yves Guillemaut"
//! lib=RavlAAM
//! date =      "01/03/2006"
//! file="Ravl/CompVision/ActiveAppearanceModels/AAMPoseEstimateCostFunction.cc"
//! rcsid="$Id: AAMPoseEstimateCostFunction.cc,v 1.3 2005/07/25 16:31:07 ees1wc Exp $"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

#include "Ravl/PatternRec/Cost.hh"
#include "Ravl/PatternRec/Parameters.hh"
#include "Ravl/Vector.hh"

#include "Ravl/Image/AAMPoseEstimateCostFunction.hh"

using namespace RavlN;

namespace RavlImageN {

  //: Constructor.
  //!param: parameters - Parameters of the cost function.
  //!param: C0 - Input value for p_C0.
  //!param: CT - Input value for p_CT.
  //!param: ST - Input value for p_ST.
  //!param: CP - Input value for p_CP.
  //!param: SP - Input value for p_SP.
  //!param: CTCP - Input value for p_CTCP.
  //!param: CTSP - Input value for p_CTSP.
  //!param: STCP - Input value for p_STCP.
  //!param: STSP - Input value for p_STSP.
  //!param: C - Input value for p_C.
  // This defines the cost function as defined above.
  AAMPoseEstimateCostFunctionBodyC::AAMPoseEstimateCostFunctionBodyC (const ParametersC &parameters, const VectorC &C0, const VectorC &CT, const VectorC &ST, const VectorC &CP, const VectorC &SP, const VectorC &CTCP, const VectorC &CTSP, const VectorC &STCP, const VectorC &STSP, const VectorC &C)
    :CostBodyC(parameters),
     p_C0(C0),
     p_CT(CT),
     p_ST(ST),
     p_CP(CP),
     p_SP(SP),
     p_CTCP(CTCP),
     p_CTSP(CTSP),
     p_STCP(STCP),
     p_STSP(STSP),
     p_C(C)
  {
  }

  //: Evaluate cost function at pose vector 'X'.
  //  'X = (theta, phi)' is a two-dimensional vector containing the pan angle 'theta' and the tilt angle 'phi' in radians.
  RealT AAMPoseEstimateCostFunctionBodyC::Cost (const VectorC &X) const
  {

    PairC<RealT> angles(X[0],X[1]);

    // residual
    VectorC r = p_C - (p_C0+p_CT*Cos(angles.Data1())+p_ST*Sin(angles.Data1())+p_CP*Cos(angles.Data2())+p_SP*Sin(angles.Data2())+p_CTCP*Cos(angles.Data1())*Cos(angles.Data2())+p_CTSP*Cos(angles.Data1())*Sin(angles.Data2())+p_STCP*Sin(angles.Data1())*Cos(angles.Data2())+p_STSP*Sin(angles.Data1())*Sin(angles.Data2()));

    return r.SumOfSqr();
  }

  //: Evaluate cost function at pose vector 'X'.
  //  'X = (theta, phi)' is a two-dimensional vector containing the pan angle 'theta' and the tilt angle 'phi' in radians.
  VectorC AAMPoseEstimateCostFunctionBodyC::Apply(const VectorC &X) const {
    VectorC ret(1);
    ret[0] = Cost(X);
    return ret;
  }

}
