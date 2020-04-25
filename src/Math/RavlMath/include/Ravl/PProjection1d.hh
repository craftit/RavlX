// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_PROJECTION1D_HEADER
#define RAVL_PROJECTION1D_HEADER 1
////////////////////////////////////////////////////////////////////////
//! file="Ravl/Math/Geometry/Projective/1D/PProjection1d.hh"
//! lib=RavlMath
//! userlevel=Normal
//! author="Radek Marik"
//! docentry="Ravl.API.Math.Projective Geometry.1D"
//! date="26.06.1995"
//! rcsid="$Id$"

#include "Ravl/Matrix2d.hh"
#include "Ravl/PPoint1d.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: Projective transformation in 1D space
  // The class PProjection1dC represents a  projective transformation
  // of objects in projective line. The projection is
  // represented by a 2 x 2 matrix. The indexes start from 0.
  
  class PProjection1dC
    : private Matrix2dC
  {
  public:
    
    //:----------------------------------------
    // Constructors, assigment, and destructor.
    
    inline PProjection1dC();
    // Creates the zero projection. All points are projected into
    // not-valid projective object.
    
    inline PProjection1dC(RealT b00, RealT b01,
			  RealT b10, RealT b11)
      : Matrix2dC(b00, b01,
		  b10, b11)
    {}
    // Creates a new projection and assign the values b[ij].
    
    
    PProjection1dC(const PPoint1dC & i,
		   const PPoint1dC & o,
		   const PPoint1dC & u);
    // Creates a projective parametrization of the projective plane that
    // transforms the projective point 'i' into the ideal point I(1,0),
    // the projective point 'o' into the origin O(0,1), and the projective
    // point 'u' into the unit point 'u'.
    // All three points 'i', 'o', 'u' must be distinct.
    
    inline PProjection1dC(const PPoint1dC & i0, const PPoint1dC & i1,
			  const PPoint1dC & o0, const PPoint1dC & o1,
			  const PPoint1dC & u0, const PPoint1dC & u1)
      
      : Matrix2dC(PProjection1dC(i1,o1,u1) * PProjection1dC(i0,o0,u0).Inverse())
    {}
    // Creates a projection transformation taking plane p0 to the plane p1.
    // The plane p0, resp. p1 contains the ideal point i0, resp. i1,
    // the origin point o0, resp. o1, and the unit point u0, resp. u1.
    // All three points x0, resp. x1, must be distinct. <br>
    // Ref.: pp. 120-121
    
    PProjection1dC(istream & inS);
    // Creates a projection from the input stream.
    
    //:--------------------------------------------
    // Access to the elements of the projection.
    
    inline const PProjection1dC & PProjection1d() const
    { return *this; }
    // Access to the constant object.
    
    inline PProjection1dC & PProjection1d()
    { return *this; }
    // Access to the object.
    
    //:----------------------
    // Projective operations.
    
    inline PProjection1dC & SetStandard() {
      Matrix2dC::operator/=(Sqrt(Abs(Det())));
      return *this;
    }
    // Scales all elements in order to the determinant of the projection
    // matrix is unit.
    
    inline PProjection1dC Inverse() const {
      // There is no necessary to divide the result by the determinant of
      // the original matrix because the projection is unique up to scalar.
      return PProjection1dC((*this)[1][1], -(*this)[0][1],
			     -(*this)[1][0],(*this)[0][0]);
    }
    // Returns the inverze projection.
    
    inline PPoint1dC operator*(const PPoint1dC & p) const
    { return Matrix2dC::operator*(p); }
    //: Projects the point 'p' into the image point. 
    // The function is equivalent to the operator()(const PPoint1dC & p).
    
    inline PPoint1dC operator()(const PPoint1dC & p) const
    { return Matrix2dC::operator*(p); }
    // Projects the point 'p' into the image point. The function
    // is equivalent to the operator*(const PPoint1dC & p).
    
    inline PProjection1dC operator*(const PProjection1dC & p) const
    { return Matrix2dC::operator*(p); }
    // Returns the composition of two projections. The result
    // projects projective objects in the same way as they would be
    // projected by projection 'p' at first and then by this projection.
    
    SizeBufferAccessC<RealT> operator[](UIntT ind) 
    { return ((Matrix2dC &) (*this))[ind]; }
    //: Access element
    
    const SizeBufferAccessC<RealT> operator[](UIntT ind) const 
    { return ((const Matrix2dC &) (*this))[ind]; }
    //: Access element
    
  protected:
    
    inline PProjection1dC(const Matrix2dC & m)
      : Matrix2dC(m)
    {}
    
    inline PProjection1dC(const RavlN::TFMatrixC<RealT, 2, 2> & m)
      : Matrix2dC(m)
    {}
    
    // Creates a projectivity transformation defined by the matrix 'm'.
  };    
  
  ostream & operator<<(ostream & outS, const PProjection1dC & p);
  // Saves the projection 'p' into the output stream 'outS'.
  
  istream & operator>>(istream & inS, PProjection1dC & p);
  // Sets the projective transformation elements according to the values
  // read from input stream 'inS'.
  
}


#endif
