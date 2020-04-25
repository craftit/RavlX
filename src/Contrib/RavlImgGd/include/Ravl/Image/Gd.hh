// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////
//! file = "Ravl/Contrib/LibGd/Gd.hh"
//! lib=RavlImgGd
//! author = "Warren Moore"
//! docentry = "Ravl.API.Images.Converters.LibGD"
//! example = "testGd.cc"

#ifndef RAVL_LIBGD_HEADER
#define RAVL_LIBGD_HEADER 1

#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include <gd.h>

namespace RavlImageN
{
  
  //: GdImageC
  // Thin wrapper and utility functions for <a href="http://www.boutell.com/gd/">GD images</a>
  
  class GdImageC
  {
  public:
    GdImageC(const IntT x, const IntT y);
    //: Constructor
    
    GdImageC(const GdImageC &copy);
    //: Copy constructor
    
    GdImageC(ImageC<ByteRGBValueC> &image);
    //: Construct and copy an image
    // This creates the a Gd image of the same size as the supplied image, and copies the image
    
    ~GdImageC();
    //: Destructor
    
    void Copy(ImageC<ByteRGBValueC> &image);
    //: Paste an RGB image into the Gd image
    
    ImageC<ByteRGBValueC> GetImage();
    //: Get a copy of the image
    // Creates a new iamge containing the contents of the Gd image
    
    gdImagePtr Ptr() const
    { return m_gdImagePtr; }
    //: Get the data pointer
    // This returns the Gd image ptr to be used with native Gd functions
    
    IntT Rows() const
    { return gdImageSY(m_gdImagePtr); }
    //: Get the row size
    
    IntT Cols() const
    { return gdImageSX(m_gdImagePtr); }
    //: Get the col size
    
    GdImageC &operator=(const GdImageC &param);
    //: Assignment operator
    
  private:
    gdImagePtr m_gdImagePtr;
  };
  
}

#endif
