// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey 
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=CSPDriver
//! file="Ravl/Contrib/ClipStationPro/ImgIOCSP.cc"

#include "Ravl/Image/ImgIOCSP.hh"

namespace RavlImageN
{
    //: Constructor for RGB types 
  template<> 
   DPIImageClipStationProBodyC<ByteRGBValueC>::DPIImageClipStationProBodyC(const StringC &dev) 
      : cspDevice ( dev, RGB) {  
      cspDevice.BuildAttributesIn (*this) ; } 

            //: Constructor for YUV types 
  template<> 
   DPIImageClipStationProBodyC<ByteYUV422ValueC>::DPIImageClipStationProBodyC(const StringC &dev) 
      : cspDevice ( dev, YUV) {  
      cspDevice.BuildAttributesIn (*this) ; } 
      
       template<class PixelT> 
   DPIImageClipStationProBodyC<PixelT>::DPIImageClipStationProBodyC(const StringC &dev) 
      : cspDevice ( dev, YUV) {  
      cspDevice.BuildAttributesIn (*this) ; } 
}

//using namespace RavlImageN ; 





