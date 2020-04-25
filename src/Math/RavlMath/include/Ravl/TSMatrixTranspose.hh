// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_TSMATRIXTRANSPOSE_HEADER
#define RAVL_TSMATRIXTRANSPOSE_HEADER 1
//! date="22/8/2002"
//! author="Charles Galambos"
//! docentry="Ravl.API.Math.Linear Algebra.Smart Matrix"
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/LinearAlgebra/General/TSMatrixTranspose.hh"

#include "Ravl/TSMatrix.hh"

namespace RavlN {
  template<class DataT> class TSMatrixTransposeC;
  
  //! userlevel=Develop
  //: Transpose matrix body
  
  template<class DataT>
  class TSMatrixTransposeBodyC
    : public TSMatrixBodyC<DataT>
  {
  public:
    TSMatrixTransposeBodyC(const TSMatrixC<DataT> &oth)
      : matrix(oth)
    {
      if(oth.IsValid()) {
	this->size[0] = oth.Cols();
	this->size[1] = oth.Rows();
      } else {
	this->size[0] = 0;
	this->size[1] = 0;
      }
    }
    //: Constructor.
    
    virtual RCBodyVC &Copy() const
    { return *new TSMatrixTransposeBodyC<DataT>(matrix.Copy()); }
    //: Create a copy of this matrix.
    
    virtual const type_info &MatrixType() const
    { return typeid(TSMatrixTransposeBodyC<DataT>); }
    //: Find the type of the matrix.
    
    virtual DataT Element(UIntT i,UIntT j) const 
    { return matrix.Element(j,i); } 
    //: Access element.
    
    virtual void Element(UIntT i,UIntT j,const DataT &val) 
    { matrix.Element(j,i,val); }
    //: Set element.
    
    virtual Array1dC<DataT> Row(UIntT i) const 
    { return matrix.Col(i); }
    //: Access a row from the matrix.
    
    virtual bool IsRowDirectAccess() const
    { return false; }
    //: Does Row() give direct access to actual data ?
    
    virtual Slice1dC<DataT> Col(UIntT j) const
    { return matrix.Row(j).Slice1d(); }
    //: Access slice from matrix.
    
    virtual bool IsColDirectAccess() const
    { return matrix.IsRowDirectAccess(); }
    //: Does Row() give direct access to actual data ?
    
    virtual DataT MulSumColumn(UIntT c,const Array1dC<DataT> &dat) const { 
      DataT sum;
      Array1dC<DataT> ra = matrix.Row(c);
      IndexRangeC rng = dat.Range();
      rng.ClipBy(ra.Range());
      if(rng.Size() <= 0) {
	SetZero(sum);
	return sum;
      }
      BufferAccessIter2C<DataT,DataT> it(dat,ra,rng);
      sum = it.Data1() * it.Data2();
      for(it++;it;it++)
	sum += it.Data1() * it.Data2();
      return sum;
    }
    //: Multiply column by values from dat and sum them.
    
    virtual DataT MulSumColumn(UIntT c,const Slice1dC<DataT> &slice) const { 
      DataT sum;
      Array1dC<DataT> ra = matrix.Row(c);
      IndexRangeC rng = slice.Range();
      rng.ClipBy(ra.Range());
      if(rng.Size() <= 0) {
	SetZero(sum);
	return sum;
      }
      Slice1dIterC<DataT> its(slice,rng);
      BufferAccessIterC<DataT> it(ra,rng);
      sum = it.Data() * its.Data();
      for(it++,its++;it;it++,its++)
	sum += it.Data() * its.Data();
      return sum;      
    }
    //: Multiply column by values from slice and sum them.

    virtual void AddIP(const TSMatrixC<DataT> &oth) {
      RavlAssertMsg(0,"TSMatrixTransposeBodyC::AddIP(), not implemented. ");
    }
    //: Add this matrix to 'oth' and return the result.
    // Note the default implementation only works where Row(UIntT), returns a real access
    // to the data in the matrix.
    
    virtual void SubIP(const TSMatrixC<DataT> &oth) {
      RavlAssertMsg(0,"TSMatrixTransposeBodyC::SubIP(), not implemented. ");
    }
    //: Subtract 'oth' from this matrix and return the result.
    // Note the default implementation only works where Row(UIntT), returns a real access
    // to the data in the matrix.
    
    virtual TSMatrixC<DataT> T() const  
    { return matrix; }
    //: Get transpose of matrix.
    
    virtual TSMatrixC<DataT> Mul(const TSMatrixC<DataT> &oth) const
    { return matrix.TMul(oth); }
    //: Get this matrix times 'oth'.
    
    virtual TVectorC<DataT> Mul(const TVectorC<DataT> &oth) const
    { return matrix.TMul(oth); }
    //: Get this matrix times 'oth'.
    
    virtual TSMatrixC<DataT> MulT(const TSMatrixC<DataT> & B) const  
    { return matrix.TMul(B.T()); }
    //: Multiplication A * B.T()
    
    virtual TSMatrixC<DataT> TMul(const TSMatrixC<DataT> & B) const
    { return matrix.Mul(B); }
    //: Multiplication A.T() * B
    
    virtual TVectorC<DataT> TMul(const TVectorC<DataT> & B) const
    { return matrix.Mul(B); }
    //: Multiplication A.T() * B
    
    virtual TSMatrixC<DataT> IMul(const TSMatrixC<DataT> & B) const
    { return matrix.ITMul(B); }
    //: Multiply B by the inverse of this matrix.
    
    virtual TSMatrixC<DataT> ITMul(const TSMatrixC<DataT> & B) const
    { return matrix.IMul(B); }
    //: Multiply B by the transpose of the inverse of this matrix.

    virtual TSMatrixC<DataT> AAT() const
    { return matrix.ATA(); }
    //: Return  A * A.T().
    
    virtual TSMatrixC<DataT> ATA() const
    { return matrix.AAT(); }
    //: Return  A.T() * A.

    virtual void SetDiagonal(const TVectorC<DataT> &d)
    { matrix.SetDiagonal(d); }
    //: Set the diagonal of this matrix.
    // If d.Size() != Cols() an error is given.
    
    virtual void AddDiagonal(const TVectorC<DataT> &d)
    { matrix.AddDiagonal(d); }
    //: Add a vector to the diagonal of this matrix.
    // If d.Size() != Cols() an error is given.
    
    virtual TMatrixC<DataT> TMatrix(bool) const { 
      // FIXME: This could be faster.
      return matrix.TMatrix().T(); 
    }
    //: Get as full matrix.
    
    const TSMatrixC<DataT> &Matrix() const
    { return matrix; }
    //: Get original matrix.
    
    TSMatrixC<DataT> &Matrix()
    { return matrix; }
    //: Get original matrix.
    
    virtual void SetSmallToBeZero(const DataT &min)
    { matrix.SetSmallToBeZero(min); }
    //: Set values smaller than 'min' to zero in vector.
    
    virtual DataT SumOfAbs() const
    { return matrix.SumOfAbs(); }
    //: Sum the absolute values of all members of the matrix.
    
    virtual void SwapRows(int i,int j)
    { RavlAssertMsg(0,"TSMatrixTransposeBodyC::SwapRows(), not implemented. "); }
    //: Swap two rows in the matrix.
    
    virtual void Fill(const DataT &dat)
    { matrix.Fill(dat); }
    //: Fill matrix with values.

  protected:
    TSMatrixC<DataT> matrix;
  };
  
  
  //! userlevel=Normal
  //: Transpose matrix
  
  template<class DataT>
  class TSMatrixTransposeC
    : public TSMatrixC<DataT>
  {
  public:    
    TSMatrixTransposeC(const TSMatrixC<DataT> &base)
      : TSMatrixC<DataT>(dynamic_cast<const TSMatrixTransposeBodyC<DataT> *>(RCHandleC<TSMatrixBodyC<DataT> >::BodyPtr(base)))
    {}
    //: Base constructor.
  protected:
    TSMatrixTransposeC(TSMatrixTransposeBodyC<DataT> &bod)
      : TSMatrixC<DataT>(bod)
    {}
    //: Body constructor.
    
    TSMatrixTransposeC(const TSMatrixTransposeBodyC<DataT> *bod)
      : TSMatrixC<DataT>(bod)
    {}
    //: Body constructor.
    
    TSMatrixTransposeBodyC<DataT> &Body()
    { return static_cast<TSMatrixTransposeBodyC<DataT> &>(TSMatrixC<DataT>::Body()); }
    //: Access body.
    
    const TSMatrixTransposeBodyC<DataT> &Body() const
    { return static_cast<const TSMatrixTransposeBodyC<DataT> &>(TSMatrixC<DataT>::Body()); }
    //: Access body.
    
  public:
    friend class TSMatrixTransposeBodyC<DataT>;
    
    const TSMatrixC<DataT> &Matrix() const
    { return Body().Matrix(); }
    //: Get scale for matrix.
    
    TSMatrixC<DataT> &Matrix()
    { return Body().Matrix(); }
    //: Get scale for matrix.
  };
  
}


#endif
