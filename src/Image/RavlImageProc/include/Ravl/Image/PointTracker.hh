// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_POINTTRACKER_HEADER
#define RAVLIMAGE_POINTTRACKER_HEADER 1
//! userlevel=Normal
//! author="Charles Galambos"
//! date="24/7/2002"
//! rcsid="$Id$"
//! docentry="Ravl.API.Images.Tracking"
//! file="Ravl/Image/Processing/Tracking/PointTracker.hh"
//! lib=RavlImageProc
//! example="tracker.cc"

#include "Ravl/Image/Image.hh"
#include "Ravl/Image/CornerDetector.hh"
#include "Ravl/DList.hh"
#include "Ravl/Image/PointTrack.hh"
#include "Ravl/Image/PointTrackModel.hh"
#include "Ravl/RCHash.hh"

namespace RavlImageN {
  
  //! userlevel=Normal
  //: Point tracker.
  // It currently uses the Susan corner detector.

  class PointTrackerC {
  public:
    PointTrackerC(int cthreshold = 20,
		  int cwidth = 7,
		  RealT nmthreshold = 20,
		  int nmwidth = 15,
		  int lifeTime = 8,
		  int searchSize = 25,
		  int newFreq = 10,
		  bool useNormCorrelation = false
		  );
    //: Constructor.
    //!param: cthreshold - minimum o/p from corner detector to be classified as corner
    //!param: cwidth - corner filter width
    //!param: nmthreshold - minimum o/p from correlator to be classified as match
    //!param: nmwidth - size of correlator patch; also minimum distance between features
    //!param: lifeTime - lifetime of a point without a match in the incoming images
    //!param: searchSize - maximum distance it will look from predicted position of feature
    //!param: newFreq - introduces new features every "newFreq" frames
    //!param: useNormCorrelation - use normalised correlation for matching
    // <b>N.B.:</b> When using normalised correlation, nmthreshold should be set to around 2.
    
    RCHashC<UIntT,PointTrackC> Apply(const ImageC<ByteT> &img);
    //: Returns a list of tracks.
    // Same as update, but returns the track list.
    
    RCHashC<UIntT,PointTrackC> GetTracks() const;
    //: Generate a hashtable of current tracks.
    
    void AddTracks(const ImageC<ByteT> &img, ImageC<ByteT>* debugimg = NULL);
    //: Detects new point tracks
    // Only use this if you want to manually force track creation, normally when newFreq is very large
    // The second pointer is for a debugging image, and is for internal use only.
    
    IntT Update(const ImageC<ByteT> &img);
    //: Update track with information from an image.    
    // New tracks will be created automatically every newFreq frames.
    
    DListC<PointTrackModelC> &Tracks()
    { return tracks; }
    //: Direct access to current tracks list.
    
    const DListC<PointTrackModelC> &Tracks() const
    { return tracks; }
    //: Direct access to current tracks list.
    
    IntT FrameNo() const
    { return frameCount; }
    //: Access the current frame number.
    
  protected:
    UIntT idAlloc;
    IntT newFreq; // new features are introduced every newFreq frames
    IntT frameCount;
    CornerDetectorC cornerDet;
    IntT mwidth;
    RealT mthreshold;
    IntT lifeTime;
    IntT searchSize;
    DListC<PointTrackModelC> tracks; // Set of tracks being considered.
    bool useNormCorrelation;
  };
}


#endif
