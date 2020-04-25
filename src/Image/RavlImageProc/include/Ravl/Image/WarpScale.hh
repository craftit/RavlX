// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_WARPSCALE_HEADER
#define RAVLIMAGE_WARPSCALE_HEADER 1
///////////////////////////////////////////////////////////////
//! docentry="Ravl.API.Images.Scaling and Warping"
//! lib=RavlImageProc
//! author="Kieron Messer"
//! rcsid="$Id$"
//! date="05/05/1999"
//! file="Ravl/Image/Processing/Filters/Warp/WarpScale.hh"

#include "Ravl/Image/Image.hh"
#include "Ravl/Image/BilinearInterpolation.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/Vector2d.hh"
#include "Ravl/Point2d.hh"

namespace RavlImageN {
  
  //! userlevel=Normal
  //: Scaling modes
  // WARPSCALE_NEARESTNEIGHBOUR = Fast scaling using a nearest-neighbour algorithm. Performs very badly when scaling down. Don't use this for anything important.<BR>
  // WARPSCALE_BILINEAR = Bilinear interpolation - slower, but much more accurate.
  enum ScaleMethodT {
    WARPSCALE_NEARESTNEIGHBOUR,
    WARPSCALE_BILINEAR,
  };

  //:-
  
  //! userlevel=Normal
  //: Scale an image
  // Note: WarpScale2d is now preferred.
  
  template <class InT, class OutT = InT>
  class WarpScaleC
  {
  public:
    WarpScaleC(const ImageRectangleC &outputRectangle, ScaleMethodT meth = WARPSCALE_BILINEAR)
      : rec(outputRectangle),
	method(meth)
    {}
    //: Constructor.
    // 'outputRectangle' is the output rectangle.
    // 'meth' is the method to use
    
    ImageC<OutT> Apply(const ImageC<InT> &img);
    //: Rescale input image working rectangle into
    //: output image rectangle.
    // The output rectangle and scaling method are specified in the constructor.
    
    void SetMethod(ScaleMethodT meth)
    { method = meth; }
    //: Set the scaling method
    
    void SetRectangle(const ImageRectangleC &nrec)
    { rec = nrec; }
    //: Set output rectangle.
    
    const ImageRectangleC &OutputRectangle()
    { return rec; }
    //: Access output rectangle.
    
    ImageC<OutT> Apply(const ImageC<InT> &img,const Point2dC &off,const Vector2dC &size);
    //: Scale.
    // Return a scaled image 
    //!param: img - Image to sample
    //!param: off - offset to start sampling at.
    //!param: size - size of rectangle to sample
    
  protected:
    
    ImageC<OutT> NearestNeighbour(const ImageC<InT> &img);
    //: Quick-scale input image working rectangle into
    //: output image rectangle.

    ImageC<OutT> BiLinear(const ImageC<InT> &img);
    //: Interpolate input image working rectangle into
    //: output image rectangle.

    ImageRectangleC rec;    

    ScaleMethodT method;
  };
  
  template <class InT, class OutT>
  ImageC<OutT> WarpScaleC<InT, OutT>::Apply(const ImageC<InT> &im)
  {
    switch (method) {
    case WARPSCALE_NEARESTNEIGHBOUR:
      return NearestNeighbour(im);
    case WARPSCALE_BILINEAR:
    default:
      return BiLinear(im);
    }
  }
  
  template <class InT, class OutT>
  ImageC<OutT> WarpScaleC<InT, OutT>::NearestNeighbour(const ImageC<InT> &im)
  {
    ImageC<OutT> res(rec);
    
    IntT imRows  = im.Rows();
    IntT imCols  = im.Cols();
    
    RealT jOff = im.LCol();
    RealT j,i = im.TRow();
    
    // First check for degenerate cases...
    
    if(imRows <= 0 && imCols <= 0) // Empty image.
      return res; 
    
    if(imRows == 1 && imCols == 1) { // Just one pixel.
      res[res.Rectangle().Origin()] = im[im.Rectangle().Origin()];
      return res;
    }
    
    // Calculate steps
    RealT stepC = ((RealT)imCols-1.000001)/(RealT)rec.Cols();
    RealT stepR = ((RealT)imRows-1.000001)/(RealT)rec.Rows();
 
    // Setup
    IntT iRow, iCol;
    // Scale rectangle
    for(Array2dIterC<OutT> it(res);it.IsElm();) {
      // Get row index
      iRow = Round(i);
      // Setup 
      j = jOff;
      // Rescale column
      do {
	// Get column index
	iCol = Round(j);
	// Copy pixel
	it.Data() = (OutT)(im[iRow][iCol]);
	// Step
	j += stepC; 
      } while(it.Next()); // True while in same row.
      // Step
      i += stepR;      
    }
    return res;
  }
  
  template <class InT, class OutT>
  ImageC<OutT> WarpScaleC<InT, OutT>::Apply(const ImageC<InT> &img,const Point2dC &off,const Vector2dC &rectSize) {
    ImageC<OutT> res(rec);
    Point2dC rowStart = off;
    Vector2dC scale = rectSize / Point2dC(res.Frame().Rows(),res.Frame().Cols());
    for(Array2dIterC<OutT> it(res);it;) {
      Point2dC at = rowStart;
      do {
        BilinearInterpolation(img,at,*it);
        at[1] += scale[1];
      } while(it.Next()); // True while in same row.
      rowStart[0] += scale[0];
    }
    return res;
  }
  
  //: Scale.
  // Return a scaled image 
  
  template <class InT, class OutT>
  ImageC<OutT> WarpScaleC<InT, OutT>::BiLinear(const ImageC<InT> &im)
  {
    ImageC<OutT> res(rec);
    
    IntT imRows  = im.Rows();
    IntT imCols  = im.Cols();
    
    Index2dC y1,y2,y3,y4;
    RealT t, u;
    RealT minC, minR;
    RealT jOff = im.LCol();
    RealT j,i = im.TRow();
    
    // First check for degenerate cases...
    
    if(imRows <= 0 && imCols <= 0) // Empty image.
      return res; 
    
    if(imRows == 1 && imCols == 1) { // Just one pixel.
      res[res.Rectangle().Origin()] = im[im.Rectangle().Origin()];
      return res;
    }
    
    RealT stepC = ((RealT)imCols-1.000001)/(RealT)rec.Cols();
    RealT stepR = ((RealT)imRows-1.000001)/(RealT)rec.Rows();
    
    if(imRows == 1) { // Just one row.
      y3.Row() = im.Rectangle().Origin().Row();
      y4.Row() = y3.Row();
      j = jOff;
      for(Array2dIterC<OutT> it(res);it.IsElm();it.Next()) {
	minC = Floor(j);
	t = (j - minC);
	j += stepC; 
	
	IntT iMinC = (IntT) minC;
	y4.Col() = iMinC;
	y3.Col() = iMinC+1;
	if(iMinC == im.Rectangle().RCol()) {// Hack!!!
	  cerr << "ERROR: Overflow in interpolation (C). \n";
	  break;
	}
	
	it.Data() = (OutT)((im[y4] * (1.0-t)) + 
			   (im[y3] * t ) 
			   );
      }
      return res;
    }
    
    if(imCols == 1) { // Just one column.    
      y2.Col() = im.Rectangle().Origin().Col();
      y4.Col() = y2.Col();
      for(Array2dIterC<OutT> it(res);it.IsElm();it.Next()) {
	minR = Floor(i);
	u = (i - minR);
	i += stepR;
	IntT iMinR = (IntT) minR;
	y4.Row() = iMinR;
	y2.Row() = iMinR+1;
	if(iMinR == im.Rectangle().BRow()) {// Hack!!!
	  cerr << "ERROR: Overflow in interpolation (R) \n";
	  break;
	}
	it.Data() = (OutT)((im[y4] * (1.0-u)) + 
			   (im[y2] * u));
      }
      
      return res;
    }
    
    for(Array2dIterC<OutT> it(res);it.IsElm();) {
      minR = Floor(i);
      u = (i - minR);
      i += stepR;
      
      IntT iMinR = (IntT) minR;
      j = jOff;
      
      y1.Row() = iMinR+1;
      y2.Row() = iMinR+1;
      y3.Row() = iMinR;
      y4.Row() = iMinR;
      
      do {
	minC = Floor(j);
	t = (j - minC);
	j += stepC; 
	
	IntT iMinC = (IntT) minC;
	
	y1.Col() = iMinC+1; 
	y3.Col() = iMinC+1;
	y2.Col() = iMinC;
	y4.Col() = iMinC;
	
	it.Data() = (OutT)((im[y4] * ((1.0-t) * (1.0-u))) + 
			   (im[y3] * (     t  * (1.0-u))) + 
			   (im[y2] * ((1.0-t) *      u )) +
			   (im[y1] * (     t  *      u )) 
			   );
      } while(it.Next()); // True while in same row.
      
    }
    return res;
  }
  
}
#endif

