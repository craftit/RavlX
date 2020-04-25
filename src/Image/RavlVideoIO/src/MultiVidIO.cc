// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlVideoIO
//! file="Ravl/Image/VideoIO/MultiVidIO.cc"

#include "Ravl/Image/MultiVidIO.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Image/ByteRGBAValue.hh"
#include "Ravl/Image/ByteYUVValue.hh"
#include "Ravl/Image/ImgIO.hh"

namespace RavlImageN
{
  using namespace RavlN;
  
  void InitMultiVidIO()
  {
  }

  FileFormatMultiVidC<ByteT> file_format_multi_vid_byte;
  FileFormatMultiVidC<ByteRGBValueC> file_format_multi_vid_rgb;
  FileFormatMultiVidC<ByteRGBAValueC> file_format_multi_vid_rgba;
  FileFormatMultiVidC<ByteYUVValueC> file_format_multi_vid_yuv;
}
