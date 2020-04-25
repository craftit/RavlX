// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DARRAY1DITER5_HEADER
#define RAVL_DARRAY1DITER5_HEADER 1
//! docentry="Ravl.API.Core.Arrays.1D"
//! lib=RavlCore
//! author="Ilias Kolonias"
//! rcsid="$Id$"
//! file="Ravl/Core/System/DArray1dIter5.hh"

#include "Ravl/DArray1dIter4.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: Dynamic array iterator.
  // This iterator does not use the position index's of the arrays,
  // it will go though each array in order the elements appear.
  
  template<class Data1T,class Data2T,class Data3T,class Data4T,class Data5T>
  class DArray1dIter5C
    : public DArray1dIter4C<Data1T,Data2T,Data3T,Data4T>
  {
  public:
    DArray1dIter5C()
    {}
    //: Default constructor.
    
    DArray1dIter5C(const DArray1dC<Data1T> &narr1,
		   const DArray1dC<Data2T> &narr2,
		   const DArray1dC<Data3T> &narr3,
		   const DArray1dC<Data4T> &narr4,
		   const DArray1dC<Data5T> &narr5)
      : DArray1dIter4C<Data1T,Data2T,Data3T,Data4T>(narr1,narr2,narr3,narr4),
	it5(narr5)
    { First(); }
    //: Constructor.
    
    bool First() {
      if(!this->it1.First())
	return false;
      if(this->it2.First() && this->it3.First() && this->it4.First() && it5.First())
	return true;
      this->it1.Invalidate();
      return false;
    }
    //: Goto first element in the array.
    // Returns true if iterator is at a valid element after operation.
    
    Data5T &Data5()
    { return it5.Data(); }
    //: Access data.
    
    const Data5T &Data5() const
    { return it5.Data(); }
    //: Access data.
    
    inline bool Next();
    //: Goto next element.
    // Returns true if iterator is at a valid element after operation.
    
    void operator++(int)
    { Next(); }
    //: Goto next element.
    
    DArray1dIter5C &operator++()
    { Next(); return *this; }
    //: Goto next element.

    IndexC Index() const
    { return this->it1.Index(); }
    //: Get index of current element.
    
  protected:
    DArray1dIterC<Data5T> it5;
  };
  
  
  template<class Data1T,class Data2T,class Data3T,class Data4T,class Data5T>
  inline bool DArray1dIter5C<Data1T,Data2T,Data3T,Data4T,Data5T>::Next() { 
    if(!this->it1.Next()) return false;
    if(this->it2.Next() && this->it3.Next() && this->it4.Next() && it5.Next())
      return true;
    this->it1.Invalidate();
    return false;
  }
  
  
}


#endif

