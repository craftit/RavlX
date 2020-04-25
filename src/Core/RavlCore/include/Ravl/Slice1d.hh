// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SLICE1D_HEADER
#define RAVL_SLICE1D_HEADER 1
////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! userlevel=Advanced
//! file="Ravl/Core/Container/Buffer/Slice1d.hh"
//! lib=RavlCore
//! docentry="Ravl.API.Core.Arrays"
//! author="Charles Galambos"
//! date="24/01/2001"

#include "Ravl/Buffer.hh"
#include "Ravl/Assert.hh"
#include "Ravl/IndexRange1d.hh"
#include "Ravl/RBfAcc.hh"
#include "Ravl/SBfAcc.hh"

//: Ravl namespace.

namespace RavlN {

  template<class DataT> class Slice1dIterC;
  template<class Data1T,class Data2T> class Slice1dIter2C;
  template<class Data1T,class Data2T,class Data3T> class Slice1dIter3C;
  //:  Slice through array
  // Slices are NOT simple arrays, they have an additional paramiter
  // 'stride' which allows them to access both rows and columns of
  // block matrixes equally.
  // This intended to support numerical types, operations
  // on the vector are implemented as would be expected
  
  template<class DataT>
  class Slice1dC {
  public:

    //:----------------
    // Type definitions.
    
    typedef DataT ElementT;
    //: Allow function templates to find type of array.
    
    typedef IndexC KeyT;
    //: Allow function templates to find type of index.
    
    typedef Slice1dIterC<DataT> IteratorT;
    //: Iterator for this container.
    
    //typedef Array1dIterC<DataT> IteratorT;
    //: Type of iterator.

    //:---------------------------------------------
    // Constructors, copy, assignment, and destructor

    Slice1dC();
    //: Creates a zero length vector.
    
    Slice1dC(SizeT nsize);
    //: Allocate a vector of size 'nsize'
    
    Slice1dC(const IndexRangeC &nrng);
    //: Allocate a Slice with a range of 'nrng'
    
    Slice1dC(BufferC<DataT> &buff,SizeT size,SizeT off,IntT stride = 1);
    //: Attach a vector to a buffer.
    // buff   - Buffer in which data is held.
    // size   - Number of elements in the slice.
    // off    - Offset into buffer of first element.
    // stride - distance between successive elements in slice.
    // Element 0 is at 'off' in buffer, and use the given stride.

    Slice1dC(BufferC<DataT> &buff,DataT *refElm,SizeT size,IntT stride = 1);
    //: Attach a vector to a buffer.
    // buff   - Buffer in which data is held.
    // size   - Number of elements in the slice.
    // refElm - Pointer to first element in the slice.
    // stride - distance between successive elements in slice.
    // Element 0 is at 'refElm' in buffer, and use the given stride.
    
    Slice1dC(BufferC<DataT> &buff,DataT *refElm,IndexRangeC range,IntT stride = 1);
    //: Attach a vector to a buffer.
    // buff   - Buffer in which data is held.
    // refElm - Pointer to element 0 in the slice. (even if its not in 'range')
    // range  - Range of indexes to map data to.
    // stride - distance between successive elements in slice.
    // Element '*refElm' is at index 0 in buffer, and use the given stride.
    
    SizeT Size() const
    { return rng.Size(); }
    //: Size of vector.
    
    bool IsEmpty() const
    { return rng.Size() <= 0; }
    //: Is slice empty ?
    
    Slice1dC Copy() const;
    //: Make copy of vector.
    
    void Fill(const DataT &val);
    //: Fill vector with value 'val'
    
    DataT &operator[](IndexC i);
    //: Access element i in vector.
    
    const DataT &operator[](IndexC i) const;
    //: Constant access to element i in vector.

    DataT &operator[](int i);
    //: Access element i in vector.
    
    const DataT &operator[](int i) const;
    //: Constant access to element i in vector.
    
    DataT &First()
    { return ref[stride * rng.Min().V()]; }
    //: Access first element in the vector.
    // Not the results of this function is undefined if Size() is zero.
    
    const DataT &First() const
    { return ref[stride * rng.Min().V()]; }
    //: Access first element in the vector.
    // Not the results of this function is undefined if Size() is zero.
    
    DataT &Last()
    { return ref[stride * rng.Max().V()]; }
    //: Access last element in vector.
    
    const DataT &Last() const
    { return ref[stride * rng.Max().V()]; }
    //: Access last element in vector.

    IntT Stride() const
    { return stride; }
    //: Access stride of vector.
    
    Slice1dC<DataT> operator+(const Slice1dC<DataT>& b) const;
    //: Returns the sum of two vectors.
    
    Slice1dC<DataT> operator-(const Slice1dC<DataT>& b) const;
    //: Returns the diference between this vector and the vector 'b'.
    
    Slice1dC<DataT> operator*(const DataT &alfa) const;
    //: Returns the multiplication of this vector by the scalar 'alpha'.
  
    Slice1dC<DataT> operator*(const Slice1dC<DataT> & b) const;
    //: Returns the vector whose elements are the results of multiplication
    //: of this vector by the vector 'b' element by element.
    
    Slice1dC<DataT> & operator+=(const Slice1dC<DataT> & b);
    //: add the vector 'b' to the vector
    
    Slice1dC<DataT> & operator+=(const DataT &alpha);
    //: add the scalar to each element of the vector
    
    Slice1dC<DataT> & operator-=(const Slice1dC<DataT> & b);
    //: subtract the vector 'b' from the vector
    
    Slice1dC<DataT> & operator-=(const DataT &alpha);
    //: subtract the scalar from each element of the vector
    
    Slice1dC<DataT> & operator*=(const DataT &alpha);
    //: multiply the vector by the scalar 'alpha'
    
    Slice1dC<DataT> & operator/=(const DataT &alpha);
    //: divide the vector by the scalar 'alpha'
    
    Slice1dC<DataT> & operator/=(const Slice1dC<DataT> &vec);
    //: (*this)[index] = (*this)[index]/vec[index]
    
    DataT Sum() const;      
    //: Returns the sum all elements of the vector.
    
    DataT Product() const;      
    //: Returns the product of all elements of the vector.
    
    DataT SumOfSqr() const;
    //: Returns the sum of the squares of all the elements of the vector.
    
    Slice1dC<DataT> & Reciprocal();
    //: All elements of the vector are changed to their reciprocal values. 
    //: It is assumed that all elements of the vector differs from zero.
    
    RealT Modulus() const
    { return Sqrt(SumOfSqr()); } 
    //: Returns the modulus of the vector.
    // The Sqrt(SumOfSqr()).
    
    DataT Dot(const Slice1dC<DataT> & v) const;         
    //: scalar product of vectors    
    
    DataT TMul(const Slice1dC<DataT> & v) const
    { return Dot(v); }
    //: An alias for dot product.
    
    BufferC<DataT> &Buffer()
    { return buffer; }
    //: Access the raw buffer 
    
    const BufferC<DataT> &Buffer() const
    { return buffer; }
    //: Access the raw buffer.
    
    const IndexRangeC &Range() const
    { return rng; }
    //: Access range of valid indexs.
    
    IndexC IMin() const
    { return rng.Min(); }
    //: Get minimum index.
    
    IndexC IMax() const
    { return rng.Max(); }
    //: Get maximum index.
    
    bool Contains(IndexC i) const
    { return rng.Contains(i); }
    //: Test if slice contains index i.

    DataT &ReferenceElm()
    { return *ref; }
    //: Access refrence element.
    // Advanced users only.
    
    const DataT &ReferenceElm() const
    { return *ref; }
    //: Access refrence element.
    // Advanced users only.
    
    void CopyFrom(const Slice1dC<DataT> &data);
    //: Copy 'data' into this slice.
    // Both must be the same length.
    
    void CopyFrom(const RangeBufferAccessC<DataT> &data);
    //: Copy 'data' into this slice.
    // Both must be the same length.
    
  protected:
    IndexRangeC rng;// Range of valid index's
    IntT stride;    // Stride of data.
    DataT *ref;     // Ptr to element 0
    BufferC<DataT> buffer;
  };

  template<class DataT>
  std::ostream &operator<<(std::ostream &out,const Slice1dC<DataT> &dat);
  //: Write to stream.
  
  template<class DataT>
  std::istream &operator>>(std::istream &out,Slice1dC<DataT> &dat);
  //: Read from stream.

}
//// Slice1dC Impl ////////////////////////////////////////////////////////////////////

#include "Ravl/Slice1dIter.hh"
#include "Ravl/Slice1dIter2.hh"
#include "Ravl/Slice1dIter3.hh"

//: Ravl namespace.

namespace RavlN {


  template<class DataT>
  Slice1dC<DataT>::Slice1dC()
    : rng(0,-1),
      stride(1),
      ref(0)
  {}
  
  template<class DataT>
  Slice1dC<DataT>::Slice1dC(BufferC<DataT> &buff,SizeT nsize,SizeT noff,IntT nstride)
    : rng(nsize),
      stride(nstride),
      ref(&(buff.ReferenceElm()[noff])),
      buffer(buff)
  { 
    RavlAssert(((nsize-1) * stride + noff) < buff.Size()); // Check it fits.
  }

  template<class DataT>
  Slice1dC<DataT>::Slice1dC(BufferC<DataT> &buff,DataT *refElm,SizeT nsize,IntT nstride ) 
    : rng(nsize),
      stride(nstride),
      ref(refElm),
      buffer(buff)
  {
    RavlAssert(((nsize-1) * stride + (IntT)(refElm - buffer.ReferenceElm())) < buff.Size()); // Check it fits.    
  }
  
  template<class DataT>
  Slice1dC<DataT>::Slice1dC(BufferC<DataT> &buff,DataT *refElm,IndexRangeC range,IntT nstride) 
    : rng(range),
      stride(nstride),
      ref(refElm),
      buffer(buff)
  {
    RavlAssert(rng.Min() <= rng.Max());
    RavlAssert(((rng.Min()) * stride + (IntT)(refElm - buffer.ReferenceElm())) >= 0); // Check it fits.    
    RavlAssert(((rng.Max()) * stride + (IntT)(refElm - buffer.ReferenceElm())) < buff.Size()); // Check it fits.    
  }								       
  
  template<class DataT>
  Slice1dC<DataT>::Slice1dC(SizeT nsize) 
    : rng(nsize),
      stride(1),
      buffer(nsize)
  { ref = buffer.ReferenceElm(); }
  
  
  template<class DataT>
  Slice1dC<DataT>::Slice1dC(const IndexRangeC &nrng)
    : rng(nrng),
      stride(1),
      buffer(nrng.Size())
  { ref = buffer.ReferenceElm() - nrng.Min().V(); }
  
  template<class DataT>
  DataT &Slice1dC<DataT>::operator[](IndexC i) { 
    RavlAssertMsg(rng.Contains(i),"Slice1dC, Index out of range.");
    return ref[i.V() * stride]; 
  }
  
  template<class DataT>
  const DataT &Slice1dC<DataT>::operator[](IndexC i) const { 
    RavlAssertMsg(rng.Contains(i),"Slice1dC, Index out of range.");
    return ref[i.V() * stride]; 
  }
  
  template<class DataT>
  DataT &Slice1dC<DataT>::operator[](int i) {
    RavlAssertMsg(rng.Contains(i),"Slice1dC, Index out of range.");
    return ref[i * stride]; 
  }
  
  template<class DataT>
  const DataT &Slice1dC<DataT>::operator[](int i) const {
    RavlAssertMsg(rng.Contains(i),"Slice1dC, Index out of range.");
    return ref[i * stride]; 
  }

  template<class DataT>
  Slice1dC<DataT> Slice1dC<DataT>::Copy() const {
    Slice1dC<DataT> ret(Size());
    for(Slice1dIter2C<DataT,DataT> it(ret,*this);it;it++)
      it.Data1() = it.Data2();
    return ret;
  }
  
  template<class DataT>
  void Slice1dC<DataT>::Fill(const DataT &val) {
    for(Slice1dIterC<DataT> it(*this);it;it++)
      *it = val;
  }
  
  template<class DataT>
  Slice1dC<DataT> Slice1dC<DataT>::operator+(const Slice1dC<DataT>& b) const {
    Slice1dC<DataT> ret(Size());
    for(Slice1dIter3C<DataT,DataT,DataT> it(ret,*this,b);it;it++)
      it.Data1() = it.Data2() + it.Data3();
    return ret;
  }

  template<class DataT>
  Slice1dC<DataT> Slice1dC<DataT>::operator-(const Slice1dC<DataT>& b) const {
    Slice1dC<DataT> ret(Size());
    for(Slice1dIter3C<DataT,DataT,DataT> it(ret,*this,b);it;it++)
      it.Data1() = it.Data2() - it.Data3();
    return ret;  
  }
  
  template<class DataT>
  Slice1dC<DataT> Slice1dC<DataT>::operator*(const DataT &alpha) const {
    Slice1dC<DataT> ret(Size());
    for(Slice1dIter2C<DataT,DataT> it(ret,*this);it;it++)
      it.Data1() = it.Data2() * alpha;
    return ret;
  }
  
  template<class DataT>
  Slice1dC<DataT> Slice1dC<DataT>::operator*(const Slice1dC<DataT> & b) const {
    Slice1dC<DataT> ret(Size());
    for(Slice1dIter3C<DataT,DataT,DataT> it(ret,*this,b);it;it++)
      it.Data1() = it.Data2() * it.Data3();
    return ret;
  }
  
  template<class DataT>
  Slice1dC<DataT> &Slice1dC<DataT>::operator+=(const Slice1dC<DataT> & b) {
    for(Slice1dIter2C<DataT,DataT> it(*this,b);it;it++)
      it.Data1() += it.Data2();
    return (*this);
  }
  
  template<class DataT>
  Slice1dC<DataT> &Slice1dC<DataT>::operator+=(const DataT &alpha) {
    for(Slice1dIterC<DataT> it(*this);it;it++)
      *it += alpha;
    return (*this);
  }
  
  template<class DataT>
  Slice1dC<DataT> &Slice1dC<DataT>::operator-=(const Slice1dC<DataT> & b) {
    for(Slice1dIter2C<DataT,DataT> it(*this,b);it;it++)
      it.Data1() -= it.Data2();
    return (*this);
  }
  
  template<class DataT>
  Slice1dC<DataT> &Slice1dC<DataT>::operator-=(const DataT &alpha) {
    for(Slice1dIterC<DataT> it(*this);it;it++)
      *it -= alpha;
    return (*this);  
  }
  
  template<class DataT>
  Slice1dC<DataT> &Slice1dC<DataT>::operator*=(const DataT &alpha) {
    for(Slice1dIterC<DataT> it(*this);it;it++)
      *it *= alpha;
    return (*this);
  }
  
  template<class DataT>
  Slice1dC<DataT> &Slice1dC<DataT>::operator/=(const DataT &alpha) {
    for(Slice1dIterC<DataT> it(*this);it;it++)
      *it /= alpha;
    return (*this);  
  }
  
  template<class DataT>
  Slice1dC<DataT> &Slice1dC<DataT>::operator/=(const Slice1dC<DataT> &vec) {
    for(Slice1dIter2C<DataT,DataT> it(*this,vec);it;it++)
      it.Data1() /= it.Data2();
    return (*this);
  }
  
  template<class DataT>
  DataT Slice1dC<DataT>::Sum() const {
    Slice1dIterC<DataT> it(*this);
    if(!it) return 0;
    DataT sum = StdCopy(*it);
    for(it++;it;it++)
      sum += *it;
    return sum;
  }
  
  template<class DataT>
  DataT Slice1dC<DataT>::Product() const  {
    Slice1dIterC<DataT> it(*this);
    if(!it) return 1; // Or throw an exception ?
    DataT prod = StdCopy(*it);
    for(it++;it;it++)
      prod *= *it;
    return prod;
  }

  //: Returns the sum of the squares of all the elements of the vector.
  
  template<class DataT>  
  DataT Slice1dC<DataT>::SumOfSqr() const {
    DataT ret;
    Slice1dIterC<DataT> it(*this);
    if(!it) {
      SetZero(ret);
      return ret;
    }
    ret = Sqr(*it);
    for(it++;it;it++)
      ret += Sqr(*it);
    return ret;
  }
  
  template<class DataT>
  Slice1dC<DataT> & 
  Slice1dC<DataT>::Reciprocal() {
    for(Slice1dIterC<DataT> it(*this);it;it++)
      *it = 1/ *it;
    return *this;
  }
  
  template<class DataT>
  DataT Slice1dC<DataT>::Dot(const Slice1dC<DataT> & v) const {
    DataT sum;
    Slice1dIter2C<DataT,DataT> it(*this,v);
    if(!it) {
      SetZero(sum);
      return sum;
    }
    sum = it.Data1() * it.Data2();
    for(it++;it;it++)
      sum += it.Data1() * it.Data2();
    return sum;  
  }
  
  template<class DataT>
  void Slice1dC<DataT>::CopyFrom(const Slice1dC<DataT> &data) {
    RavlAssert(Size() == data.Size());
    for(Slice1dIter2C<DataT,DataT> it(*this,data);it;it++)
      it.Data1() = it.Data2();
  }
  
  template<class DataT>
  void Slice1dC<DataT>::CopyFrom(const RangeBufferAccessC<DataT> &data) {
    RavlAssert(Size() == data.Size());
    if(Size() == 0)
      return ;
    const DataT *at = &(data[data.IMin()]);
    for(Slice1dIterC<DataT> it(*this);it;it++,at++)
      *it = *at;
  }
  
  template<class DataT>
  void RangeBufferAccessC<DataT>::CopyFrom(const Slice1dC<DataT> &slice) {
    RavlAssert(Size() == slice.Size());
    if(Size() == 0)
      return ;
    DataT *at = &((*this)[IMin()]);
    for(Slice1dIterC<DataT> it(slice);it;it++,at++)
      *at = *it;
  }

  template<class DataT>
  void SizeBufferAccessC<DataT>::CopyFrom(const Slice1dC<DataT> &slice) {
    RavlAssert(Size() == slice.Size());
    if(Size() == 0)
      return ;
    DataT *at = &((*this)[0]);
    for(Slice1dIterC<DataT> it(slice);it;it++,at++)
      *at = *it;
  }
  
  template<class DataT>
  std::ostream &operator<<(std::ostream &strm,const Slice1dC<DataT> &dat) {
    strm << dat.Range() << "\n";
    for(Slice1dIterC<DataT> it(dat);it;it++)
      strm << *it << " ";
    return strm;
  }
  
  template<class DataT>
  std::istream &operator>>(std::istream &strm,Slice1dC<DataT> &dat) {
    IndexRangeC s;
    strm >> s;
    Slice1dC<DataT> tmp(s);
    for(Slice1dIterC<DataT> it(tmp);it;it++)
      strm >> *it;
    dat = tmp;
    return strm;
  }
  
  
}

#endif
