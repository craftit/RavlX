/*  fft2_d.c    CCMATH mathematics library source code.
 *
 *  Copyright (C)  2000   Daniel A. Atkinson    All rights reserved.
 *  This code may be redistributed under the terms of the GNU library
 *  public license (LGPL). ( See the lgpl.license file for details.)
 * ------------------------------------------------------------------------
 */
#include "ccmath/ccmath.h"
#include <stdlib.h>
void fft2(struct ccomplex *ff,int k,int j);
void fft2_d(struct ccomplex *a,int m,int n,int f)
{ int md,nd,i,j; struct ccomplex *p,*q;
  struct ccomplex *r,*s;
  md=1<<m; nd=1<<n;
  for(i=0,p=a; i<md ;++i){
    fft2(p,n,f); p+=nd;
   }
  q=(struct ccomplex *)calloc(sizeof(*q),md);
  for(i=0,p=a-nd; i<nd ;++i){
    for(r=q,s=p,j=0; j<md ;++j) *r++ = *(s+=nd);
    fft2(q,m,f);
    for(r=q,s=p++,j=0; j<md ;++j) *(s+=nd)= *r++;
   }
  free(q);
}
