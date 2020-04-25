// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_ARRAY2DSQR2ITER2_HEADER
#define RAVL_ARRAY2DSQR2ITER2_HEADER 1
//////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Core.Arrays.2D"
//! author="Charles Galambos"
//! lib=RavlCore
//! file="Ravl/Core/Container/Array/Array2dSqr2Iter2.hh"

#include "Ravl/Array2d.hh"
#include "Ravl/BfAcc2Iter2.hh"

namespace RavlN {

  //! userlevel=Normal
  //: Iterator which slides a 2x2 window over two arrays.
  // The square can be accessed with DataBL,DataBR,DataUL and DataUR
  // which access the following array elements. <p>
  //  TL TR <br>
  //  BL BR <br>
  
  template<class Data1T,class Data2T>
  class Array2dSqr2Iter2C 
    : protected BufferAccess2dIter2C<Data1T,Data2T>
  {
  public:
    Array2dSqr2Iter2C()
    {}
    //: Default constructor.
    
    Array2dSqr2Iter2C(const Array2dC<Data1T> &narray1,const Array2dC<Data2T> &narray2) 
      : array1(narray1),
	array2(narray2)
    { 
      First(); 
    }
    //: Constructor.

    Array2dSqr2Iter2C(const Array2dC<Data1T> &narray1,const Array2dC<Data2T> &narray2,const IndexRange2dC &rng) 
      : array1(narray1,rng),
	array2(narray2,rng)
    { 
      First(); 
    }
    //: Constructor.
    
    bool First() { 
      IndexRangeC nrng1(array1.Range2().Min()+1,array1.Range2().Max());
      IndexRangeC nrng2(array2.Range2().Min()+1,array2.Range2().Max());
      RavlAssert(nrng1.Min() <= nrng1.Max());
      RavlAssert(nrng2.Min() <= nrng2.Max());
      if(!BufferAccess2dIter2C<Data1T,Data2T>::First(array1,nrng1,
						     array2,nrng2))
	return false;
      up1 = &(this->cit.Data1());
      up2 = &(this->cit.Data2());
      if(!this->NextRow()) {
	this->cit.Invalidate();
	return false;
      }
      return true;
    }
    //: Goto first element in the array.
    
    bool Next() { 
      up1++;
      up2++;
      this->cit++;
      if(this->cit)
	return true;
      up1 = &(this->rit.Data1()[this->rng1.Min()]);
      up2 = &(this->rit.Data2()[this->rng2.Min()]);
      this->rit++;
      if(!this->rit)
	return false;
      this->cit.First(this->rit.Data1(),this->rng1,
                      this->rit.Data2(),this->rng2);
      return false;
    }
    //: Goto next element.
    // Return true if pixel is on the same row.
    
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
    
    Data1T &DataBR1() 
    { return this->cit.Data1(); }
    //: Access bottom right data element 

    const Data1T &DataBR1() const
    { return this->cit.Data1(); }
    //: Access bottom right data element 

    Data1T &DataBL1() 
    { return (&(this->cit.Data1()))[-1]; }
    //: Access bottom left data element 

    const Data1T &DataBL1() const
    { return (&(this->cit.Data1()))[-1]; }
    //: Access bottom left data element 
    
    Data1T &DataTR1() 
    { return *up1; }
    //: Access upper right data element 
    
    const Data1T &DataTR1() const
    { return *up1; }
    //: Access upper right data element
    
    Data1T &DataTL1() 
    { return up1[-1]; }
    //: Access upper left data element.
    
    const Data1T &DataTL1() const
    { return up1[-1]; }
    //: Access upper left data element
    
    Data2T &DataBR2() 
    { return this->cit.Data2(); }
    //: Access bottom right data element 

    const Data2T &DataBR2() const
    { return this->cit.Data2(); }
    //: Access bottom right data element 

    Data2T &DataBL2() 
    { return (&(this->cit.Data2()))[-1]; }
    //: Access bottom left data element 

    const Data2T &DataBL2() const
    { return (&(this->cit.Data2()))[-1]; }
    //: Access bottom left data element 
    
    Data2T &DataTR2() 
    { return *up2; }
    //: Access upper right data element 
    
    const Data2T &DataTR2() const
    { return *up2; }
    //: Access upper right data element
    
    Data2T &DataTL2() 
    { return up2[-1]; }
    //: Access upper left data element.
    
    const Data2T &DataTL2() const
    { return up2[-1]; }
    //: Access upper left data element
    
    Index2dC Index() const { 
      RavlAssert(array1.IsValid());
      return Index2dC((IntT) (&(this->rit.Data1()) - array1.ReferenceElm()),
		      (IntT) (&(this->cit.Data1()) - this->rit.Data1().ReferenceElm()));
    }
    //: Get index of current location.
    // Has to be calculate, and so is slightly slow.
    
  protected:
    Array2dC<Data1T> array1;
    Array2dC<Data2T> array2;
    Data1T *up1;
    Data2T *up2;
  };
}



#endif
