/*  ftuns.c    CCMATH mathematics library source code.
 *
 *  Copyright (C)  2000   Daniel A. Atkinson    All rights reserved.
 *  This code may be redistributed under the terms of the GNU library
 *  public license (LGPL). ( See the lgpl.license file for details.)
 * ------------------------------------------------------------------------
 */
#include "ccmath/ccmath.h"
void ftuns(struct ccomplex **pt,int n)
{ struct ccomplex **p,**q; int j; double x,y,u,v,h=2.;
  p=pt+1; q=pt+n-1;
  if(n%2==0) n/=2; else n=n/2+1;
  for(j=1; j<n ;++j){
    x=((*p)->re+(*q)->re)/h; y=((*p)->im-(*q)->im)/h;
    u=((*p)->im+(*q)->im)/h; v=((*p)->re-(*q)->re)/h;
    (*p)->re=x; (*p++)->im=y;
    (*q)->re=u; (*q--)->im=v;
   }
}
