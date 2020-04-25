// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImageProc
//! author="Charles Galambos"
//! file="Ravl/Image/Processing/Filters/doFilter.cc"
//! userlevel=Normal
//! docentry="Ravl.API.Images.Filtering"

//: Apply the homomorphic transform to an image.

#include "Ravl/Option.hh"
#include "Ravl/IO.hh"
#include "Ravl/Image/HomomorphicFilter.hh"
#include "Ravl/DP/SequenceIO.hh"

using namespace RavlN;
using namespace RavlImageN;

int main(int nargs,char **argv) {
  OptionC opt(nargs,argv);
  bool bypass = opt.Boolean("b",false,"Bypass filter. ");
  RealT depth = opt.Real("d",0.5,"Depth of homomorphic filter. ");
  RealT sigma = opt.Real("s",5,"Width of filter. ");
  bool seq = opt.Boolean("seq",false,"Process a sequence. ");
  StringC inf = opt.String("","in.ppm","Input file.");
  StringC outf = opt.String("","out.ppm","Input file.");
  opt.Check();
  
  ImageC<RealT> img;
  ImageC<RealT> res;
  HomomorphicFilterC hf(sigma,depth);
  if(!seq) {
    if(!Load(inf,img)) {
      std::cerr << "Failed to load image '" << inf << "'\n";
      return 1;
    }
    if(!bypass)
      res = hf.Apply(img);
    else
      res = img;
    if(!Save(outf,res)) {
      std::cerr << "Failed to save image '" << outf << "'\n";
      return 1;
    }
  } else {
    DPIPortC<ImageC<RealT> > in;
    if(!OpenISequence(in,inf)) {
      std::cerr << "Failed to open input file '" << inf << "'\n";
      return 1;
    }
    DPOPortC<ImageC<RealT> > out;
    if(!OpenOSequence(out,outf)) {
      std::cerr << "Failed to open output file '" << outf << "'\n";
      return 1;
    }
    ImageC<RealT> img;
    while(in.Get(img)) {
      if(!bypass)
	res = hf.Apply(img);
      else
	res = img;
      out.Put(res);
    }
  }
  return 0;
}
