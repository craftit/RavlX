// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_RBFACC3D_HEADER
#define RAVL_RBFACC3D_HEADER 1
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Core/Container/Buffer/RBfAcc3d.hh"
//! lib=RavlCore
//! userlevel=Normal
//! author="Charles Galambos"
//! date="24/01/2001"
//! docentry="Ravl.API.Core.Arrays.Buffer"

#include "Ravl/RBfAcc.hh"
#include "Ravl/RBfAcc2d.hh"
#include "Ravl/Index3d.hh"
#include "Ravl/IndexRange3d.hh"
#include "Ravl/BfAcc3Iter.hh"
#include "Ravl/Types.hh"

namespace RavlN {
  
  //! userlevel=Advanced
  //: Access to 3d buffer.
  
  template <class DataT>
  class RangeBufferAccess3dC 
    : public RangeBufferAccessC<BufferAccessC<BufferAccessC<DataT > > >
  {
  public:
    RangeBufferAccess3dC()
      : rng2(0,-1),
	rng3(0,-1)
    {}
    //: Default constructor.

    RangeBufferAccess3dC(const IndexRangeC &nrng2,const IndexRangeC &nrng3)
      : rng2(nrng2),
	rng3(nrng3)
    {}
    //: Constructor.

    RangeBufferAccess3dC(const RangeBufferAccessC<BufferAccessC<BufferAccessC<DataT> > > &ab,
			 const IndexRange3dC &frame)
      : RangeBufferAccessC<BufferAccessC<BufferAccessC<DataT> > >(ab,frame.Range1()),
	rng2(frame.Range2()),
	rng3(frame.Range3())
    {}
    //: Construct a access to a frame within 'ab' with indexs 'rect'.
    
    inline bool Contains(const Index3dC & i) const
    { return Range1().Contains(i.I()) && Range2().Contains(i.J()) && Range3().Contains(i.J()); }
    //: Returns true if there is an item of the �D array
    
    inline DataT & operator[](const Index3dC & i) { 
      RavlAssertMsg(rng2.Contains(i.J()),"j index out of range");
      RavlAssertMsg(rng3.Contains(i.K()),"k index out of range");
      return RangeBufferAccessC<BufferAccessC<BufferAccessC<DataT> > >::operator[](i.I())[i.J()][i.K()]; 
    }
    //: access to the item array[(i)]
    
    inline const DataT & operator[](const Index3dC & i) const { 
      RavlAssertMsg(rng2.Contains(i.J()),"j index out of range");
      RavlAssertMsg(rng3.Contains(i.K()),"k index out of range");
      return RangeBufferAccessC<BufferAccessC<BufferAccessC<DataT> > >::operator[](i.I())[i.J()][i.K()]; 
    }
    //: return the item array[(i)]

    inline RangeBufferAccess2dC<DataT> operator[](const IntT &i)
    { return RangeBufferAccess2dC<DataT>(RangeBufferAccessC<BufferAccessC<BufferAccessC<DataT> > >::operator[](i),rng2,rng3); }
    //: access to the item array[(i)]

    inline const RangeBufferAccess2dC<DataT> operator[](const IntT &i) const
    { return RangeBufferAccess2dC<DataT>(RangeBufferAccessC<BufferAccessC<BufferAccessC<DataT> > >::operator[](i),rng2,rng3); }
    //: return the item array[(i)]

    inline RangeBufferAccess2dC<DataT> operator[](const IndexC &i)
    { return RangeBufferAccess2dC<DataT>(RangeBufferAccessC<BufferAccessC<BufferAccessC<DataT> > >::operator[](i),rng2,rng3); }
    //: access to the item array[(i)]
    
    inline const RangeBufferAccess2dC<DataT> operator[](const IndexC &i) const
    { return RangeBufferAccess2dC<DataT>(RangeBufferAccessC<BufferAccessC<BufferAccessC<DataT> > >::operator[](i),rng2,rng3); }
    //: return the item array[(i)]

    inline RangeBufferAccess2dC<DataT> operator[](const SizeC &i)
    { return RangeBufferAccess2dC<DataT>(RangeBufferAccessC<BufferAccessC<BufferAccessC<DataT> > >::operator[](i),rng2,rng3); }
    //: access to the item array[(i)]

    inline const RangeBufferAccess2dC<DataT> operator[](const SizeC &i) const
    { return RangeBufferAccess2dC<DataT>(RangeBufferAccessC<BufferAccessC<BufferAccessC<DataT> > >::operator[](i),rng2,rng3); }
    //: return the item array[(i)]

    inline const IndexRangeC &Range1() const
    { return RangeBufferAccessC<BufferAccessC<BufferAccessC<DataT > > >::Range(); }
    //: Range of first index.
    
    inline const IndexRangeC &Range2() const
    { return rng2; }
    //: Range of second index.
    
    inline const IndexRangeC &Range3() const
    { return rng3; }
    //: Range of second index.
    
    IndexRange3dC Frame() const
    { return IndexRange3dC(Range1(),Range2(),Range3()); }
    //: Return ranges of indexes
    
    IndexRange3dC Range() const
    { return IndexRange3dC(Range1(),Range2(),Range3()); }
    //: Return ranges of indexes
    
    void Fill(const DataT &d);
    //: Fill array with value.
    
    SizeT Size() const
    { return Range1().Size() * Range2().Size() * Range3().Size(); }
    //: Get the total number of elements in the array.
    
  protected:
    IndexRangeC rng2;
    IndexRangeC rng3;
    
  };

  template<class DataT>
  void RangeBufferAccess3dC<DataT>::Fill(const DataT &d) {
    for(BufferAccess3dIterC<DataT> it(*this,rng2,rng3);it;it++)
      *it = d;
  }

  
  template <class DataT>
  std::ostream & operator<<(std::ostream & s, const RangeBufferAccess3dC<DataT> & arr) {
    for(BufferAccess3dIterC<DataT> it(arr,arr.Range2(),arr.Range3());it;) {
      s << *it;
      for(;it.Next();) 
	s << ' ' << *it;
      s << '\n';
    }    
    return s;
  }
  
  template <class DataT>
  std::istream & operator>>(std::istream & s, RangeBufferAccess3dC<DataT> & arr) {
    for(BufferAccess3dIterC<DataT> it(arr,arr.Range2(),arr.Range3());it;it++) 
      s >> *it;
    return s;
  }

  template<class DataT>
  BinOStreamC &operator<<(BinOStreamC & s, const RangeBufferAccess3dC<DataT> & arr) {
    for(BufferAccess3dIterC<DataT> it(arr,arr.Range2(),arr.Range3());it;it++)
      s << *it;
    return s;
  }
  
  template<class DataT>
  BinIStreamC &operator>>(BinIStreamC & s, RangeBufferAccess3dC<DataT> & arr) {
    for(BufferAccess3dIterC<DataT> it(arr,arr.Range2(),arr.Range3());it;it++)
      s >> *it;
    return s;
  }
  
}

#endif
