// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006-14, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_VOXEL_VOXELOCTREE_HEADER
#define RAVL_VOXEL_VOXELOCTREE_HEADER 1
// Voxel Set defined on an octree structure
// Types are defined for a ByteVoxelOctreeC
//! lib=RavlVoxels

#include "Ravl/Tuple2.hh"
#include "Ravl/Stack.hh"
#include "Ravl/IndexRange3d.hh"

#include "Ravl/Voxels/VoxelSet.hh"

namespace RavlN { namespace VoxelsN {


   template <class DataTypeT> class VoxelOctreeC;
   template <class DataTypeT> class VoxelOctreeNodeC;
   typedef VoxelOctreeC<ByteT> ByteVoxelOctreeC;
   typedef VoxelSetIterC<ByteT> ByteVoxelOctreeIterC;
   typedef VoxelOctreeC<FloatT> FloatVoxelOctreeC;
   typedef VoxelSetIterC<FloatT> FloatVoxelOctreeIterC;

   // Octree node
   template <class DataTypeT>
   class VoxelOctreeNodeBodyC 
      : public RCBodyC
   { 
   public:
      VoxelOctreeNodeBodyC(IntT level, DataTypeT val, IndexRange3dC range)
         : m_level(level), m_val(val), m_range(range)
      { }
      //: Constructor

   public:
      void Expand(DataTypeT val);
      // Expand the node and set the child nodes to val

      void Collapse();
      // Collapse the node, deleting unreferenced children

		DataTypeT Get(Index3dC index, IntT level) const;
      // Return the value for a node with an index

      void Set(Index3dC index, IntT level, DataTypeT val);
      // Set the value at a node with the index

      bool Contains(Index3dC index, IntT level) const;
      // Determine whether a node exists for an index

      bool IsExpanded() const
      {
         return m_next[0].IsValid();
      }
      // Return whether the node is expanded

      VoxelOctreeNodeC<DataTypeT> Child(int i) const
      {
         return m_next[i];
      }
      // Return access to a child node

      IntT Level() const
      {
         return m_level;
      }
      // Return the level in the octree

      const DataTypeT& Value() const
      {
         return m_val;
      }
      // Return the value at the node

      DataTypeT& Value()
      {
         return m_val;
      }
      // Return the value at the node

      IndexRange3dC Range() const
      {
         return m_range;
      }
      // Return the indices covered by the node

   private:
      IntT m_level; // The level in the octree hierarchy for the node
      DataTypeT m_val; // The value at the node
      IndexRange3dC m_range; // Range of indices covered by the node
      VoxelOctreeNodeC<DataTypeT> m_next[8]; // Child nodes

   protected:
      template <class DataT>
      friend BinIStreamC &operator>>(BinIStreamC &is, VoxelOctreeNodeBodyC<DataT> &obj);
   };

   template <class DataTypeT>
   class VoxelOctreeNodeC 
      : public RCHandleC<VoxelOctreeNodeBodyC<DataTypeT> >
   {
   public:
      VoxelOctreeNodeC()
      {	}
      //: default constructor, creates an invalid handle

      VoxelOctreeNodeC(IntT level, DataTypeT val, IndexRange3dC range)
         : RCHandleC<VoxelOctreeNodeBodyC<DataTypeT> >(*new VoxelOctreeNodeBodyC<DataTypeT>(level, val, range))
      {	}
      //: Constructor

   protected:

      VoxelOctreeNodeC(VoxelOctreeNodeBodyC<DataTypeT> &bod) 
         : RCHandleC<VoxelOctreeNodeBodyC<DataTypeT> >(bod)
      {}
      //: Body Constructor.

      VoxelOctreeNodeBodyC<DataTypeT> &Body() 
      { return RCHandleC<VoxelOctreeNodeBodyC<DataTypeT> >::Body(); }
      //: Access body.

      const VoxelOctreeNodeBodyC<DataTypeT> &Body() const
      { return RCHandleC<VoxelOctreeNodeBodyC<DataTypeT> >::Body(); }
      //: Access body.  

   public:
      void Expand(DataTypeT val)
      {
         Body().Expand(val);
      }
      // Expand the node and set the child nodes to val

      void Collapse()
      {
         Body().Collapse();
      }
      // Collapse the node, deleting unreferenced children

		DataTypeT Get(Index3dC index, IntT level) const
      {
         return Body().Get(index, level);
      }
      // Return the value for a node with an index

      void Set(Index3dC index, IntT level, DataTypeT val)
      {
         Body().Set(index, level, val);
      }
      // Set the value at a node with the index

      bool Contains(Index3dC index, IntT level) const
      {
         return Body().Contains(index, level);
      }
      // Determine whether a node exists for an index

      bool IsExpanded() const
      {
         return Body().IsExpanded();
      }
      // Return whether the node is expanded

      VoxelOctreeNodeC<DataTypeT> Child(int i) const
      {
         return Body().Child(i);
      }
      // Return access to a child node

      IntT Level() const
      {
         return Body().Level();
      }
      // Return the level in the octree

      const DataTypeT& Value() const
      {
         return Body().Value();
      }
      // Return the value at the node

      DataTypeT& Value()
      {
         return Body().Value();
      }
      // Return the value at the node

      IndexRange3dC Range() const
      {
         return Body().Range();
      }
      // Return the indices covered by the node

   protected:
      template <class DataT>
      friend BinIStreamC &operator>>(BinIStreamC &is, VoxelOctreeC<DataT> &obj);
      template <class DataT>
      friend BinIStreamC &operator>>(BinIStreamC &is, VoxelOctreeNodeBodyC<DataT> &obj);
   };

   //: Body for the voxel Octree class
   template <class DataTypeT>
   class VoxelOctreeBodyC
      : public VoxelSetBodyC<DataTypeT>
   {
   public:

      VoxelOctreeBodyC(const Matrix3dC& R, const Vector3dC& t, RealT voxelSize, IntT numlevels, DataTypeT threshold = 0) 
	: VoxelSetBodyC<DataTypeT>(R, t, voxelSize, threshold), m_numlevels(numlevels), m_root(0,threshold,IndexRange3dC((IntT)Pow(2,numlevels),(IntT)Pow(2,numlevels),(IntT)Pow(2,numlevels)))
      { }
      //: Default constructor.

      virtual ~VoxelOctreeBodyC()
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

      virtual bool IsElm(const Index3dC& x);
      // Return whether the voxel is an element in the voxel set

      IntT Depth() const 
      {
         return m_numlevels;
      }
      // Return the number of levels in the hierarchy

      VoxelOctreeNodeC<DataTypeT> Root() const
      {
         return m_root;
      }
      // Return access to the root node of the octree

      void Clear()
      {
         m_root = VoxelOctreeNodeC<DataTypeT>(0,VoxelSetBodyC<DataTypeT>::m_threshold,m_root.Range());
      }
      // Clear the octree, deleting unreferenced children

   protected:
      IntT m_numlevels;
      VoxelOctreeNodeC<DataTypeT> m_root;
   };

   //: Handle for the voxel set class
   template <class DataTypeT>
   class VoxelOctreeC 
      : public VoxelSetC<DataTypeT>
   {
   public: 

      VoxelOctreeC() 
      {}
      //: Default constructor, creates an invalid handle

      VoxelOctreeC(const Matrix3dC& R, const Vector3dC& t, RealT voxelSize, IntT numlevels, DataTypeT threshold = 0) 
         : VoxelSetC<DataTypeT>(* new VoxelOctreeBodyC<DataTypeT>(R, t, voxelSize, numlevels, threshold))
      {}
      //: Constructor

      VoxelOctreeC(const Vector3dC& cog, RealT voxelSize, RealT volumeSize, DataTypeT threshold = 0)  
         : VoxelSetC<DataTypeT>(* new VoxelOctreeBodyC<DataTypeT>(
            Matrix3dC::I(), 
            Vector3dC(1,1,1)*(( Pow(2, Ceil(Log(volumeSize/voxelSize)/Log(2)))*voxelSize - voxelSize)/2.0) - cog,
            voxelSize, 
            (IntT)Ceil(Log(volumeSize/voxelSize)/Log(2)), 
            threshold))
      {}
      //: Construct given a centroid, voxel size and volume size

      VoxelOctreeC(const VoxelSetC<DataTypeT> &oth) 
         : VoxelSetC<DataTypeT>(oth)
      {
         if (dynamic_cast<VoxelOctreeBodyC<DataTypeT> *>(&VoxelSetC<DataTypeT>::Body()) == 0)
         {
            RCHandleC<VoxelSetBodyC<DataTypeT> >::Invalidate();
         }
      }
      //: Copy constructor for the handle.

   protected:

      VoxelOctreeC(VoxelOctreeBodyC<DataTypeT> &bod)
         : VoxelSetC<DataTypeT>(bod)
      {}
      //: Body constructor

      VoxelOctreeBodyC<DataTypeT> &Body() 
      { return static_cast<VoxelOctreeBodyC<DataTypeT> &>(VoxelSetC<DataTypeT>::Body()); }
      //: Access body.

      const VoxelOctreeBodyC<DataTypeT> &Body() const
      { return static_cast<const VoxelOctreeBodyC<DataTypeT> &>(VoxelSetC<DataTypeT>::Body()); }
      //: Access body.

   public:

      IntT Depth() const 
      {
         return Body().Depth();
      }
      // Return the number of levels in the hierarchy

      VoxelOctreeNodeC<DataTypeT> Root() const
      {
         return Body().Root();
      }
      // Return access to the root node of the octree

      void Clear()
      {
         Body().Clear();
      }
      // Clear the octree, deleting unreferenced children
   };

   // Voxel Octree iterator
   // Performs a depth-first iteration over the octree nodes at the deepest level
   template <class DataTypeT>
   class VoxelOctreeIterBodyC 
      : public VoxelSetIterBodyC<DataTypeT>
   {
   public: 
      VoxelOctreeIterBodyC(const VoxelOctreeBodyC<DataTypeT>& voxels)
         : m_numlevels(voxels.Depth()), m_root(voxels.Root())
      {
         First();
      }
      // Constructor

   public:
      virtual void First()
      {
         m_stack.Empty();
         m_stack.Push( Tuple2C<IntT, VoxelOctreeNodeC<DataTypeT> >(0, m_root) );
         Next();
      }
      //: Goto the first voxel

      virtual void Next();
      //: Move to the next node at the deepest level in the octree

      virtual bool IsElm()
      {
         return !m_stack.IsEmpty();
      }
      //: At a valid voxel

      virtual Index3dC Index()
      {
         return m_stack.Top().Data2().Range().Origin();
      }
      //: Return the index for the current voxel

      virtual DataTypeT& Data()
      {
         return m_stack.Top().Data2().Value();
      }
      //: Access the current voxel data.

   protected:
      IntT m_numlevels;
      VoxelOctreeNodeC<DataTypeT> m_root;
      StackC< Tuple2C<IntT, VoxelOctreeNodeC<DataTypeT> > > m_stack;
   };

   ////////////////////////////////////////////////////////////////////////////////

   // Expand the node and set the child nodes to val
   template <class DataTypeT>
   void
   VoxelOctreeNodeBodyC<DataTypeT>::Expand(DataTypeT val)
   {
      for (IntT i=0; i<8; i++)
      {
         if (!m_next[i].IsValid())
         {
            // Determine the index range covered by the node
            Index3dC org = m_range.Origin();
            IntT size = m_range.IRange().Size() / 2;
            if (i&1) 
            {
               org.I() += size;
            }
            if (i&2) 
            {
               org.J() += size;
            }
            if (i&4) 
            {
               org.K() += size;
            }
            m_next[i] = VoxelOctreeNodeC<DataTypeT>(m_level+1, val, IndexRange3dC(org.I(),org.I()+size-1,org.J(),org.J()+size-1,org.K(),org.K()+size-1));
         }
         else
         {
            m_next[i].Value() = val;
         }
      }
   }

   // Collapse the node deleting all children
   template <class DataTypeT>
   void
   VoxelOctreeNodeBodyC<DataTypeT>::Collapse()
   {
      for (IntT i=0; i<8; i++)
      {
         m_next[i] = VoxelOctreeNodeC<DataTypeT>();
      }
   }

   // Return the value for a node with an index
   template <class DataTypeT>
   DataTypeT
   VoxelOctreeNodeBodyC<DataTypeT>::Get(Index3dC index, IntT level) const
   {
      if (level<1)
      {
         return m_val;
      }
      level--;
      IntT half = 1<<level;
      IntT i = 0;
      if (index.I() >= half)
      {
         i = 1;
         index.I() -= half;
      }
      if (index.J() >= half)
      {
         i |= 2;
         index.J() -= half;
      }
      if (index.K() >= half)
      {
         i |= 4;
         index.K() -= half;
      }
      if (m_next[i].IsValid())
      {
         return m_next[i].Get(index, level);
      }
      return m_val;
   }

   // Set the value at a node with the index
   template <class DataTypeT>
   void
   VoxelOctreeNodeBodyC<DataTypeT>::Set(Index3dC index, IntT level, DataTypeT val)
   {
      if (level<1)
      {
         m_val = val;
         return;
      }
      level--;
      IntT half = 1<<level;
      IntT i = 0;
      if (index.I() >= half)
      {
         i = 1;
         index.I() -= half;
      }
      if (index.J() >= half)
      {
         i |= 2;
         index.J() -= half;
      }
      if (index.K() >= half)
      {
         i |= 4;
         index.K() -= half;
      }
      VoxelOctreeNodeC<DataTypeT> next = m_next[i];
      if (!next.IsValid())
      {
         Expand(m_val);
         next = m_next[i];
      }
      next.Set(index, level, val);
   }

   // Return the value for a node with an index
   template <class DataTypeT>
   bool
   VoxelOctreeNodeBodyC<DataTypeT>::Contains(Index3dC index, IntT level) const
   {
      if (level<1)
      {
         return true;
      }
      level--;
      IntT half = 1<<level;
      IntT i = 0;
      if (index.I() >= half)
      {
         i = 1;
         index.I() -= half;
      }
      if (index.J() >= half)
      {
         i |= 2;
         index.J() -= half;
      }
      if (index.K() >= half)
      {
         i |= 4;
         index.K() -= half;
      }
      if (m_next[i].IsValid())
      {
         return m_next[i].Contains(index, level);
      }
      return false;
   }

   // Save node to a binary stream
   template <class DataTypeT>
   BinOStreamC &operator<<(BinOStreamC &os, const VoxelOctreeNodeC<DataTypeT> &obj) 
   {
      os << obj.Level();
      os << obj.Value();
      os << obj.Range();
      if (!obj.IsExpanded())
      {
         os << (ByteT)0;
      }
      else
      {
         os << (ByteT)1;
         for (IntT i = 0; i<8; i++)
         {
            os << obj.Child(i);
         }
      }
      return os;
   }

   // Read node from  a binary stream
   template <class DataTypeT>
   BinIStreamC &operator>>(BinIStreamC &is, VoxelOctreeNodeBodyC<DataTypeT> &obj) 
   {
      ByteT expanded;
      is >> obj.m_level;
      is >> obj.m_val;
      is >> obj.m_range;
      is >> expanded;
      if (expanded)
      {
         obj.Expand(obj.m_val);
         for (IntT i = 0; i<8; i++)
         {
            is >> obj.Child(i).Body();
         }
      }
      return is;
   }

   ////////////////////////////////////////////////////////////////////////////////

   // Create a deep copy
   template <class DataTypeT>
   VoxelSetBodyC<DataTypeT>&
   VoxelOctreeBodyC<DataTypeT>::Copy() const
   {
      VoxelOctreeBodyC<DataTypeT>& bod = *new VoxelOctreeBodyC<DataTypeT>(VoxelSetBodyC<DataTypeT>::m_R, VoxelSetBodyC<DataTypeT>::m_t, VoxelSetBodyC<DataTypeT>::m_voxelSize, m_numlevels, VoxelSetBodyC<DataTypeT>::m_threshold);
      VoxelSetIterC<DataTypeT> it = this->Iterator();
      for (; it; it++)
      {
         bod.SetVoxel(it.Index(), it.Data());
      }

      return static_cast<VoxelSetBodyC<DataTypeT> &>(bod);
   }

   // Access a voxel
   template <class DataTypeT>
   DataTypeT
   VoxelOctreeBodyC<DataTypeT>::GetVoxel(const Index3dC& x) const
   {
      return m_root.Get(x,m_numlevels);
   }

   // Set the value for a voxel
   template <class DataTypeT>
   void
   VoxelOctreeBodyC<DataTypeT>::SetVoxel(const Index3dC& x, DataTypeT val)
   {
      return m_root.Set(x,m_numlevels,val);
   }

   // Return whether the voxel is an element in the voxel set
   template <class DataTypeT>
   bool
   VoxelOctreeBodyC<DataTypeT>::IsElm(const Index3dC& x)
   {
      return m_root.Contains(x,m_numlevels);
   }

   // Load from a binary stream
   template <class DataTypeT>
   BinIStreamC &operator>>(BinIStreamC &is, VoxelOctreeC<DataTypeT> &obj) 
   {
      // Read grid setup
      Matrix3dC R;
      Vector3dC t;
      RealT voxelSize;
      IntT numlevels;
      DataTypeT threshold;
      is >> R;
      is >> t;
      is >> voxelSize;
      is >> numlevels;
      is >> threshold;

      obj = VoxelOctreeC<DataTypeT>(R, t, voxelSize, numlevels, threshold);
      is >> obj.Root().Body();

      return is;
   }

   // Save to a binary stream
   template <class DataTypeT>
   BinOStreamC &operator<<(BinOStreamC &os, VoxelOctreeC<DataTypeT> &obj) 
   {
      if (obj.IsValid())
      {
         // Write voxel setup
         os << obj.R();
         os << obj.t();
         os << obj.VoxelSize();
         os << obj.Depth();
         os << obj.OccupiedThreshold();
         // Write out the octree nodes
         os << obj.Root();
      }
      return os;
   }

   // Return the iterator for the class
   template <class DataTypeT>
   VoxelSetIterC<DataTypeT>
   VoxelOctreeBodyC<DataTypeT>::Iterator()
   {
      VoxelSetIterC<DataTypeT> it(*new VoxelOctreeIterBodyC<DataTypeT>(*this));
      return it;
   }

   template <class DataTypeT>
   VoxelSetIterC<DataTypeT>
   VoxelOctreeBodyC<DataTypeT>::Iterator() const
   {
      VoxelSetIterC<DataTypeT> it(*new VoxelOctreeIterBodyC<DataTypeT>(*this));
      return it;
   }

   ////////////////////////////////////////////////////////////////////////////////

   // Move to the next node at the deepest level in the octree
   template <class DataTypeT>
   void
   VoxelOctreeIterBodyC<DataTypeT>::Next()
   {
      if (!m_stack.IsEmpty())
      {
         // Expand the depth first stack if necessary
         if (m_stack.Top().Data2().IsExpanded())
         {
            // Create a depth first stack
            while (m_stack.Top().Data2().IsExpanded())
            {
               m_stack.Push( Tuple2C<IntT, VoxelOctreeNodeC<DataTypeT> >(0, m_stack.Top().Data2().Child(0)) );
            }
            if (m_stack.Top().Data2().Level() == m_numlevels)
            {
               return;
            }
         }
         // Move up the hierarchy to the point where a sibling node can be found
         Tuple2C<IntT, VoxelOctreeNodeC<DataTypeT> > prev;
         do 
         {
            prev = m_stack.Pop();
         } while (!m_stack.IsEmpty() && prev.Data1()==7);
         if (!m_stack.IsEmpty())
         {
            // Add the sibling to the stack
            IntT child = prev.Data1()+1;
            m_stack.Push( Tuple2C<IntT, VoxelOctreeNodeC<DataTypeT> >(child , m_stack.Top().Data2().Child(child) ));
            // Move on if this is not at the correct level
            if (m_stack.Top().Data2().Level() != m_numlevels)
            {
               Next();
            }
         }
      }
   }

}}
#endif
