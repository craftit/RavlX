// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Signals/1D/exFFT1d.cc"
//! author="Charles Galambos"
//! docentry="Ravl.API.Math.Signals.1D"
//! userlevel=Normal

#include "Ravl/FFT1d.hh"
#include "Ravl/Random.hh"
#include "Ravl/Array1dIter.hh"

using namespace RavlN;

int main()
{
  int size = 16;
  FFT1dC fft(size);
  Array1dC<RealT> data(size);
  for(Array1dIterC<RealT> it(data);it;it++)
    it.Data() = Random1();
  std::cerr << "Doing FFT:\n";
  Array1dC<ComplexC> res = fft.Apply(data);
  std::cerr << "Result:";
  std::cerr << res << "\n";
  return 0;
}
