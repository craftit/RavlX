// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DARRAY1DITER3_HEADER
#define RAVL_DARRAY1DITER3_HEADER 1
//! docentry="Ravl.API.Core.Arrays.1D"
//! lib=RavlCore
//! author="Charles Galambos"
//! rcsid="$Id$"
//! file="Ravl/Core/System/DArray1dIter3.hh"

#include "Ravl/DArray1dIter2.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: Dynamic array iterator.
  // This iterator does not use the position index's of the arrays,
  // it will go though each array in order the elements appear.
  
  template<class Data1T,class Data2T,class Data3T>
  class DArray1dIter3C 
    : public DArray1dIter2C<Data1T,Data2T>
  {
  public:
    DArray1dIter3C()
    {}
    //: Default constructor.
    
    DArray1dIter3C(const DArray1dC<Data1T> &narr1,const DArray1dC<Data2T> &narr2,const DArray1dC<Data3T> &narr3)
      : DArray1dIter2C<Data1T,Data2T>(narr1,narr2),
	it3(narr3)
    { First(); }
    //: Constructor.
    
    bool First() {
      if(!this->it1.First())
	return false;
      if(this->it2.First() && this->it3.First())
	return true;
      this->it1.Invalidate();
      return false;
    }
    //: Goto first element in the array.
    // Returns true if iterator is at a valid element after operation.
    
    Data3T &Data3()
    { return it3.Data(); }
    //: Access data.
    
    const Data3T &Data3() const
    { return it3.Data(); }
    //: Access data.
    
    inline bool Next();
    //: Goto next element.
    // Returns true if iterator is at a valid element after operation.
    
    void operator++(int)
    { Next(); }
    //: Goto next element.
    
    DArray1dIter3C &operator++()
    { Next(); return *this; }
    //: Goto next element.

    IndexC Index() const
    { return this->it1.Index(); }
    //: Get index of current element.
    
  protected:
    DArray1dIterC<Data3T> it3;
  };
  
  
  template<class Data1T,class Data2T,class Data3T>
  inline bool DArray1dIter3C<Data1T,Data2T,Data3T>::Next() { 
    if(!this->it1.Next()) return false;
    if(this->it2.Next() && it3.Next())
      return true;
    this->it1.Invalidate();
    return false;
  }
  
  
}


#endif
