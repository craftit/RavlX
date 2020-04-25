/*
 * Plot.cc
 *
 *  Created on: 10 Aug 2012
 *      Author: kieron
 */

#include "Ravl/Plot/GnuPlot2d.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/Assert.hh"
#include "Ravl/SysLog.hh"
#include "Ravl/Exception.hh"
#include "Ravl/PatternRec/DataSet2Iter.hh"
#include "Ravl/PatternRec/Classifier.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/SumsNd2.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  /*
   * Construct
   */
  GnuPlot2dC::GnuPlot2dC(const StringC & title)
   : Plot2dC(title),
     m_gnuPlot((StringC) "gnuplot", false, false, true),
     m_plotNum(0)
  {
    // check it is still running
    if (!m_gnuPlot.IsRunning()) {
      RavlError("GnuPlot not running!");
      throw ExceptionBadConfigC("gnuplot not running!");
    }

    // set-up the default terminal
    if (!title.IsEmpty()) {
      Command("set title \"" + title + "\"");
    }

    CommandTerminalWindow();

  }

  //! Create a temporary file.
  StringC GnuPlot2dC::SetupTmpFile()
  {
    if(m_tmpFile.IsEmpty()) {
      FilenameC tmpFile = "/tmp/data";
      m_tmpFile = tmpFile.MkTemp(6, -1);
    } else {
      FilenameC old(m_tmpFile + StringC(m_plotNum));
      if(old.Exists())
        old.Remove();
    }
    m_plotNum++;
    if(m_plotNum > 4)
      m_plotNum = 0;
    // Switch between two files to avoid truncation problems.
    return m_tmpFile + StringC(m_plotNum);
  }

  /*
   * Plot all on a single graph
   */

  bool GnuPlot2dC::Plot(const SArray1dC<Point2dC> & data, const StringC & dataName)
  {
    // check it is still running
    if (!m_gnuPlot.IsRunning()) {
      RavlError("GnuPlot not running!");
      return false;
    }
    if(data.Size() == 0) {
      RavlWarning("No data to plot for '%s' ",dataName.c_str());
      return false;
    }
    // make the command
    StringC cmd;
    if (dataName.IsEmpty()) {
      cmd.form("plot \"-\" notitle");
    } else {
      cmd.form("plot \"-\" title \'%s\'", dataName.data());
    }
    Command(cmd);

    // copy the data across
    for (SArray1dIterC<Point2dC> it(data); it; it++) {
      StringC d;
      d.form("%f %f", it.Data().Row(), it.Data().Col());
      Command(d);
    }
    Command("end");
    Flush();

    return true;
  }

  /*
   * Plot as separate plots on same graph
   * Keep this function here to avoid changing the class interface.
   */
  bool GnuPlot2dC::Plot(const RCHashC<StringC, CollectionC<Point2dC> > & data)
  { return Plot2dC::Plot(data); }

  //: Plot all plots on same canvas, preserve order so we keep the same markers
  bool GnuPlot2dC::Plot(const CollectionC<Tuple2C<StringC, CollectionC<Point2dC> > > & data)
  {
    // check it is still running
    if (!m_gnuPlot.IsRunning()) {
      RavlError("GnuPlot not running!");
      return false;
    }
    if(data.Size() == 0) {
      RavlWarning("No data to plot. ");
      return false;
    }

    /*
     * Not a big fan of this but can not pipe data to GnuPlot
     * for more complicated plots...
     */
    StringC tmpFile = SetupTmpFile();
    OStreamC os(tmpFile);
    StringC cmd = "plot ";
    UIntT i = 0;
    int foundCount = 0;
    for (CollectionIterC<Tuple2C<StringC, CollectionC<Point2dC> > > hshIt(data); hshIt; hshIt++) {
      if(hshIt.Data().Data2().Size() == 0) {
        RavlWarning("No data to plot for '%s', skipping ",hshIt.Data().Data1().c_str());
        continue;
      }
      foundCount++;
      os << "# " << hshIt->Data1() << endl;
      StringC localPlot;
      localPlot.form("\'%s\' index %d title '%s',", tmpFile.c_str(), i, hshIt->Data1().c_str());
      cmd += localPlot;
      for (SArray1dIterC<Point2dC> it(hshIt->Data2().SArray1d()); it; it++) {
        StringC d;
        d.form("%f %f", it.Data().Row(), it.Data().Col());
        os << d << endl;
      }
      i++;
      os << "\n\n"; // double line important
    }
    if(!foundCount) {
      RavlWarning("No data at all to plot. ");
      return false;
    }
    cmd.del((int) cmd.Size() - 1, (int) 1);
    // send the command
    Command(cmd);

    Flush();
    return true;
  }

  /*
   * Plot a function
   */
  bool GnuPlot2dC::Plot(const StringC & function)
  {
    m_gnuPlot.StdIn() << "plot " << function << endl;
    Flush();
    return true;
  }

  /*
   * Plot a straight line
   */
  bool GnuPlot2dC::Plot(const LineABC2dC & line)
  {
    SArray1dC<Point2dC> arr(100);
    RealT step = m_xrange.Size() / 100.0;
    UIntT c = 0;
    for (RealT x = m_xrange.Min(); x <= m_xrange.Max(); x += step) {
      RealT y = line.ValueY(x);
      arr[c] = Point2dC(x, y);
      c++;
    }

    return Plot(arr);
  }

  /*
   * Make a scatter plot of a data set
   */
  bool GnuPlot2dC::Plot(const DataSetVectorLabelC & dataSet, UIntT fv1, UIntT fv2, UIntT samplesPerClass)
  {

    if (fv1 >= dataSet.Sample1().VectorSize() || fv2 >= dataSet.Sample1().VectorSize()) {
      RavlError("Feature index larger than in data set");
      return false;
    }

    DataSetVectorLabelC useDataSet = dataSet;
    if (samplesPerClass != 0) {
      useDataSet = dataSet.ExtractPerLabel(samplesPerClass);
    }

    SArray1dC<FieldInfoC> fieldInfo = dataSet.Sample1().FieldInfo();
    if (fieldInfo.IsValid()) {
      SetXLabel(fieldInfo[fv1].Name());
      SetYLabel(fieldInfo[fv2].Name());
    }

    // have to find min max

    RealRangeC xrange(RavlConstN::maxReal, -RavlConstN::maxReal);
    RealRangeC yrange(RavlConstN::maxReal, -RavlConstN::maxReal);
    for (DataSet2IterC<SampleVectorC, SampleLabelC> it(dataSet); it; it++) {
      xrange.Min() = Min(xrange.Min(), it.Data1()[fv1]);
      xrange.Max() = Max(xrange.Max(), it.Data1()[fv1]);
      yrange.Min() = Min(yrange.Min(), it.Data1()[fv2]);
      yrange.Max() = Max(yrange.Max(), it.Data1()[fv2]);
    }
    if(!m_xrange.IsValid())
      SetXRange(xrange);
    if(!m_yrange.IsValid())
      SetYRange(yrange);
    //: Plot a function

    StringC tmpFile = SetupTmpFile();
    OStreamC os(tmpFile);
    StringC cmd;

    UIntT label = 0;
    UIntT count = 0;
    StringC bigCmd = "plot ";
    for (SArray1dIterC<SampleVectorC> it(useDataSet.SeperateLabels()); it; it++) {
      StringC className;
      if (!dataSet.Sample2().GetClassName(label, className)) {
        className = "Label " + (StringC) label;
      }
      StringC cmd;
      cmd.form("'%s' every ::%d::%d with points pointtype 1 title \'%s\'",
          tmpFile.data(),
          count,
          count + it.Data().Size().V() - 1,
          className.data());
      for (SampleIterC<VectorC> vecIt(it.Data()); vecIt; vecIt++) {
        os << vecIt.Data()[fv1] << ' ' << vecIt.Data()[fv2] << endl;
      }
      label++;
      count += it.Data().Size();
      bigCmd += cmd + ",";
    }
    bigCmd.del((int) bigCmd.Size().V() - 1, (int) 1);
    os.Close();
    Command(bigCmd);
    Flush();
    return true;
  }

  /*
   * Plot right wrong decisions made by a trained classifier
   */
  bool GnuPlot2dC::Plot(const ClassifierC & classifier, const DataSetVectorLabelC & dataSet, UIntT feature1, UIntT feature2)
  {
    RavlInfo("Classifier plot....");

    // need to find min and max of features
    SampleVectorC sv = dataSet.Sample1();
    VectorC first = sv.First();

    if (feature1 >= sv.Size() || feature2 >= sv.Size()) {
      RavlError("Requested feature index larger than dimension of data set");
      return false;
    }

    RealT min1 = first[feature1];
    RealT max1 = first[feature1];
    RealT min2 = first[feature2];
    RealT max2 = first[feature2];
    SumsNd2C sums(first.Size()); // may as well get mean at same time

    StringC tmpFileData = SetupTmpFile();
    OStreamC osData(tmpFileData);
    for (DataSet2IterC<SampleVectorC, SampleLabelC> it(dataSet); it; it++) {
      sums += it.Data1();
      min1 = Min(min1, it.Data1()[feature1]);
      max1 = Max(max1, it.Data1()[feature1]);
      min2 = Min(min2, it.Data1()[feature2]);
      max2 = Max(max2, it.Data1()[feature2]);
      osData << it.Data1()[feature1] << ' ' << it.Data1()[feature2] << ' ' << it.Data2() << endl;
    }
    osData.Close();

    StringC tmpFile = SetupTmpFile() +"B";
    OStreamC os(tmpFile);

    RealT step1 = (max1 - min1) / 500.0;
    RealT step2 = (max2 - min2) / 500.0;
    VectorC mean = sums.Mean();

    for (RealT x1 = min1; x1 <= max1; x1 += step1) {
      for (RealT x2 = min2; x2 <= max2; x2 += step2) {
        mean[feature1] = x1;
        mean[feature2] = x2;
        UIntT label = classifier.Classify(mean);
        os << x1 << ' ' << x2 << ' ' << label << '\n';
      }
      os << '\n'; // have to have a line between rows for gnnuplot
    }
    os.Close();

    SetXRange(RealRangeC(min1, max1));
    SetYRange(RealRangeC(min2, max2));
    StringC cmd;
    cmd.form("set palette rgbformulae -7,2,-7");
    Command(cmd);
    cmd.form("plot \'%s\' using 1:2:3 with image, \'%s\' using 1:2:3 with labels notitle", tmpFile.data(), tmpFileData.data());
    Command(cmd);

    return true;
  }

  /*
   * Plot a function between min and max
   */
  bool GnuPlot2dC::Plot(const FunctionC & function, const VectorC & min, const VectorC & max)
  {
    if (min.Size() != 2 || max.Size() != 2) {
      RavlError("Dimension not supported yet!");
      return false;
    }

    if(function.OutputSize() != 1) {
      RavlError("Output dimension not equal to 1");
      return false;
    }

    StringC tmpFile = SetupTmpFile();
    OStreamC os(tmpFile);

    for (RealT d2 = min[1]; d2 < max[1]; d2 += (max[1] - min[1]) / 50.0) {
      for (RealT d1 = min[0]; d1 < max[0]; d1 += (max[0] - min[0]) / 50.0) {
        VectorC v(2);
        v[0] = d1;
        v[1] = d2;
        os << d1 << ' ' << d2 << ' ' << function.Apply(v)[0] << '\n';
      }
      os << '\n';
    }

    StringC cmd;
    cmd.form("set pm3d at b");
    Command(cmd);
    cmd.form("splot \'%s\' with lines notitle", tmpFile.data());
    Command(cmd);

    return true;
  }

  /*
   * Set the X label
   */
  bool GnuPlot2dC::SetXLabel(const StringC & xlabel)
  {
    Command("set xlabel \'" + xlabel + "\'");
    return false;
  }

  /*
   * Set the Y label
   */
  bool GnuPlot2dC::SetYLabel(const StringC & ylabel)
  {
    Command("set ylabel \'" + ylabel + "\'");
    return false;
  }

  /*
   *  Set axis range of a plot
   */

  bool GnuPlot2dC::SetXRange(const RealRangeC & xrange)
  {
    m_xrange = xrange;
    StringC cmd;
    cmd.form("set xrange [%f:%f]", xrange.Min(), xrange.Max());
    return Command(cmd);
  }

  /*
   *  Set axis range of a plot
   */

  bool GnuPlot2dC::SetYRange(const RealRangeC & yrange)
  {
    m_yrange = yrange;
    StringC cmd;
    cmd.form("set yrange [%f:%f]", yrange.Min(), yrange.Max());
    return Command(cmd);
  }

  /*
   * Set the line style
   */

  bool GnuPlot2dC::SetLineStyle(const StringC & lineStyle)
  {
    StringC cmd;
    cmd.form("set style data %s", lineStyle.data());
    return Command(cmd);
  }

  /*
   * Set the output.  Either X11 or a filename.
   */

  bool GnuPlot2dC::SetOutput(const StringC & output, const IndexRange2dC & rec)
  {
    if (output == "x11" || output == "") {
      CommandTerminalWindow();
    } else {
      FilenameC fn(output);
      StringC cmd;
      if (fn.HasExtension("png")) {
        cmd.form("set terminal png size %d, %d", (int) rec.Cols().V(), (int) rec.Rows().V());
        Command(cmd);
      } else if (fn.HasExtension("jpg") || fn.HasExtension("jpeg")) {
        cmd.form("set terminal jpg size %d, %d", (int) rec.Cols().V(), (int) rec.Rows().V());
        Command(cmd);
      } else {
        RavlError("gnuplot terminal not supported yet '%s'.", output.data());
        return false;
      }
      cmd.form("set output \'%s\'", output.data());
      Command(cmd);
    }
    return true;
  }

  /*
   * Send the plotting device a generic command
   */

  bool GnuPlot2dC::Command(const StringC & command)
  {
    //RavlInfo("gnuplot: '%s'", command.data());
    m_gnuPlot.StdIn() << command << endl;
    return false;
  }

  void GnuPlot2dC::Flush()
  {
    m_gnuPlot.StdIn().os().flush();
  }

  //! Set the terminal to a window.
  void GnuPlot2dC::CommandTerminalWindow()
  {
#if RAVL_OS_MACOSX
    Command("set terminal x11 persist noraise"); // raise xwt
#else
    Command("set terminal wxt persist noraise");
#endif
  }


  /*
   * Stuff to make gnuplot get used if it is linked in
   */
  Plot2dC::RefT CreatePlot2d_gnuplot(const StringC & title) {
    return new GnuPlot2dC(title);
  }


  int GnuPlotInit()
  {
    ONDEBUG(RavlDebug("Using GnuPlot for plotting"));
    g_createPlot2d = &CreatePlot2d_gnuplot;
    return 0;
  }

  static int a = GnuPlotInit();

  void LinkGnuPlot2d()
  {}

} /* namespace RavlN */
