// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_ARRAY2DSQR2ITER_HEADER
#define RAVL_ARRAY2DSQR2ITER_HEADER 1
//////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Core.Arrays.2D"
//! author="Charles Galambos"
//! lib=RavlCore
//! file="Ravl/Core/Container/Array/Array2dSqr2Iter.hh"

#include "Ravl/Array2d.hh"
#include "Ravl/BfAcc2Iter.hh"

namespace RavlN {

  //! userlevel=Normal
  //: Iterator which slides a 2x2 window over an array.
  // The square can be accessed with DataBL,DataBR,DataUL and DataUR
  // which access the following array elements. <p>
  //  TL TR <br>
  //  BL BR <br>
  
  template<class DataT>
  class Array2dSqr2IterC 
    : protected BufferAccess2dIterC<DataT>
  {
  public:
    Array2dSqr2IterC()
    {}
    //: Default constructor.
    
    Array2dSqr2IterC(const Array2dC<DataT> &narray) 
      : array(narray)
    { 
      First(); 
    }
    //: Constructor.
    
    bool First() {
      IndexRangeC nrng(array.Range2().Min()+1,array.Range2().Max());
      RavlAssert(nrng.Min() <= nrng.Max());
      if(!BufferAccess2dIterC<DataT>::First(array,nrng))
	return false;
      up = &(*this->cit);
      if(!this->NextRow()) {
	this->cit.Invalidate();
	return false;
      }
      return true;
    }
    //: Goto first element in the array.
    
    bool NextRow(int n = 1) {
      this->rit += n;
      if(!this->rit)
	return false;
      up = &((&(*(this->rit)))[-1][this->rng.Min()]);
      this->cit.First(this->rit.Data(),this->rng);
      return true;
    }
    //: Goto the next row.
    
    bool Next() { 
      up++;
      this->cit++;
      if(this->cit)
	return true;
      up = &((*(this->rit))[this->rng.Min()]);
      this->rit++;
      if(!this->rit)
	return false;
      this->cit.First(this->rit.Data(),this->rng);
      return false;
    }
    //: Goto next element.
    // Returns true if its on the same row.

    bool Next(int n) { 
      up += n;
      this->cit += n;
      if(this->cit)
	return true;
      NextRow(n);
      return false;
    }
    //: Goto next element.
    // Returns true if its on the same row.
    
    void NextCol(int n) { 
      up += n;
      this->cit += n;
    }
    //: Goto next column
    // This will NOT automaticly go to the next row.
    // Returns true if is a valid element.
    
    bool IsElm() const
    { return this->cit.IsElm(); }
    //: Test if iterator is at a valid element.
    
    operator bool() const
    { return this->cit.IsElm(); }
    //: Test if iterator is at a valid element.
    
    void operator++() 
    { Next(); }
    //: Goto next element.

    void operator++(int) 
    { Next(); }
    //: Goto next element.
    
    DataT &operator*() 
    { return *this->cit; }
    //: Access bottom right data element 
    
    const DataT &operator*() const
    { return *this->cit; }
    //: Access bottom right data element 
    
    DataT &DataBR() 
    { return *this->cit; }
    //: Access bottom right data element 

    const DataT &DataBR() const
    { return *this->cit; }
    //: Access bottom right data element 

    DataT &DataBL() 
    { return (&(*this->cit))[-1]; }
    //: Access bottom left data element 

    const DataT &DataBL() const
    { return (&(*this->cit))[-1]; }
    //: Access bottom left data element 
    
    DataT &DataTR() 
    { return *up; }
    //: Access upper right data element 
    
    const DataT &DataTR() const
    { return *up; }
    //: Access upper right data element
    
    DataT &DataTL() 
    { return up[-1]; }
    //: Access upper left data element.
    
    const DataT &DataTL() const
    { return up[-1]; }
    //: Access upper left data element
    
    Index2dC Index() const { 
      RavlAssert(array.IsValid());
      return BufferAccess2dIterC<DataT>::Index(array.ReferenceElm());
    }
    //: Get index of current location.
    // Has to be calculate, and so is slightly slow.
    
  protected:
    Array2dC<DataT> array;
    DataT *up;
  };
}



#endif
