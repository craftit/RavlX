// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_CHARTDETECTOR_HEADER
#define RAVLIMAGE_CHARTDETECTOR_HEADER 1
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlChartDetector
//! docentry="Ravl.API.Images.Chart Detector"
//! file="Ravl/Image/Processing/ChartDetector/ChartDetector.hh"

#include "Ravl/Image/Image.hh"
#include "Ravl/Image/SegmentExtrema.hh"
#include "Ravl/Image/ChartDetectorRegion.hh"
#include "Ravl/Projection2d.hh"
#include "Ravl/Affine2d.hh"

namespace RavlImageN {
  
  
  //! userlevel=develop
  //: Chart detector.
  
  class ChartDetectorBodyC
    : public RCBodyC
  {
  public:
    ChartDetectorBodyC(const ImageC<ByteT> &exampleImage,UIntT minRegionSize = 40,UIntT minRegionMargin = 10);
    //: Constructor.
    //!param:exampleImage - A frontal image of the chart to be detected.
    //!param:minRegionSize - Minimum region size ot consider.
    //!param:minRegionMargin - Region stability threshold.
    
    bool Apply(const ImageC<ByteT> &image,Projection2dC &transform);
    //: Search for the chart in an image.
    //!param:transform - Estimate of the projective transform of the chart.
    //!return:Returns true if chart has been found.
    
    void SetInlierThreshold(RealT dist)
    { inlierDistanceThreshold = Sqr(dist); }
    //: Set distance  to use for inlier threshold.
    
    void SetRANSACIterations(UIntT size)
    { ransacIterations = size; }
    // Number of ransac iterations to try.
    
    void SetAcceptThreshold(RealT threshold)
    { acceptCorrelationThreshold=threshold; }
    //: Accept threshold
    // This is point at which the correlation of example image and the scene.
    
  protected:
    DListC<ChartDetectorRegionC> FeatureExtract(const ImageC<ByteT> &image);
    //: Extract features from an image.
    
    SArray1dC<SArray1dC<ChartDetectorRegionC> > BuildArrays(const DListC<ChartDetectorRegionC> &features);
    //: Turn features into arrays of the various region types.
    
    RealT TestHypothesis(const Affine2dC &transform,SArray1dC<SArray1dC<ChartDetectorRegionC> > &sceneModel);
    //: Test affine hypothesis.
    
    DListC<Tuple2C<ChartDetectorRegionC,ChartDetectorRegionC> > ListInliers(const Affine2dC &transform,SArray1dC<SArray1dC<ChartDetectorRegionC> > &sceneModel);
    //: Generate a list of inliers
    //!param: transform - Transform hypothesis
    //!param: sceneModel - Regions from the current scene.
    //!return: List of matches, chart regions to scene regions.
    
    Projection2dC FitProjection(DListC<Tuple2C<ChartDetectorRegionC,ChartDetectorRegionC> > &matches);
    //: Fit a projective transform to the matches.
    
    ImageC<ByteT> exampleImage;
    SegmentExtremaC<ByteT> segmenter;
    
    SArray1dC<SArray1dC<ChartDetectorRegionC> > chartModel; // Regions we found in the chart.
    
    RealT inlierDistanceThreshold; // Distance to which accept matches as inliers.
    UIntT ransacIterations; // Number of ransac iterations to try.
    RealT acceptCorrelationThreshold; // Threshold at which to accept hypothesis.
  };
  
  //! userlevel=Normal
  //: Chart detector. 
  //!cwiz:author
  
  class ChartDetectorC
    : public RCHandleC<ChartDetectorBodyC>
  {
  public:
    
    ChartDetectorC(const ImageC<ByteT> & exampleImage,UIntT minRegionSize = 40,UIntT minRegionMargin = 10) 
      : RCHandleC<ChartDetectorBodyC>(*new ChartDetectorBodyC(exampleImage,minRegionSize,minRegionMargin))
    {}
    //: Constructor. 
    //!param:exampleImage - A frontal image of the chart to be detected.
    //!param:affineMatch - When true use affine matching otherwise use projective matching.
    //!param:minRegionSize - Minimum region size ot consider.
    //!param:minRegionMargin - Region stability threshold.
    //!cwiz:author
    
    bool Apply(const ImageC<ByteT> & image,Projection2dC & transform) 
    { return Body().Apply(image,transform); }
    //: Search for the chart in an image. 
    //!param:transform - Estimate of the projective transform of the chart.
    //!return:Returns true if chart has been found.
    //!cwiz:author
    
    void SetInlierThreshold(RealT dist) 
    { Body().SetInlierThreshold(dist); }
    //: Set distance  to use for inlier threshold. 
    //!cwiz:author
    
    void SetRANSACIterations(UIntT size) 
    { Body().SetRANSACIterations(size); }
    // Number of ransac iterations to try.
    //!cwiz:author
    
    void SetAcceptThreshold(RealT threshold) 
    { Body().SetAcceptThreshold(threshold); }
    //: Accept threshold 
    // This is point at which the correlation of example image and the scene.
    //!cwiz:author
    
  protected:
    ChartDetectorC(RCHandleC<ChartDetectorBodyC> &bod)
     : RCHandleC<ChartDetectorBodyC>(bod)
    {}
    //: Body constructor. 
    
    ChartDetectorBodyC& Body()
    { return static_cast<ChartDetectorBodyC &>(RCHandleC<ChartDetectorBodyC>::Body()); }
    //: Body Access. 
    
    const ChartDetectorBodyC& Body() const
    { return static_cast<const ChartDetectorBodyC &>(RCHandleC<ChartDetectorBodyC>::Body()); }
    //: Body Access. 
    
  };
  
}

#endif
