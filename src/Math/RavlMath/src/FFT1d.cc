// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Signals/1D/FFT1d.cc"

#include "Ravl/FFT1d.hh"
#include "Ravl/Slice1d.hh"
#include "Ravl/Exception.hh"
#include "ccmath/ccmath.h"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  //: Constructor.
  
  FFT1dBodyC::FFT1dBodyC(int nn,bool iinv,bool nZeroPad) 
    : n(0),
      inv(iinv),
      zeroPad(nZeroPad)
  { Init(nn,iinv); }
  
  //: Destructor
  
  FFT1dBodyC::~FFT1dBodyC()  {
  }
  
  //: Create a plan with the given setup.
  
  bool FFT1dBodyC::Init(int nn,bool iinv) {
    // Remeber settings in case we asked...
    inv = iinv;
    pwr2 = IsPow2(nn);
    n = nn;
#if 0
    for(int i = 0;i < 32;i++)
      primeFactors[i] = 0;
#endif
    nf = pfac(n,primeFactors,'o');
    RavlAssertMsg(nf == n,"FFT1dBodyC::Init(), Failed to find prime factors. ");
    return true;
  }
  
  //: Apply transform to array.
  
  Array1dC<ComplexC> FFT1dBodyC::Apply(const Array1dC<ComplexC> &dat)
  {
    ONDEBUG(std::cerr << "FFT1dBodyC::Apply(Array1dC<ComplexC>) n=" << n << " inv=" << inv << " \n");
    Array1dC<ComplexC> ret(n);
    Array1dC<ComplexC> tmpArr(n);
    if (dat.Size() < (UIntT) n && zeroPad) {
      ONDEBUG(std::cerr << "Zero padding. \n");
      ret = Array1dC<ComplexC>(n);
      // Copy original data.
      for (BufferAccessIter2C<ComplexC, ComplexC> it(dat, tmpArr); it; it++)
        it.Data2() = it.Data1();
      // Zero pad it.
      for (BufferAccessIterC<ComplexC> ita(tmpArr, IndexRangeC(dat.Size(), n - 1)); ita; ita++)
        *ita = 0;
    } else {
      RavlAssert(dat.Size() == (UIntT) n);
      tmpArr = dat.Copy();
    }
    Array1dC<ccomplex *> ptrArr(n);
    //ptrArr.Fill(0);
    //cerr << dat <<  "\n";
    // TODO :- Would it be quicker to copy the array and use fft2 if length is a power of two ?
    if (inv) { // Do inverse.
      for (BufferAccessIter2C<ccomplex *, ComplexC> it(ptrArr, tmpArr); it; it++)
        it.Data1() = (ccomplex *) (&it.Data2());
      fftgc((ccomplex **) ((void *) &(ptrArr[0])), (ccomplex *) ((void *) &(tmpArr[0])), n, primeFactors, 'i');
      for (BufferAccessIter2C<ccomplex *, ComplexC> itb(ptrArr, ret); itb; itb++)
        itb.Data2() = *((ComplexC *) itb.Data1());
    } else { // Do forward.
      for (BufferAccessIter2C<ccomplex *, ComplexC> it(ptrArr, ret); it; it++)
        it.Data1() = (ccomplex *) (&it.Data2());

      fftgc((ccomplex **) ((void *) &(ptrArr[0])), (ccomplex *) ((void *) &(tmpArr[0])), n, primeFactors, 'd');

      for (BufferAccessIter2C<ccomplex *, ComplexC> itb(ptrArr, ret); itb; itb++)
        itb.Data2() = *((ComplexC *) itb.Data1());
    }

    //cerr << "result:" << ret << "\n";;
    return ret;
  }
  
  //: Apply transform to array.
  
  Array1dC<ComplexC> FFT1dBodyC::Apply(const Array1dC<RealT> &dat) {
    ONDEBUG(std::cerr << "FFT1dBodyC::Apply(Array1dC<RealT>) n=" << n << " inv=" << inv << " \n");
    if(dat.Size() < (UIntT) n && zeroPad) {
      ONDEBUG(std::cerr << "Zero padding. \n");
      Array1dC<RealT> ndat(n);
      // Copy original data.
      for(BufferAccessIter2C<RealT,RealT> it(dat,ndat);it;it++)
        it.Data2() = it.Data1();
      // Zero pad it.
      for(BufferAccessIterC<RealT> ita(ndat,IndexRangeC(dat.Size(),n-1));ita;ita++)
        *ita = 0;
      // Then try again.
      return Apply(ndat);
    } else {
      RavlAssert(dat.Size() == (UIntT) n);
    }
    Array1dC<ComplexC> ret(n); 
    if(inv)
      fftgr((double *) &(dat[0]),
	    (ccomplex *) ((void *)&(ret[0])),
	    n,
	    primeFactors,
	    'i');
    else
      fftgr((double *) &(dat[0]),
	    (ccomplex *) ((void *)&(ret[0])),
	    n,
	    primeFactors,
	    'd');
    return ret; 
  }

  
}
