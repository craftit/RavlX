// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DARRAY1DITER2_HEADER
#define RAVL_DARRAY1DITER2_HEADER 1
//! docentry="Ravl.API.Core.Arrays.1D"
//! lib=RavlCore
//! author="Charles Galambos"
//! rcsid="$Id$"
//! file="Ravl/Core/System/DArray1dIter2.hh"

#include "Ravl/DArray1dIter.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: Dynamic array iterator.
  // This iterator does not use the position index's of the arrays,
  // it will go though each array in order the elements appear.
  
  template<class Data1T,class Data2T>
  class DArray1dIter2C {
  public:
    DArray1dIter2C()
    {}
    //: Default constructor.
    
    DArray1dIter2C(const DArray1dC<Data1T> &narr1,const DArray1dC<Data2T> &narr2)
      : it1(narr1),
	it2(narr2)
    { First(); }
    //: Constructor.
    
    bool First() {
      if(!it1.First())
	return false;
      if(it2.First())
	return true;
      it1.Invalidate();
      return false;
    }
    //: Goto first element in the array.
    // Returns true if iterator is at a valid element after operation.
    
    bool IsElm() const
    { return it1.IsElm(); }
    //: At a valid element ?
    
    operator bool() const
    { return it1.IsElm(); }
    //: At a valid element ?

    Data1T &Data1()
    { return it1.Data(); }
    //: Access data.
    
    const Data1T &Data1() const
    { return it1.Data(); }
    //: Access data.
    
    Data2T &Data2()
    { return it2.Data(); }
    //: Access data.
    
    const Data2T &Data2() const
    { return it2.Data(); }
    //: Access data.
    
    inline
    bool Next();
    //: Goto next element.
    // Returns true if iterator is at a valid element after operation.
    
    void operator++(int)
    { Next(); }
    //: Goto next element.
    
    DArray1dIter2C &operator++()
    { Next(); return *this; }
    //: Goto next element.

    IndexC Index() const
    { return it1.Index(); }
    //: Get index of current element.
    
  protected:
    DArray1dIterC<Data1T> it1;
    DArray1dIterC<Data2T> it2;
  };


  template<class Data1T,class Data2T>
  inline bool DArray1dIter2C<Data1T,Data2T>::Next() { 
    if(!it1.Next())
      return false;
    if(it2.Next())
      return true;
    it1.Invalidate();
    return false;
  }
  

}


#endif
