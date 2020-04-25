// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////
//! file = "Ravl/Contrib/LibGd/Gd.cc"
//! lib=RavlImgGd
//! author = "Warren Moore"

#include "Ravl/Image/Gd.hh"

#define DEBUG 0
#if DEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN
{

  GdImageC::GdImageC(const IntT x, const IntT y) :
    m_gdImagePtr(NULL)
  {
    m_gdImagePtr = gdImageCreateTrueColor(x, y);
  }
  
  GdImageC::GdImageC(const GdImageC &copy) :
    m_gdImagePtr(NULL)
  {
    m_gdImagePtr = gdImageCreateTrueColor(copy.Cols(), copy.Rows());
    gdImageCopy(m_gdImagePtr, copy.Ptr(), 0, 0, 0, 0, copy.Cols(), copy.Rows());
  }
  
  GdImageC::GdImageC(ImageC<ByteRGBValueC> &image) :
    m_gdImagePtr(NULL)
  {
    m_gdImagePtr = gdImageCreateTrueColor(image.Cols(), image.Rows());
    Copy(image);
  }
  
  GdImageC::~GdImageC()
  {
    if (m_gdImagePtr)
      gdImageDestroy(m_gdImagePtr);
  }
  
  void GdImageC::Copy(ImageC<ByteRGBValueC> &image)
  {
    if (!m_gdImagePtr)
      return;
    
    IntT xmax = (static_cast<IntT>(image.Cols()) < gdImageSX(m_gdImagePtr) ? static_cast<IntT>(image.Cols()) : gdImageSX(m_gdImagePtr));
    IntT ymax = (static_cast<IntT>(image.Rows()) < gdImageSY(m_gdImagePtr) ? static_cast<IntT>(image.Rows()) : gdImageSY(m_gdImagePtr));
    
    // Transform from RGB to BGRA
    for (IntT row = 0; row < ymax; row++)
    {
      ByteRGBValueC *srcPtr = image.Row(row);
      int *dstPtr = m_gdImagePtr->tpixels[row];
      for (IntT col = 0; col < xmax; col++)
      {
        int colIndex = gdImageColorAllocate(m_gdImagePtr, srcPtr->Red(), srcPtr->Green(), srcPtr->Blue());
        *dstPtr = colIndex;
        srcPtr++;
        dstPtr++;
      }
    }
  }
  
  ImageC<ByteRGBValueC> GdImageC::GetImage()
  {
    if (!m_gdImagePtr)
      return ImageC<ByteRGBValueC>();
    
    ImageC<ByteRGBValueC> image(gdImageSY(m_gdImagePtr), gdImageSX(m_gdImagePtr));

    // Transform from BGRA to RGB
    for (IntT row = 0; row < gdImageSY(m_gdImagePtr); row++)
    {
      char *srcPtr = reinterpret_cast<char*>(m_gdImagePtr->tpixels[row]);
      ByteRGBValueC *dstPtr = image.Row(row);
      for (IntT col = 0; col < gdImageSX(m_gdImagePtr); col++)
      {
        *dstPtr = ByteRGBValueC(srcPtr[2], srcPtr[1], srcPtr[0]);
        srcPtr += 4;
        dstPtr++;
      }
    }
    
    return image;
  }
  
  GdImageC &GdImageC::operator=(const GdImageC &param)
  {
    if (m_gdImagePtr)
      gdImageDestroy(m_gdImagePtr);
    m_gdImagePtr = gdImageCreateTrueColor(param.Cols(), param.Rows());
    gdImageCopy(m_gdImagePtr, param.Ptr(), 0, 0, 0, 0, param.Cols(), param.Rows());
    
    return *this;
  }
  
}
