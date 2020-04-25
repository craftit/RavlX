// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! date="18/2/1997"
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Lines/PixelMap.cc"

#include "Ravl/Image/PixelMap.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/CDLIter.hh"

namespace RavlImageN {
  
  // Create map from an image.
  
  const PixelMapC &PixelMapC::operator= (const ImageC<ByteT> &img) {
    local.Empty();
    for(Array2dIterC<ByteT> it(img);it;it++) {
      if(*it)
	Insert(it.Index());
    }
    return *this;
  }
  
  ////////////////////////
  // Print pixels to stream.
  
  void PixelMapC::Dump(ostream &out) {
    BinIterC<Point2dC,Index2dC,DListC<Index2dC > > It(local.Iter());
    out << "PixelMapC::Dump() \n" ;
    for(;It.IsElm();It.Next()) {
      out << "Bin " << It.Key() << " :";
      for(ConstDLIterC<Index2dC > Pix(It.Data());Pix.IsElm();Pix.Next())
	out << Pix.Data() << " ";
      out << "\n";
    }
  }
  
  ///////////////////////
  //: Create an image with pixels marked.
  
  ImageC<ByteT> PixelMapC::Image(ByteT forground,ByteT background) {
    BinIterC<Point2dC,Index2dC,DListC<Index2dC > > it(local.Iter());
    if(!it)
      return ImageC<ByteT>(); // No points so return an empty image.
    // Decide how big the image has to be.
    ImageRectangleC rect(0,0,0,0); 
    for(;it.IsElm();it.Next()) {
      for(ConstDLIterC<Index2dC > pix(it.Data());pix.IsElm();pix.Next())
	rect.Involve(pix.Data());
    }
    // Create an image and update the image.
    ImageC<ByteT> pixMap(rect);
    pixMap.Fill(background);
    for(it.First();it.IsElm();it.Next()) { 
      for(ConstDLIterC<Index2dC > pix(it.Data());pix.IsElm();pix.Next())
	pixMap[pix.Data()] = forground;
    }
    return pixMap;
  }
}
