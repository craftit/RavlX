/*  tsvalS.c    CCMATH mathematics library source code.
 *
 *  Copyright (C)  2000   Daniel A. Atkinson    All rights reserved.
 *  This code may be redistributed under the terms of the GNU library
 *  public license (LGPL). ( See the lgpl.license file for details.)
 * ------------------------------------------------------------------------
 */
/*     Test for numerical instability in SVD code. The output
 *     will contain non-numeric values Inf (or possibly NaN)
 *     if the QR fix has not been installed correctly.
 *     This can be tested by piping the output into the shell
 *     command: grep -c 'Inf'.
 *
 *     input integer N = number of test cases to generate.
 *
*/
/* Modified by Charles Galambos for use with QMake testing. */
#include "Ravl/StdMath.hh"
#include <time.h>
#include <stdio.h>
#include "ccmath/ccmath.h"
#include "Ravl/Stream.hh"
#include "Ravl/config.h"
using namespace RavlN;
#if RAVL_COMPILER_MIPSPRO
ExceptionC dummyvar ("dummy") ;
#endif

int main(int na,char **av)
{  
  double d[4], a[16];
  int i,j,nn;
  unsigned int seed;
  nn=5000;
  seed=(unsigned int)time(0);
  setunfl(seed);
  cout << "Seed used = " << seed << "\n";
  for(j=0; j<nn ;++j){
    for(i=0; i<16;++i) a[i] = 2.*unfl()-1.;
    svdval ( d, a, 4, 4 );
    for(int i = 0;i < 4;i++) {
      if(IsInf(d[i])) {
	cerr << "Test failed " << j << d[0] << " " << d[1]  << " " << d[2] << " " << d[3] << "\n";
	return 1;
      }
    }
    //printf ( "%d : %f %f %f %f\n", j, d[0], d[1], d[2], d[3] );
  }
  cout << "Test passed. \n";
  return 0;
}


