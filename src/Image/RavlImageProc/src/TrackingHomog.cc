// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#include "Ravl/Image/TrackingHomog.hh"
#include "Ravl/IO.hh"
#include "Ravl/Image/ImgIO.hh"
#include "Ravl/Ransac.hh"
#include "Ravl/HashIter.hh"
#include "Ravl/ObservationHomog2dPoint.hh"
#include "Ravl/LevenbergMarquardt.hh"
#include "Ravl/ObservationManager.hh"
#include "Ravl/StateVectorHomog2d.hh"
//! rcsid="$Id$"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Tracking/TrackingHomog.cc"

namespace RavlImageN {


  TrackingHomogBodyC::TrackingHomogBodyC(bool nVerbose)
    : tracker(30,7,20,17,8,25,1),
      zhomog(100), fitHomog2d(zhomog,zhomog), // 100 is default projective scale
      epos(MatrixRSC::Identity(2)), // set corner error covariance matrix to 2x2 identity
      evalInliers(1.0,2.0),
      verbose(nVerbose)
  {}


  bool TrackingHomogBodyC::SetMask(const StringC& fileName) {
    if (!Load(fileName, mask)) {
      std::cerr << "Failed to load file: " << fileName << std::endl;
      return false;
    }
    return true;
  }


  Projection2dC TrackingHomogBodyC::Apply(const ImageC<ByteT> &img) {
    // Apply tracker to image.
    corners = tracker.Apply(img);
    // Generate an observation set for tracked points.
    DListC<ObservationC> obsList;
    for(HashIterC<UIntT,PointTrackC> it(corners);it;it++) {
      //      std::cout << "Confidence: " << it->Confidence() << std::endl;
      if(it->Confidence() < 0.1)
	continue; // Filter out points we haven't got recent info on.
      PointTrackC lat;
      if(!last.Lookup(it->ID(),lat))
	continue; // Matching point not found.
      
      const Point2dC& loc1=lat.Location();
      const Point2dC& loc2=it->Location();
      if (mask.IsEmpty() || (mask.Contains(loc1) && mask[Index2dC(loc1)])) {
	obsList.InsLast(ObservationHomog2dPointC(
            Vector2dC(loc1[0],loc1[1]),epos,Vector2dC(loc2[0],loc2[1]),epos));
      }
    }
    
    last = corners;

    try {
      FitHomog2dPointsC fitHomog2d(zhomog,zhomog);
      return fitHomog2d.FitModelRobust(obsList);
    }
    catch (...) {
    }
    return Projection2dC();
  }

}
