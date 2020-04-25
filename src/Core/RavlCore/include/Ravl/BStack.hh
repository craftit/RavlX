// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BSTACK_HEADER
#define RAVL_BSTACK_HEADER 1
//! rcsid="$Id$"
//! lib=RavlCore
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Branch"
//! file="Ravl/Core/Container/Branch/BStack.hh"

#include "Ravl/BList.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: Stack.
  
  template<class DataT>
  class BStackC
    : public BListC<DataT>
  {
  public:
    BStackC()
    {}
    //: Default constructor.
    
    DataT Pop() {
      DataT dat(this->list->Data());
      RavlAssert(this->list.IsValid()); 
      this->list = this->list.Next();
      return dat;
    }
    //: Pop element off the top of the stack.
    
    void PopDel() {
      RavlAssert(this->list.IsValid()); 
      this->list = this->list.Next();
    }
    //: Pop element off the top of the stack but don't return it
    
    void Push(const DataT &dat) 
    { this->InsFirst(dat); }
    //: Push item onto the top of stack.
    
    const DataT &Top() const
    { return this->First(); }
    //: Access element at the top of the stack.
  };
}

#endif
