// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=Optimisation
//! file="Ravl/PatternRec/Optimise/testBrent.cc"

#include "Ravl/PatternRec/OptimiseBrent.hh"
#include "Ravl/PatternRec/FuncOrthPolynomial.hh"
#include "Ravl/PatternRec/DistanceEuclidean.hh"
#include "Ravl/PatternRec/CostFunction.hh"
#include "Ravl/PatternRec/CostFunction1d.hh"
#include "Ravl/PatternRec/OptimisePowell.hh"
#include "Ravl/PatternRec/OptimiseRandomUniform.hh"
#include "Ravl/PatternRec/BracketMinimum.hh"
#include "Ravl/PatternRec/TestFunctions.hh"
#include "Ravl/PatternRec/OptimiseConjugateGradient.hh"
#include "Ravl/PatternRec/OptimiseLevenbergMarquardt.hh"

using namespace RavlN;

int main()
{

  /*
   * This is our test function we wish to minimise
   */
  //FunctionAckleyC func(2); // Complicated function with many local minima
  //FunctionSphereC func(2); // Easy bowl looking function
  FunctionGriewankC func(2); // Pretty evil looking function with plenty of local minima

  /*
   * Set-up our cost function
   */
  VectorC minP(-30.0, -30.0);
  VectorC maxP(30.0, 30.0);
  SArray1dC<IntT> steps(2); // how many steps to do between min/max in each dimension...
  steps.Fill(100);
  VectorC startX(30.0, 30.0); // where to start the search
  ParametersC parameters(minP, maxP, steps);
  parameters.SetConstP(startX);
  VectorC targetX(0.0, 0.0); // known minimum for above functions
  CostC cost = CostFunctionC(parameters, targetX, func, DistanceEuclideanC());
  RavlInfo("Start Cost %0.4f", func.Apply(startX)[0]);
  RavlInfo("Target Cost %0.4f", func.Apply(targetX)[0]);

  /*
   * Always good to do a bit of a random search first to get good starting point
   */
  OptimiseRandomUniformC optimiseRandom(1000);
  VectorC X = optimiseRandom.MinimalX(cost);
  RavlInfo("Random Solution [%0.2f, %0.2f] End Cost %0.2f ", X[0], X[1], cost.Cost(X));

  /*
   * Now lets apply other optimiser
   */
  //OptimiseLevenbergMarquardtC optimise(1000);
  //OptimiseConjugateGradientC optimise(10000);
  OptimisePowellC optimise(10, 1e-6);

  parameters.SetConstP(X); // set the start position as the best found by random search
  X = optimise.MinimalX(cost);
  RavlInfo("Solution [%0.2f, %0.2f] End Cost %0.2f ", X[0], X[1], cost.Cost(X));

  return 0;
}
