// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLTFMATRIX_HEADER
#define RAVLTFMATRIX_HEADER 1
///////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Core/Math/TFMatrix.hh"
//! lib=RavlCore
//! author="Charles Galambos"
//! date="24/01/2001"
//! docentry="Ravl.API.Math"

#include "Ravl/TFVector.hh"
#include "Ravl/Index2d.hh"
#include "Ravl/SBfAcc.hh"

namespace RavlN {
  
  class BinIStreamC;
  class BinOStreamC;
  
  //! userlevel=Advanced
  //: Fixed size NxM matrix.
  // Provides basic matrix arithmetic operations for
  // an arbitrary sized matrix.
  
  template<class DataT,unsigned int N,unsigned int M>
  class TFMatrixC {
  public:
    TFMatrixC()
    {}
    //: Default constructor.

    TFMatrixC(const DataT *init);
    //: Constructor.
    // Initalise matrix with values from 'init'.
    
    SizeT Size1() const
    { return N; }
    //: Get size of matrix in the first dimension

    SizeT Size2() const
    { return M; }
    //: Get size of matrix in the second dimension

    SizeT Rows() const
    { return N; }
    //: Get the number of rows in the matrix

    SizeT Cols() const
    { return M; }
    //: Get the number of columns in the matrix
    
    bool Contains(const Index2dC &i) const
    { return ((UIntT) i.Row().V()) < Size1() && ((UIntT) i.Col().V()) < Size2(); }
    //: Test if array contains index i�
    
    DataT &operator[](const Index2dC &ind) { 
#if RAVL_CHECK
      if(!Contains(ind)) 
	IssueError(__FILE__,__LINE__,"Index %d,%d out of range, 0 - %u,0 - %u",ind[0].V(),ind[1].V(),N,M);
#endif
      return data[ind.Row().V()][ind.Col().V()]; 
    }
    //: Access item.
    
    const DataT &operator[](const Index2dC &ind) const { 
#if RAVL_CHECK
      if(!Contains(ind))
	IssueError(__FILE__,__LINE__,"Index %d,%d out of range, 0 - %u , 0 - %u",ind[0].V(),ind[1].V(),N,M);
#endif
      return data[ind.Row().V()][ind.Col().V()]; 
    }
    //: Access item.
    
    SizeBufferAccessC<DataT> operator[](UIntT ind) { 
#if RAVL_CHECK
      if(ind >= Size1())
	IssueError(__FILE__,__LINE__,"Index %d out of range, 0 - %u",ind,N);
#endif
      return SizeBufferAccessC<DataT>((DataT *)(data[ind]),M);
    }
    //: Access item.

    const SizeBufferAccessC<DataT> operator[](UIntT ind) const { 
#if RAVL_CHECK
      if(ind >= Size1())
	IssueError(__FILE__,__LINE__,"Index %d out of range, 0 - %u ",ind,N);
#endif
      return SizeBufferAccessC<DataT>((DataT *)(data[ind]),M);
    }
    //: Access item.

    void Fill(const DataT &dat);
    //: Fill array with value 'dat'.
    
    bool operator==(const TFMatrixC<DataT,N,M> &oth) const;
    //: Is exactly equal ?

    bool operator!=(const TFMatrixC<DataT,N,M> &oth) const;
    //: Is not exactly equal ?
    
    TFMatrixC<DataT,N,M> operator+(const TFMatrixC<DataT,N,M> & mat) const;
    //: Sum 2 matrixes.

    TFMatrixC<DataT,N,M> operator-(const TFMatrixC<DataT,N,M> & mat) const;
    //: Subtract 2 matrixes.

    TFMatrixC<DataT,N,M> operator*(const DataT &val) const;
    //: Multiply all elements of the matrix by a scalar 'val'.
    // put results in a new matrix

    TFMatrixC<DataT,N,M> operator/(const DataT &val) const;
    //: Divide all elements of the matrix by a scalar 'val'.
    // put results in a new matrix

    const TFMatrixC<DataT,N,M> &operator+=(const TFMatrixC<DataT,N,M> & mat);
    //: Add another matrix to this one.

    const TFMatrixC<DataT,N,M> &operator-=(const TFMatrixC<DataT,N,M> & mat);
    //: Subtract another matrix from this one.
    
    const TFMatrixC<DataT,N,M> &operator*=(const DataT &val);
    //: Multiply all elements of this matrix by a scalar 'val'.
    
    const TFMatrixC<DataT,N,M> &operator/=(const DataT &val);
    //: Divide all elements of this matrix by a scalar 'val'.
    
    TFVectorC<DataT,N> operator*(const TFVectorC<DataT,M> & mat) const;
    //: Multiply vector by the matrix.

    template<unsigned int MT>
    TFMatrixC<DataT,N,MT> operator*(const TFMatrixC<DataT,M,MT> & mat) const
    {
      TFMatrixC<DataT,N,MT> ret;
      for(UIntT i = 0;i < N;i++)
	for(UIntT j = 0;j < MT;j++) {
	  DataT &val = ret[i][j];
	  val = data[i][0] * mat[0][j];
	  for(UIntT k = 1;k < M;k++)
	    val += data[i][k] * mat[k][j];
	}
      return ret;
    }
    //: Mutiply two matrixes.
    
    TFVectorC<DataT,M> SliceRow(IndexC r) const {
      TFVectorC<DataT,M> ret;
      for(UIntT c = 0; c < M; c++)
	ret[c] = data[r.V()][c];
      return ret;
    }
    //: Access to row as a vector
    
    TFVectorC<DataT,N> SliceCol(IndexC c) const {
      TFVectorC<DataT,N> ret;
      for(UIntT r = 0; r < N; r++)
	ret[r] = data[r][c.V()];
      return ret;
    }
    // Access to column as a vector
    
    template<unsigned int MT>
    TFMatrixC<DataT,M,MT> TMul(const TFMatrixC<DataT,N,MT> & mat) const 
    {
      TFMatrixC<DataT,M,MT> ret;
      for(UIntT i = 0;i < M;i++)
	for(UIntT j = 0;j < MT;j++) {
	  DataT &val = ret[i][j];
	  val = data[0][i] * mat[0][j];
	  for(UIntT k = 1;k < N;k++)
	    val += data[k][i] * mat[k][j];
	}
      return ret;
    }
    //: Transpose this matrix and Multiply by 'mat'
    
    template<unsigned int NT>
    TFMatrixC<DataT,N,NT> MulT(const TFMatrixC<DataT,NT, M> & mat) const {
      TFMatrixC<DataT,N,NT> ret;
      for(UIntT i = 0;i < N;i++)
	for(UIntT j = 0;j < NT;j++) {
	  DataT &val = ret[i][j];
	  val = data[i][0] * mat[j][0];
	  for(UIntT k = 1; k < M;k++)
	    val += data[i][k] * mat[j][k];
	}
      return ret;
    }
    //: Mutiply this matrix by transpose of 'mat'
    
    TFVectorC<DataT,M> TMul(const TFVectorC<DataT,N>& vec) const  {
      TFVectorC<DataT,M> ret;
      for(UIntT i = 0; i < M; i++)
	{
	  ret[i] = data[0][i] * vec[0];
	  for(UIntT j = 1; j < N;j++)
	    ret[i] += data[j][i] * vec[j];
	}
      return ret;
    }
    //: Transpose this matrix and multiply by 'vec'

    TFMatrixC<DataT,M,N> T() const;
    //: Matrix transpose.
    
    DataT SumOfAbs() const;
    //: Return the sum of the absolute values of the matrix.
    
    DataT SumOfSqr() const;
    //: Calculate the sum of the squares of all the elements in the matrix
    
    inline bool Limit(const DataT &min,const DataT &max);
    //: Limit all values in this matrix to between min and max.
    // Returns true if all values in the matrix are between the limits.
    
    static TFMatrixC<DataT,N,M> I();
    //: Create an identity matrix.
    
    const TFMatrixC<DataT,N,M> &SetDiagonal(const TFVectorC<DataT,N> &d);
    //: Set the diagonal of this matrix.
    
    const TFMatrixC<DataT,N,M> &AddDiagonal(const TFVectorC<DataT,N> &d);
    //: Add a vector to the diagonal of this matrix.
    
    SizeT Hash() const {
      SizeT val = 0;
      for(unsigned i = 0;i < N;i++) {
        for(unsigned j = 0;j < M;j++) {
          val += StdHash(data[i][j]);
          val += val << 11;
        }
      }
      return false;
    }
    //: Generate hash value.
  protected:

    DataT data[N][M];

    //friend class TFMatrixC<DataT,M,N>; // Make the transpose a friend.

  };

  template<class DataT,unsigned int N,unsigned int M>
  inline
  void SetZero(TFMatrixC<DataT,N,M> &x) { 
    DataT xv;
    SetZero(xv);
    x.Fill(xv);
  }
  //: Set vector to zero.

  template<class DataT,unsigned int N,unsigned int M>
  void MulAdd(const TFMatrixC<DataT,N,M> &mat,const TFVectorC<DataT,M> &vec,const TFVectorC<DataT,N> &add,TFVectorC<DataT,N> &result) {
    for(unsigned int i = 0;i < N;i++) {
      result[i] = add[i] + mat[i][0]*vec[0];
      for(unsigned int j = 1;j < M;j++) 
        result[i] += mat[i][j]*vec[j];
    }
  }
  //: Compute result = vec * mat + add;
  // Unfortunately return by value is a little slow, this gets around that by passing the location to store
  // the result.
  
  template<class DataT,unsigned int N,unsigned int M>
  void Mul(const TFMatrixC<DataT,N,M> &mat,const TFVectorC<DataT,M> &vec,TFVectorC<DataT,N> &result) {
    for(unsigned int i = 0;i < N;i++) {
      result[i] = mat[i][0]*vec[0];
      for(unsigned int j = 1;j < M;j++) 
	result[i] += mat[i][j]*vec[j];
    }
  }
  //: Compute result = vec * mat;
  // Unfortunately return by value is a little slow, this gets around that by passing the location to store
  // the result.

  template<class DataT,unsigned int N,unsigned int M,unsigned int MT>
  void MulM(const TFMatrixC<DataT,N,M> &fmat,const TFMatrixC<DataT,M,MT> &mat,TFMatrixC<DataT,N,MT> &result) {
    for(UIntT i = 0;i < N;i++)
      for(UIntT j = 0;j < MT;j++) {
	DataT &val = result[i][j];
	val = fmat[i][0] * mat[0][j];
	for(UIntT k = 1;k < M;k++)
	  val += fmat[i][k] * mat[k][j];
      }
  }
  //: Compute result = fmat * mat;
  // Unfortunately return by value is a little slow, this gets around that by passing the location to store
  // the result.
  
  template<class DataT,unsigned int N,unsigned int M>
  void TMul(const TFMatrixC<DataT,N,M> &mat,const TFVectorC<DataT,N>& vec,TFVectorC<DataT,M> &result) {
    for(UIntT i = 0; i < M; i++) {
      DataT &val = result[i];
      val = mat[0][i] * vec[0];
      for(UIntT j = 1; j < N;j++)
	val += mat[j][i] * vec[j];
    }
  }
  //: Compute result = mat.T() * vec;
  // Unfortunately return by value is a little slow, this gets around that by passing the location to store
  // the result.
  // Transpose this matrix and multiply by 'vec'
  
  template<class DataT,unsigned int N,unsigned int M,unsigned int NT>
  void MulT(const TFMatrixC<DataT,N,M> &fmat,const TFMatrixC<DataT,NT, M> & mat ,TFMatrixC<DataT,N,NT> &result) {
    for(UIntT i = 0;i < N;i++)
      for(UIntT j = 0;j < NT;j++) {
	DataT &val = result[i][j];
	val = fmat[i][0] * mat[j][0];
	for(UIntT k = 1; k < M;k++)
	  val += fmat[i][k] * mat[j][k];
      }
  }
  //: Multiply matrix fmat by transpose of 'mat'
  // Unfortunately return by value is a little slow, this gets around that by passing the location to store
  // the result.
  
  template<class DataT,unsigned int N,unsigned int M>
  std::ostream &operator<<(std::ostream &s,const TFMatrixC<DataT,N,M> &oth) {
    for(UIntT i = 0;i < N;i++) {
      for(UIntT j = 0;j < M;j++)
	s << oth[i][j] << ' ';
      s << '\n';
    }
    return s;
  }

  template<class DataT,unsigned int N,unsigned int M>
  std::istream &operator>>(std::istream &s,TFMatrixC<DataT,N,M> &oth) {
    for(UIntT i = 0;i < N;i++)
      for(UIntT j = 0;j < M;j++)
	s >> oth[i][j];
    return s;
  }
  
  template<class DataT,unsigned int N,unsigned int M>
  BinIStreamC &operator>>(BinIStreamC &s,TFMatrixC<DataT,N,M> &oth) {
    for(UIntT i = 0;i < N;i++) {
      for(UIntT j = 0;j < M;j++)
	s >> oth[i][j];
    }
    return s;
  }
  
  template<class DataT,unsigned int N,unsigned int M>
  BinOStreamC &operator<<(BinOStreamC &s,const TFMatrixC<DataT,N,M> &oth) {    
    for(UIntT i = 0;i < N;i++)
      for(UIntT j = 0;j < M;j++)
	s << oth[i][j];
    return s;
  }

  template<class DataT,unsigned int N,unsigned int M>
  TFMatrixC<DataT,N,M>::TFMatrixC(const DataT *init) {
    const DataT *place = init;
    for(UIntT i = 0;i < N;i++)
      for(UIntT j = 0;j < M;j++)
	data[i][j] = *(place++);
  }
  
  template<class DataT,unsigned int N,unsigned int M>
  void TFMatrixC<DataT,N,M>::Fill(const DataT &dat) {
    for(UIntT i = 0;i < N;i++)
      for(UIntT j = 0;j < M;j++)
	data[i][j] = dat;
  }

  template<class DataT,unsigned int N,unsigned int M>
  bool TFMatrixC<DataT,N,M>::operator==(const TFMatrixC<DataT,N,M> &oth) const {
    for(UIntT i = 0;i < N;i++)
      for(UIntT j = 0;j < M;j++)
	if(data[i][j] != oth.data[i][j])
	  return false;
    return true;
  }
  
  template<class DataT,unsigned int N,unsigned int M>
  bool TFMatrixC<DataT,N,M>::operator!=(const TFMatrixC<DataT,N,M> &oth) const {
    for(UIntT i = 0;i < N;i++)
      for(UIntT j = 0;j < M;j++)
	if(data[i][j] != oth.data[i][j])
	  return true;
    return false;
  }

  template<class DataT,unsigned int N,unsigned int M>
  TFMatrixC<DataT,N,M> TFMatrixC<DataT,N,M>::operator+(const TFMatrixC<DataT,N,M> & mat) const {
    TFMatrixC<DataT,N,M> ret;
    for(UIntT i = 0;i < N;i++)
      for(UIntT j = 0;j < M;j++)
	ret.data[i][j] = data[i][j] + mat.data[i][j];
    return ret;
  }

  template<class DataT,unsigned int N,unsigned int M>
  TFMatrixC<DataT,N,M> TFMatrixC<DataT,N,M>::operator-(const TFMatrixC<DataT,N,M> & mat) const {
    TFMatrixC<DataT,N,M> ret;
    for(UIntT i = 0;i < N;i++)
      for(UIntT j = 0;j < M;j++)
	ret.data[i][j] = data[i][j] - mat.data[i][j];
    return ret;
  }

  template<class DataT,unsigned int N,unsigned int M>
  TFMatrixC<DataT,N,M> TFMatrixC<DataT,N,M>::operator*(const DataT & val) const {
    TFMatrixC<DataT,N,M> ret;
    for(UIntT i = 0;i < N;i++)
      for(UIntT j = 0;j < M;j++)
	ret.data[i][j] = data[i][j] * val;
    return ret;
  }

  template<class DataT,unsigned int N,unsigned int M>
  TFMatrixC<DataT,N,M> TFMatrixC<DataT,N,M>::operator/(const DataT & val) const {
    TFMatrixC<DataT,N,M> ret;
    for(UIntT i = 0;i < N;i++)
      for(UIntT j = 0;j < M;j++)
	ret.data[i][j] = data[i][j] / val;
    return ret;
  }
  
  template<class DataT,unsigned int N,unsigned int M>
  const TFMatrixC<DataT,N,M> &TFMatrixC<DataT,N,M>::operator+=(const TFMatrixC<DataT,N,M> & mat) {
    for(UIntT i = 0;i < N;i++)
      for(UIntT j = 0;j < M;j++)
	data[i][j] += mat.data[i][j];
    return *this;
  }
  
  template<class DataT,unsigned int N,unsigned int M>
  const TFMatrixC<DataT,N,M> &TFMatrixC<DataT,N,M>::operator-=(const TFMatrixC<DataT,N,M> & mat) {
    for(UIntT i = 0;i < N;i++)
      for(UIntT j = 0;j < M;j++)
	data[i][j] -= mat.data[i][j];
    return *this;
  }

  template<class DataT,unsigned int N,unsigned int M>
  const TFMatrixC<DataT,N,M> &TFMatrixC<DataT,N,M>::operator*=(const DataT & val) {
    for(UIntT i = 0;i < N;i++)
      for(UIntT j = 0;j < M;j++)
	data[i][j] *= val;
    return *this;
  }

  template<class DataT,unsigned int N,unsigned int M>
  const TFMatrixC<DataT,N,M> &TFMatrixC<DataT,N,M>::operator/=(const DataT & val) {
    for(UIntT i = 0;i < N;i++)
      for(UIntT j = 0;j < M;j++)
	data[i][j] /= val;
    return *this;
  }
  
  template<class DataT,unsigned int N,unsigned int M>
  TFVectorC<DataT,N> TFMatrixC<DataT,N,M>::operator*(const TFVectorC<DataT,M> & vec) const {
    TFVectorC<DataT,N> ret; // N=r M=c
    Mul(*this,vec,ret);
    return ret;
  }
  
  template<class DataT,unsigned int N,unsigned int M>
  TFMatrixC<DataT,M,N> TFMatrixC<DataT,N,M>::T() const {
    TFMatrixC<DataT,M,N> ret;
    for(UIntT i = 0;i < N;i++)
      for(UIntT j = 0;j < M;j++)
	ret[j][i] = data[i][j];
    return ret;
  }
  
  template<class DataT,unsigned int N,unsigned int M>
  DataT TFMatrixC<DataT,N,M>::SumOfAbs() const {
    DataT ret;
    SetZero(ret);
    for(UIntT i = 0;i < N;i++)
      for(UIntT j = 0;j < M;j++)
	ret += Abs(data[i][j]);
    return ret;
  }
  
  template<class DataT,unsigned int N,unsigned int M>
  DataT TFMatrixC<DataT,N,M>::SumOfSqr() const {
    DataT ret;
    SetZero(ret);
    for(UIntT i = 0;i < N;i++)
      for(UIntT j = 0;j < M;j++)
	ret += Sqr(data[i][j]);
    return ret;    
  }
  
  template<class DataT,unsigned int N,unsigned int M>
  inline bool TFMatrixC<DataT,N,M>::Limit(const DataT &min,const DataT &max) {
    bool ret = true;
    for(UIntT i = 0;i < N;i++)
      for(UIntT j = 0;j < M;j++) {
	if(data[i][j] > max) {
	  data[i][j] = max;
	  ret = false;
	  continue;
	}
	if(data[i][j] < min) {
	  data[i][j] = min;
	  ret = false;
	}
      }
    
    return ret;
  }

  template<class DataT,unsigned int N,unsigned int M>
  TFMatrixC<DataT,N,M> operator*(const DataT & val,const TFMatrixC<DataT,N,M> &mat) {
    TFMatrixC<DataT,N,M> ret;
    for(UIntT i = 0;i < N;i++)
      for(UIntT j = 0;j < M;j++)
	ret[i][j] = val * mat[i][j];
    return ret;
  }
  
  template<class DataT,unsigned int N,unsigned int M>
  TFMatrixC<DataT,N,M> operator/(const DataT & val,const TFMatrixC<DataT,N,M> &mat) {
    TFMatrixC<DataT,N,M> ret;
    for(UIntT i = 0;i < N;i++)
      for(UIntT j = 0;j < M;j++)
	ret[i][j] = val / mat[i][j];
    return ret;
  }

  template<class DataT,unsigned int N,unsigned int M>
  TFMatrixC<DataT,N,M> TFMatrixC<DataT,N,M>::I() {
    TFMatrixC<DataT,N,M> ret;
    for(UIntT i = 0;i < N;i++) 
      for(UIntT j = 0;j < M;j++)
	ret[i][j] = (i == j)?1.0:0.0;
    return ret;
  }
  
  template<class DataT,unsigned int N,unsigned int M>
  const TFMatrixC<DataT,N,M> &TFMatrixC<DataT,N,M>::SetDiagonal(const TFVectorC<DataT,N> &d) {
    unsigned int max = Min(N,M);
    for(unsigned int i = 0;i < max;i++)
      data[i][i] = d[i];
    return *this;
  }
  
  template<class DataT,unsigned int N,unsigned int M>
  const TFMatrixC<DataT,N,M> &TFMatrixC<DataT,N,M>::AddDiagonal(const TFVectorC<DataT,N> &d) {
    unsigned int max = Min(N,M);
    for(unsigned int i = 0;i < max;i++)
      data[i][i] += d[i];
    return *this;
  }

  //// TFVectorC methods that use TFMatrixC.
  
  template<class DataT,unsigned int N>
  const TFMatrixC<DataT,1,N> &TFVectorC<DataT,N>::T() const
  { return *((const TFMatrixC<DataT,1,N> *) ((void *)this)); }
  
  template<class DataT,unsigned int N>
  TFMatrixC<DataT,N,N> &TFVectorC<DataT,N>::OuterProduct(const TFVectorC<DataT,N> &av, 
							 TFMatrixC<DataT,N,N> &result) const {
    for(unsigned int i = 0;i < N;i++) 
      for(unsigned int j = 0;j < N;j++) 
	result[i][j] = av[i] * data[j];
    return result;
  }
  
  template<class DataT,unsigned int N>
  TFMatrixC<DataT,N,N> &TFVectorC<DataT,N>::OuterProduct(TFMatrixC<DataT,N,N> &result) const {
    for(unsigned int i = 0;i < N;i++) 
      for(unsigned int j = 0;j < N;j++) 
	result[i][j] = data[i] * data[j];
    return result;
  }
  
  template<class DataT,unsigned int N,unsigned int M>
  TFMatrixC<DataT,N,M> operator*(const TFVectorC<DataT,N> &vec,const TFMatrixC<DataT,1,M> &mat) {
    TFMatrixC<DataT,N,M> ret;
    for(UIntT i = 0;i < N;i++)
      for(UIntT j = 0;j < M;j++)
	ret[i][j] = vec[i] * mat[0][j];
    return ret;
  }
  //: Vector multiply a matrix.
  // The implementation for this can be found in "Ravl/TFMatrix.hh"  
}

#endif
