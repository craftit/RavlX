#include "Ravl/Plot/GnuPlot.hh"
#include "Ravl/Array1dIter.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/IO.hh"
#include "Ravl/SysLog.hh"
//! lib=RavlGnuPlot

namespace RavlGUIN {

  GnuPlotC::GnuPlotC(IntT num) :
      plots(num), m_legendTitles(num), xrange(0, 0), yrange(0, 0)
  {
    n = num;
    SetTerminalWindow();
    Size(1.0);
    Title("GnuPlot");
    Xlabel("x-label");
    Ylabel("y-label");
    With("lines");
  }

  GnuPlotC::GnuPlotC(const StringC & title, const StringC & xlabel, const StringC & ylabel, const Array1dC<StringC> & subPlotTitles) :
      plots(subPlotTitles.Size()), m_legendTitles(subPlotTitles), xrange(0, 0), yrange(0, 0)
  {
    n = plots.Size();
    SetTerminalWindow();
    Size(1.0);
    Title(title);
    Xlabel(xlabel);
    Ylabel(ylabel);
    With("points");
  }

  void GnuPlotC::Set(const StringC & com, const StringC &val)
  {
    list.Insert(com, val);
  }

  void GnuPlotC::AddPoint(IntT i, const Point2dC &pt)
  {
    if (i > n - 1) {
      RavlIssueError("adding point to not numbered plot\n");
    }
    plots[i].InsLast(pt);
  }

  void GnuPlotC::AddPoint(IntT i, RealT x, RealT y)
  {
    if (i > n - 1) {
      RavlIssueError("adding point to not numbered plot\n");
    }
    Point2dC pt(x, y);
    plots[i].InsLast(pt);
  }

  bool GnuPlotC::AddLegendTitle(IntT i, const StringC & legendTitle)
  {
    if (i > m_legendTitles.Size() - 1) {
      return false;
    }
    m_legendTitles[i] = legendTitle;
    return true;
  }

  bool GnuPlotC::SetAxisRange(RealT xlo, RealT xhi, RealT ylo, RealT yhi)
  {

    // FIXME: Do some checking
    xrange.X() = xlo;
    xrange.Y() = xhi;
    yrange.X() = ylo;
    yrange.Y() = yhi;
    return true;
  }

  bool GnuPlotC::Plot(const StringC & outfile) const
  {

    // check we have valid data
    if (plots[0].Size() == 0) {
      RavlIssueWarning("bool GnuPlotC::Plot() const: No points defined");
      return false;
    }

    FilenameC tmpGnu = "/tmp/gnu";
    tmpGnu = tmpGnu.MkTemp();
    Array1dC<StringC> names(plots.Size());
    {

      OStreamC ofsGnu(tmpGnu);

      //: Print out the data
      for (Array1dIterC<PlotT> it(plots); it; it++) {
        FilenameC tmpData = "/tmp/data" + StringC(it.Index().V());
        tmpData = tmpData.MkTemp();
        names[it.Index()] = tmpData;
        OStreamC ofsData(tmpData);
        for (DLIterC<Point2dC> pt(*it); pt.IsElm(); pt.Next())
          ofsData << *pt << endl;
      }

      //: Print out the commands
      ofsGnu << "set title \'" << list["title"] << "\'" << endl;
      ofsGnu << "set xlabel \'" << list["xlabel"] << "\'" << endl;
      ofsGnu << "set ylabel \'" << list["ylabel"] << "\'" << endl;
      if (!outfile.IsEmpty())
        ofsGnu << "set output \'" << outfile << "\'" << endl;
      ofsGnu << "set size " << list["size"] << endl;
      ofsGnu << "set terminal " << list["terminal"] << endl;
      ofsGnu << "set grid" << endl;
      ofsGnu << "plot ";

      //: output ranges
#if 0
      if ((xrange.X() == 0.0) && (xrange.Y() == 0.0))
      ofsGnu << "[]";
      else
      ofsGnu << "[" << xrange.X() << ":" << xrange.Y() << "]";
      if ((yrange.X() == 0.0) && (yrange.Y() == 0.0))
      ofsGnu << "[]";
      else
      ofsGnu << "[" << yrange.X() << ":" << yrange.Y() << "] ";
#endif

      StringC with = list["with"];
      bool first = true;
      for (Array1dIterC<PlotT> it(plots); it; it++) {

        // check for empty data
        if (it.Data().IsEmpty()) {
          continue;
        }

        // Add a comma if it is not first
        if (!first) {
          ofsGnu << ",";
        }

        FilenameC tmpData = names[it.Index()];
        // If we are plotting multiple graphs with points
        // then let change the shape for each plot
        StringC localWith = with;
        if (with == "points") {
          localWith = "points pointtype " + (StringC) (it.Index() + 1);
        }
        ofsGnu << "\'" << tmpData << "\' with " << localWith;
        // Put in a nice title for the legend
        ofsGnu << " title \"" << m_legendTitles[it.Index()] << "\"";
        first = false;
      }
    }

    //: Run gnuplot
    StringC com = "gnuplot " + tmpGnu;
    int sysRet = system(com);
    bool ok = true;
    if(sysRet < 0) {
      RavlError("Failed to call gnuplot. ");
      ok = false;
    }
    Sleep(1.0); // give it time to run
    //: Then remove tmp files
    if (!tmpGnu.Remove()) {
      return false;
    }
    for (Array1dIterC<PlotT> it(plots); it; it++) {
      FilenameC tmpData = names[it.Index()];
      if (!tmpData.Remove())
        return false;
    }
    //: return success
    return ok;
  }

  bool GnuPlotC::MultiPlot(const StringC & outfile) const
  {

    if (plots[0].Size() == 0) {
      RavlIssueWarning("bool GnuPlotC::Plot() const: No points defined");
      return false;
    }

    // Sort out how to display plots
    UIntT rows = 3;
    UIntT cols = 3;
    if(plots.Size() == 1) {
      rows = 1;
      cols = 1;
    }
    else if (plots.Size() == 2) {
      rows = 1;
      cols = 2;
    } else if (plots.Size() == 3) {
      rows = 1;
      cols = 3;
    } else if (plots.Size() == 4) {
      rows = 2;
      cols = 2;
    } else if (plots.Size() == 5 || plots.Size() == 6) {
      rows = 2;
      cols = 3;
    }

    FilenameC tmpGnu = "/tmp/gnu";
    tmpGnu = tmpGnu.MkTemp();
    FilenameC pngFile = "/tmp/plot.png";
    pngFile = pngFile.MkTemp();
    Array1dC<StringC> names(plots.Size());
    {
      OStreamC ofsGnu(tmpGnu);

      //: Print out the data
      for (Array1dIterC<PlotT> it(plots); it; it++) {
        FilenameC tmpData = "/tmp/data" + StringC(it.Index().V());
        tmpData = tmpData.MkTemp();
        names[it.Index()] = tmpData;
        OStreamC ofsData(tmpData);
        for (DLIterC<Point2dC> pt(*it); pt.IsElm(); pt.Next())
          ofsData << *pt << endl;
      }

      //: Print out the commands
      ofsGnu << "set xlabel \'" << list["xlabel"] << "\'" << endl;
      ofsGnu << "set ylabel \'" << list["ylabel"] << "\'" << endl;
      ofsGnu << "set output \'" << pngFile << "\'" << endl;
      ofsGnu << "set size " << list["size"] << endl;
      ofsGnu << "set terminal " << list["terminal"] << endl;
      if (!outfile.IsEmpty())
        ofsGnu << "set output \'" << outfile << "\'" << endl;

      ofsGnu << "set grid" << endl;
      ofsGnu << "set size 1, 1" << endl;
      ofsGnu << "set origin 0,0" << endl;
      ofsGnu << "set multiplot layout " << rows << "," << cols << " title \'" << list["title"] << "\' columnsfirst scale 1.1,0.9" << endl;

      StringC with = list["with"];

      for (Array1dIterC<PlotT> it(plots); it; it++) {
        if (it.Data().IsEmpty()) {
          continue;
        }

        // Set title based on legend title
        ofsGnu << "set title \'" << m_legendTitles[it.Index()] << "\'" << endl;

        FilenameC tmpData = names[it.Index()];
        // If we are plotting multiple graphs with points
        // then let change the shape for each plot
        StringC localWith = with;
        if (with == "points") {
          localWith = "points pointtype " + (StringC) (it.Index() + 1);
        }

        ofsGnu << "plot ";
        //: output ranges
        if ((xrange.X() == 0.0) && (xrange.Y() == 0.0))
          ofsGnu << "[]";
        else
          ofsGnu << "[" << xrange.X() << ":" << xrange.Y() << "]";
        if ((yrange.X() == 0.0) && (yrange.Y() == 0.0))
          ofsGnu << "[]";
        else
          ofsGnu << "[" << yrange.X() << ":" << yrange.Y() << "] ";
        ofsGnu << " \'" << tmpData << "\' with " << localWith << " title \'" << m_legendTitles[it.Index()] << "\'" << endl;
      }
      ofsGnu << "unset multiplot" << endl;
    }

    //: Run gnuplot
    StringC com = "gnuplot " + tmpGnu;
    int sysRet = system(com);
    bool ok= true;
    if(sysRet < 0) {
      RavlError("Failed to call gnuplot.");
      ok= false;
    }
    Sleep(1.0); // give it time to run...

    //: Then remove tmp files
    if (!tmpGnu.Remove()) {
      return false;
    }

    //: Then remove tmp files
    if (!pngFile.Remove()) {
      return false;
    }

    for (Array1dIterC<PlotT> it(plots); it; it++) {
      FilenameC tmpData = names[it.Index()];
      if (!tmpData.Remove())
        return false;
    }

    //: return success
    return ok;
  }

  void GnuPlotC::Terminal(const StringC &term)
  {
    Set("terminal", term);
  }

// Get an image of the plot
  ImageC<ByteRGBValueC> GnuPlotC::Image(UIntT rows, UIntT cols)
  {

    // Sort out the terminal
    SetTerminalImage(rows,cols);

    // Make the temporary png file
    FilenameC pngFile = "/tmp/plot.png";
    pngFile = pngFile.MkTemp();

    // Do the plot
    ImageC<ByteRGBValueC> image;
    if (!Plot(pngFile)) {
      return image;
    }

    // Load image
    if (!RavlN::Load(pngFile, image)) {
      return image;
    }

    // Remove png file
    pngFile.Remove();

    SetTerminalWindow();

    // and return
    return image;
  }

  //! Set the terminal to a window.
  void GnuPlotC::SetTerminalWindow()
  {
    // Reset terminal to x11
#if RAVL_OS_MACOSX
    Terminal("x11 persist noraise "); // raise
#else
    Terminal("wxt persist noraise "); // raise
#endif
  }

  //! Set teriminal to a png
  void GnuPlotC::SetTerminalImage(int rows,int cols)
  {
    StringC term;
    term.form("png size %d, %d", cols, rows);
    Terminal(term);
  }


  ImageC<ByteRGBValueC> GnuPlotC::MultiPlotImage()
  {

    // Sort out a decent image size based on number of plots
    UIntT imageRows = 1000;
    UIntT imageCols = 1000;
    if (plots.Size() == 2) {
      imageRows = 500;
      imageCols = 1000;
    } else if (plots.Size() == 3) {
      imageRows = 500;
      imageCols = 1000;
    } else if (plots.Size() == 4) {
      imageRows = 1000;
      imageCols = 1000;
    } else if (plots.Size() == 5 || plots.Size() == 6) {
      imageRows = 666;
      imageCols = 1000;
    } else {
      imageRows = 1000;
      imageCols = 1000;
    }

    // Sort out the terminal
    StringC term;
    term.form("png size %d, %d", imageCols, imageRows);
    Terminal(term);

    // Make the temporary png file
    FilenameC pngFile = "/tmp/plot.png";
    pngFile = pngFile.MkTemp();

    // Do the plot
    ImageC<ByteRGBValueC> image;
    if (!MultiPlot(pngFile)) {
      return image;
    }

    // Load image
    if (!RavlN::Load(pngFile, image)) {
      return image;
    }

    // Remove png file
    pngFile.Remove();

    // Reset terminal to x11
    SetTerminalWindow();

    return image;
  }

  // Plot a RAVL function
  bool PlotFunction(const FunctionC & function, RealT min, RealT max, RealT step)
  {
    GnuPlotC plot(function.OutputSize());
    std::cerr << function.OutputSize() << std::endl;
    for(RealT x=min;x<=max;x+=step) {
      VectorC xvec(function.InputSize());
      xvec.Fill(x);
      VectorC y=function.Apply(xvec);
      for(SArray1dIterC<RealT>it(y);it;it++) {
        Point2dC pt(x, *it);
        plot.AddPoint(it.Index().V(), pt);
      }
    }
    plot.MultiPlot();
    return true;
  }


  std::ostream &operator<<(std::ostream & s, const GnuPlotC & out)
  {
    // output your class members

    return s;
  }

  std::istream &operator>>(std::istream & s, GnuPlotC & in)
  {
    // input your class members
    return s;
  }

}
