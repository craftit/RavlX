// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_REALRANGE2DSET_HEADER
#define RAVL_REALRANGE2DSET_HEADER 1
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Core/System/RealRange2dSet.hh"
//! lib=RavlCore
//! docentry="Ravl.API.Core.Indexing"
//! author="Charles Galambos"
//! date="11/10/1999"

#include "Ravl/RealRange2d.hh"
#include "Ravl/DList.hh"

namespace RavlN {
  //! userlevel=Normal
  //: Rectangle set.
  // Handles a set region defined by a set of nonoverlaping rectangles.
  // The methods in this class ensure that each part of the range is only
  // covered by a single rectangle.
  
  class RealRange2dSetC
    : public DListC<RealRange2dC>
  {
  public:
    RealRange2dSetC()
    {}
    //: Default constructor.
  
    RealRange2dSetC(const RealRange2dC &rect)
    { InsLast(rect); }
    //: Constructor.
    
    static RealRange2dSetC Subtract(const RealRange2dC &rect1,const RealRange2dC &rect2);
    //: Subtract rect2 from rect1.
  
    static RealRange2dSetC Add(const RealRange2dC &rect1,const RealRange2dC &rect2);
    //: Add rect2 and rect1.
  
    RealRange2dC Enclosing() const;
    //: Get minimum enclosing rectangle for set.
    
    RealRange2dSetC Subtract(const RealRange2dC &rect) const;
    //: Remove 'rect' rectangle from the region given by the set.

    RealRange2dSetC Subtract(const RealRange2dSetC &rectset) const;
    //: Remove 'rectset' from the region given by the set.

    RealRange2dSetC SubtractFrom(const RealRange2dC &rect) const;
    //: Remove set from rect.
  
    RealRange2dSetC Add(const RealRange2dC &rect) const;
    //: Add this rectangle to the set.

    RealRange2dSetC Add(const RealRange2dSetC &rect) const;
    //: Add  rectangle set to this set.
  
    bool Contains(const RealRange2dC &rect) const;
    //: Does this set wholly contain 'rect' ?
    
    RealT Area() const;
    //: Total area of set.
  };

}


#endif
