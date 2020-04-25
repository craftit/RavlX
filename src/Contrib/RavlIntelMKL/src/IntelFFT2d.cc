// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlIntelMKL
//! file="Ravl/Contrib/IntelMKL/IntelFFT2d.cc"

#include "Ravl/IntelFFT2d.hh"
#include "Ravl/Exception.hh"
#include "Ravl/SArray2dIter2.hh"
#include "Ravl/SArray2dIter3.hh"
#include "Ravl/Slice1d.hh"
#include "Ravl/Assert.hh"

#include "mkl_blas.h"
#include "mkl_dfti.h"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

#define DFTI_ERROR_HANDLING 1
#if DFTI_ERROR_HANDLING
#define DFTI_ERRORCHECK(x) {long status = x; if(status != DFTI_NO_ERROR) RavlIssueError(DftiErrorMessage(status));}
#else
#define DFTI_ERRORCHECK(x) x
#endif

#define DFTI_HANDLE(x) ((DFTI_DESCRIPTOR_HANDLE &)(x))

namespace RavlN {
  
#if RAVL_COMPILER_MIPSPRO
  static Slice1dC<ComplexC*> fixSGIBug1;
  static Slice1dIter2C<ComplexC*,ComplexC> fixSGIBug2;
#endif
  
  //: Constructor.
  
  IntelFFT2dBodyC::IntelFFT2dBodyC(Index2dC nn,bool iinv) 
    : size(nn),
      inv(iinv),
      dftiComplex(0),
      dftiReal(0)
  { 
    Init(nn,iinv);
  }
  
  //: Destructor
  
  IntelFFT2dBodyC::~IntelFFT2dBodyC()  
  {
    if(dftiComplex != 0) {
      DFTI_ERRORCHECK(DftiFreeDescriptor(&DFTI_HANDLE(dftiComplex)));
    }
    if(dftiReal != 0) {
      DFTI_ERRORCHECK(DftiFreeDescriptor(&DFTI_HANDLE(dftiReal)));
    }
  }
  
  //: Create a plan with the given setup.
  
  bool IntelFFT2dBodyC::Init(Index2dC nsize,bool iinv) {
    //cerr << "IntelFFT2dBodyC::Init(), Called. Size=" << nsize << " \n";
    size = nsize;
    inv = iinv;
    long l[2];
    l[0] = nsize[0].V();
    l[1] = nsize[1].V();

    //cerr << "Setup complex fft. \n";
    // Create complex descriptor.
    DFTI_ERRORCHECK(DftiCreateDescriptor(&DFTI_HANDLE(dftiComplex),DFTI_DOUBLE,DFTI_COMPLEX,2,l));
    DFTI_ERRORCHECK(DftiSetValue(DFTI_HANDLE(dftiComplex),DFTI_PLACEMENT,DFTI_NOT_INPLACE));
    DFTI_ERRORCHECK(DftiSetValue(DFTI_HANDLE(dftiComplex),DFTI_FORWARD_SCALE,1/((RealT) l[0]*l[1])));
    DFTI_ERRORCHECK(DftiCommitDescriptor(DFTI_HANDLE(dftiComplex)));

#if 1
    //cerr << "Setup real fft. \n";
    // Create real descriptor.
    DFTI_ERRORCHECK(DftiCreateDescriptor(&DFTI_HANDLE(dftiReal),DFTI_DOUBLE,DFTI_COMPLEX,2,l));
    DFTI_ERRORCHECK(DftiSetValue(DFTI_HANDLE(dftiReal),DFTI_FORWARD_SCALE,1/((RealT) l[0]*l[1])));
    DFTI_ERRORCHECK(DftiCommitDescriptor(DFTI_HANDLE(dftiReal)));
#endif    
    //cerr << "Done.\n";

    // Clean up
    //MKL_FreeBuffers();

    return true;
  }
  
  //: Apply transform to array.
  
  SArray2dC<ComplexC> IntelFFT2dBodyC::Apply(const SArray2dC<ComplexC> &dat) {
    SArray2dC<ComplexC> ret(dat.Size1(),dat.Size2());
    // Check stride ?!?

    if(inv) {
      DFTI_ERRORCHECK(DftiComputeBackward(DFTI_HANDLE(const_cast<void *&>(dftiComplex)),const_cast<ComplexC *>(&(dat[0][0])),&(ret[0][0])));
    }
    else {
      DFTI_ERRORCHECK(DftiComputeForward(DFTI_HANDLE(const_cast<void *&>(dftiComplex)),const_cast<ComplexC *>(&(dat[0][0])),&(ret[0][0])));
    }
    return ret;
  }
  
  //: Apply transform to array.
  
  SArray2dC<ComplexC> IntelFFT2dBodyC::Apply(const SArray2dC<RealT> &dat) {
    //cerr << "IntelFFT2dBodyC::Apply(), dat.Size=" << dat.Frame() << " Size=" << size << "\n";
    SArray2dC<ComplexC> ret(dat.Size1(),dat.Size2());
    for(SArray2dIter2C<ComplexC,RealT> it(ret,dat);it;it++)
      it.Data1() = it.Data2();

    if(inv) {
      DFTI_ERRORCHECK(DftiComputeBackward(DFTI_HANDLE(const_cast<void *&>(dftiReal)),const_cast<ComplexC *>(&(ret[0][0]))));
    }
    else {
      DFTI_ERRORCHECK(DftiComputeForward(DFTI_HANDLE(const_cast<void *&>(dftiReal)),const_cast<ComplexC *>(&(ret[0][0]))));
    }
    return ret;    
  }
  
  
  //: FFTShift image of complex numbers
  SArray2dC<ComplexC> IntelFFT2dC::FFTShift(const SArray2dC<ComplexC> &dat) {
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
  SArray2dC<RealT> IntelFFT2dC::FFTShift(const SArray2dC<RealT> &dat)
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
  Array2dC<ComplexC> IntelFFT2dC::FFTShift(const Array2dC<ComplexC> &dat)
  {
    SArray2dC<ComplexC> shiftedSArray = FFTShift( const_cast<Array2dC<ComplexC> &>(dat).SArray2d(true) );

    Array2dC<ComplexC> shifted(shiftedSArray);

    // Translate shifted to dat position
    shifted.ShiftIndexes1(-dat.Range1().Min());
    shifted.ShiftIndexes2(-dat.Range2().Min());
  
    return shifted;
  }
  
  //: FFTShift image.
  Array2dC<RealT> IntelFFT2dC::FFTShift(const Array2dC<RealT> &dat)
  {
    SArray2dC<RealT> shiftedSArray = FFTShift( const_cast<Array2dC<RealT> &>(dat).SArray2d(true) );

    Array2dC<RealT> shifted(shiftedSArray);

    // Translate shifted to dat position
    shifted.ShiftIndexes1(-dat.Range1().Min());
    shifted.ShiftIndexes2(-dat.Range2().Min());
  
    return shifted;
  }
}
