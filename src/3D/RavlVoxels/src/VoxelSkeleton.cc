// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006-14, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
// Ravl includes
#include "Ravl/HSet.hh"
#include "Ravl/DList.hh"
#include "Ravl/Array3dIter2.hh"
#include "Ravl/IndexRange3dIter.hh"
#include "Ravl/RandomMersenneTwister.hh"
#include "Ravl/Voxels/VoxelSet.hh"
//! lib=RavlVoxels

// MVTGeom includes
#include "Ravl/Voxels/VoxelSkeleton.hh"

namespace RavlN { namespace VoxelsN {

   // CONSTRUCTION/DESTRUCTION /////////////////////////////////////////////

   //: Construct from a voxel set
   VoxelSkeletonC::VoxelSkeletonC(const Array3dC<ByteT>& voxelArray, ByteT thresh)
      :  m_array(voxelArray), threshold(thresh)
   { 
      frame = m_array.Frame();
      n = frame.Range1().Size();
      m = frame.Range2().Size(); 
      l = frame.Range3().Size();
      m_l = m*l;
   }

   // SKELETON EXTRACTION //////////////////////////////////////////////////

   // Determine if a voxel is an end-point with only 1 26-connected neighbour
   bool 
   VoxelSkeletonC::VoxelEndPoint(const Index3dC& voxel) const
   {
      const ByteT* pvoxel = &m_array[voxel];
      // Count the number of neighbours in the 26-neighbourhood
      IntT count = 0;
      // 6-neighbourhood
      if (*(pvoxel+1) ) count++;
      if (*(pvoxel-1) ) count++;
      if (*(pvoxel+l) ) count++;
      if (*(pvoxel-l) ) count++;
      if (*(pvoxel+m_l) ) count++;
      if (*(pvoxel-m_l) ) count++;
      if (*(pvoxel+1) ) count++;
      // 18-neighbourhood
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
      // 26-neighbourhood
      if (*(pvoxel+m_l - l - 1) ) count++;
      if (*(pvoxel+m_l - l + 1) ) count++;
      if (*(pvoxel+m_l + l - 1) ) count++;
      if (*(pvoxel+m_l + l + 1) ) count++;
      if (*(pvoxel-m_l - l - 1) ) count++;
      if (*(pvoxel-m_l - l + 1) ) count++;
      if (*(pvoxel-m_l + l - 1) ) count++;
      if (*(pvoxel-m_l + l + 1) ) count++;

      return (count==1);
   }

   // Return if a given voxel has a single surrounding foreground
   bool 
   VoxelSkeletonC::SingleForeground(const Index3dC& voxel) const
   {
      const ByteT* pvoxel = &m_array[voxel];
      IndexRange3dC frame(-1,1,-1,1,-1,1);
      Array3dC<ByteT> foreground(frame);
      Array3dC<ByteT> visited(frame);
      foreground.Fill((ByteT)0);
      visited.Fill((ByteT)0);

      // Extract the 26-connected foreground
      Index3dC s;
      IntT fcount = 0;
      if (*(pvoxel+1) ) { s=Index3dC(1,0,0); foreground[s]=(ByteT)255; fcount++; }
      if (*(pvoxel-1) ) { s=Index3dC(-1,0,0); foreground[s]=(ByteT)255; fcount++; }
      if (*(pvoxel+l) ) { s=Index3dC(0,1,0); foreground[s]=(ByteT)255; fcount++; }
      if (*(pvoxel-l) ) { s=Index3dC(0,-1,0); foreground[s]=(ByteT)255; fcount++; }
      if (*(pvoxel+m_l) ) { s=Index3dC(0,0,1); foreground[s]=(ByteT)255; fcount++; }
      if (*(pvoxel-m_l) ) { s=Index3dC(0,0,-1); foreground[s]=(ByteT)255; fcount++; }
      if (*(pvoxel+m_l     - 1) ) { s=Index3dC(-1,0,1); foreground[s]=(ByteT)255; fcount++; }
      if (*(pvoxel     + l - 1) ) { s=Index3dC(-1,1,0); foreground[s]=(ByteT)255; fcount++; }
      if (*(pvoxel     - l - 1) ) { s=Index3dC(-1,-1,0); foreground[s]=(ByteT)255; fcount++; }
      if (*(pvoxel-m_l     - 1) ) { s=Index3dC(-1,0,-1); foreground[s]=(ByteT)255; fcount++; }
      if (*(pvoxel+m_l     + 1) ) { s=Index3dC(1,0,1); foreground[s]=(ByteT)255; fcount++; }
      if (*(pvoxel     + l + 1) ) { s=Index3dC(1,1,0); foreground[s]=(ByteT)255; fcount++; }
      if (*(pvoxel     - l + 1) ) { s=Index3dC(1,-1,0); foreground[s]=(ByteT)255; fcount++; }
      if (*(pvoxel-m_l     + 1) ) { s=Index3dC(1,0,-1); foreground[s]=(ByteT)255; fcount++; }
      if (*(pvoxel+m_l - l    ) ) { s=Index3dC(0,-1,1); foreground[s]=(ByteT)255; fcount++; }
      if (*(pvoxel-m_l - l    ) ) { s=Index3dC(0,-1,-1); foreground[s]=(ByteT)255; fcount++; }
      if (*(pvoxel+m_l + l    ) ) { s=Index3dC(0,1,1); foreground[s]=(ByteT)255; fcount++; }
      if (*(pvoxel-m_l + l    ) ) { s=Index3dC(0,1,-1); foreground[s]=(ByteT)255; fcount++; }
      if (*(pvoxel+m_l - l - 1) ) { s=Index3dC(-1,-1,1); foreground[s]=(ByteT)255; fcount++; }
      if (*(pvoxel+m_l - l + 1) ) { s=Index3dC(1,-1,1); foreground[s]=(ByteT)255; fcount++; }
      if (*(pvoxel+m_l + l - 1) ) { s=Index3dC(-1,1,1); foreground[s]=(ByteT)255; fcount++; }
      if (*(pvoxel+m_l + l + 1) ) { s=Index3dC(1,1,1); foreground[s]=(ByteT)255; fcount++; }
      if (*(pvoxel-m_l - l - 1) ) { s=Index3dC(-1,-1,-1); foreground[s]=(ByteT)255; fcount++; }
      if (*(pvoxel-m_l - l + 1) ) { s=Index3dC(1,-1,-1); foreground[s]=(ByteT)255; fcount++; }
      if (*(pvoxel-m_l + l - 1) ) { s=Index3dC(-1,1,-1); foreground[s]=(ByteT)255; fcount++; }
      if (*(pvoxel-m_l + l + 1) ) { s=Index3dC(1,1,-1); foreground[s]=(ByteT)255; fcount++; }
      if (fcount==0) return false;

      // Starting from a seed point check that all foreground voxels are 26-connected
      IntT vcount = 0;
      DListC<Index3dC> visit;
      visit += s;
      visited[s] = (ByteT)255;
      DLIterC<Index3dC> itv(visit);
      for (; itv; itv++)
      {
         vcount++;
         const Index3dC& vindex = itv.Data();
         IndexRange3dIterC it( IndexRange3dC(vindex,1).ClipBy(frame) );
         for (; it; it++)
         {
            const Index3dC& n = it.Data();
            if (n != vindex)
            {
               if (foreground[n] && visited[n] == 0)
               {
                  visit += n;
                  visited[n] = (ByteT)255;
               }
            }
         }
      }

      return (vcount == fcount);
   }

#if 0
   // Return if a given voxel has a single surrounding foreground
   bool 
   VoxelSkeletonC::SingleForeground(const Index3dC& voxel) const
   {
      const ByteT* pvoxel = &m_array[voxel];
      IntT i,j,k;
      // Get all the foreground voxels in the 26-neighbourhood (except this voxel)
      HSetC<Index3dC> foreground;
      if (*(pvoxel+1) ) foreground.Insert(Index3dC(1,0,0));
      if (*(pvoxel-1) ) foreground.Insert(Index3dC(-1,0,0));
      if (*(pvoxel+l) ) foreground.Insert(Index3dC(0,1,0));
      if (*(pvoxel-l) ) foreground.Insert(Index3dC(0,-1,0));
      if (*(pvoxel+m_l) ) foreground.Insert(Index3dC(0,0,1));
      if (*(pvoxel-m_l) ) foreground.Insert(Index3dC(0,0,-1));
      if (*(pvoxel+m_l     - 1) ) foreground.Insert(Index3dC(-1,0,1));
      if (*(pvoxel     + l - 1) ) foreground.Insert(Index3dC(-1,1,0));
      if (*(pvoxel     - l - 1) ) foreground.Insert(Index3dC(-1,-1,0));
      if (*(pvoxel-m_l     - 1) ) foreground.Insert(Index3dC(-1,0,-1));
      if (*(pvoxel+m_l     + 1) ) foreground.Insert(Index3dC(1,0,1));
      if (*(pvoxel     + l + 1) ) foreground.Insert(Index3dC(1,1,0));
      if (*(pvoxel     - l + 1) ) foreground.Insert(Index3dC(1,-1,0));
      if (*(pvoxel-m_l     + 1) ) foreground.Insert(Index3dC(1,0,-1));
      if (*(pvoxel+m_l - l    ) ) foreground.Insert(Index3dC(0,-1,1));
      if (*(pvoxel-m_l - l    ) ) foreground.Insert(Index3dC(0,-1,-1));
      if (*(pvoxel+m_l + l    ) ) foreground.Insert(Index3dC(0,1,1));
      if (*(pvoxel-m_l + l    ) ) foreground.Insert(Index3dC(0,1,-1));
      if (*(pvoxel+m_l - l - 1) ) foreground.Insert(Index3dC(-1,-1,1));
      if (*(pvoxel+m_l - l + 1) ) foreground.Insert(Index3dC(1,-1,1));
      if (*(pvoxel+m_l + l - 1) ) foreground.Insert(Index3dC(-1,1,1));
      if (*(pvoxel+m_l + l + 1) ) foreground.Insert(Index3dC(1,1,1));
      if (*(pvoxel-m_l - l - 1) ) foreground.Insert(Index3dC(-1,-1,-1));
      if (*(pvoxel-m_l - l + 1) ) foreground.Insert(Index3dC(1,-1,-1));
      if (*(pvoxel-m_l + l - 1) ) foreground.Insert(Index3dC(-1,1,-1));
      if (*(pvoxel-m_l + l + 1) ) foreground.Insert(Index3dC(1,1,-1));

      if (foreground.IsEmpty()) return false;
      // Starting from a seed point check that all foreground voxels are 26-connected
      HSetC<Index3dC> visited;
      DListC<Index3dC> visit;
      visit += foreground.First();
      visited.Insert(visit.First());
      while (!visit.IsEmpty())
      {
         Index3dC vindex = visit.PopFirst();
         for (i=-1; i<2; i++)
         {
            for (j=-1; j<2; j++)
            {
               for (k=-1; k<2; k++)
               {
                  if (i==0 && j==0 && k==0) continue;
                  Index3dC n(vindex.I()+i,vindex.J()+j,vindex.K()+k);
                  if (foreground.Contains(n) && !visited.Contains(n)) 
                  {
                     visit += n;
                     visited.Insert(n);
                  }
               }
            }
         }
      }

      return (visited.Size() == foreground.Size());
   }
#endif

   // Return if a voxel is an outer voxel
   // Checks for a single connected background component under 6-connectivity in the 18-neighbourhood
   bool 
   VoxelSkeletonC::OuterVoxel(const Index3dC& voxel) const
   {
      const ByteT* pvoxel = &m_array[voxel];
      Array3dC<ByteT> background(IndexRange3dC(-1,1,-1,1,-1,1));
      Array3dC<ByteT> visited(IndexRange3dC(-1,1,-1,1,-1,1));
      background.Fill((ByteT)0);
      visited.Fill((ByteT)0);

      // Get the background voxels in the 18 neighbourhood
      Index3dC s;
      IntT bcount = 0;
      if (*(pvoxel+1)!=0 ) { s = Index3dC(1,0,0); background[s] = (ByteT)255; bcount++; }
      if (*(pvoxel-1)!=0 ) { s = Index3dC(-1,0,0); background[s] = (ByteT)255; bcount++; }
      if (*(pvoxel+l)!=0 ) { s = Index3dC(0,1,0); background[s] = (ByteT)255; bcount++; }
      if (*(pvoxel-l)!=0 ) { s = Index3dC(0,-1,0); background[s] = (ByteT)255; bcount++; }
      if (*(pvoxel+m_l)!=0 ) { s = Index3dC(0,0,1); background[s] = (ByteT)255; bcount++; }
      if (*(pvoxel-m_l)!=0 ) { s = Index3dC(0,0,-1); background[s] = (ByteT)255; bcount++; }
      if (*(pvoxel+m_l     - 1)!=0 ) { s = Index3dC(-1,0,1); background[s] = (ByteT)255; bcount++; }
      if (*(pvoxel     + l - 1)!=0 ) { s = Index3dC(-1,1,0); background[s] = (ByteT)255; bcount++; }
      if (*(pvoxel     - l - 1)!=0 ) { s = Index3dC(-1,-1,0); background[s] = (ByteT)255; bcount++; }
      if (*(pvoxel-m_l     - 1)!=0 ) { s = Index3dC(-1,0,-1); background[s] = (ByteT)255; bcount++; }
      if (*(pvoxel+m_l     + 1)!=0 ) { s = Index3dC(1,0,1); background[s] = (ByteT)255; bcount++; }
      if (*(pvoxel     + l + 1)!=0 ) { s = Index3dC(1,1,0); background[s] = (ByteT)255; bcount++; }
      if (*(pvoxel     - l + 1)!=0 ) { s = Index3dC(1,-1,0); background[s] = (ByteT)255; bcount++; }
      if (*(pvoxel-m_l     + 1)!=0 ) { s = Index3dC(1,0,-1); background[s] = (ByteT)255; bcount++; }
      if (*(pvoxel+m_l - l    )!=0 ) { s = Index3dC(0,-1,1); background[s] = (ByteT)255; bcount++; }
      if (*(pvoxel-m_l - l    )!=0 ) { s = Index3dC(0,-1,-1); background[s] = (ByteT)25; bcount++; }
      if (*(pvoxel+m_l + l    )!=0 ) { s = Index3dC(0,1,1); background[s] = (ByteT)255; bcount++; }
      if (*(pvoxel-m_l + l    )!=0 ) { s = Index3dC(0,1,-1); background[s] = (ByteT)255; bcount++; }
      if (bcount == 0) return false;

      // Starting from a seed point check that all background voxels are 6-connected
      IntT vcount = 0;
      DListC<Index3dC> visit;
      visit += s;
      visited[s] = (ByteT)255;
      DLIterC<Index3dC> itv(visit);
      for (; itv; itv++)
      {
         vcount++;
         const Index3dC& i = itv.Data();
         // Check for 6-connected background voxels
         Index3dC east(i.I()+1,i.J(),i.K()), west(i.I()-1,i.J(),i.K());
         Index3dC north(i.I(),i.J()+1,i.K()), south(i.I(),i.J()-1,i.K());
         Index3dC up(i.I(),i.J(),i.K()+1), down(i.I(),i.J(),i.K()-1);
         if (i.I()<1 && background[east] && visited[east]==0 ) { visit += east; visited[east] = (ByteT)255; }
         if (i.I()>-1 && background[west] && visited[west]==0) { visit += west; visited[west] = (ByteT)255; }
         if (i.J()<1 && background[north] && visited[north]==0) { visit += north; visited[north] = (ByteT)255; }
         if (i.J()>-1 && background[south] && visited[south]==0) { visit += south; visited[south] = (ByteT)255; }
         if (i.K()<1 && background[up] && visited[up]==0) { visit += up; visited[up] = (ByteT)255; }
         if (i.K()>-1 && background[down] && visited[down]==0) { visit += down; visited[down] = (ByteT)255; }
      }

      return (vcount == bcount);
   }

#if 0
   // Return if a voxel is an outer voxel
   // Checks for a single connected background component under 6-connectivity in the 18-neighbourhood
   bool 
   VoxelSkeletonC::OuterVoxel(const Index3dC& voxel) const
   {
      const ByteT* pvoxel = &m_array[voxel];

      // Get all the background voxels in the 18-neighbourhood
      HSetC<Index3dC> background;
      if (!*(pvoxel+1) ) background.Insert(Index3dC(1,0,0));
      if (!*(pvoxel-1) ) background.Insert(Index3dC(-1,0,0));
      if (!*(pvoxel+l) ) background.Insert(Index3dC(0,1,0));
      if (!*(pvoxel-l) ) background.Insert(Index3dC(0,-1,0));
      if (!*(pvoxel+m_l) ) background.Insert(Index3dC(0,0,1));
      if (!*(pvoxel-m_l) ) background.Insert(Index3dC(0,0,-1));
      if (!*(pvoxel+m_l     - 1) ) background.Insert(Index3dC(-1,0,1));
      if (!*(pvoxel     + l - 1) ) background.Insert(Index3dC(-1,1,0));
      if (!*(pvoxel     - l - 1) ) background.Insert(Index3dC(-1,-1,0));
      if (!*(pvoxel-m_l     - 1) ) background.Insert(Index3dC(-1,0,-1));
      if (!*(pvoxel+m_l     + 1) ) background.Insert(Index3dC(1,0,1));
      if (!*(pvoxel     + l + 1) ) background.Insert(Index3dC(1,1,0));
      if (!*(pvoxel     - l + 1) ) background.Insert(Index3dC(1,-1,0));
      if (!*(pvoxel-m_l     + 1) ) background.Insert(Index3dC(1,0,-1));
      if (!*(pvoxel+m_l - l    ) ) background.Insert(Index3dC(0,-1,1));
      if (!*(pvoxel-m_l - l    ) ) background.Insert(Index3dC(0,-1,-1));
      if (!*(pvoxel+m_l + l    ) ) background.Insert(Index3dC(0,1,1));
      if (!*(pvoxel-m_l + l    ) ) background.Insert(Index3dC(0,1,-1));
      if (background.IsEmpty()) return false;

      // Starting from a seed point check that all background voxels are 6-connected
      HSetC<Index3dC> visited;
      DListC<Index3dC> visit;
      visit += background.First();
      visited.Insert(visit.First());
      while (!visit.IsEmpty())
      {

         Index3dC i = visit.PopFirst();
         // Check for 6-connected background voxels
         Index3dC east(i.I()+1,i.J(),i.K()), west(i.I()-1,i.J(),i.K());
         Index3dC north(i.I(),i.J()+1,i.K()), south(i.I(),i.J()-1,i.K());
         Index3dC up(i.I(),i.J(),i.K()+1), down(i.I(),i.J(),i.K()-1);
         if (background.Contains(east) && !visited.Contains(east)) { visit += east; visited.Insert(east); }
         if (background.Contains(west) && !visited.Contains(west)) { visit += west; visited.Insert(west); }
         if (background.Contains(north) && !visited.Contains(north)) { visit += north; visited.Insert(north); }
         if (background.Contains(south) && !visited.Contains(south)) { visit += south; visited.Insert(south); }
         if (background.Contains(up) && !visited.Contains(up)) { visit += up; visited.Insert(up); }
         if (background.Contains(down) && !visited.Contains(down)) { visit += down; visited.Insert(down); }
      }

      return (visited.Size() == background.Size());
   }
#endif
   // Erode a border using a working array
   bool
   VoxelSkeletonC::ErodeBorder(Array3dC<ByteT>& array, 
      Array3dC<ByteT>& workArray,
      Border border,
      bool thin)
   {
      bool ret = false;

      IntT inc = 0;
      switch ( border )
      {
      case East: inc = 1; break;
      case West: inc = -1; break;
      case North: inc = l; break;
      case South: inc = -l; break;
      case Up: inc = m_l; break;
      case Down: inc = -m_l; break;
      default:
         inc = 1;
      }

      Array3dIter2C<ByteT, ByteT> it(array, workArray, frame.Erode());
      IndexRange3dIterC rit(frame.Erode());
      for (; it; it++, rit++)
      {
         ByteT *pavoxel = &it.Data1();
         ByteT *pwvoxel = &it.Data2();
         if (*pavoxel && !*(pavoxel+inc)) // If border point
         {
            if (!VoxelEndPoint(rit.Data()) && (thin || OuterVoxel(rit.Data())))
            {
               *pwvoxel = 0;
            }
         }
      }
      for (rit.First(), it.First(); it; it++, rit++)
      {
         ByteT *pavoxel = &it.Data1();
         ByteT *pwvoxel = &it.Data2();
         if (*pavoxel && !*pwvoxel) // If we deleted a border point
         {
            if (SingleForeground(rit.Data()))
            {
               *pavoxel = 0;
               ret = true;
            }
            else
            {
               *pwvoxel = threshold;
            }
         }
      }

      return ret;
   }

   //: erode a voxel set down to a skeleton 
   void 
   VoxelSkeletonC::ExtractSkeleton(bool thin)
   {
      RandomMersenneTwisterC random;

      // Create a working copy of the array
      Array3dC<ByteT> work = m_array.Copy();

      // Generate surface skeleton using 6-sub iteration technique:
      bool bThin = false;
      bool bContinue = true;
      while (bContinue) 
      {
         bContinue = false;

         // randomise the order of border erosion
         HSetC<IntT> order;
         for (IntT it=0; it<3; it++)
         {
            IntT border = 0;
            do
            {
               border = Floor( random.Double()*(RealT)(2) + 0.5);
            } while (order[border]);
            border *= 2;

            // Randomise the direction of border erosion
            IntT first = Floor( random.Double() + 0.5);
            IntT second = (first==0) ? 1 : 0;

            bContinue |= ErodeBorder(m_array, work, (Border)(border+first), bThin);
            bContinue |= ErodeBorder(m_array, work, (Border)(border+second), bThin);
         }
         // Thin to a single voxel thick
         if (!bContinue && !bThin && thin)
         {
            bContinue = bThin = true;
         }
      }
   }

}}
