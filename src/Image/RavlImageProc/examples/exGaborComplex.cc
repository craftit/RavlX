// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! docentry="Ravl.API.Images.Filtering"
//! userlevel=Normal
//! rcsid="$Id: exGaborFilter.cc 5240 2005-12-06 17:16:50Z plugger $"
//! lib=RavlImageProc
//! author="Bill Christmas"
//! file="Ravl/Image/Processing/Filters/exGaborComplex.cc"

#include "Ravl/Image/GaborComplex.hh"
#include "Ravl/Array2d.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/IO.hh"
#include "Ravl/Option.hh"

using namespace RavlN;
using namespace RavlImageN;

int main(int argc, char* argv[]) {

  OptionC opt(argc, argv);
  IntT nscale      = opt.Int ("s", 3,   "no. of scales");
  IntT ntheta      = opt.Int ("t", 6,   "no. of angles");
  RealT subsample  = opt.Real("ss",1,   "output image shrunk by this factor (>1)");
  RealT centreFreq = opt.Real("U", 0.35,"prototype filter centre frequency");
  RealT ratio      = opt.Real("R", 2,   "filter radial scale ratio");
  RealT sfactor    = opt.Real("S", 1,   "rescaling sigma");
  RealT efactor    = opt.Real("E", 1,   "rescaling filter aspect ratio");
  bool offset      = opt.Boolean("O",   "alternate scales are offset");
  StringC ipfile   = opt.String("i","", "Name of input image (default: uses impulse)");
  StringC opfile   = opt.String("o","", "Name of filter output images (default: none)");
  StringC freqplot = opt.String("p","@X:frequency plot", "Name of filter plot");
  IntT im_size     = opt.Int ("is",200, "image size (if using impulse image)");
  opt.DependXor("i is");
  opt.Check();

  // if no image specified, create image of delta function
  ImageC<RealT> in(im_size,im_size);
  if (ipfile.IsEmpty()) {
    in.Fill(0);
    in[im_size/2][im_size/2] = 255;
  }
  else
    RavlN::Load (ipfile, in, "", true);

  // create Gabor filter bank
  GaborComplexC bank(nscale, ntheta, offset);
  if (opt.IsOnCommandLine("ss"))bank.SetSubsample(subsample);  
  if (opt.IsOnCommandLine("U")) bank.SetFreq(centreFreq);  
  if (opt.IsOnCommandLine("S")) bank.AdjustSigma(sfactor);  
  if (opt.IsOnCommandLine("E")) bank.AdjustLambda(efactor);
  if (opt.IsOnCommandLine("R")) bank.SetRadialScale(ratio);  

  // apply filter bank to image
  Array2dC<ImageC<RealT> > out = bank.Apply(in);

  // plot filter outputs
  for (IntT iscale=0; iscale < nscale; ++iscale) {
    for (IntT itheta=0; itheta < ntheta; ++itheta) {
      if (opt.IsOnCommandLine("o")) {
        if (opfile.contains("@X", 0))
          RavlN::Save(StringC("@X:frequency = ") + centreFreq/pow(ratio,iscale) + "f_s; orientation = " + itheta + "pi/" + ntheta, out[iscale][itheta]);
        else
          RavlN::Save(opfile + "_" + iscale + "_" + itheta + ".pgm", out[iscale][itheta]);
      }
    }
  }
  // plot filter spectra
  RavlN::Save(freqplot, bank.FilterSpectrum()*128);
  return 0;
}
