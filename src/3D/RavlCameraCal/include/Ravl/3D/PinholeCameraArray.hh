// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef _CAMERA_CAL_PINHOLE_CAMERA_ARRAY_HH_
#define _CAMERA_CAL_PINHOLE_CAMERA_ARRAY_HH_
//! docentry="Ravl.API.3D.Camera Modelling"
//! lib=RavlCameraCal

#include "Ravl/DList.hh"
#include "Ravl/Tuple2.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/Stream.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/3D/PinholeCamera.hh"

namespace Ravl3DN
{
  using namespace RavlN;

  //! userlevel=Normal
  //: Container for an array of pinhole cameras
  // This wrapper provides IO to construct the appropriate camera models and return access via PinholeCameraC

  class PinholeCameraArrayC 
  {
  public: 
  
    PinholeCameraArrayC()
    { }
    // default constructor

    PinholeCameraArrayC(UIntT size, UIntT distortion);
    // Constructor
  
  public:

    SArray1dC<PinholeCameraC>& Array()
    {
      return m_cameras;
    }    
    //: Access the array of cameras

    const SArray1dC<PinholeCameraC>& Array() const
    {
      return m_cameras;
    }    
    //: Access the array of cameras

    SizeT Size() const
    {
       return m_cameras.Size();
    }
    //: Return the number of cameras in the array

    const PinholeCameraC& operator[](IntT i) const
    {
       return m_cameras[i];
    }
    // Read-only access to the 'i'-th element of the array.     
    
    inline PinholeCameraC& operator[](IntT i)
    {
       return m_cameras[i];
    }
    // Read-write access  to the 'i'-th element of the array. 

    UIntT DistortionModel() const
    {
      return m_distortion;
    }
    //: Return the distortion model used
    // i.e 0 = PinholeCamera0C, 1 = PinholeCamera1C etc.
    // If you need to access the camera parameters you can construct a handle from the base handle as follows
    // PinholeCamera1C c(dynamic_cast<PinholeCameraBody1C&>(camera.Body()));

    bool Triangulate(Vector3dC& pt3D, const DListC<Tuple2C<UIntT, Vector2dC> >& points) const;
    //: Triangulate points in a set of camera images
    // Returns false if <2 points or singular

  protected:
  
    UIntT m_distortion;
    SArray1dC<PinholeCameraC> m_cameras; // The reference counted container
    
  }; 

  istream& operator>>(istream& s, PinholeCameraArrayC& camera);
  //:Read camera parameters from a text stream.

  ostream& operator<<(ostream& s, const PinholeCameraArrayC& camera);
  //:Write camera parameters to a text stream.

  BinIStreamC& operator>>(BinIStreamC& s, PinholeCameraArrayC& camera);
  //:Read camera parameters from a binary stream.

  BinOStreamC& operator<<(BinOStreamC& s, const PinholeCameraArrayC& camera);
  //:Write camera parameters to a binary stream.
};

#endif

