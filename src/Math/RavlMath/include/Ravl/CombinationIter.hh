// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLCOMBINATIONITER_HEADER
#define RAVLCOMBINATIONITER_HEADER 1
/////////////////////////////////////////////////////
//! userlevel=Normal
//! author="Charles Galambos"
//! rcsid="$Id$"
//! file="Ravl/Math/Sequence/CombinationIter.hh"
//! docentry="Ravl.API.Math.Sequences"
//! date="24/08/98"
//! lib=RavlMath

#include "Ravl/DList.hh"
#include "Ravl/DLIter.hh"

namespace RavlN {
  //: Combination iterator.
  // This iterator works from shorter to longer combinations
  // progressively. <p>
  // SMALL OBJECT
  
  template<class DataT>
  class CombinationIterC {
  public:
    CombinationIterC(IntT nStartLen = 1);
    //: Default constructor.
    
    CombinationIterC(const DListC<DataT> &items,IntT nStartLen = 1);
    //: Constructor.
    
    CombinationIterC(const CombinationIterC<DataT> &oth);
    //: Copy constructor.
    // This is a medium depth copy, it only
    // copies enough to get an independant iteration
    // of combination. The items list is NOT copied,
    // so modifying it will affect the iterator.
    
    const CombinationIterC<DataT> &operator=(const DListC<DataT> &items);
    //: Assign to new list. 
    // Does an implicit First().
    
    inline bool IsElm() const
    { return !data.IsEmpty(); }
    //: Is there a valid combination remaining ?

    operator bool() const
    { return IsElm(); }
    //: Is there a valid combination remaining ?
    
    inline DListC<DataT> &Data()
    { return data; }
    //: Current combination.
    
    inline IntT Terms() const
    { return combSize; }
    //: Get number of items in current combination.
    
    bool First();
    //: Goto first combination.
    // Returns false if none.
    
    bool Next();
    //: Goto next combination.
    // Returns false if none.
    
    void operator++(int)
    { Next(); }
    //: Goto next combination.
    
  private:
    bool IncrComb(DLIterC<DLIterC<DataT > > lst);
    //: Increment combination.
    
    IntT combSize; // Number of items in combination.  
    IntT startLen; // Start length.
    DListC<DLIterC<DataT > > cIter; // List of combination iters.
    DListC<DataT> items; // List of all items.
    DListC<DataT> data;  // Current combination.
  };
  
  /////////////////////////////////////////////////////////////
  
  template<class DataT>
  CombinationIterC<DataT>::CombinationIterC(IntT nStartLen) 
    : combSize(0),
      startLen(nStartLen)
  {}
  
  template<class DataT>
  CombinationIterC<DataT>::CombinationIterC(const DListC<DataT> &nitems,IntT nStartLen)
    : combSize(1),
      startLen(nStartLen),
      items(nitems)
  { First(); }
  
  template<class DataT>
  CombinationIterC<DataT>::CombinationIterC(const CombinationIterC<DataT> &oth)
    : combSize(oth.combSize),
      cIter(oth.cIter.Copy()),
      items(oth.items),
      data(oth.data.Copy())
  {}
  
  template<class DataT>
  const CombinationIterC<DataT> &
  CombinationIterC<DataT>::operator=(const DListC<DataT> &nItems) {
    items = nItems;
    First();
    return *this;
  }
  
  template<class DataT>
  bool CombinationIterC<DataT>::First() {
    combSize = startLen;
    cIter.Empty();
    if(combSize != 1)
      return Next();
    DLIterC<DataT> sIt(items);
    if(!sIt.IsElm())
      return false;
    cIter.InsFirst(sIt);  
    data.Empty();
    data.InsLast(sIt.Data());
    return true;
  }
  
  template<class DataT>
  bool CombinationIterC<DataT>::Next() {
    data.Empty();
    while(1) {
      if(cIter.IsEmpty()) {
	DLIterC<DataT> sIt(items);
	// Setup iteration of new combination length.
	for(int j = 0;j < combSize;j++) {
	  if(!sIt.IsElm())
	    return false;
	  cIter.InsFirst(sIt);
	  sIt.Next();
	}
      } else {
	// Generate combinations.
	DLIterC<DLIterC<DataT> > cIt(cIter);
	if(!IncrComb(cIt)) {
	  // Out of combination length 'combSize'.
	  combSize++;
	  cIter.Empty();
	  continue;
	}
      }
      break;
    } 
    // Setup data.
    for(DLIterC<DLIterC<DataT > > cIt(cIter);cIt.IsElm();cIt.Next()) 
      data.InsFirst(cIt.Data().Data());
    return true;    
  }
  
  template<class DataT>
  bool CombinationIterC<DataT>::IncrComb(DLIterC<DLIterC<DataT> > lst) {
    // I'm sure this is NOT the best way of iterating combinations.
    // but it works.
    
    lst.Data().Next(); // Inc LSB of remaining lst.
    if(lst.Data().IsElm()) // Off the end ?
      return true; // No, so done.
    lst.Next(); // Have to increment next.
    if(!lst.IsElm())
      return false; // No higher index !
    if(!IncrComb(lst))
      return false; // Incrementing next failed.
    DLIterC<DataT> newPos;
    do {
      newPos = lst.Data();
      newPos.Next();
      if(!newPos.IsElm()) {
	if(!IncrComb(lst))
	  return false;
      } else
	break;
    } while(1);
    lst.Prev();
    lst.Data() = newPos;
    return true;
  }
}


#endif
