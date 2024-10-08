// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlCameraCal
//! author="Joel Mitchelson"
//! docentry="Ravl.API.3D.Camera Modelling"
//! file="Ravl/3D/CameraCal/PinholeCamera2.hh"
#ifndef _PINHOLE_CAMERA_2_HH_
#define _PINHOLE_CAMERA_2_HH_

#include "Ravl/3D/PinholeCamera0.hh"

namespace Ravl3DN
{
  using namespace RavlN;
  using namespace RavlImageN;

  //! userlevel=Develop
  //: Body for a pinhole camera model with 3rd and 5th order radial lens distortion

  class PinholeCameraBody2C 
    : public PinholeCameraBody0C
  {
  public: 
    PinholeCameraBody2C()
    { }
    //: default constructor
    
    PinholeCameraBody2C(const PinholeCameraBody0C& c0)
    : PinholeCameraBody0C(c0)
    { }
    //: construct from an undistorted pinhole camera
  
    PinholeCameraBody2C(const RealT& cx, const RealT& cy, const RealT& fx, const RealT& fy, const RealT& sizex, const RealT& sizey, const RealT& k3, const RealT& k5, const Matrix3dC& R, const Vector3dC& t, const IndexRange2dC& frame)
    : PinholeCameraBody0C(cx,cy,fx,fy,R,t,frame), m_sizex(sizex), m_sizey(sizey), m_k3(k3), m_k5(k5)
    { }
    //: data constructor

    virtual PinholeCameraBodyC& Copy() const
    {
       return static_cast<PinholeCameraBodyC&>(*new PinholeCameraBody2C(m_cx,m_cy,m_fx,m_fy,m_sizex,m_sizey,m_k3,m_k5,m_R,m_t,m_frame));
    }
    //: Make a copy of body.

  public:

    const RealT& k3() const 
    {
      return m_k3; 
    };
    //: 3rd order radial distortion
    
    const RealT& k5() const 
    {
      return m_k5; 
    };
    //: 5th order radial distortion

    const RealT& SizeX() const 
    {
      return m_sizex; 
    };
    //: Effective pixel size 
    
    const RealT& SizeY() const 
    {
      return m_sizey; 
    };
    //: Effective pixel size 

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
       // Distortion-free projection
       Vector3dC Rx = (m_R * x) + m_t;
       RealT dx = m_fx * Rx[0] / Rx[2];
       RealT dy = m_fy * Rx[1] / Rx[2];
       z = Distort(Vector2dC(m_cx + dx, m_cy + dy));
    }
    //: Project 3D point in space to 2D image point
    //  Can result in a divide-by-zero for degenerate points.
    //  See ProjectCheck if this is to be avoided.

    virtual bool ProjectCheck(Vector2dC& z, const Vector3dC& x) const
    {
       // Distortion-free projection
       Vector3dC Rx = (m_R * x) + m_t;
       if (Rx[2] > -1E-3 && Rx[2] < 1E-3)
          return false;
       RealT dx = m_fx * Rx[0] / Rx[2];
       RealT dy = m_fy * Rx[1] / Rx[2];
       z = Distort(Vector2dC(m_cx + dx, m_cy + dy));
       return true;
    }
    //: Project 3D point in space to 2D image point
    // The same as Project(...) but checks that the point
    // is not degenerate.

    virtual void ProjectInverseDirection(Vector3dC& x, const Vector2dC& z) const
    {
      Vector3dC Rx;
      Vector2dC uz = Undistort(z);
      Rx[0] = (uz[0] - m_cx) / m_fx;
      Rx[1] = (uz[1] - m_cy) / m_fy;
      Rx[2] = 1.0;
      TMul(m_R,Rx,x);
    } 
    //:Inverse projection up to a scale factor
    // Origin + lambda*ProjectInverseDirection is the camera ray
    // corresponding to image point z.
        
    virtual Vector2dC Undistort(const Vector2dC& z) const
    {
       Vector2dC ret = z;
       if (m_k3 != 0.0 || m_k5 != 0.0)
       {
          RealT xh = m_sizex * (z[0] - m_cx);
          RealT yh = m_sizey * (z[1] - m_cy);
          RealT rd = xh * xh + yh * yh;
          RealT scale = 1 + m_k3*rd + m_k5 * rd * rd;
          ret[0] = (xh * scale)/m_sizex + m_cx;
          ret[1] = (yh * scale)/m_sizey + m_cy;
       }
       return ret;
    }
    //: Return an undistorted image point for a PinholeCamera0C model

    virtual Vector2dC Distort(const Vector2dC& z) const
    {
       Vector2dC ret = z;
       // NOTE: do not undistort a point greater than one image width/height outside the image as this may not converge
       if ((m_k3 != 0.0 || m_k5 != 0.0) && 
           z[0]>(m_frame.LCol()-m_frame.Cols()) && z[0]<(m_frame.RCol()+m_frame.Cols()) &&
           z[1]>(m_frame.TRow()-m_frame.Rows()) && z[1]<(m_frame.BRow()+m_frame.Rows()))
       {
          RealT xu = m_sizex * ( z[0] - m_cx );
          RealT yu = m_sizey * ( z[1] - m_cy );
          RealT dl = Sqrt(xu * xu + yu * yu);
          // Calculate distance from the centre to the distorted point
          dl = undist2dist(dl, m_k3, m_k5);
          if (dl >= 0)   // if distorting process converged......
          {
             RealT sqdl = dl*dl;
             RealT cudl = sqdl * sqdl;
             RealT scale = 1 + m_k3*sqdl + m_k5 * cudl;
             ret[0] = (xu / scale)/m_sizex + m_cx;
             ret[1] = (yu / scale)/m_sizey + m_cy;
          }
       }
       return ret;
    }
    //: Transform from a simple pinhole model point to a distorted image point

  protected:

     /*
     * Function undist2dist: 
     *    ... given the distance between optical center and ideal projected point
     *    Pu, this function calculates the distance to a Point Pd (distorted),
     *    which describes a (real) projection with radial lensdistortion (set by
     *    parameters K3 & K5). The function performs an iterative search ...
     * PROVIDED BY BBC UNDER IVIEW PROJECT
     */
     static const int max_distiter = 50;
     static double undist2dist(double ru, double k3, double k5)
     {
        int i = max_distiter;
        double rd;

        // new method: expand undistorted radius function (r * scale) as a 1st order Taylor series:
        // undistorted radius = U(distorted radius)
        // U(r) = r * scale = r + K3 * r^3 + K5 * r^5
        //  giving
        // U(r + d) = U(r) + d * dU/dr (+ higher order terms)
        // then write ru = U(r + d) where r is a first guess, and solve for d
        // d  =  ru - U(r) / (dU/dr)
        // and then update estimate r to r+d and iterate until d gets very small

        double delta;
        rd = ru;   // initial estimate for distorted radius = undistorted radius
        do {
           delta = (ru - rd - k3 * rd * rd * rd - k5 * rd * rd * rd * rd * rd) /
              (1 + 3.0 * k3 * rd * rd + 5.0 * k5 * rd * rd * rd * rd);
           rd = rd + delta;
        } while ((fabs(delta) > 1e-8) && i--);

        if (i<0)
        {
           rd = -1.0;    // if iteration fails, return a negative radius
        }
        return rd;
     }

  protected:
    RealT m_sizex; // x pixel size 
    RealT m_sizey; // y pixel size 
    RealT m_k3; // 3rd order radial distortion
    RealT m_k5; // 5th order radial distortion
  };
  
  //! userlevel=Normal
  //: Pinhole camera model with 3rd and 5th order radial lens distortion
  // It uses the polynomial model to undistort the image during back-projection and a numerical approximation to distort the image during projection.  This is the reverse of <a href="Ravl3DN.PinholeCamera3C.html">PinholeCamera2C</a>.
  
  class PinholeCamera2C 
    : public PinholeCamera0C
  {
  public:
    PinholeCamera2C()
      : PinholeCamera0C(*new PinholeCameraBody2C())
    {}
    //: Default constructor

    PinholeCamera2C(const RealT& cx, const RealT& cy, const RealT& fx, const RealT& fy, const RealT& sizex, const RealT& sizey, const RealT& k3, const RealT& k5, const Matrix3dC& R, const Vector3dC& t, const IndexRange2dC& frame)
      : PinholeCamera0C(*new PinholeCameraBody2C(cx, cy, fx, fy, sizex, sizey, k3, k5, R, t,frame))
    {}
    //: Data constructor

    PinholeCamera2C(const PinholeCameraC &cam)
       : PinholeCamera0C(cam)
    {
       if (dynamic_cast<PinholeCameraBody2C *>(&PinholeCameraC::Body()) == 0)
       {
          Invalidate();
       }
    }
    //: Base class constructor.

  protected:
  
    PinholeCamera2C(PinholeCameraBody2C &bod)
      : PinholeCamera0C(bod)
    {}
    //: Body constructor
    
    PinholeCameraBody2C &Body() 
    { return static_cast<PinholeCameraBody2C &>(PinholeCameraC::Body()); }
    //: Access body.

    const PinholeCameraBody2C &Body() const
    { return static_cast<const PinholeCameraBody2C &>(PinholeCameraC::Body()); }
    //: Access body.
    
  public:
  
    const RealT& k3() const 
    {
      return Body().k3(); 
    };
    //: 3rd order radial distortion
    
    const RealT& k5() const 
    {
      return Body().k5(); 
    };
    //: 5th order radial distortion

    const RealT& SizeX() const 
    {
      return Body().SizeX(); 
    };
    //: Effective pixel size 
    
    const RealT& SizeY() const 
    {
      return Body().SizeY(); 
    };
    //: Effective pixel size 
  };

};

#endif
