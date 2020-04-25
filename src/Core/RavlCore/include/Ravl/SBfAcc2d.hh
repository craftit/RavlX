// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SBFACC2D_HEADER
#define RAVL_SBFACC2D_HEADER 1
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Core/Container/Buffer/SBfAcc2d.hh"
//! lib=RavlCore
//! userlevel=Develop
//! author="Charles Galambos"
//! date="24/01/2001"
//! docentry="Ravl.API.Core.Arrays.Buffer"

#include "Ravl/SBfAcc.hh"
#include "Ravl/BfAcc2Iter.hh"
#include "Ravl/Index2d.hh"
#include "Ravl/Types.hh"
#include "Ravl/IndexRange2d.hh"

namespace RavlN {
  
  class BinOStreamC;
  class BinIStreamC;
  
  //: Basic access to buffer with limited size
  // The class SizeBufferAccessC enables to random indexed access to
  // a sequentially organised continuous part of memory called buffer.
  // The access functions check if an accessed element is valid only in
  // debug mode.
  
  template <class DataT>
  class SizeBufferAccess2dC 
    : public SizeBufferAccessC<BufferAccessC<DataT> >
  {
  public:
    SizeBufferAccess2dC(SizeT nsize2)
      : size2(nsize2)
    {}
    //: Constructor.
    
    SizeBufferAccess2dC(const SizeBufferAccessC<BufferAccessC<DataT> > &ab,SizeT nsize2)
      : SizeBufferAccessC<BufferAccessC<DataT> >(ab),
	size2(nsize2)
    {}
    //: Constructor.
    
    SizeBufferAccess2dC(const BufferAccessC<BufferAccessC<DataT> > &ab,SizeT nsize1,SizeT nsize2)
      : SizeBufferAccessC<BufferAccessC<DataT> >(ab,nsize1),
	size2(nsize2)
    {}
    //: Constructor.

    SizeBufferAccess2dC(const SizeBufferAccess2dC<DataT > &ab,SizeT nsize1,SizeT nsize2)
      : SizeBufferAccessC<BufferAccessC<DataT> >(ab,nsize1),
	size2(nsize2)
    {
      RavlAssert(nsize1 <= ab.Size1());
      RavlAssert(nsize2 <= ab.Size2());
    }
    //: Constructor.
    
    SizeBufferAccess2dC()
      : size2(0)
    {}
    //: Default constructor.
    
    inline bool Contains(const Index2dC &i) const { 
      return (((UIntT) i.Col().V()) < size2) && 
	(((UIntT) i.Row().V()) < Size1()); 
    }
    //: Does this buffer contain the index i ?
    // Returns true if yes.
    
    inline DataT & operator[](const Index2dC & i) { 
#if RAVL_CHECK
      if (((size_t) i.Col().V()) >= size2)
	IssueError(__FILE__,__LINE__,"Index %d out of index range 0-%zu  ",i.Col().V(),static_cast<size_t>(size2-1));
#endif
      return SizeBufferAccessC<BufferAccessC<DataT> >::operator[](i.Row())[i.Col()]; 
    }
    //: access to the item array[(i)]
    
    inline const DataT & operator[](const Index2dC & i) const { 
#if RAVL_CHECK
      if (((size_t) i.Col().V()) >= size2)
	IssueError(__FILE__,__LINE__,"Index %d out of index range 0-%u  ",i.Col().V(),static_cast<size_t>(size2-1));
#endif
      return SizeBufferAccessC<BufferAccessC<DataT> >::operator[](i.Row())[i.Col()]; 
    }
    //: return the item array[(i)]
    
    inline SizeBufferAccessC<DataT> operator[](const IndexC &i)
    { return SizeBufferAccessC<DataT>(SizeBufferAccessC<BufferAccessC<DataT> >::operator[](i),size2); }
    //: access to the item array[(i)]
    
    inline const SizeBufferAccessC<DataT> operator[](const IndexC &i) const
    { return SizeBufferAccessC<DataT>(SizeBufferAccessC<BufferAccessC<DataT> >::operator[](i),size2); }
    //: return the item array[(i)]

    inline SizeBufferAccessC<DataT> operator[](const SizeC &i)
    { return SizeBufferAccessC<DataT>(SizeBufferAccessC<BufferAccessC<DataT> >::operator[](i),size2); }
    //: access to the item array[(i)]

    inline const SizeBufferAccessC<DataT> operator[](const SizeC &i) const
    { return SizeBufferAccessC<DataT>(SizeBufferAccessC<BufferAccessC<DataT> >::operator[](i),size2); }
    //: return the item array[(i)]

    inline SizeBufferAccessC<DataT> operator[](int i)
    { return SizeBufferAccessC<DataT>(SizeBufferAccessC<BufferAccessC<DataT> >::operator[](i),size2); }
    //: access to the item array[(i)]
    
    inline const SizeBufferAccessC<DataT> operator[](int i) const
    { return SizeBufferAccessC<DataT>(SizeBufferAccessC<BufferAccessC<DataT> >::operator[](i),size2); }
    //: return the item array[(i)]
    
    inline SizeT Size1() const
    { return SizeBufferAccessC<BufferAccessC<DataT> >::Size(); }
    //: Size.
    
    inline SizeT Size2() const
    { return size2; }
    //: Size.
    
    SizeT Size() const 
    { return Size1() * Size2(); }
    //: Get the total number of elements in the array.
    
    void Fill(const DataT &d);
    //: Fill array with value.
    
    IntT Stride() const {
      if(Size1() <= 1)
	return size2;
      return (IntT) (SizeBufferAccessC<BufferAccessC<DataT> >::operator[](1).ReferenceElm() -  
		     SizeBufferAccessC<BufferAccessC<DataT> >::operator[](0).ReferenceElm());
    }
    //: Get the stride of the 2d array. 
    
    bool IsContinuous() const 
    { return Stride() == (IntT) size2; }
    //: Test if the array is allocated in a continuous area of memory.
    // Note: this only checks the first two rows follow each other in
    // memory, this may miss other discontinuities.
    
    IndexRange2dC Frame() const
    { return IndexRange2dC(0,(IntT) Size1()-1,0,(IntT) size2-1); }
    //: Return ranges of indexes

    BufferAccessC<DataT> &RowPtr(IndexC i)
    { return SizeBufferAccessC<BufferAccessC<DataT> >::operator[](i); }
    //: Access element 0 of row i.
    // Note: The element may not actually exist. <p>
    // Advanced users only!
    
    void SetRowPtr(IndexC i,const BufferAccessC<DataT> &rowPtr)
    { SizeBufferAccessC<BufferAccessC<DataT> >::operator[](i) = rowPtr; }
    //: Set element 0 of row i to 'rowPtr'.
    // Advanced users only! <p>
    // This directly manipulates the row pointer table. It can be used to
    // change the way the array is stored. NOTE: Many of the numerical operations
    // in RAVL assume the data is stored in linearly in a contiguous chunk of
    // memory. Manipulating the layout with this method will break this method
    // will break this assumption.  This method is provided to allow operations
    // like deinterlacing image to be done without copying large amounts of memory.
    
  protected:
    SizeT size2;
  };

  
  template<class DataT>
  void SizeBufferAccess2dC<DataT>::Fill(const DataT &d) {
    for(BufferAccess2dIterC<DataT> it(*this,size2);it;it++)
      *it = d;
  }
  
  template <class DataT>
  std::ostream & operator<<(std::ostream & s, const SizeBufferAccess2dC<DataT> & arr) {
    for(BufferAccess2dIterC<DataT> it(arr,arr.Size2());it;) {
      s << *it;
      for(;it.Next();) 
	s << ' ' << *it;
      s << '\n';
    }    
    return s;
  }
  
  template <class DataT>
  std::istream & operator>>(std::istream & s, SizeBufferAccess2dC<DataT> & arr) {
    for(BufferAccess2dIterC<DataT> it(arr,arr.Size2());it;it++) 
      s >> *it;
    return s;
  }
  
  template<class DataT>
  BinOStreamC &operator<<(BinOStreamC & s, const SizeBufferAccess2dC<DataT> & arr) {
    for(BufferAccess2dIterC<DataT> it(arr,arr.Size2());it;it++)
      s << *it;
    return s;
  }
  
  template<class DataT>
  BinIStreamC &operator>>(BinIStreamC & s, SizeBufferAccess2dC<DataT> & arr) {
    for(BufferAccess2dIterC<DataT> it(arr,arr.Size2());it;it++)
      s >> *it;
    return s;
  }
  
  //: Save real array to binary stream 
  BinOStreamC &operator<<(BinOStreamC &out,const SizeBufferAccess2dC<RealT> &img);
  
  //: Load real array image from binary stream 
  BinIStreamC &operator>>(BinIStreamC &in,SizeBufferAccess2dC<RealT> &img);  
  
  //: Save float array image to binary stream 
  BinOStreamC &operator<<(BinOStreamC &out,const SizeBufferAccess2dC<FloatT> &img);
  
  //: Load float array image from binary stream 
  BinIStreamC &operator>>(BinIStreamC &in,SizeBufferAccess2dC<FloatT> &img);  
  
  //: Save byte array to binary stream 
  BinOStreamC &operator<<(BinOStreamC &out,const SizeBufferAccess2dC<ByteT> &img);
  
  //: Load byte array from binary stream 
  BinIStreamC &operator>>(BinIStreamC &in,SizeBufferAccess2dC<ByteT> &img);  
  
  
}
#endif
