// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DARRAY1DITER4_HEADER
#define RAVL_DARRAY1DITER4_HEADER 1
//! docentry="Ravl.API.Core.Arrays.1D"
//! lib=RavlCore
//! author="Charles Galambos"
//! rcsid="$Id$"
//! file="Ravl/Core/System/DArray1dIter4.hh"

#include "Ravl/DArray1dIter3.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: Dynamic array iterator.
  // This iterator does not use the position index's of the arrays,
  // it will go though each array in order the elements appear.
  
  template<class Data1T,class Data2T,class Data3T,class Data4T>
  class DArray1dIter4C 
    : public DArray1dIter3C<Data1T,Data2T,Data3T>
  {
  public:
    DArray1dIter4C()
    {}
    //: Default constructor.
    
    DArray1dIter4C(const DArray1dC<Data1T> &narr1,
		   const DArray1dC<Data2T> &narr2,
		   const DArray1dC<Data3T> &narr3,
		   const DArray1dC<Data4T> &narr4)
      : DArray1dIter3C<Data1T,Data2T,Data3T>(narr1,narr2,narr3),
	it4(narr4)
    { First(); }
    //: Constructor.
    
    bool First() {
      if(!this->it1.First())
	return false;
      if(this->it2.First() && this->it3.First() && it4.First())
	return true;
      this->it1.Invalidate();
      return false;
    }
    //: Goto first element in the array.
    // Returns true if iterator is at a valid element after operation.
    
    Data4T &Data4()
    { return it4.Data(); }
    //: Access data.
    
    const Data4T &Data4() const
    { return it4.Data(); }
    //: Access data.
    
    inline bool Next();
    //: Goto next element.
    // Returns true if iterator is at a valid element after operation.
    
    void operator++(int)
    { Next(); }
    //: Goto next element.
    
    DArray1dIter4C &operator++()
    { Next(); return *this; }
    //: Goto next element.

    IndexC Index() const
    { return this->it1.Index(); }
    //: Get index of current element.
    
  protected:
    DArray1dIterC<Data4T> it4;
  };
  
  
  template<class Data1T,class Data2T,class Data3T,class Data4T>
  inline bool DArray1dIter4C<Data1T,Data2T,Data3T,Data4T>::Next() { 
    if(!this->it1.Next()) return false;
    if(this->it2.Next() && this->it3.Next() && it4.Next())
      return true;
    this->it1.Invalidate();
    return false;
  }
  
  
}


#endif
