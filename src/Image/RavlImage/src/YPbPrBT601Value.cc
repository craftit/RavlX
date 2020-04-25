// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2008-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlImage

#include "Ravl/Image/YPbPrBT601Value.hh"
#include "Ravl/Image/RGBValue.hh"

namespace RavlImageN {
  
  // Convert a pixel type from RGB to YPbPrBT601
  
  void RGBFloat2YPbPrBT601Float(const RGBValueC<float> &value,YPbPrBT601ValueC<float> &outValue) {
    outValue.Set(+ 0.299000 * value.Red()  + 0.587000 * value.Green()  + 0.114000 * value.Blue(),
                 - 0.168736 * value.Red()  - 0.331264 * value.Green()  + 0.500000 * value.Blue(),
                 + 0.500000 * value.Red()  - 0.418688 * value.Green()  - 0.081312 * value.Blue());
  }
  
  // Convert a pixel type from YPbPrBT601 to RGB
  
  void YPbPrBT601Float2RGBFloat(const YPbPrBT601ValueC<float> &value,RGBValueC<float> &outValue) {
    outValue.Set(value.Y()  + 0.000000 * value.Pb()  + 1.402000 * value.Pr(),
                 value.Y()  - 0.344136 * value.Pb()  - 0.714136 * value.Pr(),
                 value.Y()  + 1.814210 * value.Pb()  + 0.000000 * value.Pr());
  }
  
     
}
