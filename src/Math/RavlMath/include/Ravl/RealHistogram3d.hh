// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_REALHISTOGRAM3D_HEADER
#define RAVL_REALHISTOGRAM3D_HEADER 1

//! author="Peng Huang, Charles Galambos, Bill Christmas"
//! docentry="Ravl.API.Math.Statistics.Histogram"
//! lib=RavlMath


#include "Ravl/StdMath.hh"
#include "Ravl/IntC.hh"
#include "Ravl/SArray3d.hh"
#include "Ravl/Vector3d.hh"
#include "Ravl/Point3d.hh"

namespace RavlN {
  class MeanVarianceC;
  class MeanCovariance3dC;
  
  //! userlevel=Normal
  //: Create a histogram from triples of real values
  // Point3dC is used to represent the triple of values
  
  class RealHistogram3dC
    : public SArray3dC<UIntT>
  {
  public:
    RealHistogram3dC()
    {}
    //: Default construct
    
    RealHistogram3dC(const Point3dC &min,const Point3dC &max,const Index3dC &steps);
    //: Create a histogram.
    
    RealHistogram3dC(const Vector3dC &nscale,const Point3dC &noffset,const SArray3dC<UIntT> &array)
      : SArray3dC<UIntT>(array),
	scale(nscale),
	offset(noffset)
    { Reset(); }
    //: Create a histogram from an offset, scale and an existing SArray3dC of binned values.
    
    Vector3dC Scale() const
    { return scale; } 
    //: Scaling.
    
    Point3dC Offset() const
    { return offset; }
    //: Offset used in hash table.
    
    Index3dC Bin(const Point3dC &v) const
    { Point3dC pnt((v-offset)/scale); return Index3dC((IntT) pnt[0],(IntT) pnt[1],(IntT) pnt[2]); }
    //: Get the bin which value 'v' falls into.
    
    Point3dC MidBin(const Index3dC &bin) const
    { return ((Point3dC(bin) + Vector3dC(0.5,0.5,0.5)) * scale) + offset; }
    //: Get the middle of given bin.
    
    void Reset()
    { Fill(0); }
    //: Reset counters in histogram to zero.
    
    void Vote(const Point3dC &v)
    { (*this)[Bin(v)]++; }
    //: Vote for value.
    // Note, this will not check that the value is within the histogram.
    // In check mode this will cause an error, in optimised it will corrupt
    // memory.
    
    void Vote(const Point3dC &v, UIntT nvotes)
    { (*this)[Bin(v)]+=nvotes; }
    //: Vote "nvotes" times for value.
    // Note, this will not check that the value is within the histogram.
    // In check mode this will cause an error, in optimised it will corrupt
    // memory.
    
    bool CheckVote(const Point3dC &v, UIntT nvotes=1) { 
      Index3dC b =  Bin(v);
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
    
    MeanCovariance3dC MeanVariance() const;
    //: Calculate the mean and variance for the signal.
    
    Point3dC Peak() const;
    //: Look for the position of the largest value in the accumulator.
    
  protected:
    Vector3dC scale; // Scale factor.
    Point3dC offset;   // Offset of 0,0,0 index.
  };

  ostream &operator<<(ostream &strm,const RealHistogram3dC &hist);
  //: Write to stream.
  
  istream &operator>>(istream &strm,RealHistogram3dC &hist);
  //: Read from stream.
  
  BinOStreamC &operator<<(BinOStreamC & s,const RealHistogram3dC &hist);
  //: Binary stream IO.
  
  BinIStreamC &operator>>(BinIStreamC & s,RealHistogram3dC &hist);
  //: Binary stream IO.
  
}


#endif
