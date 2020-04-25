// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_TVECTOR_HEADER
#define RAVL_TVECTOR_HEADER 1
///////////////////////////////////////////////////
//! userlevel=Normal
//! docentry="Ravl.API.Math.Linear Algebra"
//! rcsid="$Id$"
//! file="Ravl/Math/LinearAlgebra/General/TVector.hh"
//! author="Charles Galambos"
//! date="10/09/1998"
//! lib=RavlMath

#include "Ravl/SArray1d.hh"
#include "Ravl/Slice1d.hh"
#include "Ravl/TFVector.hh"
#include "Ravl/VectorUtils.hh"

namespace RavlN {
  template<class DataT> class TMatrixC;
  
  //! userlevel=Advanced
  //: Templated vectors.
  
  template<class DataT>
  class TVectorC 
    : public SArray1dC<DataT>
  {
  public:
    inline TVectorC()
    {}
    //: Default constructor.
    
    inline TVectorC(const SArray1dC<DataT> &oth)
      : SArray1dC<DataT>(oth)
    {}
    //: Constructor for array of DataT's.

    template<typename OtherT>
    explicit TVectorC(const SArray1dC<OtherT> &oth)
      : SArray1dC<DataT>(oth.Size())
    {
      for(unsigned i = 0;i < oth.Size();i++)
        (*this)[i] = oth[i];
    }
    //: Construct from an array of a different ype.

    inline TVectorC(const Slice1dC<DataT> &oth,bool alwaysCopy = true)
      : SArray1dC<DataT>(oth,alwaysCopy)
    {}
    //: Construct from a slice.
    
    inline TVectorC(SizeT size,DataT *data,bool useCopy,bool manageMemory = false)
      : SArray1dC<DataT>(BufferC<DataT>(size,data,useCopy,manageMemory),size)
    {}
    //: Constructor.
    // This allows 'data' to be used in the array.  
    // If 'useCopy' is true the 'manageMemory' flag has no effect.
    
    explicit inline TVectorC(SizeT n)
      : SArray1dC <DataT>(n)
    {}
    //: Constructor.
    
    TVectorC(const BufferC<DataT> &buf,SizeT size)
      : SArray1dC<DataT>(buf,size)
    {}
    //: Construct from buffer
    
    TVectorC(const BufferC<DataT> & vv,DataT *start, const SizeT dim)
      : SArray1dC<DataT>(vv,start,dim)
    {}
    //: Creates the array of size 'dim' using the buffer 'bf'.
    // Use buffer 'bf',  make access of 'dim' elements. 
    // the start of the buffer should be 'start'.
    
#if !RAVL_COMPILER_VISUALCPP6
    template<unsigned int N>
    inline TVectorC(const TFVectorC<DataT,N> &dat)
      : SArray1dC<DataT>(N)
    {
      const DataT *at = &(dat[0]);
      for(BufferAccessIterC<DataT> it(*this);it;it++,at++)
        *it = *at;
    }
    //: Create from a fixed size vector.
    
    template<unsigned int N>
    operator TFVectorC<DataT,N> () const {
      RavlAssertMsg(N == this->Size(),"Size mismatch converting to fixed size array. ");
      TFVectorC<DataT,N> ret;
      DataT *at = &(ret[0]);
      for(BufferAccessIterC<DataT> it(*this);it;it++,at++)
        *at = *it;
      return ret;
    }
    //: Convert to a fixed size vector.
#endif
    
    DataT Product() const;      
    //: Returns the product of all elements of the vector.
    
    DataT SumOfSqr() const;
    //: Returns the sum of the squares of all the elements of the vector.
    
    DataT SumOfAbs() const;
    //: Returns the sum of the absolute values of all the elements of the vector.
    
    DataT MaxValue() const;
    //: Largest value in the array.
    
    DataT MaxMagnitude() const;
    //: Value of the largest magnitude in the vector.
    
    DataT MaxAbsValue() const
    { return MaxMagnitude(); }
    //: Value of the largest magnitude in the vector.
    //: This is an alias for MaxMagnitude.
    
    DataT MinValue() const;
    //: Smallest value in the array.
    
    DataT MinAbsValue() const;
    //: Smallest absolute value in the array.
    
    const TVectorC<DataT> & Reciprocal();
    //: All elements of the vector are changed to their reciprocal values. 
    //: It is assumed that all elements of the vector differs from zero.
    
    DataT Modulus() const
    { return Sqrt(SumOfSqr()); } 
    //: Returns the modulus of the vector.
    // The Sqrt(SumOfSqr()).
    
    DataT TMul(const TVectorC<DataT> & b) const
    { return Dot(b); }
    //: multiplication 'DataT' = (*this).T() * b
    
    DataT Dot(const TVectorC<DataT> & v) const;         
    //: scalar product of vectors    
    
    DataT Dot2(const TVectorC<DataT> & v1, const TVectorC<DataT> &v2) const;
    //: returns this * v1 + this^2 * v2

    void ElemMul(const TVectorC<DataT> &v2,TVectorC<DataT> &result) const {
      RavlAssert(v2.Size() == this->Size());
      if(result.Size() != this->Size())
        result = SArray1dC<DataT>(this->Size());
      for(BufferAccessIter3C<DataT,DataT,DataT> it(result,*this,v2);it;it++)
        it.Data1() = it.Data2() * it.Data3();
    }
    //: Return the element wise product of this vector times v2.

    void ElemSum(const TVectorC<DataT> &v2,TVectorC<DataT> &result) const {
      RavlAssert(v2.Size() == this->Size());
      if(result.Size() != this->Size())
        result = SArray1dC<DataT>(this->Size());
      for(BufferAccessIter3C<DataT,DataT,DataT> it(result,*this,v2);it;it++)
        it.Data1() = it.Data2() + it.Data3();
    }
    //: Return the element wise sum of v2 and this vector.

    void ElemSubtract(const TVectorC<DataT> &v2,TVectorC<DataT> &result) const {
      RavlAssert(v2.Size() == this->Size());
      if(result.Size() != this->Size())
        result = SArray1dC<DataT>(this->Size());
      for(BufferAccessIter3C<DataT,DataT,DataT> it(result,*this,v2);it;it++)
        it.Data1() = it.Data2() - it.Data3();
    }
    //: Return the element wise of v2 subtracted from this vector.

    const TVectorC<DataT> &SetSmallToBeZero(const DataT &min);
    //: Set values smaller than 'min' to zero in vector.
    
    TMatrixC<DataT> OuterProduct(const TVectorC<DataT> &a) const;
    //: Calculate the outer product of this vector and a.
    // To use the function you must also include 'Ravl/Matrix.hh'.
    
    TMatrixC<DataT> OuterProduct(const TVectorC<DataT> &a,DataT b) const;
    //: Calculate the outer product of this vector and a multiplied by b.
    // To use the function you must also include 'Ravl/Matrix.hh'.
    
    TMatrixC<DataT> OuterProduct() const;
    //: Calculate the outer product of this vector with itself.
    // To use the function you must also include 'Ravl/Matrix.hh'.
    
    TVectorC<DataT> Unit() const
    { return (*this) / Modulus(); }
    //: Return a unit vector
   
    const TVectorC<DataT> &MakeUnit()
    { (*this) = Unit(); return *this; }
    //: Make this a unit vector.
    
    const TVectorC<DataT> &MulAdd(const TVectorC<DataT> & i,DataT a);
    //: Multiply i by a and add it to this vector.
    // Returns a reference to this vector.
    
    //:-
    // Distance calculations
    // ---------------------
    
    DataT MaxValueDistance(const TVectorC<DataT> & i) const;
    //: Returns the distance of two indexes in maximum value metric.
    
    DataT CityBlockDistance(const TVectorC<DataT> & i) const;
    //: Returns the distance of two indexes in absolute value metric.
    
    DataT SqrEuclidDistance(const TVectorC<DataT> & i) const;
    //: Returns the distance of two indexes in square Euclid metric.
    
    DataT EuclidDistance(const TVectorC<DataT> & i) const;
    //: Returns the distance of two indexes in square Euclid metric.
    
    IndexC MaxIndex() const;
    //: Find the index with the most positive valued index.
    
    IndexC MaxAbsIndex() const;
    //: Find the index with the absolute maximum valued index.

    IndexC MinIndex() const;
    //: Find the index with the most negative valued index.
    
    IndexC MinAbsIndex() const;
    //: Find the index with the absolute minimum valued index.
  };
  
  ////////////////////////////////////////

  template<class DataC>
  ostream &operator<<(ostream & s, const TVectorC<DataC> & arr) {
    // This uses spaces instead of '\n' which makes vectors more readable.
    // otherwise its no different from the SArray1dC version.
    s << arr.Size() << ' '; 
    for(BufferAccessIterC<DataC> it(arr);it;it++)
      s << ((const DataC &) *it) << ' ';
    return s;
  }
  
  template<class DataT>
  DataT TVectorC<DataT>::Product() const  {
    DataT prod;
    BufferAccessIterC<DataT> it(*this);
    if(!it) {
      prod = 1;
      return prod; // Or throw an exception ?
    }
    prod = StdCopy(*it);
    for(it++;it;it++)
      prod *= *it;
    return prod;
  }
  
  template<class DataT>
  const TVectorC<DataT> &TVectorC<DataT>::Reciprocal() {
    for(BufferAccessIterC<DataT> it(*this);it;it++)
      it.Data() = 1/it.Data();
    return *this;
  }

  template<class DataT>
  DataT TVectorC<DataT>::SumOfSqr() const {
    DataT ret;
    BufferAccessIterC<DataT> it(*this);
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
  DataT TVectorC<DataT>::SumOfAbs() const {
    DataT ret;
    SetZero(ret);
    for(BufferAccessIterC<DataT> it(*this);it;it++)
      ret += Abs(*it);
    return ret;
  }
  
  template<class DataT>
  DataT TVectorC<DataT>::Dot(const TVectorC<DataT> & v) const {
    RavlAssert(v.Size() == this->Size());
    DataT sum;
    BufferAccessIter2C<DataT,DataT> it(*this,v);
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
  const TVectorC<DataT> &TVectorC<DataT>::SetSmallToBeZero(const DataT &min) {
    for(BufferAccessIterC<DataT> it(*this);it;it++)
      if(Abs(*it) < min)
	SetZero(*it);
    return *this;
  }


  template<class DataT>
  DataT TVectorC<DataT>::MaxValue() const {
    DataT max;
    BufferAccessIterC<DataT> it(*this);
    if(!it.IsElm()) {
      SetZero(max);
      return max;
    }
    max = *it;
    for(it++;it;it++) 
      if(*it > max)
	max = *it;
    return max;
  }

  template<class DataT>
  DataT TVectorC<DataT>::MaxMagnitude() const {
    DataT max;
    BufferAccessIterC<DataT> it(*this);
    if(!it.IsElm()) {
      SetZero(max);
      return max;
    }
    max = Abs(*it);
    for(it++;it;it++) {
      DataT mag = Abs(*it);
      if(mag > max)
	max = mag;
    }
    return max;    
  }
  
  template<class DataT>
  DataT TVectorC<DataT>::MinValue() const {
    BufferAccessIterC<DataT> it(*this);
    DataT min;
    if(!it.IsElm()) {
      SetZero(min);
      return min;
    }
    min = *it;
    for(it++;it;it++) 
      if(*it < min)
	min = *it;
    return min;
  }

  template<class DataT>
  DataT TVectorC<DataT>::MinAbsValue() const {
    DataT min;
    BufferAccessIterC<DataT> it(*this);
    if(!it.IsElm()) {
      SetZero(min);
      return min;
    }
    min = Abs(*it);
    for(it++;it;it++) {
      DataT mag = Abs(*it);
      if(mag < min)
	min = mag;
    }
    return min;    
  }
  
  template<class DataT>
  IndexC TVectorC<DataT>::MaxIndex() const {
    IndexC ind = 0;
    BufferAccessIterC<DataT> it(*this);
    if(!it) return ind;
    DataT maxVal = *it;
    for(it++;it;it++) {
      if(*it > maxVal) {
	maxVal = *it;
	ind = (IntT) (&(*it) - &(*this)[0]);
      }
    }
    return ind;
  }
  
  template<class DataT>
  IndexC TVectorC<DataT>::MaxAbsIndex() const {
    IndexC ind = 0;
    BufferAccessIterC<DataT> it(*this);
    if(!it) return ind;
    DataT maxVal = *it;
    for(it++;it;it++) {
      DataT aM = Abs(*it);
      if(aM > maxVal) {
	maxVal = aM;
	ind = (IntT) (&(*it) - &(*this)[0]);
      }
    }
    return ind;
  }
  
  template<class DataT>
  IndexC TVectorC<DataT>::MinIndex() const {
    IndexC ind = 0;
    BufferAccessIterC<DataT> it(*this);
    if(!it) return ind;
    DataT minVal = *it;
    for(it++;it;it++) {
      if(*it < minVal) {
	minVal = *it;
	ind = (IntT) (&(*it) - &(*this)[0]);
      }
    }
    return ind;
  }
  
  template<class DataT>
  IndexC TVectorC<DataT>::MinAbsIndex() const {
    IndexC ind = 0;
    BufferAccessIterC<DataT> it(*this);
    if(!it) return ind;
    DataT minVal = *it;
    for(it++;it;it++) {
      DataT aM = Abs(*it);
      if(aM < minVal) {
	minVal = aM;
	ind = (IntT) (&(*it) - &(*this)[0]);
      }
    }
    return ind;
  }

  template<class DataT>
  DataT TVectorC<DataT>::MaxValueDistance(const TVectorC<DataT> & i) const {
    RavlAssert(i.Size() == this->Size());
    DataT max;
    BufferAccessIter2C<DataT,DataT> it(*this,i);
    if(!it) {
      SetZero(max);
      return max;
    }
    max = Abs(it.Data1() - it.Data2());
    for(it++;it;it++) {
      DataT tmp = Abs(it.Data1() - it.Data2());
      if(tmp > max)
	max =tmp;
    }
    return max;  
  }
  
  template<class DataT>
  DataT TVectorC<DataT>::CityBlockDistance(const TVectorC<DataT> & i) const {
    RavlAssert(i.Size() == this->Size());
    DataT sum;
    BufferAccessIter2C<DataT,DataT> it(*this,i);
    if(!it) {
      SetZero(sum);
      return sum;
    }
    sum = Abs(it.Data1() - it.Data2());
    for(it++;it;it++)
      sum += Abs(it.Data1() - it.Data2());
    return sum;  
  }
  
  template<class DataT>
  DataT TVectorC<DataT>::SqrEuclidDistance(const TVectorC<DataT> & i) const {
    RavlAssert(i.Size() == this->Size());
    DataT sum;
    BufferAccessIter2C<DataT,DataT> it(*this,i);
    if(!it) {
      SetZero(sum);
      return sum;
    }
    sum = Sqr(it.Data1() - it.Data2());
    for(it++;it;it++)
      sum += Sqr(it.Data1() - it.Data2());
    return sum;  
  }

  template<class DataT>
  inline 
  DataT TVectorC<DataT>::EuclidDistance(const TVectorC<DataT> & i) const 
  { return Sqrt(SqrEuclidDistance(i)); }
  
  template<class DataT>
  const TVectorC<DataT> &TVectorC<DataT>::MulAdd(const TVectorC<DataT> & i,DataT a) {
    for(BufferAccessIter2C<DataT,DataT> it(*this,i);it;it++)
      it.Data1() += it.Data2() * a;
    return *this;
  }
  
  //: Template specialisation for dot product
  
  template<>
  inline RealT TVectorC<RealT>::Dot(const TVectorC<RealT> & v) const {
    RavlAssert(v.Size() == this->Size());
    return RavlBaseVectorN::DotProduct(this->DataStart(), v.DataStart(), this->Size());  
  }
  
  template<>
  inline FloatT TVectorC<FloatT>::Dot(const TVectorC<FloatT> & v) const {
    RavlAssert(v.Size() == this->Size());
    return RavlBaseVectorN::DotProduct(this->DataStart(), v.DataStart(), this->Size());  
  }

  template<>
  inline RealT TVectorC<RealT>::Dot2(const TVectorC<RealT> & v1,
		                     const TVectorC<RealT> & v2) const
  {
     RavlAssert(v1.Size() == this->Size());
     RavlAssert(v2.Size() == this->Size());
     return RavlBaseVectorN::QuadProduct(this->DataStart(), v1.DataStart(), v2.DataStart(), this->Size());
  }

  template<>
  inline FloatT TVectorC<FloatT>::Dot2(const TVectorC<FloatT> & v1,
		                       const TVectorC<FloatT> & v2) const
  {
    RavlAssert(v1.Size() == this->Size());
    RavlAssert(v2.Size() == this->Size());
    return RavlBaseVectorN::QuadProduct(this->DataStart(), v1.DataStart(), v2.DataStart(), this->Size());
  }

}
#endif
