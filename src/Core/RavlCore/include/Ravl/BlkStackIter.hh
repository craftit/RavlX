// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLBLKSTACKITER_HEADER
#define RAVLBLKSTACKITER_HEADER 1
/////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Stacks"
//! lib=RavlCore
//! date="26/08/2001"
//! file="Ravl/Core/Container/Misc/BlkStackIter.hh"

#include "Ravl/BlkStack.hh"

namespace RavlN {

  //! userlevel=Advanced
  //: BlkStackC iterator.
  // Iterate back through the contents of a block stack.
  
  template<class DataT>
  class BlkStackIterC
  {
  public:
    BlkStackIterC()
      : at(0),
	bob(0)
    {}
    //: Default constructor.

    BlkStackIterC(BlkStackC<DataT> &stk)
      : block(stk.topBlk),
	at(stk.top),
	bob(stk.bob)
    {}
    //: Constructor.
    
    const BlkStackIterC<DataT> &operator=(BlkStackC<DataT> &stk) {
      at = stk.top;
      bob = stk.bob;
      block = stk.topBlk;
      return *this;
    }
    //: Assigned to another stack.
    // the iterator will be at the top of the stack after
    // the assignment operations.
    
    bool Next() {
      at--;
      if(at != bob)
	return true;
      if(block->last == 0)
	return false;
      block = block->last;
      bob = &block->data[-1];
      at = &(block->data[block->size-1]);
      return true;
    }
    //: Goto next element.
    
    bool IsElm() const
    { return at != bob; }
    //: At a valid element ?
    
    operator bool() const
    { return at != bob; }
    //: At a valid element.
    
    DataT &Data()
    { return *at; }
    //: Access data element.
    // This may only be called if IsElm() returns true.
    
    const DataT &Data() const
    { return *at; }
    //: Access data element.
    // This may only be called if IsElm() returns true.
    
    
  protected:
    typename BlkStackC<DataT>::SBlkC *block;
    DataT *at;
    DataT *bob;
  };
}

#endif
