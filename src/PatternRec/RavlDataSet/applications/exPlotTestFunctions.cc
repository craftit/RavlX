// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, James Smith
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlURLIO
//! file="Ravl/Contrib/Plotting/GnuPlot/exGnuPlot.cc"
//! author="Kieron Messer"
//! docentry="Ravl.Contrib.Plotting.GnuPlot"
//! userlevel=Normal

#include "Ravl/Plot/Plot2d.hh"
#include "Ravl/Option.hh"
#include "Ravl/PatternRec/TestFunctions.hh"

using namespace RavlN;

/*
 * Some pretty functions.  Useful for testing Optimisers and the like....
 */

int main(int nargs, char *args[])
{

  // Get command-line options
  OptionC opt(nargs, args);
  opt.Check();

  FunctionSphereC funcSphere(2);
  VectorC minSphere(-5.12, -5.12);
  VectorC maxSphere(5.12, 5.12);
  Plot2dC::RefT plotSphere = CreatePlot2d("Sphere Function");
  plotSphere->Plot(funcSphere, minSphere, maxSphere);

  FunctionAckleyC funcAckley(2);
  VectorC minAckley(-30.0, -30.0);
  VectorC maxAckley(30.0, 30.0);
  Plot2dC::RefT plotAckley = CreatePlot2d("Ackley Function");
  plotAckley->Plot(funcAckley, minAckley, maxAckley);

  FunctionGriewankC funcGriewank(2);
  VectorC minGriewank(-200.0, -200.0);
  VectorC maxGriewank(200.0, 200.0);
  Plot2dC::RefT plotGriewank = CreatePlot2d("Griewank Function");
  plotGriewank->Plot(funcGriewank, minGriewank, maxGriewank);
  return 0;
}

