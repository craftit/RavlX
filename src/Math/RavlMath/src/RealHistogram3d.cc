// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#include "Ravl/RealHistogram3d.hh"
#include "Ravl/SArray3dIter.hh"
//! lib=RavlMath

namespace RavlN {
  
  //: Create a histogram.
  
  RealHistogram3dC::RealHistogram3dC(const Point3dC &min,const Point3dC &max,const Index3dC &steps) 
    : SArray3dC<UIntT>( (SizeT)RealT(steps[0]), (SizeT)RealT(steps[1]), (SizeT)RealT(steps[2]) )
  {
    scale = max - min;
    //cerr << "Diff=" << scale << "\n";
    scale /= Vector3dC(((RealT) steps[0]) - (1e-8),
		       ((RealT) steps[1]) - (1e-8),
		       ((RealT) steps[2]) - (1e-8));
    //cerr << "Scale=" << scale << "\n";
    offset = min;
    Reset();
  }
  
  //: Find the total number of votes cast.
  
  UIntT RealHistogram3dC::TotalVotes() const {
    UIntT c = 0;
    for(SArray3dIterC<UIntT> it(*this);it;it++) 
      c += *it;
    return c;
  }
  
  //: Calculate the amount of information represented by the histogram.
  // This is also known as the entropy of the histogram.
  
  RealT RealHistogram3dC::Information() const {
    RealT totalp = 0;
    RealT total = TotalVotes();
    for(SArray3dIterC<UIntT> it(*this);it;it++) {
      RealT prob = (RealT) *it / total;
      if(prob>0) totalp += -prob * Log2(prob);
    }
    return totalp;
  }
  
  //: Calculate the energy represented by the original signal.
  
  RealT RealHistogram3dC::Energy() const {
    UIntT total = TotalVotes();
    RealT sum = 0;
    for(SArray3dIterC<UIntT> it(*this);it;it++)
      sum += Pow((RealT) *it / total,2);
    return sum;
  }

  //: Look for the position of the largest value in the accumulator.
  
  Point3dC RealHistogram3dC::Peak() const {
    RealT maxValue = -1;
    Index3dC maxAt(0,0,0);
    for(SArray3dIterC<UIntT> it(*this);it;it++) {
      if(*it > maxValue) {
	maxValue = *it;
	maxAt = it.Index();
      }
    }
    return MidBin(maxAt);
  }
  
  ostream &operator<<(ostream &strm,const RealHistogram3dC &hist) {
    strm << hist.Offset() << ' ' << hist.Scale() << ' ' << (const SArray3dC<UIntT> &)(hist);
    return strm;
  }
  
  istream &operator>>(istream &strm,RealHistogram3dC &hist) {
    Point3dC offset;
    Vector3dC scale;
    SArray3dC<UIntT> xhist;
    strm >> offset >> scale >> xhist;
    hist = RealHistogram3dC(scale,offset,xhist);
    return strm;
  }

}
