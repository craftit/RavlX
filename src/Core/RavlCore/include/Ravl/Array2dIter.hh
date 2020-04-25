// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_ARRAY2DITER_HEADER
#define RAVL_ARRAY2DITER_HEADER 1
///////////////////////////////////////////////////////////////////////
//! file="Ravl/Core/Container/Array/Array2dIter.hh"
//! lib=RavlCore
//! userlevel=Basic
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Arrays.2D"
//! rcsid="$Id$"
//! example=exArray2.cc
//! date="24/08/1999"

#include "Ravl/BfAccIter.hh"
#include "Ravl/Array2d.hh"
#include "Ravl/Assert.hh"

namespace RavlN {
  
  template <class DataC> class Array2dC;
  
  //! userlevel=Normal
  //: Array2dC iterator.
  
  template<class DataT>
  class Array2dIterC  
    : public BufferAccess2dIterC<DataT>
  {
  public:
    Array2dIterC()
    {}
    //: Default constructor.
    
    Array2dIterC(const Array2dC<DataT> &arr,const IndexRange2dC &rng)
      : dat(arr,rng)
    { First(); }
    //: Constructor.

    Array2dIterC(const Array2dC<DataT> &arr)
      : dat(arr)
    { First(); }
    //: Constructor.
    
    inline bool First() 
    { return BufferAccess2dIterC<DataT>::First(dat,dat.Range2()); }
    
    //: Goto first element in the array.
    // Return TRUE if there actually is one.
    
    const Array2dIterC<DataT> &operator=(const Array2dC<DataT> &arr) {
      dat = arr;
      First();
      return *this;
    }
    //: Assign to another array.

    Index2dC Index() const { 
      RavlAssert(dat.IsValid());
      return BufferAccess2dIterC<DataT>::Index(dat.ReferenceElm());
    }
    //: Get index of current location.
    // Has to be calculate, and so is slightly slow.
    
  protected:
    Array2dC<DataT> dat;
  };
  
}

#endif

