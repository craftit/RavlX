// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_REALHISTOGRAM2D_HEADER
#define RAVL_REALHISTOGRAM2D_HEADER 1
/////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! author="Charles Galambos, Bill Christmas"
//! docentry="Ravl.API.Math.Statistics.Histogram"
//! lib=RavlMath
//! file="Ravl/Math/Statistics/Histogram/RealHistogram2d.hh"
//! date="28/04/2002"

#include "Ravl/StdMath.hh"
#include "Ravl/IntC.hh"
#include "Ravl/SArray2d.hh"
#include "Ravl/Vector2d.hh"
#include "Ravl/Point2d.hh"

namespace RavlN {
  class MeanVarianceC;
  class MeanCovariance2dC;
  
  //! userlevel=Normal
  //: Create a histogram from pairs of real values
  // Point2dC is used to represent the pair of values
  
  class RealHistogram2dC
    : public SArray2dC<UIntT>
  {
  public:
    RealHistogram2dC()
    {}
    //: Default construct
    
    RealHistogram2dC(const Point2dC &min,const Point2dC &max,const Index2dC &steps);
    //: Create a histogram.
    
    RealHistogram2dC(const Vector2dC &nscale,const Point2dC &noffset,const SArray2dC<UIntT> &array,bool doReset = true)
      : SArray2dC<UIntT>(array),
	scale(nscale),
	offset(noffset)
    {
      if(doReset)
        Reset();
    }
    //: Create a histogram from an offset, scale and an existing SArray2dC of binned values.
    
    Vector2dC Scale() const
    { return scale; } 
    //: Scaling.
    
    Point2dC Offset() const
    { return offset; }
    //: Offset used in hash table.
    
    Index2dC Bin(const Point2dC &v) const
    { Point2dC pnt((v-offset)/scale); return Index2dC((IntT) pnt[0],(IntT) pnt[1]); }
    //: Get the bin which value 'v' falls into.
    
    Point2dC MidBin(const Index2dC &bin) const
    { return ((Point2dC(bin) + Vector2dC(0.5,0.5)) * scale) + offset; }
    //: Get the middle of given bin.
    
    void Reset()
    { Fill(0); }
    //: Reset counters in histogram to zero.
    
    void Vote(const Point2dC &v)
    { (*this)[Bin(v)]++; }
    //: Vote for value.
    // Note, this will not check that the value is within the histogram.
    // In check mode this will cause an error, in optimised it will corrupt
    // memory.
    
    void Vote(const Point2dC &v, UIntT nvotes)
    { (*this)[Bin(v)]+=nvotes; }
    //: Vote "nvotes" times for value.
    // Note, this will not check that the value is within the histogram.
    // In check mode this will cause an error, in optimised it will corrupt
    // memory.
    
    bool CheckVote(const Point2dC &v, UIntT nvotes=1) { 
      Index2dC b =  Bin(v);
      if(!Contains(b))
	return false;
      (*this)[b]+=nvotes; 
      return true;
    }
    //: Vote "nvotes" times for value.
    // Returns false if value is out of range.
    
    UIntT TotalVotes() const;
    //: Find the total number of votes cast.
    // This is computed not stored, and so is relatively slow.
    
    RealT Information() const;
    //: Calculate the amount of information represented by the histogram.
    // This is also known as the entropy of the histogram.
    
    RealT Energy() const;
    //: Calculate the energy represented by the original signal.
    
    MeanCovariance2dC MeanVariance() const;
    //: Calculate the mean and variance for the signal.
    
    Point2dC Peak() const;
    //: Look for the position of the largest value in the accumulator.
    
  protected:
    Vector2dC scale; // Scale factor.
    Point2dC offset;   // Offset of 0,0 index.
  };

  ostream &operator<<(ostream &strm,const RealHistogram2dC &hist);
  //: Write to stream.
  
  istream &operator>>(istream &strm,RealHistogram2dC &hist);
  //: Read from stream.
  
  BinOStreamC &operator<<(BinOStreamC & s,const RealHistogram2dC &hist);
  //: Binary stream IO.
  
  BinIStreamC &operator>>(BinIStreamC & s,RealHistogram2dC &hist);
  //: Binary stream IO.
  
}


#endif
