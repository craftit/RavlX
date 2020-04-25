// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef SHOTBOUNDHISTINT
#define SHOTBOUNDHISTINT 1
//! rcsid="$Id$"
//! lib=RavlVideo

#include "Ravl/SArray1d.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Image/Image.hh"

//! docentry="Ravl.API.Images.Shot Boundary Detection"
//! author="Alexey Kostin, Bill Christmas"
//! userlevel=Normal

namespace RavlImageN {
  using namespace RavlN;

  //: Basic video shot boundary detector, based on colour histogram intersection, for ByteRGBValueC pixel type

  class ShotBoundHistIntC {
    
  public:
    
    ShotBoundHistIntC(RealT ShotDetTh=0.7, UIntT NoOfBins=16, UIntT Subsample=4);
    //: Constructor on shot detection parameters
    //!param: ShotDetTh - threshold for shot boundary measure [0..1]
    //!param: NoOfBins - no of histogram bins per colour channel
    //!param: Subsample - each frame is subsampled both horizontally and vertically by this factor (1 -> no subsampling)
    
    bool Apply(const ImageC<ByteRGBValueC>& frame);
    //: Process next video frame
    // Returns <code>true</code> if boundary detected

    RealT Proximity() const
    { return (shotDetThr - value);}
    //: Returns proximity to threshold of last decision
    // +ve / -ve value indicates boundary / not boundary respectively
    
  private:
    
    UIntT noOfBins;
    UIntT subsample; // spatial subsampling factor
    RealT shotDetThr;
    bool firstFrame;
    SArray1dC<int> prevColourHist;
    RealT value;
    
    SArray1dC<int> CalcHist(const ImageC<ByteRGBValueC> &Image);

  };
  

}




#endif
