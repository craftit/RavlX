// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_VECTORMATRIX_HEADER
#define RAVL_VECTORMATRIX_HEADER 1
////////////////////////////////////////////////////////////////////////
//! userlevel=Normal
//! author="Kieron Messer"
//! rcsid="$Id$"
//! lib=RavlMath
//! docentry="Ravl.API.Math.Linear Algebra"
//! file="Ravl/Math/LinearAlgebra/General/VectorMatrix.hh"

#include "Ravl/Matrix.hh"
#include "Ravl/Vector.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: A vector and a matrix together for n-D space
  
  class VectorMatrixC
    : protected VectorC,
      protected MatrixC
  { 
  public:
    VectorMatrixC()
    {}
    //: Empty object
    
    VectorMatrixC(const UIntT dim);
    //: Constructor of an empty vector and matrix of the dimension 'dim'.
    
    VectorMatrixC(const VectorC & vec, const MatrixC & mat);  
    //: Constructs the object from the vector 'vec' and the matrix 'mat'.
    //: The new object contains handles of 'vec' and 'matrix' (BIG OBJECT).
    
    VectorMatrixC(const VectorMatrixC &vm)
      : VectorC(vm.Vector()),
        MatrixC(vm.Matrix())
    {}
    //: Copy constructor.
    
    inline const VectorC & Vector() const
    { return *this; }
    //: Access to the vector.
    
    inline const MatrixC & Matrix() const
    { return *this; }
    //: Access to the matrix.
    
    VectorC & Vector()
    { return *this; }
    //: Non-constant access to the vector.
    
    MatrixC & Matrix()
    { return *this; }
    //: Non-constant access to the matrix.
    
    VectorMatrixC Copy() const;
    //: Returns the physical copy of this vector matrix.
    
    void SetZero();
    //: Zeros the vector and the matrix.
    
    void Sort();
    //: Sort in place according to the vector value. 
    // The first value will be the  biggest one. 
    
  protected:    
    friend istream & operator>>(istream & s, VectorMatrixC & vm);
  };
  
  ostream & operator<<(ostream & s, const VectorMatrixC & vm);
  //: Writes the object 'vm' into the output stream.
  
  istream & operator>>(istream & s, VectorMatrixC & vm);
  //: Sets the object 'vm' according to the object saved in the input stream.
  
  BinOStreamC & operator<<(BinOStreamC & s, const VectorMatrixC & vm);
  //: Writes the object 'vm' into the output stream.
  
  BinIStreamC & operator>>(BinIStreamC & s, VectorMatrixC & vm);
  //: Sets the object 'vm' according to the object saved in the input stream.
    
}


#endif
