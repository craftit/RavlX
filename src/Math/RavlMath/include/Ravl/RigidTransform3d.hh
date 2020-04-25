// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_RIGIDTRANSFORM_HEADER
#define RAVL_RIGIDTRANSFORM_HEADER 1
//! date="13/11/1995"
//! file="Ravl/Math/Geometry/Euclidean/3D/RigidTransform3d.hh"
//! author="Andrew Stoddart"
//! userlevel = Default
//! lib=RavlMath
//! rcsid     = "$Id$"
//! docentry="Ravl.API.Math.Geometry.3D"

#include "Ravl/Quatern3d.hh"
#include "Ravl/Vector3d.hh"
#include "Ravl/Matrix3d.hh"
#include "Ravl/Vector.hh"

namespace RavlN {
  
  class RigidTransform3dC;
  
  ostream & operator<<(ostream & outS, const RigidTransform3dC & rt);
  //: output stream operator
  
  istream & operator>>(istream & ins, RigidTransform3dC & rt);
  //: input stream operator
  
  BinOStreamC & operator<<(BinOStreamC & outS, const RigidTransform3dC & rt);
  //: output stream operator
  
  BinIStreamC & operator>>(BinIStreamC & ins, RigidTransform3dC & rt);
  //: input stream operator
  
  //! userlevel=Normal
  //: Rigid transform in 3 dimensions.
  // A RigigTransform3dC is a 3D Rigid body transformation comprising of a 
  // rotation followed by a translation. The order is very important.
  // <p>
  // A  RigigTransform3dC can also be thought of as a Pose, i.e. a position and
  // orientation.
  // <p>
  // This library is based on an active scheme, i.e. there is one fixed
  // coordinate system and the rotation and translation may be envisaged
  // as acting ON an object. [ See CoordSysC for a passive scheme ]
  
  class RigidTransform3dC {
  public:
    
    //:-------------------------------------
    // Constructors,  destructor, assignment

    RigidTransform3dC()
    {}
    //: Null Constructor 

    RigidTransform3dC(const Vector3dC &t, const Quatern3dC &q)
      : trans(t), rot(q)
    {}
    //: Constructor
    
    RigidTransform3dC(const Vector3dC &t,const Vector3dC &axtheta)
      : trans(t), rot(axtheta)
    {}
    //: Constructor 
    
    RigidTransform3dC(const Vector3dC &t, const Vector3dC &axis, double theta)
      : trans(t), rot(axis, theta)
    {}
    //: Constructor 
    
    RigidTransform3dC(const Vector3dC &t, const Matrix3dC &r)
      : trans(t), rot(r)
    {}
    //: Constructor 
    
    //:---------------------------------------------------------
    //: Access
    
    RigidTransform3dC & Set(const Vector3dC &t,const Quatern3dC &q)  {
      rot = q; trans=t;
      return *this;
    }
    //: Set as specified

    RigidTransform3dC & Set(const Vector3dC &t,const Vector3dC &axis, double theta) {
      rot.Set(axis, theta); trans=t;
      return *this;
    }
    //: Set as specified
    
    RigidTransform3dC & Set(const Vector3dC &translate,const Matrix3dC &rotate) {
      rot.Set(rotate); trans=translate;
      return *this;
    }
    //: Set as specified
 
    const Quatern3dC &Rotation(void) const
    { return rot; }
    //: Return the rotation as a quarternion
    
    RigidTransform3dC & SetRotation(const Quatern3dC &q) {
      rot=q;
      return *this;
    }
    //: Set the rotation, leave trans unchanged

    const Vector3dC &Translation(void) const 
    { return trans; }
    //: Return the translation
    
    RigidTransform3dC & SetTranslation(const Vector3dC &t) {
      trans=t;
      return *this;
    }
    //: Set the translation, leave rotation unchanged
    
    //:------------------
    // General operations

    void NormalizeQuart(void)
    { rot.Normalize(); }
    // Make absolutely sure that the quarternion is valid with norm=1
    
    Vector3dC Transform(const Vector3dC &v) const
    { return rot.Rotate(v) + trans; }
    // applys rigid body transformation to v: v1=Rv +t
    
    Vector3dC NTransform(const Vector3dC &n) const
    { return rot.Rotate(n); }
    // applys rigid body transformation to a normal vector : n1=Rn,
    // i.e. the vector is not translated

    Vector3dC InverseTransform(const Vector3dC &v) {
      Vector3dC temp=rot.Inverse().Rotate(v-trans);
      return temp;
    }
    //: Applys rigid body transformation to v: v1=R^-1 (v-T)t
    
    Vector3dC InverseNTransform(const Vector3dC &n) {
      Vector3dC temp=rot.Inverse().Rotate(n);
      return temp;
    }
    //: Applys rigid body transformation  to a normal vector : n1=R^-1 n,
    // i.e. the vector is not translated
    
    RigidTransform3dC Inverse(void) const;
    //: Returns the inverse transform
    
    RigidTransform3dC & ApplyRotationOnly(const Quatern3dC &q);
    //: Apply the specified rotation only to the rot (not the trans)
    
    RigidTransform3dC & ApplyRotationPivot(const Quatern3dC &q, const Vector3dC &pivot);
    //: Apply the specified rotation around the specified pivot point
    
    RigidTransform3dC & ApplyTranslate(const Vector3dC &t);
    //: Translate the RigidT by t
    
    
    const Vector3dC &ExportTranslation() const
    { return trans; }
    //: return translation
    
    Vector3dC ExportAxTheta() const
    { return rot.ExportAxTheta(); }
    //: Return rot as a unit vector axis times theta in radians
    // Possible problems for angles near PI, see Quatern.hh
    
    double ExportRotationAngle() const
    { return rot.ExportRotationAngle(); }
    //: Return the rotation angle in radians.
    // this is around the axis given by ExportRotAxis()
    
    Vector3dC ExportRotationAxis() const
    { return rot.ExportRotationAxis(); }
    //: Return rot as a unit vector axis around which rotated.
    // if Rotation matrix is identity return axis as (0,0,0) vector
    
    const Quatern3dC &ExportRotationQuart() const
    { return rot; }
    //: Return rotation as quarternion
    
    Matrix3dC ExportRotationMatrix() const
    { return rot.ExportRotationMatrix(); }
    //: Return rotation as 3x3 rotation matrix
 
    void Print();
    //: Simple printout 
    
    void LongPrint();
    //: Detailed printout 
    
    RigidTransform3dC Abs();
    //: Normalise rotation quaternian
    
    RigidTransform3dC operator*(const RigidTransform3dC & p) const;
    //: Returns q x p ie applies p then q
    
    inline Vector3dC operator*(const Vector3dC & v) const 
    { return rot.Rotate(v) + trans; }
    //: Apply transform to v
    
    RigidTransform3dC operator*(const RealT& val) const;
    //: returns trans[i] = p1.trans[i]*val (i=0..2) and rot[i] = p1.rot[i]*val (i=0..3)
    
    inline RigidTransform3dC operator+(const RigidTransform3dC& p2) const
    { return RigidTransform3dC(trans + p2.trans,rot + p2.rot); }
    //: returns trans[i] = p1.trans[i] + p2.trans[i] (i=0..2) and 
    //  rot[i] = p1.rot[i] + p2.rot[i] (i=0..3)
    
    inline RigidTransform3dC operator-(const RigidTransform3dC& p2) const
    { return RigidTransform3dC(trans - p2.trans, rot - p2.rot); }
    //: returns trans[i] = p1.trans[i] - p2.trans[i] (i=0..2) and 
    //  rot[i] = p1.rot[i] - p2.rot[i] (i=0..3)
    
    friend ostream & operator<<(ostream & outS, const RigidTransform3dC & rt);
    //: output stream operator
    
    friend istream & operator>>(istream & ins, RigidTransform3dC & rt);
    //: input stream operator

    friend BinOStreamC & operator<<(BinOStreamC & outS, const RigidTransform3dC & rt);
    //: output stream operator
    
    friend BinIStreamC & operator>>(BinIStreamC & ins, RigidTransform3dC & rt);
    //: input stream operator
    
  protected:
    Vector3dC trans;
    Quatern3dC rot;
  };
  
  // -------------------------------------------------------------------------
  
  
  VectorC ConvertRTtoV6(RigidTransform3dC rt);
  // Converts a RigidTrans to a 6 component vector containing 3 vector r
  // and 3 vector trans, note problems for angles near PI 
  // (ExportAxTheta in Quatern.hh)
  
  RigidTransform3dC ConvertV6toRT(VectorC v);
  // Converts a 6 component vector containing 3 vector r and 3 vector trans
  // to a RigidTransform3dC

  
  
}
#endif
