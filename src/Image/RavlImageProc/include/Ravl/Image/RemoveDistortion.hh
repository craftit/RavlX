// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_REMOVEDISTORTION_HEADER
#define RAVLIMAGE_REMOVEDISTORTION_HEADER 1
//! rcsid="$Id$"
//! lib=RavlImageProc
//! docentry="Ravl.API.Images.Scaling and Warping"
//! file="Ravl/Image/Processing/Filters/Misc/RemoveDistortion.hh"
//! author="Philip McLauchlan, Alex Kostin, Bill Christmas"
//! date="16/07/2002"

#include "Ravl/Image/PixelMixer.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/Array2dIter3.hh"
#include "Ravl/Matrix3d.hh"
#include "Ravl/Vector3d.hh"
#include "Ravl/Point2d.hh"
#include "Ravl/RealRange2d.hh"
#include "Ravl/LinePP2d.hh"
#include "Ravl/Image/BilinearInterpolation.hh"


namespace RavlImageN {

  // thresholds on change in radial distance R value
#define R_CHANGE_THRES 1.0e-7

  // minimum/maximum number of iterations to apply when applying inverse
  // distortion
#define MIN_UND_ITERATIONS   6
#define MAX_UND_ITERATIONS 100

  // threshold on closeness of camera coordinates to optical axis */
#define SMALL_DR_THRES 0.001

  //! userlevel=Normal
  //: Remove radial distortion from an image.
  // This method assumes the usual 2-parameter radial distortion model (e.g. as in CVPR 2001 v. I, pp. 125-132).  A look-up table is used for the interpolated pixel positions; this is initialised when the first image is processed.
  
  template <class InT, class OutT = InT,class MixerT = PixelMixerAssignC<InT,OutT> >
  class RemoveDistortionC
  {
  public:
    RemoveDistortionC()
    {}
    //: Default constructor.
    
    RemoveDistortionC(const IndexRange2dC &oRec,
		      RealT cRow, RealT cCol,
		      RealT fRow=1.0, RealT fCol=1.0,
		      RealT K1=0.0, RealT K2=0.0,
		      bool FillBackground = true,
		      const MixerT &mix = MixerT())
      : cx(cRow),
	cy(cCol),
	fx(fRow),
	fy(fCol),
	nK1(K1),
	nK2(K2),
	rec(oRec),
	fillBackground(FillBackground),
	mixer(mix)
    { Init(); }
    //: Constructor that specifies the output image frame.
    //!param: oRec - the output image rectangle
    //!param: cRow, cCol - image centre coordinates
    //!param: fRow, fCol - focal distance in vertical / horizontal pixels respectively
    //!param: K1 - cubic radial distortion coefficient. Typical value is around 10^-7.  &plusmn; values for concave / convex distortion respectively.
    //!param: K2 - quintic radial distortion coefficient. If used, typical value is 10^-14 (but set K1 first).
    //!param: FillBackground - fill background with black
    //!param: mix - pixel type for calculations internal to class
    
    RemoveDistortionC(RealT cRow, RealT cCol,
		      RealT fRow=1.0, RealT fCol=1.0,
		      RealT K1=0.0, RealT K2=0.0,
		      bool FillBackground = true,
		      const MixerT &mix = MixerT())
      : cx(cRow),
	cy(cCol),
	fx(fRow),
	fy(fCol),
	nK1(K1),
	nK2(K2),
	fillBackground(FillBackground),
	mixer(mix)
    { Init(); }
    //: Constructor that assumes output image dimensions are specified in the <code>void Apply(const ImageC<InT> &img,ImageC<OutT> &out)</code> method.
    // Otherwise, arguments as in previous constructor

    void Apply(const ImageC<InT> &img,ImageC<OutT> &out);
    //: Remove distortion from image `img' and write it into `out' 
    // The method assumes that image <code>out</code> already has a valid frame

    ImageC<OutT> Apply(const ImageC<InT> &img) {
      ImageC<OutT> out(rec);
      Apply(img,out);
      return out;
    }
    //: Remove distortion from image `img' and write it to returned image
    //: using image frame specified in the constructor.
    // <font color="red">Warning:</font> If this method is used, the output
    // frame <i>must</i> be specified in the constructor.

    Point2dC ProjectPoint(const Point2dC &pnti) const
    {
      /* compute squared radial distance R2 */
      RealT X2 = pnti[0]*pnti[0], Y2 = pnti[1]*pnti[1];
      RealT R2 = X2 + Y2, d;

      /* check if we are at a point outside the distortion model */
      if ( R2 > thres_dR )
	// use outer linear model
	d = outer_a + outer_b/sqrt(R2);
      else
	// calculate distortion and apply it to the image position
	d = 1.0 + (nK1 + nK2*R2)*R2;

      return Point2dC(cx + d*fx*pnti[0], cy + d*fy*pnti[1]);
    }
    //: Project point from ideal camera coordinates into image coordinates

    Point2dC ProjectPointLinear(const Point2dC &pnti) const
    {
      return Point2dC(cx + fx*pnti[0], cy + fy*pnti[1]);
    }
    //: Project point from ideal camera coordinates into image coordinates
    // Uses linear (pinhole) camera model

    Point2dC BackProjectPoint(const Point2dC &pnti) const
    {
      RavlAssert ( fx != 0.0 && fy != 0.0 );

      // compute distorted X/Y coordinates of scene point pnti on plane Z=1
      Point2dC pnto((pnti[0] - cx)/fx, (pnti[1] - cy)/fy);

      // compute distortion value
      RealT d = ComputeDistortion(pnto);
                                          
      // remove distortion from camera coordinates
      pnto /= d;
      return pnto;
    }
    //: Back-project point from image into ideal camera coordinates

    Point2dC BackProjectPointLinear(const Point2dC &pnti) const
    {
      RavlAssert ( fx != 0.0 && fy != 0.0 );

      // compute X/Y coordinates of scene point pnti on plane Z=1
      return Point2dC((pnti[0] - cx)/fx, (pnti[1] - cy)/fy);
    }
    //: Back-project point from image into ideal camera coordinates
    // Uses linear (pinhole) camera model

    Point2dC RemoveDist(const Point2dC &pnti) const;
    //: Remove distortion from a point

    Point2dC AddDist(const Point2dC &pnti) const;
    //: Add distortion to a point
    
  protected:
    void Init();
    RealT ComputeDistortion(const Point2dC &pnti) const;
    void CreateCoordsTable(const ImageRectangleC &InFrame, const ImageRectangleC &OutFrame);

    RealT cx, cy, fx, fy, nK1, nK2;
    IndexRange2dC rec;
    bool fillBackground;
    MixerT mixer;

    RealT thres_R2, thres_dR; // Thresholds on R^2and d*R
    RealT outer_a, outer_b; // Outer linear model parameters

    Array2dC<Point2dC> coordsTable;
    Array2dC<bool> isInImage;
    ImageRectangleC inFrame;
  };

  template <class InT, class OutT,class MixerT>
  void RemoveDistortionC<InT,OutT,MixerT>::Init() {
    // Compute threshold on radial camera coordinate to see if position
    // is within valid region
    if ( nK2 == 0.0 ) {
      thres_R2 = thres_dR = outer_a = outer_b = RavlConstN::maxReal;
    }
    else {
      RealT disc = 0.09*nK1*nK1 - 0.2*nK2;

      if ( disc > 0.0 ) {
	RealT R2a, R2b;

	disc = sqrt(disc);

	R2a = (-0.3*nK1 + disc)/nK2;
	R2b = (-0.3*nK1 - disc)/nK2;
	if ( R2a > 0.0 ) {
	  // two solutions: take solution closest to zero
	  if ( R2a < R2b ) thres_R2 = R2a;
	  else             thres_R2 = R2b;
	}
	else if ( R2b > 0.0 )
	  thres_R2 = R2b;
	else
	  thres_R2 = thres_dR = outer_a = outer_b = RavlConstN::maxReal;
      }
      else
	thres_R2 = thres_dR = outer_a = outer_b = RavlConstN::maxReal;
    }

    // check whether a limit on radial distance has been imposed
    if ( thres_R2 != RavlConstN::maxReal ) {
      double R = sqrt(thres_R2);

      // compute limited on distorted radial distance
      thres_dR = (1.0 + thres_R2*(nK1 + thres_R2*nK2))*R;

      // compute parameters of outer linear model
      outer_a = 1.0 + thres_R2*(3.0*nK1 + thres_R2*5.0*nK2);
      outer_b = thres_dR - outer_a*R;
    }
  }
  
  template <class InT, class OutT,class MixerT>
  RealT RemoveDistortionC<InT,OutT,MixerT>::ComputeDistortion(const Point2dC &pnti) const {
    RealT dR = sqrt(pnti[0]*pnti[0]+pnti[1]*pnti[1]);

    // return immediately with zero distortion if the distortion coefficients
    // are zero or the distorted radial distance is small
    if ( (nK1 == 0.0 && nK2 == 0.0) || dR < SMALL_DR_THRES )
      return 1.0;

    // check whether d*R is outside range of distortion model
    if ( dR > thres_dR )
      // use outer linear model
      // compute distortion value dR/R by solving linear equation for R
      return ( dR*outer_a/(dR - outer_b) );
                                          
    // if initialisation takes us outside the range of the camera,
    // move the start point back to the middle of the range
    RealT R;
    if ( dR*dR >= thres_R2 )
      R = 0.5*sqrt(thres_R2);
    else
      // initialise assuming zero distortion
      R = dR;
    RealT R2;
    for ( IntT it = 1; ; it++ ) {
      R2 = R*R;
      RealT deriv = 1.0 + R2*(3.0*nK1 + 5.0*R2*nK2);
      if ( deriv == 0.0 )
        throw ExceptionNumericalC("Division by zero in template <class InT, class OutT,class MixerT> RealT RemoveDistortionC<InT,OutT,MixerT>::ComputeDistortion(const Point2dC &) const");

      // compute change to make to R
      RealT Rdiff = (R*(1.0 + R2*(nK1 + R2*nK2)) - dR)/deriv;
      R -= Rdiff;

      // check whether the new R is outside the range
      if ( R < 0.0 || R*R >= thres_R2 )
	throw ExceptionNumericalC("Outside range in template <class InT, class OutT,class MixerT> RealT RemoveDistortionC<InT,OutT,MixerT>::ComputeDistortion(const Point2dC &) const");

      // check whether we have converged, observing the minimum number of
      // iterations
      if ( fabs(Rdiff) < R_CHANGE_THRES && it >= MIN_UND_ITERATIONS )
         break;
        
      // check whether we have exceeded the maximum number of iterations
      if ( it > MAX_UND_ITERATIONS )
	throw ExceptionC("Too many iterations in template <class InT, class OutT,class MixerT> RealT RemoveDistortionC<InT,OutT,MixerT>::ComputeDistortion(const Point2dC &) const");
    }

    // compute distortion and remove it from camera coordinates
    return (1.0 + R2*(nK1 + R2*nK2));
  }
  
  template <class InT, class OutT,class MixerT>
  Point2dC RemoveDistortionC<InT,OutT,MixerT>::RemoveDist(const Point2dC &pnti) const
  {
    // backproject point
    Point2dC pnto = BackProjectPoint(pnti);

    // reproject point using linear camera model
    return ProjectPointLinear(pnto);
  }
  
  template <class InT, class OutT,class MixerT>
  Point2dC RemoveDistortionC<InT,OutT,MixerT>::AddDist(const Point2dC &pnti) const
  {
    // backproject point using linear camera model
    Point2dC pnto = BackProjectPointLinear(pnti);

    // reproject point with distortion
    return ProjectPoint(pnto);
  }

  template <class InT, class OutT,class MixerT>
      void RemoveDistortionC<InT,OutT,MixerT>::CreateCoordsTable(const ImageRectangleC &InFrame, const ImageRectangleC &OutFrame)
  {
    RealRange2dC irng;

    // adjust source window for area where bilinear interpolation can be
    // computed safely. Using 0.51 instead of 0.5 ensures that points on the
    // boundary are not used, for safety.
    irng.TRow() = InFrame.TRow() + 0.500001;
    irng.BRow() = InFrame.BRow() - 0.500001;
    irng.LCol() = InFrame.LCol() + 0.500001;
    irng.RCol() = InFrame.RCol() - 0.500001;

    // set pat as top-left pixel in output image
    Point2dC pat(OutFrame.Origin());
    coordsTable = Array2dC<Point2dC>(OutFrame);
    isInImage = Array2dC<bool>(OutFrame);
    isInImage.Fill(true);

    Array2dIter2C<Point2dC,bool> it(coordsTable,isInImage);

    // Do simple check for each pixel that its contained in the input image.
    RealT yStart = pat[1];
    for(;it;) {
      do {
        Point2dC ipat = AddDist(pat);
        if(irng.Contains(ipat))
          it.Data1() = ipat;
        else
          it.Data2() = false;
        pat[1]++;
      } while(it.Next()) ;
      pat[0]++;
      pat[1] = yStart;
    }
  }


  template <class InT, class OutT,class MixerT>
  void RemoveDistortionC<InT,OutT,MixerT>::Apply(const ImageC<InT> &src,ImageC<OutT> &outImg)
  {
    RavlAssertMsg(!outImg.IsEmpty(), "Output image must have valid frame before applying distortion correction");
    if(src.Frame() != inFrame || outImg.Frame() != coordsTable.Frame())
    {
      inFrame = src.Frame();
      CreateCoordsTable(inFrame, outImg.Frame());
    }
    Array2dIter3C<OutT, Point2dC, bool> it(outImg, coordsTable, isInImage);
    OutT tmp;
    for(;it;++it) {
      const Point2dC p = it.Data2();
      if(it.Data3()) {
        BilinearInterpolation(src, p, tmp);
        mixer(it.Data1(), tmp);
      }
      else if(fillBackground) {
        SetZero(it.Data1());
      }
    }
  }
}


#endif
