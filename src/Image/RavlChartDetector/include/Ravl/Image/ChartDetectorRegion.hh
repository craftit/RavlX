// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_CHARTDETECTORREGION_HEADER
#define RAVLIMAGE_CHARTDETECTORREGION_HEADER 1
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlChartDetector
//! docentry="Ravl.API.Images.Chart Detector"
//! file="Ravl/Image/Processing/ChartDetector/ChartDetectorRegion.hh"

#include "Ravl/Image/Segmentation.hh"
#include "Ravl/Point2d.hh"

namespace RavlImageN {
  
  //! userlevel=Develop
  //: Chart detector Region model

  class ChartDetectorRegionBodyC 
    : public RCBodyC
  {
  public:
    ChartDetectorRegionBodyC()
    {}
    //: Default constructor.
    // Contents of model are undefined.
    
    ChartDetectorRegionBodyC(const SegmentationC &aSegmentation,UIntT nLabel,UIntT nRegionType);
    //: Region model.
    
    void UpdateStats();
    //: Update stats on region.
    
    UIntT RegionType() const
    { return regionType; }
    //: Segmentation type.
    
    const SegmentationC &Segmentation() const
    { return segmentation; }
    //: Region segmentation.
    
    UIntT RegionTypeId() const
    { return regionType + ((holes < 4) ? (2 * holes) : 6); }
    //: Get a unique number for region type.
    
    static UIntT MaxRegionTypeId()
    { return 7; }
    //: Get the maximum region type id that will be returned.
    
    const Point2dC &Centroid() const
    { return centroid; }
    //: Get the centroid of the region.
    
  protected:
    SegmentationC segmentation;
    UIntT label;      // Label for region in segmentation.
    UIntT regionType; // Type id for segmentation. 0-Positive 1-Negative.
    
    UIntT holes;      // Number of holes in the region.
    Point2dC centroid; // Centroid of region.
  };
  
  //! userlevel=Normal
  //: Chart detector Region model 
  //!cwiz:author
  
  class ChartDetectorRegionC
    : public RCHandleC<ChartDetectorRegionBodyC>
  {
  public:
    ChartDetectorRegionC() 
      : RCHandleC<ChartDetectorRegionBodyC>(*new ChartDetectorRegionBodyC())
    {}
    //: Default constructor. 
    // Contents of model are undefined.
    //!cwiz:author
    
    ChartDetectorRegionC(const SegmentationC & aSegmentation,UIntT nLabel,UIntT nRegionType) 
      : RCHandleC<ChartDetectorRegionBodyC>(*new ChartDetectorRegionBodyC(aSegmentation,nLabel,nRegionType))
    {}
    //: Region model. 
    //!cwiz:author
    
    void UpdateStats() 
    { Body().UpdateStats(); }
    //: Update stats on region. 
    //!cwiz:author
    
    UIntT RegionType() const
    { return Body().RegionType(); }
    //: Segmentation type. 
    //!cwiz:author
    
    const SegmentationC & Segmentation() const
    { return Body().Segmentation(); }
    //: Region segmentation. 
    //!cwiz:author
    
    static UIntT MaxRegionTypeId()
    { return 7; }
    //: Get the maximum region type id that will be returned.
    
    UIntT RegionTypeId() const
    { return Body().RegionTypeId(); }
    //: Get a unique number for region type. 
    //!cwiz:author
    
    const Point2dC & Centroid() const
    { return Body().Centroid(); }
    //: Get the centroid of the region. 
    //!cwiz:author
    
  protected:
    ChartDetectorRegionC(RCHandleC<ChartDetectorRegionBodyC> &bod)
     : RCHandleC<ChartDetectorRegionBodyC>(bod)
    {}
    //: Body constructor. 
    
    ChartDetectorRegionBodyC& Body()
    { return static_cast<ChartDetectorRegionBodyC &>(RCHandleC<ChartDetectorRegionBodyC>::Body()); }
    //: Body Access. 
    
    const ChartDetectorRegionBodyC& Body() const
    { return static_cast<const ChartDetectorRegionBodyC &>(RCHandleC<ChartDetectorRegionBodyC>::Body()); }
    //: Body Access. 
    
  };

}

#endif
