// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Statistics/Histogram/RealHistogram1d.cc"

#include "Ravl/RealHistogram1d.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/SArray1dIterR.hh"
#include "Ravl/Array1dIter.hh"
#include "Ravl/DList.hh"

namespace RavlN {
  
  //: Create a histogram.
  
  RealHistogram1dC::RealHistogram1dC(RealT min,RealT max,UIntT noOfBins) 
    : SArray1dC<UIntC>(noOfBins)
  {
    scale = (max - min) / ((RealT) noOfBins - 1e-8);
    offset = min;
    Reset();
  }
  

  //: Add to histogram bins using "data"
  
  bool RealHistogram1dC::ArrayVote(const Array1dC<RealT> &data) {
    bool inrange(true);
    for (Array1dIterC<RealT> i(data); i; ++i) inrange &= CheckVote(*i);
    return inrange;
  }

  //: Find the total number of votes cast.
  
  UIntT RealHistogram1dC::TotalVotes() const {
    UIntT c = 0;
    for(SArray1dIterC<UIntC> it(*this);it;it++) 
      c += *it;
    return c;
  }
  
  //: Calculate the amount of information represented by the histogram.
  // This is also known as the entropy of the histogram.
  
  RealT RealHistogram1dC::Information() const {
    RealT totalp = 0;
    RealT total = TotalVotes();
    for(SArray1dIterC<UIntC> it(*this);it;it++) {
      RealT prob = (RealT) *it / total;
      if(prob>0) totalp += -prob * Log2(prob);  // cant take log of number which is negative or zero
    }
    return totalp;
  }
  
  //: Calculate the energy represented by the original signal.
  
  RealT RealHistogram1dC::Energy() const {
    UIntT total = TotalVotes();
    RealT sum = 0;
    for(SArray1dIterC<UIntC> it(*this);it;it++)
      sum += Pow((RealT) *it / total,2);
    return sum;
  }

  //: Evaluate histogram as a smoothed pdf.
  
  RealT RealHistogram1dC::SmoothedPDF(IntT bin,RealT sigma) const {
    RealT smoothedMeasure = 0.0;
    for(SArray1dIterC<UIntC> it(*this);it;it++) {
      RealT arg = (RealT) (it.Index() - bin) /( sigma * scale);
      smoothedMeasure += (RealT) *it * Exp(-RavlConstN::pi * Sqr(arg));
    }
    return smoothedMeasure / (TotalVotes() * sigma * scale);
  }

  //: Find a list of peaks in the histogram.
  // The peaks are bigger than 'threshold' and larger than all those within +/- width.
  
  DListC<RealT> RealHistogram1dC::Peaks(UIntT width,UIntT threshold) const {
    DListC<RealT> ret;    
    for(SArray1dIterC<UIntC> it(*this);it;it++) {
      UIntT max = *it;
      if(max < threshold)
	continue;
      IndexC at = it.Index();
      IndexRangeC rng(at - (int) width,at + (int) width);
      rng.ClipBy(Range());
      BufferAccessIterC<UIntC> sit(*this,rng);
      for(;sit;sit++)
	if(*sit >= max && (&(*it)) != (&(*sit))) break;
      if(!sit) // Found a peak ?
	ret.InsLast(MidBin(at));
    }
    return ret;
  }

  //: Find the minimum and maximum bins with votes in.
  // Returns false if the histogram is empty, true otherwise.
  
  bool RealHistogram1dC::MinMax(IndexC &min,IndexC &max) const {
    min = -1;
    max = -1;
    for(SArray1dIterC<UIntC> it(*this);it;it++) {
      if(*it != 0) {
	min = it.Index();
	break;
      }
    }
    if(min == -1)
      return false;
    for(SArray1dIterRC<UIntC> rit(*this);rit;rit--) {
      if(*rit != 0) {
	max=rit.Index();
	break;
      }
    }
    return true;
  }
  
  //: Sum votes in the bins from min to max.
  
  UIntT RealHistogram1dC::Sum(IndexC min,IndexC max) const {
    UIntT ret = 0;
    if(min > max)
      return ret;
    for(SArray1dIterC<UIntC> it(SArray1dC<UIntC>(*this).From(min.V(),(max - min).V()+1));it;it++)
      ret += *it;
    return ret;
  }

  
  ostream &operator<<(ostream &strm,const RealHistogram1dC &hist) {
    strm << hist.Offset() << ' ' << hist.Scale() << ' ' << (const SArray1dC<UIntC> &)(hist);
    return strm;
  }
  
  istream &operator>>(istream &strm,RealHistogram1dC &hist) {
    RealT offset,scale;
    SArray1dC<UIntC> xhist;
    strm >> offset >> scale >> xhist;
    hist = RealHistogram1dC(scale,offset,xhist);
    return strm;
  }

}
