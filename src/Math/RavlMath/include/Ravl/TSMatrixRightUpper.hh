// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_TSMATRIXRIGHTUPPER_HEADER
#define RAVL_TSMATRIXRIGHTUPPER_HEADER 1
//! rcsid="$Id$"
//! lib=RavlMath
//! author="Charles Galambos"
//! docentry="Ravl.API.Math.Linear Algebra.Smart Matrix"
//! date="19/8/2002"
//! file="Ravl/Math/LinearAlgebra/General/TSMatrixRightUpper.hh"

#include "Ravl/TSMatrixPartial.hh"
#include "Ravl/TSMatrixSymmetric.hh"
#include "Ravl/Math.hh"

namespace RavlN {
  template<class DataT> class TSMatrixRightUpperC;
  
  //! userlevel=Develop
  //: RightUpper matrix body
  
  template<class DataT>
  class TSMatrixRightUpperBodyC
    : public TSMatrixPartialBodyC<DataT>
  {
  public:
    static inline SizeT TriangleSize(int i)
    { return (i * (i+1))/2; }
    //: Size of triangle with size i.
    
    inline SizeT ElementIndex(int i,int j) const
    { return (i * this->Rows() - (i * (i + 1))/2 + j); }
    //: Find index of element i,j
    
    inline SizeT RowStart(int i) const
    { return i * (2 * this->Rows() + 1 - i) / 2; }
    //: Address of row start.
    
    TSMatrixRightUpperBodyC(int i)
      : TSMatrixPartialBodyC<DataT>(i,i,SArray1dC<DataT>(TriangleSize(i)))
    {}
    //: Constructor.

    TSMatrixRightUpperBodyC(int i,const SArray1dC<DataT> &ndata)
      : TSMatrixPartialBodyC<DataT>(i,i,ndata)
    { RavlAssert(TriangleSize(i) == ndata.Size()); }
    //: Constructor.
    
    TSMatrixRightUpperBodyC(const TMatrixC<DataT> &ndata);
    //: Construct by copying elements from a regular matrix.
    
    virtual RCBodyVC &Copy() const
    { return *new TSMatrixRightUpperBodyC<DataT>(this->Rows(),this->data.Copy()); }
    //: Create a copy of this matrix.
    
    virtual const type_info &MatrixType() const
    { return typeid(TSMatrixRightUpperBodyC<DataT>); }
    //: Find the type of the matrix.
    
    virtual DataT Element(UIntT i,UIntT j) const { 
      if(j < i)
	return 0;
      return this->data[ElementIndex(i,j)];
    }
    //: Access element.
    
    virtual void Element(UIntT i,UIntT j,const DataT &val) {
      if(j < i) {
	if(val != 0) {
	  cerr << "Attempting to set invalid element of matrix." << i << " " << j << "\n";
	  RavlAssert(0);
	}
	return ;
      }
      this->data[ElementIndex(i,j)] = val;
    }
    //: Set element.
    
    virtual Array1dC<DataT> Row(UIntT i) const
    { return Array1dC<DataT>(const_cast<BufferC<DataT> &>(this->data.Buffer()),
			     RangeBufferAccessC<DataT>(const_cast<DataT *>( &(this->data[RowStart(i)])),IndexRangeC(i,this->Cols()-1))); }
    //: Access a row from the matrix.
    
    virtual bool IsRowDirectAccess() const
    { return true; }
    //: Does Row() give direct access to actual data ?

    virtual DataT MulSumColumn(UIntT c,const Array1dC<DataT> &dat) const;
    //: Multiply column by values from dat and sum them.
    
    virtual Slice1dC<DataT> Col(UIntT j) const;
    //: Access slice from matrix.
    
    virtual bool IsColDirectAccess() const
    { return false; }
    //: Does Col() give direct access to actual data ?
    
    virtual DataT MulSumColumn(UIntT c,const Slice1dC<DataT> &slice) const;
    //: Multiply column by values from slice and sum them.
    
    virtual TSMatrixC<DataT> Mul(const TSMatrixC<DataT> &oth) const;
    //: Get this matrix times 'oth'.
    
#if 0    
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
#endif
    
    virtual TMatrixC<DataT> TMatrix(bool) const;
    //: Get as full matrix.
    
    virtual void SetDiagonal(const TVectorC<DataT> &d) { 
      RavlAssert(0);
    }
    //: Set the diagonal of this matrix.
    // If d.Size() != Cols() an error is given.
    
    virtual void AddDiagonal(const TVectorC<DataT> &d) {  
      RavlAssert(0);
    }
    //: Add a vector to the diagonal of this matrix.
    // If d.Size() != Cols() an error is given.
    
    TVectorC<DataT> Solve(const TVectorC<DataT> &b);
    //: Solve for x, A.x = b
    // Where A is this matrix, and x is returned.
    // !! NOT IMPLEMENTED !!
    
  };
  
  //! userlevel=Normal
  //: RightUpper matrix
  
  template<class DataT>
  class TSMatrixRightUpperC
    : public TSMatrixPartialC<DataT>
  {
  public:
    TSMatrixRightUpperC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    explicit TSMatrixRightUpperC(int i)
      : TSMatrixPartialC<DataT>(*new TSMatrixRightUpperBodyC<DataT>(i))
    {}
    //: Create a diagonal matrix of size i by i .
    // The contents of the matrix are undefined.

    TSMatrixRightUpperC(const TMatrixC<DataT> &ndata)
      : TSMatrixPartialC<DataT>(*new TSMatrixRightUpperBodyC<DataT>(ndata))
    {}
    //: Construct by copying elements from a regular matrix.
    
  protected:
    TSMatrixRightUpperC(TSMatrixRightUpperBodyC<DataT> &bod)
      : TSMatrixPartialC<DataT>(bod)
    {}
    //: Body constructor.
    
    TSMatrixRightUpperBodyC<DataT> &Body()
    { return static_cast<TSMatrixRightUpperBodyC<DataT> &>(TSMatrixC<DataT>::Body()); }
    //: Access body.
    
    const TSMatrixRightUpperBodyC<DataT> &Body() const
    { return static_cast<const TSMatrixRightUpperBodyC<DataT> &>(TSMatrixC<DataT>::Body()); }
    //: Access body.
    
  public:
    friend class TSMatrixRightUpperBodyC<DataT>;
  };
  
  
  template<class DataT>
  TSMatrixRightUpperBodyC<DataT>::TSMatrixRightUpperBodyC(const TMatrixC<DataT> &ndata)
    : TSMatrixPartialBodyC<DataT>(ndata.Rows(),ndata.Cols(),SArray1dC<DataT>(TriangleSize(ndata.Rows())))
  {
    RavlAssert(ndata.Rows() == ndata.Cols());
    DataT *at1 = &(this->data[0]);
    for(UIntT j = 0;j < this->Rows();j++) {
      const DataT *at2 = &(ndata[j][j]);
      DataT *end = &at1[this->Rows() - j];
      for(;at1 != end;at1++,at2++)
	*at1 = *at2;
    }
  }
  
  
  template<class DataT>
  DataT TSMatrixRightUpperBodyC<DataT>::MulSumColumn(UIntT c,const Array1dC<DataT> &dat) const {
    DataT sum;
    if(dat.Size() == 0) {
      SetZero(sum);
      return sum;
    }
    UIntT s = dat.Range().Min().V();
    UIntT end = Min((UIntT) dat.Range().Max().V(),c)+1;
    const DataT *at2 = &(dat[s]);
    const DataT *endp = &(at2[end - s]);
    if(at2 >= endp) {
      SetZero(sum);
      return sum;
    }
    const DataT *at = &this->data[ElementIndex(s,c)];
    sum = (*at2) * (*at);
    UIntT z = (this->Cols() - s) -1;
    at += z;
    for(z--,at2++;at2 != endp;at2++) {
      sum += (*at2) * (*at);
      at += z--;
    }
    return sum;
  }
  
  template<class DataT>
  Slice1dC<DataT> TSMatrixRightUpperBodyC<DataT>::Col(UIntT j) const {
    Slice1dC<DataT> ret(IndexRangeC(0,j));
    const DataT *at = &this->data[j];
    int i = this->Cols()-1;
    for(Slice1dIterC<DataT> it(ret);it;it++) {
      *it = *at;
      at += i--;
    }
    return ret;
  }
  
  template<class DataT>
  DataT TSMatrixRightUpperBodyC<DataT>::MulSumColumn(UIntT c,const Slice1dC<DataT> &slice) const {
    DataT sum;
    IndexRangeC rng(0,c);
    rng.ClipBy(slice.Range());
    if(rng.Size() <= 0) {
      SetZero(sum);
      return sum;
    }
    const DataT *at = &this->data[ElementIndex(rng.Min().V(),c)];
    UIntT z = (this->Cols() - rng.Min().V()) -1;
    Slice1dIterC<DataT> it(slice,rng);
    sum = (*it) * (*at);
    at += z--;
    for(it++;it;it++) {
      sum += (*it) * (*at);
      at += z--;
    }
    return sum;
  }
  
  template<class DataT>
  TSMatrixC<DataT> TSMatrixRightUpperBodyC<DataT>::Mul(const TSMatrixC<DataT> &mat) const {
    if(mat.MatrixType() == typeid(TSMatrixRightUpperBodyC<DataT>)) {
      RavlAssert(this->Cols() == mat.Rows());
      const SizeT rdim = this->Rows();
      const SizeT cdim = mat.Cols();
      TSMatrixRightUpperC<DataT> out(rdim);
      SArray1dIterC<DataT> it(out.Data());
      for (UIntT r = 0; r < rdim; r++) {
	Array1dC<DataT> row = Row(r);
	for (UIntT c = r; c < cdim; c++,it++)
	  *it = mat.MulSumColumn(c,row);
      }
      return out;
    }
    return TSMatrixBodyC<DataT>::Mul(mat); // Use default.
  }
  
#if 0  
  template<class DataT>
  TVectorC<DataT> TSMatrixRightUpperBodyC<DataT>::Mul(const TVectorC<DataT> &oth) const {
    RavlAssert(0);
    return TVectorC<DataT>();
  }
  
  template<class DataT>
  TSMatrixC<DataT> TSMatrixRightUpperBodyC<DataT>::MulT(const TSMatrixC<DataT> & B) const {
    RavlAssert(0);
    return TSMatrixC<DataT>();
  }
  
  template<class DataT>
  TSMatrixC<DataT> TSMatrixRightUpperBodyC<DataT>::TMul(const TSMatrixC<DataT> & B) const {
    RavlAssert(0);
    return TSMatrixC<DataT>();
  }
  
  template<class DataT>
  TVectorC<DataT> TSMatrixRightUpperBodyC<DataT>::TMul(const TVectorC<DataT> & B) const {
    RavlAssert(0);
    return TVectorC<DataT>();
  }  
  
  template<class DataT>
  TSMatrixC<DataT> TSMatrixRightUpperBodyC<DataT>::AAT() const {
    RavlAssert(0);
    return TSMatrixC<DataT>();
  }
  
  template<class DataT>
  TSMatrixC<DataT> TSMatrixRightUpperBodyC<DataT>::ATA() const {
    RavlAssert(0);
    return TSMatrixC<DataT>();
  }
#endif
  
  template<class DataT>
  TMatrixC<DataT> TSMatrixRightUpperBodyC<DataT>::TMatrix(bool) const {
    TMatrixC<DataT> ret(this->Rows(),this->Cols());
    const DataT *at1 = &(this->data[0]);
    for(UIntT j = 0;j < this->Rows();j++) {
      DataT *at2 = &(ret[j][0]);
      DataT *end2 = &(at2[j]);
      for(;at2 != end2;at2++)
	SetZero(*at2);
      const DataT *end = &at1[this->Rows() - j];
      for(;at1 != end;at1++,at2++)
	*at2 = *at1;
    }
    return ret;
  }  
  
#if 0
  //: Solve for x, A.x = b
  // Where A is this matrix, and x is returned.
  
  template<class DataT>
  TVectorC<DataT> TSMatrixRightUpperBodyC<DataT>::Solve(const TVectorC<DataT> &b) {
    TVectorC<DataT> ret(this->Rows());
    for(int i = this->Rows()-1;i >= 0;i--) {
      RealT sum = b[i];
      Array1dC<DataT> row = Row(i);
      
      ret[i] = sum / Element(i,i);
    }
    return ret;
  }
#endif

}

#endif
