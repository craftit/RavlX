// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_VECTOR_HEADER
#define RAVL_VECTOR_HEADER 1
///////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Math/LinearAlgebra/General/Vector.hh"
//! lib=RavlMath
//! userlevel=Normal
//! author="Charles Galambos"
//! date="24/01/2001"
//! docentry="Ravl.API.Math.Linear Algebra"

#include "Ravl/TVector.hh"

namespace RavlN {
  
  template<unsigned int N> class FVectorC;
  
  //! userlevel=Normal
  //: Vector of real values.
  
  class VectorC
    : public TVectorC<RealT>
  {
  public:
    VectorC()
    {}
    //: Default constructor.
    
    explicit VectorC(SizeT size)
      : TVectorC<RealT>(size)
    {}
    //: Constructor.
    // Create a vector of 'size' elements
    
    VectorC(const TVectorC<RealT> &oth)
      : TVectorC<RealT>(oth)
    {}
    //: Base class constructor.
    
    VectorC(const SArray1dC<RealT> &oth)
      : TVectorC<RealT>(oth)
    {}
    //: Base class constructor.

    VectorC(const Slice1dC<RealT> &oth,bool alwaysCopy = true)
      : TVectorC<RealT>(oth,alwaysCopy)
    {}
    //: Construct from a slice 
    
    VectorC(const SArray1dC<FloatT> &oth)
      : TVectorC<RealT>(oth.Size())
    {
      for(BufferAccessIter2C<RealT,FloatT> it(*this,oth);it;it++)
        it.Data1() = static_cast<RealT>(it.Data2());
    }
    //: Convert from a float vector.

    VectorC(const BufferC<RealT> &buf,SizeT size)
      : TVectorC<RealT>(buf,size)
    {}
    //: Base class constructor.
    
    VectorC(const BufferC<RealT> & vv,RealT *start, const SizeT dim)
      : TVectorC<RealT>(vv,start,dim)
    {}
    //: Creates the array of size 'dim' using the buffer 'bf'.
    // Use buffer 'bf',  make access of 'dim' elements. 
    // the start of the buffer should be 'start'.
    
    static VectorC ConstructAligned(const SizeT dim,UIntT align)
    { return VectorC(SingleBufferC<RealT>(dim,align),dim); }
    //: Creates an uninitialised array with the range <0, 'dim'-1> and byte alignment of the first element 'align'
    // align must be a power of 2.

    operator TVectorC<FloatT> () const {
      TVectorC<FloatT> ret(Size());
      for(BufferAccessIter2C<RealT,FloatT> it(*this,ret);it;it++)
        it.Data2() = static_cast<FloatT>(it.Data1());
      return ret;
    }
    //: Convert to a float vector.
    
#if !RAVL_COMPILER_VISUALCPP6
    template<unsigned int N>
    inline VectorC(const TFVectorC<RealT,N> &dat) 
      : TVectorC<RealT>(dat)
    {}
    //: Construct from a fixed size array.
    
    template<unsigned int N>
    operator FVectorC<N> () {
      RavlAssertMsg(N == Size(),"Size mismatch converting to fixed size array. ");
      FVectorC<N> ret;
      RealT *at = &(ret[0]);
      for(BufferAccessIterC<RealT> it(*this);it;it++,at++)
        *at = *it;
      return ret;
    }
    //: Convert to a fixed size vector.
#else
    VectorC(const TFVectorC<RealT,2> &dat)
      : TVectorC<RealT>(2)
    {
      (*this)[0] = dat[0];
      (*this)[1] = dat[1];
    }
    //: Construct from a 2d vector.
    // This is a hack for Visual C++ which doesn't
    // support templated member functions properly.
    
    VectorC(const TFVectorC<RealT,3> &dat)
      : TVectorC<RealT>(3)
    {
      (*this)[0] = dat[0];
      (*this)[1] = dat[1];
      (*this)[2] = dat[2];
    }
    //: Construct from a 3d vector.
    // This is a hack for Visual C++ which doesn't
    // support templated member functions properly.
#endif
    
    VectorC(RealT v1,RealT v2)
      : TVectorC<RealT>(2)
    {
      (*this)[0] = v1;
      (*this)[1] = v2;
    }
    //: Create a vector with two real values.

    VectorC(RealT v1,RealT v2,RealT v3)
      : TVectorC<RealT>(3)
    {
      (*this)[0] = v1;
      (*this)[1] = v2;
      (*this)[2] = v3;
    }
    //: Create a vector with three real values.

    VectorC(RealT v1,RealT v2,RealT v3,RealT v4)
      : TVectorC<RealT>(4)
    {
      (*this)[0] = v1;
      (*this)[1] = v2;
      (*this)[2] = v3;
      (*this)[3] = v4;
    }
    //: Create a vector with four real values.
    
    VectorC(RealT v1,RealT v2,RealT v3,RealT v4,RealT v5)
      : TVectorC<RealT>(5)
    {
      (*this)[0] = v1;
      (*this)[1] = v2;
      (*this)[2] = v3;
      (*this)[3] = v4;
      (*this)[4] = v5;
    }
    //: Create a vector with five real values.

    VectorC(RealT v1,RealT v2,RealT v3,RealT v4,RealT v5,RealT v6)
      : TVectorC<RealT>(6)
    {
      (*this)[0] = v1;
      (*this)[1] = v2;
      (*this)[2] = v3;
      (*this)[3] = v4;
      (*this)[4] = v5;
      (*this)[5] = v6;
    }
    //: Create a vector with six real values.

    
    bool IsReal() const;
    //: Test if vector only contains real values.
    // This will return false if either nan's (Not an number) 
    // or infinite values are found.
    
    void Dump() const;
    //: Dump to cout in a human readable format.

    const VectorC &operator+=(const TVectorC<float> &arr) {
      for(BufferAccessIter2C<RealT,float> it(*this,arr);it;it++)
        it.Data1() += it.Data2();
      return *this;
    }
    //: Add a float vector to a real one.

    const VectorC &operator-=(const TVectorC<float> &arr) {
      for(BufferAccessIter2C<RealT,float> it(*this,arr);it;it++)
        it.Data1() -= it.Data2();
      return *this;
    }
    //: Subtract a float vector from a real one.

    const TVectorC<RealT> &MulAdd(const TVectorC<float> &vec,float scale);
    //: Multiply vec by scale and add it to this one.

    using TVectorC<RealT>::MulAdd;
    using SArray1dC<RealT>::operator+=;
    using SArray1dC<RealT>::operator-=;
  };
  
  VectorC RandomVector(int n,RealT scale = 1);
  //: Create a random vector of values between -scale and scale with the given size.
  
  inline void SetZero(VectorC &vec)
  { vec.Fill(0); }
  //: Fill vector with zero's.
  
  VectorC Sigmoid(const VectorC &z);
  //: Compute the element wise sigmoid of z.

  void SigmoidIP(VectorC &z);
  //: Compute sigmoid values and store the results in place.

  VectorC Log(const VectorC &z);
  //: Compute the element wise log of z.

  void LogIP(VectorC &z);
  //: Compute the element wise log of z and store the results in place.

  VectorC Exp(const VectorC &z);
  //: Compute the element wise exponent of z.

}

#endif
