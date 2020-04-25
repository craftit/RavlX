// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_PYRAMIDSCAN_HEADER
#define RAVLIMAGE_PYRAMIDSCAN_HEADER 1
///////////////////////////////////////////////////////
//! rcsid="$Id$"
//! author="Charles Galambos"
//! lib=RavlImageProc
//! docentry="Ravl.API.Images.Misc"
//! file="Ravl/Image/Processing/Tools/PyramidScan.hh"

#include "Ravl/IndexRange2d.hh"

namespace RavlImageN {
  using namespace RavlN;
  
  //! userlevel=Normal
  //: Scan an image from course to fine resolution.
  
  class PyramidScanC {
  public:
    PyramidScanC()
    {}
    //: Default constructor.
    
    PyramidScanC(const IndexRange2dC &nframe,Index2dC nroiSize,IntT minColSize,RealT nscale = 0.5,RealT nstep = 0.5);
    //: Setup a scan.
    // frame - Rectangle to be scanned.
    // roiSize - Size of rectangle to start from.
    // minColSize - Smallest number of columns for window.
    // scale - Size of scale steps.
    // step - fraction of roi to step through when scanning an image.
    
    RealT Scale() const
    { return scale; }
    //: Scale between scans.
    
    RealT Step() const
    { return step; }
    //: Step size in scan.
    
    IntT Level() const
    { return level; }
    //: Current level we're scanning.
    
    const IndexRange2dC &Frame() const
    { return frame; }
    //: Rectangle being scanned.
    
    const IndexRange2dC &ROI() const
    { return roi; }
    //: Get current Region Of Interest.
    
    IndexRange2dC &operator*()
    { return roi; }
    //: Get current Region Of Interest.
    
    bool First() {
      level=0;
      return First(level);
    }
    //: Reset to the first rectangle of the top level.
    
    bool First(int level);
    //: Setup first iteration at given level.
    
    bool Next() {
      roi.Range2() += colStep;
      if(roi.Range2().Max() <= frame.Range2().Max())
	return true;
      return NextRow();
    }
    //: Goto next region.
    
    void operator++(int)
    { Next(); }
    //: Goto next region.
    
    bool IsElm() const
    { return level < levels; }
    //: At a valid rectangle ?
    
    operator bool() const
    { return IsElm(); }
    //: At a valid rectangle ?
    
  protected:
    bool NextRow();
    //: Goto next row.
    
    IntT NoLevels(IntT minSize);
    //: Calculate number of levels before minSize is reached.
    
    IndexRange2dC frame; // Overall size of rectangle to scan.
    Index2dC roiSize;
    RealT scale; // Scale between scans.
    RealT step;  // Size of steps to scan through an image at.
    IntT levels; // Number of levels to scan.
    
    IntT level; // Current level we're scanning.
    IndexRange2dC roi; // Current roi
    IndexC rowSize;
    IntT colStep;
    IntT rowStep;
  };}


#endif
