// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006-14, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#include "Ravl/Hash.hh"
#include "Ravl/RealRange2d.hh"
//! lib=RavlVisualHull

#include "Ravl/VisualHull/VisualHull.hh"

namespace RavlN { namespace VisualHullN {

   namespace
   {
      bool 
         VoxLessThanOrEqual(const Tuple2C< RealT, Index3dC>& v1,
         const Tuple2C< RealT, Index3dC>& v2)
      {
         return (v1.Data1() <= v2.Data1());
      }
   }

   // Determine the ordered set of voxels corresponding to each pixel in a camera image
   // This determines the complete set of voxels whose projection overlaps each pixel
   // set singleIntersection to only return pixels where there is a single intersection with the voxel set
   // expand defines the size to expand each pixel to catch the voxel projection
   HashC<Index2dC, DListC<Index3dC> > OrderedVoxelProjection(
      const HSetC<Index2dC>& pixels,
      const ByteVoxelGridC& voxels,
      const PinholeCameraC& camera,
      bool singleIntersection ,
      IntT expand)
   {
      HashC<Index2dC, DListC<Index3dC> > ret;

      // Determine the position of the voxel
      RealT voxelSize = voxels.VoxelSize();
      Matrix3dC RT = voxels.R().T() * (voxelSize);
      Vector3dC tw;
      TMul(voxels.R(),voxels.t()*-1.0,tw); 
      RealT halfVoxelSize = voxelSize / 2.0;
      RealT maxVoxelDist = voxelSize * 1.8;
      Vector3dC origin;
      camera.Origin(origin);

      // Create a list of voxels that project to a pixel in the image 
      HashC<Index2dC, DListC< Tuple2C<RealT, Index3dC> > > projection;

      // Iterate over the voxel set
      const Array3dC<ByteT>& arr = voxels.Array();
      Array3dIterC<ByteT> ita(arr);
      IndexRange3dIterC iti(arr.Frame());
      for (; ita; ita++, iti++)
      {
         if (ita.Data())
         {
            const Index3dC& vox = iti.Data();

            // Determine the voxel projection in the camera image
            Vector3dC pt = RT*Vector3dC((RealT)vox.I().V(),(RealT)vox.J().V(),(RealT)vox.K().V()) + tw;
            RealT dist = pt.EuclidDistance(origin);

            // Project each corner of the voxel into the image and determine the pixel range
            Vector2dC imgPt;
            camera.Project(imgPt,pt + Vector3dC(-halfVoxelSize, -halfVoxelSize, -halfVoxelSize));
            RealRange2dC pixrange(imgPt,0);
            camera.Project(imgPt,pt + Vector3dC(-halfVoxelSize, -halfVoxelSize, halfVoxelSize));
            pixrange.Involve(imgPt);
            camera.Project(imgPt,pt + Vector3dC(-halfVoxelSize, halfVoxelSize, -halfVoxelSize));
            pixrange.Involve(imgPt);
            camera.Project(imgPt,pt + Vector3dC(-halfVoxelSize, halfVoxelSize, halfVoxelSize));
            pixrange.Involve(imgPt);
            camera.Project(imgPt,pt + Vector3dC(halfVoxelSize, -halfVoxelSize, -halfVoxelSize));
            pixrange.Involve(imgPt);
            camera.Project(imgPt,pt + Vector3dC(halfVoxelSize, -halfVoxelSize, halfVoxelSize));
            pixrange.Involve(imgPt);
            camera.Project(imgPt,pt + Vector3dC(halfVoxelSize, halfVoxelSize, -halfVoxelSize));
            pixrange.Involve(imgPt);
            camera.Project(imgPt,pt + Vector3dC(halfVoxelSize, halfVoxelSize, halfVoxelSize));
            pixrange.Involve(imgPt);

            // Construct the image index range ( and convert from x-y pixrange to row-col indexrange)
            IndexRange2dC frame(Floor(pixrange.LCol()), Ceil(pixrange.RCol()), Floor(pixrange.TRow()), Ceil(pixrange.BRow()));
            frame = frame.Expand(expand);
            frame.ClipBy(camera.Frame());

            // Rasterise the range and store the voxel against every pixel
            IndexRange2dIterC itpix(frame);
            for (; itpix; itpix++)
            {
               if (pixels.Contains(itpix.Data()))
               {
                  projection[ itpix.Data() ] += Tuple2C<RealT, Index3dC>(dist, vox);
               }
            }
         }
      }

      // Sort the voxels for each pixel by distance and store single intersections
      HashIterC<Index2dC, DListC< Tuple2C<RealT, Index3dC> > > itp(projection);
      for (; itp; itp++)
      {
         // Sort the voxels by distance along the ray
         itp.Data().MergeSort(VoxLessThanOrEqual);

         if (singleIntersection)
         {
            // Check for multiple intersections with the voxel set
            bool discontinuity = false;
            DLIterC< Tuple2C<RealT, Index3dC> > it(itp.Data());
            RealT lastdist = it.Data().Data1();
            for (it++; it; it++)
            {
               RealT dist = it.Data().Data1();
               if (dist - lastdist > maxVoxelDist)
               {
                  discontinuity = true;
                  break;
               }
               lastdist = dist;
            }

            // If only a single intersection add to the result
            if (!discontinuity)
            {
               DListC<Index3dC>& intersection = ret[itp.Key()];
               for (it.First(); it; it++)
               {
                  intersection += it.Data().Data2();
               }
            }
         }
         else
         {
            // Always add to the result
            DListC<Index3dC>& intersection = ret[itp.Key()];
            DLIterC< Tuple2C<RealT, Index3dC> > it(itp.Data());
            for (; it; it++)
            {
               intersection += it.Data().Data2();
            }
         }
      }

      return ret;
   }

   // Determine the set of voxels corresponding to each pixel in a camera image
   // Returns an unordered list of voxels for each pixel
   // expand defines the size to expand each pixel to catch the voxel projection
   HashC<Index2dC, DListC<Index3dC> > VoxelProjection(
      const HSetC<Index2dC>& pixels,
      const ByteVoxelGridC& voxels,
      const PinholeCameraC& camera,
      IntT expand)
   {
      HashC<Index2dC, DListC<Index3dC> > projection;

      // Determine the position of the voxel
      RealT voxelSize = voxels.VoxelSize();
      Matrix3dC RT = voxels.R().T() * (voxelSize);
      Vector3dC tw;
      TMul(voxels.R(),voxels.t()*-1.0,tw); 
      RealT halfVoxelSize = voxelSize / 2.0;
      Vector3dC origin;
      camera.Origin(origin);

      // Create a list of voxels that project to a pixel in the image 

      // Iterate over the voxel set
      const Array3dC<ByteT>& arr = voxels.Array();
      Array3dIterC<ByteT> ita(arr);
      IndexRange3dIterC iti(arr.Frame());
      for (; ita; ita++, iti++)
      {
         if (ita.Data())
         {
            const Index3dC& vox = iti.Data();

            // Determine the voxel projection in the camera image
            Vector3dC pt = RT*Vector3dC((RealT)vox.I().V(),(RealT)vox.J().V(),(RealT)vox.K().V()) + tw;
            // Project each corner of the voxel into the image and determine the pixel range
            Vector2dC imgPt;
            camera.Project(imgPt,pt + Vector3dC(-halfVoxelSize, -halfVoxelSize, -halfVoxelSize));
            RealRange2dC pixrange(imgPt,0);
            camera.Project(imgPt,pt + Vector3dC(-halfVoxelSize, -halfVoxelSize, halfVoxelSize));
            pixrange.Involve(imgPt);
            camera.Project(imgPt,pt + Vector3dC(-halfVoxelSize, halfVoxelSize, -halfVoxelSize));
            pixrange.Involve(imgPt);
            camera.Project(imgPt,pt + Vector3dC(-halfVoxelSize, halfVoxelSize, halfVoxelSize));
            pixrange.Involve(imgPt);
            camera.Project(imgPt,pt + Vector3dC(halfVoxelSize, -halfVoxelSize, -halfVoxelSize));
            pixrange.Involve(imgPt);
            camera.Project(imgPt,pt + Vector3dC(halfVoxelSize, -halfVoxelSize, halfVoxelSize));
            pixrange.Involve(imgPt);
            camera.Project(imgPt,pt + Vector3dC(halfVoxelSize, halfVoxelSize, -halfVoxelSize));
            pixrange.Involve(imgPt);
            camera.Project(imgPt,pt + Vector3dC(halfVoxelSize, halfVoxelSize, halfVoxelSize));
            pixrange.Involve(imgPt);

            // Construct the image index range ( and convert from x-y pixrange to row-col indexrange)
            IndexRange2dC frame(Floor(pixrange.LCol()), Ceil(pixrange.RCol()), Floor(pixrange.TRow()), Ceil(pixrange.BRow()));
            frame = frame.Expand(expand);
            frame.ClipBy(camera.Frame());

            // Rasterise the range and store the voxel against every pixel
            IndexRange2dIterC itpix(frame);
            for (; itpix; itpix++)
            {
               if (pixels.Contains(itpix.Data()))
               {
                  projection[ itpix.Data() ] += vox;
               }
            }
         }
      }

      // Return the set of voxels stored against pixels
      return projection;
   }

   // Extract the surface voxels from a voxel set as all foreground voxels 6-connected to background voxels
   HSetC<Index3dC> 
      ExtractSurfaceVoxels(const ByteVoxelGridC& voxels)
   {
      HSetC<Index3dC> ret;
      ByteT thresh = voxels.OccupiedThreshold();
      const Array3dC<ByteT>& arr = voxels.Array();
      IndexRange3dC frame = arr.Frame();

      // Check that this is a surface voxel
      Array3dIterC<ByteT> ita(arr);
      IndexRange3dIterC iti(frame);
      for (; ita; ita++, iti++)
      {
         if (ita.Data() > thresh)
         {
            const Index3dC& vox = iti.Data();
            // Use a 6-connected neighbourhood to determine the surface set
            Index3dC n1(vox.I()-1,vox.J(),vox.K());
            Index3dC n2(vox.I()+1,vox.J(),vox.K());
            Index3dC n3(vox.I(),vox.J()-1,vox.K());
            Index3dC n4(vox.I(),vox.J()+1,vox.K());
            Index3dC n5(vox.I(),vox.J(),vox.K()-1);
            Index3dC n6(vox.I(),vox.J(),vox.K()+1);
            if ((!frame.Contains(n1) || arr[n1]<=thresh) ||
                (!frame.Contains(n2) || arr[n2]<=thresh) ||
                (!frame.Contains(n3) || arr[n3]<=thresh) ||
                (!frame.Contains(n4) || arr[n4]<=thresh) ||
                (!frame.Contains(n5) || arr[n5]<=thresh) ||
                (!frame.Contains(n6) || arr[n6]<=thresh))
            {
               // This is a surface voxel
               ret += vox;
            }
         }
      }

      return ret;
   }

}}

