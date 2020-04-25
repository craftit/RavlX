// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLMATH_RANSAC_HEADER
#define RAVLMATH_RANSAC_HEADER 1
//! userlevel=Normal
//! author="Phil McLauchlan"
//! date="24/7/2002"
//! rcsid="$Id$"
//! docentry="Ravl.API.Pattern Recognition.Optimisation2"
//! example="OrthogonalRegressionTest.cc Homography2dFitTest.cc"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/Ransac.hh"

#include "Ravl/StoredState.hh"
#include "Ravl/ObservationManager.hh"
#include "Ravl/FitToSample.hh"
#include "Ravl/EvaluateSolution.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //! autoLink=on
  //: RANSAC algorithm class
  // This is a generic implementation of RANSAC, which fits a model to data.
  // The assumption is that the data are corrupted by outlers.
  // RANSAC takes repeated samples, and for each sample computes a candidate
  // model. The samples are usually (but not always) chosen the minimum size
  // that allows the model parameters to be computed.
  // The model computed from the sample is then evaluates using some kind of
  // voting scheme over the whole set of observations. The model that gets
  // the best vote is chosen as the solution.
  // <p>
  // In Ravl, the model is represented by a StateVectorC class, and the
  // candidate observations by instances of the ObservationC class.
  // Once you have chosen from Ravl or derived yourself the relevant
  // sub-classes of these base-class that you need for your problem,
  // you should specify:
  // <ul>
  //   <li> An "observation manager" which defines how samples are taken from
  //        the set of candidate observations, and how to generate a list of
  //        candidate observations to be evaluated. To do this you will need
  //        to create an instance of the ObservationManagerC class.
  //        If all the candidate observations are available in memory as a
  //        single list, you can use the ObservationListManagerC class,
  //        which also provides a default random sampling scheme.
  //   <li> A method for fitting the model parameters to a sample of
  //        observations. This is defined by the FitToSampleC class, from
  //        which you should derive a class that fits your model to a list
  //        of observations.
  //   <li> An evaluator which computes a "vote" for a candidate solution,
  //        typically by summing votes from the individual observations.
  //        The base class for evaluators is EvaluateSolutionC.
  //        The EvaluateNumInliersC class provides an evaluator that counts
  //        the number of inlier observations given a threshold on the errors,
  //        which was the original RANSAC algorithm. Summing the log-likelihood
  //        function is the alternative MLESAC algorithm by Philip Torr,
  //        where the observations need to be provided with robust
  //        error distribution models.
  // </ul>
  //  <a href="../../../html/Optimisation.pdf"> More information about RANSAC </a>

  class RansacC
    : public StoredStateC
  {
  public:
    RansacC(ObservationManagerC &obsManager,
	    FitToSampleC &modelFitter,
	    EvaluateSolutionC &evaluator);
    //: Constructor.

    virtual ~RansacC(){}
    //: Virtual destructor,

    bool ProcessSample(UIntT minNumConstraints);
    //: Generate sample, compute vote and update best solution and vote
    // This creates and evaluates a single sample. Call ProcessSample()
    // multiple times to guarantee as far as possible that you get at least
    // one uncorrupted sample. The simplest way to run RANSAC is to call
    // ProcessSample() a fixed number of times.

  private:
    ObservationManagerC &obsManager; // observation manager
    FitToSampleC &modelFitter; // for fitting model parameters to samples
    EvaluateSolutionC &evaluator; // for evaluating the vote for a solution
    RealT highestVote; // best vote so far

  public:
    RealT GetHighestVote() const;
    //: Return the highes vote found so far 
  };
}


#endif
