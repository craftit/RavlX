// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006-14, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
// Ravl includes
#include "Ravl/Polygon2d.hh"
#include "Ravl/Array3dIter.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/Array2dIter3.hh"
#include "Ravl/IndexRange2dIter.hh"
//! lib=RavlVoxels

#include "Ravl/Voxels/VoxeliseMesh.hh"

namespace RavlN { namespace VoxelsN {

  using Ravl3DN::TriC;

   namespace
   {
      bool 
         DistLE(const RealT& val1, const RealT& val2)
      {
         return val1 <= val2;
      }
   }

   // Wrapper function to automatically setup the voxel grid required for a mesh
   ByteVoxelGridC 
   VoxeliseMesh(const TriMeshC& mesh, RealT voxelSize, bool verbose)
   {
      Vector3dC mmin, mmax;
      mesh.Limits(mmin,mmax);
      RealT volumeSize = Max( Abs(mmax.X()-mmin.X()), Max( Abs(mmax.Y()-mmin.Y()), Abs(mmax.Z()-mmin.Z()) ) );
      Vector3dC centroid = (mmin + mmax) * 0.5;
      ByteVoxelGridC voxels(centroid, voxelSize, volumeSize + voxelSize*2.0);
      VoxeliseMeshC voxmesh(voxels);
      voxmesh.Update(mesh, verbose);
      return voxels;
   }

   void
   VoxeliseMeshC::Update(const TriMeshC& mesh, bool verbose)
   {
      Array3dC<ByteT>& arr = m_voxels.Array();
      IndexRange3dC voxframe = arr.Frame();
      IndexRangeC irange = voxframe.Range1();
      IndexRangeC jrange = voxframe.Range2();
      IndexRangeC krange = voxframe.Range3();

      // Create a layered depth image for each axis
      Array2dC< DListC<RealT> > imgIJ(irange,jrange);
      Array2dC< DListC<RealT> > imgJK(jrange,krange);
      Array2dC< DListC<RealT> > imgKI(krange,irange);

      // Rasterise the mesh
      if (verbose)
      {
         cerr << "Rendering mesh...\n";
      }
      SArray1dC< Vector3dC > vox(3);
      SArray1dIterC<TriC> itf(mesh.Faces());
      for (; itf; itf++) 
      {
         // Get the set of points
         DListC<Point2dC> ptsIJ, ptsJK, ptsKI;
         for (IntT v=0; v<3; v++)
         {
            Vector3dC idx = m_voxels.VoxelIndexReal( itf.Data().Position(v) );
            vox[v] = idx;
            ptsIJ += Point2dC(idx.X(), idx.Y());
            ptsJK += Point2dC(idx.Y(), idx.Z());
            ptsKI += Point2dC(idx.Z(), idx.X());
         }
         // Rasterise the polygon in each image
         {
            Polygon2dC poly(ptsIJ);
            IndexRange2dC bbox = poly.BoundingRectangle().IndexRange();
            bbox.ClipBy(imgIJ.Frame());
            Array2dIterC< DListC<RealT> > it(imgIJ,bbox);
            IndexRange2dIterC itpix(bbox);
            for (; it; it++, itpix++) 
            {
               // Check if pixel is inside the polygon
               Point2dC pnt(itpix.Data());
               if (poly.Contains(pnt)) 
               {
                  // Calculate barycentric coords
                  SArray1dC<RealT> c = poly.BarycentricCoordinate(pnt);
                  // Calculate interpolated value
                  it.Data() += vox[0][2] * c[0] + vox[1][2] * c[1] + vox[2][2] * c[2];
               }
            }
         }
         {
            Polygon2dC poly(ptsJK);
            IndexRange2dC bbox = poly.BoundingRectangle().IndexRange();
            bbox.ClipBy(imgJK.Frame());
            Array2dIterC< DListC<RealT> > it(imgJK,bbox);
            IndexRange2dIterC itpix(bbox);
            for (; it; it++, itpix++) 
            {
               // Check if pixel is inside the polygon
               Point2dC pnt(itpix.Data());
               if (poly.Contains(pnt)) 
               {
                  // Calculate barycentric coords
                  SArray1dC<RealT> c = poly.BarycentricCoordinate(pnt);
                  // Calculate interpolated value
                  // Naughty naughty this is not a projective mapping
                  it.Data() += vox[0][0] * c[0] + vox[1][0] * c[1] + vox[2][0] * c[2];
               }
            }
         }
         {
            Polygon2dC poly(ptsKI);
            IndexRange2dC bbox = poly.BoundingRectangle().IndexRange();
            bbox.ClipBy(imgKI.Frame());
            Array2dIterC< DListC<RealT> > it(imgKI,bbox);
            IndexRange2dIterC itpix(bbox);
            for (; it; it++, itpix++) 
            {
               // Check if pixel is inside the polygon
               Point2dC pnt(itpix.Data());
               if (poly.Contains(pnt)) 
               {
                  // Calculate barycentric coords
                  SArray1dC<RealT> c = poly.BarycentricCoordinate(pnt);
                  // Calculate interpolated value
                  // Naughty naughty this is not a projective mapping
                  it.Data() += vox[0][1] * c[0] + vox[1][1] * c[1] + vox[2][1] * c[2];
               }
            }
         }
      }
      // Sort the image depths to lookup occupancy
      if (verbose)
      {
         cerr << "Depth sorting...\n";
      }
      Array2dIter3C< DListC<RealT>, DListC<RealT>, DListC<RealT> > itimg(imgIJ, imgJK, imgKI);
      for (; itimg; itimg++)
      {
         if (!itimg.Data1().IsEmpty())
         {
            itimg.Data1().MergeSort(DistLE);
         }
         if (!itimg.Data2().IsEmpty())
         {
            itimg.Data2().MergeSort(DistLE);
         }
         if (!itimg.Data3().IsEmpty())
         {
            itimg.Data3().MergeSort(DistLE);
         }
      }

      // Now fill in the voxel occupancy
      if (verbose)
      {
         cerr << "Setting occupancy...\n";
      }
      IndexRange3dIterC itvox(voxframe);
      Array3dIterC<ByteT> itarr(arr);
      for (; itvox; itvox++, itarr++)
      {
         // Check the occupancy of the voxel
         const Index3dC& vox = itvox.Data();
         const DListC<RealT>& ivals = imgJK[vox.J()][vox.K()];
         const DListC<RealT>& jvals = imgKI[vox.K()][vox.I()];
         const DListC<RealT>& kvals = imgIJ[vox.I()][vox.J()];
         IntT internal = 0;
         // Count the number of intersections along each axis to check for an internal / external region
         if (!ivals.IsEmpty())
         {
            IntT count = 0;
            DLIterC<RealT> it(ivals);
            while (it && it.Data() < vox.I()) { count++; it++; }
            if (count % 2 != 0) internal++;
         }
         if (!jvals.IsEmpty())
         {
            IntT count = 0;
            DLIterC<RealT> it(jvals);
            while (it && it.Data() < vox.J()) { count++; it++; }
            if (count % 2 != 0) internal++;
         }
         if (!kvals.IsEmpty())
         {
            IntT count = 0;
            DLIterC<RealT> it(kvals);
            while (it && it.Data() < vox.K()) { count++; it++; }
            if (count % 2 != 0) internal++;
         }
         // Use the majority vote to classify the voxel
         itarr.Data() = (internal > 1) ? 255 : 0;
      }
      m_voxels.OccupiedThreshold() = 127;
   }

}}
