// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef _PINHOLE_CAMERA_HH_
#define _PINHOLE_CAMERA_HH_
//! docentry="Ravl.API.3D.Camera Modelling"
//! lib=RavlCameraCal

#include "Ravl/RefCounter.hh"
#include "Ravl/Vector2d.hh"
#include "Ravl/Vector3d.hh"
#include "Ravl/Stream.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/IndexRange2dIter.hh"
#include "Ravl/Image/BilinearInterpolation.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ImageRectangle.hh"

namespace Ravl3DN
{
  using namespace RavlN;
  using namespace RavlImageN;

  //! userlevel=Normal
  //: Base body for the pinhole camera model
  
  class PinholeCameraBodyC
    : public RCBodyC 
  {
  public:
  
    PinholeCameraBodyC()
    { }
    //: Default constructor.
    
    virtual ~PinholeCameraBodyC()
    { }
    //: Destructor.

    virtual PinholeCameraBodyC& Copy() const = 0;
    //: Make a copy of body.
    // This should be provided in derived classes.

    virtual bool Load(istream &strm) = 0;
    //: Load from stream.
    // This should be provided in derived classes.
    
    virtual bool Load(BinIStreamC &strm) = 0;
    //: Load from binary stream.
    // This should be provided in derived classes.
    
    virtual bool Save(ostream &out) const = 0;
    //: Writes object to stream
    // This should be provided in derived classes.
    
    virtual bool Save(BinOStreamC &out) const = 0;
    //: Writes object to binary stream
    // This should be provided in derived classes.
    
    virtual void Project(Vector2dC& z, const Vector3dC& x) const = 0;
    //: Project a 3D point to a 2D image point
    // This should be provided in derived classes.

    virtual bool ProjectCheck(Vector2dC& z, const Vector3dC& x) const = 0;
    //: Project 3D point in space to 2D image point checking for degeneracy
    // This should be provided in derived classes.

    virtual void ProjectInverseDirection(Vector3dC& x, const Vector2dC& z) const =0;
    //:Inverse projection up to a scale factor
    // Origin + lambda*x is the camera ray for image point z.
    // This should be provided in derived classes.
    
    virtual void ProjectJacobian(FMatrixC<2,3>& Jz, const Vector3dC& x) const = 0;
    //:The Jacobian matrix of the projection funtion
    // This should be provided in derived classes.

    virtual void Origin(Vector3dC& org) const = 0;
    //: Origin of the camera in world co-ordinates
    // This should be provided in derived classes.

    virtual void Direction(Vector3dC& dit) const = 0;
    //: Look direction for the camera in the world co-ordinate frame
    // This should be provided in derived classes.

    virtual const ImageRectangleC& Frame() const = 0;
    //: Image frame for the camera
    // This should be provided in derived classes.

    virtual void SetFrame(const ImageRectangleC& frame) = 0;
    //: Set the image frame for the camera
    // This should be provided in derived classes.

    virtual Vector2dC Undistort(const Vector2dC& z) const = 0;
    //: Transform from a camera point to a point in a simple pinhole model
    // This should be provided in derived classes.

    virtual Vector2dC Distort(const Vector2dC& z) const = 0;
    //: Transform from a simple pinhole model point to a distorted image point
    // This should be provided in derived classes.

    template<class PixelT> 
    ImageC<PixelT> UndistortImage(const ImageC<PixelT>& img, bool resize = false) const;
    //: Return an undistorted image for a simple pinhole model

    template<class PixelT> 
    ImageC<PixelT> UndistortImage(const ImageC<PixelT>& img, const PixelT bg, bool resize = false) const;
    //: Return an undistorted image for a simple pinhole model filling empty pixels with bg value

    template<class PixelT> 
    ImageC<PixelT> DistortImage(const ImageC<PixelT>& img, bool resize = false) const;
    //: Return a distorted image for a simple pinhole model

    template<class PixelT> 
    ImageC<PixelT> DistortImage(const ImageC<PixelT>& img, const PixelT bg, bool resize = false) const;
    //: Return a distorted image for a simple pinhole model filling empty pixels with bg value

  };
    //: Transform from a camera point to a point in a simple pinhole model

  //! userlevel=Normal
  //: Base Pinhole Camera handle.
  // This class provides access to transform between 3D world and 2D image points.
  
  class PinholeCameraC 
    : public RCHandleC<PinholeCameraBodyC>
  {
  public: 
  
    PinholeCameraC() 
    {}
    //: Default constructor.
    
    PinholeCameraC(const PinholeCameraC &oth) 
      : RCHandleC<PinholeCameraBodyC>(oth)
    {}
    //: Copy constructor.
    
  protected:
     
    PinholeCameraC(PinholeCameraBodyC &bod) 
      : RCHandleC<PinholeCameraBodyC>(bod)
    {}
    //: Body Constructor.
    
    PinholeCameraBodyC &Body() 
    { return RCHandleC<PinholeCameraBodyC>::Body(); }
    //: Access body.
    
    const PinholeCameraBodyC &Body() const
    { return RCHandleC<PinholeCameraBodyC>::Body(); }
    //: Access body.  
  
  public:
  
    PinholeCameraC Copy()
    {
       return PinholeCameraC(Body().Copy());
    }
    //: Return a copy

    void Project(Vector2dC& z, const Vector3dC& x) const
    {
      Body().Project(z,x);
    }
    //: Project a 3D point to a 2D image point

    bool ProjectCheck(Vector2dC& z, const Vector3dC& x) const
    {
      return Body().ProjectCheck(z,x);
    }
    //: Project 3D point in space to 2D image point checking for degeneracy

    void ProjectInverseDirection(Vector3dC& x, const Vector2dC& z) const
    {
      Body().ProjectInverseDirection(x,z);
    }
    //:Inverse projection up to a scale factor
    // Origin + lambda*x is the camera ray for image point z.
    
    void ProjectJacobian(FMatrixC<2,3>& Jz, const Vector3dC& x) const
    {
      Body().ProjectJacobian(Jz, x);
    }
    //:The Jacobian matrix of the projection funtion

    void Origin(Vector3dC& org) const
    {
      Body().Origin(org);
    }
    //: Origin of the camera in world co-ordinates
    
    void Direction(Vector3dC& dir) const
    {
      Body().Direction(dir);
    }
    //: Origin of the camera in world co-ordinates

    const ImageRectangleC& Frame() const
    {
       return Body().Frame();
    }
    //: Image frame for the camera

    void SetFrame(const ImageRectangleC& frame)
    {
       Body().SetFrame(frame);
    }
    //: Set the image frame for the camera

    Vector2dC Undistort(const Vector2dC& z) const
    {
      return Body().Undistort(z);
    }
    //: Transform from a camera point to a point in a simple pinhole model

    Vector2dC Distort(const Vector2dC& z) const
    {
      return Body().Distort(z);
    }
    //: Transform from a simple pinhole model point to a distorted image point

    template<class PixelT> ImageC<PixelT> 
    UndistortImage(const ImageC<PixelT>& img, bool resize = false) const
    {
      return Body().UndistortImage(img, resize);
    }
    //: Return an undistorted image for a simple pinhole model

    template<class PixelT> ImageC<PixelT> 
    UndistortImage(const ImageC<PixelT>& img, const PixelT bg, bool resize = false) const
    {
      return Body().UndistortImage(img, bg, resize);
    }
    //: Return a distorted image for a simple pinhole model filling empty pixels with bg value

    template<class PixelT> ImageC<PixelT> 
    DistortImage(const ImageC<PixelT>& img, bool resize = false) const
    {
      return Body().DistortImage(img, resize);
    }
    //: Return an undistorted image for a simple pinhole model

    template<class PixelT> ImageC<PixelT> 
    DistortImage(const ImageC<PixelT>& img, const PixelT bg, bool resize = false) const
    {
      return Body().DistortImage(img, bg, resize);
    }
    //: Return a distorted image for a simple pinhole model filling empty pixels with bg value

    friend istream &operator>>(istream &strm, PinholeCameraC &obj);
    friend ostream &operator<<(ostream &out, const PinholeCameraC &obj);
    friend BinIStreamC &operator>>(BinIStreamC &strm, PinholeCameraC &obj);
    friend BinOStreamC &operator<<(BinOStreamC &out, const PinholeCameraC &obj);
  };

 
  inline istream &operator>>(istream &strm, PinholeCameraC &obj) 
  {
    obj.Body().Load(strm);
    return strm;
  }
  //: Load from a stream.
  
  inline ostream &operator<<(ostream &out, const PinholeCameraC &obj) 
  {
    obj.Body().Save(out);
    return out;
  }
  //: Save to a stream.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm, PinholeCameraC &obj) 
  {
    obj.Body().Load(strm);
    return strm;
  }
  //: Load from a binary stream.
  
  inline BinOStreamC &operator<<(BinOStreamC &out, const PinholeCameraC &obj) 
  {
    obj.Body().Save(out);
    return out;
  }
  //: Save to a binary stream.

  template<class PixelT> ImageC<PixelT>
  PinholeCameraBodyC::UndistortImage(const ImageC<PixelT>& img, bool resize) const
  {
     SampleBilinearC<PixelT, PixelT> sample;
     IndexRange2dC rect;
     RealT minr = img.Frame().TRow(); RealT maxr = img.Frame().BRow();
     RealT minc = img.Frame().LCol(); RealT maxc = img.Frame().RCol();

     if(resize) {
       Array2dIterC<PixelT> it0(img);
       IndexRange2dIterC itrect0(img.Frame());
       for (; it0; it0++, itrect0++)
       {
         Index2dC pix = itrect0.Data();
         if(pix.Row()==minr || pix.Row()==maxr || pix.Col()==minc || pix.Col()==maxc) {
           Vector2dC z = Undistort(Vector2dC(pix.Col().V(), pix.Row().V()));
           rect.Involve(Index2dC(z.Y(),z.X()));
         }
       }
     } else {
        rect = img.Frame();
     }

     ImageC<PixelT> ret(rect);

     Array2dIterC<PixelT> it(ret);
     IndexRange2dIterC itrect(rect);
     for (; it; it++, itrect++)
     {
       Index2dC pix = itrect.Data();
       Vector2dC z = Distort(Vector2dC(pix.Col().V(), pix.Row().V()));
       if (z.Y()>minr && z.Y()<maxr && z.X()>minc && z.X()<maxc)
       {
         sample(img, Vector2dC(z.Y(),z.X()), it.Data());
       }
     }

     return ret;
  }
  //: Derive undistort an image using the 1st order radial distortion model

  template<class PixelT> ImageC<PixelT>
  PinholeCameraBodyC::UndistortImage(const ImageC<PixelT>& img, const PixelT bg, bool resize) const
  {
     SampleBilinearC<PixelT, PixelT> sample;
     IndexRange2dC rect;
     RealT minr = img.Frame().TRow(); RealT maxr = img.Frame().BRow();
     RealT minc = img.Frame().LCol(); RealT maxc = img.Frame().RCol();

     if(resize) {
       Array2dIterC<PixelT> it0(img);
       IndexRange2dIterC itrect0(img.Frame());
       for (; it0; it0++, itrect0++)
       {
         Index2dC pix = itrect0.Data();
         if(pix.Row()==minr || pix.Row()==maxr || pix.Col()==minc || pix.Col()==maxc) {
           Vector2dC z = Undistort(Vector2dC(pix.Col().V(), pix.Row().V()));
           rect.Involve(Index2dC(z.Y(),z.X()));
         }
       }
     } else {
        rect = img.Frame();
     }

     ImageC<PixelT> ret(rect);

     Array2dIterC<PixelT> it(ret);
     IndexRange2dIterC itrect(rect);
     for (; it; it++, itrect++)
     {
       Index2dC pix = itrect.Data();
       Vector2dC z = Distort(Vector2dC(pix.Col().V(), pix.Row().V()));
       if (z.Y()>minr && z.Y()<maxr && z.X()>minc && z.X()<maxc)
       {
         sample(img, Vector2dC(z.Y(),z.X()), it.Data());
       } else {
         it.Data() = bg;
       }
     }

     return ret;
  }
  //: Derive undistort an image using the 1st order radial distortion model filling empty pixels with bg value

  template<class PixelT> ImageC<PixelT>
  PinholeCameraBodyC::DistortImage(const ImageC<PixelT>& img, bool resize) const
  {
     SampleBilinearC<PixelT, PixelT> sample;
     IndexRange2dC rect;
     RealT minr = img.Frame().TRow(); RealT maxr = img.Frame().BRow();
     RealT minc = img.Frame().LCol(); RealT maxc = img.Frame().RCol();

     if(resize) {
       Array2dIterC<PixelT> it0(img);
       IndexRange2dIterC itrect0(img.Frame());
       for (; it0; it0++, itrect0++)
       {
         Index2dC pix = itrect0.Data();
         if(pix.Row()==minr || pix.Row()==maxr || pix.Col()==minc || pix.Col()==maxc) {
           Vector2dC z = Distort(Vector2dC(pix.Col().V(), pix.Row().V()));
           rect.Involve(Index2dC(z.Y(),z.X()));
         }
       }
     } else {
        rect = img.Frame();
     }

     ImageC<PixelT> ret(rect);

     Array2dIterC<PixelT> it(ret);
     IndexRange2dIterC itrect(rect);
     for (; it; it++, itrect++)
     {
       Index2dC pix = itrect.Data();
       Vector2dC z = Undistort(Vector2dC(pix.Col().V(), pix.Row().V()));
       if (z.Y()>minr && z.Y()<maxr && z.X()>minc && z.X()<maxc)
       {
         sample(img, Vector2dC(z.Y(),z.X()), it.Data());
       }
     }

     return ret;
  }
  //: Derive distort an image using the 1st order radial distortion model

  template<class PixelT> ImageC<PixelT>
  PinholeCameraBodyC::DistortImage(const ImageC<PixelT>& img, const PixelT bg, bool resize) const
  {
     SampleBilinearC<PixelT, PixelT> sample;
     RealT minr = img.Frame().TRow(); RealT maxr = img.Frame().BRow();
     RealT minc = img.Frame().LCol(); RealT maxc = img.Frame().RCol();
     IndexRange2dC rect;

     if(resize) {
       Array2dIterC<PixelT> it0(img);
       IndexRange2dIterC itrect0(img.Frame());
       for (; it0; it0++, itrect0++)
       {
         Index2dC pix = itrect0.Data();
         if(pix.Row()==minr || pix.Row()==maxr || pix.Col()==minc || pix.Col()==maxc) {
           Vector2dC z = Distort(Vector2dC(pix.Col().V(), pix.Row().V()));
           rect.Involve(Index2dC(z.Y(),z.X()));
         }
       }
     } else {
        rect = img.Frame();
     }

     ImageC<PixelT> ret(rect);

     Array2dIterC<PixelT> it(ret);
     IndexRange2dIterC itrect(rect);
     for (; it; it++, itrect++)
     {
       Index2dC pix = itrect.Data();
       Vector2dC z = Undistort(Vector2dC(pix.Col().V(), pix.Row().V()));
       if (z.Y()>minr && z.Y()<maxr && z.X()>minc && z.X()<maxc)
       {
         sample(img, Vector2dC(z.Y(),z.X()), it.Data());
       } else {
         it.Data() = bg;
       }
     }

     return ret;
  }
  //: Derive distort an image using the 1st order radial distortion model filling empty pixels with bg value

};

#endif
