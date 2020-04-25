// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_INDEXRANGE3DITER_HEADER
#define RAVL_INDEXRANGE3DITER_HEADER 1
//! rcsid="$Id$"
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Indexing"
//! lib=RavlCore
//! file="Ravl/Core/Base/IndexRange3dIter.hh"

#include "Ravl/IndexRange3d.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: Iterate through a 3d range.
  
  class IndexRange3dIterC {
  public:
    IndexRange3dIterC()
    {}
    //: Default constructor.

    IndexRange3dIterC(const IndexRange3dC &rng)
      : range(rng)
    { First(); }
    //:  Construct from an index range.
    
    bool First() {
      at = range.Origin();
      return true;
    }
    //: Goto first index.
    
    const Index3dC &Data() const
    { return at; }
    //: Access current position.
    
    const Index3dC &operator*() const
    { return at; }
    //: Access current position.
    
    bool Next() {
      if(++at.K() <= range.MaxK())
	return true;
      at.K() = range.MinK();
      if(++at.J() <= range.MaxJ())
	return false;
      at.J() = range.MinJ();
      at.I()++;
      return false;
    }
    //: Goto next index.
    // Returns true if on the same line.
    
    void operator++(int)
    { Next(); }
    //: Goto next index.
    
    bool IsElm() const
    { return at.I() <= range.MaxI(); }
    //: At a valid index in rectangle ?
    
    operator bool() const
    { return IsElm(); }
    //: At a valid index in rectangle ?
    
    const IndexRange3dC &Frame() const
    { return range; }
    //: Access the frame we're iterating.
    
    IndexRange3dC &Frame()
    { return range; }
    //: Access the frame we're iterating.
    
  protected:
    Index3dC at;
    IndexRange3dC range;
  };
}


#endif
