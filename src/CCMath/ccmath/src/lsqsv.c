/*  lsqsv.c    CCMATH mathematics library source code.
 *
 *  Copyright (C)  2000   Daniel A. Atkinson    All rights reserved.
 *  This code may be redistributed under the terms of the GNU library
 *  public license (LGPL). ( See the lgpl.license file for details.)
 * ------------------------------------------------------------------------
 */
#include <stdlib.h>
#include "ccmath/ccmath.h"
double lsqsv(double *x,int *pr,double *var,double *d,double *b,double *v,
		int m,int n,double th)
{ double ssq,sig,*y,*p;
  int i,k;
  y=(double *)calloc(n,sizeof(double));
  for(i=n,ssq=0.,p=b+n; i<m ;++i,++p) ssq+= *p* *p;
  for(i=k=0; i<n ;++i){
    if(d[i]<th){ y[i]=0.; ssq+=b[i]*b[i];}
    else{ y[i]=b[i]/d[i]; ++k;}
   }
  *pr=k;
  vmul(x,v,y,n);
  if(var!=0 && m>n){
    sig=ssq/(double)(m-n);
    for(i=0; i<n ;++i){
      if(d[i]<th) y[i]=0.; else y[i]=sig/(d[i]*d[i]);
     }
    smgen(var,y,v,n);
   }
  free(y);
  return ssq;
}
