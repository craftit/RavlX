// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FVECTORMATRIX_HEADER
#define RAVL_FVECTORMATRIX_HEADER 1
//! rcsid=$Id$
//! author="Charles Galambos"
//! date="13/04/2002"
//! docentry="Ravl.API.Math.Geometry"
//! lib=RavlMath
//! file="Ravl/Math/LinearAlgebra/FixedSize/FVectorMatrix.hh"

#include "Ravl/FVector.hh"
#include "Ravl/FMatrix.hh"

namespace RavlN {
  
  template<unsigned int N>
  class FVectorMatrixC
    : protected FVectorC<N>,
      protected FMatrixC<N,N>
  {
  public:
    FVectorMatrixC()
    {}
    //: Default constructor.
    
    FVectorMatrixC(const FVectorC<N> &vec,const FMatrixC<N,N> &mat)
      : FVectorC<N>(vec),
	FMatrixC<N,N>(mat)
    {}
    //: Construct from a vector and a matrix.
    
    FVectorC<N> & Vector()
    { return *this; }
    //: Access to the vector.
    
    FMatrixC<N,N> & Matrix()
    { return *this; }
    //: Access to the matrix.
    
    const FVectorC<N> & Vector() const
    { return *this; }
    //: Access to the vector.
    
    const FMatrixC<N,N> & Matrix() const
    { return *this; }
    //: Access to the matrix.
    
    void Fill(RealT value) {
      FMatrixC<N,N>::Fill(value);
      FVectorC<N>::Fill(value);
    }
    //: Fills both the vector and matrix with 'value'
    
    void Sort();
    //: Sort in place according to the vector value. 
    // The first value will be the  biggest one. 
  };


  template<unsigned int N>
  void FVectorMatrixC<N>::Sort() {
    const UIntT vn = Vector().Size();
    for (UIntT i=0; i<vn-1; i++) {
      // Find the maximum item in the rest of the vector.
      UIntT   maxIndex = i;
      RealT maxValue = Vector()[i];
      for (UIntT j=i+1; j<vn; j++)
	if (Vector()[j] > maxValue) 
	  maxValue = Vector()[maxIndex=j];
      
      if (maxIndex != i) { 
	// Swap columns of the matrices and items of the vector.
	Vector()[maxIndex]=Vector()[i];
	Vector()[i]=maxValue;
	for (UIntT j=0; j<vn; j++)  { 
	  // Swap two elements of the matrix.
	  RealT value    = Matrix()[j][i];
	  Matrix()[j][i] = Matrix()[j][maxIndex];
	  Matrix()[j][maxIndex]=value;
	}
      }
    }
  }
}

#endif
