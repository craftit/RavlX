// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_TSMATRIX_HEADER
#define RAVL_TSMATRIX_HEADER 1
///////////////////////////////////////////////////
//! rcsid="$Id$"
//! date="15/8/2002"
//! lib=RavlMath
//! author="Charles Galambos"
//! docentry="Ravl.API.Math.Linear Algebra.Smart Matrix"
//! file="Ravl/Math/LinearAlgebra/General/TSMatrix.hh"

#include "Ravl/TMatrix.hh"
#include "Ravl/TVector.hh"
#include "Ravl/RefCounter.hh"
#include "Ravl/Math.hh"
#include "Ravl/Array1d.hh"
#include "Ravl/IndexRange2d.hh"
#include "Ravl/Array1dIter3.hh"
#include "Ravl/Array1dIter2.hh"
#include "Ravl/Array1dIter.hh"

namespace RavlN {
  template<class DataT> class TSMatrixC;
  template<class DataT> class TSMatrixSymmetricC;
  template<class DataT> class TSMatrixTransposeC;
  template<class DataT> class TSMatrixTransposeBodyC;
  template<class DataT> class TSMatrixFullBodyC;
  template<class DataT> class TSMatrixSymmetricBodyC;
  
  template<class DataT>
  DataT MultiplySum(const RangeBufferAccessC<DataT> &ar1,const RangeBufferAccessC<DataT> &ar2) {
    DataT sum;
    IndexRangeC rng = ar1.Range();
    rng.ClipBy(ar2.Range());
    if(rng.Size() <= 0) {
      SetZero(sum);
      return sum;
    }
    BufferAccessIter2C<DataT,DataT> it(ar1,ar2,rng);
    sum = it.Data1() * it.Data2();
    for(it++;it;it++)
      sum += it.Data1() * it.Data2();
    return sum;
  }
  //! userlevel=Advanced
  //: Multiply the contents of matching entries in two arrays together and sum them.

  
  //:-
  
  //! userlevel=Develop
  //: Smart Matrix Body.
  
  template<class DataT>
  class TSMatrixBodyC 
    : public RCBodyVC
  {
  public:
    TSMatrixBodyC()
    {}
    //: Default constructor.

    TSMatrixBodyC(UIntT rows,UIntT cols)
      : size(rows,cols)
    {}
    //: Constructor.
    
    virtual const type_info &MatrixType() const
    { return typeid(void); }
    //: Find the type of the matrix.
    
    virtual DataT Element(UIntT i,UIntT j) const { 
      RavlAssert(0);
      return DataT();
    } 
    //: Access element.
    
    virtual void Element(UIntT i,UIntT j,const DataT &val) 
    { RavlAssert(0); }
    //: Set element.
    
    virtual Array1dC<DataT> Row(UIntT i) const
    { RavlAssert(0); return Array1dC<DataT>(); }
    //: Access a row from the matrix.
    
    virtual bool IsRowDirectAccess() const
    { return true; }
    //: Does Row() give direct access to actual data ?
    
    virtual DataT MulSumColumn(UIntT c,const Array1dC<DataT> &dat) const
    { RavlAssert(0); return DataT(); }
    //: Multiply column by values from dat and sum them.
    
    virtual Slice1dC<DataT> Col(UIntT j) const
    { RavlAssert(0); return Slice1dC<DataT>(); }
    //: Access slice from matrix.
    
    virtual bool IsColDirectAccess() const
    { return true; }
    //: Does Col() give direct access to actual data ?
    
    virtual DataT MulSumColumn(UIntT c,const Slice1dC<DataT> &slice) const
    { RavlAssert(0); return DataT(); }
    //: Multiply column by values from slice and sum them.
    
    virtual TSMatrixC<DataT> Add(const TSMatrixC<DataT> &oth) const;
    //: Add this matrix to 'oth' and return the result.
    
    virtual TSMatrixC<DataT> Sub(const TSMatrixC<DataT> &oth) const;
    //: Subtract 'oth' from this matrix and return the result.
    
    virtual void AddIP(const TSMatrixC<DataT> &oth);
    //: Add this matrix to 'oth' and return the result.
    // Note the default implementation only works where Row(UIntT), returns a real access
    // to the data in the matrix.
    
    virtual void SubIP(const TSMatrixC<DataT> &oth);
    //: Subtract 'oth' from this matrix and return the result.
    // Note the default implementation only works where Row(UIntT), returns a real access
    // to the data in the matrix.
    
    virtual TSMatrixC<DataT> T() const;
    //: Get transpose of matrix.
    
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
    
    virtual TSMatrixC<DataT> IMul(const TSMatrixC<DataT> & B) const
    { RavlAssert(0); return TSMatrixC<DataT>(); }
    //: Multiply B by the inverse of this matrix.
    
    virtual TSMatrixC<DataT> ITMul(const TSMatrixC<DataT> & B) const
    { RavlAssert(0); return TSMatrixC<DataT>(); }
    //: Multiply B by the transpose of the inverse of this matrix.
    
    virtual TSMatrixC<DataT> AAT() const;
    //: Return  A * A.T().
    
    virtual TSMatrixC<DataT> ATA() const;
    //: Return  A.T() * A.
    
    virtual void SetDiagonal(const TVectorC<DataT> &d);
    //: Set the diagonal of this matrix.
    // If d.Size() != Cols() an error is given.
    
    virtual void AddDiagonal(const TVectorC<DataT> &d);
    //: Add a vector to the diagonal of this matrix.
    // If d.Size() != Cols() an error is given.
    
    virtual void AddOuterProduct(const TVectorC<DataT> &vec1,const TVectorC<DataT> &vec2);
    //: Add outer product of vec1 and vec2 to this matrix.
    
    virtual void AddOuterProduct(const TVectorC<DataT> &vec1,const TVectorC<DataT> &vec2,const DataT &a);
    //: Add outer product of vec1 and vec2 multiplied by a to this matrix .
    
    virtual void SetSmallToBeZero(const DataT &min);
    //: Set values smaller than 'min' to zero in vector.
    
    virtual DataT SumOfAbs() const;
    //: Sum the absolute values of all members of the matrix.
    
    virtual void SwapRows(int i,int j);
    //: Swap two rows in the matrix.
    
    virtual TMatrixC<DataT> TMatrix(bool alwaysCopy = false) const
    { RavlAssert(0); return TMatrixC<DataT>(); }
    //: Access as a TMatrix.
    // Note, the returned matrix may not be a copy and should not be changed!
    
    virtual void Fill(const DataT &dat);
    //: Fill matrix with values.
    
    const Index2dC &Size() const
    { return size; }
    //: Find size of matrix.
    
    UIntT Rows() const
    { return size[0].V(); }
    //: Number of rows in matrix.
    
    UIntT Cols() const
    { return size[1].V(); }
    //: Number of cols in matrix.
    
  protected:
    Index2dC size;
  };
  
  //! userlevel=Normal
  //: Smart Matrix.
  
  template<class DataT>
  class TSMatrixC 
    : public RCHandleC<TSMatrixBodyC<DataT> >
  {
  public:
    TSMatrixC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    TSMatrixC(const TMatrixC<DataT> &mat);
    //: This actually creates a TSMatrixFullC<DataT>
    
    TSMatrixC(UIntT rows,UIntT cols);
    //: Create a full matrix, rows by cols in size.
    
  protected:
    TSMatrixC(TSMatrixBodyC<DataT> &bod)
      : RCHandleC<TSMatrixBodyC<DataT> >(bod)
    {}
    //: Body constructor.
    
    TSMatrixC(const TSMatrixBodyC<DataT> *bod)
      : RCHandleC<TSMatrixBodyC<DataT> >(bod)
    {}
    //: Body constructor.
    
    TSMatrixBodyC<DataT> &Body()
    { return RCHandleC<TSMatrixBodyC<DataT> >::Body(); }
    //: Access body.
    
    const TSMatrixBodyC<DataT> &Body() const
    { return RCHandleC<TSMatrixBodyC<DataT> >::Body(); }
    //: Access body.
    
  public:
    const type_info &MatrixType() const
    { return Body().MatrixType(); }
    //: Find the type of the matrix.
    
    TSMatrixC<DataT> Copy() const
    { return TSMatrixC<DataT>(static_cast<TSMatrixBodyC<DataT> &>(Body().Copy())); }
    //: Create a copy.
    
    DataT Element(UIntT i,UIntT j) const
    { return Body().Element(i,j); }
    //: Access element.
    
    void Element(UIntT i,UIntT j,const DataT &val) 
    { Body().Element(i,j,val); }
    //: Set element.
    
    Array1dC<DataT> Row(UIntT i) const
    { return Body().Row(i); }
    //: Access a row from the matrix.
    
    bool IsRowDirectAccess() const
    { return Body().IsRowDirectAccess(); }
    //: Does Row() give direct access to actual data ?
    
    DataT MulSumColumn(UIntT c,const Array1dC<DataT> &dat) const
    { return Body().MulSumColumn(c,dat); }
    //: Multiply column by values from dat and sum them.

    Slice1dC<DataT> Col(UIntT j) const
    { return Body().Col(j); }
    //: Access slice from matrix.
    
    bool IsColDirectAccess() const
    { return Body().IsColDirectAccess(); }
    //: Does Col() give direct access to actual data ?

    DataT MulSumColumn(UIntT c,const Slice1dC<DataT> &slice) const
    { return Body().MulSumColumn(c,slice); }
    //: Multiply column by values from slice and sum them.

    DataT operator[](const Index2dC &ind)
    { return Body().Element(ind[0].V(),ind[1].V()); }
    //: Access element.

    const Index2dC &Size() const
    { return Body().Size(); }
    //: Find size of matrix.
    
    UIntT Rows() const
    { return Body().Rows(); }
    //: Number of rows in matrix.
    
    UIntT Cols() const
    { return Body().Cols(); }
    //: Number of cols in matrix.
    
    TSMatrixC<DataT> Add(const TSMatrixC<DataT> &oth) const
    { return Body().Add(oth); }
    //: Add this matrix to 'oth' and return the result.
    
    TSMatrixC<DataT> operator+(const TSMatrixC<DataT> &oth) const
    { return Body().Add(oth); }
    //: Add this matrix to 'oth' and return the result.
    
    TSMatrixC<DataT> Sub(const TSMatrixC<DataT> &oth) const 
    { return Body().Sub(oth); }
    //: Subtract 'oth' from this matrix and return the result.
    
    TSMatrixC<DataT> operator-(const TSMatrixC<DataT> &oth) const
    { return Body().Sub(oth); }
    //: Subtract 'oth' from this matrix and return the result.
    
    void AddIP(const TSMatrixC<DataT> &oth) 
    { Body().AddIP(oth); }
    //: Add 'oth' to this matrix in place..
    
    TSMatrixC<DataT> &operator+=(const TSMatrixC<DataT> &oth)
    { Body().AddIP(oth); return *this; }
    //: Add 'oth' to this matrix in place..
    
    void SubIP(const TSMatrixC<DataT> &oth) 
    { Body().SubIP(oth); }
    //: Subtract 'oth' from this matrix in place.
    
    TSMatrixC<DataT> &operator-=(const TSMatrixC<DataT> &oth) 
    { Body().Sub(oth); return *this; }
    //: Subtract 'oth' from this matrix in place.
    
    TSMatrixC<DataT> T() const
    { return Body().T(); }
    //: Get transpose of matrix.
    
    TSMatrixC<DataT> Mul(const TSMatrixC<DataT> &oth) const
    { return Body().Mul(oth); }
    //: Get this 'oth' multiplied by this.
    
    TSMatrixC<DataT> operator*(const TSMatrixC<DataT> &oth) const
    { return Body().Mul(oth); }
    //: Get this 'oth' multiplied by this.
    
    TVectorC<DataT> Mul(const TVectorC<DataT> &oth) const
    { return Body().Mul(oth); }
    //: Get this matrix times 'oth'.
    
    TVectorC<DataT> operator*(const TVectorC<DataT> &oth) const
    { return Body().Mul(oth); }
    //: Get this 'oth' multiplied by this.
    
    TSMatrixC<DataT> MulT(const TSMatrixC<DataT> & B) const
    { return Body().MulT(B); }
    //: Multiplication A * B.T()
    
    TSMatrixC<DataT> TMul(const TSMatrixC<DataT> & B) const
    { return Body().TMul(B); }
    //: Multiplication A.T() * B
    
    TVectorC<DataT> TMul(const TVectorC<DataT> & B) const
    { return Body().TMul(B); }
    //: Multiplication A.T() * B
    
    TSMatrixC<DataT> IMul(const TSMatrixC<DataT> & B) const
    { return Body().IMul(B); }
    //: Multiply B by the inverse of this matrix.
    // Note: this operation might not be implemented for all matrix types.
    
    TSMatrixC<DataT> ITMul(const TSMatrixC<DataT> & B) const
    { return Body().ITMul(B); }
    //: Multiply B by the transpose of the inverse of this matrix.
    // Note: this operation might not be implemented for all matrix types.
    
    TSMatrixC<DataT> AAT() const 
    { return Body().AAT(); }      
    //: Return  A * A.T().
    
    TSMatrixC<DataT> ATA() const 
    { return Body().ATA(); }
    //: Return  A.T() * A.
    
    void SetDiagonal(const TVectorC<DataT> &d)
    { Body().SetDiagonal(d); }
    //: Set the diagonal of this matrix.
    // If d.Size() != Cols() an error is given.
    
    void AddDiagonal(const TVectorC<DataT> &d)
    { Body().AddDiagonal(d); }
    //: Add a vector to the diagonal of this matrix.
    // If d.Size() != Cols() an error is given.
    
    TMatrixC<DataT> TMatrix(bool alwaysCopy = false) const
    { return Body().TMatrix(alwaysCopy); }
    //: Access as a TMatrix.
    // Note, this may not be a copy and should not be changed!

    void AddOuterProduct(const TVectorC<DataT> &vec1,const TVectorC<DataT> &vec2)
    { Body().AddOuterProduct(vec1,vec2); }
    //: Add outer product of vec1 and vec2 to this matrix.
    // Note: this operation might not be implemented for all matrix types.
    
    void AddOuterProduct(const TVectorC<DataT> &vec1,const TVectorC<DataT> &vec2,const DataT &a)
    { Body().AddOuterProduct(vec1,vec2,a); }
    //: Add outer product of vec1 and vec2 multiplied by a to this matrix .
    // Note: this operation might not be implemented for all matrix types.
    
    void SetSmallToBeZero(const DataT &min)
    { Body().SetSmallToBeZero(min); }
    //: Set values smaller than 'min' to zero in vector.
    
    DataT SumOfAbs() const
    { return Body().SumOfAbs(); }
    //: Sum the absolute values of all members of the matrix.
    
    void SwapRows(int i,int j)
    { Body().SwapRows(i,j); }
    //: Swap two rows in the matrix.
    
    void Fill(const DataT &dat)
    { Body().Fill(dat); }
    //: Fill matrix with values.
    
    friend class TSMatrixBodyC<DataT>;
  };

}

#include "Ravl/TSMatrixTranspose.hh"

namespace RavlN {
  
  
  template<class DataT>
  TSMatrixC<DataT> TSMatrixBodyC<DataT>::Add(const TSMatrixC<DataT> &oth) const {
    RavlAssert(Size() == oth.Size());
    TSMatrixC<DataT> ret;
    const type_info &thisType = MatrixType();
      if(thisType == oth.MatrixType())
      ret = TSMatrixC<DataT>(static_cast<TSMatrixBodyC<DataT> &>(Copy()));
    else 
      ret = TSMatrixC<DataT>(TMatrix(true));
    ret.AddIP(oth);
    return ret;
  }
  
  template<class DataT>
  TSMatrixC<DataT> TSMatrixBodyC<DataT>::Sub(const TSMatrixC<DataT> &oth) const {
    RavlAssert(Size() == oth.Size());
    TSMatrixC<DataT> ret;
    const type_info &thisType = MatrixType();
      if(thisType == oth.MatrixType())
      ret = TSMatrixC<DataT>(static_cast<TSMatrixBodyC<DataT> &>(Copy()));
    else
      ret = TSMatrixC<DataT>(TMatrix(true));
    ret.SubIP(oth);
    return ret;
  }

  template<class DataT>
  void TSMatrixBodyC<DataT>::AddIP(const TSMatrixC<DataT> &oth) {
    RavlAssert(oth.Rows() == Rows() && oth.Cols() == Cols());
    RavlAssert(IsRowDirectAccess());
    for(UIntT i = 0;i < Rows();i++) {
      Array1dC<DataT> row2 = oth.Row(i);
      if(row2.Range().Size() <= 0)
	continue;
      Array1dC<DataT> row1 = Row(i);
      RavlAssert(row1.Range().Contains(row2.Range()));
      for(Array1dIter2C<DataT,DataT> it(row1,row2,row2.Range());it;it++)
	it.Data1() += it.Data2();
    }
  }
  
  template<class DataT>
  void TSMatrixBodyC<DataT>::SubIP(const TSMatrixC<DataT> &oth)  {
    RavlAssert(oth.Rows() == Rows() && oth.Cols() == Cols());
    RavlAssert(IsRowDirectAccess());
    for(UIntT i = 0;i < Rows();i++) {
      Array1dC<DataT> row2 = oth.Row(i);
      if(row2.Range().Size() <= 0)
	continue;
      Array1dC<DataT> row1 = Row(i);
      RavlAssert(row1.Range().Contains(row2.Range()));
      for(Array1dIter2C<DataT,DataT> it(row1,row2,row2.Range());it;it++)
	it.Data1() -= it.Data2();
    }
  }
  
  template<class DataT>
  TSMatrixC<DataT> TSMatrixBodyC<DataT>::T() const
  { return TSMatrixC<DataT>(*new TSMatrixTransposeBodyC<DataT>(TSMatrixC<DataT>(const_cast<TSMatrixBodyC<DataT> &>(*this)))); }
  
  template<class DataT>
  TSMatrixC<DataT> TSMatrixBodyC<DataT>::Mul(const TSMatrixC<DataT> &mat) const {
    RavlAssert(Cols() == mat.Rows());
    const SizeT rdim = Rows();
    const SizeT cdim = mat.Cols();
    //cerr << "TSMatrixBodyC<DataT>::Mul(const TSMatrixC<DataT> &), Called. " << rdim << " " << cdim << " Cols()=" << Cols() << "  \n";
    TMatrixC<DataT> out(rdim, cdim);
    SArray2dIterC<DataT> it(out);
    for (UIntT r = 0; r < rdim; r++) {
      Array1dC<DataT> row = Row(r);
      for (UIntT c = 0; c < cdim; c++,it++)
	*it = mat.MulSumColumn(c,row);
    }
    return TSMatrixC<DataT>(out);
  }
  
  template<class DataT>
  TVectorC<DataT> TSMatrixBodyC<DataT>::Mul(const TVectorC<DataT> &vector) const {
    RavlAssert(vector.Size() == Cols());
    const SizeT rdim = Rows();
    TVectorC<DataT> out(rdim);
    if(rdim < 1) // Zero size vector ?
      return out;
    for (UIntT i = 0; i < rdim; ++i) {
      Array1dC<DataT> row = Row(i);
      out[i] = MultiplySum(row,RangeBufferAccessC<DataT>(IndexRangeC(0,vector.Size()),vector));
    }
    return out;
  }
  
  template<class DataT>
  TSMatrixC<DataT> TSMatrixBodyC<DataT>::MulT(const TSMatrixC<DataT> & mat) const { 
    RavlAssert(Cols() == mat.Cols());
    const SizeT rdim = Rows();
    const SizeT cdim = mat.Rows();
    //cerr << "TSMatrixBodyC<DataT>::MulT(const TSMatrixC<DataT> &), Called. " << rdim << " " << cdim << " Rows=" << Rows() << "  \n";
    SArray1dC<Array1dC<DataT> > rowArr(cdim);
    for(UIntT c = 0;c < cdim;c++)
      rowArr[c] = mat.Row(c);
    TMatrixC<DataT> out(rdim, cdim);
    for (UIntT r = 0; r < rdim; r++) {
      Array1dC<DataT> row = Row(r);
      for (UIntT c = 0; c < cdim; c++)
	out[r][c] = MultiplySum(row,rowArr[c]);
    }
    return TSMatrixC<DataT>(out);
  }
  
  template<class DataT>
  TSMatrixC<DataT> TSMatrixBodyC<DataT>::TMul(const TSMatrixC<DataT> & mat) const { 
    RavlAssert(Rows() == mat.Rows());
    const SizeT rdim = Cols();
    const SizeT cdim = mat.Cols();
    //cerr << "TSMatrixBodyC<DataT>::TMul(const TSMatrixC<DataT> &), Called. " << rdim << " " << cdim << " Rows=" << Rows() << "  \n";
    TMatrixC<DataT> out(rdim, cdim);
    for (UIntT r = 0; r < rdim; r++) {
      Slice1dC<DataT> col = Col(r);
      for (UIntT c = 0; c < cdim; c++)
	out[r][c] = mat.MulSumColumn(c,col);
    }
    return TSMatrixC<DataT>(out);
  }
  
  template<class DataT>
  TVectorC<DataT> TSMatrixBodyC<DataT>::TMul(const TVectorC<DataT> & vector) const { 
    RavlAssert(vector.Size() == Rows());
    const SizeT cdim = Cols();
    TVectorC<DataT> out(cdim);
    for (UIntT i = 0; i < cdim; ++i)
      out[i] = MulSumColumn(i,vector);
    return out;
  }
  
  
  template<class DataT>
  void TSMatrixBodyC<DataT>::SetDiagonal(const TVectorC<DataT> &d) {
    RavlAssert(d.Size() <= Rows() && d.Size() <= Cols());
    for(UIntT i = 0;i < d.Size();i++)
      Element(i,i,d[i]);
  }
  
  template<class DataT>
  void TSMatrixBodyC<DataT>::AddDiagonal(const TVectorC<DataT> &d) {
    RavlAssert(d.Size() <= Rows() && d.Size() <= Cols());
    for(UIntT i = 0;i < d.Size();i++)
      Element(i,i,Element(i,i) + d[i]);    
  }
  
  template<class DataT>
  void TSMatrixBodyC<DataT>::AddOuterProduct(const TVectorC<DataT> &vec1,const TVectorC<DataT> &vec2) {
    AddIP(TSMatrixC<DataT>(vec1.OuterProduct(vec2)));
  }
  
  template<class DataT>
  void TSMatrixBodyC<DataT>::AddOuterProduct(const TVectorC<DataT> &vec1,const TVectorC<DataT> &vec2,const DataT &a) {
    AddIP(TSMatrixC<DataT>(vec1.OuterProduct(vec2,a)));
  }
  
  template<class DataT>
  void TSMatrixBodyC<DataT>::SetSmallToBeZero(const DataT &min) {
    for(UIntT i = 0;i < Rows();i++) {
      for(Array1dIterC<DataT> it(Row(i));it;it++) {
	if(Abs(*it) < min)
	  SetZero(*it);
      }
    }
  }
  
  template<class DataT>
  DataT TSMatrixBodyC<DataT>::SumOfAbs() const {
    DataT ret;
    SetZero(ret);
    for(UIntT i = 0;i < Rows();i++) {
      for(Array1dIterC<DataT> it(Row(i));it;it++)
	ret += *it;
    }
    return ret;
  }
  
  template<class DataT>
  void TSMatrixBodyC<DataT>::Fill(const DataT &dat) {
    for(UIntT i = 0;i < Rows();i++) {
      for(Array1dIterC<DataT> it(Row(i));it;it++)
	SetZero(*it);
    }
  }
  
  template<class DataT>
  void TSMatrixBodyC<DataT>::SwapRows(int i,int j) {
    RavlAssert(0);
  }
    
  
}

#endif
