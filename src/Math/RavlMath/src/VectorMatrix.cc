// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////////////////
//! author="Kieron Messer"
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/LinearAlgebra/General/VectorMatrix.cc"

#include "Ravl/Stream.hh"
#include "Ravl/VectorMatrix.hh"
#include "Ravl/BinStream.hh"

namespace RavlN {


  VectorMatrixC::VectorMatrixC(const UIntT dim)
    : VectorC(dim),
      MatrixC(dim,dim)
  {}
  
  VectorMatrixC::VectorMatrixC(const VectorC & vec, const MatrixC & mat)
    : VectorC(vec),
      MatrixC(mat)
  {}
  
  VectorMatrixC VectorMatrixC::Copy() const 
  { return VectorMatrixC(VectorC::Copy(),MatrixC::Copy()); }

  void VectorMatrixC::SetZero() {
    VectorC::Fill(0.0);
    MatrixC::Fill(0.0);
  }
  
  void VectorMatrixC::Sort() {
    // The straight insertion method is used for sorting.
    const UIntT vn = Vector().Size();
    
    for (UIntT i=0; i<vn-1; i++) {
      // Find the maximum item in the rest of the vector.
      UIntT maxIndex = i;
      RealT maxValue = Vector()[i];
      for (UIntT j=i+1; j<vn; j++) {
	if (Vector()[j] > maxValue) {
	  maxIndex = j;
	  maxValue = Vector()[maxIndex];
	}
      }
      
      if (maxIndex != i) { 
	// Swap columns of the matrices and items of the vector.
	Vector()[maxIndex]=Vector()[i];
	Vector()[i]=maxValue;
	for (UIntT j=0; j<vn; j++)  { 
	  // Swap two elements of the matrix.
	  RavlN::Swap((Matrix()[j][i]),(Matrix()[j][maxIndex]));
	}
      }
    }
  }
  
  //--------------------------------------------------------------------
  // *************** Input - Output ************************************
  //--------------------------------------------------------------------
  
  ostream &operator<<(ostream & s, const VectorMatrixC & vm) {
    const VectorC & v = vm.Vector();
    const MatrixC & m = vm.Matrix();
    s << v << '\n' << m;
    return s;
  }
  
  istream &operator>>(istream & s, VectorMatrixC & vm) {
    VectorC & v = vm.Vector();
    MatrixC & m = vm.Matrix();
    s >> v >> m;
    return s;
  }

  //: Writes the object 'vm' into the output stream.
  
  BinOStreamC & operator<<(BinOStreamC & s, const VectorMatrixC & vm) {
    const VectorC & v = vm.Vector();
    const MatrixC & m = vm.Matrix();
    s << v << m;
    return s;
  }
  
  //: Sets the object 'vm' according to the object saved in the input stream.
  
  BinIStreamC & operator>>(BinIStreamC & s, VectorMatrixC & vm) {
    VectorC & v = vm.Vector();
    MatrixC & m = vm.Matrix();
    s >> v >> m;
    return s;
  }
  
}
