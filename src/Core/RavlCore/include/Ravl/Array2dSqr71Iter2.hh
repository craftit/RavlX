// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_ARRAY2DSQR71ITER2_HEADER
#define RAVL_ARRAY2DSQR71ITER2_HEADER 1
//////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Core.Arrays.2D"
//! author="Charles Galambos"
//! lib=RavlCore
//! file="Ravl/Core/Container/Array/Array2dSqr71Iter2.hh"

#include "Ravl/Array2d.hh"
#include "Ravl/BfAcc2Iter2.hh"

namespace RavlN {

  //! userlevel=Normal
  //: Iterate through an array accessing 7x7 area, 1 pixel in a second array.
  
  template<class Data1T,class Data2T>
  class Array2dSqr71Iter2C 
    : protected BufferAccess2dIter2C<Data1T,Data2T>
  {
  public:
    Array2dSqr71Iter2C()
    {}
    //: Default constructor.
    
    Array2dSqr71Iter2C(const Array2dC<Data1T> &narray1,const Array2dC<Data2T> &narray2) 
      : array1(narray1),
	array2(narray2)
    { First(); }
    //: Constructor.
    
    bool First() {
      // Setup second index size for the two arrays.
      this->rng1 = IndexRangeC(array1.Range2().Min()+3,array1.Range2().Max()-3);
      this->rng2 = array2.Range2();
      this->rng2.ClipBy(this->rng1);
      // Setup first index for the two arrays.
      IndexRangeC srng1(array1.Range1().Min()+3,array1.Range1().Max()-3);
      IndexRangeC srng2 = array2.Range1();
      srng2.ClipBy(srng1);
      if(!this->rit.First(array1,srng1,
                          array2,srng2)) {
	this->cit.Invalidate();
	return false;
      }
      this->cit.First(this->rit.Data1(),this->rng1,
                      this->rit.Data2(),this->rng2);
      if(!this->cit) {
	this->cit.Invalidate();
	return false;
      }
      up3 = &((&(this->rit.Data1()))[-3][this->rng1.Min()]);
      up2 = &((&(this->rit.Data1()))[-2][this->rng1.Min()]);
      up1 = &((&(this->rit.Data1()))[-1][this->rng1.Min()]);
      dn1 = &((&(this->rit.Data1()))[ 1][this->rng1.Min()]);
      dn2 = &((&(this->rit.Data1()))[ 2][this->rng1.Min()]);
      dn3 = &((&(this->rit.Data1()))[ 3][this->rng1.Min()]);
      return true;
    }
    //: Goto first element in the array.
    
    bool Next() { 
      up3++;
      up2++;
      up1++;
      dn1++;
      dn2++;
      dn3++;
      this->cit++;
      if(this->cit)
	return true;
      this->rit++;
      if(!this->rit)
	return false;
      up3 = &((&(this->rit.Data1()))[-3][this->rng1.Min()]);
      up2 = &((&(this->rit.Data1()))[-2][this->rng1.Min()]);
      up1 = &((&(this->rit.Data1()))[-1][this->rng1.Min()]);
      dn1 = &((&(this->rit.Data1()))[ 1][this->rng1.Min()]);
      dn2 = &((&(this->rit.Data1()))[ 2][this->rng1.Min()]);
      dn3 = &((&(this->rit.Data1()))[ 3][this->rng1.Min()]);
      this->cit.First(this->rit.Data1(),this->rng1,
                      this->rit.Data2(),this->rng2);
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

    Data1T *DataU3() 
    { return up3; }
    //: Access upper row
    // Use with [] operator. Only use index's from -3 to 3. 
    
    Data1T *DataU2() 
    { return up2; }
    //: Access upper row
    // Use with [] operator. Only use index's from -3 to 3. 
    
    Data1T *DataU1() 
    { return up1; }
    //: Access upper row
    // Use with [] operator. Only use index's from -3 to 3. 
    
    Data1T *DataM0() 
    { return &this->Data1(); }
    //: Access middle row
    // Use with [] operator. Only use index's from -3 to 3. 
    
    Data1T *DataD1() 
    { return dn1; }
    //: Access lower row
    // Use with [] operator. Only use index's from -3 to 3. 
    
    Data1T *DataD2() 
    { return dn2; }
    //: Access lowest row
    // Use with [] operator. Only use index's from -3 to 3. 

    Data1T *DataD3() 
    { return dn3; }
    //: Access lowest row
    // Use with [] operator. Only use index's from -3 to 3. 
    
    Data2T &Data2() 
    { return this->cit.Data2(); }
    //: Access data from second array.
    
    const Data2T &Data2() const
    { return this->cit.Data2(); }
    //: Access data from second array.
    
  protected:
    Array2dC<Data1T> array1;
    Array2dC<Data2T> array2;
    Data1T *up3;
    Data1T *up2;
    Data1T *up1;
    Data1T *dn1;
    Data1T *dn2;
    Data1T *dn3;
  };
}



#endif
