// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_REALHISTOGRAM1D_HEADER
#define RAVL_REALHISTOGRAM1D_HEADER 1
/////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! author="Charles Galambos"
//! docentry="Ravl.API.Math.Statistics.Histogram"
//! lib=RavlMath
//! file="Ravl/Math/Statistics/Histogram/RealHistogram1d.hh"

#include "Ravl/StdMath.hh"
#include "Ravl/IntC.hh"
#include "Ravl/Array1d.hh"

namespace RavlN {
  class MeanVarianceC;
  template<class DataT> class DListC;
  
  //! userlevel=Normal
  //: Create a histogram from real values.
  
  class RealHistogram1dC
    : public SArray1dC<UIntC>
  {
  public:
    RealHistogram1dC()
      : scale(1.0),
        offset(0)
    {}
    //: Default constructor.

    RealHistogram1dC(RealT min,RealT max,UIntT noOfBins);
    //: Create a histogram.
    // The bin width is therefore (max-min) / noOfBins.
   
    RealHistogram1dC(RealT nscale,RealT noffset,const SArray1dC<UIntC> &array)
      : SArray1dC<UIntC>(array),
	scale(nscale),
	offset(noffset)
    {}
    //: Create a histogram from a bin width, an offset and an existing  SArray1dC of binned values.

    RealT Scale() const
    { return scale; } 
    //: Scaling - i.e. bin width.
    
    RealT Offset() const
    { return offset; }
    //: Offset used in table.
    
    IndexC Bin(RealT v) const
    { return IndexC((RealT) (v-offset)/scale); }
    //: Get the bin which value 'v' falls into.
    
    RealT MidBin(IndexC bin) const
    { return (RealT) (((RealT) bin + 0.5)* scale) + offset; }
    //: Get the middle of given bin.
    
    RealT MinBin(IndexC bin) const
    { return (RealT) (((RealT) bin)* scale) + offset; }
    //: Get the lower limit of given bin.
    
    RealT MaxBin(IndexC bin) const
    { return (RealT) (((RealT) bin+1)* scale) + offset; }
    //: Get the upper limit of given bin.
    
    RealT MinLimit() const
    { return (RealT) IMin().V() * scale + offset; }
    //: Lower limit on values in the histogram range.

    RealT MaxLimit() const
    { return (RealT) (IMax().V()+1) * scale + offset; }
    //: Lower limit on values in the histogram range.
    
    void Reset()
    { Fill(0); }
    //: Reset counters in histogram to zero.
    
    void Vote(RealT v)
    { (*this)[Bin(v)]++; }
    //: Vote for value.
    // Note, this will not check that the value is within the histogram.
    // In check mode this will cause an error, in optimised it will corrupt
    // memory.
    
    void Vote(RealT v,IntT n)
    { (*this)[Bin(v)] += n; }
    //: Vote for value n times.
    // Note, this will not check that the value is within the histogram.
    // In check mode this will cause an error, in optimised it will corrupt
    // memory.
    
    bool CheckVote(RealT v) { 
      UIntT b = (UIntT) Bin(v).V();
      if(b >= Size())
	return false;
      (*this)[b]++; 
      return true;
    }
    //: Vote for value.
    // Returns false if value is out of range.
    
    bool CheckVote(RealT v,IntT n) { 
      UIntT b = (UIntT) Bin(v).V();
      if(b >= Size())
	return false;
      (*this)[b] += n; 
      return true;
    }
    //: Vote for value n times.
    // Returns false if value is out of range.

    bool ArrayVote(const Array1dC<RealT> &data);
    //: Add to histogram bins using "data"
    // Returns false if any values from "data" are out of range.
    
    UIntT TotalVotes() const;
    //: Find the total number of votes cast.
    // This is computed not stored, and so is relatively slow.
    
    RealT Information() const;
    //: Calculate the amount of information represented by the histogram.
    // This is also known as the entropy of the histogram.
    
    RealT Energy() const;
    //: Calculate the energy represented by the original signal.

    MeanVarianceC MeanVariance() const;
    //: Calculate the mean and variance for the signal.
    
    RealT SmoothedPDF(IntT bin,RealT sigma = 1) const;
    //: Evaluate histogram as a smoothed pdf.
    
    DListC<RealT> Peaks(UIntT width,UIntT threshold = 0) const;
    //: Find a list of peaks in the histogram.
    // The peaks are bigger than 'threshold' and larger than all those within +/- width.
    
    bool MinMax(IndexC &min,IndexC &max) const;
    //: Find the minimum and maximum bins with votes in.
    // Returns false if the histogram is empty, true otherwise.
    
    UIntT Sum(IndexC min,IndexC max) const;
    //: Sum votes in the bins from min to max inclusive.
    
  protected:
    RealT scale; // Scale factor.
    RealT offset;   // Offset.
  };
  
  ostream &operator<<(ostream &strm,const RealHistogram1dC &hist);
  //: Write to stream.
  
  istream &operator>>(istream &strm,RealHistogram1dC &hist);
  //: Read from stream.

  BinOStreamC &operator<<(BinOStreamC & s,const RealHistogram1dC &hist);
  //: Binary stream IO.
  
  BinIStreamC &operator>>(BinIStreamC & s,RealHistogram1dC &hist);
  //: Binary stream IO.
  
}
#endif
