// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#include "Ravl/CPUID.hh"
#include "Ravl/Option.hh"
#include <iostream>
//! lib=RavlCore


using namespace RavlN;


int main(int argc, char **argv) {

  CPUIDC cpuid;

  cpuid.Info();

  //test non class functions
  std::cout << "Non class functions test\n";
  std::cout << "MMX: " << MMX()  << std::endl;
  std::cout << "SSE: " << SSE()  << std::endl;
  std::cout << "SSE2:" << SSE2() << std::endl;
  std::cout << "HTT: " << HTT()  << std::endl;

  return 0;
}

