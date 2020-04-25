// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////
//! rcsid="$Id: RealRange1dSet.cc 4457 2004-07-29 14:31:20Z craftit $"
//! lib=RavlCore
//! file="Ravl/Core/System/RealRange1dSet.cc"

#include "Ravl/RealRange1dSet.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/DListExtra.hh"

#define DODEBUG 0
#if DODEBUG
#include "Ravl/Stream.hh"
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  //: Get minimum enclosing range for set.

  RealRangeC RealRange1dSetC::Enclosing() const {
    DLIterC<RealRangeC> it(*this);
    if(!it.IsElm())
      return RealRangeC(0,0);
    RealRangeC ret(it.Data());
    it.Next();
    for(;it.IsElm();it.Next())
      ret.Involve(it.Data());
    return ret;
  }

  //: Subtract range2 from range1.

  RealRange1dSetC RealRange1dSetC::Subtract(const RealRangeC &range1,const RealRangeC &range2) {
    if(!range1.IsOverlapping(range2)) {
      // Ranges don't overlap.
      return RealRange1dSetC(range1);
    }

    RealRange1dSetC ret;
    ONDEBUG(std::cerr << "range overlap. \n");

    // Before ?
    if(range1.Min() < range2.Min()) {
      ret.InsLast(RealRangeC(range1.Min(),range2.Min()));
    }

    // After ?
    if(range1.Max() > range2.Max()) {
      ret.InsLast(RealRangeC(range2.Max(),range1.Max()));
    }

    return ret;
  }

  //: Add range2 and range1.

  RealRange1dSetC RealRange1dSetC::Add(const RealRangeC &range1,const RealRangeC &range2) {
    RealRange1dSetC ret;
    if(!range1.IsOverlapping(range2)) { // No overlap.
      ret.InsLast(range1);
      ret.InsLast(range2);
      return ret;
    }
    RealRangeC tmp = range1;
    tmp.Involve(range2);

    ret.InsLast(tmp);
    return ret;
  }

  //: Remove 'range' from the region given by the set.

  RealRange1dSetC RealRange1dSetC::Subtract(const RealRangeC &range) const {
    RealRange1dSetC ret;
    for(DLIterC<RealRangeC> it(*this);it.IsElm();it.Next())
      ret += Subtract(it.Data(),range);
    return ret;
  }

  //: Remove 'rangeSet' from the region given by the set.

  RealRange1dSetC RealRange1dSetC::Subtract(const RealRange1dSetC &rangeSet) const {
    RealRange1dSetC ret = (*this);
    for(DLIterC<RealRangeC> it(rangeSet);it.IsElm();it.Next())
      ret = ret.Subtract(it.Data());
    return ret;
  }

  //: Remove set from range.

  RealRange1dSetC RealRange1dSetC::SubtractFrom(const RealRangeC &range) const
  { return RealRange1dSetC(range).Subtract(*this); }

  //: Add this range to the set.

  RealRange1dSetC RealRange1dSetC::Add(const RealRangeC &range) const {
    RealRangeC newRange = range;
    RealRange1dSetC ret;
    // Merge in any ranges it overlaps and remove from results.
    for(DLIterC<RealRangeC> it(*this);it.IsElm();it.Next()) {
      if(it->IsOverlapping(newRange)) {
        newRange.Involve(*it);
      } else {
        ret.InsLast(*it);
      }
    }
    // Add in new range.
    ret.InsLast(newRange);
    return ret;
  }

  //: Add  range set to this set.

  RealRange1dSetC RealRange1dSetC::Add(const RealRange1dSetC &range) const {
    RealRange1dSetC ret(*this);
    for(DLIterC<RealRangeC> it(*this);it.IsElm();it.Next())
      ret = ret.Add(it.Data());
    return ret;
  }

  //: Does this set wholy contain 'range' ?

  bool RealRange1dSetC::Contains(const RealRangeC &range) const {
    RealRange1dSetC remainder(range);
    for(DLIterC<RealRangeC> it(*this);it.IsElm();it.Next()) {
      remainder = remainder.Subtract(it.Data());
      if(remainder.IsEmpty())
	return true;
    }
    return false;
  }

  //: Total area of set.

  RealT RealRange1dSetC::Length() const {
    RealT ret = 0;
    for(DLIterC<RealRangeC> it(*this);it.IsElm();it.Next()) {
      ONDEBUG(std::cerr << "+Area: " << it.Data().Area() << "\n");
      ret += it.Data().Size();
    }
    return ret;
  }

}
