// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2012, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlImageIO
//! file="Ravl/Image/ImageIO/ImgIOInit.cc"


#include "Ravl/Image/ImgIO.hh"


namespace RavlImageN {
  void InitDPImageIO(){
    InitStdImageCnv();
    InitStdImageCnv2();
    InitStdImageCnv3();
    InitStdImageCnv4();
    InitStdImageCnv5();
    InitStdImageCnv6();
    InitStdImageCnv7();
    InitPNMFormat();
    InitYUVFormat();
    InitImgIOInt();
    InitImgIOByte();
    InitImgIOReal();
    InitImgIOUInt16();
    InitImgIOFloat();
    
    InitImgIOByteRGB();
    InitImgIORealRGB();
    InitImgIOByteYUV();
    InitImgIORealYUV();    
    InitImgIOByteYUV422();
    InitImgIOByteVYU();
    InitImgIOUInt16RGB();
    InitImgIOByteRGBA();
    InitImgIORealRGBA();
    
    InitRGBImageCnv();
    
    InitImgIOByteYCbCrBT601Value();
    InitImgIOUInt16YCbCrBT601Value();
    InitImgIOYCbCrBT601ValueFloat();
    InitImgIOByteYCbCr422BT601();
    InitImgIOFloatYPbPr422BT709();
#if 0    
    InitImgIOByteVYU();
    InitPixelIO();
#endif
  }
}
  
