// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006-14, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
// Ravl includes
#include "Ravl/DList.hh"
#include "Ravl/Array3dIter.hh"
#include "Ravl/IndexRange3dIter.hh"
//! lib=RavlVisualHull

// MVTGeom includes
#include "Ravl/VisualHull/VoxelClean.hh"
#include "Ravl/Voxels/VoxelSkeleton.hh"

namespace RavlN { namespace VisualHullN {
  using VoxelsN::VoxelSkeletonC;

   // CONSTRUCTION/DESTRUCTION /////////////////////////////////////////////

   //: Construct from a voxel set
   VoxelCleanC::VoxelCleanC(const ByteVoxelGridC& voxels)
      :  m_voxels(voxels)
   {
      voxframe = m_voxels.Array().Frame();
      n = voxframe.Range1().Size();
      m = voxframe.Range2().Size(); 
      l = voxframe.Range3().Size();
      n_minus_1 = n - 1;
      m_minus_1 = m - 1;
      l_minus_1 = l - 1;
      m_l = m*l;
   }

   // CONNECTED COMPONENTS /////////////////////////////////////////////////

   // File scope helper classes
   namespace
   {
      // Line of voxels in a voxel set
      class VoxelLineC
      {
      public:
         VoxelLineC() : m_valid(false)
         { }

         VoxelLineC(const Index3dC &voxel) : m_valid(true), m_line(voxel,0)
         { }

         inline const VoxelLineC& operator+=(const Index3dC &voxel)
         { 
            if (!m_valid)
            {
               m_line = IndexRange3dC(voxel,0);
               m_valid = true;
            }
            else
            {
               m_line.Involve(voxel);
            }
            return *this;
         }

         inline bool IsAdjacent(const Index3dC &voxel) const
         {
            if (!m_valid) return false;
            return ( m_line.Expand(1).Contains(voxel) );
         }

         inline bool IsAdjacent(const VoxelLineC &oth) const
         {
            if (!m_valid) return false;
            return ( m_line.Expand(1).IsOverlapping(oth.m_line) );
         }

         inline UIntT Size() const
         {
            if (!m_valid) return 0;
            Index3dC s=m_line.Size();
            return (s.I()>s.J()) ? ( (s.I()>s.K())?s.I().V():s.K().V() ) : ( (s.J()>s.K())?s.J().V():s.K().V() );
         }

         bool m_valid;
         IndexRange3dC m_line;
      };

      // Area of voxels in a voxel set
      class VoxelAreaC
      {
      public:
         VoxelAreaC() : m_valid(false)
         { }

         VoxelAreaC(const VoxelLineC &line) : m_valid(true)
         { m_lines += line; }

         bool operator==(const VoxelAreaC &oth) const
         {
            return (this==&oth);
         }

         inline const VoxelAreaC& operator+=(const VoxelLineC &line)
         { 
            m_lines += line;
            m_valid = true;
            return *this;
         }

         inline bool IsAdjacent(const VoxelLineC &line) const
         {
            if (!m_valid) return false;
            DLIterC<VoxelLineC> it(m_lines);
            for (; it; it++)
            {
               if (it.Data().IsAdjacent(line)) return true;
            }
            return false;
         }

         inline bool IsAdjacent(const VoxelAreaC &area) const
         {
            if (!m_valid) return false;
            DLIterC<VoxelLineC> it(m_lines);
            DLIterC<VoxelLineC> ito(area.m_lines);
            for (; it; it++)
            {
               for (ito.First(); ito; ito++)
               {
                  if (ito.Data().IsAdjacent(it.Data())) return true;
               }
            }
            return false;
         }

         inline void Merge(const VoxelAreaC &area)
         {
            DLIterC<VoxelLineC> ito(area.m_lines);
            for (ito.First(); ito; ito++)
            {
               m_lines.InsFirst(ito.Data());
               m_valid = true;
            }
         }

         inline UIntT Size() const
         {
            if (!m_valid) return 0;
            UIntT count=0;
            DLIterC<VoxelLineC> it(m_lines);
            for (; it; it++)
            {
               count += it.Data().Size();
            }
            return count;
         }

         bool m_valid;
         DListC<VoxelLineC> m_lines;
      };

      // Volume of voxels in a voxel set
      class VoxelVolumeC
      {
      public:
         VoxelVolumeC() : m_valid(false)
         { }

         bool operator==(const VoxelVolumeC &oth) const
         {
            return (this==&oth);
         }

         VoxelVolumeC(const VoxelAreaC &area) : m_valid(true)
         { m_areas += area; }

         inline const VoxelVolumeC& operator+=(const VoxelAreaC &area) 
         { 
            m_areas += area;
            m_valid = true;
            return *this;
         }

         inline bool IsAdjacent(const VoxelAreaC &area) const
         {
            if (!m_valid) return false;
            DLIterC<VoxelAreaC> it(m_areas);
            for (; it; it++)
            {
               if (it.Data().IsAdjacent(area)) return true;
            }
            return false;
         }

         inline void Merge(const VoxelVolumeC &vol)
         {
            DLIterC<VoxelAreaC> ito(vol.m_areas);
            for (ito.First(); ito; ito++)
            {
               m_areas.InsFirst(ito.Data());
               m_valid = true;
            }
         }

         inline UIntT Size() const
         {
            if (!m_valid) return 0;
            UIntT count=0;
            DLIterC<VoxelAreaC> it(m_areas);
            for (; it; it++)
            {
               count += it.Data().Size();
            }
            return count;
         }

         bool m_valid;
         DListC<VoxelAreaC> m_areas;
      };
   }
   // VOXEL SET CLEANING ///////////////////////////////////////////////////

   // Remove disconnected voxel regions to extract the largest 6-connected region
   void 
   VoxelCleanC::ExtractSingleRegion()
   {
      if (!m_voxels.IsValid()) return;

      IndexRange3dC frame = m_voxels.Array().Frame();
      IndexRange3dIterC voxit(frame);
      Array3dIterC<ByteT> voxelsit(m_voxels.Array());
      IndexC I, J, K;
      // Collect the volumes in the voxel set
      DListC<VoxelVolumeC> vols;
      for (I=frame.Range1().Min(); I<=frame.Range1().Max(); I++)
      {
         // Collect the areas on this plane
         DListC<VoxelAreaC> areas;
         for (J=frame.Range2().Min(); J<=frame.Range2().Max(); J++)
         {
            // Collect the lines on this row
            DListC<VoxelLineC> lines;
            VoxelLineC line;
            for (K=frame.Range3().Min(); K<=frame.Range3().Max(); K++, voxit++, voxelsit++)
            {
               if (voxelsit.Data())
               {
                  line += voxit.Data();
               }
               if (!voxelsit.Data() || K==frame.Range3().Max())
               {
                  if (line.m_valid)
                  {
                     lines += line;
                  }
                  // start a new line
                  line = VoxelLineC();
               }
            } // K
            //Add the lines to the areas they are adjacent to
            DLIterC<VoxelLineC> itl(lines);
            for (; itl; itl++)
            {
               DLIterC<VoxelAreaC> ita(areas);
               UIntT count=0;
               DListC<VoxelAreaC*> mergeset;
               for ( ; ita; ita++)
               {
                  if (ita.Data().IsAdjacent(itl.Data()))
                  {
                     ita.Data() += itl.Data();
                     count++;
                     mergeset += &ita.Data();
                  }
               }
               if (!count)
               {
                  // Need to start a new area
                  areas += VoxelAreaC(itl.Data());
               }
               else if (count>1)
               {
                  // Areas now need merging
                  DLIterC<VoxelAreaC*> itmergeset(mergeset);
                  VoxelAreaC& merged = *itmergeset.Data();
                  for (itmergeset++; itmergeset; itmergeset++)
                  {
                     merged.Merge(*itmergeset.Data());
                     areas.Del(*itmergeset.Data());
                  }
               }
            }
         } // J
         // Add areas to the volumes created previously
         DLIterC<VoxelAreaC> ita(areas);
         for (; ita; ita++)
         {
            DLIterC<VoxelVolumeC> itv(vols);
            UIntT count=0;
            DListC<VoxelVolumeC*> mergeset;
            for ( ; itv; itv++)
            {
               if (itv.Data().IsAdjacent(ita.Data()))
               {
                  itv.Data() += ita.Data();
                  count++;
                  mergeset += &itv.Data();
               }
            }
            if (!count)
            {
               // Need to start a new volume
               vols += VoxelVolumeC(ita.Data());
            }
            else if (count>1)
            {
               // Volumes now need merging
               DLIterC<VoxelVolumeC*> itmergeset(mergeset);
               VoxelVolumeC& merged = *itmergeset.Data();
               for (itmergeset++; itmergeset; itmergeset++)
               {
                  merged.Merge(*itmergeset.Data());
                  vols.Del(*itmergeset.Data());
               }
            }
         }
      } // I

      // Find the largest volume
      if (vols.Size()>1)
      {
         VoxelVolumeC *largest = NULL;
         UIntT size = 0;
         // Find the largest volume
         DLIterC<VoxelVolumeC> itv(vols);
         for ( ; itv; itv++)
         {
            UIntT s = itv.Data().Size();
            if (s>size)
            {
               largest = &itv.Data();
               size = s;
            }
         }
         // Remove the voxels in all other volumes
         for (itv.First() ; itv; itv++)
         {
            if (&itv.Data() != largest)
            {
               DLIterC<VoxelAreaC> ita(itv.Data().m_areas);
               for (; ita; ita++)
               {
                  DLIterC<VoxelLineC> itl(ita.Data().m_lines);
                  for (; itl; itl++)
                  {
                     Array3dIterC<ByteT> itdata(m_voxels.Array(),itl.Data().m_line);
                     for (; itdata; itdata++)
                     {
                        itdata.Data() = 0;
                     }
                  }
               }
            }
         }
      }
   }

   // Erode end points from a voxel set to remove noise on the surface
   void 
   VoxelCleanC::CleanEndPoints()
   {
      IntT i,j,k;

      // Use a voxel skeleton to determine voxel connectivity
      VoxelSkeletonC skel(m_voxels.Array(), m_voxels.OccupiedThreshold());

      // Iterate through the voxels
      ByteT* pvoxel = &m_voxels.Array()[voxframe.Origin()];
      pvoxel += m_l; // avoid first slice
      for (i = 1; i < n_minus_1; i++) 
      {
         pvoxel += l; // avoid first row
         for (j = 1; j < m_minus_1; j++)
         {
            pvoxel++; // avoid first cell
            for (k = 1; k < l_minus_1; k++)
            {
               if (*pvoxel)
               {
                  // Check 6-connectivity for a surface voxel
                  int count = 0;
                  if (*(pvoxel+1) ) count++;
                  if (*(pvoxel-1) ) count++;
                  if (*(pvoxel+l) ) count++;
                  if (*(pvoxel-l) ) count++;
                  if (*(pvoxel+m_l) ) count++;
                  if (*(pvoxel-m_l) ) count++;
                  if (count<2)
                  {
                     // remove this voxel if it is an end-point
                     *pvoxel=0;
                  }
                  else if (count < 6) 
                  {
                     // Remove surface voxels that connect regions
                     if (!skel.SingleForeground(Index3dC(i,j,k)))
                     {
                        *pvoxel=0;
                     }
                  }
               }
               // move to next voxel
               pvoxel++; 
            } // Iterate through row of voxels
            // skip last cell
            pvoxel++; 
         } // Iterate through voxels in slice
         // skip last row
         pvoxel += l; 
      } // Iterate through slices in the voxel set

   }

   // Dilate a voxel set to fill holes
   void 
   VoxelCleanC::FillHoles()
   {
      // Dilate the voxel set
      Array3dIterC<ByteT> it(m_voxels.Array(),  voxframe.Erode());
      for (; it; it++)
      {
         if (!it.Data1())
         {
            // Check the 6-neighbourhood for a surface voxel
            ByteT* pvoxel = &it.Data();
            IntT count=0;
            if (*(pvoxel+1) ) count++;
            if (*(pvoxel-1) ) count++;
            if (*(pvoxel+l) ) count++;
            if (*(pvoxel-l) ) count++;
            if (*(pvoxel+m_l) ) count++;
            if (*(pvoxel-m_l) ) count++;
            if (count>0)
            {
               // Count how many voxels are in the 18-neighbourhood
               if (*(pvoxel+m_l     - 1) ) count++;
               if (*(pvoxel     + l - 1) ) count++;
               if (*(pvoxel     - l - 1) ) count++;
               if (*(pvoxel-m_l     - 1) ) count++;
               if (*(pvoxel+m_l     + 1) ) count++;
               if (*(pvoxel     + l + 1) ) count++;
               if (*(pvoxel     - l + 1) ) count++;
               if (*(pvoxel-m_l     + 1) ) count++;
               if (*(pvoxel+m_l - l    ) ) count++;
               if (*(pvoxel-m_l - l    ) ) count++;
               if (*(pvoxel+m_l + l    ) ) count++;
               if (*(pvoxel-m_l + l    ) ) count++;
               if (count>12)
               {
                  // Fill the hole
                  it.Data() = 255;
               }
            }
         }
      }
   }

   // Erode the voxel set
   void 
   VoxelCleanC::Erode()
   {
     ByteT mask = 255;

     // Create a working array
     Array3dC<ByteT> &array = m_voxels.Array();
     Array3dC<ByteT> work(voxframe);

     // Only work on the interior voxels of the set
     IndexRange3dC frame = voxframe.Erode();

     // Erode the voxel set
     Array3dIterC<ByteT> it(work, frame);
     IndexRange3dIterC rit(frame);
     for (rit.First(); rit; it++, rit++)
     {
        ByteT* pvoxel = &array[rit.Data()];
        if (!*pvoxel || !*(pvoxel+1) || !*(pvoxel-1) || !*(pvoxel+l) || 
           !*(pvoxel-l) || !*(pvoxel+m_l) || !*(pvoxel-m_l) )
        {
           // If the voxel is unoccupied or a surface voxel, set as unoccupied in the work array
           it.Data() = 0;
        }
        else
        {
           it.Data() = mask;
        }
     }

     m_voxels.Array() = work;
   }

   // Dilate the voxel set while not connecting disconnected regions
   void 
   VoxelCleanC::Dilate()
   {
     ByteT mask = 255;
     Array3dC<ByteT> &array = m_voxels.Array();

     // Create a working array
     Array3dC<ByteT> work(voxframe);

     // Only work on the interior voxels of the set
     // The external voxels should always be empty to ensure that we get a closed mesh
     IndexRange3dC frame = voxframe.Erode();

     // Use the skeleton to test voxel connectivity
     VoxelSkeletonC skel(array, m_voxels.OccupiedThreshold());

     // Dilate the voxel set but do not connect separate regions
     Array3dIterC<ByteT> it(work, frame);
     IndexRange3dIterC rit(frame);
     for (; rit; it++, rit++)
     {
        Index3dC i = rit.Data();
        it.Data() = 0;
        ByteT* pvoxel = &array[i];
        if (*pvoxel)
        {
           it.Data() = mask;
        }
        else if (*(pvoxel+1) || *(pvoxel-1) || *(pvoxel+l) || 
                 *(pvoxel-l) || *(pvoxel+m_l) || *(pvoxel-m_l) )
        {
           // Do not connect regions
           if (skel.SingleForeground(i))
           {
              it.Data() = mask;
           }
        }
     }

     m_voxels.Array() = work;
   }

   // Remove surface noise by eroding then dilating
   void 
   VoxelCleanC::MorphologicalOpen()
   {
     ByteT mask = 255;
     ByteT thresh = m_voxels.OccupiedThreshold();

     // Create a working array
     Array3dC<ByteT> &array = m_voxels.Array();
     Array3dC<ByteT> work(voxframe);

     // Only work on the interior voxels of the set
     IndexRange3dC frame = voxframe.Erode();

     // Erode the voxel set
     Array3dIterC<ByteT> it(work, frame);
     IndexRange3dIterC rit(frame);
     for (rit.First(); rit; it++, rit++)
     {
        ByteT* pvoxel = &array[rit.Data()];
        if (*pvoxel<=thresh || *(pvoxel+1)<=thresh || *(pvoxel-1)<=thresh || *(pvoxel+l)<=thresh || 
            *(pvoxel-l)<=thresh || *(pvoxel+m_l)<=thresh || *(pvoxel-m_l)<=thresh )
        {
           // If the voxel is unoccupied or a surface voxel, set as unoccupied in the work array
           it.Data() = 0;
        }
        else
        {
           it.Data() = mask;
        }
     }

     // Use the skeleton to test voxel connectivity
     VoxelSkeletonC skel(work, thresh);

     // Dilate the voxel set but do not connect separate regions
     it = Array3dIterC<ByteT>(array, frame);
     for (rit.First(); rit; it++, rit++)
     {
        // Keep occupied voxels that are adjacent to, or overlap with, the eroded set
        // i.e delete a voxel that is not adjacent
        if (it.Data() > m_voxels.OccupiedThreshold())
        {
           Index3dC i = rit.Data();
           ByteT* pvoxel = &work[i];
           if (!*pvoxel)
           {
              if ( *(pvoxel+1) || *(pvoxel-1) || *(pvoxel+l) || 
                   *(pvoxel-l) || *(pvoxel+m_l) || *(pvoxel-m_l) )
              {
                 // Remove voxels that connect separate regions
                 if (!skel.SingleForeground(i))
                 {
                    it.Data() = 0;
                 }
              }
              else
              {
                 // Remove voxels that are not adjacent to the eroded set
                 it.Data() = 0;
              }
           }
        }
     }
   }

   // Fill holes in the voxel set by dilating then eroding
   void 
   VoxelCleanC::MorphologicalClose()
   {
     ByteT mask = 255;
     ByteT thresh = m_voxels.OccupiedThreshold();
     Array3dC<ByteT> &array = m_voxels.Array();

     // Create a working array
     Array3dC<ByteT> work(voxframe);

     // Only work on the interior voxels of the set
     // The external voxels should always be empty to ensure that we get a closed mesh
     IndexRange3dC frame = voxframe.Erode();

     // Use the skeleton to test voxel connectivity
     VoxelSkeletonC skel(array, m_voxels.OccupiedThreshold());

     // Dilate the voxel set but do not connect separate regions
     Array3dIterC<ByteT> it(work, frame);
     IndexRange3dIterC rit(frame);
     for (; rit; it++, rit++)
     {
        Index3dC i = rit.Data();
        it.Data() = 0;
        ByteT* pvoxel = &array[i];
        if (*pvoxel > thresh)
        {
           it.Data() = mask;
        }
        else if (*(pvoxel+1)>thresh || *(pvoxel-1)>thresh || *(pvoxel+l)>thresh || 
                 *(pvoxel-l)>thresh || *(pvoxel+m_l)>thresh || *(pvoxel-m_l)>thresh )
        {
           // Do not connect regions
           if (skel.SingleForeground(i))
           {
              it.Data() = mask;
           }
        }
     }

     // Erode the voxel set
     it = Array3dIterC<ByteT>(array, frame);
     for (rit.First(); rit; it++, rit++)
     {
        ByteT* pvoxel = &work[rit.Data()];
        if (!*pvoxel || !*(pvoxel+1) || !*(pvoxel-1) || !*(pvoxel+l) || 
           !*(pvoxel-l) || !*(pvoxel+m_l) || !*(pvoxel-m_l) )
        {
          it.Data() = 0;
        }
        else
        {
           it.Data() = mask;
        }
     }
   }

}}
