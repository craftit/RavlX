// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlChartDetector
//! file="Ravl/Image/Processing/ChartDetector/ChartLocalise.cc"

#include "Ravl/Optimise2dHomography.hh"
#include "Ravl/Image/ChartLocalise.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/RealRange2d.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/SArray1dIter3.hh"
#include "Ravl/Affine2d.hh"
#include "Ravl/Image/CostAffineImageCorrelation.hh"
#include "Ravl/PatternRec/OptimiseDescent.hh"
#include "Ravl/PatternRec/OptimisePowell.hh"
#include "Ravl/Image/WarpAffine.hh"
#include "Ravl/Image/DrawCross.hh"
#include "Ravl/Image/NormalisedCorrelation.hh"
#include "Ravl/IO.hh"
#include "Ravl/OS/Date.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {

  //: Constructor.
  //!param:exampleChart - Example image of the chart
  //!param:chartPoints - Points on the chart for which fine localisation is required.
  //!param:patchSize - number of pixels around the points to use for localisation.

  ChartLocaliseBodyC::ChartLocaliseBodyC(const ImageC<ByteT> &exampleChart,const SArray1dC<Point2dC> &nChartPoints,UIntT nPatchSize) 
    : patchSize(nPatchSize),
      exampleImage(exampleChart),
      chartPoints(nChartPoints),
      verbose(false)
  { CalibrateExample(); }

  //: Calibrate the given chart coordinates and the example image.
  
  bool ChartLocaliseBodyC::CalibrateExample() {
    ONDEBUG(cerr << "ChartLocaliseBodyC::CalibrateExample(), Called. \n");

    // Find the range of features in the example image.
    
    IndexRange2dC exampleRange;
    bool isFirst = true;
    for(Array2dIterC<ByteT> it(exampleImage);it;it++) {
      if(*it > 128)
	continue;
      if(isFirst) {
	exampleRange = IndexRange2dC(it.Index(),0);
	isFirst = false;
	continue;
      }
      exampleRange.Involve(it.Index());
    }
    
    // Look through the chart coordinates.
    
    SArray1dIterC<Point2dC> it(chartPoints);
    RealRange2dC coordRange(*it,0);
    for(it++;it;it++)
      coordRange.Involve(*it);
    
    // Create mapping from chart coordinates to example image.
    
    c2e_scale = Vector2dC(static_cast<RealT>(exampleRange.Rows()) / coordRange.Rows(),
		      static_cast<RealT>(exampleRange.Cols()) / coordRange.Cols());
    c2e_offset = Vector2dC(Point2dC(exampleRange.Origin())/c2e_scale) - coordRange.Origin();
    
    ONDEBUG(cerr << "ChartLocaliseBodyC::CalibrateExample(), Done. Offset=" << c2e_offset << " Scale=" << c2e_scale <<  "\n");
    
    return true;
  }
  
  //: Map a position from chart to example image.
  
  Point2dC ChartLocaliseBodyC::Chart2Example(const Point2dC &point) const 
  { return (point + c2e_offset) * c2e_scale; }
  
  //: Map a position from example image to chart.
  
  Point2dC ChartLocaliseBodyC::Example2Chart(const Point2dC &point) const 
  { return (point / c2e_scale) - c2e_offset; }
  
  //: Get fine localisation for points.
  //!param:scene - Image in which the chart has been found.
  //!param:position - Estimated position of the chart
  //!param:matches - Array to put points from the scene corresponding to 'chartPoints' provided in the constructor.
  
  bool ChartLocaliseBodyC::Apply(const ImageC<ByteT> &scene,const Projection2dC &position,SArray1dC<Point2dC> &matches,SArray1dC<bool> &matchOk) {
    ONDEBUG(cerr << "ChartLocaliseBodyC::Apply(), Called. \n");
    matches = SArray1dC<Point2dC>(chartPoints.Size());
    // Go through points find there matches.
    bool ret = true;
#if 1
    //RavlN::Save("@X:Opt",exampleImage);
    //RavlN::Save("@X:OOpt",exampleImage);
#endif
    matchOk = SArray1dC<bool>(matches.Size());
    UIntT pass = 0;
    for(SArray1dIter3C<Point2dC,Point2dC,bool> it(chartPoints,matches,matchOk);it;it++) {
      it.Data3() = MatchPoint(it.Data1(),scene,position,it.Data2());
      if(it.Data3())
	pass++;
      ret &= it.Data3();
    }
    if(ret) {
      ONDEBUG(cerr << "ChartLocaliseBodyC::Apply(), Done in first pass. \n");
      return true;
    }
#if 0    
    // ---  Use points we're happy with to have another go.---
    
    cerr << "Reestimating chart position. \n";
    DListC<Point2dPairObsC> matchList;
    MatrixRSC modelNoise = MatrixRSC(2);
    modelNoise.Fill(0);
    modelNoise[0][0] = 0.0001;
    modelNoise[1][1] = 0.0001;
    
    MatrixRSC sceneNoise = MatrixRSC(2);
    sceneNoise.Fill(0);
    sceneNoise[0][0] = 0.1;
    sceneNoise[1][1] = 0.1;
    
    for(SArray1dIter3C<Point2dC,Point2dC,bool> it(chartPoints,matches,matchOk);it;it++) {
      if(!it.Data3())
	continue;
      matchList.InsLast(Point2dPairObsC(Chart2Example(it.Data1()),modelNoise,
					it.Data2(),sceneNoise));
    }
    StateVectorHomog2dC stateVector;
    try {
      stateVector = Optimise2dHomography (matchList,200,200);
    } catch(ExceptionC &) {
      ONDEBUG(cerr << "Numerical problem detected. \n");
    }
    
    Projection2dC newPosition(stateVector.GetHomog(),stateVector.GetZH1(),stateVector.GetZH2());
    ret = true;
    for(SArray1dIter3C<Point2dC,Point2dC,bool> it(chartPoints,matches,matchOk);it;it++) {
      if(it.Data3())
	continue; // Got this one already.
      it.Data3() = MatchPoint(it.Data1(),scene,newPosition,it.Data2());
      if(it.Data3())
	pass++;
      ret &= it.Data3();
    }
#endif
    // All done.
    return ret;
  }
  
  //: Match a single point from the chart.
  //!param:chartPoint - location on chart.
  //!param:result - Corresponding point in scene.
  //!return: True if point localised successfully.
  
  bool ChartLocaliseBodyC::MatchPoint(Point2dC chartPoint,const ImageC<ByteT> &sceneImage,const Projection2dC &position,Point2dC &result) {
    
    Point2dC exPoint = Chart2Example(chartPoint);
    
    ONDEBUG(cerr << "ChartLocaliseBodyC::MatchPoint(), Called. chartPoint=" << chartPoint << " Example=" << exPoint << "\n");
    
    // --- First cut a point out of the example image ---
    
    IndexRange2dC cutOutRect((Index2dC)exPoint,patchSize/2);
    RavlAssertMsg(exampleImage.Frame().Contains(cutOutRect),"Example image too small or patch size too large.");
    ImageC<ByteT> patch(exampleImage,cutOutRect);
    
    // --- Now estimate the locale affine transform for the point. ---
    
    SArray1dC<Point2dC> exPoints(5),imPoints(5);
    exPoints[0] = exPoint;
    exPoints[1] = exPoint + Vector2dC(patchSize/4.0,0);
    exPoints[2] = exPoint + Vector2dC(0,patchSize/4.0);
    exPoints[3] = exPoint - Vector2dC(patchSize/4.0,0);
    exPoints[4] = exPoint - Vector2dC(0,patchSize/4.0);
    
    
    // Take projective transform
    
    for(SArray1dIter2C<Point2dC,Point2dC> it(imPoints,exPoints);it;it++)
      it.Data1() = position * it.Data2();
    
    // Fit affine transform to the points.
    
    Affine2dC affPos = FitAffine(exPoints,imPoints);
    
#if 0
    {
      WarpAffineC<ByteT> ewarp(exampleImage.Frame(),affPos);
      ImageC<ByteT> testi = ewarp.Apply(sceneImage);
      //RavlN::Save("@X:Target",testi);
      
      WarpAffineC<ByteT> bwarp(patch.Frame(),affPos);
      ImageC<ByteT> rImage = bwarp.Apply(sceneImage);
      RavlN::Save("@X:Opt",rImage);
      Sleep(2);
    }
#endif
    
    // --- Optimise position ---
    
    OptimisePowellC optimiser(200,1e-5);
    //OptimiseDescentC optimiser(500);
    RealT minCost = 0,correlation=1;
    
#if 1
    VectorC searchRange(6);
    searchRange[0] = 20; // Row offsets
    searchRange[1] = 20; // Col offsets
    searchRange[2] = 0.4;  // Variation from the identity matrix.
    searchRange[3] = 0.4;
    searchRange[4] = 0.4;
    searchRange[5] = 0.4;
    
    CostAffineImageCorrelationC domain(patch,sceneImage,affPos,searchRange);
    VectorC optResult = optimiser.MinimalX(domain,minCost);
    //cerr << "Opt=" << optResult << "\n";
    Affine2dC refinedPos = domain.Vector2Affine(optResult);
    
    // Check the correlation of the result.
    
    WarpAffineC<ByteT> xwarp(patch.Frame(),refinedPos);
    xwarp.SetMidPixelCorrection(false);
    
    ImageC<ByteT> xImage = xwarp.Apply(sceneImage);
    //RavlN::Save("@X:ORef",patch);
    //RavlN::Save("@X:OOpt",xImage);
    correlation = NormalisedCorrelation(patch,xImage);
    ONDEBUG(cerr << "Correlation=" << correlation << "\n");
    // --- Get refined position for chart point. ---
    
    result =  refinedPos * exPoint;
#endif
    if(verbose)
      cerr << " " << chartPoint << " -> " << result << " (" << correlation << ")\n";
    ONDEBUG(cerr << "ChartLocaliseBodyC::MatchPoint(), Done. match=" << result << " \n");
    //Sleep(1);
    if(correlation < 0.85)
      return false; // Its just not a good enough fit.
    return true;
  }
  
  
}
