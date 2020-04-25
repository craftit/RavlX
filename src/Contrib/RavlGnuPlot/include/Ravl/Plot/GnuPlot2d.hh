/*
 * Plot.hh
 *
 *  Created on: 10 Aug 2012
 *      Author: kieron
 */

#ifndef RAVLN_GNUPLOT2D_HH_
#define RAVLN_GNUPLOT2D_HH_

#include "Ravl/Plot/Plot2d.hh"
#include "Ravl/OS/ChildOSProcess.hh"

//! docentry="Ravl.API.Graphics.Plotting.GnuPlot"

namespace RavlN {
  
  /*
   * Perform some plotting using 'gnuplot'
   */
  class GnuPlot2dC : public Plot2dC
  {
  public:
    GnuPlot2dC(const StringC & title = "My Plot");
    //: Construct with a set number of plots

    using Plot2dC::Plot;

    virtual bool Plot(const SArray1dC<Point2dC> & points, const StringC & dataName = "");
    //: Plot all points on a graph

    virtual bool Plot(const RCHashC<StringC, CollectionC<Point2dC> > & data);
    //: Plot all points on same graph

    virtual bool Plot(const CollectionC<Tuple2C<StringC, CollectionC<Point2dC> > > & data);
    //: Plot all plots on same canvas, preserve order so we keep the same markers

    virtual bool Plot(const LineABC2dC & line);
    //: Plot a line

    virtual bool Plot(const StringC & function);
    //: Plot a function using a string, e.g. sin(x)

    virtual bool Plot(const DataSetVectorLabelC & dataSet, UIntT feature0 = 0, UIntT feature1 = 1, UIntT samplesPerClass = 0);
    //: Make a scatter plot of the data.  Only the first two dimensions will be used...

    virtual bool Plot(const ClassifierC & classifier, const DataSetVectorLabelC & dataSet, UIntT feature1=0, UIntT feature2=1);
    //: Plot right/wrong decisions by classifier

    virtual bool Plot(const FunctionC & function, const VectorC & min, const VectorC & max);
    //: Plot a function between min and max

    virtual bool SetXLabel(const StringC & xlabel);
    //: Set the x-label

    virtual bool SetYLabel(const StringC & ylabel);
    //: Set the y-label

    virtual bool SetXRange(const RealRangeC & xrange);
    //: Set the range of the x-axis

    virtual bool SetYRange(const RealRangeC & yrange);
    //: Set the range of the y-axis

    virtual bool SetLineStyle(const StringC & lineStyle);
    //: Set plot style

    virtual bool SetOutput(const StringC & output, const IndexRange2dC & rec = IndexRange2dC(750, 1000));
    //: Set output x11 or a filename


    virtual bool Command(const StringC & command);
    //: General method to send a command to the plotting library

    typedef RavlN::SmartPtrC<GnuPlot2dC> RefT;

  protected:

    //! Set the terminal to a window.
    void CommandTerminalWindow();

    void Flush();

    //! Create a temporary file.
    StringC SetupTmpFile();

    ChildOSProcessC m_gnuPlot; //< The GnuPlot process
    RealRangeC m_xrange;
    RealRangeC m_yrange;
    FilenameC m_tmpFile;
    int m_plotNum;
  };

  Plot2dC::RefT CreatePlot2d_gnuplot(const StringC & name);
  //: The hook used to replace the Ravl EigenVectors function


} /* namespace RavlN */
#endif /* PLOT2D_HH_ */
