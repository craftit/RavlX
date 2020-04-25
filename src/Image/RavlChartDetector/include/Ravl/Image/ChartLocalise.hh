// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_CHARTLOCALISE_HEADER
#define RAVLIMAGE_CHARTLOCALISE_HEADER 1
//! rcsid="$Id$"
//! lib=RavlChartDetector
//! file="Ravl/Image/Processing/ChartDetector/ChartLocalise.hh"
//! docentry="Ravl.API.Images.Chart Detector"

#include "Ravl/Image/Image.hh"
#include "Ravl/Point2d.hh"
#include "Ravl/Projection2d.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/RefCounter.hh"

namespace RavlImageN {
  
  //! userlevel=Develop
  //: Fine localisation for chart points.
  // It is assumed that the chart features are black on white and that the extreme
  // values of the chart points match those of the black patches in the example image.
  
  class ChartLocaliseBodyC 
    : public RCBodyC 
  {
  public:
    ChartLocaliseBodyC(const ImageC<ByteT> &exampleChart,const SArray1dC<Point2dC> &chartPoints,UIntT patchSize = 10);
    //: Constructor.
    // It is assumed that the chart features are black on white and that the extreme
    // values of the chart points match those of the black patches in the example image.
    //!param:exampleChart - Example image of the chart
    //!param:chartPoints - Points on the chart for which fine localisation is required.
    //!param:patchSize - number of pixels around the points to use for localisation.
    
    bool Apply(const ImageC<ByteT> &scene,const Projection2dC &position,SArray1dC<Point2dC> &matches,SArray1dC<bool> &matchOk);
    //: Get fine localisation for points.
    //!param:scene - Image in which the chart has been found.
    //!param:position - Estimated position of the chart
    //!param:matches - Array to put points from the scene corresponding to 'chartPoints' provided in the constructor.
    
    void SetVerbose(bool verboseMode)
    { verbose = verboseMode; }
    //: Set verbose mode.
    
    Point2dC Chart2Example(const Point2dC &point) const;
    //: Map a position from chart to example image.
    
    Point2dC Example2Chart(const Point2dC &point) const;
    //: Map a position from example image to chart.
    
  protected:
    bool CalibrateExample();
    //: Calibrate the given chart coordinates and the example image.
    
    bool MatchPoint(Point2dC chartPoint,const ImageC<ByteT> &sceneImage,const Projection2dC &position,Point2dC &result);
    //: Match a single point from the chart.
    //!param:chartPoint - location on chart.
    //!param:sceneImage - Scene image in which we're sea
    //!param:position - Estimate of the position of the chart.
    //!param:result - Corresponding point in scene.
    //!return: True if point localised successfully.
    
    
    UIntT patchSize;
    ImageC<ByteT> exampleImage;
    SArray1dC<Point2dC> chartPoints; 
    
    Vector2dC c2e_scale;  // Chart to example scale factor
    Vector2dC c2e_offset; // Chart to example offset.
    
    bool verbose;
  };
  
  //! userlevel=Normal
  //: Fine localisation for chart points. 
  // It is assumed that the chart features are black on white and that the extreme
  // values of the chart points match those of the black patches in the example image.
  //!cwiz:author
  
  class ChartLocaliseC
    : public RCHandleC<ChartLocaliseBodyC>
  {
  public:
    ChartLocaliseC(const ImageC<ByteT> & exampleChart,const SArray1dC<Point2dC> & chartPoints,UIntT patchSize = 10) 
      : RCHandleC<ChartLocaliseBodyC>(*new ChartLocaliseBodyC(exampleChart,chartPoints,patchSize))
    {}
    //: Constructor. 
    // It is assumed that the chart features are black on white and that the extreme
    // values of the chart points match those of the black patches in the example image.
    //!param:exampleChart - Example image of the chart
    //!param:chartPoints - Points on the chart for which fine localisation is required.
    //!param:patchSize - number of pixels around the points to use for localisation.
    //!cwiz:author
    
    bool Apply(const ImageC<ByteT> & scene,const Projection2dC & position,SArray1dC<Point2dC> & matches,SArray1dC<bool> & matchOk) 
    { return Body().Apply(scene,position,matches,matchOk); }
    //: Get fine localisation for points. 
    //!param:scene - Image in which the chart has been found.
    //!param:position - Estimated position of the chart
    //!param:matches - Array to put points from the scene corresponding to 'chartPoints' provided in the constructor.
    //!cwiz:author
    
    void SetVerbose(bool verboseMode) 
    { Body().SetVerbose(verboseMode); }
    //: Set verbose mode. 
    //!cwiz:author
    
    Point2dC Chart2Example(const Point2dC & point) const
    { return Body().Chart2Example(point); }
    //: Map a position from chart to example image. 
    //!cwiz:author
    
    Point2dC Example2Chart(const Point2dC & point) const
    { return Body().Example2Chart(point); }
    //: Map a position from example image to chart. 
    //!cwiz:author
    
  protected:
    ChartLocaliseC(RCHandleC<ChartLocaliseBodyC> &bod)
     : RCHandleC<ChartLocaliseBodyC>(bod)
    {}
    //: Body constructor. 
    
    ChartLocaliseBodyC& Body()
    { return static_cast<ChartLocaliseBodyC &>(RCHandleC<ChartLocaliseBodyC>::Body()); }
    //: Body Access. 
    
    const ChartLocaliseBodyC& Body() const
    { return static_cast<const ChartLocaliseBodyC &>(RCHandleC<ChartLocaliseBodyC>::Body()); }
    //: Body Access. 
    
  };
  
}

#endif
