// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2006, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef GNUPLOT_HH
#define GNUPLOT_HH
////////////////////////////////////////////////////////////////////////////
//! author="Kieron Messer, Bill Christmas"
//! date=""
//! userlevel=Normal
//! docentry="Ravl.API.Graphics.Plotting.GnuPlot"
//! file=""
//! rcsid="$id$"
//! lib=RavlGnuPlot

#include "Ravl/RCHash.hh"
#include "Ravl/String.hh"
#include "Ravl/DList.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/Point2d.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/Stream.hh"
#include "Ravl/Array1d.hh"
#include "Ravl/Vector2d.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/PatternRec/Function.hh"

// --------------------------------------------------------------------------
// **********  GnuPlotC  **************************************************
// --------------------------------------------------------------------------
//: DEPRACTED: GnuPlot2dC preferred.
//

namespace RavlGUIN {

  using namespace RavlN;
  using namespace RavlImageN;

  typedef DListC<Point2dC> PlotT;

  class GnuPlotC
  {

  public:
    //:-----------------------------------------------

    GnuPlotC(IntT num = 1);
    //: Constructor on: number of plots required; name of output file

    GnuPlotC(const StringC & title, const StringC & xlabel, const StringC & ylabel, const Array1dC<StringC> & subPlotTitles);
    //: Construct with tile, axis labels and subplot titles

    //:-------
    //: Methods
    void Set(const StringC &command, const StringC &value);
    //: General method to invoke any GnuPlot command

    void AddPoint(IntT n, const Point2dC &pt);
    //: Add a point to plot n

    void AddPoint(IntT n, RealT x, RealT y);
    //: Add a point to plot n

    bool AddLegendTitle(IntT n, const StringC & legendTitle);
    //: Add a legend title to a plot

    bool Plot(const StringC & outputFile = "") const;
    //: Plot all the current graphs on one single plot

    ImageC<ByteRGBValueC> Image(UIntT rows = 500, UIntT cols = 750);
    //: Plot all the current graphs on one single plot and return image

    bool MultiPlot(const StringC & outputFile = "") const;
    //: Plot the current graphs as separate plots on same canvas

    ImageC<ByteRGBValueC> MultiPlotImage();
    //: Plot the current graphs as a multi-plot and return the image

    void Terminal(const StringC &term);
    //: Set the terminal parameters

    void Title(const StringC &title)
    {
      Set("title", title);
    }
    //: Set the graph title

    void Xlabel(const StringC &xlabel)
    {
      Set("xlabel", xlabel);
    }
    //: Set the x-label axis

    void Ylabel(const StringC &ylabel)
    {
      Set("ylabel", ylabel);
    }
    //: Set the y-label axis

    void Xlo(RealT lo)
    {
      xrange.X() = lo;
    }
    //: Set the x-axis lower limit

    void Xhi(RealT hi)
    {
      xrange.Y() = hi;
    }
    //: Set the x-axis upper limit

    void Ylo(RealT lo)
    {
      yrange.X() = lo;
    }
    //: Set the y-axis lower limit

    void Yhi(RealT hi)
    {
      yrange.Y() = hi;
    }
    //: Set the y-axis upper limit

    bool SetAxisRange(RealT xlo, RealT xhi, RealT ylo, RealT yhi);
    // Set the range of the axis

    void Size(const RealT sz)
    {
      Set("size", (StringC) sz);
    }
    //: Set display size <i>relative to</i> default size

    void Size(RealT vsz, RealT hsz)
    {
      Set("size", StringC(vsz) + ", " + StringC(hsz));
    }
    //: Set display size <i>relative to</i> default size

    void With(const StringC &with)
    {
      Set("with", with);
    }
    //: Set the plotting style
    // Style must be one of:<br>
    // <code> lines points linespoints impulses dots steps fsteps histeps errorbars labels xerrorbars yerrorbars xyerrorbars errorlines xerrorlines yerrorlines xyerrorlines boxes histograms filledcurves boxerrorbars boxxyerrorbars financebars candlesticks vectors image rgbimage pm3d </code> <br>
    // (See <a href="http://www.gnuplot.info/docs/gnuplot.pdf">manual</a> for more info)

    //:-------
    //: Friends
    friend ostream &operator<<(ostream &s, const GnuPlotC &out);
    //: output stream operator
    friend istream &operator>>(istream &s, GnuPlotC &in);
    //: input stream operator

  protected:
    //! Set the terminal to a window.
    void SetTerminalWindow();

    //! Set teriminal to a png
    void SetTerminalImage(int rows,int cols);

    //: Class members
    RCHashC<StringC, StringC> list;
    //: Holding gnuplots commands
    Array1dC<PlotT> plots;
    //: Holds points of graph
    Array1dC<StringC> m_legendTitles;
    IntT n;
    //: How many plots on same graph
    Vector2dC xrange;
    //: Store the xrange
    Vector2dC yrange;
    //: Store the yrange

    StringC m_windowType;
  };

  // Plot a RAVL function
  bool PlotFunction(const FunctionC & function, RealT min, RealT max, RealT step);

}

#endif
