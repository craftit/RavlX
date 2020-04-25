// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlChartDetector
//! file="Ravl/Image/Processing/ChartDetector/ChartDetector.cc"

#include "Ravl/Image/ChartDetector.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/SArray1dIter3.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/IO.hh"
#include "Ravl/Optimise2dHomography.hh"
#include "Ravl/Image/WarpAffine.hh"
#include "Ravl/Image/NormalisedCorrelation.hh"

#define DODEBUG 0

#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {
  
  ////////////////////////////////////////////////////////////////////////////////
  
  //: Constructor.
  //!param:exampleImage - A frontal image of the chart to be detected.
  
  ChartDetectorBodyC::ChartDetectorBodyC(const ImageC<ByteT> &nexampleImage,UIntT minRegionSize,UIntT minRegionMargin) 
    : exampleImage(nexampleImage),
      segmenter(minRegionSize,minRegionMargin,256),
      inlierDistanceThreshold(20),
      ransacIterations(100000),
      acceptCorrelationThreshold(0.7)
  { chartModel = BuildArrays(FeatureExtract(exampleImage)); }
  
  //: Extract features from an image.
  
  DListC<ChartDetectorRegionC> ChartDetectorBodyC::FeatureExtract(const ImageC<ByteT> &image) {
    ONDEBUG(cerr << "ChartDetectorBodyC::FeatureExtract(), Called. \n");
    DListC<ChartDetectorRegionC> features;
    
    // Do positive regions.
    
    DListC<ImageC<IntT> > posRegions = segmenter.ApplyMask(image);
    ONDEBUG(cerr << "PosRegions=" << posRegions.Size() << "\n");
    
    for(DLIterC<ImageC<IntT> > it(posRegions);it;it++) {
      SegmentationC seg(*it);
      features.InsLast(ChartDetectorRegionC(seg,1,0));
    }
    
    //Save("@X",image);
    
    // Do negative regions.
    
    ImageC<ByteT> negImage(image.Frame());
    for(Array2dIter2C<ByteT,ByteT> it(negImage,image);it;it++)
      it.Data1() = 255 - it.Data2();
    DListC<ImageC<IntT> > negRegions = segmenter.ApplyMask(negImage);
    ONDEBUG(cerr << "NegRegions=" << negRegions.Size() << "\n");
    for(DLIterC<ImageC<IntT> > it(negRegions);it;it++) {
      SegmentationC seg(*it);
      features.InsLast(ChartDetectorRegionC(seg,1,1));
    }
    
    ONDEBUG(cerr << "ChartDetectorBodyC::FeatureExtract(), Done. Features=" << features.Size() << " \n");
    
    return features;
  }

  //: Turn features into arrays of the various region types.
  
  SArray1dC<SArray1dC<ChartDetectorRegionC> > ChartDetectorBodyC::BuildArrays(const DListC<ChartDetectorRegionC> &features) {
    ONDEBUG(cerr << "ChartDetectorBodyC::BuildArrays(), Called. \n");
    
    // Build feature array's for fast comparison.
    
    SArray1dC<SArray1dC<ChartDetectorRegionC> > ret(ChartDetectorRegionC::MaxRegionTypeId()+1);
    
    // First count how many of each type there are.
    
    SArray1dC<UIntT> counts(ChartDetectorRegionC::MaxRegionTypeId()+1);
    counts.Fill(0);
    for(DLIterC<ChartDetectorRegionC> it(features);it;it++) 
      counts[it->RegionTypeId()]++;
    
    // Initialise the arrays and setup iterators.
    
    SArray1dC<SArray1dIterC<ChartDetectorRegionC> > iterArray(ChartDetectorRegionC::MaxRegionTypeId()+1);
    for(SArray1dIter3C<SArray1dC<ChartDetectorRegionC>,UIntT,SArray1dIterC<ChartDetectorRegionC> > it(ret,counts,iterArray);it;it++) {
      //ONDEBUG(cerr << "ChartDetectorBodyC::BuildArrays(), Type=" << it.Index() << " Count=" << it.Data2() << " \n");
      it.Data1() = SArray1dC<ChartDetectorRegionC>(it.Data2()); // Construct array
      it.Data3() = SArray1dIterC<ChartDetectorRegionC>(it.Data1()); // Construct iterator.
    }
    
    // Copy the regions in.
    
    for(DLIterC<ChartDetectorRegionC> it(features);it;it++) {
      SArray1dIterC<ChartDetectorRegionC> &ait = iterArray[it->RegionTypeId()];
      *ait = *it; // Put region into array.
      ait++; // Goto next element.
    }
    
    // We're done.
    
    ONDEBUG(cerr << "ChartDetectorBodyC::BuildArrays(), Done. \n");
    return ret;
  }

  //: Test affine hypothesis.
  
  RealT ChartDetectorBodyC::TestHypothesis(const Affine2dC &transform,SArray1dC<SArray1dC<ChartDetectorRegionC> > &sceneModel) {
    SizeT regions = 0;
    SizeT inliers = 0;
    // Go through model points looking for the closest point in the image.
    
    for(SArray1dIter2C<SArray1dC<ChartDetectorRegionC>,SArray1dC<ChartDetectorRegionC> > rit(chartModel,sceneModel);rit;rit++) {
      regions += rit.Data1().Size();
      // Go through regions in model looking for closetest match in scene.
      
      for(SArray1dIterC<ChartDetectorRegionC> mit(rit.Data1());mit;mit++) {
	for(SArray1dIterC<ChartDetectorRegionC> sit(rit.Data2());sit;sit++) {
	  Point2dC newPnt = transform * sit->Centroid();
	  RealT dist = mit->Centroid().SqrEuclidDistance(newPnt);
	  if(dist < inlierDistanceThreshold) {
	    //cerr << "Dist=" << dist << "\n";
	    inliers++; // Found inlier.
	    break;
	  }
	}
      }
    }
    //cerr << "Inlier=" << inliers << "\n";
    return static_cast<RealT>(inliers) / static_cast<RealT>(regions);
  }
  
  //: Generate a list of inliers
  //!param: transform - Transform hypothesis
  //!param: imageRegions - Regions from the current image.
  //!return: List of matches, chart regions to image regions.
  
  DListC<Tuple2C<ChartDetectorRegionC,ChartDetectorRegionC> > ChartDetectorBodyC::ListInliers(const Affine2dC &transform,SArray1dC<SArray1dC<ChartDetectorRegionC> > &sceneModel) {
    DListC<Tuple2C<ChartDetectorRegionC,ChartDetectorRegionC> > ret;
    
    for(SArray1dIter2C<SArray1dC<ChartDetectorRegionC>,SArray1dC<ChartDetectorRegionC> > rit(chartModel,sceneModel);rit;rit++) {
      
      // Go through regions in model looking for closetest match in scene.
      for(SArray1dIterC<ChartDetectorRegionC> mit(rit.Data1());mit;mit++) {
	RealT minDist = inlierDistanceThreshold;
	ChartDetectorRegionC bestRegion;
	for(SArray1dIterC<ChartDetectorRegionC> sit(rit.Data2());sit;sit++) {
	  Point2dC newPnt = transform * sit->Centroid();
	  RealT dist = mit->Centroid().SqrEuclidDistance(newPnt);
	  if(dist >= minDist)
	    continue;
	  minDist = dist;
	  bestRegion = *sit;
	}
	if(minDist < inlierDistanceThreshold)
	  ret.InsLast(Tuple2C<ChartDetectorRegionC,ChartDetectorRegionC>(*mit,bestRegion));
      }
    }
    return ret;
  }
  
  
  //: Search for the chart in an image.
  //!param:transform - Estimate of the projective transform of the chart.
  //!return:Returns true if chart has been found.
  
  bool ChartDetectorBodyC::Apply(const ImageC<ByteT> &sceneImage,Projection2dC &transform) {
    ONDEBUG(cerr << "ChartDetectorBodyC::Apply(), Called. \n");
    
    SArray1dC<SArray1dC<ChartDetectorRegionC> > sceneModel = BuildArrays(FeatureExtract(sceneImage));
    
    // Lets go RANSAC the scene
    
    // Build an array to even out distribution of region types we use.
    
    UIntT distArray = 100;
    SizeT total = 0;
    UIntT bias = 4; // This adds a slight bias towards more unusual regions.
    
    for(SArray1dIterC<SArray1dC<ChartDetectorRegionC> > it(chartModel);it;it++)
      total += it->Size();
    UIntT stotal = 0;
    for(SArray1dIterC<SArray1dC<ChartDetectorRegionC> > it(chartModel);it;it++) {
      if(it->Size() == 0)
	continue;
      stotal += ((it->Size() * distArray) / total) + bias;
    }
    SArray1dC<UIntT> rtDist(stotal);
    UIntT at = 0;
    for(SArray1dIterC<SArray1dC<ChartDetectorRegionC> > it(chartModel);it;it++) {
      if(it->Size() == 0)
	continue;
      UIntT next = ((it->Size() * distArray) / total) + bias;
      for(UIntT i = 0;i < next;i++)
	rtDist[at++] = it.Index().V();
    }
    RavlAssert(at == stotal);
    ONDEBUG(cerr << "Total dist bins=" << stotal << "\n");
    
    // Setup pointsets for RANSAC.
    
    UIntT noPoints = 3;
    SArray1dC<Point2dC> scenePos(noPoints);
    SArray1dC<Point2dC> modelPos(noPoints);
    
    Affine2dC bestAffine;
    RealT bestScore = 0;
    RealT bestCorrelation = 0;
    IntT bestIter = 0;
    
    ONDEBUG(cerr << "ChartDetectorBodyC::Apply(), Starting RANSAC. \n");
    
    for(UIntT i = 0;i < ransacIterations;i++) {
      //cerr << "ChartDetectorBodyC::Apply(), Iter=" << i << " \n";
      
      // Pick some point correspondances.
      
      for(UIntT j = 0;j < noPoints;j++) {
	UIntT regionType = rtDist[RandomInt() % rtDist.Size()];
	if(sceneModel[regionType].Size() == 0) { // Make sure there's some in the scene.
	  j--;
	  continue;
	}
	UIntT ind1 = RandomInt() % chartModel[regionType].Size();
	UIntT ind2 = RandomInt() % sceneModel[regionType].Size();
	modelPos[j] = chartModel[regionType][ind1].Centroid();
	scenePos[j] = sceneModel[regionType][ind2].Centroid();
      }
      
      //cerr << "Fitting Affine. \n";
      // Should fit a projective transform here.
      Affine2dC affine;
      try {
	affine = FitAffine(scenePos,modelPos);
      } catch(ExceptionNumericalC &) {
	//cerr << "Reject. \n";
	// Reject hypothesis for numerical reasons.
	continue;
      }
      
      // Check for mirror images or transforms with very high shear.
      
      if(affine.OrthogonalCrossProduct() <= 0.0001)
	continue; // Just ignore it.
      
      // Test hypothesis.
      
      //cerr << "Test hypothesis. \n";
      
      RealT inlierFraction = TestHypothesis(affine,sceneModel);
      if(inlierFraction <= bestScore) 
	continue;
      //cerr << "Model=" << modelPos << "\n";
      //cerr << "Image=" << scenePos << "\n";
      
      // Lets take a closer look...
      
      WarpAffineC<ByteT> warp(exampleImage.Frame(),bestAffine.I());
      ImageC<ByteT> result = warp.Apply(sceneImage);
      RealT corr = NormalisedCorrelation(exampleImage,result);
      //if(corr < bestCorrelation) 
      //	continue;
      
      //cerr << "Score=" << score << "\n";
      if(corr > acceptCorrelationThreshold)
	break;
      
      bestAffine = affine;
      bestScore = inlierFraction;
      bestIter = i;
      bestCorrelation = corr;
    }

    ONDEBUG(cerr << "ChartDetectorBodyC::Apply(), Affine Hypothesis. BestScore=" << bestScore << " Iter=" << bestIter << " Best=" << bestAffine << " \n");
    
    if(bestScore < 0.25)
      return false;
    
#if 0
    WarpAffineC<ByteT> warp(exampleImage.Frame(),bestAffine.I());
    ImageC<ByteT> result = warp.Apply(sceneImage);
    ONDEBUG(cerr << "ResultSize=" << result.Frame() << "\n");
    Save("@X:Target",result);
#endif
    
    // Get a list of inlier matches.
    
    DListC<Tuple2C<ChartDetectorRegionC,ChartDetectorRegionC> > inliers = ListInliers(bestAffine,sceneModel);
    
    if(inliers.Size() < 6) {
      cerr << "WARNING: Insufficient inliers detected (" << inliers.Size() << ") \n";
      return false;
    }
    
    transform = FitProjection(inliers);
    
    //cerr << "ChartDetectorBodyC::Apply(), Projective Hypothesis=" << transform << " \n";
    
    return true;
  }
  
  //: Fit a projective transform to the matches.
  
  Projection2dC ChartDetectorBodyC::FitProjection(DListC<Tuple2C<ChartDetectorRegionC,ChartDetectorRegionC> > &matches) {
    DListC<Point2dPairObsC> matchList;
    MatrixRSC modelNoise = MatrixRSC(2);
    modelNoise.Fill(0);
    modelNoise[0][0] = 0.0001;
    modelNoise[1][1] = 0.0001;
    
    MatrixRSC sceneNoise = MatrixRSC(2);
    sceneNoise.Fill(0);
    sceneNoise[0][0] = 1;
    sceneNoise[1][1] = 1;
    
    for(DLIterC<Tuple2C<ChartDetectorRegionC,ChartDetectorRegionC> > it(matches);it;it++) {
      matchList.InsLast(Point2dPairObsC(it->Data1().Centroid(),modelNoise,
					it->Data2().Centroid(),sceneNoise));
    }
    StateVectorHomog2dC stateVector;
    try {
      stateVector = Optimise2dHomography (matchList,200,200);
    } catch(ExceptionC &) {
      ONDEBUG(cerr << "Numerical problem detected. \n");
    }
    
    return Projection2dC(stateVector.GetHomog(),stateVector.GetZH1(),stateVector.GetZH2());
  }
  
  
}
