// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_TSMATRIXDIAGONAL_HEADER
#define RAVL_TSMATRIXDIAGONAL_HEADER 1
//! rcsid="$Id$"
//! lib=RavlMath
//! author="Charles Galambos"
//! docentry="Ravl.API.Math.Linear Algebra.Smart Matrix"
//! date="19/8/2002"
//! file="Ravl/Math/LinearAlgebra/General/TSMatrixDiagonal.hh"

#include "Ravl/TSMatrixPartial.hh"
#include "Ravl/SArray1dIter3.hh"

namespace RavlN {
  template<class DataT> class TSMatrixDiagonalC;
  
  //! userlevel=Develop
  //: Diagonal matrix body
  
  template<class DataT>
  class TSMatrixDiagonalBodyC
    : public TSMatrixPartialBodyC<DataT>
  {
  public:
    TSMatrixDiagonalBodyC(int i)
      : TSMatrixPartialBodyC<DataT>(i,i,SArray1dC<DataT>(i))
    {}
    //: Constructor.
    
    TSMatrixDiagonalBodyC(const SArray1dC<DataT> &dat)
      : TSMatrixPartialBodyC<DataT>(dat.Size(),dat.Size(),dat)
    {}
    //: Construct diagonal from an array
    
    virtual RCBodyVC &Copy() const
    { return *new TSMatrixDiagonalBodyC<DataT>(this->data.Copy()); }
    //: Create a copy of this matrix.
    
    virtual const type_info &MatrixType() const
    { return typeid(TSMatrixDiagonalBodyC<DataT>); }
    //: Find the type of the matrix.
    
    virtual DataT Element(UIntT i,UIntT j) const { 
      if(i != j) return 0;
      return this->data[i];
    } 
    //: Access element.
    
    virtual void Element(UIntT i,UIntT j,const DataT &val) {
      if(i != j) {
	if(val != 0)
	  cerr << "Attempting to set off diagonal of diagonal matrix. \n";
	return ;
      }
      this->data[i] = val;
    }
    //: Set element.
    
    virtual Array1dC<DataT> Row(UIntT i) const
    { return Array1dC<DataT>(const_cast<BufferC<DataT> &>(this->data.Buffer()),
			     RangeBufferAccessC<DataT>(const_cast<DataT *>( &(this->data[i])),IndexRangeC(i,i))); }
    //: Access a row from the matrix.
    
    virtual Slice1dC<DataT> Col(UIntT j) const { 
      SArray1dC<DataT> &diag = const_cast<SArray1dC<DataT> &>(this->data);
      return Slice1dC<DataT>(diag.Buffer(),&(diag[0]),IndexRangeC(j,j),1);
    }
    //: Access slice from matrix.
    
    virtual DataT MulSumColumn(UIntT c,const Slice1dC<DataT> &slice) const {  
      if(!slice.Contains(c)) {
	DataT ret;
	SetZero(ret);
	return ret;
      }
      return slice[c] * this->data[c];      
    }
    //: Multiply column by values from slice and sum them.
    
    virtual DataT MulSumColumn(UIntT c,const Array1dC<DataT> &dat) const { 
      if(!dat.Contains(c)) {
	DataT ret;
	SetZero(ret);
	return ret;
      }
      return dat[c] * this->data[c];
    }
    //: Multiply column by values from dat and sum them.
    
    virtual TSMatrixC<DataT> T() const { 
      // FIXME: This should really be a copy ?
      return TSMatrixDiagonalC<DataT>(const_cast<TSMatrixDiagonalBodyC<DataT> &>(*this)); 
    }
    //: Get transpose of matrix.
    // This is a no-op.
    
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
    
    virtual TSMatrixC<DataT> AAT() const;
    //: Return  A * A.T().
    
    virtual TSMatrixC<DataT> ATA() const;
    //: Return  A.T() * A.
    
    virtual TSMatrixC<DataT> IMul(const TSMatrixC<DataT> & B) const;
    //: Multiply B by the inverse of this matrix.
    
    virtual TSMatrixC<DataT> ITMul(const TSMatrixC<DataT> & B) const;
    //: Multiply B by the transpose of the inverse of this matrix.
    
    virtual TMatrixC<DataT> TMatrix(bool) const;
    //: Get as full matrix.
    
    virtual void SetDiagonal(const TVectorC<DataT> &d) { 
      RavlAssert(this->data.Size() == d.Size());
      this->data = d.Copy(); 
    }
    //: Set the diagonal of this matrix.
    // If d.Size() != Cols() an error is given.
    
    virtual void AddDiagonal(const TVectorC<DataT> &d) {  
      for(SArray1dIter2C<DataT,DataT> it(this->data,d);it;it++)
	it.Data1() += it.Data2();
    }
    //: Add a vector to the diagonal of this matrix.
    // If d.Size() != Cols() an error is given.
    
  };
  
  
  //! userlevel=Normal
  //: Diagonal matrix
  
  template<class DataT>
  class TSMatrixDiagonalC
    : public TSMatrixPartialC<DataT>
  {
  public:
    explicit TSMatrixDiagonalC(int i)
      : TSMatrixPartialC<DataT>(*new TSMatrixDiagonalBodyC<DataT>(i))
    {}
    //: Create a diagonal matrix of size i by i .
    // The contents of the matrix are undefined.
    
    TSMatrixDiagonalC(const TVectorC<DataT> &data)
      : TSMatrixPartialC<DataT>(*new TSMatrixDiagonalBodyC<DataT>(data))
    {}
    //: Create a diagonal matrix from a vector.
    
    TSMatrixDiagonalC(const TSMatrixC<DataT> &mat)
      : TSMatrixPartialC<DataT>(dynamic_cast<const TSMatrixDiagonalBodyC<DataT> *>(RCHandleC<TSMatrixBodyC<DataT> >::BodyPtr(mat)))
    {}
    //: Create a diagonal matrix from a vector.
    
    TSMatrixDiagonalC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
  protected:
    TSMatrixDiagonalC(TSMatrixDiagonalBodyC<DataT> &bod)
      : TSMatrixPartialC<DataT>(bod)
    {}
    //: Body constructor.
    
    TSMatrixDiagonalC(const TSMatrixDiagonalBodyC<DataT> *bod)
      : TSMatrixPartialC<DataT>(bod)
    {}
    //: Body constructor.
    
    TSMatrixDiagonalBodyC<DataT> &Body()
    { return static_cast<TSMatrixDiagonalBodyC<DataT> &>(TSMatrixC<DataT>::Body()); }
    //: Access body.
    
    const TSMatrixDiagonalBodyC<DataT> &Body() const
    { return static_cast<const TSMatrixDiagonalBodyC<DataT> &>(TSMatrixC<DataT>::Body()); }
    //: Access body.
    
  public:
    friend class TSMatrixDiagonalBodyC<DataT>;
  };
  
  
  template<class DataT>
  TSMatrixC<DataT> TSMatrixDiagonalBodyC<DataT>::Mul(const TSMatrixC<DataT> &mat) const {
    if(mat.MatrixType() == typeid(TSMatrixDiagonalBodyC<DataT>)) {
      TSMatrixDiagonalC<DataT> diag(mat);
      TVectorC<DataT>  tmp(mat.Rows());
      for(SArray1dIter3C<DataT,DataT,DataT> it(tmp,this->data,diag.Data());it;it++)
	it.Data1() = it.Data2() * it.Data3();
      return TSMatrixDiagonalC<DataT>(tmp);
    }
    RavlAssert(this->Cols() == mat.Rows());
    const SizeT rdim = this->Rows();
    const SizeT cdim = mat.Cols();
    if(mat.IsRowDirectAccess()) {
      //cerr << "Using copy... \n";
      TSMatrixC<DataT> ret(mat.Copy()); // Use same type as input.
      BufferAccessIterC<DataT> dit(this->data);
      for (UIntT r = 0; r < rdim; r++,dit++) {
	Array1dC<DataT> ra = ret.Row(r);
	for(BufferAccessIterC<DataT> it(ra);it;it++)
	  it.Data() *= (*dit);
      }
      return ret;
    }
    //cerr << "Using direct... \n";
    TMatrixC<DataT> out(rdim, cdim);
    BufferAccessIterC<DataT> dit(this->data);
    IndexRangeC crng(0,cdim-1);
    for (UIntT r = 0; r < rdim; r++,dit++) {
      Array1dC<DataT> ra = mat.Row(r);
      RangeBufferAccessC<DataT> ro(out[r],crng);
      //cerr <<"Row=" << ra << "\n";
      IndexRangeC rng = ra.Range();
      if(rng.IsEmpty()) {
	for(BufferAccessIterC<DataT> it(ro,crng);it;it++)
	  SetZero(*it);
	continue;
      }
      if(rng.Min() > 0) {
	for(BufferAccessIterC<DataT> it(ro,IndexRangeC(0,rng.Min()-1));it;it++)
	  SetZero(*it);
      }
      for(BufferAccessIter2C<DataT,DataT> it1(ra,ro,rng);it1;it1++) {
	//	cerr << "(" << (*dit) << " " << it1.Data1() << ") ";
	it1.Data2() = (*dit) * it1.Data1();
      }
      //cerr << "\n";
      if(rng.Max() < crng.Max()) {
	for(BufferAccessIterC<DataT> it(ro,IndexRangeC(rng.Max()+1,crng.Max()));it;it++)
	  SetZero(*it);
      }
    }
    return out;
  }
  
  template<class DataT>
  TVectorC<DataT> TSMatrixDiagonalBodyC<DataT>::Mul(const TVectorC<DataT> &oth) const {
    RavlAssert(this->Rows() == oth.Size());
    TVectorC<DataT> ret(oth.Size());
    for(SArray1dIter3C<DataT,DataT,DataT> it(ret,this->data,oth);it;it++)
      it.Data1() = it.Data2() * it.Data3();
    return ret;
  }
  
  template<class DataT>
  TSMatrixC<DataT> TSMatrixDiagonalBodyC<DataT>::MulT(const TSMatrixC<DataT> & B) const {
    return TSMatrixDiagonalBodyC<DataT>::Mul(B.T());
  }
  
  template<class DataT>
  TSMatrixC<DataT> TSMatrixDiagonalBodyC<DataT>::TMul(const TSMatrixC<DataT> & B) const {
    return TSMatrixDiagonalBodyC<DataT>::Mul(B);
  }
  
  template<class DataT>
  TVectorC<DataT> TSMatrixDiagonalBodyC<DataT>::TMul(const TVectorC<DataT> & B) const {
    return TSMatrixDiagonalBodyC<DataT>::Mul(B);
  }
  
  template<class DataT>
  TSMatrixC<DataT> TSMatrixDiagonalBodyC<DataT>::AAT() const {
    TSMatrixDiagonalC<DataT> diag(this->Rows());
    for(SArray1dIter2C<DataT,DataT> it(diag.Data(),this->data);it;it++)
      it.Data1() = Sqr(it.Data2());
    return diag;
  }
  
  template<class DataT>
  TSMatrixC<DataT> TSMatrixDiagonalBodyC<DataT>::ATA() const {
    return TSMatrixDiagonalBodyC<DataT>::AAT();
  }
  
  template<class DataT>
  TSMatrixC<DataT> TSMatrixDiagonalBodyC<DataT>::IMul(const TSMatrixC<DataT> & mat) const {
    RavlAssert(this->Cols() == mat.Rows());
    const SizeT rdim = this->Rows();
    const SizeT cdim = mat.Cols();
    TMatrixC<DataT> out(rdim, cdim);
    BufferAccessIterC<DataT> dit(this->data);
    for (UIntT r = 0; r < rdim; r++,dit++) {
      Array1dC<DataT> ra = mat.Row(r);
      for(BufferAccessIter2C<DataT,DataT> it(ra,RangeBufferAccessC<DataT>(out[r],IndexRangeC(0,cdim)));it;it++)
	it.Data2() = it.Data1() / (*dit);
    }
    return out;
  }
  
  template<class DataT>
  TSMatrixC<DataT> TSMatrixDiagonalBodyC<DataT>::ITMul(const TSMatrixC<DataT> & B) const 
  { return TSMatrixDiagonalBodyC<DataT>::IMul(B); }
  
  template<class DataT>
  TMatrixC<DataT> TSMatrixDiagonalBodyC<DataT>::TMatrix(bool) const {
    TMatrixC<DataT> ret(this->Rows(),this->Cols());
    ret.Fill(0);
    ret.SetDiagonal(this->data);
    return ret;
  }
}

#endif
