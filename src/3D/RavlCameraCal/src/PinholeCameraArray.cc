// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#include "Ravl/Matrix.hh"
#include "Ravl/Vector.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/3D/PinholeCamera0.hh"
#include "Ravl/3D/PinholeCamera1.hh"
#include "Ravl/3D/PinholeCamera2.hh"
#include "Ravl/3D/PinholeCamera3.hh"
#include "Ravl/3D/PinholeCameraArray.hh"
//! lib=RavlCameraCal

namespace Ravl3DN
{
  using namespace RavlN;

  PinholeCameraArrayC::PinholeCameraArrayC(UIntT size, UIntT dist)
    : m_distortion(dist), m_cameras(size)
  {
    SArray1dIterC<PinholeCameraC> it(m_cameras);
    // Initialise the correct camera model in the array
    for (; it; it++)
    { 
      switch (dist)
      {
      case 3:
        it.Data() = PinholeCamera3C();
        break;
      case 2:
        it.Data() = PinholeCamera2C();
        break;
      case 1:
        it.Data() = PinholeCamera1C();
        break;
      default:
        it.Data() = PinholeCamera0C();
        break;
      }
    }
  }

  // Triangulate points in a set of camera images
  bool 
  PinholeCameraArrayC::Triangulate(Vector3dC& pt3D, const DListC<Tuple2C<UIntT, Vector2dC> >& points) const
  {
    IntT iNum = points.Size();
    if (iNum<2)
    {
      return false;
    }

    // Triangulate the first 2 points
    Tuple2C<UIntT, Vector2dC> imgPt1 = points.Nth(0);
    Tuple2C<UIntT, Vector2dC> imgPt2 = points.Nth(1);
    Vector3dC orig1, orig2;
    m_cameras[imgPt1.Data1()].Origin(orig1);
    m_cameras[imgPt2.Data1()].Origin(orig2);
    Vector3dC dir1,dir2;
    m_cameras[imgPt1.Data1()].ProjectInverseDirection(dir1,imgPt1.Data2());
    m_cameras[imgPt2.Data1()].ProjectInverseDirection(dir2,imgPt2.Data2());
    dir1.MakeUnit(); dir2.MakeUnit();
    double dN1N2 = dir1.Dot(dir2);
    // Check not singular
    if (Abs(dN1N2) < 1e-12) 
    { 
       return false;
    }
    Vector3dC t = orig2 - orig1;
    double d1 = t.Dot(dir1-dir2*dN1N2)/(1-dN1N2*dN1N2);
    double d2 = t.Dot(dir1*dN1N2-dir2)/(1-dN1N2*dN1N2);
    pt3D = (orig1+dir1*d1+orig2+dir2*d2)*0.5;
    if (iNum == 2)
    {
       return true;
    }

    // Now minimise the image plane error to all points
    MatrixC J(2*iNum,3);
    VectorC Z(2*iNum);
    IntT it, iCam;
    for (it=0; it<10; it++) 
    {
       // Construct residual vector and jacobian matrix
       for (iCam=0; iCam<iNum; iCam++) 
       {
          Tuple2C<UIntT, Vector2dC> imgPt1 = points.Nth(iCam);
          // Get the jacobian
          FMatrixC<2,3> Jz;
          m_cameras[imgPt1.Data1()].ProjectJacobian(Jz, pt3D);
          IntT i,j;
          for (i=0; i<2; i++) for (j=0; j<3; j++) J[2*iCam+i][j] = Jz[i][j];
          // Get the residual
          Vector2dC imgPt;
          m_cameras[imgPt1.Data1()].Project(imgPt, pt3D);
          Z[2*iCam] = imgPt.X() - imgPt1.Data2().X();
          Z[2*iCam+1] = imgPt.Y() - imgPt1.Data2().Y();
       }
       // Find the update to solve the least squares problem J*d=Z
       MatrixC JTJ = J.T()*J;
       VectorC JTZ = J.T()*Z;
       JTJ.InverseIP();
       VectorC d = JTJ*JTZ;
       pt3D -= Vector3dC(d[0],d[1],d[2]);
    }
    
    return true;    
  }

  istream& operator>>(istream& s, PinholeCameraArrayC& cameras)
  {
    UIntT count, dist;
    s >> count >> dist;
    cameras = PinholeCameraArrayC(count, dist);
    SArray1dIterC<PinholeCameraC> it(cameras.Array());
    for (; it; it++)
    {
      s >> it.Data();
    }
    return s;
  }

  ostream& operator<<(ostream& s, const PinholeCameraArrayC& camera)
  {
    s << camera.Array().Size() << " " << camera.DistortionModel() << endl;
    SArray1dIterC<PinholeCameraC> it(camera.Array());
    for (; it; it++)
    {
      s << it.Data();
    }
    return s;
  }

  BinIStreamC& operator>>(BinIStreamC& s, PinholeCameraArrayC& cameras)
  {
    UIntT count, dist;
    s >> count >> dist;
    cameras = PinholeCameraArrayC(count, dist);
    SArray1dIterC<PinholeCameraC> it(cameras.Array());
    for (; it; it++)
    {
      s >> it.Data();
    }
    return s;
  }

  BinOStreamC& operator<<(BinOStreamC& s, const PinholeCameraArrayC& camera)
  {
    s << camera.Array().Size() << camera.DistortionModel();
    SArray1dIterC<PinholeCameraC> it(camera.Array());
    for (; it; it++)
    {
      s << it.Data();
    }
    return s;
  }
};
