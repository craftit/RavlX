// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlChartDetector
//! file="Ravl/Image/Processing/ChartDetector/ChartDetectorRegion.cc"

#include "Ravl/Image/ChartDetectorRegion.hh"
#include "Ravl/IO.hh"
#include "Ravl/Boundary.hh"

#define DODEBUG	0

#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {
  
  //: Region model.
  
  ChartDetectorRegionBodyC::ChartDetectorRegionBodyC(const SegmentationC &aSegmentation,UIntT nLabel,UIntT nRegionType)
    : segmentation(aSegmentation),
      label(nLabel),
      regionType(nRegionType)
  { UpdateStats(); }
  
  //: Update stats on region.
  
  void ChartDetectorRegionBodyC::UpdateStats() {
    // Compute the number of holes in the region.
    BoundaryC boundary(static_cast<Array2dC<UIntT> >(segmentation.SegMap()),1);
    DListC<BoundaryC> edges = boundary.OrderEdges();
    if(edges.Size() == 0) {
      RavlN::Save("@X:Region",segmentation.SegMap());
      cerr << "ChartDetectorRegionBodyC::UpdateStats(), WARNING: Zero boundries for region ?!? \n";
      holes = 0;
    } else
      holes = edges.Size() - 1;
    //cerr << "Holes=" << holes << "\n";
    // Could do more with the moments...
    SArray1dC<Moments2d2C> momentArray = segmentation.ComputeMoments(true);
    centroid = momentArray[1].Centroid();
  }
  
  
}
