// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_Quatern3D_HEADER
#define RAVL_Quatern3D_HEADER
//! date="13/11/95"
//! author="Andrew Stoddart"
//! lib=RavlMath
//! docentry="Ravl.API.Math.Geometry.3D"
//! userlevel=Normal
//! rcsid="$Id$"
//! file="Ravl/Math/Geometry/Euclidean/3D/Quatern3d.hh"

#include "Ravl/Vector3d.hh"
#include "Ravl/Matrix3d.hh"
#include "Ravl/Matrix4d.hh"
#include "Ravl/StdMath.hh"
#include "Ravl/Matrix.hh"

namespace RavlN {
  
  enum JointT  { JNULL, RX, RY, RZ, RXRY, RXRZ, RYRZ, RXRYRZ, TXTYTZRXRYRZ, TXTYTZRZRYRX};
  // This list may be extended in time. Therefore please always use 
  // default in a switch to test for unprocessed option, and not for
  // any other task.
  
  //! userlevel=Obsolete
  //: Quaternion 
  //
  //  <i>This is an excellent class but its interface is not consistent
  //  with the new EulerAngleC and AxisAngleC.  A new version is
  //  on its way soon.</i>
  //
  // Two quarternions may represent the same rotation q and -q,
  // both are valid, but it can be convenient to keep q[0]>0.
  // This code usually tries to keep q[0] positive. All the non trivial
  // constructors build a quarternion with q[0]>=0. 
  // The multiply operation can exit with q[0]<0, this is necessary if
  // the Rotate member function is to work.
  // <p>
  // There are many tricky conversions, and stability issues
  // with rotations and the conversions.
  // Be careful of angles in particular angles near 0 or PI. This affects
  // some conversions as indicated
  // <p>
  // A "valid" quarternion has norm 1, however when a quarternion
  // contains a vector it does not have norm 1.
  // Be careful when using operations that may cause the quarternion
  // not to be valid. 
  // <p>
  // When a quarternion is read in it should always be normalised
  // so that it obeys normalisation to full machine precision
  // which is the normal state of affairs
  // <p>
  // In addition it is possible that small errors may creep in during
  // repeated multiplication. The user should consider using the 
  // Normalize() function to correct this. It is NOT  done automatically 
  // during multiplication.
  // <p>
  
  class Quatern3dC
  {
  public:
    
    //:-------------------------------------
    //: Constructors
    
    Quatern3dC()
    { q[0]=1.0; q[1]=0.0; q[2]=0.0; q[3]=0.0; }
    //: Null Constructor, zero rotation, (1,0,0,0) 
    
    Quatern3dC(RealT q0, RealT q1, RealT q2, RealT q3) 
    { q[0]=q0; q[1]=q1; q[2]=q2; q[3]=q3; }
    //: Constructor - quarternion is not tested for validity.
    
    Quatern3dC(const Vector3dC &axtheta) 
    { Set(axtheta); }
    //: Constructor from axis * angle in radians
    
    Quatern3dC(const Vector3dC &axis, RealT theta) 
    { Set(axis, theta); }
    //: Constructor 
    // If theta != zero then axis must be non zero
    //  the axis vector is normalized inside this routine!
    
    Quatern3dC(const Matrix3dC &rotate, bool check= true)
    { Set(rotate,check); }
    //: Constructor 
    // matrix is checked for orthogonality if check!=0 
    // This routine is reliable for small angles!!
    // NOT ACCURATE FOR ANGLES NEAR PI
    
    //:------------------
    //: General operations 
    
    void Set(const Vector3dC &axtheta);
    // Set as specifed
    
    void Set(const Vector3dC &axis, RealT theta);
    // Set as specifed
    
    void Set(const Matrix3dC &rotate, bool check= true);
    // Set as specifed

    RealT & operator[](UIntT i) { 
      RavlAssert(i <= 4);
      return q[i]; 
    }
    //: Access to ith element of the quarternion
    
    RealT operator[](UIntT i) const { 
      RavlAssert(i <= 4);
      return q[i]; 
    }
    //: Access to ith element of the quarternion
    
    void MakePositive(void);
    // replace quarternion by one with q[0] positive
    // which performs the same rotation
    
    RealT Norm()
    { return Sqrt(q[0]*q[0] + q[1]*q[1]+ q[2]*q[2]+ q[3]*q[3]); }
    //: the norm of the quaternion (quareroot of the sum of the squares of the elements)
    
    void Normalize(void);
    // impose the condition that norm=1
    
    Quatern3dC Inverse() const
      { return Quatern3dC(q[0], -q[1], -q[2], -q[3]); }
    // returns inverse of q, does not change q (Doesn't normalize q; is in fact 
    // the conjugate of q)
    
    Quatern3dC Conjugate() const
      { return Quatern3dC(q[0], -q[1], -q[2], -q[3]); }
    //: Conjugate
    
    Quatern3dC I() const;
    // returns inverse of q, does not change q
    
    Vector3dC  Rotate(Vector3dC  r) const;
    // applies rotation q to r and returns the result
    
    Vector3dC ExportAxTheta() const;
    // return the rotation as a unit vector axis times angle in radians
    // Note warnings for ExportRotAxis();
    
    RealT ExportRotationAngle() const;
    // return the angle of rotation, which should always be 0-PI
    // I believe this routine is accurate for theta near 0 and near PI
    
    Vector3dC ExportRotationAxis() const;
    // return the rotation as a unit vector axis around which rotated
    // If Null Rotation return axis as (1,1,1) vector
    // ********** NB ************
    // For angles very close to PI this routines is not reliable
    // but it is OK for angles near zero.
    // If angle==PI the axis is garbage!
    
    Matrix4dC ExportQuatMat() const;
    // return quaternion as a 4x4 matrix - see Schmitts ECCV 98 paper
    
    MatrixC ExportQuatMat2() const;
    // return quaternion as a MatrixC (4x4) - see Schmitts ECCV 98 paper
    
    Matrix3dC ExportRotationMatrix(bool normalize=false) const;
    // return rotation as 3x3 rotation matrix
    // as far as I know always OK
    
    Vector3dC ExportVector() const;
    // returns the 3 vector components as they are, used for rotating vectors
    
    Vector3dC ExportEuler() const;
    // The angles returned correspond to rotations 
    // around static axes XYZ in radians. 
    
    Vector3dC ExportEuler(const JointT& jtype, const bool& premult) const;
    // The angles returned correspond to rotations 
    
    MatrixC DQuatDRotVec() const;
    //: derivative of the quaternion to the rotation vector
    
    MatrixC DRotVecDQuat() const;
    //: derivative of the rotation vector to the quaternion
    
    RealT func_alpha(const RealT& x, const RealT& thres) const;
    RealT func_gRavl(const RealT& x, const RealT& thres) const;
    RealT func_kappa(const RealT& x, const RealT& thres) const;
    RealT func_lambda(const RealT& x, const RealT& thres) const;
    RealT func_tau(const RealT& thres) const;
    RealT func_ups(const RealT& thres) const;
    //: functions used in DQuatDRotVec & DRotVecDQuat
    
    MatrixC Dq0q1Dq(const Quatern3dC& q, const int& nr) const;
    //: derivative of the product ('this' * 'q') with respect to 'this' (nr=0) 
    //  or 'q' (nr=1)
    
    MatrixC Dq0q1q2Dq(const Quatern3dC& q1, const Quatern3dC& q2, const int& nr) const;
    //: derivative of the product ('this' * 'q1' * 'q2') with respect to 
    //  'this' (nr=0), 'q1' (nr=1) or 'q2' (nr=2)
    
    void Print() const;
    // short print out
    
    void LongPrint() const;
    // detailed print out
    
    Quatern3dC operator*(const Quatern3dC p) const;
    // returns q x p, quarternion multiplication
    // can return q[0] < 0
    
    Quatern3dC operator*(const RealT& val) const
    { return Quatern3dC(q[0]*val, q[1]*val, q[2]*val, q[3]*val); }
    //: returns q[i] = q1[i]*val, for i=0..3
    
    Quatern3dC operator+(const Quatern3dC& p) const
    { return Quatern3dC(p[0]+q[0], p[1]+q[1], p[2]+q[2], p[3]+q[3]); }
    // returns q[i] = q1[i] + q2[i], for i=0..3
    
    Quatern3dC operator-(const Quatern3dC& p) const
      { return Quatern3dC(q[0]-p[0], q[1]-p[1], q[2]-p[2], q[3]-p[3]); }
    // returns q[i] = q1[i] - q2[i], for i=0..3
    
    friend ostream & operator<<(ostream &s, const Quatern3dC & quartern);
    // ouput stream operator

    friend istream & operator>>(istream &s, Quatern3dC & quartern);
    // ouput stream operator

    friend BinOStreamC & operator<<(BinOStreamC &s, const Quatern3dC & quartern);
    // ouput stream operator
    
    friend BinIStreamC & operator>>(BinIStreamC &s, Quatern3dC & quartern);
    // ouput stream operator
    
  private:
    
    RealT q[4];
    
  };
 
  // -------------------------------------------------------------------------
  
  ostream & operator<<(ostream &s, const Quatern3dC & quartern);
  //: output stream operator
  
  istream & operator>>(istream &s,Quatern3dC & quartern);
  //: input stream operator
  
  BinOStreamC & operator<<(BinOStreamC &s, const Quatern3dC & quartern);
  //: Binary output stream operator
  
  BinIStreamC & operator>>(BinIStreamC &s, Quatern3dC & quartern);
  //: Binary input stream operator
  
}
#endif
