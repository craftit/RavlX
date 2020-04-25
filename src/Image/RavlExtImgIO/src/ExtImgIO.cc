// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////
//! lib=RavlExtImgIO
//! file="Ravl/Image/ExternalImageIO/Core/ExtImgIO.cc"

#include "Ravl/Image/ExtImgIO.hh"

namespace RavlImageN {

   void ExtImgIO_Init()
   {
#ifdef MUSTLINK_JPEG
      InitJPEGCompressConv();
#endif
#ifdef MUSTLINK_TIFF
      InitTIFFFormat();
#endif
#ifdef MUSTLINK_JPEG
      InitJPEGFormat();
#endif
#ifdef MUSTLINK_PNG
      InitPNGFormat();
#endif
   }
}
