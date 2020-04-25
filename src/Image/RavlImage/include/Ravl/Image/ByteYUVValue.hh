// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BYTEYUVVALUE_HEADER
#define RAVL_BYTEYUVVALUE_HEADER 1
/////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Image/Base/ByteYUVValue.hh"
//! lib=RavlImage
//! userlevel=Normal
//! author="Charles Galambos"
//! date="24/01/2001"
//! docentry="Ravl.API.Images.Pixel Types.YUV"

#include "Ravl/Image/YUVValue.hh"
#include "Ravl/Stream.hh"

namespace RavlImageN {
#if RAVL_VISUALCPP_NAMESPACE_BUG
  using namespace RavlN;
#endif
  class RealYUVValueC;
  
  //! userlevel=Normal
  //: Byte YUV value class.
  
  class ByteYUVValueC
  {
  public:
    ByteYUVValueC()
      {}
    //: Default constructor.
    // creates an undefined YUV pixel.
    
    ByteYUVValueC(ByteT xy,SByteT xu, SByteT xv)
      : y(xy),
	u(xu),
	v(xv)
      {}
    //: Construct from components.
    
    template<class OCompT>
    ByteYUVValueC(YUVValueC<OCompT> &oth) {
      y = static_cast<ByteT>(oth.Y());
      u = static_cast<SByteT>(oth.U());
      v = static_cast<SByteT>(oth.V());
    }
    //: Construct from another component type.

    ByteYUVValueC(YUVValueC<RealT> &oth) {
      y = static_cast<ByteT>(Round(oth.Y()));
      u = static_cast<SByteT>(Round(oth.U()));
      v = static_cast<SByteT>(Round(oth.V()));
    }
    //: Construct from real value.
    
    ByteYUVValueC(YUVValueC<float> &oth) {
      y = static_cast<ByteT>(Round(oth.Y()));
      u = static_cast<SByteT>(Round(oth.U()));
      v = static_cast<SByteT>(Round(oth.V()));
    }
    //: Construct from a float value

    void Set(const ByteT &ny,const SByteT &nu,const SByteT &nv) {
      y =ny;
      u =nu;
      v =nv;
    }
    //: Set the values.

    void LimitYUV(const ByteT &minY,const ByteT &maxY,const SByteT &minUV,const SByteT &maxUV) {
      if(this->y < minY)
        this->y = minY;
      if(this->y > maxY)
        this->y = maxY;
      
      if(this->u < minUV)
        this->u = minUV;
      if(this->u > maxUV)
        this->u = maxUV;
      
      if(this->v < minUV)
        this->v = minUV;
      if(this->v > maxUV)
        this->v = maxUV;
    }
    //: Limit colour values.
    
    inline const ByteT & Y() const
    { return y; }
    //: Returns the level of the Y component.
    
    inline const SByteT & U() const
    { return u; }
    //: Returns the level of the U component.
    
    inline const SByteT & V() const
    { return v; }
    //: Returns the level of the V component.
    
    inline ByteT & Y() 
    { return y; }
    //: Returns the level of the Y component.
    
    inline SByteT & U()
    { return u; }
    //: Returns the level of the U component.
    
    inline SByteT & V()
    { return v; }
    //: Returns the level of the V component.
    
    operator YUVValueC<RealT> () const
    { return YUVValueC<RealT>((RealT)y,(RealT)u,(RealT)v); }
    //: Convert to real values.
  protected:
    ByteT  y;
    SByteT u;
    SByteT v;
  };

  inline
  std::istream &operator>>(std::istream &strm,ByteYUVValueC &val) { 
    int u,v,y;
    strm >> y >> u >> v;
    val.Y() = y;
    val.U() = u;
    val.V() = v; 
    return strm;
  }
  //: Stream input.
  
  inline
  std::ostream &operator<<(std::ostream &strm,const ByteYUVValueC &val) 
  { return strm << ((int) val.Y()) << ' ' << ((int) val.U()) << ' '  << ((int) val.V()); }
  //: Stream output.
  
  
  inline
  ByteYUVValueC Average(const ByteYUVValueC &d1,const ByteYUVValueC &d2,
			const ByteYUVValueC &d3,const ByteYUVValueC &d4) { 
    return ByteYUVValueC(((IntT) d1.Y() + (IntT) d2.Y() + (IntT) d3.Y() + (IntT) d4.Y())/4,
			 ((IntT) d1.U() + (IntT) d2.U() + (IntT) d3.U() + (IntT) d4.U())/4,
			 ((IntT) d1.V() + (IntT) d2.V() + (IntT) d3.V() + (IntT) d4.V())/4
			 );
  }
  //: Average of 4 values.

  inline
  ByteYUVValueC Average(const ByteYUVValueC &d1,const ByteYUVValueC &d2) { 
    return ByteYUVValueC(((IntT) d1.Y() + (IntT) d2.Y())/2,
			 ((IntT) d1.U() + (IntT) d2.U())/2,
			 ((IntT) d1.V() + (IntT) d2.V())/2
			 );
  }
  //: Average of 2 values.
  
  template<class DataT> class ImageC;
  
  BinOStreamC &operator<<(BinOStreamC &out,const ImageC<ByteYUVValueC> &img);
  //: Save byte image to binary stream 
  
  BinIStreamC &operator>>(BinIStreamC &in,ImageC<ByteYUVValueC> &img);  
  //: Load byte image from binary stream 
  
  BinOStreamC &operator<<(BinOStreamC &out,const ByteYUVValueC &img);
  //: Save pixel to binary stream
  
  BinIStreamC &operator>>(BinIStreamC &in,ByteYUVValueC &img);
  //: Load pixel from binary stream
  
}


namespace RavlN {
  
  //! userlevel=Advanced
  //: Traits for type
  
  template<>
  struct NumericalTraitsC<RavlImageN::ByteYUVValueC > {
    typedef RavlImageN::YUVValueC<UInt16T> AccumT;    //: Type to use for accumulator, guarantee's at least 2x no bits for interger types.
    typedef RavlImageN::RealYUVValueC RealAccumT; //: Type to use for a floating point accumulator.
    typedef RavlImageN::YUVValueC<Int64T> LongAccumT; //: Type to use for accumulators that can take large sums.(10000's of elements at least.)
  };
}

#endif
