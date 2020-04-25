// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id: doFilter.cc 5240 2005-12-06 17:16:50Z plugger $"
//! lib=RavlImageProc
//! author="Charles Galambos"
//! file="Ravl/Image/Processing/Filters/doFilter.cc"
//! userlevel=Normal
//! docentry="Ravl.API.Images.Filtering"

//: Apply a user-specified separable convolution to an image.

#include "Ravl/Option.hh"
#include "Ravl/IO.hh"
#include "Ravl/Image/ConvolveSeparable2d.hh"
#include "Ravl/Array1d.hh"
#include "Ravl/Array1dIter.hh"
#include "Ravl/DP/SequenceIO.hh"

using namespace RavlN;
using namespace RavlImageN;

int main(int nargs,char **argv) {
  OptionC opt(nargs,argv);
  bool seq = opt.Boolean("seq","Process a sequence. ");
  bool norm = opt.Boolean("norm","Normalise coefficients");
  StringC inf = opt.String("i","in.ppm","Input file");
  StringC outf = opt.String("o","out.ppm","Output file");
  bool row = opt.Boolean("h","horizontal filtering only");
  bool col = opt.Boolean("v","vertical filtering only");
  DListC<StringC>coeffList = opt.List("c","1","List of coefficients");
  opt.Check();
  
  ImageC<RealT> img;
  ImageC<RealT> res;
  SizeT s(coeffList.Size());
  Array1dC<RealT> coeffs(-(s/2),s-(s/2)-1);
  RealT sum(0);
  for (Array1dIterC<RealT>i(coeffs); i; ++i) {
    *i = coeffList.First().RealValue();
    sum += Abs(*i);
    coeffList.PopFirst();
  }
  if (norm) coeffs /= sum;
  Array1dC<RealT> dummy(1);
  dummy[0] = 1.0;
  ConvolveSeparable2dC<RealT,RealT> hf;
  if (row) hf.SetKernel(dummy, coeffs);
  else if (col) hf.SetKernel(coeffs, dummy);
  else hf.SetKernel(coeffs, coeffs);
  if(!seq) {
    if(!Load(inf,img)) {
      cerr << "Failed to load image '" << inf << "'\n";
      return 1;
    }
    res = hf.Apply(img);
    if(!Save(outf,res)) {
      cerr << "Failed to save image '" << outf << "'\n";
      return 1;
    }
  } else {
    DPIPortC<ImageC<RealT> > in;
    if(!OpenISequence(in,inf)) {
      cerr << "Failed to open input file '" << inf << "'\n";
      return 1;
    }
    DPOPortC<ImageC<RealT> > out;
    if(!OpenOSequence(out,outf)) {
      cerr << "Failed to open output file '" << outf << "'\n";
      return 1;
    }
    ImageC<RealT> img;
    while(in.Get(img)) {
      res = hf.Apply(img);
      out.Put(res);
    }
  }
  return 0;
}
