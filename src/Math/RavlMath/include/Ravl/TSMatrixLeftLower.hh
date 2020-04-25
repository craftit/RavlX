// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_TSMATRIXLEFTLOWER_HEADER
#define RAVL_TSMATRIXLEFTLOWER_HEADER 1
//! rcsid="$Id$"
//! lib=RavlMath
//! author="Charles Galambos"
//! docentry="Ravl.API.Math.Linear Algebra.Smart Matrix"
//! date="19/8/2002"
//! file="Ravl/Math/LinearAlgebra/General/TSMatrixLeftLower.hh"

#include "Ravl/TSMatrixPartial.hh"
#include "Ravl/TSMatrixSymmetric.hh"
#include "Ravl/Math.hh"

namespace RavlN {
  template<class DataT> class TSMatrixLeftLowerC;
  
  //! userlevel=Develop
  //: LeftLower matrix body
  
  template<class DataT>
  class TSMatrixLeftLowerBodyC
    : public TSMatrixPartialBodyC<DataT>
  {
  public:
    static inline SizeT TriangleSize(int i)
    { return (i * (i+1))/2; }
    //: Size of triangle with size i.
    
    inline SizeT ElementIndex(int i,int j) const
    { return (i * (i+1))/2 + j; }
    //: Find index of element i,j
    
    inline SizeT RowStart(int i) const
    { return (i * (i+1))/2; }
    //: Address of row start.
    
    TSMatrixLeftLowerBodyC(int i)
      : TSMatrixPartialBodyC<DataT>(i,i,SArray1dC<DataT>(TriangleSize(i)))
    {}
    //: Constructor.
    
    TSMatrixLeftLowerBodyC(int i,const SArray1dC<DataT> &ndata)
      : TSMatrixPartialBodyC<DataT>(i,i,ndata)
    {}
    //: Constructor.
    
    TSMatrixLeftLowerBodyC(const TMatrixC<DataT> &ndata);
    //: Construct by coping from a regular matrix.
    
    virtual RCBodyVC &Copy() const
    { return *new TSMatrixLeftLowerBodyC<DataT>(this->Rows(),this->data.Copy()); }
    //: Create a copy of this matrix.
    
    virtual const type_info &MatrixType() const
    { return typeid(TSMatrixLeftLowerBodyC<DataT>); }
    //: Find the type of the matrix.
    
    virtual DataT Element(UIntT i,UIntT j) const { 
      if(j > i)
	return 0;
      return this->data[ElementIndex(i,j)];
    }
    //: Access element.
    
    virtual void Element(UIntT i,UIntT j,const DataT &val) {
      RavlAssertMsg(j <= i,"Attempt to set invalid matrix element.");
      this->data[ElementIndex(i,j)] = val;
    }
    //: Set element.
    
    virtual Array1dC<DataT> Row(UIntT i) const
    { return Array1dC<DataT>(const_cast<BufferC<DataT> &>(this->data.Buffer()),
			     RangeBufferAccessC<DataT>(const_cast<DataT *>( &(this->data[RowStart(i)])),IndexRangeC(0,i))); }
    //: Access a row from the matrix.
    
    virtual bool IsRowDirectAccess() const
    { return true; }
    //: Does Row() give direct access to actual data ?

    virtual DataT MulSumColumn(UIntT c,const Array1dC<DataT> &dat) const;
    //: Multiply column by values from dat and sum them.
    
    virtual TSMatrixC<DataT> Mul(const TSMatrixC<DataT> &oth) const;
    //: Get this matrix times 'oth'.
    
    virtual Slice1dC<DataT> Col(UIntT j) const;
    //: Access slice from matrix.
    
    virtual bool IsColDirectAccess() const
    { return false; }
    //: Does Col() give direct access to actual data ?
    
    virtual DataT MulSumColumn(UIntT c,const Slice1dC<DataT> &slice) const;
    //: Multiply column by values from slice and sum them.
    
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
    
  };
  
  //! userlevel=Normal
  //: LeftLower matrix
  
  template<class DataT>
  class TSMatrixLeftLowerC
    : public TSMatrixPartialC<DataT>
  {
  public:
    TSMatrixLeftLowerC()
    {}
    //: Default constructor.
    // Creates an invalid header.
    
    explicit TSMatrixLeftLowerC(int i)
      : TSMatrixPartialC<DataT>(*new TSMatrixLeftLowerBodyC<DataT>(i))
    {}
    //: Create a diagonal matrix of size i by i .
    // The contents of the matrix are undefined.
    
    TSMatrixLeftLowerC(const TMatrixC<DataT> &ndata)
      : TSMatrixPartialC<DataT>(*new TSMatrixLeftLowerBodyC<DataT>(ndata))
    {}
    //: Construct by coping from a regular matrix.
    
  protected:
    TSMatrixLeftLowerC(TSMatrixLeftLowerBodyC<DataT> &bod)
      : TSMatrixPartialC<DataT>(bod)
    {}
    //: Body constructor.
    
    TSMatrixLeftLowerBodyC<DataT> &Body()
    { return static_cast<TSMatrixLeftLowerBodyC<DataT> &>(TSMatrixC<DataT>::Body()); }
    //: Access body.
    
    const TSMatrixLeftLowerBodyC<DataT> &Body() const
    { return static_cast<const TSMatrixLeftLowerBodyC<DataT> &>(TSMatrixC<DataT>::Body()); }
    //: Access body.
    
  public:
    friend class TSMatrixLeftLowerBodyC<DataT>;
  };
  
}

#include "Ravl/TSMatrixRightUpper.hh"

namespace RavlN {
  
  template<class DataT>
  TSMatrixLeftLowerBodyC<DataT>::TSMatrixLeftLowerBodyC(const TMatrixC<DataT> &ndata) 
    : TSMatrixPartialBodyC<DataT>(ndata.Rows(),ndata.Cols(),SArray1dC<DataT>(TriangleSize(ndata.Rows())))
  {
    RavlAssert(ndata.Rows() == ndata.Cols());
    DataT *at1 = &(this->data[0]);
    for(UIntT j = 0;j < this->Rows();j++) {
      const DataT *at2 = &(ndata[j][0]);
      const DataT *end2 = &(at2[j+1]);
      for(;at2 != end2;at2++,at1++)
	*at1 = *at2;
    }
  }
  
  
  template<class DataT>
  DataT TSMatrixLeftLowerBodyC<DataT>::MulSumColumn(UIntT c,const Array1dC<DataT> &dat) const {
    RavlAssert(c < this->Cols());
    UIntT s = Max((UIntT) dat.Range().Min().V(),c);
    if(dat.Size() == 0 || !dat.Contains(s)){
      DataT ret;
      SetZero(ret);
      return ret;
    }
    const DataT *at2 = &(dat[s]);
    const DataT *endp = &(at2[dat.Range().Max().V()-s + 1]);
    const DataT *at = &this->data[ElementIndex(s,c)];
    DataT sum = (*at2) * (*at);
    s++;
    at += s;
    for(at2++,s++;at2 < endp;s++,at2++) {
      sum += (*at2) * (*at);
      at += s;
    }
    return sum;
  }
  
  template<class DataT>
  Slice1dC<DataT> TSMatrixLeftLowerBodyC<DataT>::Col(UIntT j) const {
    RavlAssert(j < this->Cols());
    const UIntT lrow = this->Rows()-1;
    IndexRangeC rng(j,lrow);
    Slice1dC<DataT> ret(rng);
    const DataT *at = &this->data[ElementIndex(j,j)];
    int i = j+1;
    for(Slice1dIterC<DataT> it(ret);it;it++) {
      *it = *at;
      at += i++;
    }
    //cerr << "Col("<< j << ")=" << ret <<"\n";
    return ret;
  }
  
  template<class DataT>
  DataT TSMatrixLeftLowerBodyC<DataT>::MulSumColumn(UIntT c,const Slice1dC<DataT> &slice) const {
    DataT sum;
    IndexRangeC rng(c,(this->Cols()-1));
    rng.ClipBy(slice.Range());
    if(rng.Size() <= 0) {
      SetZero(sum);
      return sum;
    }
    const DataT *at = &this->data[ElementIndex(rng.Min().V(),c)];
    UIntT z = rng.Min().V() + 1;
    Slice1dIterC<DataT> it(slice,rng);
    sum = (*it) * (*at);
    at += z++;
    for(it++;it;it++) {
      sum += (*it) * (*at);
      at += z++;
    }
    return sum;
  }

  
  template<class DataT>
  TSMatrixC<DataT> TSMatrixLeftLowerBodyC<DataT>::Mul(const TSMatrixC<DataT> &mat) const {
    if(mat.MatrixType() != typeid(TSMatrixLeftLowerBodyC<DataT>))
      return TSMatrixBodyC<DataT>::Mul(mat); // Use default.
    RavlAssert(this->Cols() == mat.Rows());
    const SizeT rdim = this->Rows();
    TSMatrixLeftLowerC<DataT> out(rdim);
    SArray1dIterC<DataT> it(out.Data());
    for (UIntT r = 0; r < rdim; r++) {
      //RangeBufferAccessC<DataT>(const_cast<DataT *> row( &(this->data[RowStart(i)])),IndexRangeC(0,i));
      Array1dC<DataT> row = Row(r);
      for (UIntT c = 0; c <= r; c++,it++) {
	*it = mat.MulSumColumn(c,row);
      }
    }
    return out;
  }
  
#if 0
  template<class DataT>
  TVectorC<DataT> TSMatrixLeftLowerBodyC<DataT>::Mul(const TVectorC<DataT> &oth) const {
    RavlAssert(0);
    return TVectorC<DataT>();
  }
  
  template<class DataT>
  TSMatrixC<DataT> TSMatrixLeftLowerBodyC<DataT>::MulT(const TSMatrixC<DataT> & B) const {
    RavlAssert(0);
    return TSMatrixC<DataT>();
  }
  
  template<class DataT>
  TSMatrixC<DataT> TSMatrixLeftLowerBodyC<DataT>::TMul(const TSMatrixC<DataT> & B) const {
    RavlAssert(0);
    return TSMatrixC<DataT>();
  }
  
  template<class DataT>
  TVectorC<DataT> TSMatrixLeftLowerBodyC<DataT>::TMul(const TVectorC<DataT> & B) const {
    RavlAssert(0);
    return TVectorC<DataT>();
  }  
  
  template<class DataT>
  TSMatrixC<DataT> TSMatrixLeftLowerBodyC<DataT>::AAT() const {
    RavlAssert(0);
    return TSMatrixC<DataT>();
  }
  
  template<class DataT>
  TSMatrixC<DataT> TSMatrixLeftLowerBodyC<DataT>::ATA() const {
    RavlAssert(0);
    return TSMatrixC<DataT>();
  }
#endif
  
  template<class DataT>
  TMatrixC<DataT> TSMatrixLeftLowerBodyC<DataT>::TMatrix(bool) const {
    TMatrixC<DataT> ret(this->Rows(),this->Cols());
    const DataT *at1 = &(this->data[0]);
    for(UIntT j = 0;j < this->Rows();j++) {
      DataT *at2 = &(ret[j][0]);
      const DataT *end = &at2[this->Cols()];
      DataT *end2 = &(at2[j+1]);
      for(;at2 != end2;at2++,at1++)
	*at2 = *at1;
      for(;at2 != end;at2++)
	SetZero(*at2);
    }
    return ret;
  }
  
}

#endif
