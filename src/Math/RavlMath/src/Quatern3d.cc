// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! author="Andrew Stoddart"
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Euclidean/3D/Quatern3d.cc"

#include "Ravl/StdConst.hh"
#include "Ravl/Quatern3d.hh"
//#include "Ravl/Homtm.hh"
#include "Ravl/Stream.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/config.h"

//#include <iomanip.h>


namespace RavlN {

#if RAVL_OS_IRIX
  // Help the compiler on sgi
  #include "Ravl/TFMatrix.hh"
  TFMatrixC<double,3,3> sgi_xyz; 
#endif

  const RealT axthetaconv=1e-8;
  // used in the conversion between axtheta and quartern
  // when below this bound use shortcut for better accuracy
  
  const RealT nearlyzero=1e-13;
  // used in the orthogonality error check for the constructor from matrix
  
  const RealT underflow = 1e-50;
  // angles smaller than this value are assumed zero, used when constructing
  // from angle and axis.
  // If the length of the axis is less than this, an error is recorded
  // Export Angle and Export Axis are also affected.
  
  void Quatern3dC::Set(const Vector3dC &axtheta) {
    RealT theta=axtheta.Magnitude();
    if (theta<axthetaconv) {
      q[0]=1.0; 
      q[1]=axtheta[0]/2.0;
      q[2]=axtheta[1]/2.0;
      q[3]=axtheta[2]/2.0;
      return;
    }
    
    Vector3dC unitAxtheta = axtheta.Unit();
    q[0]=cos(theta/2.0);
    RealT s=sin(theta/2.0);
    for (int i=0; i<3; ++i)
      q[i+1] = s * unitAxtheta[i];
    
    //MakePositive();
    // Without "MakePositive()" Quatern3dC(vector v).ExporAxTheta gives back 
    //  the original vector v for |v| = phi < 360 deg
    // With "MakePositive()" Quatern3dC(vector v).ExporAxTheta gives back 
    // the original vector v for |v| = phi < 180 deg
  }  
  
  void Quatern3dC::Set(const Vector3dC &axis, RealT theta) {
    if (fabs(theta)<underflow) {
      q[0]=1.0; q[1]=0.0; q[2]=0.0; q[3]=0.0;
      return;
    }
    RavlAlwaysAssertMsg(axis.Magnitude() > underflow,
		  "Construct Quatern3dC from axis and angle\n"
		  "axis = (0,0,0) for non zero angle");
    
    Vector3dC unitAxis = axis.Unit();
    q[0]=cos(theta/2.0);
    RealT s=sin(theta/2.0);
    for (int i=0; i<3; ++i) 
      q[i+1] = s * unitAxis[i];
    MakePositive();
  }
  
  
  // The angle of rotation is always 0-PI on exit, i.e. q[0] > 0
  
  void Quatern3dC::Set(const Matrix3dC &rot,bool check) {
    // Error checking
    // --------------
    if (check) {
      Matrix3dC temp; //=rot * rot.T();
#if RAVL_COMPILER_VISUALCPP
      MulM<RealT,3,3,3>(rot,rot.T(),temp);
#else
      MulM(rot,rot.T(),temp);
#endif
      RealT error=0.0;
      int i,j;
      for (i=0; i<3; ++i) {
	for (j=0; j<3; ++j) {
	  error=error + fabs(temp[i][j] - (i==j));
	}
      }
      if ((error>10*nearlyzero) || (fabs(rot.Det() - 1.0)>nearlyzero)) {
	cerr << "Invalid rotation matrix\n" 
	     << "matrix =" << rot << "\n"
	     << "1 - det  =" << 1.0 - rot.Det() << "\n"
	     << "error in rot * rot.T() =" <<  error << "\n"
	     << "rot * rot.T() =" << temp << "\n";
	RavlAssert(0);
      }
    }
    
    // Now convert the rotation matrix to a quarternion
    // ------------------------------------------------
    RealT trace=rot[0][0]+rot[1][1]+rot[2][2];
    RealT q0=this->q[0]=sqrt(fabs((1.0+trace)/4.0)); 
    // the fabs shouldnt be necessary
    
    RealT n0=rot[2][1]-rot[1][2];
    RealT n1=rot[0][2]-rot[2][0];
    RealT n2=rot[1][0]-rot[0][1];
    
    if (q0 > 0.5) { // the case of angle near zero
      this->q[1] = n0/4.0/q0;
      this->q[2] = n1/4.0/q0;
      this->q[3] = n2/4.0/q0;
    } else { // large angles up to PI
      
      // based on code by Peter Kurdel,  utrrkurd@savba.sk
      // this works correctly for angles near PI and on PI
      // but can be inaccurate because q[0] is not found to the precision
      // it can be!
      RealT x1,yy,u,v;
      
      x1 = rot[0][0] - rot[1][1];
      yy = rot[0][0] + rot[1][1];
      u  = 1 - rot[2][2];
      v  = 1 + rot[2][2];
      q[1] = sqrt( fabs( u + x1 ) ) * .5;
      q[2] = sqrt( fabs( u - x1 ) ) * .5;
      q[3] = sqrt( fabs( v - yy ) ) * .5;
      
      if( n0 < 0. ) q[1] = -q[1] ;
      if( n1 < 0. ) q[2] = -q[2] ;
      if( n2 < 0. ) q[3] = -q[3] ;
    }
  }
  
  // Access to ith element of the quarternion
  
  
  void Quatern3dC::MakePositive(void) {
    if (q[0]<0) 
      for (int i=0; i<4; ++i) 
	q[i] = - q[i];
  }
  
  void Quatern3dC::Normalize(void) {
    RealT denom=sqrt(q[0]*q[0] + q[1]*q[1]+ q[2]*q[2]+ q[3]*q[3]);    
    for (int i=0; i<4; ++i) 
      q[i] = q[i]/denom;
  }
  
  Quatern3dC Quatern3dC::I() const {
    Quatern3dC qq = *this;
    RealT norm2 = q[0]*q[0] + q[1]*q[1]+ q[2]*q[2]+ q[3]*q[3];
    for (int i=0; i<4; ++i) 
      qq[i] = qq[i]/norm2;
    return Quatern3dC(qq[0], -qq[1], -qq[2], -qq[3]);
  }
  
  // applies rotation q to v1 and returns the result
  
  Vector3dC Quatern3dC::Rotate(Vector3dC  v1) const {
    Quatern3dC v(0.0, v1[0], v1[1], v1[2]);
    v =   (*this)*v*(this->Inverse());
    return  v.ExportVector();
  }
  
  Vector3dC Quatern3dC::ExportAxTheta() const {
    RealT theta=ExportRotationAngle();
    if (theta<axthetaconv) {
      // small angles
      Vector3dC r(q[1],q[2],q[3]);
      return r*2.0;
    } else {
      // normal angles
      Vector3dC r(q[1],q[2],q[3]);
      r.MakeUnit();
      return r*theta;
    }
  }
  
  RealT Quatern3dC::ExportRotationAngle() const {
    if ((q[0]>1.0000000001) || (q[0]<-1.0000000001)) {
      cerr << "Pathological quarternion in \n"  
	   << "q[0]="        << q[0]           << "\n"
	   << "fabs(q[0]-1)=" << fabs(q[0])-1.0 << "\n";
      RavlAssert(0);
    }
    
    RealT theta;
    
    // We must be tolerant to small deviations above 1 for q[0]
    // due to roundoff error
    if ((q[0]>1.0) || (q[0]<-1.0)) {
      theta=0.0;  
    } else {
      theta=2.0 * acos(q[0]);
    }
    
    // special routine for small angles
    // --------------------------------
    if (theta<1e-4) {
      RealT temp=ExportVector().Magnitude();
      RavlAlwaysAssertMsg((temp<=1.0) && (q[0]>=-1.0),"Pathological quarternion. ");
      theta=2.0 * asin(temp);
    }
    
    return theta;
  }
  
  Vector3dC Quatern3dC::ExportRotationAxis() const {
    RealT theta=ExportRotationAngle();
    
    if (theta<underflow) {
      Vector3dC r(1,1,1);
      return r;
    } else {
      Vector3dC r(q[1],q[2],q[3]);
      r.MakeUnit();
      return r;
    }
  }
    
  Matrix3dC Quatern3dC::ExportRotationMatrix(bool normalize) const {
    Quatern3dC t = (*this);
    if (normalize) t.Normalize();
    
    Matrix3dC m;
    
    m[0][0]=t.q[0]*t.q[0] + t.q[1]*t.q[1] - t.q[2]*t.q[2] - t.q[3]*t.q[3];
    m[0][1]=2.0*(t.q[1]*t.q[2] - t.q[0]*t.q[3]);
    m[0][2]=2.0*(t.q[1]*t.q[3] + t.q[0]*t.q[2]);
    m[1][0]=2.0*(t.q[1]*t.q[2] + t.q[0]*t.q[3]);
    m[1][1]=t.q[0]*t.q[0] - t.q[1]*t.q[1] + t.q[2]*t.q[2] - t.q[3]*t.q[3];
    m[1][2]=2.0*(t.q[2]*t.q[3] - t.q[0]*t.q[1]);
    m[2][0]=2.0*(t.q[1]*t.q[3] - t.q[0]*t.q[2]);
    m[2][1]=2.0*(t.q[2]*t.q[3] + t.q[0]*t.q[1]);
    m[2][2]=t.q[0]*t.q[0] - t.q[1]*t.q[1] - t.q[2]*t.q[2] + t.q[3]*t.q[3];
    
    return m;
  }
  
  Matrix4dC Quatern3dC::ExportQuatMat() const {
    Matrix4dC m;
    
    m[0][0] =  q[0];
    m[0][1] = -q[1];
    m[0][2] = -q[2];
    m[0][3] = -q[3];
    
    m[1][0] =  q[1];
    m[1][1] =  q[0];
    m[1][2] = -q[3];
    m[1][3] =  q[2];
    
    m[2][0] =  q[2];
    m[2][1] =  q[3];
    m[2][2] =  q[0];
    m[2][3] = -q[1];
    
    m[3][0] =  q[3];
    m[3][1] = -q[2];
    m[3][2] =  q[1];
    m[3][3] =  q[0];
    
    return m;
  }
  
  MatrixC Quatern3dC::ExportQuatMat2() const {
    MatrixC m(4, 4);
    
    m[0][0] =  q[0];
    m[0][1] = -q[1];
    m[0][2] = -q[2];
    m[0][3] = -q[3];
    
    m[1][0] =  q[1];
    m[1][1] =  q[0];
    m[1][2] = -q[3];
    m[1][3] =  q[2];
    
    m[2][0] =  q[2];
    m[2][1] =  q[3];
    m[2][2] =  q[0];
    m[2][3] = -q[1];
    
    m[3][0] =  q[3];
    m[3][1] = -q[2];
    m[3][2] =  q[1];
    m[3][3] =  q[0];
    
    return m;
  }
  
  Vector3dC Quatern3dC::ExportVector() const {
    Vector3dC v(q[1], q[2], q[3]);
    return v;
  } 
  
#if 0
  Vector3dC Quatern3dC::ExportEuler() const {
    return HomtmC::RotToEuler(this->ExportRotationMatrix());
  }
  
  Vector3dC Quatern3dC::ExportEuler(const JointT& jtype, const bool& premult) const {
    return HomtmC::RotToEuler(this->ExportRotationMatrix(), jtype, premult);
  }
#endif
  
  MatrixC Quatern3dC::DQuatDRotVec() const {
    RealT     theta  = ExportRotationAngle();
    Vector3dC rotvec = ExportAxTheta();
    Vector3dC qvec   = ExportVector();
    MatrixC   dqdr(4,3);
    for (int i=0; i<3; i++) {
      dqdr[0][i] = -0.5 * qvec[i];
      for (int j=0; j<3; j++) {
	dqdr[i+1][j] = func_lambda(theta, axthetaconv) * rotvec[i]*rotvec[j];
      }
      dqdr[i+1][i] += func_kappa(theta, axthetaconv);
    }
    return dqdr;
  }
  
  MatrixC Quatern3dC::DRotVecDQuat() const {
    Vector3dC rotvec = ExportRotationAxis();
    Vector3dC qvec   = ExportVector();
    MatrixC   drdq(3,4);
    for (int i=0; i<3; i++) {
      drdq[i][0] = -2.0 * qvec[i];
      for (int j=0; j<3; j++) {
	drdq[i][j+1] = func_ups(axthetaconv) * rotvec[i]*rotvec[j];
      }
      drdq[i][i+1] += func_tau(axthetaconv);
    }
    return drdq;
  }
  
  
  const RealT s=1e-16; 
  
  RealT Quatern3dC::func_alpha(const RealT& x, const RealT& thres) const {
    if (x >= thres) return sin(x)/x;
    else {
      int n=1; RealT f=0.0; RealT d=1.0; 
      do { 
	f+=d; 
	d=-d*x*x; 
	d/=++n; 
	d/=++n; 
      } while (fabs(d)>s);
      return f;
    }
    return 0;
  }
  
  RealT Quatern3dC::func_gRavl(const RealT& x, const RealT& thres) const {
    // equals d(func_alpha)d(x)
    
    if (x >= thres) return cos(x)/(x*x) - sin(x)/(x*x*x);
    else {
      int n=3; RealT f=0.0; RealT d=-1./6.; 
      do { 
	f+=(n-1)*d; 
	d=-d*x*x; 
	d/=++n; 
	d/=++n; 
      } while (fabs(d)>s);
      return f;
    }
    return 0;
  }
  
  RealT Quatern3dC::func_kappa(const RealT& x, const RealT& thres) const {  
    return 0.5*func_alpha(0.5*x, thres);  
  }

  RealT Quatern3dC::func_lambda(const RealT& x, const RealT& thres) const {  
    return func_gRavl(0.5*x, thres) / 8;  
  }

  RealT Quatern3dC::func_tau(const RealT& thres) const {
    RealT x = ExportVector().Magnitude();
    if (x >= thres) return 2*Sign(q[0])*asin(x)/x;
    else { 
      int n = 1; RealT f = 0.0; RealT d = 1;
      do { 
	f += d; 
	d *= x*x*n*n; 
	d /= ++n; 
	d /= ++n; 
      } while (fabs(d)>s);
      return f;
    }
    return 0;
  }
  
  RealT Quatern3dC::func_ups(const RealT& thres) const {
    RealT x = ExportVector().Magnitude();
    if (x >= thres) 
      return (2*Sign(q[0])*(x*sqrt(1-x*x) - asin(x))/(x*x*x));
    else {
      RealT f = -2./3.+(-1./5.+(-3./28.+(-5./72.+(-35./704.+(-63./1664.+(-77./2560.+(-429./17408.-6435./311296.*x*x)*x*x)*x*x)*x*x)*x*x)*x*x)*x*x)*x*x;  
    return f;
    }
    return 0;
  }
  
  
  MatrixC Quatern3dC::Dq0q1Dq(const Quatern3dC& q1, const int& nr) const {
    int i,j;
    Quatern3dC q;
    MatrixC   dq0q1dq(4,4); dq0q1dq.Fill(0);
    Matrix3dC cross; 
    cross.Fill(0);
    if (nr==0) {
      q = q1;
      cross = Matrix3dC(0,q[3],-q[2], -q[3],0,q[1], q[2],-q[1],0);
    }
    if (nr==1) {
      q = *this;
      cross = Matrix3dC(0,-q[3],q[2], q[3],0,-q[1], -q[2],q[1],0);
    }
    dq0q1dq[0][0] = q[0];
    for (i=0; i<3; i++) {
      dq0q1dq[0][i+1] = -q[i+1];
      dq0q1dq[i+1][0] = q[i+1];
      for (j=0; j<3; j++)
	dq0q1dq[i+1][j+1] = cross[i][j];
      dq0q1dq[i+1][i+1] += q[0];
    }
    return dq0q1dq;
  }
  
  
  MatrixC Quatern3dC::Dq0q1q2Dq(const Quatern3dC& q1, const Quatern3dC& q2, const int& nr) const {
    Quatern3dC q3 = q1 * q2;
    if (nr==0) return  Dq0q1Dq(q3, 0);
    if (nr==1) return (Dq0q1Dq(q3, 1) * q1.Dq0q1Dq(q2, 0));
    if (nr==2) return (Dq0q1Dq(q3, 1) * q1.Dq0q1Dq(q2, 1));
    cout << "Error: Wrong input in Quatern3dC::Dq0q1q2Dq. \n";
    return MatrixC();
  }
  
  void Quatern3dC::Print() const {
    RealT temp=ExportRotationAngle();
    
    cout << "The rotation is equivalent to a rotation of angle \n"
	 << temp/RavlConstN::pi*180 << " degrees " 
	 << "around a unit vector \n"
	 << ExportRotationAxis()  << "\n";
  }
  
  
  void Quatern3dC::LongPrint() const {
    Matrix3dC rot=ExportRotationMatrix();
    cout << "Quatern3dC: The rotation matrix is \n" 
	 << rot << "\n";
    
    RealT temp=ExportRotationAngle();
    
    cout << "The rotation is equivalent to a rotation of angle \n"
	 << temp << "  radians (" 
	 << temp/RavlConstN::pi*180 << " degrees) " 
	 << "around a unit vector \n"
	 << ExportRotationAxis()  << "\n";
    
    cout << "The quarternion is \n"
	 << q[0] << "   " 
	 << q[1] << "   " 
	 << q[2] << "   " 
	 << q[3] << "\n";
  }
  
  // returns q x p, quarternion multiplication
  // Do NOT MakePositive, this causes errors in Rotate()
  Quatern3dC Quatern3dC::operator*(const Quatern3dC p) const {
    return Quatern3dC( q[0]*p.q[0] - q[1]*p.q[1] - q[2]*p.q[2] - q[3]*p.q[3],
			q[0]*p.q[1] + q[1]*p.q[0] + q[2]*p.q[3] - q[3]*p.q[2],
			q[0]*p.q[2] + q[2]*p.q[0] + q[3]*p.q[1] - q[1]*p.q[3],
			q[0]*p.q[3] + q[3]*p.q[0] + q[1]*p.q[2] - q[2]*p.q[1]);
  }
  
  ostream & operator<<(ostream & outS, const Quatern3dC & quartern) {
    outS << quartern.q[0] << " " << quartern.q[1] << " "
	 << quartern.q[2] << " " << quartern.q[3];
    return outS;
  }
  
  //: input stream operator
  
  istream & operator>>(istream &s,Quatern3dC & quartern) {
    s >> quartern.q[0] >> quartern.q[1] >> quartern.q[2] >> quartern.q[3];
    return s;
  }
  
  //: Binary output stream operator
  
  BinOStreamC & operator<<(BinOStreamC &s, const Quatern3dC & quartern) {
    s << quartern.q[0] << quartern.q[1] << quartern.q[2] << quartern.q[3];
    return s;
  }
  
  //: Binary input stream operator
  
  BinIStreamC & operator>>(BinIStreamC &s, Quatern3dC & quartern) {
    s >> quartern.q[0] >> quartern.q[1] >> quartern.q[2] >> quartern.q[3];
    return s;
  }

}
