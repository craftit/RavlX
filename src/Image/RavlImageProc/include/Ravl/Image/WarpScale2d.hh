// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_WARPSCALE2D_HEADER
#define RAVLIMAGE_WARPSCALE2D_HEADER 1
///////////////////////////////////////////////////////////////
//! docentry="Ravl.API.Images.Scaling and Warping"
//! lib=RavlImageProc
//! author="Charles Galambos"
//! date="05/05/1999"
//! file="Ravl/Image/Processing/Filters/Warp/WarpScale2d.hh"

#include "Ravl/Image/Image.hh"
#include "Ravl/Image/BilinearInterpolation.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/Vector2d.hh"
#include "Ravl/Point2d.hh"
#include "Ravl/RealRange2d.hh"
#include "Ravl/Traits.hh"
#include "Ravl/SArray1d.hh"

#define CLEVER_BILINEAR
namespace RavlImageN {

  //! userlevel=Normal

  template <class InT, class OutT>
  bool WarpScaleBilinear(const ImageC<InT> &img,
                         const Vector2dC &scale, // Distance between samples in the input image.
                         ImageC<OutT> &result    // Output of scaling. The image must be of the appropriate size
                        )
  {
    //call subsampling function
    //cout << "WarpScaleBilinear scale:" << scale << std::endl;
    if(scale[0] >= 1. && scale[1] >= 1.)
      return WarpSubsample(img, scale, result);

                      //cout << "src frame:" << img.Frame() << std::endl;
    if(result.Frame().IsEmpty()) {
      const IndexRange2dC &imgFrame = img.Frame();
      IndexRange2dC rng;
      rng.TRow() = Ceil(imgFrame.TRow() / scale[0]);
      rng.LCol() = Ceil(imgFrame.LCol() / scale[1]);
#ifdef CLEVER_BILINEAR
      rng.BRow() = Floor((imgFrame.BRow() - 0) / scale[0]);
      rng.RCol() = Floor((imgFrame.RCol() - 0) / scale[1]);
#else
      rng.BRow() = Floor((imgFrame.BRow() - 1) / scale[0]);
      rng.RCol() = Floor((imgFrame.RCol() - 1) / scale[1]);
#endif
      result = ImageC<OutT>(rng);
    }
    //cout << "res frame:" << result.Frame() << std::endl;
    Point2dC origin(result.Frame().TRow() * scale[0], result.Frame().LCol() * scale[1]);
    //cout << "origin:" << origin << std::endl;

#if 0
    // Simple implementation.
    Point2dC rowStart = origin;
    for(Array2dIterC<OutT> it(result);it;) {
      Point2dC pnt = rowStart;
      do {
        BilinearInterpolation(img,pnt,*it);
        pnt[1] += scale[1];
      } while(it.Next()); // True while in same row.
      rowStart[0] += scale[0];
    }
#else
#ifdef CLEVER_BILINEAR
    Point2dC rowStart = origin;
    for(Array2dIterC<OutT> it(result);it;) {
      Point2dC pnt = rowStart;

      IntT fx = Floor(pnt[0]); // Row
      IntT fxp1 = fx + 1;
      if(fxp1 >= img.Rows()) fxp1 = fx;
      RealT u = pnt[0] - fx;
      if(u < 1e-5) {
        do {
          IntT fy = Floor(pnt[1]); // Col
          RealT t = pnt[1] - fy;
          if(t < 1e-5) {
            const InT* pixel1 = &(img)[fx][fy];
            *it = OutT(pixel1[0]);
            pnt[1] += scale[1];
          } else {
            RealT onemt = (1.0-t);

            //printf("x:%g  y:%g  fx:%i  fy:%i\n", pnt[0], pnt[1], fx, fy);
            const InT* pixel1 = &(img)[fx][fy];
            *it = OutT((pixel1[0] * onemt) +
                        (pixel1[1] * t));
            pnt[1] += scale[1];
          }
        } while(it.Next()); // True while in same row.
      } else {
        RealT onemu = (1.0-u);
        do {
          IntT fy = Floor(pnt[1]); // Col
          RealT t = pnt[1] - fy;
          if(t < 1e-5) {
            const InT* pixel1 = &(img)[fx][fy];
            const InT* pixel2 = &(img)[fxp1][fy];
            *it = OutT((pixel1[0] * onemu) +
                        (pixel2[0] * u));
            pnt[1] += scale[1];
          } else {
            RealT onemt = (1.0-t);

            //printf("x:%g  y:%g  fx:%i  fy:%i\n", pnt[0], pnt[1], fx, fy);
            const InT* pixel1 = &(img)[fx][fy];
            const InT* pixel2 = &(img)[fxp1][fy];
            *it = OutT((pixel1[0] * (onemt*onemu)) +
                        (pixel1[1] * (t*onemu)) +
                        (pixel2[0] * (onemt*u)) +
                        (pixel2[1] * (t*u)));
            pnt[1] += scale[1];
          }
        } while(it.Next()); // True while in same row.
      }

      rowStart[0] += scale[0];
    }
#else
    Point2dC rowStart = origin;
    for(Array2dIterC<OutT> it(result);it;) {
      Point2dC pnt = rowStart;

      IntT fx = Floor(pnt[0]); // Row
      RealT u = pnt[0] - fx;
      RealT onemu = (1.0-u);
      do {
        IntT fy = Floor(pnt[1]); // Col
        RealT t = pnt[1] - fy;
        RealT onemt = (1.0-t);

        //printf("x:%g  y:%g  fx:%i  fy:%i\n", pnt[0], pnt[1], fx, fy);
        const InT* pixel1 = &(img)[fx][fy];
        const InT* pixel2 = &(img)[fx+1][fy];
        *it = OutT((pixel1[0] * (onemt*onemu)) +
                    (pixel1[1] * (t*onemu)) +
                    (pixel2[0] * (onemt*u)) +
                    (pixel2[1] * (t*u)));
        pnt[1] += scale[1];
      } while(it.Next()); // True while in same row.

      rowStart[0] += scale[0];
    }
#endif
#endif
    return true;
  }
  //: Rescale an image
  //!param: img - input image
  //!param: scale - output image size is input size <i>divided</i> by <code>scale</code>
  //!param: result - output image
  // If <code>result</code> is empty, the correct size is computed.


  template <class InT, class OutT>
  bool WarpScaleBilinear(const ImageC<InT> &img,ImageC<OutT> &result)
  {
    if(result.Frame().IsEmpty()) return false;
    Vector2dC scale; // Distance between samples in the input image.
    scale[0] = (RealT) img.Rows() / (RealT) result.Rows();
    scale[1] = (RealT) img.Cols() / (RealT) result.Cols();
    return WarpScaleBilinear(img, scale, result);
  }
  //: Rescale an image
  //!param: img - input image
  //!param: result - output image
  // This version computes the scaling factor from the input and output image sizes


  //:--
  //! userlevel=Develop
  template <class InT, class OutT>
  inline void WS_prepareRow(const ImageC<InT> &img, int srcRowI, double srcColR, double scaleColR,
                     OutT *resPtr, int resCols)
  {
    //cerr << "srcRowI:" << srcRowI << endl;
    //cerr << "srcColR:" << srcColR << endl;
    //cerr << "scaleColR:" << scaleColR << endl;
    //cerr << "resCols:" << resCols << endl;
    int srcColI = Floor(srcColR);
    double t = srcColR - srcColI;

    const InT* srcPtr = &(img)[srcRowI][srcColI];

    InT pixVal = *srcPtr;
    //cerr << "pixVal:" << pixVal << endl;
    srcPtr++;
    for(int i = 0; i < resCols; i++) {
      //cerr << "i:" << i << endl;
      //first partial pixel in the row
      const double onemt = 1. - t;
      OutT resPixel = (OutT)(pixVal) * onemt;

      //all full pixels
      const double srcLastColR = srcColR + scaleColR;
      const int srcLastColI = Floor(srcLastColR);
      for(srcColI++; srcColI < srcLastColI; srcColI++) {
        resPixel += (OutT)(*srcPtr);
        srcPtr++;
      }

      //last partial pixel
      t = srcLastColR - srcLastColI;
      pixVal = *srcPtr;
      srcPtr++;
      //cerr << "t:" << t << endl;
      if(t > 1e-5) {
        resPixel += (OutT)(pixVal) * t;
      }

      *resPtr = resPixel;
      resPtr++;
      srcColR = srcLastColR;
      srcColI = srcLastColI;
    }
  }

  //the only difference with previous function is that this adds pixels to result
  template <class InT, class OutT>
  inline void WS_prepareRowAdd(const ImageC<InT> &img, int srcRowI, double srcColR, double scaleColR,
                        OutT *resPtr, int resCols)
  {
    int srcColI = Floor(srcColR);
    double t = srcColR - srcColI;

    const InT* srcPtr = &(img)[srcRowI][srcColI];

    InT pixVal = *srcPtr;
    srcPtr++;
    for(int i = 0; i < resCols; i++) {
      //first partial pixel in the row
      const double onemt = 1. - t;
      OutT resPixel = (OutT)(pixVal) * onemt;

      //all full pixels
      const double srcLastColR = srcColR + scaleColR;
      const int srcLastColI = Floor(srcLastColR);
      for(srcColI++; srcColI < srcLastColI; srcColI++) {
        resPixel += (OutT)(*srcPtr);
        srcPtr++;
      }

      //last partial pixel
      t = srcLastColR - srcLastColI;
      pixVal = *srcPtr; //this could read outside the row, but the value will not be used
      srcPtr++;
      if(t > 1e-5) {
        resPixel += (OutT)(pixVal) * t;
      }

      *resPtr += resPixel;
      resPtr++;
      srcColR = srcLastColR;
      srcColI = srcLastColI;
    }
  }

  /*bool CheckRow(float *buf, int numCols, float val){
    for(int i = 0; i < numCols; i++) {
      float dif = buf[i] - val;
      if(RavlN::Abs(dif) > (buf[i] + val) * 1e-6) {
        cerr << "Error:" << i << "   " << buf[i] << "  " << val << "   " << dif << endl;
        return false;
      }
    }
    return true;
  }*/

  template <class InT, class OutT>
  bool WarpSubsample(const ImageC<InT> &img,
                     const Vector2dC &scale, // Distance between samples in the input image.
                    ImageC<OutT> &result    // Output of scaling. The image must be of the appropriate size
                    )
  {
    //cout << "WarpSubsample scale:" << scale << endl;
    //we can't do supersampling
    if(scale[0] < 1 || scale[1] < 1)
      return false;

    //cout << "src frame:" << img.Frame() << std::endl;
    if(result.Frame().IsEmpty()) {
      const IndexRange2dC &imgFrame = img.Frame();
      IndexRange2dC rng;
      rng.TRow() = Ceil(imgFrame.TRow() / scale[0]);
      rng.LCol() = Ceil(imgFrame.LCol() / scale[1]);
      rng.BRow() = Floor((imgFrame.BRow() + 1) / scale[0]) - 1;
      rng.RCol() = Floor((imgFrame.RCol() + 1) / scale[1]) - 1;
      result = ImageC<OutT>(rng);
    } else {
      const IndexRange2dC &imgFrame = img.Frame();
      IndexRange2dC rng;
      rng.TRow() = Ceil(imgFrame.TRow() / scale[0]);
      rng.LCol() = Ceil(imgFrame.LCol() / scale[1]);
      rng.BRow() = Floor((imgFrame.BRow() + 1) / scale[0]) - 1;
      rng.RCol() = Floor((imgFrame.RCol() + 1) / scale[1]) - 1;
      if(!rng.Contains(result.Frame())) {
        cerr << "Resulting image is too large\n";
        return false;
      }
    }

    //cout << "res frame:" << result.Frame() << std::endl;
    const Point2dC origin(result.Frame().TRow() * scale[0], result.Frame().LCol() * scale[1]);
    //cout << "origin:" << origin << std::endl;

    const int resRows = (int)(result.Rows());
    const int resCols = (int)(result.Cols());

    const double norm = 1. / (scale[0] * scale[1]);

    typedef typename RavlN::NumericalTraitsC<OutT>::RealAccumT RealAccumT;
    SArray1dC<RealAccumT> bufferRow(resCols);
    SArray1dC<RealAccumT> bufferRes(resCols);

    //prepare row buffer
    double srcRowR = origin.Row();
    int srcRowI = Floor(srcRowR);
    double u = srcRowR - srcRowI;

    WS_prepareRow(img, srcRowI, origin.Col(), scale[1], bufferRow.DataStart(), resCols);
    //if(!CheckRow(buffer, resCols, scale[1])) return false;

    for(int j = 0; j < resRows; j++) {
      //cerr << "j:" << j << endl;
      //first partial row
      double onemu = 1. - u;
      for(int i = 0; i < resCols; i++) {
        bufferRes[i] = bufferRow[i] * onemu;
      }

      //all full rows
      const double srcLastRowR = srcRowR + scale[0];
      const int srcLastRowI = Floor(srcLastRowR);
      //cerr << "srcRowI:" << srcRowI << endl;
      //cerr << "srcLastRowI:" << srcLastRowI << endl;
      for(srcRowI++; srcRowI < srcLastRowI; srcRowI++) {
        //cerr << "srcRowI:" << srcRowI << endl;
        WS_prepareRowAdd(img, srcRowI, origin.Col(), scale[1], bufferRes.DataStart(), resCols);
      }

      //last partial pixel
      u = srcLastRowR - srcLastRowI;
      //cerr << "u:" << u << endl;
      if(u > 1e-5) {
        WS_prepareRow(img, srcRowI, origin.Col(), scale[1], bufferRow.DataStart(), resCols);
        //if(!CheckRow(buffer, resCols, scale[1])) return false;
        for(int i = 0; i < resCols; i++) {
          bufferRes[i] += bufferRow[i] * u;
        }
      } else {
        //check if we need buffer for next iteration
        if(j + 1 < resRows) {
          //cerr << "u srcRowI:" << srcRowI << endl;
          WS_prepareRow(img, srcRowI, origin.Col(), scale[1], bufferRow.DataStart(), resCols);
          //if(!CheckRow(buffer, resCols, scale[1])) return false;
        }
      }
      //if(!CheckRow(resRowPtr, resCols, scale[1]*scale[0])) return false;

      //copy and scale result
      OutT *resRowPtr = &(result[j+result.TRow()][result.LCol()]);
      for(int i = 0; i < resCols; i++) {
        resRowPtr[i] = (OutT)(bufferRes[i] * norm);
      }

      srcRowR = srcLastRowR;
      srcRowI = srcLastRowI;
    }

    return true;
  }
  //: Fast image subsample


}

#endif
