// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Signals/2D/FFT2d.cc"

#include "Ravl/FFT2d.hh"
#include "Ravl/Exception.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/SArray2dIter2.hh"
#include "Ravl/SArray2dIter3.hh"
#include "Ravl/Slice1d.hh"
#include "Ravl/Assert.hh"

#include "ccmath/ccmath.h"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif


namespace RavlN {

#if RAVL_COMPILER_MIPSPRO
  static Slice1dC<ComplexC*> fixSGIBug1;
  static Slice1dIter2C<ComplexC*,ComplexC> fixSGIBug2;
#endif

  //: Constructor.

  FFT2dBodyC::FFT2dBodyC(Index2dC nn,bool iinv)
    : n(0),
      m(0),
      inv(iinv),
      base2(false)
  { Init(nn,iinv); }

  //: Destructor

  FFT2dBodyC::~FFT2dBodyC()  {
  }

  //: Create a plan with the given setup.

  bool FFT2dBodyC::Init(Index2dC nsize,bool iinv) {
    // Remeber settings in case we asked...
    inv = iinv;
    size = nsize;
    if(IsPow2(size[0].V()) && IsPow2(size[1].V())) {
      ONDEBUG(cerr << "FFT2dBodyC::Init(), Initalise power of 2 transform. \n");
      base2 = true;
      m = ILog2(size[0].V());
      n = ILog2(size[1].V());
    } else {
      ONDEBUG(cerr << "FFT2dBodyC::Init(), Initalise arbitary size transform. \n");
      base2 = false;
      int nf1 = pfac(size[0].V(),primeFactors1,'o');
      int nf2 = pfac(size[1].V(),primeFactors2,'o');

      // CCMath pfac (prime factor) function does not work for prime factors > 101.
      StringC errTxt0 = ((StringC)"FFT2dBodyC::Init(), row size of "    + StringC( size[0].V() ) + (StringC)" has a prime factor > 101.");
      StringC errTxt1 = ((StringC)"FFT2dBodyC::Init(), column size of " +  StringC( size[1].V() ) + (StringC)" has a prime factor > 101.");
      RavlAlwaysAssertMsg( size[0].V() == nf1, &errTxt0[0]);
      RavlAlwaysAssertMsg( size[1].V() == nf2, &errTxt1[0]);
    }
    return true;
  }

  //: Apply transform to array.

  SArray2dC<ComplexC> FFT2dBodyC::Apply(const SArray2dC<ComplexC> &dat) const
  {
    ONDEBUG(cerr << "FFT2dBodyC::Apply(SArray2dC<ComplexC>) n=" << n << " inv=" << inv << " Size=" << size << "\n");
    RavlAssert(dat.Size1() == (UIntT) size[0].V());
    RavlAssert(dat.Size2() == (UIntT) size[1].V());
    SArray2dC<ComplexC> ret;
    if(base2) {
      ret = dat.Copy();
      //cerr << dat <<  "\n";
      if(inv)
	fft2_d((ccomplex *) ((void *) ret[0].DataStart()),m,n,'i');
      else
	fft2_d((ccomplex *) ((void *) ret[0].DataStart()),m,n,'d');
      //cerr << "result:" << ret << "\n";;
      return ret;
    }
    SArray2dC<ComplexC> tmp1(size[0].V(),size[1].V());
    SArray2dC<ComplexC *> tmp1Ptr(size[0].V(),size[1].V());
    ret = SArray2dC<ComplexC>(size[0].V(),size[1].V());
    int i,j;
    SArray1dC<ComplexC> idat(size[0].V());
    SArray1dC<ComplexC *> ptrArr(size[0].V());
    for(SArray2dIter3C<ComplexC *,ComplexC,ComplexC> it(tmp1Ptr,tmp1,dat);it;it++) {
      it.Data2() = it.Data3();
      it.Data1() = &it.Data2();
    }

    if(inv) {
      // fft of rows.
      for(i = 0;i < (int) dat.Size1();i++)
	fftgc((Cpx **)((void *) (tmp1Ptr[i].DataStart())),
	      (ccomplex *)((void *) (tmp1[i].DataStart())),
	      size[1].V(),(int*)primeFactors2,'i');

      // fft of cols.
      for(j = 0;j < (int) dat.Size2();j++) {
	Slice1dC<ComplexC> slice(tmp1.Buffer().Data(),tmp1Ptr[0][j],tmp1.Size1(),tmp1.Stride());
	idat.CopyFrom(slice);
	for(BufferAccessIter2C<ComplexC *,ComplexC> it(ptrArr,idat);it;it++)
	  it.Data1() = &it.Data2();
	//cerr << idat << "\n";
	fftgc((ccomplex **) ((void *)&(ptrArr[0])),
	      (ccomplex *) ((void *) idat.DataStart()),
	      idat.Size(),(int*)primeFactors1,'i');

	for(Slice1dIter2C<ComplexC *,ComplexC> itb(ptrArr.Slice1d(),ret.SliceColumn(j));itb;itb++)
	  itb.Data2() = *itb.Data1();
      }
    } else {
      // fft of rows.
      for(i = 0;i < (int) dat.Size1();i++)
	fftgc((Cpx **)((void *) (tmp1Ptr[i].DataStart())),
	      (ccomplex *)((void *) (tmp1[i].DataStart())),
	      size[1].V(),(int*)primeFactors2,'d');
      // fft of cols.
      for(j = 0;j < (int) dat.Size2();j++) {
	Slice1dC<ComplexC> slice(tmp1.Buffer().Data(),tmp1Ptr[0][j],dat.Size1(),dat.Stride());
	idat.CopyFrom(slice);
	//cerr << const_cast<SArray2dC<ComplexC> &>(dat).SliceColumn(j) << "\n";
	for(BufferAccessIter2C<ComplexC *,ComplexC> it(ptrArr,idat);it;it++)
	  it.Data1() = &it.Data2();

	fftgc((ccomplex **) ((void *) ptrArr.DataStart()),
	      (ccomplex *) ((void *) idat.DataStart()),
	      idat.Size(),(int*)primeFactors1,'d');

	for(Slice1dIter2C<ComplexC *,ComplexC> itb(ptrArr.Slice1d(),ret.SliceColumn(j));itb;itb++)
	  itb.Data2() = *itb.Data1();
      }
    }
    return ret;
  }

  //: Apply transform to array.

  SArray2dC<ComplexC> FFT2dBodyC::Apply(const SArray2dC<RealT> &dat) const {
    ONDEBUG(cerr << "FFT2dBodyC::Apply(SArray2dC<ComplexC>) n=" << n << " inv=" << inv << " \n");
    RavlAssert(dat.Size1() == (UIntT) size[0].V());
    RavlAssert(dat.Size2() == (UIntT) size[1].V());
    SArray2dC<ComplexC> ret(dat.Size1(),dat.Size2());
    if(base2) {
      for(SArray2dIter2C<RealT,ComplexC> it(dat,ret);it;it++)
	it.Data2() = ComplexC(it.Data1(),0);
      //cerr << dat <<  "\n";
      if(inv)
	fft2_d((ccomplex *) ((void *) ret[0].DataStart()),m,n,'i');
      else
	fft2_d((ccomplex *) ((void *) ret[0].DataStart()),m,n,'d');
      //cerr << "result:" << ret << "\n";;
      return ret;
    }
    int i,j;
    SArray1dC<ComplexC> idat(size[0].V());
    SArray1dC<ComplexC *> ptrArr(size[0].V());
    SArray2dC<ComplexC> tmp1(size[0].V(),size[1].V());
    SArray2dC<ComplexC *> tmp1Ptr(size[0].V(),size[1].V());

    if(inv) {
      // fft of rows.
      for(i = 0;i < (int) dat.Size1();i++)
	fftgr(dat[i].DataStart(),
	      (ccomplex *)((void *) tmp1[i].DataStart()),
	      size[1].V(),(int*)primeFactors2,'i');
      // fft of cols.
      for(j = 0;j < (int) dat.Size2();j++) {
	idat.CopyFrom(tmp1.SliceColumn(j));
	for(BufferAccessIter2C<ComplexC *,ComplexC> it(ptrArr,idat);it;it++)
	  it.Data1() = &it.Data2();

	fftgc((ccomplex **) ((void *)&(ptrArr[0])),
	      (ccomplex *) ((void *) idat.DataStart()),
	      idat.Size(),(int*)primeFactors1,'i');

	for(Slice1dIter2C<ComplexC *,ComplexC> itb(ptrArr.Slice1d(),ret.SliceColumn(j));itb;itb++)
	  itb.Data2() = *itb.Data1();
      }
    } else {
      // fft of rows.
      for(i = 0;i < (int) dat.Size1();i++)
	fftgr(dat[i].DataStart(),
	      (ccomplex *)((void *) tmp1[i].DataStart()),
	      size[1].V(),(int*)primeFactors2,'d');
      // fft of cols.
      for(j = 0;j < (int) dat.Size2();j++) {
	idat.CopyFrom(tmp1.SliceColumn(j));
	for(BufferAccessIter2C<ComplexC *,ComplexC> it(ptrArr,idat);it;it++)
	  it.Data1() = &it.Data2();

	fftgc((ccomplex **) ((void *)&(ptrArr[0])),
	      (ccomplex *) ((void *) idat.DataStart()),
	      idat.Size(),(int*)primeFactors1,'d');

	for(Slice1dIter2C<ComplexC *,ComplexC> itb(ptrArr.Slice1d(),ret.SliceColumn(j));itb;itb++)
	  itb.Data2() = *itb.Data1();
      }
    }
    return ret;
  }


  //: FFTShift image of complex numbers
  SArray2dC<ComplexC> FFT2dC::FFTShift(const SArray2dC<ComplexC> &dat)
  {
    //: Quartile numbering: 0 1
    //:                     2 3
    //: Reordering:         0 1   --->  3 2
    //:                     2 3         1 0
    //: Care is required for odd sizes: 0 1 2 ---> 5 3 4
    //:                                 3 4 5      2 0 1

    //: Input quartiles
    //=================
    Index2dC datQuartile3TopLeft( dat.Size1() / 2, dat.Size2() / 2 );
    if( dat.Size1() & true ) datQuartile3TopLeft.Row()++;
    if( dat.Size2() & true ) datQuartile3TopLeft.Col()++;

    IndexRange2dC datQuartile0Range( 0, datQuartile3TopLeft.Row() - 1,
                                     0, datQuartile3TopLeft.Col() - 1 );
    IndexRange2dC datQuartile1Range( 0, datQuartile3TopLeft.Row() - 1,
                                     datQuartile3TopLeft.Col(), dat.Size2() - 1 );
    IndexRange2dC datQuartile2Range( datQuartile3TopLeft.Row(), dat.Size1() - 1,
                                     0, datQuartile3TopLeft.Col() - 1 );
    IndexRange2dC datQuartile3Range( datQuartile3TopLeft.Row(), dat.Size1() - 1,
                                     datQuartile3TopLeft.Col(), dat.Size2() - 1 );

    //: Output quartiles
    //==================
    IndexRange2dC resQuartile0Range = datQuartile3Range;
    IndexRange2dC resQuartile1Range = datQuartile2Range;
    IndexRange2dC resQuartile2Range = datQuartile1Range;
    IndexRange2dC resQuartile3Range = datQuartile0Range;

    Index2dC resQuartile0Origin( 0              , 0 );
    Index2dC resQuartile1Origin( 0              , dat.Size2() / 2 );
    Index2dC resQuartile2Origin( dat.Size1() / 2, 0 );
    Index2dC resQuartile3Origin( dat.Size1() / 2, dat.Size2() / 2 );

    resQuartile0Range.SetOrigin(resQuartile0Origin);
    resQuartile1Range.SetOrigin(resQuartile1Origin);
    resQuartile2Range.SetOrigin(resQuartile2Origin);
    resQuartile3Range.SetOrigin(resQuartile3Origin);

    //: Copy each quartile of dat to shifted
    //======================================
    SArray2dC<ComplexC> shifted(dat.Size1(), dat.Size2());

    SArray2dC<ComplexC> datQuartile3(const_cast<SArray2dC<ComplexC> &>(dat), datQuartile3Range);
    SArray2dC<ComplexC> resQuartile0(shifted, resQuartile0Range);
    for( SArray2dIter2C<ComplexC, ComplexC> it(datQuartile3, resQuartile0); it; it++ )
      it.Data2() = it.Data1();

    SArray2dC<ComplexC> datQuartile0(const_cast<SArray2dC<ComplexC> &>(dat), datQuartile0Range);
    SArray2dC<ComplexC> resQuartile3(shifted, resQuartile3Range);
    for( SArray2dIter2C<ComplexC, ComplexC> it(datQuartile0, resQuartile3); it; it++ )
      it.Data2() = it.Data1();

    SArray2dC<ComplexC> datQuartile2(const_cast<SArray2dC<ComplexC> &>(dat), datQuartile2Range);
    SArray2dC<ComplexC> resQuartile1(shifted, resQuartile1Range);
    for( SArray2dIter2C<ComplexC, ComplexC> it(datQuartile2, resQuartile1); it; it++ )
      it.Data2() = it.Data1();

    SArray2dC<ComplexC> datQuartile1(const_cast<SArray2dC<ComplexC> &>(dat), datQuartile1Range);
    SArray2dC<ComplexC> resQuartile2(shifted, resQuartile2Range);
    for( SArray2dIter2C<ComplexC, ComplexC> it(datQuartile1, resQuartile2); it; it++ )
      it.Data2() = it.Data1();

    return shifted;
  }


  //: FFTShift image of reals
  SArray2dC<RealT> FFT2dC::FFTShift(const SArray2dC<RealT> &dat)
  {
    //: Quartile numbering: 0 1
    //:                     2 3
    //: Reordering:         0 1   --->  3 2
    //:                     2 3         1 0
    //: Care is required for odd sizes: 0 1 2 ---> 5 3 4
    //:                                 3 4 5      2 0 1

    //: Input quartiles
    //=================
    Index2dC datQuartile3TopLeft( dat.Size1() / 2, dat.Size2() / 2 );
    if( dat.Size1() & true ) datQuartile3TopLeft.Row()++;
    if( dat.Size2() & true ) datQuartile3TopLeft.Col()++;

    IndexRange2dC datQuartile0Range( 0, datQuartile3TopLeft.Row() - 1,
                                     0, datQuartile3TopLeft.Col() - 1 );
    IndexRange2dC datQuartile1Range( 0, datQuartile3TopLeft.Row() - 1,
                                     datQuartile3TopLeft.Col(), dat.Size2() - 1 );
    IndexRange2dC datQuartile2Range( datQuartile3TopLeft.Row(), dat.Size1() - 1,
                                     0, datQuartile3TopLeft.Col() - 1 );
    IndexRange2dC datQuartile3Range( datQuartile3TopLeft.Row(), dat.Size1() - 1,
                                     datQuartile3TopLeft.Col(), dat.Size2() - 1 );

    //: Output quartiles
    //==================
    IndexRange2dC resQuartile0Range = datQuartile3Range;
    IndexRange2dC resQuartile1Range = datQuartile2Range;
    IndexRange2dC resQuartile2Range = datQuartile1Range;
    IndexRange2dC resQuartile3Range = datQuartile0Range;

    Index2dC resQuartile0Origin( 0              , 0 );
    Index2dC resQuartile1Origin( 0              , dat.Size2() / 2 );
    Index2dC resQuartile2Origin( dat.Size1() / 2, 0 );
    Index2dC resQuartile3Origin( dat.Size1() / 2, dat.Size2() / 2 );

    resQuartile0Range.SetOrigin(resQuartile0Origin);
    resQuartile1Range.SetOrigin(resQuartile1Origin);
    resQuartile2Range.SetOrigin(resQuartile2Origin);
    resQuartile3Range.SetOrigin(resQuartile3Origin);

    //: Copy each quartile of dat to shifted
    //======================================
    SArray2dC<RealT> shifted(dat.Size1(), dat.Size2());

    SArray2dC<RealT> datQuartile3(const_cast<SArray2dC<RealT> &>(dat), datQuartile3Range);
    SArray2dC<RealT> resQuartile0(shifted, resQuartile0Range);
    for( SArray2dIter2C<RealT, RealT> it(datQuartile3, resQuartile0); it; it++ )
      it.Data2() = it.Data1();

    SArray2dC<RealT> datQuartile0(const_cast<SArray2dC<RealT> &>(dat), datQuartile0Range);
    SArray2dC<RealT> resQuartile3(shifted, resQuartile3Range);
    for( SArray2dIter2C<RealT, RealT> it(datQuartile0, resQuartile3); it; it++ )
      it.Data2() = it.Data1();

    SArray2dC<RealT> datQuartile2(const_cast<SArray2dC<RealT> &>(dat), datQuartile2Range);
    SArray2dC<RealT> resQuartile1(shifted, resQuartile1Range);
    for( SArray2dIter2C<RealT, RealT> it(datQuartile2, resQuartile1); it; it++ )
      it.Data2() = it.Data1();

    SArray2dC<RealT> datQuartile1(const_cast<SArray2dC<RealT> &>(dat), datQuartile1Range);
    SArray2dC<RealT> resQuartile2(shifted, resQuartile2Range);
    for( SArray2dIter2C<RealT, RealT> it(datQuartile1, resQuartile2); it; it++ )
      it.Data2() = it.Data1();

    return shifted;
  }

  //: FFTShift image.
  Array2dC<ComplexC> FFT2dC::FFTShift(const Array2dC<ComplexC> &dat)
  {
    SArray2dC<ComplexC> shiftedSArray = FFTShift( const_cast<Array2dC<ComplexC> &>(dat).SArray2d(true) );

    Array2dC<ComplexC> shifted(shiftedSArray);

    // Translate shifted to dat position
    shifted.ShiftArray(dat.Frame().Origin());

    return shifted;
  }

  //: FFTShift image.
  Array2dC<RealT> FFT2dC::FFTShift(const Array2dC<RealT> &dat)
  {
    SArray2dC<RealT> shiftedSArray = FFTShift( const_cast<Array2dC<RealT> &>(dat).SArray2d(true) );

    Array2dC<RealT> shifted(shiftedSArray);

    // Translate shifted to dat position
    shifted.ShiftArray(dat.Frame().Origin());

    return shifted;
  }


  //: Returns real part of complex image
  SArray2dC<RealT> FFT2dC::Real(const SArray2dC<ComplexC> &dat)
  {
    SArray2dC<RealT> out(dat.Size1(),dat.Size2());
    for (SArray2dIter2C<RealT,ComplexC> i(out,dat); i; ++i) i.Data1() = i.Data2().Re();
    return out;
  }


  //: Returns imaginary part of complex image
  SArray2dC<RealT> FFT2dC::Imag(const SArray2dC<ComplexC> &dat)
  {
    SArray2dC<RealT> out(dat.Size1(),dat.Size2());
    for (SArray2dIter2C<RealT,ComplexC> i(out,dat); i; ++i) i.Data1() = i.Data2().Im();
    return out;
  }


  //: Returns modulus of complex image
  SArray2dC<RealT> FFT2dC::Mag(const SArray2dC<ComplexC> &dat)
  {
    SArray2dC<RealT> out(dat.Size1(),dat.Size2());
    for (SArray2dIter2C<RealT,ComplexC> i(out,dat); i; ++i) i.Data1() = i.Data2().Mag();
    return out;
  }


  //: Returns real part of complex image
  Array2dC<RealT> FFT2dC::Real(const Array2dC<ComplexC> &dat)
  {
    Array2dC<RealT> out(dat.Frame());
    for (Array2dIter2C<RealT,ComplexC> i(out,dat); i; ++i) i.Data1() = i.Data2().Re();
    return out;
  }


  //: Returns imaginary part of complex image
  Array2dC<RealT> FFT2dC::Imag(const Array2dC<ComplexC> &dat)
  {
    Array2dC<RealT> out(dat.Frame());
    for (Array2dIter2C<RealT,ComplexC> i(out,dat); i; ++i) i.Data1() = i.Data2().Im();
    return out;
  }


  //: Returns imaginary part of complex image
  Array2dC<RealT> FFT2dC::Mag(const Array2dC<ComplexC> &dat)
  {
    Array2dC<RealT> out(dat.Frame());
    for (Array2dIter2C<RealT,ComplexC> i(out,dat); i; ++i) i.Data1() = i.Data2().Mag();
    return out;
  }


}

