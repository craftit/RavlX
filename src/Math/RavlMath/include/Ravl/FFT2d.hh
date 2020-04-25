// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FFT2D_HEADER
#define RAVL_FFT2D_HEADER 1
////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Math.Signals.2D;Ravl.API.Images.Transforms"
//! lib=RavlMath
//! file="Ravl/Math/Signals/2D/FFT2d.hh"
//! example ="exFFT2d.cc"

#include "Ravl/RefCounter.hh"
#include "Ravl/Complex.hh"
#include "Ravl/SArray2d.hh"
#include "Ravl/Array2d.hh"
#include "Ravl/FFT1d.hh"
#include "Ravl/Array2dIter2.hh"

namespace RavlN {

  //! userlevel=Develop
  //: 2d FFT

  class FFT2dBodyC
    : public RCBodyC
  {
  public:
    FFT2dBodyC(Index2dC size, bool iinv);
    //: Constructor.

    ~FFT2dBodyC();
    //: Destructor

    bool Init(Index2dC size, bool iinv);
    //: Create a plan with the given setup.

    SArray2dC<ComplexC> Apply(const SArray2dC<ComplexC> &dat) const;
    //: Apply transform to array.
    // Note, only the first 'n' byte of dat are proccessed.
    // if the array is shorter than the given length, an
    // exception 'ErrorOutOfRangeC' will be thrown.

    SArray2dC<ComplexC> Apply(const SArray2dC<RealT> &dat) const;
    //: Apply transform to real array
    // Note, only the first 'n' byte of dat are proccessed.
    // if the array is shorter than the given length, an
    // exception 'ErrorOutOfRangeC' will be thrown.

    Index2dC Size() const
      { return size; }
    //: The size of the transform.

    template <class DataT>
      Array2dC<DataT> RotateFreq(const Array2dC<DataT> &dat) const;
    //: Returns array in which elements of the array <code>dat</code> are rotated to move top left to centre and vice versa

  protected:
    Index2dC size;  // Size of the transform.
    IntT n,m;
    bool inv;      // Is the transform backward ??
    bool base2;    // Are both dimensions a power of 2?

    int primeFactors1[32];

    int primeFactors2[32];
  };

  //! userlevel=Normal
  //: 2d FFT.
  // Will assert if FFT row or column size has a prime factor > 101. This
  // appears to be a limitation of the CCMath prime factor function but not the
  // CCMath general radix FFT functions.

  class FFT2dC
    : public RCHandleC<FFT2dBodyC>
  {
  public:
    FFT2dC()
      {}
    //: Default constructor.

    FFT2dC(Index2dC size,bool iinv = false)
      : RCHandleC<FFT2dBodyC>(*new FFT2dBodyC(size,iinv))
      {}
    //: Create a fft class.
    // If iinv is true do an inverse transform

    FFT2dC(SizeT height,SizeT width,bool iinv = false)
      : RCHandleC<FFT2dBodyC>(*new FFT2dBodyC(Index2dC(height,width),iinv))
      {}
    //: Create a fft class.
    // If iinv is true do an inverse transform

    bool Init(Index2dC size,bool iinv = false)
    { return Body().Init(size,iinv); }
    //: Create a plan with the given setup.

    SArray2dC<ComplexC> Apply(const SArray2dC<ComplexC> &dat) const
    { return Body().Apply(dat); }
    //: Apply transform to array.
    // Note, only the first 'n' byte of dat are proccessed.
    // if the array is shorter than the given length, an
    // exception 'ErrorOutOfRangeC' will be thrown.

    SArray2dC<ComplexC> Apply(const SArray2dC<RealT> &dat) const
    { return Body().Apply(dat); }
    //: Apply transform to real array
    // Note, only the first 'n' byte of dat are proccessed.
    // if the array is shorter than the given length, an
    // exception 'ErrorOutOfRangeC' will be thrown.

    Array2dC<ComplexC> Apply(const Array2dC<ComplexC> &dat) const
    { return Array2dC<ComplexC>(Body().Apply(const_cast<Array2dC<ComplexC> &>(dat).SArray2d(true))); }
    //: Apply transform to array.
    // Note, only the first 'n' byte of dat are proccessed.
    // if the array is shorter than the given length, an
    // exception 'ErrorOutOfRangeC' will be thrown. <p>
    // Note: The output will always have an origin of 0,0.

    Array2dC<ComplexC> Apply(const Array2dC<RealT> &dat) const
    { return Array2dC<ComplexC>(Body().Apply(const_cast<Array2dC<RealT> &>(dat).SArray2d(true))); }
    //: Apply transform to real array
    // Note, only the first 'n' byte of dat are proccessed.
    // if the array is shorter than the given length, an
    // exception 'ErrorOutOfRangeC' will be thrown. <p>
    // Note: The output will always have an origin of 0,0.

    Index2dC Size() const
    { return Body().Size(); }
    //: The size of the transform.

    template <class DataT>
      Array2dC<DataT> RotateFreq(const Array2dC<DataT> &dat) const
      { return Body().RotateFreq(dat); }
    //: Returns array in which elements of the array <code>dat</code> are rotated to move top left to centre and vice versa
    // Used for 2D spectra to shift zero frequency component to centre etc.<br>
    // Coordinate system is also moved appropriately (regardless of coords of  "<code>dat</code>"):<br>
    // <ul><li>for forward FFT, origin is moved to centre;</li>
    // <li>for inverse FFT, origin is moved to top l.h.</li></ul>
    // Forward rotate followed by inverse works correctly even with odd-sized arrays.

    static SArray2dC<ComplexC> FFTShift(const SArray2dC<ComplexC> &dat);
    //: FFTShift image.
    // DC moved from top left to centre or centre to top left.
    // FFTShift(FFTShift(a)) == a for even sized a, not quite for odd sized a.
    //!deprecated: use non-static <code>RotateFreq()</code> instead (works correctly for odd-sized images)

    static SArray2dC<RealT> FFTShift(const SArray2dC<RealT> &dat);
    //: FFTShift image.
    // DC moved from top left to centre or centre to top left.
    // FFTShift(FFTShift(a)) == a for even sized a, not quite for odd sized a.
    //!deprecated: use non-static <code>RotateFreq()</code> instead (works correctly for odd-sized images)

    static Array2dC<ComplexC> FFTShift(const Array2dC<ComplexC> &dat);
    //: FFTShift image.
    // DC moved from top left to centre or centre to top left.
    // FFTShift(FFTShift(a)) == a for even sized a, not quite for odd sized a.
    //!deprecated: use non-static <code>RotateFreq()</code> instead (works correctly for odd-sized images)

    static Array2dC<RealT> FFTShift(const Array2dC<RealT> &dat);
    //: FFTShift image.
    // DC moved from top left to centre or centre to top left.
    // FFTShift(FFTShift(a)) == a for even sized a, not quite for odd sized a.
    //!deprecated: use non-static <code>RotateFreq()</code> instead (works correctly for odd-sized images)

    static  SArray2dC<RealT> Real(const SArray2dC<ComplexC> &dat);
    //: Returns real part of complex image

    static  SArray2dC<RealT> Imag(const SArray2dC<ComplexC> &dat);
    //: Returns imaginary part of complex image

    static  SArray2dC<RealT> Mag(const SArray2dC<ComplexC> &dat);
    //: Returns magnitude of complex image

    static  Array2dC<RealT> Real(const Array2dC<ComplexC> &dat);
    //: Returns real part of complex image

    static  Array2dC<RealT> Imag(const Array2dC<ComplexC> &dat);
    //: Returns imaginary part of complex image

    static  Array2dC<RealT> Mag(const Array2dC<ComplexC> &dat);
    //: Returns magnitude part of complex image


  };

  template <class DataT>
  Array2dC<DataT> FFT2dBodyC::RotateFreq(const Array2dC<DataT> &in) const {
    // Numbering of array quarters:    0 1
    //                                 2 3
    // Reordering:                     0 1  --->  3 2
    //                                 2 3        1 0
    //
    // Care is required for odd sizes: 0 1 2 ---> 5 3 4
    //                                 3 4 5      2 0 1
    // I.e. destination quarters are slightly different from source quarters.
    //
    // To do the inverse:              0 1 2 ---> 4 5 3
    //                                 3 4 5      1 2 0
    //
    // Hence 2 lots of "half", "quarter" to cater for the 2 different cases:
    // 1st lot for src, 2nd lot for destination.
    // If doing inverse (i.e. origin back to top lh), 2 lots are swapped.
    // (For even-sized arrays the 2 cases are identical.)
    //
    // "half" defines the boundary halfway across the array: it is actually
    // the bottom r.h. corner of the top l.h. quarter array (i.e. = quarter[i][0].BottomRight())
    SArray1dC<Index2dC> half(2);
    SArray2dC<IndexRange2dC> quarter(2,4); // array divided into quarters

    // Set up coords for 4 quarters of arrays
    const IndexRange2dC& f(in.Frame());
    for (IntT i(0); i<=1; ++i) { // i indexes over the 2 possibilities for odd=sized arrays
      // magic definition for "half" that works for even and odd sized arrays
      half[i] = (f.TopLeft()+f.BottomRight()+Index2dC(i-1,i-1))/2;
      quarter[i][0] = IndexRange2dC(f.TRow(),        half[i].Row(), f.LCol(),        half[i].Col()); // top left
      quarter[i][1] = IndexRange2dC(half[i].Row()+1, f.BRow(),      f.LCol(),        half[i].Col()); // top right
      quarter[i][2] = IndexRange2dC(f.TRow(),        half[i].Row(), half[i].Col()+1, f.RCol());      // bottom left
      quarter[i][3] = IndexRange2dC(half[i].Row()+1, f.BRow()     , half[i].Col()+1, f.RCol());      // bottom right
    }
    Array2dC<DataT> out(f);
    for (IntT q(0); q<=3; ++q){
      for (Array2dIter2C<DataT,DataT> p(Array2dC<DataT>(in,quarter[(int)inv][q]),
                                        Array2dC<DataT>(out,quarter[1-(int)inv][3-q])); p; ++p) {
        p.Data2() = p.Data1();
      }
    }
    out.ShiftArray(inv?-f.TopLeft():-(quarter[1-(int)inv][3].TopLeft()));
    return out;
  }
}

#endif
