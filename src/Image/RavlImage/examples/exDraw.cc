// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImage
//! file="Ravl/Image/Base/exDraw.cc"
//! author="James Smith"
//! date="27/10/2002"
//! docentry="Ravl.API.Images"
//! userlevel=Normal

#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Image/DrawPolygon.hh"
#include "Ravl/DP/FileFormatIO.hh"

// The defs.mk file will need this line to compile correctly:
// PROGLIBS=RavlImageIO RavlDPDisplay


using namespace RavlImageN;

int main(int nargs,char **argv) {

  ImageC<ByteRGBValueC> img(100,100);

  // Draw shaded polygon 
  {
    DListC<ByteRGBValueC> colours;
    colours.InsLast(ByteRGBValueC(255,0,0));
    colours.InsLast(ByteRGBValueC(255,0,0));
    colours.InsLast(ByteRGBValueC(255,255,255));
    colours.InsLast(ByteRGBValueC(255,255,255));
    Polygon2dC poly;
    poly.InsLast(Point2dC(20,20));
    poly.InsLast(Point2dC(50,70));
    poly.InsLast(Point2dC(60,50));
    poly.InsLast(Point2dC(30,10));
    DrawPolygon(img,colours,poly,true);
  }

  // Draw flat filled polygon 
  {
    ByteRGBValueC colour(255,0,255);
    Polygon2dC poly;
    poly.InsLast(Point2dC(60,90));
    poly.InsLast(Point2dC(20,70));
    poly.InsLast(Point2dC(30,60));
    poly.InsLast(Point2dC(70,80));
    DrawPolygon(img,colour,poly,false);
  }
  
  RavlN::Save("@X",img);

  return 0;
}
