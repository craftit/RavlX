// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_REALRANGE1DSET_HEADER
#define RAVL_REALRANGE1DSET_HEADER 1
/////////////////////////////////////////////////////////
//! rcsid="$Id: RealRange1dSet.hh 5240 2005-12-06 17:16:50Z plugger $"
//! file="Ravl/Core/System/RealRange1dSet.hh"
//! lib=RavlCore
//! docentry="Ravl.API.Core.Indexing"
//! author="Charles Galambos"
//! date="11/10/1999"

#include "Ravl/RealRange1d.hh"
#include "Ravl/DList.hh"

namespace RavlN {
  //! userlevel=Normal
  //: rangeangle set.
  // Handles a set region defined by a set of non-overlaping ranges.
  
  class RealRange1dSetC
    : public DListC<RealRangeC>
  {
  public:
    RealRange1dSetC()
    {}
    //: Default constructor.
  
    RealRange1dSetC(const RealRangeC &range)
    { InsLast(range); }
    //: Constructor.
    
    static RealRange1dSetC Subtract(const RealRangeC &range1,const RealRangeC &range2);
    //: Subtract range2 from range1.
  
    static RealRange1dSetC Add(const RealRangeC &range1,const RealRangeC &range2);
    //: Add range2 and range1.
  
    RealRangeC Enclosing() const;
    //: Get minimum enclosing range for set.
    
    RealRange1dSetC Subtract(const RealRangeC &range) const;
    //: Remove 'range' range from the region given by the set.

    RealRange1dSetC Subtract(const RealRange1dSetC &rangeSet) const;
    //: Remove 'rangeSet' from the region given by the set.

    RealRange1dSetC SubtractFrom(const RealRangeC &range) const;
    //: Remove set from range.
  
    RealRange1dSetC Add(const RealRangeC &range) const;
    //: Add this range to the set.

    RealRange1dSetC Add(const RealRange1dSetC &range) const;
    //: Add range set to this set.
  
    bool Contains(const RealRangeC &range) const;
    //: Does this set wholly contain 'range' ?
    
    RealT Length() const;
    //: Total length of set.
  };

}


#endif
