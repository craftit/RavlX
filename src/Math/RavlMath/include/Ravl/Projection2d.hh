// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_PROJECTION2D_HEADER
#define RAVL_PROJECTION2D_HEADER 1
/////////////////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! author="Charles Galambos"
//! lib=RavlMath
//! date="17/10/2002"
//! docentry="Ravl.API.Math.Geometry.2D;Ravl.API.Math.Projective Geometry.2D"
//! file="Ravl/Math/Geometry/Euclidean/2D/Projection2d.hh"

#include "Ravl/Matrix3d.hh"
#include "Ravl/Vector3d.hh"
#include "Ravl/Vector2d.hh"
#include "Ravl/Matrix2d.hh"
#include "Ravl/Point2d.hh"
#include "Ravl/LineABC2d.hh"
#include "Ravl/LinePP2d.hh"
#include "Ravl/FAffine.hh"
#include "Ravl/Pair.hh"

namespace RavlN {
  template<class DataT> class DListC;
  class PointSet2dC;
  class Polygon2dC;
  
  //! userlevel=Normal
  //: 2D Projective transform.
  // <p>Represents a perspective projection in 2D space. </p>
  // <p>The difference between this class and <a href="RavlN.PProjection2dC.html">PProjection2dC</a> is:</p><ul>
  // <li> This class (Projection2dC) includes the relevant projective scaling parameters within the projection object.  Use this class when dealing with Point2dC Euclidean 2D points.</li>
  // <li> PProjection2dC does not include the projective scaling parameters: it is for use with projective 2D points PPoint2dC which themselves already contain the scaling parameters.</li></ul>

  class Projection2dC {
  public:
    Projection2dC()
      : trans(Matrix3dC(0,0,0,
			0,0,0,
			0,0,0)),
	iz(1),
	oz(1)
    {}
    //: Default constructor.
    // Creates an invalid (zero) transform.

    Projection2dC(const Matrix3dC &transform,RealT Oz = 1,RealT Iz = 1)
      : trans(transform),
	iz(Iz),
	oz(Oz)
    {}
    //: Construct a projective transform.
    //!param: transform - the 2D projective transformation
    //!param: Iz, Oz - the projective scale values for the input and output vectors
    // <p>These are the scale values that the last term in the projective vectors must have for correct normalisation.  They are usually set = 1.  However for some optimisation operations better results are obtained if values more representative of typical components of the vector are used.
    // In the projection "b = P a", Iz and Oz is the scale values for a and b respectively.</p>
    // <p> This constructor assumes that the values of the last column of "transform" have already been set to correspond to the value of "iz".</p>

    Projection2dC(const FAffineC<2> &affineTransform, RealT Oz = 1, RealT Iz = 1) 
      : iz(Iz),
        oz(Oz)
    {
      trans[0][0] = affineTransform.SRMatrix()[0][0];
      trans[1][0] = affineTransform.SRMatrix()[1][0];
      trans[0][1] = affineTransform.SRMatrix()[0][1];
      trans[1][1] = affineTransform.SRMatrix()[1][1];
      trans[0][2] = affineTransform.Translation()[0]/iz;
      trans[1][2] = affineTransform.Translation()[1]/iz;
      trans[2][2] = oz/iz;
      trans[2][0] = trans[2][1] = 0;
    }
    //: Construct a projective transform from an affine one
    //!param: affineTransform - the 2D affine transform
    //!param: Iz, Oz - the projective scale values for the input and output vectors
    // The ambiguous parameters that are not specified by the affine transform are set to 0.
    
    Point2dC Project(const Point2dC &pnt) const {
      Vector3dC vo = trans * Vector3dC(pnt[0],pnt[1],iz);
      return Point2dC(oz*vo[0]/vo[2],oz*vo[1]/vo[2]);          
    }
    //: Project a point through the transform.
    
    Point2dC operator*(const Point2dC &pnt) const
    { return Project(pnt); }
    //: Project a point through the transform.
    
    LineABC2dC Project(const LineABC2dC &line) const {
      Vector3dC vo = trans.Inverse() * Vector3dC(line.A(),line.B(),line.C()/iz);
      return LineABC2dC(vo[0],vo[1],vo[2]*oz);          
    }
    //: Project a line through the transform.
    // Current implementation is slow, as it inverts the projection each time the method is called.
    
    LineABC2dC operator*(const LineABC2dC &line) const
    { return Project(line); }
    //: Project a line through the transform.
    // Current implementation is slow, as it inverts the projection each time the method is called.

    LinePP2dC Project(const LinePP2dC &line) const
    { return LinePP2dC(this->Project(line[0]),this->Project(line[1])); }
    //: Project a line through the transform.
    
    LinePP2dC operator*(const LinePP2dC &line) const
    { return LinePP2dC(this->Project(line[0]),this->Project(line[1])); }
    //: Project a line through the transform.

    Projection2dC operator*(const Projection2dC &oth) const {
      Matrix3dC diag(Matrix3dC::I());
      diag[2][2] = iz/oth.oz;
      Matrix3dC transform = trans * diag * oth.trans;
      return Projection2dC(transform, oz, oth.iz);
    }
    //: Combine two transforms
    //!param: oth - the other transform to be combined with this one
    //!return: the result of cascading this transform with the other one.<br>
    // Note that the iz and oz values of the two transforms are combined
    // for the resulting one.

    Projection2dC Inverse() const
    { return Projection2dC(trans.Inverse(),iz,oz); }
    //: Invert transform.
    
    static Projection2dC I(RealT oz=1, RealT iz=1) {
      Matrix3dC m (Matrix3dC::I());
      m[2][2] = oz/iz;
      return Projection2dC(m, oz, iz);
    }
    //: Returns identity projection

    Matrix3dC &Matrix()
    { return trans; }
    //: Access transformation matrix.
    // NOTE: This is NOT the homography between images
    // unless the scaling factors are both 1.
    
    const Matrix3dC &Matrix() const
    { return trans; }
    //: Access transformation matrix.
    // NOTE: This is NOT the homography between images
    // unless the scaling factors are both 1.
    
    RealT IZ() const
    { return iz; }
    //: Accesss iz.

    RealT OZ() const
    { return oz; }
    //: Accesss oz.

    RealT &IZ()
    { return iz; }
    //: Accesss iz.

    RealT &OZ()
    { return oz; }
    //: Accesss oz.
    
    Matrix3dC Homography() const;
    //: Get homography
    // This returns the projection normalised to make the projective scales both = 1
    
    bool IsNearAffine(const RealT tolerance = 1e-6) const 
    { return (Abs(trans[2][2]) + Abs(trans[2][0])) * (iz / oz) < tolerance; }
    //: Test if projection is near affine.
    
    FAffineC<2> AffineApproximation() const;
    //: Get an affine approximation of this projective transform
    //!return: the affine approximation
    
    inline bool IsValid() const
    { return ((trans != Matrix3dC(0,0,0,0,0,0,0,0,0)) && trans.IsReal()); }
    // True if not the zero projection and Matrix3dC is "real"
    
  protected:
    Matrix3dC trans;
    RealT iz, oz;
  };
  
  Projection2dC FitProjection(const DListC<Point2dC> &org,const DListC<Point2dC> &newPos,RealT &residual);
  //: Fit a projective transform given to the mapping between original and newPos.
  // Note: In the current version of the routine 'residual' isn't currently computed.
  
  Projection2dC FitProjection(const DListC<Point2dC> &org,const DListC<Point2dC> &newPos);
  //: Fit a projective transform given to the mapping between original and newPos.
  
  Projection2dC FitProjection(const SArray1dC<Point2dC> &org,const SArray1dC<Point2dC> &newPos,RealT &residual);
  //: Fit a projective transform given to the mapping between original and newPos.
  // Note: In the current version of the routine 'residual' isn't currently computed.

  Projection2dC FitProjection(const SArray1dC<PairC<Point2dC> > &matchPairs, RealT &residual);
  //: Fit a projective transform given an array of matched points.
  
  Projection2dC FitProjection(const SArray1dC<Point2dC> &org,const SArray1dC<Point2dC> &newPos);
  //: Fit a projective transform given to the mapping between original and newPos.
  
  Projection2dC FitProjection(const SArray1dC<Point2dC> &org,const SArray1dC<Point2dC> &newPos,const SArray1dC<RealT> &weight);
  //: Fit a projective transform given to the mapping between original and newPos with weighting for points.
  
  bool FitProjection(const SArray1dC<Point2dC> &from,const SArray1dC<Point2dC> &to,Matrix3dC &proj);
  //: Fit a projective matrix.
  
  bool FitProjection(const SArray1dC<Point2dC> &from,const SArray1dC<Point2dC> &to,const SArray1dC<RealT> &weight,Matrix3dC &proj);
  //: Fit a projective matrix with weighting for points.
  
  PointSet2dC operator*(const Projection2dC &trans,const PointSet2dC &points);
  //: Apply a projective transform to a point set
  
  Polygon2dC operator*(const Projection2dC &trans,const Polygon2dC &points);
  //: Apply a projective transform to a polygon
  
  istream &operator>>(istream &s,Projection2dC &proj);  
  //: Read from a stream.
  
  ostream &operator<<(ostream &s,const Projection2dC &proj);  
  //: Write to a stream.
  
  BinIStreamC &operator>>(BinIStreamC &s,Projection2dC &proj);  
  //: Read from a binary stream.
  
  BinOStreamC &operator<<(BinOStreamC &s,const Projection2dC &proj);  
  //: Write to a binary stream.
  
    
}



#endif
