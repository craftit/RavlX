// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlCameraCal
//! author="Joel Mitchelson"
//! docentry="Ravl.API.3D.Camera Modelling"
//! file="Ravl/3D/CameraCal/PinholeCamera1.hh"
#ifndef _PINHOLE_CAMERA_1_HH_
#define _PINHOLE_CAMERA_1_HH_

#include "Ravl/3D/PinholeCamera0.hh"

namespace Ravl3DN
{
  using namespace RavlN;
  using namespace RavlImageN;

  //! userlevel=Develop
  //: Body for a pinhole camera model with 1st order radial lens distortion

  class PinholeCameraBody1C 
    : public PinholeCameraBody0C
  {
  public: 
    PinholeCameraBody1C()
    { }
    //: default constructor
    
    PinholeCameraBody1C(const PinholeCameraBody0C& c0)
    : PinholeCameraBody0C(c0)
    { }
    //: construct from an undistorted pinhole camera
  
    PinholeCameraBody1C(const RealT& cx, const RealT& cy, const RealT& fx, const RealT& fy, const RealT& k1, const Matrix3dC& R, const Vector3dC& t, const IndexRange2dC& frame)
    : PinholeCameraBody0C(cx,cy,fx,fy,R,t,frame), m_k1(k1)
    { }
    //: data constructor

    virtual PinholeCameraBodyC& Copy() const
    {
       return static_cast<PinholeCameraBodyC&>(*new PinholeCameraBody1C(m_cx,m_cy,m_fx,m_fy,m_k1,m_R,m_t,m_frame));
    }
    //: Make a copy of body.

  public:
    
    RealT& k1() 
    { 
      return m_k1; 
    };
    //: first order radial distortion
    
    const RealT& k1() const 
    {
      return m_k1; 
    };
    //: first order radial distortion

  public:
  
    virtual bool Load(istream &strm);
    //: Load from stream.
    
    virtual bool Load(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save(ostream &out) const;
    //: Writes object to stream
    
    virtual bool Save(BinOStreamC &out) const;
    //: Writes object to binary stream
    
  public:
  
    virtual void Project(Vector2dC& z, const Vector3dC& x) const
    {
      Vector3dC Rx = (m_R * x) + m_t;
      RealT dx = m_fx * Rx[0] / Rx[2];
      RealT dy = m_fy * Rx[1] / Rx[2];
      RealT r = Sqrt(dx*dx + dy*dy);
      // Map from undistorted to distorted point
      z[0] = m_cx + dx * (1 + m_k1*r);
      z[1] = m_cy + dy * (1 + m_k1*r);
    }
    //: Project 3D point in space to 2D image point
    //  Projects according to:<br>
    //    z[0] = cx + fx*( (R*x + t)[0] / (R*x + t)[2] )* (1 + k1 * r) <br>
    //    z[1] = cy + fy*( (R*x + t)[1] / (R*x + t)[2] )* (1 + k1 * r) <br>
    //  Can result in a divide-by-zero for degenerate points.
    //  See ProjectCheck if this is to be avoided.

    virtual bool ProjectCheck(Vector2dC& z, const Vector3dC& x) const
    {
      Vector3dC Rx = (m_R * x) + m_t;
      if (Rx[2] > -1E-3 && Rx[2] < 1E-3)
	return false;
      RealT dx = m_fx * Rx[0] / Rx[2];
      RealT dy = m_fy * Rx[1] / Rx[2];
      RealT r = Sqrt(dx*dx + dy*dy);
      z[0] = m_cx + dx * (1 + m_k1*r);
      z[1] = m_cy + dy * (1 + m_k1*r);
      return true;
    }
    //: Project 3D point in space to 2D image point
    // The same as Project(...) but checks that the point
    // is not degenerate.

    virtual void ProjectInverseDirection(Vector3dC& x, const Vector2dC& z) const
    {
      // Map from distorted to undistorted point
      RealT dx = z[0] - m_cx;
      RealT dy = z[1] - m_cy;
      RealT r = sqrt(dx*dx+dy*dy);
      RealT rescale(1.0);
      if (r > 1 && m_k1 > 0 && 1+4*m_k1*r >= 0.0)
      {
         rescale = (-1.0 + sqrt(1+4*m_k1*r)) / (2*m_k1*r);
      }
      Vector3dC Rx;
      Rx[0] = dx * rescale / m_fx;
      Rx[1] = dy * rescale / m_fy;
      Rx[2] = 1.0;
      TMul(m_R,Rx,x);
    } 
    //:Inverse projection up to a scale factor
    // Origin + lambda*ProjectInverseDirection is the camera ray
    // corresponding to image point z.
    
    
    virtual void ProjectJacobian(FMatrixC<2,3>& Jz, const Vector3dC& x) const
    {
      Vector3dC Rx = (m_R * x) + m_t;
      RealT r_Rx2_2 = 1.0 / (Rx[2]*Rx[2]);
      RealT dz0dx0 = m_fx * (m_R[0][0]*Rx[2] - m_R[2][0]*Rx[0]) * r_Rx2_2;
      RealT dz0dx1 = m_fx * (m_R[0][1]*Rx[2] - m_R[2][1]*Rx[0]) * r_Rx2_2;
      RealT dz0dx2 = m_fx * (m_R[0][2]*Rx[2] - m_R[2][2]*Rx[0]) * r_Rx2_2;
      RealT dz1dx0 = m_fy * (m_R[1][0]*Rx[2] - m_R[2][0]*Rx[1]) * r_Rx2_2;
      RealT dz1dx1 = m_fy * (m_R[1][1]*Rx[2] - m_R[2][1]*Rx[1]) * r_Rx2_2;
      RealT dz1dx2 = m_fy * (m_R[1][2]*Rx[2] - m_R[2][2]*Rx[1]) * r_Rx2_2;
      RealT dx = m_fx * Rx[0] / Rx[2];
      RealT dy = m_fy * Rx[1] / Rx[2];
      RealT r = Sqrt(dx*dx + dy*dy);
      RealT a = 1 + m_k1*r;
      Jz[0][0] = dz0dx0 * a;
      Jz[0][1] = dz0dx1 * a;
      Jz[0][2] = dz0dx2 * a;
      Jz[1][0] = dz1dx0 * a;
      Jz[1][1] = dz1dx1 * a;
      Jz[1][2] = dz1dx2 * a;
      if (r > 1 && m_k1>0.0)
      {
        Jz[0][0] += dx * m_k1 * (dz0dx0 + dz1dx0) / r;
        Jz[0][1] += dx * m_k1 * (dz0dx1 + dz1dx1) / r;
        Jz[0][2] += dx * m_k1 * (dz0dx2 + dz1dx2) / r;
        Jz[1][0] += dy * m_k1 * (dz0dx0 + dz1dx0) / r;
        Jz[1][1] += dy * m_k1 * (dz0dx1 + dz1dx1) / r;
        Jz[1][2] += dy * m_k1 * (dz0dx2 + dz1dx2) / r;
      }
    }
    //:The Jacobian matrix of the projection funtion
    
    virtual Vector2dC Undistort(const Vector2dC& z) const
    {
      // Map from distorted to undistorted image
      RealT dx = z.X() - m_cx;
      RealT dy = z.Y() - m_cy;
      RealT d = sqrt(dx*dx+dy*dy);
      RealT rescale(1.0);
      if (d > 1 && m_k1 > 0 && 1+4*m_k1*d >= 0.0)
      {
        rescale = (-1.0 + sqrt(1+4*m_k1*d)) / (2*m_k1*d);
      }
      return Vector2dC( m_cx + rescale*dx, m_cy + rescale*dy );
    }
    //: Return an undistorted image point for a PinholeCamera0C model

    virtual Vector2dC Distort(const Vector2dC& z) const
    {
      RealT dx = z.X() - m_cx;
      RealT dy = z.Y() - m_cy;
      RealT d = sqrt(dx*dx+dy*dy);
      RealT rescale(1.0);
      if (d > 1)
      {
        rescale = (1 + m_k1*d);
      }
      return Vector2dC( m_cx + rescale*dx, m_cy + rescale*dy );
    }
    //: Transform from a simple pinhole model point to a distorted image point

  protected:
    RealT m_k1;
    
  };
  
  //! userlevel=Normal
  //: Pinhole camera model with first order radial lens distortion
  // This class adds distortion (1 + k1 * radius) to the pinhole camera model
  //  Projects 2D image points z from 3D points x according to:<br>
  //    z[0] = cx + fx*( (R*x + t)[0] / (R*x + t)[2] )* (1 + k1 * r) <br>
  //    z[1] = cy + fy*( (R*x + t)[1] / (R*x + t)[2] )* (1 + k1 * r) <br>
  
  class PinholeCamera1C 
    : public PinholeCamera0C
  {
  public:
    PinholeCamera1C()
      : PinholeCamera0C(*new PinholeCameraBody1C())
    {}
    //: Default constructor

    PinholeCamera1C(const RealT& cx, const RealT& cy, const RealT& fx, const RealT& fy, const RealT& k1, const Matrix3dC& R, const Vector3dC& t, const IndexRange2dC& frame)
      : PinholeCamera0C(*new PinholeCameraBody1C(cx, cy, fx, fy, k1, R, t,frame))
    {}
    //: Data constructor

    PinholeCamera1C(const PinholeCameraC &cam)
       : PinholeCamera0C(cam)
    {
       if (dynamic_cast<PinholeCameraBody1C *>(&PinholeCameraC::Body()) == 0)
       {
          Invalidate();
       }
    }
    //: Base class constructor.

  protected:
  
    PinholeCamera1C(PinholeCameraBody1C &bod)
      : PinholeCamera0C(bod)
    {}
    //: Body constructor
    
    PinholeCameraBody1C &Body() 
    { return static_cast<PinholeCameraBody1C &>(PinholeCameraC::Body()); }
    //: Access body.

    const PinholeCameraBody1C &Body() const
    { return static_cast<const PinholeCameraBody1C &>(PinholeCameraC::Body()); }
    //: Access body.
    
  public:
  
    RealT& k1() 
    { 
      return Body().k1();
    };
    //: centre of projection, x co-ordinate

    const RealT& k1() const 
    {
       return Body().k1(); 
    };
    //: centre of projection, x co-ordinate

  };

};

#endif
