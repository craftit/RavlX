// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImageProc
//! author="Charles Galambos"
//! docentry="Ravl.API.Images.Scaling and Warping"
//! userlevel=Normal
//! file="Ravl/Image/Processing/Filters/doThinPlateWarp.cc"

#include "Ravl/Image/Image.hh"
#include "Ravl/Image/WarpThinPlateSpline.hh"
#include "Ravl/Stream.hh"
#include "Ravl/Option.hh"
#include "Ravl/Array1d.hh"
#include "Ravl/Point2d.hh"
#include "Ravl/IO.hh"

using namespace RavlN;
using namespace RavlImageN;

int main(int nargs,char **argv) {
  OptionC opt(nargs,argv);
  RealT sigma = opt.Real("s",4,"Stiffness for warping. ");
  StringC inFile = opt.String("","in.ppm","Input file. ");
  StringC outFile = opt.String("","@X","Output file. ");
  opt.Check();
  
  ImageC<ByteT> inImg;
  
  if(!Load(inFile,inImg)) {
    cerr << "Failed to load '" << inFile << "'\n";
    return 1;
  }
  
  Array1dC<Point2dC> orgPnts(7);
  orgPnts[0] = Point2dC(10,10);
  orgPnts[1] = Point2dC(100,200);
  orgPnts[2] = Point2dC(200,200);
  orgPnts[3] = Point2dC(10,200);
  orgPnts[4] = Point2dC(200,10);
  orgPnts[5] = Point2dC(50,50);
  orgPnts[6] = Point2dC(60,60);
  
  Array1dC<Point2dC> newPnts(7);
  newPnts[0] = Point2dC(10,10);
  newPnts[1] = Point2dC(100,200);
  newPnts[2] = Point2dC(200,200);
  newPnts[3] = Point2dC(10,200);
  newPnts[4] = Point2dC(200,10);
  newPnts[5] = Point2dC(45,45);
  newPnts[6] = Point2dC(65,65);
  
  WarpThinPlateSplineC<ByteT> warp(sigma);
  ImageC<ByteT> outImg;
  warp.Apply(inImg,orgPnts,newPnts,outImg);
  
  if(!Save(outFile,outImg)) {
    cerr << "Failed to save '" << outFile << "'\n";
    return 1;
  }
  
  return 0;
}
