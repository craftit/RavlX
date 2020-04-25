// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006-14, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_VOXEL_VOXELSET_HEADER
#define RAVL_VOXEL_VOXELSET_HEADER 1
//! lib=RavlVoxels

// Templated base class for a voxel set

#include "Ravl/Vector3d.hh"
#include "Ravl/Matrix3d.hh"
#include "Ravl/BinStream.hh"

#include "Ravl/Voxels/VoxelSetIter.hh"

namespace RavlN { namespace VoxelsN {


   template <class DataTypeT> class VoxelSetC;
   typedef VoxelSetC<ByteT> ByteVoxelSetC;
   typedef VoxelSetIterC<ByteT> ByteVoxelSetIterC;

   //: Body for the voxel set base class
   template <class DataTypeT>
   class VoxelSetBodyC
      : public RCBodyC 
   {
   public:

      VoxelSetBodyC(const Matrix3dC& R, const Vector3dC& t, RealT voxelSize, DataTypeT threshold = 0) 
      : m_R(R), m_t(t), m_voxelSize(voxelSize), m_threshold(threshold)
      { 
         // Transformation from voxel to world coordinates
         m_RT = R.T() * voxelSize;
         TMul(R,t,m_tw); 
         m_tw *= -1.0;
      }
      //: Default constructor.

      virtual ~VoxelSetBodyC()
      { 
      }
      //: Destructor.

   public:
      // Copy and iterator

      virtual VoxelSetBodyC<DataTypeT>& Copy() const = 0;
      // Make a copy of body.
      // This should be provided in derived classes.

      virtual VoxelSetIterC<DataTypeT> Iterator() = 0;
      virtual VoxelSetIterC<DataTypeT> Iterator() const = 0;
      //: Return the iterator for the class

   public:
      // Access

      RealT VoxelSize() const
      {
         return m_voxelSize;
      }
      // Return the smallest voxel size used in the set

      const Matrix3dC& R() const 
      { 
         return m_R; 
      }
      // Return the orientation
      // R*x + t transforms a 3D point x from world co-ordinates to voxel co-ordinates

      const Vector3dC& t() const 
      { 
         return m_t;
      }
      // Return the position
      // R*x + t transforms a 3D point x from world co-ordinates to voxel co-ordinates

      Vector3dC VoxelIndexReal(const Vector3dC& xw) const
      {
         Vector3dC x = m_R*xw + m_t;
         return Vector3dC(x[0]/m_voxelSize, x[1]/m_voxelSize, x[2]/m_voxelSize);
      }
      // Return the voxel index for a world point

      Index3dC VoxelIndex(const Vector3dC& xw) const
      {
         Vector3dC x = m_R*xw + m_t;
         return Index3dC(IndexC(x[0]/m_voxelSize+0.5),IndexC(x[1]/m_voxelSize+0.5),IndexC(x[2]/m_voxelSize+0.5));
      }
      // Return the index for a world point

      Vector3dC WorldPosition(const Index3dC& vox) const
      {
         return m_RT*Vector3dC(vox.I(),vox.J(),vox.K()) + m_tw;
      }
      // Return the world position for a voxel index

      Vector3dC WorldPosition(const Vector3dC& vox) const
      {
         return m_RT*vox + m_tw;
      }
      // Return the world position for a sub-voxel index

      DataTypeT& OccupiedThreshold() 
      { 
         return m_threshold; 
      }
      // Return the threshold for an occupied voxel, defaults to 0
      // An internal point is evaluated as val > OccupiedThreshold
      // NOTE: OccupiedThreshold equates to empty

      const DataTypeT& OccupiedThreshold() const 
      { 
         return m_threshold; 
      }
      // Return the threshold for an occupied voxel, defaults to 0
      // An internal point is evaluated as val > OccupiedThreshold
      // NOTE: OccupiedThreshold equates to empty

      virtual DataTypeT GetVoxel(const Index3dC& x) const = 0;
      // Access a voxel, should be provided in a derived class
      // This should return empty (OccupiedThreshold) for voxels not contained in the voxel set

      virtual void SetVoxel(const Index3dC& x, DataTypeT val) = 0;
      // Set the value for a voxel, should be provided in a derived class

      virtual bool IsElm(const Index3dC& x) = 0;
      // Return whether the voxel is an element in the voxel set at the VoxelSize resolution

   protected:
      Matrix3dC m_R; // Rotation from world to voxel set
      Vector3dC m_t; // World origin in voxel coordinate frame
      Matrix3dC m_RT; // Rotation and scale from voxel index to world frame
      Vector3dC m_tw; // Voxel origin in world frame
      RealT m_voxelSize; // The size of the voxel resolution
      DataTypeT m_threshold; // The threshold for an internal region in the voxel set
   };

   //: Handle for the voxel set base class
   template <class DataTypeT>
   class VoxelSetC 
      : public RCHandleC<VoxelSetBodyC<DataTypeT> >
   {
   public: 

      VoxelSetC() 
      {}
      //: Default constructor.

      VoxelSetC(const VoxelSetC<DataTypeT> &oth) 
         : RCHandleC<VoxelSetBodyC<DataTypeT> >(oth)
      {}
      //: Copy constructor for the handle.

   protected:

      VoxelSetC(VoxelSetBodyC<DataTypeT> &bod) 
         : RCHandleC<VoxelSetBodyC<DataTypeT> >(bod)
      {}
      //: Body Constructor.

      VoxelSetBodyC<DataTypeT> &Body() 
      { return RCHandleC<VoxelSetBodyC<DataTypeT> >::Body(); }
      //: Access body.

      const VoxelSetBodyC<DataTypeT> &Body() const
      { return RCHandleC<VoxelSetBodyC<DataTypeT> >::Body(); }
      //: Access body.  

   public:

      VoxelSetC<DataTypeT> Copy()
      {
         return VoxelSetC<DataTypeT>(Body().Copy());
      }
      //: Return a copy

      VoxelSetIterC<DataTypeT> Iterator()
      {
         return Body().Iterator();
      }
      //: Return the iterator for the class

   public:

      const Matrix3dC& R() const 
      { 
         return Body().R();
      }
      // Return the orientation
      // R*x + t transforms a 3D point x from world co-ordinates to voxel co-ordinates

      const Vector3dC& t() const 
      { 
         return Body().t();
      }
      // Return the position
      // R*x + t transforms a 3D point x from world co-ordinates to voxel co-ordinates

      RealT VoxelSize() const
      {
         return Body().VoxelSize();
      }
      // Return the smallest voxel size used in the set

      DataTypeT& OccupiedThreshold() 
      { 
         return Body().OccupiedThreshold(); 
      }
      // Return the threshold for an occupied voxel, defaults to 0
      // An internal point is evaluated as val > OccupiedThreshold

      const DataTypeT& OccupiedThreshold() const 
      { 
         return Body().OccupiedThreshold(); 
      }
      // Return the threshold for an occupied voxel, defaults to 0
      // An internal point is evaluated as val > OccupiedThreshold

      Vector3dC VoxelIndexReal(const Vector3dC& xw) const
      {
         return Body().VoxelIndexReal(xw);
      }
      // Return the index for a world point as a real valued index

      Index3dC VoxelIndex(const Vector3dC& xw) const
      {
         return Body().VoxelIndex(xw);
      }
      // Return the index for a world point

      Vector3dC WorldPosition(const Index3dC& vox) const
      {
         return Body().WorldPosition(vox);
      }
      // Return the world position for a voxel index

      Vector3dC WorldPosition(const Vector3dC& vox) const
      {
         return Body().WorldPosition(vox);
      }
      // Return the world position for a sub-voxel index

      DataTypeT GetVoxel(const Index3dC& x) const
      {
         return Body().GetVoxel(x);
      }
      // Access a voxel, should be provided in a derived class
      // Returns empty for voxels not contained in the voxel set

      void SetVoxel(const Index3dC& x, DataTypeT val)
      {
         return Body().SetVoxel(x, val);
      }
      // Set the value for a voxel, should be provided in a derived class

      bool IsElm(const Index3dC& x)
      {
         return Body().IsElm(x);
      }
      // Returns whether the voxel is an element in the voxel set at the VoxelSize resolution
   };

}}

#endif
