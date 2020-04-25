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
#include "Ravl/PatternRec/BracketMinimum.hh"

using namespace RavlN;

int main() {
  OptimiseBrentC optimiser (100, 1e-6); 
  FuncOrthPolynomialC quad (2,1,2);
  MatrixC coeffs(1,5);
  coeffs[0][0] = 0.0;
  coeffs[0][1] = 0.0;
  coeffs[0][2] = 0.0;
  coeffs[0][3] = 1.0;
  coeffs[0][4] = 1.0;
  quad.SetTransform(coeffs);
  VectorC minP(2);
  VectorC maxP(2);
  SArray1dC<IntT> steps(2);
  minP.Fill(0.0);
  maxP.Fill(0.0);
  steps.Fill(1);
  ParametersC parameters(minP,maxP,steps);
  parameters.SetConstP(VectorC(1.0,2.0));
  VectorC Yd(1);
  Yd[0] = 0.0;
  CostC cost = CostFunctionC(parameters, Yd, quad, DistanceEuclideanC());

  VectorC minP1d(1);
  VectorC maxP1d(1);
  SArray1dC<IntT> steps1d(1);
  minP1d.Fill(-1.0);
  maxP1d.Fill(1.0);
  steps1d.Fill(10);
  ParametersC parameters1d(minP1d, maxP1d, steps1d);
  VectorC point(-1.0, -1.0);
  VectorC direction(1.0, 1.0);
  CostFunction1dC cost1d (parameters1d, cost, point, direction);
  RealT minY;
  VectorC X(2);
  X.Fill(0.0);
  std::cout << quad(X) << std::endl;
  VectorC Y(1);
  Y[0] = 0;
  std::cout << cost1d.Cost(Y) << std::endl;
  BracketMinimum(cost1d);
  X = optimiser.MinimalX(cost1d, minY);
  std::cout << cost1d.Point(X) << std::endl;
  std::cout << minY << std::endl;

  OptimisePowellC powell (100, 1e-6);
  X = powell.MinimalX(cost, minY);
  std::cout << X << std::endl;
  std::cout << minY << std::endl;

  return 0;
}
