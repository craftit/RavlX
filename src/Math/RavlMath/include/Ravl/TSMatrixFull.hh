// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_TSMATRIXFULL_HEADER
#define RAVL_TSMATRIXFULL_HEADER 1
///////////////////////////////////////////////////
//! rcsid="$Id$"
//! date="15/8/2002"
//! lib=RavlMath
//! docentry="Ravl.API.Math.Linear Algebra.Smart Matrix"
//! file="Ravl/Math/LinearAlgebra/General/TSMatrixFull.hh"

#include "Ravl/TSMatrix.hh"
#include "Ravl/TMatrix.hh"
#include "Ravl/Slice1dIter.hh"
#include "Ravl/Slice1dIter2.hh"

#define RAVL_TSMATRIXFULL_DEFAULT 0

namespace RavlN {
  //! userlevel=Develop
  //: Full Matrix Body.
  
  template<class DataT>
  class TSMatrixFullBodyC 
    : public TSMatrixBodyC<DataT>
  {
  public:
    TSMatrixFullBodyC()
    {}
    //: Default constructor.
    
    TSMatrixFullBodyC(UIntT rows,UIntT cols)
      : TSMatrixBodyC<DataT>(rows,cols),
	matrix(rows,cols)
    {}
    //: Create a matrix of rows by cols
    
    TSMatrixFullBodyC(const TMatrixC<DataT> &mat)
      : TSMatrixBodyC<DataT>(mat.Rows(),mat.Cols()),
	matrix(mat)
    {}
    //: Constructor.
    
    virtual RCBodyVC &Copy() const
    { return *new TSMatrixFullBodyC<DataT>(matrix.Copy()); }
    //: Create a copy of this matrix.
    
    virtual const type_info &MatrixType() const
    { return typeid(TSMatrixFullBodyC<DataT>); }
    //: Find the type of the matrix.
    
    virtual DataT Element(UIntT i,UIntT j) const 
    { return matrix[i][j]; } 
    //: Access element.
    
    virtual void Element(UIntT i,UIntT j,const DataT &val) 
    { matrix[i][j] = val; }
    //: Set element.
    
    virtual Array1dC<DataT> Row(UIntT i) const
    { return Array1dC<DataT>(const_cast<TMatrixC<DataT> &>(matrix).SliceRow(i)); }
    //: Access a row from the matrix.
    
    virtual DataT MulSumColumn(UIntT c,const Array1dC<DataT> &dat) const;
    //: Multiply column by values from dat and sum them.
    
    virtual Slice1dC<DataT> Col(UIntT j) const
    { return const_cast<TMatrixC<DataT> &>(matrix).SliceColumn(j); }
    //: Access slice from matrix.
    
    virtual DataT MulSumColumn(UIntT c,const Slice1dC<DataT> &slice) const;
    //: Multiply column by values from slice and sum them.
    
    virtual TSMatrixC<DataT> Add(const TSMatrixC<DataT> &oth) const;
    //: Add this matrix to 'oth' and return the result.
    
    virtual TSMatrixC<DataT> Sub(const TSMatrixC<DataT> &oth) const;
    //: Subtract 'oth' from this matrix and return the result.
    
    virtual void AddIP(const TSMatrixC<DataT> &oth);
    //: Add this matrix to 'oth' and return the result.
    
    virtual void SubIP(const TSMatrixC<DataT> &oth);
    //: Subtract 'oth' from this matrix and return the result.
    
    virtual TSMatrixC<DataT> T() const
    { return TSMatrixC<DataT>(matrix.T()); }
    //: Get transpose of matrix.
    
#if !RAVL_TSMATRIXFULL_DEFAULT    
    virtual TSMatrixC<DataT> Mul(const TSMatrixC<DataT> &oth) const;
    //: Get this matrix times 'oth'.
    
    virtual TVectorC<DataT> Mul(const TVectorC<DataT> &oth) const;
    //: Get this matrix times 'oth'.
    
    virtual TSMatrixC<DataT> MulT(const TSMatrixC<DataT> & B) const;
    //: Multiplication A * B.T()
    
    virtual TSMatrixC<DataT> TMul(const TSMatrixC<DataT> & B) const;
    //: Multiplication A.T() * B
    
    virtual TVectorC<DataT> TMul(const TVectorC<DataT> & B) const;
    //: Multiplication A.T() * B
#endif
    
    virtual TSMatrixC<DataT> AAT() const
    { return matrix.AAT(); }
    //: Return  A * A.T().
    
    virtual TSMatrixC<DataT> ATA() const
    { return matrix.ATA(); }
    //: Return  A.T() * A.
    
    virtual void SetDiagonal(const TVectorC<DataT> &d)
    { matrix.SetDiagonal(d); }
    //: Set the diagonal of this matrix.
    // If d.Size() != Cols() an error is given.
    
    virtual void AddDiagonal(const TVectorC<DataT> &d)
    { matrix.AddDiagonal(d); }
    //: Add a vector to the diagonal of this matrix.
    // If d.Size() != Cols() an error is given.

    void AddOuterProduct(const TVectorC<DataT> &vec1,const TVectorC<DataT> &vec2)
    { matrix.AddOuterProduct(vec1,vec2); }
    //: Add outer product of vec1 and vec2 to this matrix.
    
    void AddOuterProduct(const TVectorC<DataT> &vec1,const TVectorC<DataT> &vec2,const DataT &a)
    { matrix.AddOuterProduct(vec1,vec2,a); }
    //: Add outer product of vec1 and vec2 multiplied by a to this matrix .
    
    void SetSmallToBeZero(const DataT &min)
    { matrix.SetSmallToBeZero(min); }
    //: Set values smaller than 'min' to zero in vector.
    
    DataT SumOfAbs() const
    { return matrix.SumOfAbs(); }
    //: Sum the absolute values of all members of the matrix.
    
    void SwapRows(int i,int j)
    { matrix.SwapRows(i,j); }
    //: Swap two rows in the matrix.
    
    TMatrixC<DataT> TMatrix(bool alwaysCopy) const { 
      if(alwaysCopy) return matrix.Copy();
      return matrix; 
    }
    //: Access as normal matrix.
    
    TMatrixC<DataT> &FullTMatrix()
    { return matrix; }
    //: Access normal matrix.
    
    const TMatrixC<DataT> &FullTMatrix() const
    { return matrix; }
    //: Access normal matrix.
  protected:
    TMatrixC<DataT> matrix;
  };
  
  //! userlevel=Normal
  //: Full SMatrix.
  
  template<class DataT>
  class TSMatrixFullC 
    : public TSMatrixC<DataT>
  {
  public:
    TSMatrixFullC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    TSMatrixFullC(const TMatrixC<DataT> &mat)
      : TSMatrixC<DataT>(*new TSMatrixFullBodyC<DataT>(mat))
    {}
    //: Constructor.
    
    TSMatrixFullC(const TSMatrixC<DataT> &mat)
      : TSMatrixC<DataT>(dynamic_cast<const TSMatrixFullBodyC<DataT> *>(this->BodyPtr(mat)))
    {}
    //: Base constructor.
    
  protected:
    TSMatrixFullC(TSMatrixFullBodyC<DataT> &bod)
      : TSMatrixC<DataT>(bod)
    {}
    //: Body constructor.

    TSMatrixFullC(const TSMatrixFullBodyC<DataT> *bod)
      : TSMatrixC<DataT>(bod)
    {}
    //: Body constructor.
    
    TSMatrixFullBodyC<DataT> &Body()
    { return static_cast<TSMatrixFullBodyC<DataT> &>(TSMatrixC<DataT>::Body()); }
    //: Access body.
    
    const TSMatrixFullBodyC<DataT> &Body() const
    { return static_cast<const TSMatrixFullBodyC<DataT> &>(TSMatrixC<DataT>::Body()); }
    //: Access body.
    
  public:
    TMatrixC<DataT> &FullTMatrix()
    { return Body().FullTMatrix(); }
    //: Access normal matrix.
    
    const TMatrixC<DataT> &FullTMatrix() const
    { return Body().FullTMatrix(); }
    //: Access normal matrix.
  };
  
  template<class DataT>  
  TSMatrixC<DataT> TSMatrixFullBodyC<DataT>::Add(const TSMatrixC<DataT> &oth) const {
    RavlAssert(oth.Rows() == this->Rows() && oth.Cols() == this->Cols());
    TSMatrixFullC<DataT> ts(oth);
    if(ts.IsValid())
      return TSMatrixC<DataT>(matrix + ts.FullTMatrix());
    TMatrixC<DataT> ret(this->Rows(),this->Cols());
    // We know we'll need a full matrix for result.
    IndexC cmax = matrix.Size2()-1;
    for(UIntT i = 0;i < this->Rows();i++) {
      Array1dC<DataT> row2 = oth.Row(i);
      if(row2.Range().Size() <= 0) {
	for(BufferAccessIter2C<DataT,DataT> it(ret[i],matrix[i]);it;it++)
	  it.Data1() = it.Data2();
	continue;
      }
      if(row2.Range().Min() > 0) {
	IndexRangeC rng(0,row2.Range().Min()-1);
	for(BufferAccessIter2C<DataT,DataT> it(ret[i],matrix[i],rng);it;it++)
	  it.Data1() = it.Data2();
      }
      for(BufferAccessIter3C<DataT,DataT,DataT> it(ret[i],matrix[i],row2,row2.Range());it;it++)
	it.Data1() = it.Data2() + it.Data3();
      if(row2.Range().Max() < cmax ) {
	IndexRangeC rng(row2.Range().Max()+1,cmax);
	for(BufferAccessIter2C<DataT,DataT> it(ret[i],matrix[i],rng);it;it++)
	  it.Data1() = it.Data2();
      }
    }
    return TSMatrixC<DataT>(ret);
  }
  
  template<class DataT>  
  TSMatrixC<DataT> TSMatrixFullBodyC<DataT>::Sub(const TSMatrixC<DataT> &oth) const {
    RavlAssert(oth.Rows() == this->Rows() && oth.Cols() == this->Cols());
    TSMatrixFullC<DataT> ts(oth);
    if(ts.IsValid()) 
      return TSMatrixC<DataT>(matrix - ts.FullTMatrix());
    TMatrixC<DataT> ret(this->Rows(),this->Cols());
    // We know we'll need a full matrix for result.
    IndexC cmax = matrix.Size2()-1;
    for(UIntT i = 0;i < this->Rows();i++) {
      Array1dC<DataT> row2 = oth.Row(i);
      if(row2.Range().Size() <= 0) {
	for(BufferAccessIter2C<DataT,DataT> it(ret[i],matrix[i]);it;it++)
	  it.Data1() = it.Data2();
	continue;
      }
      if(row2.Range().Min() > 0) {
	IndexRangeC rng(0,row2.Range().Min()-1);
	for(BufferAccessIter2C<DataT,DataT> it(ret[i],matrix[i],rng);it;it++)
	  it.Data1() = it.Data2();
      }
      for(BufferAccessIter3C<DataT,DataT,DataT> it(ret[i],matrix[i],row2,row2.Range());it;it++)
	it.Data1() = it.Data2() - it.Data3();
      if(row2.Range().Max() < cmax ) {
	IndexRangeC rng(row2.Range().Max()+1,cmax);
	for(BufferAccessIter2C<DataT,DataT> it(ret[i],matrix[i],rng);it;it++)
	  it.Data1() = it.Data2();
      }
    }
    return TSMatrixC<DataT>(ret);    
  }
  
  template<class DataT>
  void TSMatrixFullBodyC<DataT>::AddIP(const TSMatrixC<DataT> &oth) {
    TSMatrixFullC<DataT> ts(oth);
    if(ts.IsValid()) {
      matrix += ts.FullTMatrix();
      return ;
    }
    RavlAssert(oth.Rows() == this->Rows() && oth.Cols() == this->Cols());
    for(UIntT i = 0;i < this->Rows();i++) {
      Array1dC<DataT> row2 = oth.Row(i);
      if(row2.Range().Size() <= 0)
	continue;
      for(BufferAccessIter2C<DataT,DataT> it(matrix[i],row2,row2.Range());it;it++)
	it.Data1() += it.Data2();
    }
  }
  
  template<class DataT>  
  void TSMatrixFullBodyC<DataT>::SubIP(const TSMatrixC<DataT> &oth) {
    TSMatrixFullC<DataT> ts(oth);
    if(ts.IsValid()) {
      matrix -= ts.FullTMatrix();
      return ;
    }
    RavlAssert(oth.Rows() == this->Rows() && oth.Cols() == this->Cols());
    for(UIntT i = 0;i < this->Rows();i++) {
      Array1dC<DataT> row2 = oth.Row(i);
      if(row2.Range().Size() <= 0)
	continue;
      for(BufferAccessIter2C<DataT,DataT> it(matrix[i],row2,row2.Range());it;it++)
	it.Data1() -= it.Data2();
    }
  }
  
#if !RAVL_TSMATRIXFULL_DEFAULT    
  template<class DataT>  
  TSMatrixC<DataT> TSMatrixFullBodyC<DataT>::Mul(const TSMatrixC<DataT> &oth) const {
    TSMatrixFullC<DataT> ts(oth);
    if(!ts.IsValid())
      return TSMatrixBodyC<DataT>::Mul(oth); // Use default
    return TSMatrixC<DataT>(matrix * ts.FullTMatrix());
  }
  
  template<class DataT>  
  TVectorC<DataT> TSMatrixFullBodyC<DataT>::Mul(const TVectorC<DataT> &oth) const 
  { return matrix * oth; }
  
  template<class DataT>  
  TSMatrixC<DataT> TSMatrixFullBodyC<DataT>::MulT(const TSMatrixC<DataT> & B) const {
    TSMatrixFullC<DataT> ts(B);
    if(!ts.IsValid())
      return TSMatrixBodyC<DataT>::MulT(B); // Use default
    return TSMatrixC<DataT>(matrix.MulT(ts.FullTMatrix()));
  }
  
  template<class DataT>  
  TSMatrixC<DataT> TSMatrixFullBodyC<DataT>::TMul(const TSMatrixC<DataT> & B) const {
    TSMatrixFullC<DataT> ts(B);
    if(!ts.IsValid())
      return TSMatrixBodyC<DataT>::TMul(B); // Use default
    return TSMatrixC<DataT>(matrix.TMul(ts.FullTMatrix()));
  }
  
  template<class DataT>  
  TVectorC<DataT> TSMatrixFullBodyC<DataT>::TMul(const TVectorC<DataT> & B) const 
  { return matrix.TMul(B); }
  
#endif
  
  template<class DataT>  
  DataT TSMatrixFullBodyC<DataT>::MulSumColumn(UIntT c,const Array1dC<DataT> &dat) const { 
    if(dat.Size() <= 0) {
      DataT ret;
      SetZero(ret);
      return ret;
    }
    RavlAssert(IndexRangeC(0,this->Rows()-1).Contains(dat.Range()));
    const IndexRangeC &rng = dat.Range();
    BufferAccessIterC<DataT > it(dat);
    Slice1dIterC<DataT> sit(const_cast<TMatrixC<DataT> &>(matrix).SliceColumn(c),rng);
    DataT ret = (*sit) * (*it);
    for(it++,sit++;it;sit++,it++)
      ret += (*sit) * (*it);
    return ret;
  }

  template<class DataT>  
  DataT TSMatrixFullBodyC<DataT>::MulSumColumn(UIntT c,const Slice1dC<DataT> &slice) const {
    DataT ret;;
    if(slice.Size() == 0) {
      SetZero(ret);
      return ret;
    }
    RavlAssert(IndexRangeC(0,this->Rows()-1).Contains(slice.Range()));
    Slice1dIter2C<DataT,DataT> it(const_cast<TMatrixC<DataT> &>(matrix).SliceColumn(c),
				  slice,
				  slice.Range());
    if(!it) {
      SetZero(ret);
      return ret;
    }
    ret = it.Data1() * it.Data2();
    for(it++;it;it++)
      ret += it.Data1() * it.Data2();
    return ret;    
  }
  
  ///////////////////////////////////////////////////////////////////
  
  template<class DataT> 
  TSMatrixC<DataT>::TSMatrixC(const TMatrixC<DataT> &mat)
    : RCHandleC<TSMatrixBodyC<DataT> >(*new TSMatrixFullBodyC<DataT>(mat))
  {}

  template<class DataT> 
  TSMatrixC<DataT>::TSMatrixC(UIntT rows,UIntT cols)
    : RCHandleC<TSMatrixBodyC<DataT> >(*new TSMatrixFullBodyC<DataT>(rows,cols))
  {}
  
}


#endif
