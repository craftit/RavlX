// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_ARRAY2DSQR3ITER_HEADER
#define RAVL_ARRAY2DSQR3ITER_HEADER 1
//////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Arrays.2D"
//! lib=RavlCore
//! file="Ravl/Core/Container/Array/Array2dSqr3Iter.hh"

#include "Ravl/Array2d.hh"
#include "Ravl/BfAcc2Iter.hh"

namespace RavlN {

  //! userlevel=Normal
  //: Iterate through an array accessing 3x3 area.
  // The square can be accessed with DataBL,DataBR etc...
  // which access the following array elements. <p>
  //  TL TM TR <br>
  //  ML MM MR <br>
  //  BL BM BR <br>
  
  template<class DataT>
  class Array2dSqr3IterC 
    : protected BufferAccess2dIterC<DataT>
  {
  public:
    Array2dSqr3IterC()
    {}
    //: Default constructor.
    
    Array2dSqr3IterC(const Array2dC<DataT> &narray) 
      : array(narray)
    { First(); }
    //: Constructor.
    
    Array2dSqr3IterC(const Array2dC<DataT> &narray, const IndexRange2dC &limitedRange) 
      : array(narray,limitedRange)
    { First(); }
    //: Constructor over limited range.
    
    bool First() {
      this->rng = IndexRangeC(array.Range2().Min()+1,array.Range2().Max()-1);
      IndexRangeC srng(array.Range1().Min()+1,array.Range1().Max()-1);
      if(!this->rit.First(array,srng)) {
	this->cit.Invalidate();
	return false;
      }
      this->cit.First(*this->rit,this->rng);
      if(!this->cit) {
	this->cit.Invalidate();
	return false;
      }
      up = &((&(*this->rit))[-1][this->rng.Min()]);
      dn = &((&(*this->rit))[ 1][this->rng.Min()]);
      return true;
    }
    //: Goto first element in the array.
    
    bool NextRow() {
      this->rit++;
      if(!this->rit)
	return false;
      up = &((&(*this->rit))[-1][this->rng.Min()]);
      dn = &((&(*this->rit))[ 1][this->rng.Min()]);
      this->cit.First(this->rit.Data(),this->rng);
      return true;
    }
    //: Goto the next row.

    bool NextRow(UIntT n) {
      this->rit += n;
      if(!this->rit)
	return false;
      up = &((&(*this->rit))[-1][this->rng.Min()]);
      dn = &((&(*this->rit))[ 1][this->rng.Min()]);
      this->cit.First(this->rit.Data(),this->rng);
      return true;
    }
    //: Goto the n th next row.
    
    bool NextCol() { 
      up++;
      dn++;
      this->cit++;
      return (this->cit);
    }
    //: Goto next column.
    
    bool NextCol(UIntT n) { 
      up += n;
      dn += n;
      this->cit += n;
      return (this->cit);
    }
    //: Goto n th next column.
    
    bool Next() { 
      up++;
      dn++;
      this->cit++;
      if(this->cit)
	return true;
      NextRow();
      return false;
    }
    //: Goto next element.

    bool Next(int n) {
      up += n;
      dn += n;
      this->cit += n;
      if(this->cit)
	return true;
      NextRow(n);
      return false;
    }
    //: Goto next nth element
    // This skips n elements along the row
    // if the end of the row is reached then it will
    // then skip n rows.
    
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
    
    void operator+=(int n)
    { Next(n); }
    //: Skip n elements.
    // This skips n elements along the row
    // if the end of the row is reached then it will
    // then skip n rows.
    
    DataT &DataBL() 
    { return dn[-1]; }
    //: Access bottom left data element 

    const DataT &DataBL() const
    { return dn[-1]; }
    //: Access bottom left data element 

    DataT &DataBM() 
    { return *dn; }
    //: Access bottom mid data element 

    const DataT &DataBM() const
    { return *dn; }
    //: Access bottom mid data element 

    DataT &DataBR() 
    { return dn[1]; }
    //: Access bottom right data element 

    const DataT &DataBR() const
    { return dn[1]; }
    //: Access bottom right data element 

    DataT &DataML() 
    { return (&(*this->cit))[-1]; }
    //: Access middle left data element 
    
    const DataT &DataML() const
    { return (&(*this->cit))[-1]; }
    //: Access middle left data element 
    
    DataT &DataMM() 
    { return *this->cit; }
    //: Access middle data element 
    
    const DataT &DataMM() const
    { return *this->cit; }
    //: Access middle data element 
    
    DataT &DataMR() 
    { return (&(*this->cit))[1]; }
    //: Access middle right data element 
    
    const DataT &DataMR() const
    { return (&(*this->cit))[1]; }
    //: Access middle right data element 

    DataT &DataTL() 
    { return up[-1]; }
    //: Access top left data element.
    
    const DataT &DataTL() const
    { return up[-1]; }
    //: Access top left data element

    DataT &DataTM() 
    { return *up; }
    //: Access top middle element 
    
    const DataT &DataTM() const
    { return *up; }
    //: Access top middle element
    
    DataT &DataTR() 
    { return up[1]; }
    //: Access top right data element 
    
    const DataT &DataTR() const
    { return up[1]; }
    //: Access top right data element
    
    Index2dC Index() const 
    { return BufferAccess2dIterC<DataT>::Index(array.ReferenceElm()); }
    //: Get index of current location of the middle pixel.
    // Has to be calculate, and so is slightly slow.
    
  protected:
    Array2dC<DataT> array;
    DataT *up;
    DataT *dn;
  };
}



#endif
