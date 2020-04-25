// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlIntelMKL
//! file="Ravl/Contrib/IntelMKL/testIntelFFT2d.cc"

#include "Ravl/IntelFFT2d.hh"
#include "Ravl/SArray2dIter.hh"
#include "Ravl/SArray2dIter2.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/StrStream.hh"

using namespace RavlN;

int testIntelFFT2d();
int testIntelFFT2dPwr2();

int testRealIntelFFT2d();
int testRealIntelFFT2dPwr2();

int testFFTShift();
int testRealFFTShift();

int main()
{
  int ln;
  if((ln = testIntelFFT2d()) != 0) {
    cerr << "Error line " << ln << "\n";
    return 1;
  }
  if((ln = testIntelFFT2dPwr2()) != 0) {
    cerr << "Error line " << ln << "\n";
    return 1;
  }
  if((ln = testRealIntelFFT2d()) != 0) {
    cerr << "Error line " << ln << "\n";
    return 1;
  }
  if((ln = testRealIntelFFT2dPwr2()) != 0) {
    cerr << "Error line " << ln << "\n";
    return 1;
  }
  if((ln = testFFTShift()) != 0) {
    cerr << "Error line " << ln << "\n";
    return 1;
  }
  if((ln = testRealFFTShift()) != 0) {
    cerr << "Error line " << ln << "\n";
    return 1;
  }
  cout << "Test passed. \n";
  return 0;
}

int testIntelFFT2d() {
  cerr << "testIntelFFT2d(), Started. \n";

  //: Check x - ifft(fft(x)) = 0
  //============================
  SArray2dC<ComplexC> indat(30,20);
  int i = 0;
  for(SArray2dIterC<ComplexC> it(indat);it;it++)
    *it = ComplexC(i++,0);
  
  IntelFFT2dC fftf(indat.Size1(),indat.Size2(),false); // create forward transform.
  SArray2dC<ComplexC> fres = fftf.Apply(indat);
  
  //cerr << fres << "\n";
  
  IntelFFT2dC ffti(indat.Size1(),indat.Size2(),true);// create reverse transform.
  SArray2dC<ComplexC> ires = ffti.Apply(fres);
  
  //cerr << ires << "\n";
  
  for(SArray2dIter2C<ComplexC,ComplexC> rit(indat,ires);rit;rit++) {
    if(Abs(rit.Data1().Re() - rit.Data2().Re()) > 0.000001)
      return __LINE__;
    if(Abs(rit.Data1().Im() - rit.Data2().Im()) > 0.000001)
      return __LINE__;
  }

  //: Check fft(x) is as expected for known values
  //==============================================
  SArray2dC<ComplexC> a(2,3);
  RealT increment = 0;
  for(SArray2dIterC<ComplexC> it(a); it; it++) {
    RealT i1 = increment+=0.5;
    RealT i2 = increment+=0.5;
    *it = ComplexC(i1,i2);
  }

  //cout << "a: " << a << endl;

  IntelFFT2dC fftf2nd(a.Size1(), a.Size2(), false); // create forward transform.
  SArray2dC<ComplexC> A = fftf2nd.Apply(a);

  //cout << "A: " << A << endl;
  A *= (RealT)(a.Size1() * a.Size2());
  //cout << "A after multiplying by number of elements: " << A << endl;

  // Values from MatLab v6
  SArray2dC<ComplexC> groundTruthA(2,3);
  groundTruthA[0][0] = ComplexC(18.0000, 21.0000);
  groundTruthA[0][1] = ComplexC(-4.73205080756888, -1.26794919243112);
  groundTruthA[0][2] = ComplexC(-1.26794919243112, -4.73205080756888);
  groundTruthA[1][0] = ComplexC(-9.0000, -9.0000);
  groundTruthA[1][1] = ComplexC( 0,       0);
  groundTruthA[1][2] = ComplexC( 0,       0);

  //cout << "groundTruthA: " << groundTruthA << endl;

  for(SArray2dIter2C<ComplexC,ComplexC> rit(A,groundTruthA);rit;rit++) {
    if(Abs(rit.Data1().Re() - rit.Data2().Re()) > 0.000001)
      return __LINE__;
    if(Abs(rit.Data1().Im() - rit.Data2().Im()) > 0.000001)
      return __LINE__;
  }

  //: Check inverse FFT is as expected for known values
  //===================================================
  A /= (RealT)(a.Size1() * a.Size2());
  //cout << "A for ifft to get return_a: " << A << endl;

  IntelFFT2dC ffti2nd(A.Size1(), A.Size2(), true); // create inverse transform.
  SArray2dC<ComplexC> return_a = ffti2nd.Apply(A);

  //cout << "return_a: " << return_a << endl;

  // Check by differencing a and return_a
  for(SArray2dIter2C<ComplexC,ComplexC> rit(a, return_a);rit;rit++) {
    if(Abs(rit.Data1().Re() - rit.Data2().Re()) > 0.000001)
      return __LINE__;
    if(Abs(rit.Data1().Im() - rit.Data2().Im()) > 0.000001)
      return __LINE__;
  }

  return 0;
}

int testIntelFFT2dPwr2() {
  cerr << "testIntelFFT2dPwr2(), Started. \n";
  SArray2dC<ComplexC> indat(64,32);
  int i = 0;
  for(SArray2dIterC<ComplexC> it(indat);it;it++)
    *it = ComplexC(i++,0);
  
  IntelFFT2dC fftf(indat.Size1(),indat.Size2(),false); // create forward transform.
  SArray2dC<ComplexC> fres = fftf.Apply(indat);
  
  //cerr << fres << "\n";
  
  IntelFFT2dC ffti(indat.Size1(),indat.Size2(),true);// create reverse transform.
  SArray2dC<ComplexC> ires = ffti.Apply(fres);
  
  //cerr << ires << "\n";
  
  // Check results.
  
  for(SArray2dIter2C<ComplexC,ComplexC> rit(indat,ires);rit;rit++) {
    if(Abs(rit.Data1().Re() - rit.Data2().Re()) > 0.000001)
      return __LINE__;
    if(Abs(rit.Data1().Im() - rit.Data2().Im()) > 0.000001)
      return __LINE__;
  }
  return 0;
}



int testRealIntelFFT2d() {
  cerr << "testRealIntelFFT2d(), Started. \n";
  SArray2dC<RealT> indat(30,20);
  int i = 0;
  for(SArray2dIterC<RealT> it(indat);it;it++)
    *it = (RealT) i++;
  
  cerr << "Forward. \n";
  IntelFFT2dC fftf(indat.Size1(),indat.Size2(),false); // create forward transform.
  SArray2dC<ComplexC> fres = fftf.Apply(indat);
  
  //cerr << fres << "\n";
  cerr << "Back.\n";
  
  IntelFFT2dC ffti(indat.Size1(),indat.Size2(),true);// create reverse transform.
  SArray2dC<ComplexC> ires = ffti.Apply(fres);
  
  //cerr << ires << "\n";
  cerr << "Check.\n";
  
  for(SArray2dIter2C<RealT,ComplexC> rit(indat,ires);rit;rit++) {
    if(Abs(rit.Data1() - rit.Data2().Re()) > 0.000001)
      return __LINE__;
    if(Abs(rit.Data2().Im()) > 0.000001)
      return __LINE__;
  }
  return 0;
}

int testRealIntelFFT2dPwr2() {
  cerr << "testRealIntelFFT2dPwr2(), Started \n";
  SArray2dC<RealT> indat(64,32);
  int i = 0;
  for(SArray2dIterC<RealT> it(indat);it;it++)
    *it = i++;
  
  IntelFFT2dC fftf(indat.Size1(),indat.Size2(),false); // create forward transform.
  SArray2dC<ComplexC> fres = fftf.Apply(indat);
  
  //cerr << fres << "\n";
  
  IntelFFT2dC ffti(indat.Size1(),indat.Size2(),true);// create reverse transform.
  SArray2dC<ComplexC> ires = ffti.Apply(fres);
  
  //cerr << ires << "\n";
  
  // Check results.
  
  for(SArray2dIter2C<RealT,ComplexC> rit(indat,ires);rit;rit++) {
    if(Abs(rit.Data1() - rit.Data2().Re()) > 0.000001)
      return __LINE__;
    if(Abs(rit.Data2().Im()) > 0.000001)
      return __LINE__;
  }
  return 0;
}



int testFFTShift() {
  cerr << "testFFTShift(), Started \n";

  // Check even size using: evenSize = fftshift(fftshift(evenSize))
  SArray2dC<ComplexC> evenSize(2,4);
  int i=0;
  for( SArray2dIterC<ComplexC> it(evenSize); it; it++ ) {
    IntT i1 = i++;
    IntT i2 = i++;
    *it = ComplexC(i1, i2);
  }

  SArray2dC<ComplexC>  sEvenSize = IntelFFT2dC::FFTShift( evenSize);
  SArray2dC<ComplexC> ssEvenSize = IntelFFT2dC::FFTShift(sEvenSize);

  for(SArray2dIter2C<ComplexC,ComplexC> rit(evenSize,ssEvenSize);rit;rit++) {
    if(Abs(rit.Data1().Re() - rit.Data2().Re()) > 0.000001)
      return __LINE__;
    if(Abs(rit.Data1().Im() - rit.Data2().Im()) > 0.000001)
      return __LINE__;
  }
 
  // Fixed test for odd size
  SArray2dC<ComplexC> oddSize(3,5);
  int c=0;
  for( SArray2dIterC<ComplexC> it(oddSize); it; it++ )
    {
      int i = c++ ; 
      int j = c++ ;  
      *it = ComplexC(i,j) ;
    }
  
  SArray2dC<ComplexC> sOddSize = IntelFFT2dC::FFTShift(oddSize);

  SArray2dC<ComplexC> correctSOddSize(3,5);
  StrIStreamC istr("3 5\n26 27 28 29 20 21 22 23 24 25\n6 7 8 9 0 1 2 3 4 5\n16 17 18 19 10 11 12 13 14 15");
  istr >> correctSOddSize;

  for(SArray2dIter2C<ComplexC,ComplexC> rit(sOddSize,correctSOddSize);rit;rit++) {
    if(Abs(rit.Data1().Re() - rit.Data2().Re()) > 0.000001)
      return __LINE__;
    if(Abs(rit.Data1().Im() - rit.Data2().Im()) > 0.000001)
      return __LINE__;
  }

  return 0;
}

int testRealFFTShift() {
  cerr << "testRealFFTShift(), Started \n";

  // Check even size using: evenSize = fftshift(fftshift(evenSize))
  SArray2dC<RealT> evenSize(128,200);
  int i=0;
  for( SArray2dIterC<RealT> it(evenSize); it; it++ )
    *it = i++;

  SArray2dC<RealT>  sEvenSize = IntelFFT2dC::FFTShift( evenSize);
  SArray2dC<RealT> ssEvenSize = IntelFFT2dC::FFTShift(sEvenSize);

  for(SArray2dIter2C<RealT,RealT> rit(evenSize,ssEvenSize);rit;rit++) {
    if(Abs(rit.Data1() - rit.Data2()) > 0.000001)
      return __LINE__;
  }

  // Fixed test for odd size
  SArray2dC<RealT> oddSize(3,5);
  i=0;
  for( SArray2dIterC<RealT> it(oddSize); it; it++ )
    *it = i++;
  
  SArray2dC<RealT> sOddSize = IntelFFT2dC::FFTShift(oddSize);

  SArray2dC<RealT> correctSOddSize(3,5);
  StrIStreamC istr("3 5\n13 14 10 11 12\n3 4 0 1 2\n8 9 5 6 7");
  istr >> correctSOddSize;

  for(SArray2dIter2C<RealT,RealT> rit(sOddSize,correctSOddSize);rit;rit++) {
    if(Abs(rit.Data1() - rit.Data2()) > 0.000001)
      return __LINE__;
  }

  return 0;
}
