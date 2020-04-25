// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLMATH_EVALUATELIKELIHOOD_HEADER
#define RAVLMATH_EVALUATELIKELIHOOD_HEADER 1
//! userlevel=Normal
//! author="Phil McLauchlan"
//! date="24/7/2002"
//! rcsid="$Id$"
//! docentry="Ravl.API.Pattern Recognition.Optimisation2"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/EvaluateLikelihood.hh"

#include "Ravl/EvaluateSolution.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Solution evaluation body class
  class EvaluateLikelihoodBodyC
    : public EvaluateSolutionBodyC
  {
  public:
    EvaluateLikelihoodBodyC()
    {}
    //: Default constructor

    EvaluateLikelihoodBodyC(RealT chi2Thres);
    //: Constructor.

    virtual RealT SolutionScore(const StateVectorC &stateVec,
				const DListC<ObservationC> &obsList) const;
    //: Returns the log likelihood for the given state parameters

    DListC<ObservationC> CompatibleObservations(
					const StateVectorC &stateVec,
					const DListC<ObservationC> &obsList) const;
    //: Returns the observations compatible with the given state parameters


  private:
    RealT chi2Thres; // Threshold on normalised residual
  };

  //! userlevel=Normal
  //! autoLink=on
  //: Solution evaluation class
  // This class evaluates a proposed solution state vector by summing the
  // log-likelihoods of each observation
  class EvaluateLikelihoodC
    : public EvaluateSolutionC
  {
  public:
    
    EvaluateLikelihoodC(RealT chi2Thres=0.0)
      : EvaluateSolutionC(*new EvaluateLikelihoodBodyC(chi2Thres))
    {}
    //: Default constructor
    // The chi2Thres parameter is used as a threshold on the non-robust
    // negative log-likelihood when selecting observations compatible with
    // the best model. If you only want the model parameters and don't care
    // about the selected obsrvations, pass an empty argument list.
    
    EvaluateLikelihoodC(const EvaluateSolutionC &evaluator)
      : EvaluateSolutionC(dynamic_cast<const EvaluateLikelihoodBodyC *>(BodyPtr(evaluator)))
    {}
    //: Base class constructor.
    
  public:
    EvaluateLikelihoodC(EvaluateLikelihoodBodyC &bod)
      : EvaluateSolutionC(bod)
    {}
    //: Body constructor.
    
    EvaluateLikelihoodBodyC &Body()
    { return static_cast<EvaluateLikelihoodBodyC &>(EvaluateSolutionC::Body()); }
    //: Access body.

    const EvaluateLikelihoodBodyC &Body() const
    { return static_cast<const EvaluateLikelihoodBodyC &>(EvaluateSolutionC::Body()); }
    //: Access body.
  };
}

#endif
