// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/LinearAlgebra/General/Matrix.cc"
#include "Ravl/Types.hh"
#include "Ravl/StdMath.hh"
#include "Ravl/Vector.hh"
#include "Ravl/Matrix.hh"
#include "Ravl/LAHooks.hh"

// General matrix functions.

namespace RavlN {
  
  //: Test if matrix only contains real values.
  // This will return false if either nan's (Not an number) 
  // or infinite values are found.
  
  bool MatrixC::IsReal() const {
    for(BufferAccess2dIterC<RealT> it(*this,Size2());it;it++)
      if(IsInf(*it) || IsNan(*it))
	return false;
    return true;
  }


  //: Dump to cout .
  
  void MatrixC::Dump() const {
    cout << Rows() << ' ' << Cols() << "\n";
    for(BufferAccess2dIterC<RealT> it(*this,Size2());it;) {
      do {
        cout << *it << ' ';
      } while(it.Next()) ;
      cout << "\n";
    }
  }
  
  //: Normalise rows so they have a magnitude of 1.
  // Zero rows are ignored.
  
  void MatrixC::NormaliseRows() {
    for(UIntT i = 0;i < Rows();i++) {
      SizeBufferAccessC<RealT> row = (*this)[i];
      BufferAccessIterC<RealT> it(row);
      RealT sumsq = 0;
      for(;it;it++)
        sumsq += Sqr(*it);
      sumsq = 1/Sqrt(sumsq);
      if(sumsq != 0) {
        for(;it;it++)
          *it *= sumsq;
      }
    }
  }
  
  //: Normalise columns so they have a magnitude of 1.
  // Zero rows are ignored.
  
  void MatrixC::NormaliseColumns() {
    for(UIntT i = 0;i < Cols();i++) {
      Slice1dC<RealT> col = SliceColumn(i);
      RealT sumsq = col.SumOfSqr();
      if(sumsq != 0) {
        sumsq = 1/Sqrt(sumsq);
        col *= sumsq;
      }
    }
  }

  //: Add outer product of vec1 and vec2 to this matrix.

  const TMatrixC<RealT> &MatrixC::AddOuterProduct(const TVectorC<float> &vec1,const TVectorC<float> &vec2) {
    RavlAssert(this->Size1() == vec1.Size());
    RavlAssert(this->Size2() == vec2.Size());
    BufferAccessIterC<float> v1(vec1);
    BufferAccess2dIterC<RealT> it(*this,this->Size2());
    while(it) {
      BufferAccessIterC<float> v2(vec2);
      float r1 = (*v1);
      do {
        *it += r1 * (*v2);
        v2++;
      } while(it.Next()) ;
      v1++;
    }
    return *this;
  }

  //: Add outer product of vec1 and vec2 multiplied by a to this matrix .

  const TMatrixC<RealT> &MatrixC::AddOuterProduct(const TVectorC<float> &vec1,const TVectorC<float> &vec2,const float &a) {
    RavlAssert(this->Size1() == vec1.Size());
    RavlAssert(this->Size2() == vec2.Size());
    BufferAccessIterC<float> v1(vec1);
    BufferAccess2dIterC<RealT> it(*this,this->Size2());
    while(it) {
      BufferAccessIterC<float> v2(vec2);
      float r1 = (*v1) * a;
      do {
        *it += r1 * (*v2);
        v2++;
      } while(it.Next()) ;
      v1++;
    }
    return *this;
  }

  
#ifdef __sgi__
  // Help the compiler a bit...
  template istream &operator>> (istream&,SizeBufferAccess2dC<double>&);
  template ostream &operator<< (ostream&,const SizeBufferAccess2dC<double>&);

  template ostream &operator<<(ostream&,const SArray2dC<double>&);
  template istream &operator>>(istream&,SArray2dC<double>&);
#endif

}
