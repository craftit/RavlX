// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/LinearAlgebra/General/testMatInv.cc"

#include "Ravl/Matrix.hh"
#include "Ravl/MatrixRS.hh"
#include "Ravl/Random.hh"
#include "Ravl/Option.hh"
#include "Ravl/SArray2dIter.hh"

using namespace RavlN;

enum InvMethodT { INV_NORMAL ,INV_NEARSINGULAR, INV_PSEUDO };

int testInverse(int n,int r,InvMethodT method);


int main(int argc,char **argv) {
  OptionC  option(argc,argv,true);  
  //bool verbose  = option.Boolean("v",FALSE,"Verbose mode.");
  IntT n = option.Int("m",30,"Size of matrix to use. ");
  IntT r = option.Int("r",10,"Do r experiments. ");
  InvMethodT method = (InvMethodT) option.Int("mn",0,"Method 0=Normal, 1=NearSingular, 2=Pseudo . ");
  bool doAll = !option.IsOnCommandLine("-mn");
  
  option.Check();
  if(!doAll)
    return testInverse(n,r,method);
  for(int i = 0;i < 3;i++) {
    if(testInverse(n,r,(InvMethodT) i) != 0) {
      cerr << "Test " << i << " failed. \n";
      return 1;
    }
  }  
  cerr << "Tested passed ok. \n";
  return 0;
}


int testInverse(int n,int r,InvMethodT method) {
  switch(method) {
  case INV_NEARSINGULAR: 
    cerr << "Testing near singular.\n"; 
    break;
  case INV_PSEUDO:cerr << "Testing pseudo inverse.\n"; break;
  case INV_NORMAL:cerr << "Testing normal inverse.\n"; break;
  }
  int fail = 0;
  for(IntT i = 0;i < r;i++) {
    MatrixC x;
    switch(method) {
    case INV_NEARSINGULAR: 
      x = RandomPositiveDefiniteMatrix(n); 
      break;
    case INV_PSEUDO:
    case INV_NORMAL:
      x = RandomMatrix(n,n);
      break;
    }
    MatrixC y;
    RealT det;
    switch(method) {
    case INV_NORMAL:	
      y = x.Inverse(); break;
    case INV_NEARSINGULAR: 
      {
	MatrixRSC xrs(x);
	xrs.MakeSymmetric();
	y = xrs.NearSingularInverse(det); 
      }
      break;
    case INV_PSEUDO: 
      y = x.PseudoInverse(); break;
    }
    if(!MatrixC(x.Copy()).InverseIP()) {
      cerr << "Test matrix is singular. \n";
      return 0;
    }
    MatrixC z = (y * x) - MatrixC::Identity(n);
    RealT ret = (z.SumOfAbs() * 100)/(n*n);
    cout <<" Remainder:" << ret << endl;
    if(ret > 1e-8) {
      cerr << "WARNING: Test Failed. \n";
      fail++;
      //cerr << "Mat=" << x << "\n";
      return 1;
    }
  }
  if(fail == 0)
    return 0;
  return 1;
}
