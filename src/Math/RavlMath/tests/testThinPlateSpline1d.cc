// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlMath

#include "Ravl/ThinPlateSpline1d.hh"

int TestThinPlateSpline1d_Linear();
int TestThinPlateSpline1d_Poly();

using namespace RavlN;

int main() {
  int ln;
  if((ln =  TestThinPlateSpline1d_Linear()) != 0) {
    std::cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  if((ln =  TestThinPlateSpline1d_Poly()) != 0) {
    std::cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  std::cout << "Test passed. \n";
  return 0;
}

int TestThinPlateSpline1d_Linear() {

  // Test a simple linear mapping.
  
  Array1dC<RealT> pnt0(5);
  Array1dC<RealT> pnt1(5);
  for(IntT i = 0;i < 5;i++) {
    pnt0[i] = i;
    pnt1[i] = i * 2.5;
  }
    
  ThinPlateSpline1dC tps(0.8,pnt0,pnt1);
  for(RealT j = -1;j < 6;j += 0.3) {
    RealT v = tps.Apply(j);
    //cout << " " << j << " = " << v << "\n";
    if(Abs(v - (j * 2.5)) > 0.01)
      return __LINE__;
  }
  return 0;
}

int TestThinPlateSpline1d_Poly() {

  // Test a simple linear mapping.
  
  Array1dC<RealT> pnt0(5);
  Array1dC<RealT> pnt1(5);
  for(IntT i = 0;i < 5;i++) {
    pnt0[i] = i;
    pnt1[i] = Sqr(i);
  }
    
  ThinPlateSpline1dC tps(0.8,pnt0,pnt1);
  for(RealT j = 0;j < 5;j += 0.3) {
    RealT v = tps.Apply(j);
    //cout << " " << j << " = " << v << " (" << Sqr(j) << ") \n";
    if(Abs(v - Sqr(j)) > 0.5)
      return __LINE__;
  }
  return 0;
}
