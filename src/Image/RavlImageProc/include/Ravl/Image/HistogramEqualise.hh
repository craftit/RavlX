// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_HISTOGRAMEQUALISE_HEADER
#define RAVLIMAGE_HISTOGRAMEQUALISE_HEADER 1
//! userlevel=Normal
//! author="Kieron J Messer"
//! docentry="Ravl.API.Images.Scaling and Warping"
//! rcsid="$Id$"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Filters/Misc/HistogramEqualise.hh"

#include "Ravl/Image/Image.hh"
#include "Ravl/Array1d.hh"
#include "Ravl/Array1dIter.hh"
#include "Ravl/Array1dIter2.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/Array2dIter2.hh"

namespace RavlImageN {
  
  //! userlevel=Normal
  //: Histogram Equalisation.
  
  template<class DataT>
  class HistogramEqualiseC {
  public:
    HistogramEqualiseC(DataT nminValue = 0,DataT nmaxValue = 255,bool nrescale = false)
      : minValue(nminValue),
	maxValue(nmaxValue),
	rescale(nrescale)
    {}
    //: Constructor.
    // Takes the minimum and maximum values to be used in the
    // output.   If 'rescale' is true the input range will be scale
    // to the same be as the output before histogram equalisation is
    // done.
    
    ImageC<DataT> Apply (const ImageC<DataT> &in) const;
    //: Performs histogram equalisation on image 'in'.
    // Returns a new equalised image.
    
    static inline IntT Floor(RealT v) 
    { return RavlN::Floor(v); }
    //: Real valued floor
    
    static inline IntT Floor(ByteT v) 
    { return (IntT) v; }
    //: Real valued floor
    
    static inline IntT Floor(Int16T v) 
    { return (IntT) v; }
    //: Real valued floor
    
    static inline IntT Floor(IntT v) 
    { return v; }
    //: Real valued floor
    
    const DataT &MinValue() const
    { return minValue; }
    //: Get minimum of range.
    
    const DataT &MaxValue() const
    { return maxValue; }
    //: Get maximum of range.
    
    bool IsRescale() const
    { return rescale; }
    //: Are we rescaling the input ?
    
  protected:
    DataT minValue;
    DataT maxValue;
    bool rescale;
  };
  
  template<class DataT>
  std::ostream &operator<<(std::ostream &s,const HistogramEqualiseC<DataT> &hist) {
    int v = 0; // stream version no.
    s << v << ' ' << hist.MinValue() << ' ' << hist.MaxValue() << ' ' << ((int) hist.IsRescale());
    return s;
  }
  //: Write to a stream.
  
  template<class DataT>
  std::istream &operator>>(std::istream &s,HistogramEqualiseC<DataT> &hist) {
    int v;
    DataT min,max;
    int rescale;
    s >> v;
    if(v > 0)
      throw ExceptionOutOfRangeC("Bad version number in stream. ");
    s >> min >> max >> rescale;
    hist = HistogramEqualiseC<DataT>(min,max,rescale != 0);
    return s;
  }
  //: Read from a stream.
  
  template<class DataT>
  BinOStreamC &operator<<(BinOStreamC &s,const HistogramEqualiseC<DataT> &hist) {
    int v = 0; // stream version no.
    s << v << hist.MinValue() << hist.MaxValue() << ((int) hist.IsRescale());
    return s;
  }
  //: Write to a binary stream.
  
  template<class DataT>
  BinIStreamC &operator>>(BinIStreamC &s,HistogramEqualiseC<DataT> &hist) {
    int v;
    DataT min,max;
    int rescale;
    s >> v;
    if(v > 0)
      throw ExceptionOutOfRangeC("Bad version number in stream. ");
    s >> min >> max >> rescale;
    hist = HistogramEqualiseC<DataT>(min,max,rescale != 0);
    return s;
  }
  //: Read from a binary stream.
  
  //:-
  
  template<class DataT>
  ImageC<DataT> HistogramEqualiseC<DataT>::Apply (const ImageC<DataT> &in) const {
    // Build the histogram.
    if(in.IsEmpty())
      return ImageC<DataT>();
    RealT diff = (RealT)(maxValue - minValue);
    Array1dC<RealT> pr(Floor(minValue),Floor(maxValue));
    pr.Fill(0.0);
    RealT scale = 1;
    DataT imin = 0;
    if(rescale) {
      Array2dIterC<DataT> it(in);
      imin = *it;
      DataT imax = *it;
      for(it++;it;it++) {
	if(imin > *it)
	  imin = *it;
	else if(imax < *it)
	  imax = *it;
      }
      RealT idiff = (RealT) (imax - imin) + 1;
      scale = diff/idiff;
      //cerr << "imin=" << imin << " imax=" << imax << " scale=" << scale << "\n";
      for(Array2dIterC<DataT> it(in);it;it++)
	pr[Floor((*it - imin) * scale + minValue)]++;
    } else {
      for(Array2dIterC<DataT> it(in);it;it++)
	pr[Floor(*it)]++;
    }
    // Compute the cumalitve histogram, and use it to compute the
    // transform.
    
    RealT n = (RealT) in.Frame().Area();
    RealT sum = 0.0;
    Array1dC<DataT> tr(Floor(minValue),Floor(maxValue));
    for(Array1dIter2C<DataT,RealT> it(tr,pr);it;it++) {
      RealT val = it.Data2();
      it.Data1() = (DataT) ((RealT) (sum * diff) + minValue);
      sum += val / n;
    }
    
    // Transform the image.
    
    ImageC<DataT> op(in.Frame());
    if(rescale) {
      for(Array2dIter2C<DataT,DataT> it(in,op);it;it++)
	it.Data2() = tr[Floor((it.Data1() - imin) * scale + minValue)];
    } else {
      for(Array2dIter2C<DataT,DataT> it(in,op);it;it++)
	it.Data2() = tr[Floor(it.Data1())];
    }
    return op;
  }

}

#endif
