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
//! docentry="Ravl.API.Graphics.Plotting.GnuPlot"
//! userlevel=Normal

#include "Ravl/Plot/GnuPlot2d.hh"
#include "Ravl/Option.hh"
#include "Ravl/EntryPnt.hh"
#include "Ravl/StdConst.hh"
#include "Ravl/IO.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/Collection.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/Vector2d.hh"
#include "Ravl/Resource.hh"
#include "Ravl/PatternRec/DataSetIO.hh"
#include "Ravl/PatternRec/DesignDiscriminantFunction.hh"
#include "Ravl/PatternRec/DesignFuncLSQ.hh"
#include "Ravl/PatternRec/DesignClassifierNeuralNetwork2.hh"
#include "Ravl/PatternRec/TestFunctions.hh"

using namespace RavlN;

int exGnuPlot(int nargs, char *args[])
{

  // Get command-line options
  OptionC opt(nargs, args);

  opt.Check();

  // Plot a Sine and Cosine on same graph
  SArray1dC<CollectionC<Point2dC> > points(3);
  for (SArray1dIterC<CollectionC<Point2dC> > it(points); it; it++) {
    it.Data() = CollectionC<Point2dC>(25);
  }

  RCHashC<StringC, CollectionC<Point2dC> > allPoints;
  allPoints.Insert("sin(x)", points[0]);
  allPoints.Insert("cos(x)", points[1]);
  allPoints.Insert("err(x)", points[2]);

  for (RealT ang = -4.0 * RavlConstN::pi; ang <= 4.0 * RavlConstN::pi; ang += RavlConstN::pi / 32.0) {
    points[0].Append(Point2dC(ang, Sin(ang)));
    points[1].Append(Point2dC(ang, Cos(ang)));
    points[2].Append(Point2dC(ang, Sin(2. * Sin(2. * Sin(2. * Sin(ang))))));
  }

  GnuPlot2dC plot0("Sin(x)");
  plot0.SetLineStyle("points");

  //plot0.PlotFunction("sin(x)");
  //plot0.PlotFunction("cos(x)");
  plot0.SetXLabel("bananas");
  plot0.Plot(points[0].SArray1d(), "My Data");

  GnuPlot2dC plot0a("Sin(x) and Cos(x)");
  plot0a.SetLineStyle("lines");
  plot0a.SetXLabel("bananas");
  plot0a.Plot(allPoints);
  plot0a.SetOutput("graph.png");
  plot0a.Plot(allPoints);

  GnuPlot2dC plot1("Cos(x)");
  plot1.SetLineStyle("lines");
  plot1.Plot(points[1].SArray1d());

  /*
   * We can just use the command function to do it all as well..
   */
  GnuPlot2dC plot3("A pretty function!");
  plot3.Command("set size ratio -1");
  plot3.Command("set nokey");
  plot3.Command("set noxtics");
  plot3.Command("set noytics");
  plot3.Command("set noborder");
  plot3.Command("set parametric");
  plot3.Command("x(t) = (R-r)*cos(t) + p*cos((R-r)*t/r)");
  plot3.Command("y(t) = (R-r)*sin(t) - p*sin((R-r)*t/r)");
  plot3.Command("gcd(x,y) = (x%y==0 ? y : gcd(y,x%y))");
  plot3.Command("R = 100; r = -49; p = -66; res = 10");
  plot3.Command("rr = abs(r)");
  plot3.Command("nturns = rr / gcd(R,rr)");
  plot3.Command("samp = 1 + res * nturns");
  plot3.Command("set samples samp");
  plot3.Command("plot [t=0:nturns*2*pi] x(t),y(t)");

  /*
   * What about a data set
   */
  DataSetVectorLabelC dset = CreateDataSet();
  GnuPlot2dC scatterPlot("A scatter plot");
  scatterPlot.Plot(dset);

  DataSetVectorLabelC iris;
  if (!LoadDataSetVectorLabel(PROJECT_OUT "/share/Ravl/PatternRec/iris.csv", iris)) {
    RavlError("Trouble loading iris dataset");
    return 1;
  }

  GnuPlot2dC scatterPlot2("Iris Data");
  scatterPlot2.Plot(iris, 0, 1);
  iris.Sample1().Normalise(DATASET_NORMALISE_SCALE);
  GnuPlot2dC scatterPlot3("Scaled Iris Data");
  scatterPlot3.Plot(iris, 0, 1);

  LineABC2dC line(2, -1, 1); // i.e. y = 2x + 1
  GnuPlot2dC linePlot("y=2x + 1");
  linePlot.SetLineStyle("line");
  linePlot.SetXRange(RealRangeC(0, 1.0));
  linePlot.Plot(line);

  /*
   * We can just plot a function
   */
  GnuPlot2dC plot2("My Function");
  plot2.SetXRange(RealRangeC(0, 1.0));
  plot2.Plot((StringC)"2 * x + 1");

  FunctionGriewankC func(2);
  VectorC min(-200.0, -200.0);
  VectorC max(200.0, 200.0);
  GnuPlot2dC plotGriewank("Griewank Function");
  plotGriewank.Plot(func, min, max);

  return 0;
}

RAVL_ENTRY_POINT(exGnuPlot);
