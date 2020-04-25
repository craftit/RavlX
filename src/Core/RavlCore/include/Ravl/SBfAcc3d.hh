// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SBFACC3D_HEADER
#define RAVL_SBFACC3D_HEADER 1
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Core/Container/Buffer/SBfAcc3d.hh"
//! lib=RavlCore
//! userlevel=Develop
//! author="Charles Galambos"
//! date="24/01/2001"
//! docentry="Ravl.API.Core.Arrays.Buffer"

#include "Ravl/SBfAcc.hh"
#include "Ravl/SBfAcc2d.hh"
#include "Ravl/Index3d.hh"
#include "Ravl/BfAcc3Iter.hh"
#include "Ravl/Types.hh"

namespace RavlN {
  
  class BinOStreamC;
  class BinIStreamC;
  
  //! userlevel=Develop
  //: Access for 3D array.
  
  template <class DataT>
  class SizeBufferAccess3dC 
    : public SizeBufferAccessC<BufferAccessC<BufferAccessC<DataT > > >
  {
  public:
    SizeBufferAccess3dC()
      : size2(0),
	size3(0)
    {}
    //: Default constructor.
    
    SizeBufferAccess3dC(SizeT nsize2,SizeT nsize3)
      : size2(nsize2),
	size3(nsize3)
    {}
    //: Constructor.

    SizeBufferAccess3dC(const SizeBufferAccessC<BufferAccessC<BufferAccessC<DataT> > > &ab,SizeT nsize2,SizeT nsize3)
      : SizeBufferAccessC<BufferAccessC<BufferAccessC<DataT> > >(ab),
	size2(nsize2),
	size3(nsize3)
    {}
    //: Constructor.
    
    inline bool Contains(const Index3dC & i) const { 
      return (((UIntT) i.K().V()) < size3) &&
	(((UIntT) i.J().V()) < size2) && 
	(((UIntT) i.I().V()) < Size1());
    }
    //: Returns true if there is an item of the 3D array
    
    inline DataT & operator[](const Index3dC & i) { 
      RavlAssertMsg((((UIntT) i.K().V()) < size3) && (((UIntT) i.J().V()) < size2),"Index out of range. ");
      return SizeBufferAccessC<BufferAccessC<BufferAccessC<DataT> > >::operator[](i.I())[i.J()][i.K()]; 
    }
    //: access to the item array[(i)]
    
    inline const DataT & operator[](const Index3dC & i) const { 
      RavlAssertMsg((((UIntT) i.K().V()) < size3) && (((UIntT) i.J().V()) < size2),"Index out of range. ");
      return SizeBufferAccessC<BufferAccessC<BufferAccessC<DataT> > >::operator[](i.I())[i.J()][i.K()]; 
    }
    //: return the item array[(i)]
    
    inline SizeBufferAccess2dC<DataT> operator[](const IndexC &i) {
      return SizeBufferAccess2dC<DataT>(BufferAccessC<BufferAccessC<BufferAccessC<DataT > > >::operator[](i),
					size2,size3);
    }
    //: access to the item array[(i)]
    
    inline const SizeBufferAccess2dC<DataT> operator[](const IndexC &i) const {
      return SizeBufferAccess2dC<DataT>(BufferAccessC<BufferAccessC<BufferAccessC<DataT > > >::operator[](i),
					size2,size3);
    }
    //: return the item array[(i)]

    inline SizeBufferAccess2dC<DataT> operator[](const SizeC &i) {
      return SizeBufferAccess2dC<DataT>(BufferAccessC<BufferAccessC<BufferAccessC<DataT > > >::operator[](i),
                                        size2,size3);
    }
    //: access to the item array[(i)]
    
    inline const SizeBufferAccess2dC<DataT> operator[](const SizeC &i) const {
      return SizeBufferAccess2dC<DataT>(BufferAccessC<BufferAccessC<BufferAccessC<DataT > > >::operator[](i),
                                        size2,size3);
    }
    //: return the item array[(i)]

    inline SizeBufferAccess2dC<DataT> operator[](int i) {
      return SizeBufferAccess2dC<DataT>(BufferAccessC<BufferAccessC<BufferAccessC<DataT > > >::operator[](i),
					size2,size3);
    }
    //: access to the item array[(i)]
    
    inline const SizeBufferAccess2dC<DataT> operator[](int i) const {
      return SizeBufferAccess2dC<DataT>(BufferAccessC<BufferAccessC<BufferAccessC<DataT > > >::operator[](i),
					size2,size3);
    }
    //: return the item array[(i)]
    
    SizeT Size1() const
    { return SizeBufferAccessC<BufferAccessC<BufferAccessC<DataT > > >::Size(); }
    //: Get size of dimension 1
    
    SizeT Size2() const
    { return size2; }
    //: Get size of dimension 2
    
    SizeT Size3() const
    { return size3; }
    //: Get size of dimension 3
    
    SizeT Size() const 
    { return Size1() * Size2() * Size3(); }
    //: Get the total number of elements in the array.
    
    void Fill(const DataT &d);
    //: Fill array with value.
    
  protected:
    SizeT size2;
    SizeT size3;
  };

  template<class DataT>
  void SizeBufferAccess3dC<DataT>::Fill(const DataT &d) {
    for(BufferAccess3dIterC<DataT> it(*this,size2,size3);it;it++)
      *it = d;
  }
  
  template <class DataT>
  std::ostream & operator<<(std::ostream & s, const SizeBufferAccess3dC<DataT> & arr) {
    for(BufferAccess3dIterC<DataT> it(arr,arr.Size2(),arr.Size3());it;) {
      s << *it;
      for(;it.Next();) 
	s << ' ' << *it;
      s << '\n';
    }    
    return s;
  }
  
  template <class DataT>
  std::istream & operator>>(std::istream & s, SizeBufferAccess3dC<DataT> & arr) {
    for(BufferAccess3dIterC<DataT> it(arr,arr.Size2(),arr.Size3());it;it++) 
      s >> *it;
    return s;
  }
  
  template<class DataT>
  BinOStreamC &operator<<(BinOStreamC & s, const SizeBufferAccess3dC<DataT> & arr) {
    for(BufferAccess3dIterC<DataT> it(arr,arr.Size2(),arr.Size3());it;it++)
      s << *it;
    return s;
  }
  
  template<class DataT>
  BinIStreamC &operator>>(BinIStreamC & s, SizeBufferAccess3dC<DataT> & arr) {
    for(BufferAccess3dIterC<DataT> it(arr,arr.Size2(),arr.Size3());it;it++)
      s >> *it;
    return s;
  }
  
}

#endif
