// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006-14, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_VOXEL_VOXELGRID_HEADER
#define RAVL_VOXEL_VOXELGRID_HEADER 1
//! lib=RavlVoxels

// Voxel Set defined on a regular volume grid
// Types are defined for a ByteVoxelGridC and FloatVoxelGridC

#include "Ravl/Array3d.hh"
#include "Ravl/Array3dIter.hh"
#include "Ravl/Array3dIter2.hh"
#include "Ravl/IndexRange3dIter.hh"

#include "Ravl/Voxels/VoxelSet.hh"
#include "Ravl/Voxels/VoxelOctree.hh"

namespace RavlN { namespace VoxelsN {
   using namespace RavlN;

   template <class DataTypeT> class VoxelGridC;
   typedef VoxelGridC<ByteT> ByteVoxelGridC;
   typedef VoxelSetIterC<ByteT> ByteVoxelGridIterC;
   typedef VoxelGridC<FloatT> FloatVoxelGridC;
   typedef VoxelSetIterC<FloatT> FloatVoxelGridIterC;

   //: Body for the voxel grid class
   template <class DataTypeT>
   class VoxelGridBodyC
      : public VoxelSetBodyC<DataTypeT>
   {
   public:

      VoxelGridBodyC(const Matrix3dC& R, const Vector3dC& t, RealT voxelSize, IntT numSideVoxels, DataTypeT threshold = 0) 
      : VoxelSetBodyC<DataTypeT>(R, t, voxelSize, threshold), m_array(numSideVoxels,numSideVoxels,numSideVoxels)
      { }
      //: Default constructor.

      VoxelGridBodyC(const Matrix3dC& R, const Vector3dC& t, RealT voxelSize, IntT numSideVoxels, DataTypeT threshold, const Array3dC<DataTypeT>& array) 
      : VoxelSetBodyC<DataTypeT>(R, t, voxelSize, threshold), m_array(array)
      { }
      //: Load constructor.

      virtual ~VoxelGridBodyC()
      { }
      //: Destructor.

   public:
      // Copy and load operators

      virtual VoxelSetBodyC<DataTypeT>& Copy() const;
      // Make a copy of body.

      virtual VoxelSetIterC<DataTypeT> Iterator();
      virtual VoxelSetIterC<DataTypeT> Iterator() const;
      //: Return the iterator for the class

   public:
      // Access

      virtual DataTypeT GetVoxel(const Index3dC& x) const;
      // Access a voxel, returns false if the voxel is not a true element

      virtual void SetVoxel(const Index3dC& x, DataTypeT val);
      // Set the value for a voxel

      virtual bool IsElm(const Index3dC& x)
      {
         return m_array.Range().Contains(x);
      }
      // Return whether the voxel is an element in the voxel set

      const Array3dC<DataTypeT>& Array() const 
      { 
         return m_array; 
      }
      // Access the array

      Array3dC<DataTypeT>& Array() 
      { 
         return m_array; 
      }
      // Access the array

   protected:
      Array3dC<DataTypeT> m_array;
   };

   //: Handle for the voxel set class
   template <class DataTypeT>
   class VoxelGridC 
      : public VoxelSetC<DataTypeT>
   {
   public: 

      VoxelGridC() 
      {}
      //: Default constructor, creates an invalid handle

      VoxelGridC(const Matrix3dC& R, const Vector3dC& t, RealT voxelSize, IntT numSideVoxels, DataTypeT threshold = 0) 
         : VoxelSetC<DataTypeT>(* new VoxelGridBodyC<DataTypeT>(R, t, voxelSize, numSideVoxels, threshold))
      {}
      //: Constructor

      VoxelGridC(const Vector3dC& cog, RealT voxelSize, RealT volumeSize, DataTypeT threshold = 0)  
         : VoxelSetC<DataTypeT>(* new VoxelGridBodyC<DataTypeT>(
            Matrix3dC::I(), 
            Vector3dC(1,1,1)*((volumeSize-voxelSize)/2.0) - cog,
            voxelSize, 
            (IntT)(volumeSize/voxelSize + 0.5), 
            threshold))
      {}
      //: Construct given a centroid, volume size and volume size

      VoxelGridC(const VoxelOctreeC<DataTypeT>& oth);
      //: Construct from an octree

      VoxelGridC(const VoxelSetC<DataTypeT> &oth) 
         : VoxelSetC<DataTypeT>(oth)
      {
         if (dynamic_cast<VoxelGridBodyC<DataTypeT> *>(&VoxelSetC<DataTypeT>::Body()) == 0)
         {
            RCHandleC<VoxelSetBodyC<DataTypeT> >::Invalidate();
         }
      }
      //: Copy constructor for the handle.

   protected:

      VoxelGridC(VoxelGridBodyC<DataTypeT> &bod)
         : VoxelSetC<DataTypeT>(bod)
      {}
      //: Body constructor

      VoxelGridBodyC<DataTypeT> &Body() 
      { return static_cast<VoxelGridBodyC<DataTypeT> &>(VoxelSetC<DataTypeT>::Body()); }
      //: Access body.

      const VoxelGridBodyC<DataTypeT> &Body() const
      { return static_cast<const VoxelGridBodyC<DataTypeT> &>(VoxelSetC<DataTypeT>::Body()); }
      //: Access body.

      template <class DataT>
      friend BinIStreamC &operator>>(BinIStreamC &is, VoxelGridC<DataT> &obj);
      // IO

   public:

      const Array3dC<DataTypeT>& Array() const 
      { 
         return Body().Array(); 
      }
      // Access the array

      Array3dC<DataTypeT>& Array() 
      { 
         return Body().Array(); 
      }
      // Access the array
   };

   // Voxel grid iterator
   template <class DataTypeT>
   class VoxelGridIterBodyC 
      : public VoxelSetIterBodyC<DataTypeT>
   {
   public: 
      VoxelGridIterBodyC(const VoxelGridBodyC<DataTypeT>& voxels)
      { 
         itr = IndexRange3dIterC(voxels.Array().Range());
         itd = Array3dIterC<DataTypeT>(voxels.Array());
      }
      // Constructor

   public:
      virtual void First()
      {
         itr.First();
         itd.First();
      }
      //: Goto the first voxel

      virtual void Next()
      {
         itr++;
         itd++;
      }
      //: Goto the next voxel.

      virtual bool IsElm()
      {
         return itr.IsElm();
      }
      //: At a valid voxel

      virtual Index3dC Index()
      {
         return itr.Data();
      }
      //: Return the index for the current voxel

      virtual DataTypeT& Data()
      {
         return itd.Data();
      }
      //: Access the current voxel data.

   protected:
      IndexRange3dIterC itr;
      Array3dIterC<DataTypeT> itd;
   };

   ////////////////////////////////////////////////////////////////////////////////

   // Create a deep copy
   template <class DataTypeT>
   VoxelSetBodyC<DataTypeT>&
   VoxelGridBodyC<DataTypeT>::Copy() const
   {
      VoxelGridBodyC<DataTypeT>& bod = *new VoxelGridBodyC<DataTypeT>(VoxelSetBodyC<DataTypeT>::m_R, VoxelSetBodyC<DataTypeT>::m_t, VoxelSetBodyC<DataTypeT>::m_voxelSize, m_array.Range1().Size(), VoxelSetBodyC<DataTypeT>::m_threshold);
      Array3dIter2C<DataTypeT,DataTypeT> it(bod.Array(),m_array);
      for (; it; it++)
      {
         it.Data1() = it.Data2();
      }
      return static_cast<VoxelSetBodyC<DataTypeT> &>(bod);
   }

   // Access a voxel
   template <class DataTypeT>
   DataTypeT
   VoxelGridBodyC<DataTypeT>::GetVoxel(const Index3dC& x) const
   {
      if (m_array.Frame().Contains(x))
      {
         return m_array[x];
      }
      return VoxelSetBodyC<DataTypeT>::m_threshold;
   }

   // Set the value for a voxel
   template <class DataTypeT>
   void
   VoxelGridBodyC<DataTypeT>::SetVoxel(const Index3dC& x, DataTypeT val)
   {
      if (m_array.Frame().Contains(x))
      {
         m_array[x] = val;
      }
   }

   // Load from a binary stream
   template <class DataTypeT>
   BinIStreamC &operator>>(BinIStreamC &is, VoxelGridC<DataTypeT> &obj) 
   {
      // Read grid setup
      Matrix3dC R;
      Vector3dC t;
      RealT voxelSize;
      IntT numSideVoxels;
      DataTypeT threshold;
      is >> R;
      is >> t;
      is >> voxelSize;
      is >> numSideVoxels;
      is >> threshold;

      // Read RLE data
      Array3dC<DataTypeT> array(numSideVoxels,numSideVoxels,numSideVoxels);
      Array3dIterC<DataTypeT> it(array);
      while (it)
      {
         DataTypeT val;
         IntT i, count;
         is >> val;
         is >> count;
         for (i=0; i<count && it; i++, it++)
         {
            it.Data() = val;
         }
      }

      obj = VoxelGridC<DataTypeT>(*new VoxelGridBodyC<DataTypeT>(R,t,voxelSize,numSideVoxels,threshold,array));
      return is;
   }

   // Save to a binary stream
   template <class DataTypeT>
   BinOStreamC &operator<<(BinOStreamC &os, VoxelGridC<DataTypeT> &obj) 
   {
      if (obj.IsValid())
      {
         // Write grid setup
         os << obj.R();
         os << obj.t();
         os << obj.VoxelSize();
         os << obj.Array().Range1().Size();
         os << obj.OccupiedThreshold();
         // Write RLE data
         Array3dIterC<DataTypeT> it(obj.Array());
         DataTypeT val = it.Data();
         IntT count = 1;
         for (it++; it; it++)
         {
            const DataTypeT& newval = it.Data();
            if (val!=newval)
            {
               os << val;
               os << count;
               count = 0;
               val = newval;
            }
            count++;
         }
         os << val;
         os << count;
      }

      return os;
   }

   //: Return the iterator for the class
   template <class DataTypeT>
   VoxelSetIterC<DataTypeT>
   VoxelGridBodyC<DataTypeT>::Iterator()
   {
      VoxelSetIterC<DataTypeT> it(*new VoxelGridIterBodyC<DataTypeT>(*this));
      return it;
   }

   template <class DataTypeT>
   VoxelSetIterC<DataTypeT>
   VoxelGridBodyC<DataTypeT>::Iterator() const
   {
      VoxelSetIterC<DataTypeT> it(*new VoxelGridIterBodyC<DataTypeT>(*this));
      return it;
   }

   // Construct from an octree
   template <class DataTypeT>
   VoxelGridC<DataTypeT>::VoxelGridC(const VoxelOctreeC<DataTypeT>& oth)
         : VoxelSetC<DataTypeT>(* new VoxelGridBodyC<DataTypeT>(oth.R(), oth.t(), oth.VoxelSize(), oth.Root().Range().Range1().Size(), oth.OccupiedThreshold()))
   {
      VoxelSetIterC<DataTypeT> it = this->Iterator();
      for (; it; it++)
      {
         it.Data() = oth.GetVoxel(it.Index());
      }
   }

}}

#endif
