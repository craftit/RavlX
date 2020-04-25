// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_TSMATRIXPARTIAL_HEADER
#define RAVL_TSMATRIXPARTIAL_HEADER 1
///////////////////////////////////////////////////
//! rcsid="$Id$"
//! date="15/8/2002"
//! lib=RavlMath
//! docentry="Ravl.API.Math.Linear Algebra.Smart Matrix"
//! file="Ravl/Math/LinearAlgebra/General/TSMatrixPartial.hh"

#include "Ravl/TSMatrix.hh"
#include "Ravl/SArray1dIter2.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Partial matrix body.
  // This matrix is not intended to be used directly. It serves
  // as a base class for triangular and diagonal matrices
  
  template<class DataT>
  class TSMatrixPartialBodyC
    : public TSMatrixBodyC<DataT>
  {
  public:
    TSMatrixPartialBodyC(UIntT i,UIntT j,const SArray1dC<DataT> &ndata)
      : TSMatrixBodyC<DataT>(i,j),
	data(ndata)
    {}
    //: Construct a partial matrix.
    
    TSMatrixPartialBodyC(UIntT i,UIntT j)
      : TSMatrixBodyC<DataT>(i,j)
    {}
    //: Construct a partial matrix.

    TSMatrixPartialBodyC()
    {}
    //: Default constructor.
    
    virtual const type_info &MatrixType() const
    { return typeid(void); }
    //: Find the type of the matrix.
    
    virtual void AddIP(const TSMatrixC<DataT> &oth);
    //: Add this matrix to 'oth' and return the result.
    
    virtual void SubIP(const TSMatrixC<DataT> &oth);
    //: Subtract 'oth' from this matrix and return the result.
    
    virtual void SetSmallToBeZero(const DataT &min);
    //: Set values smaller than 'min' to zero in vector.
    
    virtual DataT SumOfAbs() const;
    //: Sum the absolute values of all members of the matrix.
    
    virtual void Fill(const DataT &dat);
    //: Fill matrix with values.
    
    SArray1dC<DataT> &Data()
    { return data; }
    //: Access data.
    
    const SArray1dC<DataT> &Data() const
    { return data; }
    //: Access data.
    
  protected:
    SArray1dC<DataT> data; // Values in matrix 
  };
  
  //! userlevel=Normal
  //: Partial matrix.
  // This matrix is not intended to be used directly. It serves
  // as a base class for triangular and diagonal matrices
  
  template<class DataT>
  class TSMatrixPartialC
    : public TSMatrixC<DataT>
  {
  public:
    TSMatrixPartialC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    TSMatrixPartialC(const TSMatrixC<DataT> &oth)
      : TSMatrixC<DataT>(oth)
    {}
    //: Base constructor.
    // Creates an invalid handle if matrix is not of this type.
    
  protected:
    TSMatrixPartialC(TSMatrixPartialBodyC<DataT> &bod)
      : TSMatrixC<DataT>(bod)
    {}
    //: Body constructor.
    
    TSMatrixPartialC(const TSMatrixPartialBodyC<DataT> *bod)
      : TSMatrixC<DataT>(bod)
    {}
    //: Body constructor.
    
    TSMatrixPartialBodyC<DataT> &Body()
    { return static_cast<TSMatrixPartialBodyC<DataT> &>(TSMatrixC<DataT>::Body()); }
    //: Access body.
    
    const TSMatrixPartialBodyC<DataT> &Body() const
    { return static_cast<const TSMatrixPartialBodyC<DataT> &>(TSMatrixC<DataT>::Body()); }
    //: Access body.
    
  public:
    SArray1dC<DataT> &Data()
    { return Body().Data(); }
    //: Access data.
    
    const SArray1dC<DataT> &Data() const
    { return Body().Data(); }
    //: Access data.
    
  };
  
  template<class DataT>
  void TSMatrixPartialBodyC<DataT>::AddIP(const TSMatrixC<DataT> &oth) {
    RavlAssert(this->Size() == oth.Size());
    if(MatrixType() != oth.MatrixType()) {
      TSMatrixBodyC<DataT>::AddIP(oth);
      return ;
    }
    TSMatrixPartialC<DataT> mp(oth);
    for(SArray1dIter2C<DataT,DataT> it(data,mp.Data());it;it++)
      it.Data1() += it.Data2();    
  }
  
  template<class DataT>
  void TSMatrixPartialBodyC<DataT>::SubIP(const TSMatrixC<DataT> &oth) {
    RavlAssert(this->Size() == oth.Size());
    if(MatrixType() != oth.MatrixType()) {
      TSMatrixBodyC<DataT>::SubIP(oth);
      return ;
    }
    TSMatrixPartialC<DataT> mp(oth);
    for(SArray1dIter2C<DataT,DataT> it(data,mp.Data());it;it++)
      it.Data1() -= it.Data2();
  }
  
  template<class DataT>
  void TSMatrixPartialBodyC<DataT>::SetSmallToBeZero(const DataT &min) {
    for(SArray1dIterC<DataT> it(data);it;it++)
      if(Abs(*it) < min)
	SetZero(*it);
  }
  
  template<class DataT>
  DataT TSMatrixPartialBodyC<DataT>::SumOfAbs() const {
    SArray1dIterC<DataT> it(data);
    DataT ret = StdCopy(*it);
    for(it++;it;it++)
      ret += *it;
    return ret;
  }
  
  template<class DataT>
  void TSMatrixPartialBodyC<DataT>::Fill(const DataT &dat) {
    data.Fill(dat);
  }
  
}

#endif
