// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_INDEX3D_HEADER
#define RAVL_INDEX3D_HEADER
///////////////////////////////////////////////////////////////////
//! file="Ravl/Core/Base/Index3d.hh"
//! lib=RavlCore
//! userlevel=Normal
//! author="Radek Marik"
//! docentry="Ravl.API.Core.Indexing"
//! date="2/8/1994"
//! rcsid="$Id$"

#include "Ravl/FIndex.hh" 
#include "Ravl/Assert.hh"

namespace RavlN {
  
  //: Index of a 3-D array
  // The class Index3dC is a triple of integer values. It is usually used
  // as a three dimensional index of three dimensional arrays. <p>
  // SMALL OBJECT.
  
  class Index3dC 
    : public FIndexC<3>
  {
  public:
    
    //:----------------------------------------------
    // Constructor, assigment, copies, and destructor
    
    explicit Index3dC(SizeT dim)
    { RavlAssert(dim == 3); }
    //: Creates an index
    // This is for compatibilty with N-D indexes, which 
    // need to be told there dimentionality.
    
    inline Index3dC(IndexC i = 0, IndexC j = 0, IndexC k = 0)
    { Set(i, j, k); }
    // Creates index <i,j,k>.
    
    inline Index3dC(RealT ir, RealT jr, RealT kr) {
      Set(IntT(ir),
	  IntT(jr),
	  IntT(kr));
    }
    // Creates index <i,j,k> where i,j,k are cut value of ir, jr, kr 
    // respectivily.
    
    inline void Set(IndexC i, IndexC j, IndexC k) {
      I() = i;
      J() = j;
      K() = k;
    }
    // Sets this index to be <i,j,k>.
    
    inline void Set(const Index3dC & index)
    { *this = index; }
    // Sets this index to be equal to 'index'.
    
    //:--------------------------
    // Access to the member items
    
    inline IndexC I() const
    { return (*this)[0]; }
    // Returns the first index.
    
    inline IndexC J() const
    { return (*this)[1]; }
    // Returns the second index.
    
    inline IndexC K() const
    { return (*this)[2]; }
    // Returns the third index.
    
    inline IndexC & I()
    { return (*this)[0]; }
    // Access to the first index.
    
    inline IndexC & J()
    { return (*this)[1]; }
    // Access to the second index.
    
    inline IndexC & K()
    { return (*this)[2]; }
    // Access to the third index.    
    
    //:-------------------------------------
    // Access to the neighbouring 3D indexes
    
    inline Index3dC & Right();
    // Shifts the index to the right.
    
    inline Index3dC & Left();
    // Shifts the index to the left.
    
    inline Index3dC & Up();
    // Shifts the index to the up.
    
    inline Index3dC & Down();
    // Shifts the index to the down.
    
    inline Index3dC RightN() const;
    // Returns the index of the right neighbour.
    
    inline Index3dC LeftN() const;
    // Returns the index of the left neighbour.
    
    inline Index3dC UpN() const;
    // Returns the index of the upper neighbour.
    
    inline Index3dC DownN() const;
    // Returns the index of the down neighbour.
    
  };
  
  template<>
  inline
  IndexC TFVectorC<IndexC,3>::SqrEuclidDistance(const TFVectorC<IndexC,3> & o) const 
  { return RavlN::Sqr(data[0] - o[0]) + RavlN::Sqr(data[1] - o[1]) + RavlN::Sqr(data[2] - o[2]); }
  
  template<>
  inline 
  TFVectorC<IndexC,3> TFVectorC<IndexC,3>::operator+(const TFVectorC<IndexC,3> & o) const {
    TFVectorC<IndexC,3> ret;
    ret[0] = data[0] + o[0];
    ret[1] = data[1] + o[1];
    ret[2] = data[2] + o[2];
    return ret;
  }
  //: Loop unrolled add.
  
  template<>
  inline 
  TFVectorC<IndexC,3> TFVectorC<IndexC,3>::operator-(const TFVectorC<IndexC,3> & o) const {
    TFVectorC<IndexC,3> ret;
    ret[0] = data[0] - o[0];
    ret[1] = data[1] - o[1];
    ret[2] = data[2] - o[2];
    return ret;
  }
  //: Loop unrolled subtract.
  
  template<>
  inline 
  bool TFVectorC<IndexC,3>::operator==(const TFVectorC<IndexC,3> & ind) const 
  { return (data[0] == ind[0]) && (data[1] == ind[1]) && (data[2] == ind[2]);  }
  
  template<>
  inline 
  bool TFVectorC<IndexC,3>::operator!=(const TFVectorC<IndexC,3> & ind) const 
  { return (data[0] != ind[0]) || (data[1] != ind[1]) || (data[2] != ind[2]);  }
  
  inline Index3dC &Index3dC::Right() {
    J()++;
    return *this;
  }
  
  inline Index3dC &Index3dC::Left() {
    J()--;
    return *this;
  }
  
  inline Index3dC &Index3dC::Up() {
    I()--;
    return *this;
  }
  
  inline Index3dC &Index3dC::Down() {
    I()++;
    return *this;
  }
  
  inline Index3dC Index3dC::RightN() const {
    Index3dC id(*this);
    id.Right();
    return(id);
  }
  
  inline Index3dC Index3dC::LeftN() const {
    Index3dC id(*this);
    id.Left();
    return id;
  }

  inline Index3dC Index3dC::UpN() const {
    Index3dC id(*this);
    id.Up();
    return id;
  }
  
  inline Index3dC Index3dC::DownN() const {
    Index3dC id(*this);
    id.Down();
    return id;
  }
  
}  
#endif
  
