// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SARR3ITER_HEADER
#define RAVL_SARR3ITER_HEADER 1
////////////////////////////////////////////////////////////
//! docentry="Ravl.API.Core.Arrays.3D"
//! file="Ravl/Core/Container/SArray/SArray3dIter.hh"
//! lib=RavlCore
//! author="Charles Galambos"
//! date="10/09/1998"
//! userlevel=Advanced

#include "Ravl/SArray3d.hh"
#include "Ravl/BfAcc3Iter.hh"
#include "Ravl/Index3d.hh"

namespace RavlN {

  //! userlevel=Advanced
  //: SArray3dC iterator.
  // Simple 3d array iterator.
  
  template<class DataT>
  class SArray3dIterC 
    : public BufferAccess3dIterC<DataT>
  {
  public:
    SArray3dIterC()
    {}
    //: Default constructor.
    
    SArray3dIterC(const SArray3dC<DataT> &narr)
      : arr(narr)
    { First(); }
    //: Constructor.
    
    const SArray3dIterC<DataT> &operator=(SArray3dC<DataT> &narr) {
      arr = narr;
      First();
      return *this;
    }
    //: Assignment to an array.
    
    inline void First()
    { BufferAccess3dIterC<DataT>::First(arr,arr.Size2(),arr.Size3()); }
    //: Goto first element in array.
    
    Index3dC Index() const { 
      RavlAssert(arr.IsValid());
      Index2dC i2 = this->sit.Index(this->rit->ReferenceElm());
 
      return Index3dC((IndexC) (&(*this->rit) - arr.ReferenceElm()),
		      (IndexC) i2.Row(),
		      (IndexC) i2.Col()); 
    }
    //: Get index of current location.
    // Has to be calculate, and so is slightly slow.
    
  protected:
    SArray3dC<DataT> arr;
  };
  
  
}

#endif
