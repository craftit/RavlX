// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Signals/1D/testGenerateSignal1d.cc"
//! author="Charles Galambos"
//! docentry="Ravl.API.Math.Signals.1D"
//! userlevel=Develop

#include "Ravl/GenerateSignal1d.hh"
#include "Ravl/IntC.hh"

using namespace RavlN;

int testGauss();

int testBinomial();

int main() {
  int ln;
  if((ln = testBinomial()) != 0) {
    std::cerr << "Test failed line :" << ln << "\n";
    return 1;
  }
  if((ln = testGauss()) != 0) {
    std::cerr << "Test failed line :" << ln << "\n";
    return 1;
  }
  std::cerr << "Test passed. \n";
  return 0;
}

int testGauss() {
  Array1dC<RealT> res = GenerateGauss1d((size_t)1,0);
  if(res.Size() != 5) return __LINE__;
  if(Abs(res[0] - 0.398942) > 0.00001) return __LINE__;
  return 0;
}

int testBinomial() {
  // Test with real.
  {
    Array1dC<RealT> res = GenerateBinomial<RealT>(5);
    //cerr << res;
    if(res.Size() != 5) return __LINE__;
    if(res[0] != 1) return __LINE__;
    if(res[1] != 4) return __LINE__;
    if(res[2] != 6) return __LINE__;
    if(res[3] != 4) return __LINE__;
    if(res[4] != 1) return __LINE__;
    
    res = GenerateBinomial<RealT>((SizeT)5,false,true);
    if(res[0] != 6) return __LINE__;
    //cerr << res;
  }
  {
    Array1dC<IntC> res = GenerateBinomial<IntC>(5);
    //cerr << res;
    if(res.Size() != 5) return __LINE__;
    if(res[0] != 1) return __LINE__;
    if(res[1] != 4) return __LINE__;
    if(res[2] != 6) return __LINE__;
    if(res[3] != 4) return __LINE__;
    if(res[4] != 1) return __LINE__;
    
    res = GenerateBinomial<IntC>(5,false,true);
    if(res[0] != 6) return __LINE__;
    //cerr << res;
  }
  
  return 0;
}
