// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCameraCal
//! author="Joel Mitchelson"
//! docentry="Ravl.API.3D.Camera Modelling"
//! file="Ravl/3D/CameraCal/PinholeCamera0.hh"
#ifndef _CAMERA_CAL_PINHOLE_CAMERA_0_HH_
#define _CAMERA_CAL_PINHOLE_CAMERA_0_HH_

#include "Ravl/Vector2d.hh"
#include "Ravl/Vector3d.hh"
#include "Ravl/Matrix3d.hh"
#include "Ravl/3D/PinholeCamera.hh"

namespace Ravl3DN
{
  using namespace RavlN;

  //! userlevel=Develop
  //: Body for a simple pinhole camera model with no distortion parameters

  class PinholeCameraBody0C 
    : public PinholeCameraBodyC
  {
  public: 
  
    inline PinholeCameraBody0C()
    { }
    //: default constructor

    PinholeCameraBody0C(const RealT& cx, const RealT& cy, const RealT& fx, const RealT& fy, const Matrix3dC& R, const Vector3dC& t, const IndexRange2dC& frame)
    : m_cx(cx), m_cy(cy), m_fx(fx), m_fy(fy), m_R(R), m_t(t), m_frame(frame)
    { }
    //: Data constructor

    virtual PinholeCameraBodyC& Copy() const
    {
       return static_cast<PinholeCameraBodyC&>(*new PinholeCameraBody0C(m_cx,m_cy,m_fx,m_fy,m_R,m_t,m_frame));
    }
    //: Make a copy of body.

  public:
    
    RealT& cx() 
    { 
      return m_cx; 
    };
    //: centre of projection, x co-ordinate

    const RealT& cx() const 
    {
       return m_cx; 
    };
    //: centre of projection, x co-ordinate
    
    RealT& cy() 
    { 
      return m_cy; 
    };
    //: centre of projection, y co-ordinate

    const RealT& cy() const
    {
      return m_cy; 
    };
    //: centre of projection, y co-ordinate

    RealT& fx() 
    { 
      return m_fx; 
    };
    //: focal length in camera y direction (pixels)

    const RealT& fx() const
    {
      return m_fx; 
    };
    //: focal length in camera y direction (pixels)

    RealT& fy() 
    {  
      return m_fy; 
    };
    //: focal length in camera z direction (pixels)

    const RealT& fy() const
    {
      return m_fy; 
    };
    //: focal length in camera z direction (pixels)

    Matrix3dC& R() 
    {
      return m_R; 
    };
    //: rotation world -> camera

    const Matrix3dC& R() const 
    { 
      return m_R; 
    };
    //: rotation world -> camera

    Vector3dC& t() 
    {
      return m_t; 
    };
    //: translation world -> camera (in camera co-ordinates)
    
    const Vector3dC& t() const
    {
      return m_t; 
    };
    //: translation world -> camera (in camera co-ordinates)

    const ImageRectangleC& Frame() const
    {
       return m_frame;
    }
    //: Image frame for the camera

    void SetFrame(const ImageRectangleC& frame)
    {
       m_frame = frame;
    }
    //: Set the image frame for the camera

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
      z[0] = m_cx + m_fx * Rx[0] / Rx[2];
      z[1] = m_cy + m_fy * Rx[1] / Rx[2];
    }
    //: Project 3D point in space to 2D image point
    //  Projects according to:<br>
    //    z[0] = cx + fx*( (R*x + t)[0] / (R*x + t)[2] )<br>
    //    z[1] = cy + fy*( (R*x + t)[1] / (R*x + t)[2] )<br>
    //  Can result in a divide-by-zero for degenerate points.
    //  See ProjectCheck if this is to be avoided.

    virtual bool ProjectCheck(Vector2dC& z, const Vector3dC& x) const
    {
      Vector3dC Rx = (m_R * x) + m_t;
      if (Rx[2] > -1E-3 && Rx[2] < 1E-3)
	return false;
      z[0] = m_cx + m_fx * Rx[0] / Rx[2];
      z[1] = m_cy + m_fy * Rx[1] / Rx[2];
      return true;
    }
    //: Project 3D point in space to 2D image point
    // The same as Project(...) but checks that the point
    // is not degenerate.

    virtual void ProjectJacobian(FMatrixC<2,3>& Jz, const Vector3dC& x) const
    {
      Vector3dC Rx = (m_R * x) + m_t;
      RealT r_Rx2_2 = 1.0 / (Rx[2]*Rx[2]);
      Jz[0][0] = m_fx * (m_R[0][0]*Rx[2] - m_R[2][0]*Rx[0]) * r_Rx2_2;
      Jz[0][1] = m_fx * (m_R[0][1]*Rx[2] - m_R[2][1]*Rx[0]) * r_Rx2_2;
      Jz[0][2] = m_fx * (m_R[0][2]*Rx[2] - m_R[2][2]*Rx[0]) * r_Rx2_2;
      Jz[1][0] = m_fy * (m_R[1][0]*Rx[2] - m_R[2][0]*Rx[1]) * r_Rx2_2;
      Jz[1][1] = m_fy * (m_R[1][1]*Rx[2] - m_R[2][1]*Rx[1]) * r_Rx2_2;
      Jz[1][2] = m_fy * (m_R[1][2]*Rx[2] - m_R[2][2]*Rx[1]) * r_Rx2_2;
    }
    //:The Jacobian matrix of the projection funtion

    virtual void ProjectInverseDirection(Vector3dC& x, const Vector2dC& z) const
    {
      Vector3dC Rx;
      Rx[0] = (z[0] - m_cx) / m_fx;
      Rx[1] = (z[1] - m_cy) / m_fy;
      Rx[2] = 1.0;
      TMul(m_R,Rx,x);
    }
    //:Inverse projection up to a scale factor
    // Origin + lambda*ProjectInverseDirection is the camera ray
    // corresponding to image point z.
    
    virtual void Origin(Vector3dC& org) const
    {
      TMul(m_R,m_t,org);
      org *= -1.0;      
    }
    //: Origin of the camera in world co-ordinates
    //  Computed as -R.T() * t.

    virtual void Direction(Vector3dC& dir) const
    {
       dir[0] = m_R[2][0];
       dir[1] = m_R[2][1];
       dir[2] = m_R[2][2];
    }
    //: Look direction for the camera in the world co-ordinate frame
    // Returns camera z-axis in world coordinate frame

    virtual Vector2dC Undistort(const Vector2dC& z) const
    {
      return z;
    }
    //: Return an undistorted image point for a simple pinhole model

    virtual Vector2dC Distort(const Vector2dC& z) const
    {
      return z;
    }
    //: Transform from a simple pinhole model point to a distorted image point

  protected:
    RealT m_cx;
    RealT m_cy;
    RealT m_fx;
    RealT m_fy;
    Matrix3dC m_R;
    Vector3dC m_t;
    ImageRectangleC m_frame;
  };

  //! userlevel=Normal
  //: Simple pinhole camera model with no lens distortion
  //  Projects 2D image points z from 3D points x according to:<br>
  //    z[0] = cx + fx*( (R*x + t)[0] / (R*x + t)[2] )<br>
  //    z[1] = cy + fy*( (R*x + t)[1] / (R*x + t)[2] )<br>
  
  class PinholeCamera0C 
    : public PinholeCameraC
  {
  public:
    PinholeCamera0C()
      : PinholeCameraC(*new PinholeCameraBody0C())
    {}
    //: Default constructor
    
    PinholeCamera0C(const RealT& cx, const RealT& cy, const RealT& fx, const RealT& fy, const Matrix3dC& R, const Vector3dC& t, const IndexRange2dC& frame)
      : PinholeCameraC(*new PinholeCameraBody0C(cx, cy, fx, fy, R, t,frame))
    {}
    //: Data constructor


    PinholeCamera0C(const PinholeCameraC &cam)
       : PinholeCameraC(cam)
    {
       if (dynamic_cast<PinholeCameraBody0C *>(&PinholeCameraC::Body()) == 0)
       {
          Invalidate();
       }
    }
    //: Base class constructor.
    
  protected:
      
    PinholeCamera0C(PinholeCameraBody0C &bod)
      : PinholeCameraC(bod)
    {}
    //: Body constructor
    
    PinholeCameraBody0C &Body() 
    { return static_cast<PinholeCameraBody0C &>(PinholeCameraC::Body()); }
    //: Access body.
    
    const PinholeCameraBody0C &Body() const
    { return static_cast<const PinholeCameraBody0C &>(PinholeCameraC::Body()); }
    //: Access body.
    
  public:
  
    RealT& cx() 
    { 
      return Body().cx();
    };
    //: centre of projection, x co-ordinate

    const RealT& cx() const 
    {
       return Body().cx(); 
    };
    //: centre of projection, x co-ordinate
    
    RealT& cy() 
    { 
      return Body().cy(); 
    };
    //: centre of projection, y co-ordinate

    const RealT& cy() const
    {
      return Body().cy(); 
    };
    //: centre of projection, y co-ordinate

    RealT& fx() 
    { 
      return Body().fx(); 
    };
    //: focal length in camera y direction (pixels)

    const RealT& fx() const
    {
      return Body().fx(); 
    };
    //: focal length in camera y direction (pixels)

    RealT& fy() 
    {  
      return Body().fy(); 
    };
    //: focal length in camera z direction (pixels)

    const RealT& fy() const
    {
      return Body().fy(); 
    };
    //: focal length in camera z direction (pixels)

    Matrix3dC& R() 
    {
      return Body().R(); 
    };
    //: rotation world -> camera

    const Matrix3dC& R() const 
    { 
      return Body().R(); 
    };
    //: rotation world -> camera

    Vector3dC& t() 
    {
      return Body().t(); 
    };
    //: translation world -> camera (in camera co-ordinates)
    
    const Vector3dC& t() const
    {
      return Body().t(); 
    };
    //: translation world -> camera (in camera co-ordinates)
  }; 
  
};

#endif
