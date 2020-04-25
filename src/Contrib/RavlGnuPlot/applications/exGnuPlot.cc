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

#include "Ravl/Plot/GnuPlot.hh"
#include "Ravl/Option.hh"
#include "Ravl/EntryPnt.hh"
#include "Ravl/StdConst.hh"
#include "Ravl/IO.hh"

using namespace RavlN;
using namespace RavlGUIN;
using namespace RavlImageN;

int exGnuPlot(int nargs, char *args[])
{

  // Get command-line options
  OptionC opt(nargs, args);

  opt.Check();

  // Plot a Sine and Cosine on same graph
  GnuPlotC plot(4);
  for (RealT ang = -4.0 * RavlConstN::pi; ang <= 4.0 * RavlConstN::pi; ang += RavlConstN::pi / 64.0) {
    Point2dC pt0(ang, Sin(ang));
    plot.AddPoint(0, pt0);
    Point2dC pt1(ang, Cos(ang));
    plot.AddPoint(1, pt1);
    Point2dC pt2(ang, Sin(2. * Sin(2. * Sin(2. * Sin(ang)))));
    plot.AddPoint(2, pt2);
    Point2dC pt3(ang, Sin(ang) / ang);
    plot.AddPoint(3, pt3);
  }
  plot.Title("Some Sine and Cosine Plots");
  plot.Xlabel("Radians");
  plot.Ylabel("");
  plot.AddLegendTitle(0, "Sin(x)");
  plot.AddLegendTitle(1, "Cos(x)");
  plot.AddLegendTitle(2, "Sin(2 Sin(2 Sin(2 Sin(x))))");
  plot.AddLegendTitle(3, "Sin(x)/x");

  // This should open an GnuPlot window and display all the graphs on 1 plot
  plot.Plot();

  // This should open an GnuPlot window and display all graphs as separate plots
  plot.MultiPlot();

  // Lets get an image of the plot
  ImageC<ByteRGBValueC> plotImage = plot.Image();
  RavlN::Save("@X: Sine and Cosine plot", plotImage);

  // Lets get an image of the multi-plot
  ImageC<ByteRGBValueC> multiPlotImage = plot.MultiPlotImage();
  RavlN::Save("@X: MultiPlot", multiPlotImage);

  return 0;
}

RAVL_ENTRY_POINT(exGnuPlot);
