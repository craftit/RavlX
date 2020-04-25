// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_INTELFFT2d_HEADER
#define RAVL_INTELFFT2d_HEADER 1
////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Math.Signals.2D"
//! lib=RavlIntelMKL
//! file="Ravl/Contrib/IntelMKL/IntelFFT2d.hh"

#include "Ravl/RefCounter.hh"
#include "Ravl/Complex.hh"
#include "Ravl/SArray2d.hh"
#include "Ravl/Array2d.hh"
#include "Ravl/FFT1d.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Intel optimised 2d FFT
  
  class IntelFFT2dBodyC
    : public RCBodyC
  {
  public:
    IntelFFT2dBodyC(Index2dC size,bool iinv);
    //: Constructor.
    
    ~IntelFFT2dBodyC();
    //: Destructor
    
    bool Init(Index2dC size,bool iinv);
    //: Create a plan with the given setup.
    
    SArray2dC<ComplexC> Apply(const SArray2dC<ComplexC> &dat);
    //: Apply transform to array.
    // Note, only the first 'n' byte of dat are proccessed.
    // if the array is shorter than the given length, an
    // exception 'ErrorOutOfRangeC' will be thrown.
    
    SArray2dC<ComplexC> Apply(const SArray2dC<RealT> &dat);
    //: Apply transform to real array 
    // Note, only the first 'n' byte of dat are proccessed.
    // if the array is shorter than the given length, an
    // exception 'ErrorOutOfRangeC' will be thrown.
    
    Index2dC Size() const
    { return size; }
    //: The size of the transform.
    
  protected:
    Index2dC size;  // Size of the transform.
    bool inv;      // Is the transform backward ??
    void *dftiComplex;
    void *dftiReal;
  };
  
  //! userlevel=Normal
  //: Intel optimised 2d FFT.
  
  class IntelFFT2dC
    : public RCHandleC<IntelFFT2dBodyC>
  {
  public:
    IntelFFT2dC()
      {}
    //: Default constructor.
    
    IntelFFT2dC(Index2dC size,bool iinv = false)
      : RCHandleC<IntelFFT2dBodyC>(*new IntelFFT2dBodyC(size,iinv))
      {}
    //: Create a fft class.
    // If iinv is true do an inverse transform

    IntelFFT2dC(SizeT s1,SizeT s2,bool iinv = false)
      : RCHandleC<IntelFFT2dBodyC>(*new IntelFFT2dBodyC(Index2dC(s1,s2),iinv))
      {}
    //: Create a fft class.
    // If iinv is true do an inverse transform
    
    bool Init(Index2dC size,bool iinv = false)
    { return Body().Init(size,iinv); }
    //: Create a plan with the given setup.
    
    SArray2dC<ComplexC> Apply(const SArray2dC<ComplexC> &dat)
    { return Body().Apply(dat); }
    //: Apply transform to array.
    // Note, only the first 'n' byte of dat are proccessed.
    // if the array is shorter than the given length, an
    // exception 'ErrorOutOfRangeC' will be thrown.
    
    SArray2dC<ComplexC> Apply(const SArray2dC<RealT> &dat)
    { return Body().Apply(dat); }
    //: Apply transform to real array 
    // Note, only the first 'n' byte of dat are proccessed.
    // if the array is shorter than the given length, an
    // exception 'ErrorOutOfRangeC' will be thrown.
    
    Array2dC<ComplexC> Apply(const Array2dC<ComplexC> &dat)
    { return Array2dC<ComplexC>(Body().Apply(const_cast<Array2dC<ComplexC> &>(dat).SArray2d(true))); }
    //: Apply transform to array.
    // Note, only the first 'n' byte of dat are proccessed.
    // if the array is shorter than the given length, an
    // exception 'ErrorOutOfRangeC' will be thrown. <p>
    // Note: The output will always have an origin of 0,0.
    
    Array2dC<ComplexC> Apply(const Array2dC<RealT> &dat)
    { return Array2dC<ComplexC>(Body().Apply(const_cast<Array2dC<RealT> &>(dat).SArray2d(true))); }
    //: Apply transform to real array 
    // Note, only the first 'n' byte of dat are proccessed.
    // if the array is shorter than the given length, an
    // exception 'ErrorOutOfRangeC' will be thrown. <p>
    // Note: The output will always have an origin of 0,0.
    
    Index2dC Size() const
    { return Body().Size(); }
    //: The size of the transform.
    
    static SArray2dC<ComplexC> FFTShift(const SArray2dC<ComplexC> &dat);
    //: FFTShift image.
    // DC moved from top left to centre or centre to top left. 
    // FFTShift(FFTShift(a)) == a; for even sized a, not quite for odd sized a.

    static SArray2dC<RealT> FFTShift(const SArray2dC<RealT> &dat);
    //: FFTShift image.
    // DC moved from top left to centre or centre to top left. 
    // FFTShift(FFTShift(a)) == a; for even sized a, not quite for odd sized a.
    
    static Array2dC<ComplexC> FFTShift(const Array2dC<ComplexC> &dat);
    //: FFTShift image.
    // DC moved from top left to centre or centre to top left. 
    // FFTShift(FFTShift(a)) == a; for even sized a, not quite for odd sized a.

    static Array2dC<RealT> FFTShift(const Array2dC<RealT> &dat);
    //: FFTShift image.
    // DC moved from top left to centre or centre to top left. 
    // FFTShift(FFTShift(a)) == a; for even sized a, not quite for odd sized a.
    
  };

}

#endif
