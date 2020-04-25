// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImageProc
//! author="Kieron J Messer"
//! docentry="Ravl.API.Images.Filtering"
//! userlevel=Normal
//! file="Ravl/Image/Processing/Filters/exGaussConvolve.cc"

#include "Ravl/Option.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/DP/FileFormatIO.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Image/GaussConvolve2d.hh"

using namespace RavlImageN;

int main(int argc, char **argv)
{  
  OptionC   opt(argc,argv);
  FilenameC   inIm = opt.String("i","in.pgm", "input image");
  FilenameC   outIm = opt.String("o","out.pgm", "output image");
  UIntT       order = opt.Int("order", 5, "order of gauss convolve");
  bool    Verbose = opt.Boolean("v", false, "verbose");
  opt.Compulsory("i");
  opt.Check();

  // Working image...
  ImageC<RealT> in, out;
  
  // Setup filter.
  GaussConvolve2dC<RealT> gc(order);
  
  // load in an image
  
  RavlN::Load(inIm, in, "", Verbose);
  
  // Filter image (method 1)
  out = gc.Apply(in);
  
  // Filter image (method 2)
  gc.Apply(in, out);
  
  // Filter image (method 3)
  gc(in, out);
  
  // Save result.
  RavlN::Save(outIm,in, "", Verbose);
  
  return 0;
}

