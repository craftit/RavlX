// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! docentry="Ravl.API.Images.Filtering"
//! userlevel=Develop
//! rcsid="$Id$"
//! lib=RavlImageProc
//! author="Charles Galambos"
//! file="Ravl/Image/Processing/Filters/exGaborFilter.cc"

#include "Ravl/Option.hh"
#include "Ravl/Image/GaborFilter2d.hh"
#include "Ravl/IO.hh"

using namespace RavlN;
using namespace RavlImageN;

int main(int nargs,char **argv) {
  OptionC opt(nargs,argv);
  opt.Check();
  
  Index2dC size(32,32);
  ImageC<RealT> img(size[0],size[1]);
  img.Fill(0);
  img[16][16] = 1;
  img[16][15] = 1;
  img[15][16] = 1;
  img[15][15] = 1;
  
  GaborFilter2dC<RealT> filter(size);
  SArray1dC<ImageC<RealT> > results = filter.Apply(img);
  
  for(UIntT i = 0;i < results.Size();i++) {
    StringC fn = StringC("@X:Gabor") + StringC(i);
    RavlN::Save(fn,results[i]);
  }
  return 0;
}
