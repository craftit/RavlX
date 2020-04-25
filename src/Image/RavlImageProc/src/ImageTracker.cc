// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Tracking/ImageTracker.cc"

#include "Ravl/Image/ImageTracker.hh"
#include "Ravl/Image/WarpProjective.hh"
#include "Ravl/Image/MatchPatch.hh"
#include "Ravl/Image/DrawCross.hh"

#define DODEBUG 0
#if DODEBUG
#include "Ravl/IO.hh"
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {
  
  //: Constructor.
  
  ImageTrackerC::ImageTrackerC(IntT nPatchSize,IntT aSearchSize,IntT nMatchThreshold)
    : cornerDet(10),
      patchSize(nPatchSize),
      searchSize(aSearchSize),
      matchThreshold(nMatchThreshold)
  {}
  
  DListC<PairC<Point2dC> > ImageTrackerC::TrackImage(const ImageC<ByteT> &mosaic,const ImageC<ByteT> &data,const Projection2dC &proj) {
    DListC<PairC<Point2dC> > ret;
    // Find corners.
    
    IndexRange2dC roi = data.Frame().Shrink(patchSize);
    
    DListC<CornerC> corners = cornerDet.Apply(data);
    ONDEBUG(std::cerr << "Corners=" << corners.Size() << "\n");
    
    Projection2dC iproj = proj.Inverse();
    
    WarpProjectiveC<ByteT> warp(iproj);
    
    // Generate patches.

    Point2dC rat;  
    IntT sum;
    ONDEBUG(ByteT white = 255);
    ONDEBUG(ImageC<ByteT> out(mosaic.Copy()));
    IntT removeThresh = Sqr(patchSize) * matchThreshold;

    for(DLIterC<CornerC> it(corners);it;it++) {
      if(!roi.Contains(it->Location()))
	continue;
      
      Point2dC cpnt = it->Location();
      
      // Project corner into mosiac space.
      Point2dC projLoc = iproj * cpnt;
      
      // Project discretised location back into the image to get true location of patch.
      Index2dC patchCenter(projLoc);
      Point2dC trueLoc = proj * Point2dC(patchCenter);
      
      // Create a patch
      IndexRange2dC fr(patchCenter,patchSize,patchSize);
      ImageC<ByteT> patch(fr);
      warp.Apply(data,patch);

      // centre the patch frame around zero
      Array2dC<ByteT> patchOffset = patch.CopyAccess(-patchCenter.Row(),-patchCenter.Col());

      // Match patches to original image.
      SearchMinAbsDifferenceCreep(patchOffset,mosaic,projLoc,rat,sum,searchSize);
      ONDEBUG(std::cerr << "Thresh=" << sum << "\n");
      // Put matches in the list.
      if(sum < removeThresh) {
	// correct coordinates to centre of pixel
	rat[0] += 0.5;
	rat[1] += 0.5;
	trueLoc[0] += 0.5;
	trueLoc[1] += 0.5;
	ret.InsLast(PairC<Point2dC>(trueLoc,rat));
      }
      ONDEBUG(DrawCross(out,white,rat,5));
    }
    ONDEBUG(RavlN::Save("@X",out));
    return ret;
  }
  
}
