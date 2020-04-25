// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BYTEVYUVALUE_HEADER
#define RAVL_BYTEVYUVALUE_HEADER 1
/////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Image/Base/ByteVYUValue.hh"
//! lib=RavlImage
//! userlevel=Normal
//! author="Charles Galambos"
//! date="24/01/2001"
//! docentry="Ravl.API.Images.Pixel Types.YUV"

#include "Ravl/Image/VYUValue.hh"
#include "Ravl/Stream.hh"

namespace RavlImageN {
  
  //! userlevel=Normal
  //: Byte VYU value class.
  
  class ByteVYUValueC
  {
  public:
    ByteVYUValueC()
    {}
    //: Default constructor.
    // creates an undefined VYU pixel.
    
    ByteVYUValueC(SByteT xv,ByteT xy,SByteT xu)
      : v(xv),
	y(xy),
	u(xu)
    {}
    //: Construct from components.
    
    template<class OCompT>
    ByteVYUValueC(VYUValueC<OCompT> &oth) {
      v = oth.V();
      y = oth.Y();
      u = oth.U();
    }
    //: Construct from another component type.

    void Set(const SByteT &nv,const ByteT &ny,const SByteT &nu) {
      v =nv;
      y =ny;
      u =nu;
    }
    //: Set the values.
    
    inline const SByteT & V() const
    { return v; }
    //: Returns the level of the V component.
    
    inline const ByteT & Y() const
    { return y; }
    //: Returns the level of the Y component.
    
    inline const SByteT & U() const
    { return u; }
    //: Returns the level of the U component.
    
    
    inline SByteT & V()
    { return v; }
    //: Returns the level of the V component.
    
    inline ByteT & Y() 
    { return y; }
    //: Returns the level of the Y component.
    
    inline SByteT & U()
    { return u; }
    //: Returns the level of the U component.
    

    operator VYUValueC<RealT> () const
    { return VYUValueC<RealT>((RealT)v,(RealT)y,(RealT)u); }
    //: Convert to real values.
  protected:
    SByteT v;
    ByteT  y;
    SByteT u;
  };

  inline
  std::istream &operator>>(std::istream &strm,ByteVYUValueC &val) { 
    int u,v,y;
    strm >> v >> y >> u;
    val.V() = v; 
    val.Y() = y;
    val.U() = u;
    return strm;
  }
  //: Stream input.
  
  inline
  std::ostream &operator<<(std::ostream &strm,const ByteVYUValueC &val) 
  { return strm << ((int) val.V()) << ' ' << ((int) val.Y()) << ' ' << ((int) val.U()); }
  //: Stream output.
  
  inline
  ByteVYUValueC Average(const ByteVYUValueC &d1,const ByteVYUValueC &d2,
			const ByteVYUValueC &d3,const ByteVYUValueC &d4) { 
    return ByteVYUValueC(((IntT) d1.V() + (IntT) d2.V() + (IntT) d3.V() + (IntT) d4.V())/4,
			 ((IntT) d1.Y() + (IntT) d2.Y() + (IntT) d3.Y() + (IntT) d4.Y())/4,
			 ((IntT) d1.U() + (IntT) d2.U() + (IntT) d3.U() + (IntT) d4.U())/4
			 );
  }
  //: Average of 4 values.

  inline
  ByteVYUValueC Average(const ByteVYUValueC &d1,const ByteVYUValueC &d2) { 
    return ByteVYUValueC(((IntT) d1.V() + (IntT) d2.V())/2,
			 ((IntT) d1.Y() + (IntT) d2.Y())/2,
			 ((IntT) d1.U() + (IntT) d2.U())/2
			 );
  }
  //: Average of 2 values.
  
  template<class DataT> class ImageC;
  
  BinOStreamC &operator<<(BinOStreamC &out,const ImageC<ByteVYUValueC> &img);
  //: Save byte image to binary stream 
  
  BinIStreamC &operator>>(BinIStreamC &in,ImageC<ByteVYUValueC> &img);  
  //: Load byte image from binary stream 

}


namespace RavlN {
  
  //! userlevel=Advanced
  //: Traits for type
  
  template<>
  struct NumericalTraitsC<RavlImageN::ByteVYUValueC > {
    typedef RavlImageN::VYUValueC<NumericalTraitsC<ByteT>::AccumT > AccumT;    //: Type to use for accumulator, guarantee's at least 2x no bits for interger types.
    typedef RavlImageN::VYUValueC<NumericalTraitsC<ByteT>::RealAccumT > RealAccumT; //: Type to use for a floating point accumulator.
    typedef RavlImageN::VYUValueC<NumericalTraitsC<ByteT>::LongAccumT > LongAccumT; //: Type to use for accumulators that can take large sums.(10000's of elements at least.)
  };
}

#endif
