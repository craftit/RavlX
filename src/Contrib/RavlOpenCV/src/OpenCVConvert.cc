// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlOpenCV

#include "Ravl/Image/OpenCVConvert.hh"
#include "Ravl/SysLog.hh"

namespace RavlImageN {

  using namespace RavlN;

  template <>
  bool RavlImage2IplImage(const ImageC<ByteT> & src, IplImage*& dest)
{
    CvSize size; size.width = src.Cols(); size.height = src.Rows();
    dest = cvCreateImage(size, IPL_DEPTH_8U, 1);
    char *dr = dest->imageData;
    for (IndexC r(src.TRow()); r<=src.BRow(); ++r) {
      const ByteT *sc = src.Row(r);
      char *dc = dr;
      for (IndexC c(src.LCol()); c<=src.RCol(); ++c)
        *dc++ = *sc++;
      dr += dest->widthStep;
    }
    return true;
  }
  
  template <>
  bool RavlImage2IplImage(const ImageC<ByteRGBValueC> & src, IplImage*& dest)
  {
    CvSize size; size.width = src.Cols(); size.height = src.Rows();
    dest = cvCreateImage(size, IPL_DEPTH_8U, 3);
    char *dr = dest->imageData;
    for (IndexC r(src.TRow()); r<=src.BRow(); ++r) {
      const ByteT *sc = (ByteT*)src.Row(r);
      char *dc = dr;
      for (IndexC c(src.LCol()); c<=src.RCol(); ++c) {
        *(dc++ +2) = *sc++; // dodgy code to do RGB->BGR
        *dc++      = *sc++;
        *(dc++ -2) = *sc++;
      }
      dr += dest->widthStep;
    }
    return true;
  }
  
  template <>
  bool RavlImage2IplImage(const ImageC<RealT> & src, IplImage*& dest)
  {
    CvSize size; size.width = src.Cols(); size.height = src.Rows();
    dest = cvCreateImage(size, IPL_DEPTH_64F, 1);
    double *dr = (double *)dest->imageData;
    for (IndexC r(src.TRow()); r<=src.BRow(); ++r) {
      const RealT *sc = src.Row(r);
      double *dc = dr;
      for (IndexC c(src.LCol()); c<=src.RCol(); ++c) 
        *dc++ = *sc++;
      dr += dest->widthStep/8;
    }
    return true;
  }
  
  template <>
  bool RavlImage2IplImage(const ImageC<RealRGBValueC> & src, IplImage*& dest)
  {
    CvSize size; size.width = src.Cols(); size.height = src.Rows();
    dest = cvCreateImage(size, IPL_DEPTH_64F, 3);
    double *dr = (double *)dest->imageData;
    for (IndexC r(src.TRow()); r<=src.BRow(); ++r) {
      const RealT *sc = (RealT*)src.Row(r);
      double *dc = dr;
      for (IndexC c(src.LCol()); c<=src.RCol(); ++c) {
        *(dc++ +2) = *sc++;
        *dc++      = *sc++;
        *(dc++ -2) = *sc++;
      }
    dr += dest->widthStep/8;
    }
    return true;
  }
  


}
