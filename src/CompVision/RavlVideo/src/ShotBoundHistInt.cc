// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#include "Ravl/Image/ShotBoundHistInt.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/Array1dIter2.hh"
//! rcsid="$Id$"
//! lib=RavlVideo

using namespace RavlN;
using namespace RavlImageN;

SArray1dC<int> ShotBoundHistIntC::CalcHist(const ImageC<ByteRGBValueC> &Image)
{
  SArray1dC<int> Hist(noOfBins * noOfBins * noOfBins);
  Hist.Fill(0);
  int binWidth = 256 / noOfBins;
  for(Array2dIterC<ByteRGBValueC> it(Image);it;it+=subsample) {
    const int c1 = int((*it)[0]) / binWidth;
    const int c2 = int((*it)[1]) / binWidth;
    const int c3 = int((*it)[2]) / binWidth;
    const int index = ((c1 * noOfBins) + c2) * noOfBins + c3;
    Hist[index]++;
  } 
  return Hist;
}


ShotBoundHistIntC::ShotBoundHistIntC(RealT ShotDetThr, UIntT NoOfBins, UIntT Subsample)
  : noOfBins(NoOfBins),
    subsample(Subsample),
    shotDetThr(ShotDetThr), 
    firstFrame(true),
    prevColourHist(noOfBins * noOfBins * noOfBins)
{
}


bool ShotBoundHistIntC::Apply(const ImageC<ByteRGBValueC>& frame) {
  // calculate histogram on new frame
  SArray1dC<int> colourHist = CalcHist(frame);
  // compute histogram intersection between this & last
  if (firstFrame)  value = shotDetThr;
  else {
    int sumIntersection = 0;
    for(Array1dIter2C<int,int> hists(prevColourHist,colourHist); hists; ++hists) {
      sumIntersection += Min( hists.Data1(), hists.Data2() );
    }
    // normalise intersection
    value =  RealT(sumIntersection) / (frame.Size()/(subsample*subsample));
  }
  prevColourHist = colourHist;
  firstFrame = false;
  return (value < shotDetThr);
}
