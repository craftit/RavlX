// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLMATH_EVALUATESOLUTION_HEADER
#define RAVLMATH_EVALUATESOLUTION_HEADER 1
//! userlevel=Normal
//! author="Phil McLauchlan"
//! date="24/7/2002"
//! rcsid="$Id$"
//! docentry="Ravl.API.Pattern Recognition.Optimisation2"
//! example="OrthogonalRegressionTest.cc Homography2dFitTest.cc QuadraticFitTest.cc"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/EvaluateSolution.hh"

#include "Ravl/RefCounter.hh"
#include "Ravl/StateVector.hh"
#include "Ravl/Observation.hh"
#include "Ravl/DList.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Solution evaluation class
  // Body class for evaluating a proposed solution state vector
  class EvaluateSolutionBodyC
    : public RCBodyVC
  {
  public:
    EvaluateSolutionBodyC()
    {}
    //: Default constructor.

    virtual RealT SolutionScore(const StateVectorC &stateVec,
				const DListC<ObservationC> &obsList) const;
    //: Returns the score/vote/likelihood for the given state parameters

    virtual DListC<ObservationC> CompatibleObservations(
					const StateVectorC &stateVec,
					const DListC<ObservationC> &obsList) const;
    //: Returns list of observations compatible with the given state parameters

    virtual DListC<bool> ObservationCompatibility(
					const StateVectorC &stateVec,
					const DListC<ObservationC> &obsList) const;
    //: Returns  list of booleans indicating which observations are compatible with the given state parameters
  };

  //! userlevel=Normal
  //! autoLink=on
  //: Solution evaluation class
  // Class for evaluating a proposed solution state vector
  class EvaluateSolutionC
    : public RCHandleC<EvaluateSolutionBodyC>
  {
  public:    
    EvaluateSolutionC()
    {}
    //: Default constructor.
    // Creates an invalid handle.

  protected:    
    EvaluateSolutionC(EvaluateSolutionBodyC &bod)
      : RCHandleC<EvaluateSolutionBodyC>(bod)
    {}
    //: Body constructor.
    
    EvaluateSolutionC(const EvaluateSolutionBodyC *bod)
      : RCHandleC<EvaluateSolutionBodyC>(bod)
    {}
    //: Body constructor.
    
    EvaluateSolutionBodyC &Body()
    { return RCHandleC<EvaluateSolutionBodyC>::Body(); }
    //: Access body.

    const EvaluateSolutionBodyC &Body() const
    { return RCHandleC<EvaluateSolutionBodyC>::Body(); }
    //: Access body.
    
  public:
    RealT SolutionScore(const StateVectorC &stateVec,
			const DListC<ObservationC> &obsList) const
    { return Body().SolutionScore(stateVec,obsList); }
    //: Returns the score/vote/likelihood for the given state parameters

    DListC<ObservationC> CompatibleObservations(const StateVectorC &stateVec,
						const DListC<ObservationC> &obsList) const
    { return Body().CompatibleObservations(stateVec,obsList); }
    //: Returns list of observations compatible with the given state parameters

    DListC<bool> ObservationCompatibility(const StateVectorC &stateVec,
                                          const DListC<ObservationC> &obsList) const
    { return Body().ObservationCompatibility(stateVec,obsList); }
    //: Returns  list of booleans indicating which observations are compatible with the given state parameters
    // Thus the number of true entries in the list corresponds to the size of the CompatibleObservations list.

  };

}

#endif
