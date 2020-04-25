// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2008-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlImage

#include "Ravl/Image/YCbCrBT601Value.hh"
#include "Ravl/Image/RGBValue.hh"

namespace RavlImageN {
  
  // Convert a pixel type from RGB to YCbCrBT601
  
  void RGBFloat2YCbCrBT601Float(const RGBValueC<float> &value,YCbCrBT601ValueC<float> &outValue) {
    outValue.Set( + 0.299    * value.Red() + 0.587    * value.Green() + 0.114    * value.Blue(),
                  - 0.168736 * value.Red() - 0.331264 * value.Green() + 0.5      * value.Blue(),
                  + 0.5      * value.Red() - 0.418688 * value.Green() - 0.081312 * value.Blue());
  }
  
  // Convert a pixel type from YCbCrBT601 to RGB
  
  void YCbCrBT601Float2RGBFloat(const YCbCrBT601ValueC<float> &value,RGBValueC<float> &outValue) {
    outValue.Set(value.Y() - 0.1218894199e-5 * value.Cb() + 1.4019995886    * value.Cr(),
                 value.Y() - 0.3441356781    * value.Cb() - 0.7141361555    * value.Cr(),
                 value.Y() + 1.772000066     * value.Cb() + 0.4062980664e-6 * value.Cr());
  }
  
  
}
