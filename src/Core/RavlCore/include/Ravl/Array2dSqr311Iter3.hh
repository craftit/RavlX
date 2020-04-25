// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_ARRAY2DSQR311ITER3_HEADER
#define RAVL_ARRAY2DSQR311ITER3_HEADER 1
//////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Core.Arrays.2D"
//! author="Charles Galambos"
//! lib=RavlCore
//! file="Ravl/Core/Container/Array/Array2dSqr311Iter3.hh"

#include "Ravl/Array2d.hh"
#include "Ravl/BfAcc2Iter3.hh"

namespace RavlN {

  //! userlevel=Normal
  //: Iterate through an array accessing 3x3 area, and 1 pixel in two other arrays.
  // The square can be accessed with DataBL,DataBR etc...
  // which access the following array elements. <p>
  //  TL TM TR <br>
  //  ML MM MR <br>
  //  BL BM BR <br>
  // for the first array.
  
  template<class Data1T,class Data2T,class Data3T>
  class Array2dSqr311Iter3C 
    : protected BufferAccess2dIter3C<Data1T,Data2T,Data3T>
  {
  public:
    Array2dSqr311Iter3C()
    {}
    //: Default constructor.
    
    Array2dSqr311Iter3C(const Array2dC<Data1T> &narray1,
			const Array2dC<Data2T> &narray2,
			const Array2dC<Data3T> &narray3) 
      : array1(narray1),
	array2(narray2),
	array3(narray3)
    { First(); }
    //: Constructor.
    
    bool First() {
      this->rng1 = IndexRangeC(array1.Range2().Min()+1,array1.Range2().Max()-1);
      this->rng2 = array2.Range2();
      this->rng3 = array3.Range2();
      this->rng2.ClipBy(this->rng1);
      this->rng3.ClipBy(this->rng1);
      IndexRangeC srng1(array1.Range1().Min()+1,array1.Range1().Max()-1);
      IndexRangeC srng2 = array2.Range1();
      IndexRangeC srng3 = array3.Range1();
      srng2.ClipBy(srng1);
      srng3.ClipBy(srng1);
      if(!this->rit.First(array1,srng1,
		    array2,srng2,
		    array3,srng3)) {
	this->cit.Invalidate();
	return false;
      }
      this->cit.First(this->rit.Data1(),this->rng1,
                      this->rit.Data2(),this->rng2,
                      this->rit.Data3(),this->rng3);
      if(!this->cit) {
	this->cit.Invalidate();
	return false;
      }
      up = &((&(this->rit.Data1()))[-1][this->rng1.Min()]);
      dn = &((&(this->rit.Data1()))[ 1][this->rng1.Min()]);
      return true;
    }
    //: Goto first element in the array.
    
    bool Next() { 
      up++;
      dn++;
      this->cit++;
      if(this->cit)
	return true;
      this->rit++;
      if(!this->rit)
	return false;
      up = &((&(this->rit.Data1()))[-1][this->rng1.Min()]);
      dn = &((&(this->rit.Data1()))[ 1][this->rng1.Min()]);
      this->cit.First(this->rit.Data1(),this->rng1,
                      this->rit.Data2(),this->rng2,
                      this->rit.Data3(),this->rng3);
      return false;
    }
    //: Goto next element.
    
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
    
    Data1T &DataBL1() 
    { return dn[-1]; }
    //: Access bottom left data element 

    const Data1T &DataBL1() const
    { return dn[-1]; }
    //: Access bottom left data element 

    Data1T &DataBM1() 
    { return *dn; }
    //: Access bottom mid data element 

    const Data1T &DataBM1() const
    { return *dn; }
    //: Access bottom mid data element 

    Data1T &DataBR1() 
    { return dn[1]; }
    //: Access bottom right data element 

    const Data1T &DataBR1() const
    { return dn[1]; }
    //: Access bottom right data element 

    Data1T &DataML1() 
    { return (&(this->cit.Data1()))[-1]; }
    //: Access middle left data element 
    
    const Data1T &DataML1() const
    { return (&(this->cit.Data1()))[-1]; }
    //: Access middle left data element 
    
    Data1T &DataMM1() 
    { return this->cit.Data1(); }
    //: Access middle data element 
    
    const Data1T &DataMM1() const
    { return this->cit.Data1(); }
    //: Access middle data element 
    
    Data1T &DataMR1() 
    { return (&(this->cit.Data1()))[1]; }
    //: Access middle right data element 
    
    const Data1T &DataMR1() const
    { return (&(this->cit.Data1()))[1]; }
    //: Access middle right data element 

    Data1T &DataTL1() 
    { return up[-1]; }
    //: Access top left data element.
    
    const Data1T &DataTL1() const
    { return up[-1]; }
    //: Access top left data element

    Data1T &DataTM1() 
    { return *up; }
    //: Access top middle element 
    
    const Data1T &DataTM1() const
    { return *up; }
    //: Access top middle element
    
    Data1T &DataTR1() 
    { return up[1]; }
    //: Access top right data element 
    
    const Data1T &DataTR1() const
    { return up[1]; }
    //: Access top right data element
    
    Data2T &Data2() 
    { return this->cit.Data2(); }
    //: Access middle data element of second array.
    
    const Data2T &Data2() const
    { return this->cit.Data2(); }
    //: Access middle data element of second array.

    Data3T &Data3() 
    { return this->cit.Data3(); }
    //: Access middle data element of 3rd array.
    
    const Data3T &Data3() const
    { return this->cit.Data3(); }
    //: Access middle data element of 3rd array.
    
  protected:
    Array2dC<Data1T> array1;
    Array2dC<Data2T> array2;
    Array2dC<Data3T> array3;
    Data1T *up;
    Data1T *dn;
  };
}



#endif
