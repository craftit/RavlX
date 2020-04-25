// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImageProc
//! docentry="Ravl.API.Images.Motion.Optical Flow"
//! userlevel=Normal
//! author="Bill Christmas"
//! file="Ravl/Image/Processing/Motion/LMSGradient/exLMSMultiScale.cc"

#include "Ravl/Option.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/DP/FileFormatIO.hh"
#include "Ravl/Image/LMSMultiScaleMotion.hh"
#include "Ravl/Image/LMSOpticFlow.hh"
#include "Ravl/Array1d.hh"
#include "Ravl/StrStream.hh"
#include "Ravl/Index.hh"
#include "Ravl/IO.hh"
	
using namespace RavlN;
using namespace RavlImageN;

int main (int argc, char **argv) {
  OptionC opt(argc, argv);
  IndexC n = opt.Int("n", 3, "No. of scales of image hierarchy");
  RealT noise = opt.Real("N", 1, "Noise level (in units of grey level)");
  // RealT scale = opt.Real("s", 1, "PostScript vector rescale factor");
  // RealT subsample = opt.Real("S", 1, "PostScript vector subsample factor");
  StringC params = opt.String("p", "", "Motion parameter file");
  IntT region = opt.Int("r", 9, "Region size for hierarchical l.m.s. fit");
  bool verb(!opt.Boolean("nv", false, "Turn off verbose o/p"));
  StringC opDisplay = opt.String("oi", "@X", "Output file name prefix");
  StringC inf1 = opt.String("", "", "1st image");
  StringC inf2 = opt.String("", "", "2nd image");
  
  opt.CompulsoryArgs(2);
  opt.Check();
  
  ImageC<RealT> im1; 
  RavlN::Load(inf1, im1);
  
  ImageC<RealT> im2; 
  RavlN::Load(inf2, im2);
  
  // Compute filter coefficients: currently 2:1 antialias filter
  Array1dC<RealT> coeffs(-4,4);
  StrIStreamC ("-4 4 -0.03008995 0.01247519 0.13510284 0.29130294 0.36395804 0.29130294 0.13510284 0.01247519 -0.03008995") >> coeffs;
  // preserve d.c. level
  RealT norm(0);
  for (IndexC i(coeffs.IMin()); i<=coeffs.IMax(); ++i) norm += coeffs[i];
  for (IndexC i(coeffs.IMin()); i<=coeffs.IMax(); ++i) coeffs[i] /= norm; 
  
  ConvolveSeparable2dC<RealT> filter(coeffs,coeffs);
  
  LMSOpticFlowC optic(verb);
  optic.SetNoiseLevel(noise);
  optic.SetRegionSize(region);
  LMSMultiScaleMotionC motion(filter, optic);
  //if (verb) motion.Verbose(scale,subsample);
  ImageC<Vector2dC> motionImg = motion.Estimate(PairC<ImageC<RealT> >(im1, im2), n);
  
  if(!opDisplay.IsEmpty()) {
    
    ImageC<ByteYUVValueC> img;
    LMSOpticFlowC::DrawMotion(im1,motionImg,img);
    RavlN::Save(opDisplay,img);
  }
  return 0;
}
