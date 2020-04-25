// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Edges/EdgeNonMaxSuppression.cc"

#include "Ravl/config.h"
#include "Ravl/Image/EdgeNonMaxSuppression.hh"
#include "Ravl/Array2dIter3.hh"
#include "Ravl/Array2dSqr3111Iter4.hh"
#include "Ravl/Array2dSqr3311Iter4.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/StdMath.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/StdConst.hh"
#include "Ravl/Image/DrawFrame.hh"


#if RAVL_USE_PARALLEL
#include "Ravl/Threads/Launch.hh"
using namespace PThread;
static const IntT numberOfCPUs = 4;
#endif

namespace RavlImageN {
  using namespace RavlN;
  
  // Check for diagonal
  static inline bool IsDiagonal(RealT A,RealT xp,RealT yp,RealT xyp,RealT dr,RealT dc) {
    RealT adr = Abs(dr);
    RealT adc = Abs(dc);
    return A > xp + ((A+xp+yp+xyp)/4 - xp)*adc/(adr+adc);
  }

  bool EdgeNonMaxSuppressionBodyC::Apply(const ImageC<RealT> & inDrIm, 
					 const ImageC<RealT> & inDcIm,  
					 const ImageC<RealT> & inGrad,
					 ImageC<RealT> & res,
					 RealT &meanVal,
					 IntT &count
					 ) const {
    ImageRectangleC rect(inDrIm.Rectangle());
    rect.ClipBy(inDcIm.Rectangle());
    rect.ClipBy(inGrad.Rectangle());
    RavlAssert(rect.Area() > 0);
    if(res.IsEmpty())
      res = ImageC<RealT>(rect);
    else
      rect.ClipBy(res.Rectangle());
    rect.Erode();
    RavlAssert(rect.Area() > 0); // make sure there's something left.
    meanVal = 0;
    count = 0;
#if RAVL_USE_PARALLEL
    IntT nrows = rect.Rnum();
    IntT ncols = rect.Cnum();
    IntT numChunks = numberOfCPUs;
    IntT chunkSize = nrows / numChunks;
    if(chunkSize < 20 || ncols < 50) {
#endif
      // Not worth doing anything clever.
      
      DoNonMaxSupp(res,meanVal,
		   count,
		   inDrIm,inDcIm,inGrad,
		   rect.TRow().V(),rect.BRow().V());
#if RAVL_USE_PARALLEL
      return Tuple2C<ImageC<RealT>,RealT>(res,meanVal);
    }
    
    // Ok do it in parrallel.
    int endRow,startRow = rect.TRow().V();
    SArray1dC<LaunchEventC> events(numChunks);
    SArray1dC<RealT> meanValues(numChunks);
    SArray1dC<IntT> countValues(numChunks);
    EdgeNonMaxSuppressionC me(*this);
    
    ImageC<RealT> img1(imgs.Data1());
    ImageC<RealT> img2(imgs.Data2());
    
    for(int i = 0; i< numChunks;i++) {
      endRow = startRow + chunkSize;
      if(endRow > rect.BRow())
	endRow = rect.BRow().V();
      events[i] = ThreadLaunch(me,&EdgeNonMaxSuppressionC::DoNonMaxSupp,res,meanValues[i],countValues[i],imgs,startRow,endRow);
      startRow = endRow+1;
    }
    // Wait for everything to finish.
    meanVal = 0;
    for(int i = 0; i< numChunks;i++) {
      events[i].Done().Wait();
      meanVal += meanValues[i];
      count += countValues[i];
    }
    meanVal /= numChunks;
#endif
    return true;
  }

  //: Apply non-maximal suppression to edge images.
  // return a list of edgels.
  
  bool EdgeNonMaxSuppressionBodyC::Apply(const ImageC<RealT> & inDrIm, 
					 const ImageC<RealT> & inDcIm,  
					 const ImageC<RealT> & inGrad,
					 SArray1dC<EdgelC> & outEdges,
					 RealT &mean) const {
    
    ImageC<RealT> res;
    int edgels;
    if(!Apply(inDrIm,inDcIm,inGrad,res,mean,edgels))
      return false;
    outEdges = SArray1dC<EdgelC>(edgels); 
    SArray1dIterC<EdgelC> eit(outEdges);
    for(Array2dIterC<RealT> it(res);it;it++) {
      if(*it == 0)
	continue;
      Index2dC at = it.Index();
      RavlAssert(eit); // outEdgels miscounted!
      *eit = EdgelC(at,inDcIm[at],inDrIm[at],res[at]);
      eit++;
    }
    RavlAssert(!eit); // outEdgels miscounted!
    return true;
  }
  
  
  void EdgeNonMaxSuppressionBodyC::DoNonMaxSupp(ImageC<RealT> &res,
						RealT &meanRes,
						IntT &points,
						const ImageC<RealT> & drIm, 
						const ImageC<RealT> & dcIm,  
						const ImageC<RealT> & grad,
						IntT startRow,IntT endRow
						) const {
    RealT meanVal = 0;
    points = 0;
    ImageC<RealT> & nonMax = res;
    
    ImageRectangleC rect(res.Rectangle());
    rect.Origin().Row() = startRow;
    rect.End().Row() = endRow;
    
    DrawFrame(res,0.0,res.Frame()); // Fill boundary with zeros
    
    // Search of local maxima with linear interpolation
    // =================================================
    
    // The value of grad in the center is compared with the value 
    // in the direction determined by dx and dc which is estimated
    // using linear interpolation of values in the neighborhood of the
    // center value.
    
    //      * 5 * 4 *           o -- dc      top
    //      6       3           |
    //      *   *   *           dr           mid
    //      7       2
    //      * 8 * 1 *                        bot
    
    // It should be checked for 8 possibilities (octants), but 
    // the they are symmetrical.
    // The expression (yx-ya) * dy > (yb-ya)*dx   is used instead of
    // yx > ya + (yb-ya) * dx/dy.
    
    for(Array2dSqr3111Iter4C<RealT,RealT,RealT,RealT> it(grad,nonMax,drIm,dcIm);it;it++) {
      bool    isMaximum = false;
      RealT dr  = it.Data3(); // direction of gradient
      RealT dc  = it.Data4();
      RealT cen = it.Data1();
      if(dc == 0.0 && dr == 0.0) {
	it.Data2() = 0.0;
	continue;
      }
      if (dc < 0) { // work in octants 1,2,3,4 by making dc positive
	dc = -dc;
	dr = -dr;
      }
      if(dr >= 0) { // octant 1 or 2 
	if(dr > dc) // octant 1
	  isMaximum =   
 	       (it.DataBR1()-it.DataBM1())*dc <= (cen-it.DataBM1())*dr
	    && (it.DataTL1()-it.DataTM1())*dc <  (cen-it.DataTM1())*dr; 
	
	else  // octant 2
	  isMaximum =   
	       (it.DataBR1()-it.DataMR1())*dr <  (cen-it.DataMR1())*dc  
	    && (it.DataTL1()-it.DataML1())*dr <= (cen-it.DataML1())*dc; 
      } else { // octant 3 or 4 
	if(-dr < dc) // octant 3
	  isMaximum =   
	       (it.DataML1()-it.DataBL1())*dr <= (cen-it.DataML1())*dc 
	    && (it.DataMR1()-it.DataTR1())*dr <  (cen-it.DataMR1())*dc;
	else // octant 4
	  // comparison reversed by negative dr
	  isMaximum =
	       (it.DataBM1()-it.DataBL1())*dc >= (cen-it.DataBM1())*dr
	    && (it.DataTM1()-it.DataTR1())*dc >  (cen-it.DataTM1())*dr;
      }
      if (isMaximum)  {
	it.Data2() = cen;
	meanVal += cen;
	points++;
      } else
	it.Data2() = 0.0;
    }
    
    
    if (eightConnectivity) {
      for(Array2dSqr3311Iter4C<RealT,RealT,RealT,RealT> it(grad,nonMax,drIm,dcIm);it;it++) {
	if(it.Data2() == 0)
	  continue;
	RealT i1 = it.DataTM2();
	RealT i2 = it.DataML2();
	RealT i3 = it.DataBM2();
	RealT i4 = it.DataMR2();
	if(!(((i1  > 0) && (i2  > 0)) || ((i2  > 0) && (i3  > 0)) ||
	     ((i3  > 0) && (i4  > 0)) || ((i4  > 0) && (i1  > 0))))
	  continue;
	bool isMaximum = false;
	RealT dr = it.Data3();
	RealT dc = it.Data4();
	RealT cen = it.Data1();
	if (Abs(dr) >= Abs(dc)) {
	  if (dr*dc > 0)
	    isMaximum = IsDiagonal(cen,it.DataMR1(),it.DataBM1(),it.DataBR1(),dr,dc)
	             && IsDiagonal(cen,it.DataML1(),it.DataTM1(),it.DataTL1(),dr,dc);
	  else
	    isMaximum = IsDiagonal(cen,it.DataMR1(),it.DataTM1(),it.DataTR1(),dr,dc)
	             && IsDiagonal(cen,it.DataML1(),it.DataBM1(),it.DataBL1(),dr,dc);
	} else {
	  if (dr*dc > 0)
	    isMaximum = IsDiagonal(cen,it.DataBM1(),it.DataMR1(),it.DataBR1(),dc,dr)
 	             && IsDiagonal(cen,it.DataTM1(),it.DataML1(),it.DataTL1(),dc,dr);
	  else 
	    isMaximum = IsDiagonal(cen,it.DataBM1(),it.DataML1(),it.DataBL1(),dc,dr)
	             && IsDiagonal(cen,it.DataTM1(),it.DataMR1(),it.DataTR1(),dc,dr);
	}
	if (!isMaximum) { 
	  it.Data2() = 0;
	  meanVal -= cen;
	  points--;
	}
      } 
    }
    
    if(points != 0)
      meanRes = meanVal / points;
    else
      meanRes = 0;
  }
  
}
