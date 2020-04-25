// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_AAMPOSEESTIMATECOSTFUNCTION_HEADER
#define RAVLIMAGE_AAMPOSEESTIMATECOSTFUNCTION_HEADER 1
//////////////////////////////////////////////////////////////////////////////
//! author =    "Jean-Yves Guillemaut"
//! lib=RavlAAM
//! date =      "01/03/2006"
//! rcsid="$Id: AAMPoseEstimateCostFunction.hh,v 1.3 2005/07/25 16:31:07 ees1wc Exp $"
//! file="Ravl/CompVision/ActiveAppearanceModels/AAMPoseEstimateCostFunction.hh"
//! userlevel = Normal
//! docentry =  "Ravl.API.Images.AAM"


#include "Ravl/PatternRec/Cost.hh"
#include "Ravl/PatternRec/Parameters.hh"
#include "Ravl/Vector.hh"

using namespace RavlN;

namespace RavlImageN {

  //! userlevel=Develop
  //: Cost function measuring the residual error between a given appearance and the appearance predicted by the statistical model of pose for a given pose value.
  // The cost function defined is:
  // <p><i>d(theta, phi) = || p_C-(p_C0 + p_CT cos(theta) + p_ST sin(theta) + p_CP cos(phi) + p_SP sin(phi) + p_CTCP cos(theta) cos(phi) + p_CTSP cos(theta) sin(phi) + p_STCP sin(theta) cos(phi) + p_STSP sin(theta) sin(phi) ||</i>,</p>
  // where 'p_C0', 'p_CT', 'p_ST', 'p_CP', 'p_SP', 'p_CTCP', 'p_CTSP', 'p_STCP', 'p_STSP' are vectors defining the statitical model of pose variation and 'p_C' is the parameter vector representing the given appearance. The pose is represented by the pan angle 'theta' and the tilt angle 'phi'; both values are expressed in radians.

  class AAMPoseEstimateCostFunctionBodyC: public CostBodyC
  {

  public:
    AAMPoseEstimateCostFunctionBodyC (const ParametersC &parameters, const VectorC &C0, const VectorC &CT, const VectorC &ST, const VectorC &CP, const VectorC &SP, const VectorC &CTCP, const VectorC &CTSP, const VectorC &STCP, const VectorC &STSP, const VectorC &C);
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

  protected:
    VectorC p_C0;
    VectorC p_CT;
    VectorC p_ST;
    VectorC p_CP;
    VectorC p_SP;
    VectorC p_CTCP;
    VectorC p_CTSP;
    VectorC p_STCP;
    VectorC p_STSP;
    VectorC p_C;

    virtual VectorC Apply (const VectorC &X) const;
    //: Evaluate cost function at pose vector 'X'.
    //  'X = (theta, phi)' is a two-dimensional vector containing the pan angle 'theta' and the tilt angle 'phi' in radians.

    virtual RealT Cost (const VectorC &X) const;
    //: Evaluate cost function at pose vector 'X'.
    //  'X = (theta, phi)' is a two-dimensional vector containing the pan angle 'theta' and the tilt angle 'phi' in radians.
  };


  //! userlevel=Advanced
  //: Cost function measuring the residual error between a given appearance and the appearance predicted by the statistical model of pose for a given pose value.
  // The cost function defined is:
  // <p><i>d(theta, phi) = || p_C-(p_C0 + p_CT cos(theta) + p_ST sin(theta) + p_CP cos(phi) + p_SP sin(phi) + p_CTCP cos(theta) cos(phi) + p_CTSP cos(theta) sin(phi) + p_STCP sin(theta) cos(phi) + p_STSP sin(theta) sin(phi) ||</i>,</p>
  // where 'p_C0', 'p_CT', 'p_ST', 'p_CP', 'p_SP', 'p_CTCP', 'p_CTSP', 'p_STCP', 'p_STSP' are vectors defining the statitical model of pose variation and 'p_C' is the parameter vector representing the given appearance. The pose is represented by the pan angle 'theta' and the tilt angle 'phi'; both values are expressed in radians.

  class AAMPoseEstimateCostFunctionC: public CostC
  {
  public:
    AAMPoseEstimateCostFunctionC (const ParametersC &parameters, const VectorC &C0, const VectorC &CT, const VectorC &ST, const VectorC &CP, const VectorC &SP, const VectorC &CTCP, const VectorC &CTSP, const VectorC &STCP, const VectorC &STSP, const VectorC &C)
      :CostC(*(new AAMPoseEstimateCostFunctionBodyC (parameters, C0, CT, ST, CP, SP, CTCP, CTSP, STCP, STSP, C))) {}
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
  };

}

#endif
