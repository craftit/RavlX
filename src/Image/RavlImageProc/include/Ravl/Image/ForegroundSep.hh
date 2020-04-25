// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef FOREGROUNDSEP_HH
#define FOREGROUNDSEP_HH
//! rcsid="$Id$"
//! file="Ravl/Image/Processing/Tracking/ForegroundSep.hh"

//! userlevel=Normal
//! author="Bill Christmas, Phil McLauchlan"
//! docentry="Ravl.API.Images.Mosaic"
//! lib=RavlImageProc

#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Image/RealRGBValue.hh"
#include "Ravl/Image/ConvolveSeparable2d.hh"
#include "Ravl/Projection2d.hh"

namespace RavlImageN {

  //! userlevel=Normal
  //! autoLink=on
  //: Class to separate foreground objects in a sequence of images from a background image.
  // <p> It is typically used to separate out foreground objects from a sequence of images for which a mosaic has been created (see <a href="RavlImageN.MosaicBuilderC.html">MosaicBuilderC</a>).  It assumes a homography between each input image and background.</p>
  // <p> The operations can be roughly summarised as:</p><ul>
  // <li> subtract the projected background from the image</li>
  // <li> threshold the difference to create a foreground mask</li>
  // <li> apply some morphological opening to the mask</li>
  // <li> use the mask to create a foreground image from the input image </li></ul>

  class ForegroundSepC {
  public:
    ForegroundSepC(const ImageC<ByteRGBValueC>& mosaic, RealT fgThreshold=8, bool progressive=true);
    //: Constructor: Initialises background image and threshold for foreground / background separation.
    //!params: mosaic - the background image that images are compared with
    //!params: fgThreshold - grey-level threshold for separation
    //!params: progressive - switch for frame- or field-base operation
    
    void SetMask(const ImageC<bool>& nmask)
      { mask = nmask; }
    //: Set binary mask to exclude regions from tracker
    // Pixels set to false are excluded.
    
    bool SetMask(const StringC& fileName);
    //: Load binary mask from file to exclude regions from tracker
    // Pixels set to zero are excluded.
    
    void SetOpening(IntT Width);
    //: Set morphological opening structuring element
    // Opens with border of width "Size" pixels (thus 0 = do nothing).<br>
    // Max "Size" is 2 (the default value).

    void SetFilter(IntT Width);
    //: Set width of Gaussian low-pass filter
    // Default is 7 (i.e. a  s.d. of about 1.2)

    ImageC<ByteRGBValueC> Apply(const ImageC<ByteRGBValueC>& img, Projection2dC& homog);
    //: Computes foreground image for corresponding input image and homography
    
  protected:
    ImageC<ByteRGBValueC> mosaic;
    ImageC<bool> kernel;
    RealT fgThreshold;  
    ImageC<bool> mask;
    IntT filterWidth;
    ConvolveSeparable2dC<ByteRGBValueC,ByteRGBValueC,RealT,RealRGBValueC> lpf;
    bool progressive;
  };
  
}

#endif
