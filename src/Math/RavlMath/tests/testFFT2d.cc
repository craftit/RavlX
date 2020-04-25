// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Signals/2D/testFFT2d.cc"

#include "Ravl/Complex.hh"
#include "Ravl/Stream.hh"
#include "Ravl/FFT2d.hh"
#include "Ravl/SArray2dIter.hh"
#include "Ravl/SArray2dIter2.hh"
#include "Ravl/SArray2dIter3.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/StrStream.hh"
#include "Ravl/OS/Date.hh"

using namespace RavlN;

int testFFT2d();
int testFFT2dPwr2();

int testRealFFT2d();
int testRealFFT2dPwr2();

int testFFTShift();
int testRealFFTShift();

int testSpeed();

int main()
{
  int ln;
  if((ln = testFFT2d()) != 0) {
    cerr << "Error line " << ln << "\n";
    return 1;
  }
  if((ln = testFFT2dPwr2()) != 0) {
    cerr << "Error line " << ln << "\n";
    return 1;
  }
  if((ln = testRealFFT2d()) != 0) {
    cerr << "Error line " << ln << "\n";
    return 1;
  }
  if((ln = testRealFFT2dPwr2()) != 0) {
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
  if((ln = testSpeed()) != 0) {
    cerr << "Error line " << ln << "\n";
    return 1;
  }
  cout << "Test passed. \n";
  return 0;
}

int testFFT2d() {
  cerr << "testFFT2d(), Started. \n";

  //: Check x - ifft(fft(x)) = 0
  //============================
  SArray2dC<ComplexC> indat(30,20);
  int i = 0;
  for(SArray2dIterC<ComplexC> it(indat);it;it++)
    *it = ComplexC(i++,0);

  FFT2dC fftf(indat.Size1(),indat.Size2(),false); // create forward transform.
  SArray2dC<ComplexC> fres = fftf.Apply(indat);

  //cerr << fres << "\n";

  FFT2dC ffti(indat.Size1(),indat.Size2(),true);// create reverse transform.
  SArray2dC<ComplexC> ires = ffti.Apply(fres);

  //cerr << ires << "\n";

  for(SArray2dIter2C<ComplexC,ComplexC> rit(indat,ires);rit;rit++) {
    if(Abs(rit.Data1().Re() - rit.Data2().Re()) > 0.000001)
      return __LINE__;
    if(Abs(rit.Data1().Im() - rit.Data2().Im()) > 0.000001)
      return __LINE__;
  }

  //: Check forward FFT is as expected for known values
  //===================================================
  SArray2dC<ComplexC> a(2,3);
  RealT increment = 0;
  for(SArray2dIterC<ComplexC> it(a); it; it++) {
    RealT v1 = increment+=0.5;
    RealT v2 = increment+=0.5;
    *it = ComplexC(v1,v2);
  }

  //cout << "a: " << a << endl;

  FFT2dC fftf2nd(a.Size1(), a.Size2(), false); // create forward transform.
  SArray2dC<ComplexC> A = fftf2nd.Apply(a);

  //cout << "A: " << A << endl;
  A *= (size_t)(a.Size1() * a.Size2());
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
    if(Abs(rit.Data1().Re() - rit.Data2().Re()) > 0.000001) {
      cerr << "Diff=" << Abs(rit.Data1().Re() - rit.Data2().Re()) << "\n";
      return __LINE__;
    }
    if(Abs(rit.Data1().Im() - rit.Data2().Im()) > 0.000001)
      return __LINE__;
  }

  //: Check inverse FFT is as expected for known values
  //===================================================
  A /= (size_t)(a.Size1() * a.Size2());
  //cout << "A for ifft to get return_a: " << A << endl;

  FFT2dC ffti2nd(A.Size1(), A.Size2(), true); // create inverse transform.
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

int testFFT2dPwr2() {
  cerr << "testFFT2dPwr2(), Started. \n";
  SArray2dC<ComplexC> indat(64,32);
  int i = 0;
  for(SArray2dIterC<ComplexC> it(indat);it;it++)
    *it = ComplexC(i++,0);

  FFT2dC fftf(indat.Size1(),indat.Size2(),false); // create forward transform.
  SArray2dC<ComplexC> fres = fftf.Apply(indat);

  //cerr << fres << "\n";

  FFT2dC ffti(indat.Size1(),indat.Size2(),true);// create reverse transform.
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



int testRealFFT2d() {
  cerr << "testRealFFT2d(), Started. \n";
  SArray2dC<RealT> indat(30,20);
  int i = 0;
  for(SArray2dIterC<RealT> it(indat);it;it++)
    *it = (RealT) i++;

  FFT2dC fftf(indat.Size1(),indat.Size2(),false); // create forward transform.
  SArray2dC<ComplexC> fres = fftf.Apply(indat);

  //cerr << fres << "\n";

  FFT2dC ffti(indat.Size1(),indat.Size2(),true);// create reverse transform.
  SArray2dC<ComplexC> ires = ffti.Apply(fres);

  //cerr << ires << "\n";

  SArray2dC<RealT> rres = ffti.Real(ires);


  for(SArray2dIter3C<RealT,ComplexC,RealT> rit(indat,ires,rres);rit;rit++) {
    if(Abs(rit.Data1() - rit.Data3()) > 0.000001)
      return __LINE__;
    if(Abs(rit.Data2().Im()) > 0.000001)
      return __LINE__;
  }
  return 0;
}

int testRealFFT2dPwr2() {
  cerr << "testRealFFT2dPwr2(), Started \n";
  SArray2dC<RealT> indat(64,32);
  int i = 0;
  for(SArray2dIterC<RealT> it(indat);it;it++)
    *it = i++;

  FFT2dC fftf(indat.Size1(),indat.Size2(),false); // create forward transform.
  SArray2dC<ComplexC> fres = fftf.Apply(indat);

  //cerr << fres << "\n";

  FFT2dC ffti(indat.Size1(),indat.Size2(),true);// create reverse transform.
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

  // Using RotateFreq should work even for odd sizes
  Array2dC<int> in(-5,-1,-1,1);
  for (Array2dIterC<int> i(in); i; ++i) *i = (i.Index().Row()*i.Index().Col()).V();
  FFT2dC forw(in.Frame().Size()), inv(in.Frame().Size(),true);
  Array2dC<int> diff(inv.RotateFreq(forw.RotateFreq(in))-in);
  for (Array2dIterC<int> i(diff); i; ++i) {
    if (*i != 0)  return __LINE__;
  }

  // Check even size using: evenSize = fftshift(fftshift(evenSize))
  SArray2dC<ComplexC> evenSize(2,4);
  int i=0;
  for( SArray2dIterC<ComplexC> it(evenSize); it; it++ ) {
    IntT i1 = i++;
    IntT i2 = i++;
    *it = ComplexC(i1, i2);
  }

  SArray2dC<ComplexC>  sEvenSize = FFT2dC::FFTShift( evenSize);
  SArray2dC<ComplexC> ssEvenSize = FFT2dC::FFTShift(sEvenSize);

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

  SArray2dC<ComplexC> sOddSize = FFT2dC::FFTShift(oddSize);

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

  SArray2dC<RealT>  sEvenSize = FFT2dC::FFTShift( evenSize);
  SArray2dC<RealT> ssEvenSize = FFT2dC::FFTShift(sEvenSize);

  for(SArray2dIter2C<RealT,RealT> rit(evenSize,ssEvenSize);rit;rit++) {
    if(Abs(rit.Data1() - rit.Data2()) > 0.000001)
      return __LINE__;
  }

  // Fixed test for odd size
  SArray2dC<RealT> oddSize(3,5);
  i=0;
  for( SArray2dIterC<RealT> it(oddSize); it; it++ )
    *it = i++;

  SArray2dC<RealT> sOddSize = FFT2dC::FFTShift(oddSize);

  SArray2dC<RealT> correctSOddSize(3,5);
  StrIStreamC istr("3 5\n13 14 10 11 12\n3 4 0 1 2\n8 9 5 6 7");
  istr >> correctSOddSize;

  for(SArray2dIter2C<RealT,RealT> rit(sOddSize,correctSOddSize);rit;rit++) {
    if(Abs(rit.Data1() - rit.Data2()) > 0.000001)
      return __LINE__;
  }

  return 0;
}

int testSpeed()
{
  cerr << "testSpeed(), Started \n";

  SArray2dC<RealT> array(300, 300);
  int i = 0;
  for(SArray2dIterC<RealT> it(array); it; it++)
    *it = i++;

  FFT2dC fftf(array.Size1(), array.Size2(),false); // create forward transform.
  FFT2dC ffti(array.Size1(), array.Size2(),true);// create reverse transform.

  DateC startTime = DateC::NowLocal();
  for(int i = 0; i < 40; i++)
  {
    SArray2dC<ComplexC> fres = fftf.Apply(array);
    SArray2dC<ComplexC> ires = ffti.Apply(fres);
  }
  RealT time = (DateC::NowLocal().Double() - startTime.Double()) / 40.;
  cerr << "Time:" << time << endl;

  return 0;
}
