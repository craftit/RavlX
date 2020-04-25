// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Tools/PyramidScan.cc"

#include "Ravl/Image/PyramidScan.hh"
#include "Ravl/Math.hh"
#include "Ravl/Stream.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {
#if RAVL_VISUALCPP_NAMESPACE_BUG
  using namespace RavlN;
#endif
  
  //: Setup a scan.
  // frame - Rectangle to be scanned.
  // roiSize - Size of rectangle to start from.
  // scale - Size of scale steps.
  // step - fraction of roi to step through when scanning an image.
  // levels - number of levels to scan.
  
  PyramidScanC::PyramidScanC(const IndexRange2dC &nframe,Index2dC nroiSize,IntT minSize,RealT nscale,RealT nstep)
    : frame(nframe),
      roiSize(nroiSize),
      scale(nscale),
      step(nstep)
  { 
    levels=NoLevels(minSize);
    ONDEBUG(cerr << "Levels=" << levels << "\n");
    First(); 
  }
  
  //: Calculate number of levels before minSize is reached.
  
  IntT PyramidScanC::NoLevels(IntT minSize) {
    RavlAssert(scale > 0);
    RavlAssert(roiSize.Col() > 0);
    IntT l = 0;
    for(;;l++) {
      RealT lscale = Pow(scale,l);
      IntT cols = (IntT)(lscale * (RealT) roiSize.Col());
      ONDEBUG(cerr << "L=" << l << " " << cols << "\n");
      if(cols < minSize)
	break;
    }
    if(l < 1)
      return 1;
    return l;
  }
  
  //: Goto next row.
  
  bool PyramidScanC::NextRow() {
    roi.Range2() = IndexRangeC(frame.Range2().Min(),
			       frame.Range2().Min() + rowSize);
    roi.Range1() += rowStep;
    if(roi.Range1().Max() <= frame.Range1().Max())
      return true;
    return First(++level);
  }
  
  //: Setup first iteration at given level.
  
  bool PyramidScanC::First(int level) {
    if(level >= levels)
      return false;
    RealT lscale = Pow(scale,level);
    RealT lstep = step * lscale;
    colStep = (IntT)(lstep * (RealT) roiSize.Col());
    rowStep = (IntT)(lstep * (RealT) roiSize.Row());
    Index2dC end = roiSize * lscale;
    end.Row()--;
    end.Col()--;
    rowSize = end.Row();
    ONDEBUG(cerr << "Level=" << level << " End=" << end << " ColStep=" << colStep << " RowStep=" << rowStep << "\n");
    roi = IndexRange2dC(frame.Origin(),frame.Origin() + end);
    return true;
  }
  

}
