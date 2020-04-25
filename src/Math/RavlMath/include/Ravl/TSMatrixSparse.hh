// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_TSMATRIXSPARSE_HEADER
#define RAVL_TSMATRIXSPARSE_HEADER 1
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/LinearAlgebra/General/TSMatrixSparse.hh"
//! author="Charles Galambos"
//! docentry="Ravl.API.Math.Linear Algebra.Smart Matrix"

#include "Ravl/TSMatrix.hh"
#include "Ravl/TSMatrixSymmetric.hh"
#include "Ravl/IntrDList.hh"
#include "Ravl/IntrDLIter.hh"
#include "Ravl/BfAcc2Iter.hh"

namespace RavlN {
  
  template<class DataT> class TSMatrixSparseRowIterC;
  template<class DataT> class TSMatrixSparseColIterC;
  template<class DataT> class TSMatrixSparseC;
  
  //! userlevel=Normal
  //: DLink with index number
  
  class IndexDLinkC
    : public DLinkC
  {
  public:
    IndexDLinkC(IndexC ind)
      : index(ind)
    {}
    //: Create a link with
    
    IndexC Index() const
    { return index; }
    //: Get index of link.
    
  protected:
    IndexC index;
  };

  //! userlevel=Normal
  //: List of indexed links.
  
  class IndexDListC
    : public IntrDListC<IndexDLinkC>
  {
  public:
    IndexDListC()
    {}
    //: Default constructor.
    
    const IndexDLinkC *Find(int i) const;
    //: Find element with given index.
    // Assumes an ordered list.
    
    bool Insert(IndexDLinkC &newun);
    //: Add an item to the list.
    // returns false if its already present.
    
    IndexRangeC Range() const {
      if(IsEmpty())
	return IndexRangeC();
      return IndexRangeC(First().Index(),Last().Index());
    }
    //: Get range of indexes in list.
  };

  //! userlevel=Normal
  //: Iterate through a pair of lists.
  
  class IndexDLIter2C
  {
  public:
    IndexDLIter2C(const IndexDListC &lst1,const IndexDListC &lst2)
      : it1(lst1),
	it2(lst2)
    {}
    //: Construct from a pair of lists.
    
    IndexDLinkC &Data1()
    { return *it1; }
    //: Access data for iter1.

    IndexDLinkC &Data2()
    { return *it2; }
    //: Access data for iter2.
    
    bool NextMatch() {
      while(it1 && it2) {
	if(it1->Index() == it2->Index())
	  return true;
	if(it1->Index() < it2->Index())
	  it1++;
	else
	  it2++;
      }
      return false;
    }
    //: Goto next matching pair of indexes.
    // Returns false if one of the iterators becomes invalid.
    
    void DuelInc() {
      it1++;
      it2++;
    }
    //: Increment both iterators.

    void Next1() 
    { it1++; }
    //: Increment iterator 1.
    
    void Next2() 
    { it2++; }
    //: Increment iterator 2.
    
    bool NextMatchOr2() {
      while(it1 && it2) {
	if(it1->Index() == it2->Index())
	  return true;
	if(it1->Index() < it2->Index()) {
	  it1++;
	  continue;
	}
	return false;
      }
      return false;
    }
    //: Return next matching pair, or valid 2 index.
    // Returns true on a match.
    // returns false if end or it2 is the lesser index.
    
    operator bool() const
    { return it1 && it2; }
    //: Return true if both iterators are valid.
    
    bool IsElm1() const
    { return it1.IsElm(); }
    //: Is iterator 1 valid ?
    
    bool IsElm2() const
    { return it2.IsElm(); }
    //: Is iterator 2 valid ?
    
  protected:    
    IntrDLIterC<IndexDLinkC> it1;
    IntrDLIterC<IndexDLinkC> it2;
  };

  //! userlevel=Develop
  //: Entry in a sparse matrix.
  
  template<class DataT>
  class TSMatrixSparseEntryC
  {
  public:
    TSMatrixSparseEntryC(const Index2dC &ind,const DataT &dat)
      : irow(ind[1]),
	icol(ind[0]),
	data(dat)
    {}
    //: Constructor.
    
    TSMatrixSparseEntryC(UIntT i,UIntT j,const DataT &dat)
      : irow(j),
	icol(i),
	data(dat)
    {}
    //: Constructor.
    
    UIntT Row() const
    { return icol.Index().V(); }
    //: Get row.

    UIntT Col() const
    { return irow.Index().V(); }
    //: Get col.
    
    IndexDLinkC &IRow()
    { return irow; }
    //: Access row link.
    
    IndexDLinkC &ICol()
    { return icol; }
    //: Access column link.
    
    const DataT &Data() const
    { return data; }
    //: Access data.
    
    DataT &Data()
    { return data; }
    //: Access data.
    
  protected:
    IndexDLinkC irow;
    IndexDLinkC icol;
    DataT data;
  };
  
  //! userlevel=Develop
  //: Sparse matrix body
  
  template<class DataT>
  class TSMatrixSparseBodyC
    : public TSMatrixBodyC<DataT>
  {
  public:
    TSMatrixSparseBodyC()
    {}
    //: Default constructor.
    
    TSMatrixSparseBodyC(UIntT nrows,UIntT ncols)
      : TSMatrixBodyC<DataT>(nrows,ncols),
	rows(nrows),
	cols(ncols)
    {}
    //: Construct a matrix of the given size.
    
    TSMatrixSparseBodyC(const TMatrixC<DataT> &mat,const DataT &zeroValue = 1e-6);
    //: Construct a sparse matrix for a full matrix.
    // Any elements with an absolute value smaller than 'zeroValue' are taken as zero.
    
  protected:
    static  TSMatrixSparseEntryC<DataT> *RowDLink2Entry(IndexDLinkC *il)
    { return (TSMatrixSparseEntryC<DataT> *) (((char *) il) - (((char *) &((TSMatrixSparseEntryC<DataT> *) 0)->IRow()) - ((char *) 0))); }
    
    static TSMatrixSparseEntryC<DataT> *ColDLink2Entry(IndexDLinkC *il)
    { return (TSMatrixSparseEntryC<DataT> *) (((char *) il) - (((char *) &((TSMatrixSparseEntryC<DataT> *) 0)->ICol()) - ((char *) 0))); }
    
    const TSMatrixSparseEntryC<DataT> *FindEntry(UIntT i,UIntT j) const { 
      const IndexDLinkC *il = rows[i].Find(j);
      if(il == 0) return 0;
      return RowDLink2Entry(const_cast<IndexDLinkC *>(il));
    }
    //: Find an index.
    
    TSMatrixSparseEntryC<DataT> *FindEntry(UIntT i,UIntT j) 
    { return const_cast<TSMatrixSparseEntryC<DataT> *>(((const TSMatrixSparseBodyC<DataT> *)this)->FindEntry(i,j)); }
    //: Find an index.
    
    void Insert(TSMatrixSparseEntryC<DataT> &entry) {
      rows[entry.Row()].Insert(entry.IRow());
      cols[entry.Col()].Insert(entry.ICol());
    }
    //: Insert an entry.
    
    IndexRangeC RowRange(int i) const 
    { return  rows[i].Range(); }
    //: Get range of column's for row i.

    IndexRangeC ColRange(int i) const 
    { return  cols[i].Range(); }
    //: Get range of rows's for column i.
    
  public:
    virtual ~TSMatrixSparseBodyC() {
      for(SArray1dIterC<IndexDListC> it(rows);it;it++) {
	while(!it->IsEmpty())
	  delete RowDLink2Entry(&(it->First()));
      }
    }
    //: Destructor.
    
    virtual RCBodyVC &Copy() const;
    //: Create a copy of this matrix.
    
    virtual const type_info &MatrixType() const
    { return typeid(TSMatrixSparseBodyC<DataT>); }
    //: Find the type of the matrix.
    
    virtual DataT Element(UIntT i,UIntT j) const { 
      const TSMatrixSparseEntryC<DataT> *entry = FindEntry(i,j);
      if(entry == 0) {
	DataT ret;
	SetZero(ret);
	return ret;
      }
      // Some magic to get the address
      return entry->Data();
    }
    //: Access element.
    
    void Element(UIntT i,UIntT j,const DataT &val) {
      TSMatrixSparseEntryC<DataT> *entry = FindEntry(i,j);
      if(val == 0) { // If exactly zero erase the entry.
	if(entry != 0)
	  delete entry;
	return ;
      }
      if(entry != 0) {
	entry->Data() = val;
	return ;
      }
      Insert(*new TSMatrixSparseEntryC<DataT>(i,j,val));
    }
    //: Set a matrix element.
    
    void ZeroRow(UIntT i) {
      for(IntrDLIterC<IndexDLinkC> it(rows[i]);it;) {
	TSMatrixSparseEntryC<DataT> *entry = RowDLink2Entry(&(*it));
	it++;
	delete entry;
      }
    }
    //: Make all entries in a row zero.
    
    void ZeroCol(UIntT i) {
      for(IntrDLIterC<IndexDLinkC> it(cols[i]);it;) {
	TSMatrixSparseEntryC<DataT> *entry = ColDLink2Entry(&(*it));
	it++;
	delete entry;
      }      
    }
    //: Make all entries in a column zero.
    
    DataT SumRow(UIntT i) const {
      DataT ret;
      SetZero(ret);
      for(IntrDLIterC<IndexDLinkC> it(rows[i]);it;it++)
	ret += RowDLink2Entry(&(*it))->Data();
      return ret;
    }
    //: Compute the sum of the entries from a row.
    
    DataT SumColumn(UIntT i) const {
      DataT ret;
      SetZero(ret);
      for(IntrDLIterC<IndexDLinkC> it(cols[i]);it;it++)
	ret += ColDLink2Entry(&(*it))->Data();
      return ret;
    }
    //: Compute the sum of the entries from a column.
    
    virtual Array1dC<DataT> Row(UIntT i) const;
    //: Access a row from the matrix.

    virtual bool IsRowDirectAccess() const
    { return false; }
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
        
    virtual TMatrixC<DataT> TMatrix(bool alwaysCopy = false) const;
    //: Access as a TMatrix.
    
    virtual void AddIP(const TSMatrixC<DataT> &oth);
    //: Add this matrix to 'oth' and return the result.
    
    virtual void SubIP(const TSMatrixC<DataT> &oth);
    //: Subtract 'oth' from this matrix and return the result.
    
    virtual TSMatrixC<DataT> T() const;
    //: Get transpose of matrix.
    
    virtual TSMatrixC<DataT> Mul(const TSMatrixC<DataT> &oth) const;
    //: Calculate this matrix times 'oth'.
    
    virtual TVectorC<DataT> Mul(const TVectorC<DataT> &oth) const;
    //: Calculate this matrix times 'oth'.
    
    virtual TSMatrixC<DataT> MulT(const TSMatrixC<DataT> & B) const;
    //: Multiplication A * B.T()
    
    virtual TVectorC<DataT> TMul(const TVectorC<DataT> & B) const;
    //: Multiplication A.T() * B
    
    virtual TSMatrixC<DataT> TMul(const TSMatrixC<DataT> & B) const;
    //: Multiplication A.T() * B
    
  protected:
    IndexDListC &RowList(int i)
    { return rows[i]; }
    //: Access list for row i
    
    IndexDListC &ColList(int i)
    { return cols[i]; }
    //: Access list for col i
    
    SArray1dC<IndexDListC > rows;
    SArray1dC<IndexDListC > cols;
    
    friend class TSMatrixSparseC<DataT>;
    friend class TSMatrixSparseRowIterC<DataT>;
    friend class TSMatrixSparseColIterC<DataT>;
  };

  //! userlevel=Normal
  //: General sparse matrix
  
  template<class DataT>
  class TSMatrixSparseC 
    : public TSMatrixC<DataT>
  {
  public:
    TSMatrixSparseC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    TSMatrixSparseC(UIntT rows,UIntT cols)
      : TSMatrixC<DataT>(*new TSMatrixSparseBodyC<DataT>(rows,cols))
    {}
    //: Create a matrix of the given size.
    // The default matrix is filled with zero's.
    
    TSMatrixSparseC(const TMatrixC<DataT> &mat,const DataT &zeroValue = 1e-6)
      : TSMatrixC<DataT>(*new TSMatrixSparseBodyC<DataT>(mat,zeroValue))
    {}
    //: Construct a sparse matrix for a full matrix.
    // Any elements with an absolute value smaller than 'zeroValue' are taken as zero.
    
    TSMatrixSparseC(TSMatrixC<DataT> &xyz)
      : TSMatrixC<DataT>(dynamic_cast<const TSMatrixSparseBodyC<DataT> *>(RCHandleC<TSMatrixBodyC<DataT> >::BodyPtr(xyz)))
    {}
    //: Base constructor.
    
  protected:
    TSMatrixSparseC(TSMatrixSparseBodyC<DataT> &bod)
      : TSMatrixC<DataT>(bod)
    {}
    //: Body constructor.
    
    TSMatrixSparseC(const TSMatrixSparseBodyC<DataT> *bod)
      : TSMatrixC<DataT>(bod)
    {}
    //: Body constructor.
    
    TSMatrixSparseBodyC<DataT> &Body()
    { return static_cast<TSMatrixSparseBodyC<DataT> &>(TSMatrixC<DataT>::Body()); }
    //: Access body.
    
    const TSMatrixSparseBodyC<DataT> &Body() const
    { return static_cast<const TSMatrixSparseBodyC<DataT> &>(TSMatrixC<DataT>::Body()); }
    //: Access body.
    
    IndexDListC &RowList(int i)
    { return Body().RowList(i); }
    //: Access list for row i
    
    IndexDListC &ColList(int i)
    { return Body().ColList(i); }
    //: Access list for col i
    
  public:
    void ZeroRow(UIntT i) 
    { Body().ZeroRow(i); }
    //: Make all entries in a row zero.
    
    void ZeroCol(UIntT i) 
    { Body().ZeroCol(i); }
    //: Make all entries in a column zero.
    
    DataT SumRow(UIntT i) const 
    { return Body().SumRow(i); }
    //: Compute the sum of the entries from a row.
    
    DataT SumColumn(UIntT i) const 
    { return Body().SumColumn(i); }
    //: Compute sum of column
    
    friend class TSMatrixSparseBodyC<DataT>;
    friend class TSMatrixSparseRowIterC<DataT>;
    friend class TSMatrixSparseColIterC<DataT>;

  };

  
  //: Construct a sparse matrix for a full matrix.
  // Any elements with an absolute value smaller than 'zeroValue' are taken as zero.
  
  template<class DataT>
  TSMatrixSparseBodyC<DataT>::TSMatrixSparseBodyC(const TMatrixC<DataT> &mat,const DataT &zeroValue) 
    : TSMatrixBodyC<DataT>(mat.Rows(),mat.Cols())
  {
    SArray2dIterC<DataT> it(mat);
    if(!it)
      return ;
    IndexDListC *rlist = &rows[0];
    do {
      IndexDListC *clist = &(cols[0]);
      do {
	if(Abs(*it) > zeroValue) {
	  TSMatrixSparseEntryC<DataT> &entry = *new TSMatrixSparseEntryC<DataT>(it.Index(),*it);
	  rlist->InsLast(entry.IRow());
	  clist->InsLast(entry.ICol());
	}
	clist++;
      } while(it.Next()) ;
      rlist++;
    } while(it) ;
  }

  //: Create a copy of this matrix.
  
  template<class DataT>
  RCBodyVC &TSMatrixSparseBodyC<DataT>::Copy() const {
    TSMatrixSparseBodyC<DataT> &newBod = *new TSMatrixSparseBodyC<DataT>(this->Rows(),this->Cols());
    IndexDListC *rlist = &(newBod.rows[0]);
    for(UIntT i = 0;i < this->Rows();i++,rlist++) {
      for(IntrDLIterC<IndexDLinkC> it(rows[i]);it;it++) {
	TSMatrixSparseEntryC<DataT> *entry = RowDLink2Entry(&(*it));
	UIntT ncol = it->Index().V();
	TSMatrixSparseEntryC<DataT> &newentry = *new TSMatrixSparseEntryC<DataT>(i,ncol,entry->Data());
	rlist->InsLast(newentry.IRow());
	newBod.cols[ncol].InsLast(newentry.ICol());
      }
    }
    return newBod;
  }

  
  //: Access a row from the matrix.
  
  template<class DataT>
  Array1dC<DataT> TSMatrixSparseBodyC<DataT>::Row(UIntT i) const {
    Array1dC<DataT> ret(RowRange(i));
    if(ret.IsEmpty())
      return ret;
    DataT *rstart = ret.ReferenceElm();
    DataT *at = &(rstart[ret.IMin().V()]);
    IntrDLIterC<IndexDLinkC> it(rows[i]);
    *at = RowDLink2Entry(&(*it))->Data();
    for(it++,at++;it;it++,at++) {
      DataT *next = &(rstart[it->Index().V()]);
      for(;at < next;at++)
	SetZero(*at);
      *at = RowDLink2Entry(&(*it))->Data();
    }
    return ret;
  }
  
  //: Multiply column by values from dat and sum them.
  
  template<class DataT>
  DataT TSMatrixSparseBodyC<DataT>::MulSumColumn(UIntT c,const Array1dC<DataT> &dat) const {
    DataT sum;
    IntrDLIterC<IndexDLinkC> it(cols[c]);
    for(;it;it++) {
      if(it->Index() >= dat.IMin())
	break;
    }
    if(!it || (it->Index() > dat.IMax())) {
      SetZero(sum);     
      return sum;
    }
    sum = dat[it->Index()] * ColDLink2Entry(&(*it))->Data();
    for(it++;it && (it->Index() <= dat.IMax());it++)
      sum += dat[it->Index()] * ColDLink2Entry(&(*it))->Data();
    return sum;
  }
  
  template<class DataT>
  Slice1dC<DataT> TSMatrixSparseBodyC<DataT>::Col(UIntT i) const {
    Slice1dC<DataT> ret(ColRange(i));
    if(ret.IsEmpty())
      return ret;
    DataT *rstart = &ret.ReferenceElm();
    DataT *at = &(rstart[ret.IMin().V()]);
    IntrDLIterC<IndexDLinkC> it(cols[i]);
    *at = ColDLink2Entry(&(*it))->Data();
    for(it++,at++;it;it++,at++) {
      DataT *next = &(rstart[it->Index().V()]);
      for(;at < next;at++)
	SetZero(*at);
      *at = ColDLink2Entry(&(*it))->Data();
    }
    //    cerr << "Col(" << i << ")=" << ret << "\n";
    return ret;    
  }
  
  template<class DataT>
  DataT TSMatrixSparseBodyC<DataT>::MulSumColumn(UIntT c,const Slice1dC<DataT> &slice) const {
    DataT sum;
    IntrDLIterC<IndexDLinkC> it(cols[c]);
    for(;it;it++) {
      if(it->Index() >= slice.IMin())
	break;
    }
    if(!it || (it->Index() > slice.IMax())) {
      SetZero(sum);
      return sum;
    }
    
    sum = slice[it->Index()] * ColDLink2Entry(&(*it))->Data();
    for(it++;it && (it->Index() <= slice.IMax());it++)
      sum += slice[it->Index()] * ColDLink2Entry(&(*it))->Data();
    return sum;
  }
  
  template<class DataT>
  TSMatrixC<DataT> TSMatrixSparseBodyC<DataT>::Mul(const TSMatrixC<DataT> &mat) const {
    RavlAssert(this->Cols() == mat.Rows());
    if(mat.MatrixType() != typeid(TSMatrixSparseBodyC<DataT>))
      return TSMatrixBodyC<DataT>::Mul(mat);
    TSMatrixSparseC<DataT> smat(const_cast<TSMatrixC<DataT> &>(mat));
    const SizeT rdim = this->Rows();
    const SizeT cdim = mat.Cols();
    TSMatrixSparseC<DataT> out(rdim, cdim);
    TSMatrixSparseBodyC<DataT> &outb = out.Body();
    for (UIntT r = 0; r < rdim; r++) {
      const IndexDListC &rowl = rows[r];
      for (UIntT c = 0; c < cdim; c++) {
	IndexDListC &coll = smat.Body().cols[c];
	IndexDLIter2C it(rowl,coll);
	if(!it.NextMatch())
	  continue;
	DataT sum = RowDLink2Entry(&it.Data1())->Data() * ColDLink2Entry(&(it.Data2()))->Data();
	it.DuelInc();
	while(it.NextMatch()) {
	  sum += RowDLink2Entry(&it.Data1())->Data() * ColDLink2Entry(&(it.Data2()))->Data();
	  it.DuelInc();
	}
	TSMatrixSparseEntryC<DataT> &newentry = *new TSMatrixSparseEntryC<DataT>(r,c,sum);
	outb.rows[r].InsLast(newentry.IRow());
	outb.cols[c].InsLast(newentry.ICol());
      }
    }
    return out;
  }
  
  template<class DataT>
  TVectorC<DataT> TSMatrixSparseBodyC<DataT>::Mul(const TVectorC<DataT> &vector) const {
    RavlAssert(vector.Size() == this->Cols());
    const SizeT rdim = this->Rows();
    TVectorC<DataT> out(rdim);
    for (UIntT i = 0; i < rdim; ++i) {
      DataT sum;
      IntrDLIterC<IndexDLinkC> it(rows[i]);
      if(!it) {
	SetZero(out[i]);
	continue;
      }
      sum = vector[it->Index()] * RowDLink2Entry(&(*it))->Data();
      for(it++;it;it++)
	sum += vector[it->Index()] * RowDLink2Entry(&(*it))->Data();
      out[i] = sum;
    }
    return out;
  }

  template<class DataT>
  TSMatrixC<DataT> TSMatrixSparseBodyC<DataT>::MulT(const TSMatrixC<DataT> & mat) const {
    RavlAssert(this->Cols() == mat.Cols());
    if(mat.MatrixType() != typeid(TSMatrixSparseBodyC<DataT>)) 
      return TSMatrixBodyC<DataT>::MulT(mat);
    TSMatrixSparseC<DataT> smat(const_cast<TSMatrixC<DataT> &>(mat));
    const SizeT rdim = this->Rows();
    const SizeT cdim = mat.Rows();
    TSMatrixSparseC<DataT> out(rdim, cdim);
    TSMatrixSparseBodyC<DataT> &outb = out.Body();
    for (UIntT r = 0; r < rdim; r++) {
      const IndexDListC &rowl = rows[r];
      for (UIntT c = 0; c < cdim; c++) {
	IndexDListC &coll = smat.Body().rows[c];
	IndexDLIter2C it(rowl,coll);
	if(!it.NextMatch())
	  continue;
	DataT sum = RowDLink2Entry(&it.Data1())->Data() * RowDLink2Entry(&(it.Data2()))->Data();
	it.DuelInc();
	while(it.NextMatch()) {
	  sum += RowDLink2Entry(&it.Data1())->Data() * RowDLink2Entry(&(it.Data2()))->Data();
	  it.DuelInc();
	}
	TSMatrixSparseEntryC<DataT> &newentry = *new TSMatrixSparseEntryC<DataT>(r,c,sum);
	outb.rows[r].InsLast(newentry.IRow());
	outb.cols[c].InsLast(newentry.ICol());
      }
    }
    return out;    
  }
  
  template<class DataT>
  TVectorC<DataT> TSMatrixSparseBodyC<DataT>::TMul(const TVectorC<DataT> & vector) const {
    RavlAssert(vector.Size() == this->Rows());
    const SizeT rdim = this->Cols();
    TVectorC<DataT> out(rdim);
    for (UIntT i = 0; i < rdim; ++i) {
      DataT sum;
      IntrDLIterC<IndexDLinkC> it(cols[i]);
      for(;it;it++) {
	if(it->Index() >= vector.IMin())
	  break;
      }
      if(!it) {
	SetZero(out[i]);
	continue;
      }
      sum = vector[it->Index()] * ColDLink2Entry(&(*it))->Data();
      for(it++;it;it++)
	sum += vector[it->Index()] * ColDLink2Entry(&(*it))->Data();
      out[i] = sum;
    }
    return out;    
  }
  
  template<class DataT>
  TSMatrixC<DataT> TSMatrixSparseBodyC<DataT>::TMul(const TSMatrixC<DataT> & mat) const {
    RavlAssert(this->Rows() == mat.Rows());
    if(mat.MatrixType() != typeid(TSMatrixSparseBodyC<DataT>))
      return TSMatrixBodyC<DataT>::TMul(mat);
    TSMatrixSparseC<DataT> smat(const_cast<TSMatrixC<DataT> &>(mat));
    const SizeT rdim = this->Cols();
    const SizeT cdim = mat.Cols();
    TSMatrixSparseC<DataT> out(rdim, cdim);
    TSMatrixSparseBodyC<DataT> &outb = out.Body();
    for (UIntT r = 0; r < rdim; r++) {
      const IndexDListC &rowl = cols[r];
      for (UIntT c = 0; c < cdim; c++) {
	IndexDListC &coll = smat.Body().cols[c];
	IndexDLIter2C it(rowl,coll);
	if(!it.NextMatch())
	  continue;
	DataT sum = ColDLink2Entry(&it.Data1())->Data() * ColDLink2Entry(&(it.Data2()))->Data();
	it.DuelInc();
	while(it.NextMatch()) {
	  sum += ColDLink2Entry(&it.Data1())->Data() * ColDLink2Entry(&(it.Data2()))->Data();
	  it.DuelInc();
	}
	TSMatrixSparseEntryC<DataT> &newentry = *new TSMatrixSparseEntryC<DataT>(r,c,sum);
	outb.rows[r].InsLast(newentry.IRow());
	outb.cols[c].InsLast(newentry.ICol());
      }
    }
    return out;
  }
  
  template<class DataT>
  void TSMatrixSparseBodyC<DataT>::AddIP(const TSMatrixC<DataT> &mat) {
    //cerr << "TSMatrixSparseBodyC<DataT>::AddIP(), Called. \n";
    RavlAssert(this->Rows() == mat.Rows());
    RavlAssert(this->Cols() == mat.Cols());
    if(mat.MatrixType() != typeid(TSMatrixSparseBodyC<DataT>)) {
      RavlAssert(0);
    }
    TSMatrixSparseC<DataT> smat(const_cast<TSMatrixC<DataT> &>(mat));
    const SizeT rdim = this->Rows();
    for (UIntT r = 0; r < rdim; r++) {
      IntrDListC<IndexDLinkC> &rlist = rows[r]; 
      IntrDLIterC<IndexDLinkC> it1(rlist);
      IntrDLIterC<IndexDLinkC> it2(smat.Body().rows[r]);
      while(it1 && it2) {
	UIntT col2 = it2->Index().V();
	UIntT col1 = it1->Index().V(); 
	if(col1 == col2) {
	  RowDLink2Entry(&(*it1))->Data() += RowDLink2Entry(&(*it2))->Data();
	  it1++;
	  it2++;
	  continue;
	}
	if(col1 < col2) {
	  it1++;
	  continue;
	}
	TSMatrixSparseEntryC<DataT> &newentry = *new TSMatrixSparseEntryC<DataT>(r,col2,RowDLink2Entry(&(*it2))->Data());
	it1.InsertBef(newentry.IRow());
	cols[col2].Insert(newentry.ICol());
	it2++;
      }
      for(;it2;it2++) {
	UIntT col2 = it2->Index().V();
	TSMatrixSparseEntryC<DataT> &newentry = *new TSMatrixSparseEntryC<DataT>(r,col2,RowDLink2Entry(&(*it2))->Data());
	rlist.InsLast(newentry.IRow());
	cols[col2].Insert(newentry.ICol());
      }
    }
  }
  
  template<class DataT>
  void TSMatrixSparseBodyC<DataT>::SubIP(const TSMatrixC<DataT> &mat) {
    cerr << "TSMatrixSparseBodyC<DataT>::AddIP(), Called. \n";
    RavlAssert(this->Rows() == mat.Rows());
    RavlAssert(this->Cols() == mat.Cols());
    if(mat.MatrixType() != typeid(TSMatrixSparseBodyC<DataT>)) {
      RavlAssert(0);
    }
    TSMatrixSparseC<DataT> smat(const_cast<TSMatrixC<DataT> &>(mat));
    const SizeT rdim = this->Rows();
    for (UIntT r = 0; r < rdim; r++) {
      IntrDListC<IndexDLinkC> &rlist = rows[r]; 
      IntrDLIterC<IndexDLinkC> it1(rlist);
      IntrDLIterC<IndexDLinkC> it2(smat.Body().rows[r]);
      while(it1 && it2) {
	UIntT col2 = it2->Index().V();
	UIntT col1 = it1->Index().V(); 
	if(col1 == col2) {
	  RowDLink2Entry(&(*it1))->Data() -= RowDLink2Entry(&(*it2))->Data();
	  it1++;
	  it2++;
	  continue;
	}
	if(col1 < col2) {
	  it1++;
	  continue;
	}
	TSMatrixSparseEntryC<DataT> &newentry = *new TSMatrixSparseEntryC<DataT>(r,col2,RowDLink2Entry(&(*it2))->Data());
	it1.InsertBef(newentry.IRow());
	cols[col2].Insert(newentry.ICol());
	it2++;
      }
      for(;it2;it2++) {
	UIntT col2 = it2->Index().V();
	TSMatrixSparseEntryC<DataT> &newentry = *new TSMatrixSparseEntryC<DataT>(r,col2,-RowDLink2Entry(&(*it2))->Data());
	rlist.InsLast(newentry.IRow());
	cols[col2].Insert(newentry.ICol());
      }
    }
  }
  
  template<class DataT>
  TSMatrixC<DataT> TSMatrixSparseBodyC<DataT>::T() const {
    TSMatrixSparseC<DataT> ret(this->Cols(),this->Rows());
    TSMatrixSparseBodyC<DataT> &newBod = ret.Body();
    IndexDListC *rlist = &(newBod.rows[0]);
    for(UIntT i = 0;i < this->Cols();i++,rlist++) {
      for(IntrDLIterC<IndexDLinkC> it(cols[i]);it;it++) {
	TSMatrixSparseEntryC<DataT> *entry = ColDLink2Entry(&(*it));
	UIntT ncol = it->Index().V();
	TSMatrixSparseEntryC<DataT> &newentry = *new TSMatrixSparseEntryC<DataT>(i,it->Index().V(),entry->Data());
	rlist->InsLast(newentry.IRow());
	newBod.cols[ncol].InsLast(newentry.ICol());
      }
    }
    
    return ret;
  }
  
  template<class DataT>
  TMatrixC<DataT> TSMatrixSparseBodyC<DataT>::TMatrix(bool) const {
    TMatrixC<DataT> ret(this->Rows(),this->Cols());
    for(UIntT i = 0;i < this->Rows();i++) {
      DataT *rstart = &(ret[i][0]);
      DataT *at = rstart;
      for(IntrDLIterC<IndexDLinkC> it(rows[i]);it;it++,at++) {
	DataT *next = &(rstart[it->Index().V()]);
	for(;at < next;at++)
	  SetZero(*at);
	*at = RowDLink2Entry(&(*it))->Data();
      }
      DataT *eor = &(rstart[this->Cols()]);
      for(;at != eor;at++)
	SetZero(*at);
    }
    return ret;
  }

  
}


#endif
