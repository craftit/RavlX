// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_PPROJECTION3D_HH
#define RAVL_PPROJECTION3D_HH
////////////////////////////////////////////////////////////////////////
//! file="Ravl/Math/Geometry/Projective/3D/PProjection3d.hh"
//! lib=RavlMath
//! userlevel=Normal
//! author="Radek Marik"
//! date="26.06.1995"
//! docentry="Ravl.API.Math.Projective Geometry.3D"

#include "Ravl/Matrix4d.hh"
#include "Ravl/PPoint3d.hh"
#include "Ravl/PPlane3d.hh"

namespace RavlN {
  class Affine3dC;
  
  //! userlevel=Normal
  //: Projective transformation in 3D space
  // The class PProjection3dC represents a  projective transformation
  // of objects in projective three-space. The projection is
  // represented by a 4x4 rank 3 matrix. The indexes
  // start from 0.

  class PProjection3dC
    : private Matrix4dC
  {
  public:

    // Constructors, assigment, and destructor.
    // ----------------------------------------

    inline PProjection3dC()
      : Matrix4dC(0, 0, 0, 0,
		  0, 0, 0, 0,
		  0, 0, 0, 0,
		  0, 0, 0, 0)
    {}
    // Creates the zero projection. All points are projected into
    // not-valid projective object.
    
    PProjection3dC(const Affine3dC &affine);
    // Construct from an affine transform.
    
    inline PProjection3dC(RealT b00, RealT b01, RealT b02, RealT b03,
			  RealT b10, RealT b11, RealT b12, RealT b13,
			  RealT b20, RealT b21, RealT b22, RealT b23,
			  RealT b30, RealT b31, RealT b32, RealT b33
			  )
      : Matrix4dC(b00, b01, b02, b03,
		   b10, b11, b12, b13,
		   b20, b21, b22, b23,
		   b30, b31, b32, b33)
    {}
    // Creates a new projection and assign the values b[ij].
    
    PProjection3dC(istream & inS);
    // Creates a projection from the input stream.
    
    //:--------------------------------------------
    // Access to the elements of the projection.
    
    using Matrix4dC::operator[];
    
    Matrix4dC &Matrix()
    { return *this; }
    //: Access as matrix.
    
    const Matrix4dC &Matrix() const
    { return *this; }
    //: Access as matrix.
    
    inline const PProjection3dC & PProjection3d() const
    { return *this; }
    // Access to the constant object.

    inline PProjection3dC & PProjection3d()
    { return *this; }
    // Access to the object.

    inline PPlane3dC Plane(UIntT i) const {
      const SizeBufferAccessC<RealT> r = (*this)[i];
      return RavlN::PPlane3dC(r[0], r[1], r[2], r[3]);
    }
    // Returns the row 'i'.
    
    inline PPoint3dC SingularPoint() const
    { return PPoint3dC(Plane(0), Plane(1), Plane(2)); }
    // Returns the singular points of the projection. If this projective
    // transformation is a perspectivity, the singular point is the optical
    // center of the projection.
    
    //:---------------------------
    // Setting of matrix elements.
    
    PProjection3dC & SetPlane(IndexC i, const PPlane3dC & p);
    // Sets the elements of the row 'i' according to the elements of 
    // the plane 'p'.
    
    //:----------------------
    // Projective operations.

    inline PPoint3dC operator*(const PPoint3dC & p) const
    { return Matrix4dC::operator*(p); }
    // Projects the point 'p' into the image point. The function
    // is equivalent to the operator()(const PPoint3dC & p).

    inline PPoint3dC operator()(const PPoint3dC & p) const
    { return Matrix4dC::operator*(p); }
    // Projects the point 'p' into the image point. The function
    // is equivalent to the operator*(const PPoint3dC & p).

    inline PProjection3dC operator*(const PProjection3dC & p) const
    { return Matrix4dC::operator*(p); }
    // Returns the composition of two projections. The result
    // projects projective objects in the same way as they would be
    // projected by projection 'p' at first and then by this projection.
    
    inline PProjection3dC(const Matrix4dC & m)
      : Matrix4dC(m)
    {}
    // Creates a projectivity transformation defined by the matrix 'm'..
    
    inline PProjection3dC(const RavlN::TFMatrixC<RealT, 4, 4>  & m)
      : Matrix4dC(m)
    {}
    // Creates a projectivity transformation defined by the matrix 'm'..
  protected:
    
    friend class PPerspective3dC;
  };    
  
  bool FitPProjection(const SArray1dC<PPoint3dC> &p1,const SArray1dC<PPoint3dC> &p2,PProjection3dC &proj);
  //: Fit a 3d projection such that  p2 = proj * p1
  
  ostream & operator<<(ostream & outS, const PProjection3dC & p);
  //: Saves the projection 'p' into the output stream 'outS'.

  istream & operator>>(istream & inS, PProjection3dC & p);
  //: Sets the projective transformation elements according to the values
  //: read from input stream 'inS'.
  
  BinOStreamC & operator<<(BinOStreamC & strm, const PProjection3dC & p);
  //: Saves the projection 'p' into the output binary stream 'strm'.
  
  BinIStreamC & operator>>(BinIStreamC & strm, PProjection3dC & p);
  //: Sets the projective transformation elements according to the values
  //: read from binary stream 'strm'.
  
}
#endif
