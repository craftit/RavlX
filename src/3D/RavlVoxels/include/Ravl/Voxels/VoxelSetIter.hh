// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006-14, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_VOXEL_VOXELSETITER_HEADER
#define RAVL_VOXEL_VOXELSETITER_HEADER 1
//! lib=RavlVoxels

// Base class for a voxel set iterator
#include "Ravl/RCBodyV.hh"
#include "Ravl/Index3d.hh"

namespace RavlN { namespace VoxelsN {

   // Abstract interface for a voxel set iterator
   template <class DataTypeT>
   class VoxelSetIterBodyC
      : public RCBodyVC 
   {
   public:
      virtual void First() = 0;
      //: Goto the first voxel
      // Implement in derived class

      virtual void Next() = 0;
      //: Goto the next voxel.
      // Implement in derived class

      virtual bool IsElm() = 0;
      //: At a valid voxel
      // Implement in derived class

      virtual Index3dC Index() = 0;
      //: Return the index for the current voxel

      virtual DataTypeT& Data() = 0;
      //: Access the current voxel data.
   };

   //: Handle for the voxel set iterator
   template <class DataTypeT>
   class VoxelSetIterC 
      : public RCHandleC<VoxelSetIterBodyC<DataTypeT> >
   {
   public: 
      VoxelSetIterC() 
      {}
      //: Default constructor, creates an invalid handle

      VoxelSetIterC(VoxelSetIterBodyC<DataTypeT> &bod) 
         : RCHandleC<VoxelSetIterBodyC<DataTypeT> >(bod)
      {}
      //: Body Constructor.

   protected:

      VoxelSetIterBodyC<DataTypeT> &Body() 
      { return RCHandleC<VoxelSetIterBodyC<DataTypeT> >::Body(); }
      //: Access body.

      const VoxelSetIterBodyC<DataTypeT> &Body() const
      { return RCHandleC<VoxelSetIterBodyC<DataTypeT> >::Body(); }
      //: Access body.  

   public:
      void First()
      {
         return Body().First();
      }
      //: Goto the first voxel

      void Next()
      {
         return Body().Next();
      }
      //: Goto the next voxel.

      bool IsElm()
      {
         return Body().IsElm();
      }
      //: At a valid voxel

      Index3dC Index()
      {
         return Body().Index();
      }
      //: Return the index for the current voxel

      DataTypeT& Data()
      {
         return Body().Data();
      }
      //: Access the current voxel data.

      DataTypeT& operator*()
      { 
         return Body().Data(); 
      }
      //: Access the current voxel.

      DataTypeT* operator->()
      { 
         return &Body().Data(); 
      }
      //: Access the current voxel.

      void operator++( int )
      { 
         Body().Next(); 
      }
      //: Goto next index.
    
      operator bool()
      { 
         return Body().IsElm(); 
      }
      //: At a valid voxel
   };
}}

#endif
