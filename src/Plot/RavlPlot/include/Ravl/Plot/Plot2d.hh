#ifndef RAVLN_PLOT2D_HH_
#define RAVLN_PLOT2D_HH_

//! docentry="Ravl.API.Graphics.Plotting"
//! lib=RavlPlot
//! author="Kieron Messer"

#include "Ravl/SmartPtr.hh"
#include "Ravl/String.hh"
#include "Ravl/Point2d.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/Collection.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/RealRange1d.hh"
#include "Ravl/PatternRec/DataSetVectorLabel.hh"
#include "Ravl/PatternRec/SampleVector.hh"
#include "Ravl/LineABC2d.hh"
#include "Ravl/Tuple2.hh"

namespace RavlN {

  class ClassifierC;
  class FunctionC;

  /*
   * Base class for plotting 2D points.  To do any plotting you will need
   * to use the GnuPlot2dC class.....
   */
  class Plot2dC : public RavlN::RCBodyVC
  {
  public:
    Plot2dC(const StringC & title = "My Plot");
    //: Construct with a set number of plots

    virtual bool Plot(const SArray1dC<Point2dC> & data, const StringC & dataName = "");
    //: Plot points, optional dataName will appear in the legend

    bool Plot(const SArray1dC<RealT> & data, const StringC & dataName = "");
    //: Plot values against their index, optional dataName will appear in the legend

    bool Plot(const SArray1dC<float> & data, const StringC & dataName = "");
    //: Plot values against their index, optional dataName will appear in the legend

    bool Plot(const SArray1dC<Point2dC> & data,
        RavlImageN::ImageC<RavlImageN::ByteRGBValueC> & image,
        const IndexRange2dC & rec = IndexRange2dC(750, 750),
        const StringC & dataName = "");
    //: Plot points and return image of plot.

    virtual bool Plot(const RCHashC<StringC, CollectionC<Point2dC> > & data);
    //: Plot all plots on same canvas

    virtual bool Plot(const CollectionC<Tuple2C<StringC, CollectionC<Point2dC> > > & data);
    //: Plot all plots on same canvas, preserve order so we keep the same markers

    bool Plot(const RCHashC<StringC, CollectionC<Point2dC> > & data,
        RavlImageN::ImageC<RavlImageN::ByteRGBValueC> & image,
        const IndexRange2dC & rec = IndexRange2dC(750, 750));
    //: Plot points and return image of plot

    virtual bool Plot(const LineABC2dC & line);
    //: Plot a straight line

    virtual bool Plot(const StringC & function);
    //: Plot a function using a string, e.g. sin(x)

    virtual bool Plot(const DataSetVectorLabelC & dataSet, UIntT feature1 = 0, UIntT feature2 = 1, UIntT samplesPerClass = 0);
    //: Make a scatter plot of the data

    bool Plot(const SampleVectorC & sample, UIntT feature1 = 0, UIntT feature2 = 1, UIntT samples = 0);
    //: Make a scatter plot of the data

    virtual bool Plot(const ClassifierC & classifier, const DataSetVectorLabelC & dataSet, UIntT feature1 = 0, UIntT feature2 = 1);
    //: Plot right/wrong decisions made by a classifier

    virtual bool Plot(const FunctionC & function, const VectorC & min, const VectorC & max);
    //: Plot right/wrong decisions made by a classifier

    virtual bool SetXLabel(const StringC & xlabel);
    //: Set the x-label

    virtual bool SetYLabel(const StringC & ylabel);
    //: Set the y-label

    virtual bool SetXRange(const RealRangeC & xrange);
    //: Set the range of the x-axis

    virtual bool SetYRange(const RealRangeC & yrange);
    //: Set the range of the y-axis

    bool SetAxisRange(const RealRangeC & xrange, const RealRangeC & yrange);
    //: Set axis range of first plot

    virtual bool SetLineStyle(const StringC & lineStyle);
    //: Set line style of plot, points, line e.t.c.

    virtual bool SetOutput(const StringC & output, const IndexRange2dC & rec = IndexRange2dC(750, 1000));
    //: Set output x11 or filename.  If filename it will guess type from extension...

    virtual bool Command(const StringC & command);
    //: General method to send a command to the plotting library, e.g. gnuplot

    typedef RavlN::SmartPtrC<Plot2dC> RefT;

  protected:
    StringC m_title; //!< The overall title

  };

  extern Plot2dC::RefT (*g_createPlot2d)(const StringC & title);

  //! userlevel=Develop
  inline Plot2dC::RefT CreatePlot2d(const StringC & title)
  {
    return g_createPlot2d(title);
  }

} /* namespace RavlN */
#endif /* PLOT2D_HH_ */
