// This file is part of Ravl,  
// Copyright (C) 2006, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! author="Charles Galambos"
//! rcsid="$Id:$"
//! lib=RavlMath

#include "Ravl/Image/Image.hh"
#include "Ravl/Option.hh"
#include "Ravl/ThinPlateSpline1d.hh"
#include "Ravl/IO.hh"

using namespace RavlImageN;

int main(int nargs,char **argv) {
  OptionC opts(nargs,argv);
  RealT sigma = opts.Real("s",1,"Sigma. ");
  opts.Check();
  
  RealT size1 = 300;
  RealT size2 = 300;
  Array1dC<RealT> ins(5);
  Array1dC<RealT> outs(5);
  ins[0] = 0.10 * size2;
  ins[1] = 0.32 * size2;
  ins[2] = 0.51 * size2;
  ins[3] = 0.71 * size2;
  ins[4] = 0.9 * size2;
  outs[0] = 0.11 * size1;
  outs[1] = 0.23 * size1;
  outs[2] = 0.90 * size1;
  outs[3] = 0.31 * size1;
  outs[4] = 0.5 * size1;
  
  ThinPlateSpline1dC tps(sigma,ins,outs);
  ImageC<ByteT> img((IntT) size1,(IntT) size2);
  img.Fill(0);
  for(IntT i = 0; i < size2;i++) {
    int a = Round(tps.Apply((RealT) i));
    //cerr << "A=" << a << "\n";
    if(img.Range1().Contains(a))
      img[a][i] = 255;
  }
  
  RavlN::Save("@X",img);

  
  return 0;
}
