// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCore
//! author="Charles Galambos"
//! docentry="Ravl.API.Math.QInt"
//! file="Ravl/Core/Base/exQInt.cc"

#include "Ravl/QInt.hh"
#include "Ravl/Option.hh"
#include "Ravl/CompilerHints.hh"

using namespace RavlN;

int main(int nargs,char **argv) {
  OptionC opt(nargs,argv);
  bool useQInt = opt.Boolean("q",false,"Use QInt methods. ");
  bool testFloor = opt.Boolean("f",false,"Test Floor() ");
  bool test = opt.Boolean("t",false,"Test QFloor and QRound ");
  RealT count = opt.Real("c",100000,"Size of test. ");
  opt.Check();
  
  IntT sum = 0;
  if(!test) {
    if(!testFloor) {
      if(useQInt) {
	cerr << "Using QRound. \n";
	for(RealT v = -count;v < count;v += 0.013) {
	  sum += QRound(v);
	}
      } else {
	cerr << "Using Round. \n";
	for(RealT v = -count;v < count;v += 0.013) {
	  sum += Round(v);
	}
      }
    } else {
      if(useQInt) {
	cerr << "Using QFloor. \n";
	for(RealT v = -count;v < count;v += 0.013) {
	  sum += QFloor(v);
	}
      } else {
	cerr << "Using Floor. \n";
	for(RealT v = -count;v < count;v += 0.013) {
	  sum += Floor(v);
	  //sum += floor(v);
	}
      }
    }
    std::cerr << "Sum=" << sum << "\n";
  } else {
    double values[] = { 0,0.1,1.1,-0.25,-3.6 , -3, 1.3,100000.01,1000.01,10000.1,10000.6,1000,-1000,-1000.6,-1e-20 };
    for(int i = 0;i < 15;i++) {
      if(testFloor) {
	cerr << "Floor " << values[i] << " N=" << Floor(values[i])  << " Q=" << QFloor(values[i]) << " floor=" << floor(values[i]) << "\n";
	if(floor(values[i]) != QFloor(values[i])) {
	  std::cerr << "*** QFloor test failed. *** \n";
	}
	if(floor(values[i]) != Floor(values[i])) {
	  std::cerr << "*** Floor test failed. *** \n";
	}
      } else {
	cerr << "Round " << values[i] << " N=" << Round(values[i])  << " Q=" << QRound(values[i]) << " \n";
	if(Round(values[i]) != QRound(values[i])) {
	  std::cerr << "test failed. \n";
	}
      }
    }
    std::cerr << "test passed. \n";
  }
  
  return 0;
}
