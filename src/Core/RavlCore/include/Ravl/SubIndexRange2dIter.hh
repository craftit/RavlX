// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SUBINDEXRANGE2DITER_HEADER
#define RAVL_SUBINDEXRANGE2DITER_HEADER 1
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Indexing"
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Base/SubIndexRange2dIter.hh"

#include "Ravl/IndexRange2d.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: Slide a rectangular window over a larger rectangle.
  // Slide a window over all possible positions in a
  // larger rectangle.
  
  class SubIndexRange2dIterC 
  {
  public:
     SubIndexRange2dIterC(const IndexRange2dC &nImageRect,const IndexRange2dC &nwindow)
      : size2(nwindow.Range2().Size()),
	imageRect(nImageRect),
	window(nwindow)
    { First(); }
    //: Constructor.
    
    IndexRange2dC &Window()
    { return window; }
    //: Access current window.

    const IndexRange2dC &Window() const
    { return window; }
    //: Access current window.
    
    void First() { 
      UIntT size1 = window.Range1().Size();
      window.Range1().Min() = imageRect.Range1().Min();
      window.Range1().Max() = window.Range1().Min() + size1 - 1;
      window.Range2().Min() = imageRect.Range2().Min();
      window.Range2().Max() = window.Range2().Min() + size2 - 1;
    }
    //: Goto first position.
    
    bool Next() {
      ++(window.Range2());
      if(window.Range2().Max() <= imageRect.Range2().Max())
	return true;
      window.Range2().Min() = imageRect.Range2().Min();
      window.Range2().Max() = window.Range2().Min() + size2 - 1;
      ++(window.Range1());
      return false;
    }
    //:Goto next position.
    // Returns true if window is on the same row.
    
    bool operator++(int) 
    { return Next(); }
    //:Goto next position.
    // Returns true if window is on the same row.
    
    bool IsElm() const
    { return window.Range1().Max() <= imageRect.Range1().Max(); }
    //: At a valid position ?

    operator bool() const
    { return IsElm(); }
    //: At a valid position ?
    
  protected:
    UIntT size2;
    IndexRange2dC imageRect;
    IndexRange2dC window;
  };
  
}

#endif
