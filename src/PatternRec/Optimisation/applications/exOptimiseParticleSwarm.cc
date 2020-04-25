// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=Optimisation
//! file="Ravl/PatternRec/Optimise/testBrent.cc"

#include "Ravl/Option.hh"
#include "Ravl/PatternRec/DistanceEuclidean.hh"
#include "Ravl/PatternRec/CostFunction.hh"
#include "Ravl/PatternRec/OptimiseParticleSwarm.hh"
#include "Ravl/PatternRec/TestFunctions.hh"
#include "Ravl/Plot/Plot2d.hh"

using namespace RavlN;



int main(int nargs, char **argv)
{

  // Get command-line options
  OptionC opt(nargs, argv);
  IntT n = opt.Int("n", 100, "Number of particles in your swarm.");
  StringC testFunc = opt.String("func", "Ackley", "Test function to optimise (Sphere, Ackely or Greiwank)");
  RealT omega = opt.Real("omega", 0.5, "Omega value");
  RealT phiP = opt.Real("p", 0.2, "phiP value");
  RealT phiG = opt.Real("g", 0.2, "phiG value");
  RealT maxIter = opt.Int("iter", 100, "Max iterations");
  RealT term = opt.Real("term", 1e-6, "When to terminate your criterion");
  UIntT d = opt.Int("d", 2, "Number of dimensions for test problem");
  RealT minBound = opt.Real("min", -20, "Minimum bound set for direction");
  RealT maxBound = opt.Real("max", 20, "Maximum bound set for direction");
  UIntT numberOfThreads = opt.Int("threads", 1, "Number of threads");
  opt.Check();

  SysLogOpen("exOptimiseParticleSwarm",false,true,true,-1,true);

  /*
   * This is our test function we wish to minimise.  They all
   * have minimums at [0, 0, ..... 0].
   */
  FunctionC func;
  if(testFunc == "Ackley") {
    func = FunctionAckleyC(d);
  } else if(testFunc == "Sphere") {
    func = FunctionSphereC(d);
  } else if(testFunc == "Griewank") {
    func = FunctionGriewankC(d);
  } else {
    RavlError("Unknown function!");
    return 1;
  }

  /*
   * Set bounds
   */
  VectorC minP(d);
  minP.Fill(minBound);
  VectorC maxP(d);
  maxP.Fill(maxBound);

  /*
   * We can plot the function if we have
   */
  if (d == 2) {
    Plot2dC::RefT plot = CreatePlot2d("Test Function");
    plot->Plot(func, minP, maxP);
  }
  /*
   * Set-up our cost function
   */

  SArray1dC<IntT> steps(d); // how many steps to do between min/max in each dimension...
  steps.Fill(100);
  VectorC startX(d); // not used by PSO
  startX.Fill(0.0);
  ParametersC parameters(minP, maxP, steps);
  parameters.SetConstP(startX);

  VectorC targetX(1); // known minimum for above functions
  targetX.Fill(0.0);
  CostC cost = CostFunctionC(parameters, targetX, func, DistanceEuclideanC());

  OptimiseParticleSwarmC optimiseParticleSwarm(n, omega, phiP, phiG, maxIter, term, numberOfThreads);
  VectorC X = optimiseParticleSwarm.MinimalX(cost);
  RavlInfo("PSO Solution '%s' End Cost %0.4f ", StringOf(X).data(), cost.Cost(X));
  return 0;
}
