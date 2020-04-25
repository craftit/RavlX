// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLCCMATH_HEADER
#define RAVLCCMATH_HEADER 1
//////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Math/CCMath/CCMath.hh"
//! lib=RavlMath
//! userlevel=Develop
//! docentry="Ravl.API.Math.Internal"
//! author="Daniel A. Atkinson"

extern "C" {
  
  int minv(double *a,int n) ;
  int psinv(double *v,int n) ;
  int ruinv(double *a,int n) ;
  int solv(double *a,double *b,int n) ;
  int solvps(double *s,double *x,int n) ;
  int solvru(double *a,double *b,int n) ;
  void solvtd(double *a,double *b,double *c,double *x,int m) ;
  int eigen(double *a,double *eval,int n) ;
  int eigval(double *a,double *eval,int n) ;
  double evmax(double *a,double *u,int n) ;
  int svdval(double *d,double *a,int m,int n) ;
  int sv2val(double *d,double *a,int m,int n) ;
  int svduv(double *d,double *a,double *u,int m,double *v,int n) ;
  int sv2uv(double *d,double *a,double *u,int m,double *v,int n) ;
  int svdu1v(double *d,double *a,int m,double *v,int n) ;
  int sv2u1v(double *d,double *a,int m,double *v,int n) ;
  void mmul(double *mat,double *a,double *b,int n) ;
  void rmmult(double *mat,double *a,double *b,int m,int k,int n) ;
  void vmul(double *vp,double *mat,double *v,int n) ;
  double vnrm(double *u,double *v,int n) ;
  void matprt(double *a,int n,int m,char *fmt) ;
  void trnm(double *a,int n) ;
  void mattr(double *a,double *b,int m,int n) ;
  void otrma(double *at,double *u,double *a,int n) ;
  void otrsm(double *st,double *u,double *s0,int n) ;
  void mcopy(double *a,double *b,int m) ;
  void ortho(double *evc,int n) ;
  void smgen(double *a,double *eval,double *evec,int n) ;
  

#if 0  
  void chcof(double *c,int m,double (*func)()) ;
  
  /* functional form: double (*func)(double) */
  
  void chpade(double *c,double *a,int m,double *b,int n) ;
  double ftch(double x,double *a,int m,double *b,int n) ;
  void cspl(double *x,double *y,double *z,int m,double tn) ;
  void csplp(double *x,double *y,double *z,int m,double tn) ;
  double csfit(double w,double *x,double *y,double *z,int m) ;
  double tnsfit(double w,double *x,double *y,double *z,
                   int m,double tn) ;
  double dcspl(double x,double *u,double *v,double *z,int m) ;
  
  /* polynominal least squares functions use the Opol structure. */
  
  void plsq(double *x,double *y,int n,Opol *c,double *ssq,int m) ;
  double pplsq(double *x,double *y,int n,double *b,int m) ;
  double evpsq(double x,Opol *c,int m) ;
  double evpsqv(double x, Opol *c,int m,double *sig,double sqv) ;
  void psqcf(double *pc,Opol *c,int m) ;
  void psqvar(double *var,double s,Opol *c,int m) ;


   /* QR transformation for linear least squares. */
  
  double qrlsq(double *a,double *b,int m,int n,int *f) ;
  double qrvar(double *v,int m,int n,double ssq) ;
  
  
  /* singular value decomposition least squares. */     
  
  double lsqsv(double *x,int *pr,double *var,double *d,double *b,
                  double *v,int m,int n,double th) ;
  int svdlsq(double *d,double *a,double *b,int m,double *v,int n) ;
  int sv2lsq(double *d,double *a,double *b,int m,double *v,int n) ;
  
    
  /* nonlinear least squares */
  
  double seqlsq(double *x,double *y,int n,double *par,double *var,
                   int m,double de,double (*func)(),int kf) ;
  
  /* functional form: double (*func)(double x,double *par) */
  
  double gnlsq(double *x,double *y,int n,double *par,
	       double *var,int m,double de,double (*func)()) ;
  
  /* functional form: double (*func)(double x,double *par) */
  
  double fitval(double x,double *s,double *par,double (*fun)(),
		double *v,int n) ;
  
  /* functional form: double (*func)(double x,double *par) */
  
  void setfval(int i,int n) ;
#endif
}

#endif
