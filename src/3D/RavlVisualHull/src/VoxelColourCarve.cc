// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006-14, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#include "Ravl/HSet.hh"
#include "Ravl/DList.hh"
#include "Ravl/Tuple3.hh"
#include "Ravl/Tuple4.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/Array3dIter.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/SArray1dIter3.hh"
#include "Ravl/IndexRange2d.hh"
#include "Ravl/IndexRange2dIter.hh"
#include "Ravl/IndexRange3dIter.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Image/RealRGBValue.hh"
#include "Ravl/3D/PinholeCamera0.hh"
#include "Ravl/3D/PinholeCameraArray.hh"
//! lib=RavlVisualHull

// MVT includes
#include "Ravl/VisualHull/VoxelColourCarve.hh"
#include "Ravl/Voxels/Ray3d.hh"
#include "Ravl/Voxels/VoxelRayIter.hh"


namespace RavlN { namespace VisualHullN {
  using VoxelsN::ByteVoxelGridC;
  using Ravl3DN::PinholeCameraArrayC;
  using Ravl3DN::PinholeCameraC;
  using RavlImageN::RealRGBValueC;

   // CONSTRUCTION/DESTRUCTION /////////////////////////////////////////////

   //: Constructor for carving of a voxel set
   VoxelColourCarveBodyC::VoxelColourCarveBodyC(ByteVoxelGridC voxels,
      PinholeCameraArrayC& cameras,
      RealT threshold)
      : m_voxels(voxels), m_threshold(threshold), 
        m_cameras(cameras),
        m_origins(cameras.Array().Size())
   {
      // Precompute the camera origins
      SArray1dIter2C<PinholeCameraC, Vector3dC> it(m_cameras.Array(), m_origins);
      for (; it; it++)
      {
         it.Data1().Origin( it.Data2() );
      }
   }

   // CARVING //////////////////////////////////////////////////////////////

   //: Determine the pixels that correspond to a voxel in an image
   // Returns true if a valid pixel range is found
   bool 
      VoxelColourCarveBodyC::LookupVoxel(IndexRange2dC& pixrange, const Index3dC& vox, const IntT cam) const
   {
      static Vector3dC delta[] = { Vector3dC(0,0,0), Vector3dC(0,0,1), Vector3dC(0,1,0), Vector3dC(0,1,1),
                                   Vector3dC(1,0,0), Vector3dC(1,0,1), Vector3dC(1,1,0), Vector3dC(1,1,1) }; 
      // Define the voxel corners
      const PinholeCameraC& camera = m_cameras[cam];
      Vector3dC p0((RealT)vox.I().V()-0.5,(RealT)vox.J().V()-0.5,(RealT)vox.K().V()-0.5);
      for (IntT i=0; i<8; i++)
      {
         Vector3dC xw = m_voxels.WorldPosition(p0 + delta[i]);
         Vector2dC imgPt;
         camera.Project(imgPt, xw);
         Index2dC pix((IntT)(imgPt.Y()+0.5),(IntT)(imgPt.X()+0.5));
         if (i==0)
         {
            pixrange = IndexRange2dC(pix,0);
         }
         else
         {
            pixrange.Involve(pix);
         }
      }
      return (pixrange.Rows()!=0 && pixrange.Cols()!=0);
   }


   // Find the first occupied voxel along the ray corresponding to a pixel in a camera image
   // Iteration through the voxel set is lifted from VoxelRayIterC as we can generate a ray in voxel indices directly
   bool 
   VoxelColourCarveBodyC::FindFirstVoxel(Index3dC& voxel, const IntT cam, const Index2dC& pix) const
   {
      const Array3dC<ByteT>& voxArray = m_voxels.Array();
      IndexRange3dC frame = voxArray.Frame();

      // Find the ray in voxel space for the image pixel
      const Vector3dC& o = m_origins[cam];
      const PinholeCameraC& camera = m_cameras[cam];
      Vector3dC n;
      camera.ProjectInverseDirection(n, Vector2dC(pix.Col(), pix.Row()) );
      // Create a normalized vector to increment in the voxel set
      RealT absX = Abs(n.X()), absY = Abs(n.Y()), absZ = Abs(n.Z());
      RealT base = (absX>absY && absX>absZ) ? absX : ( (absY>absX && absY>absZ) ? absY : absZ );
      n /= base;

      // Clip the ray against the frame of the voxel array
      RealT start=0.0;
      // Clip against +/- x
      if (absX > 1E-6)
      {
         RealT minI = (RealT)frame.Range1().Min().V();
         RealT maxI = (RealT)frame.Range1().Max().V();
         RealT minIntersect = (minI-o.X())/n.X();
         RealT maxIntersect = (maxI-o.X())/n.X();
         if (n.X()>0.0) 
         {
            if (maxIntersect<0.0) return false; // Ray starts outside bounds
            if (minIntersect>start) start = minIntersect;
         } else {
            if (minIntersect<0.0) return false; // Ray starts outside bounds
            if (maxIntersect>start) start = maxIntersect;
         }
      }
      // Clip against +/- y
      if (absY > 1E-6)
      {
         RealT minJ = (RealT)frame.Range2().Min().V();
         RealT maxJ = (RealT)frame.Range2().Max().V();
         RealT minIntersect = (minJ-o.Y())/n.Y();
         RealT maxIntersect = (maxJ-o.Y())/n.Y();
         if (n.Y()>0.0) {
            if (maxIntersect<0.0) return false; // Ray starts outside bounds
            if (minIntersect>start) start = minIntersect;
         } else {
            if (minIntersect<0.0) return false; // Ray starts outside bounds
            if (maxIntersect>start) start = maxIntersect;
         }
      }
      // Clip against +/- z
      if (absZ > 1E-6)
      {
         RealT minK = (RealT)frame.Range3().Min().V();
         RealT maxK = (RealT)frame.Range3().Max().V();
         RealT minIntersect = (minK-o.Z())/n.Z();
         RealT maxIntersect = (maxK-o.Z())/n.Z();
         if (n.Z()>0.0) {
            if (maxIntersect<0.0) return false; // Ray starts outside bounds
            if (minIntersect>start) start = minIntersect;
         } 
         else 
         {
            if (minIntersect<0.0) return false; // Ray starts outside bounds
            if (maxIntersect>start) start = maxIntersect;
         }
      }

      // Iterate along the array and find the first occupied voxel
      // Set the start point in the voxel set
      Vector3dC p = o + n*start;
      // Set the increment in the voxel set
      Index3dC at((IntT)(p.X()+0.5),(IntT)(p.Y()+0.5),(IntT)(p.Z()+0.5));
      while (frame.Contains(at))
      {
         if (voxArray[at]) 
         {
            voxel = at;
            return true;
         }
         p += n;
         at = Index3dC((IntT)(p.X()+0.5),(IntT)(p.Y()+0.5),(IntT)(p.Z()+0.5));
      }

      return false;
   }

   // Generate the layered depth image for a camera view
   ImageC< Index3dC > 
   VoxelColourCarveBodyC::GenerateLDI(const ImageC<ByteT>& sil, const IntT cam) const
   {
      ImageC< Index3dC > ldi = ImageC< Index3dC >(sil.Frame());

      Array2dIter2C< ByteT, Index3dC > it(sil,ldi);
      IndexRange2dIterC itpix(sil.Frame());

      for (; it; it++, itpix++)
      {
         it.Data2() = Index3dC(-1,-1,-1);
         if (it.Data1())
         {
            Index3dC voxel;
            if (FindFirstVoxel(voxel,cam,itpix.Data()))
            {
               it.Data2() = voxel;
            }
         }
      }

      return ldi;
   }

   //: The Generalized Voxel Coloring algorithm using layered depth images
   // Returns the numbder of carved voxels
   IntT
      VoxelColourCarveBodyC::Update(
      const SArray1dC< ImageC<ByteRGBValueC> >& rgbimages,
      const SArray1dC< ImageC<ByteT> >& silimages)
   {
      IntT countCarved = 0;

      // Set-up to iterate through the voxel set
      Array3dC<ByteT> &voxArray = m_voxels.Array();
      ByteT mask = m_voxels.OccupiedThreshold();
      IndexRange3dC voxframe = m_voxels.Array().Frame();
      ByteT* pvoxel = &voxArray[voxframe.Origin()];
      IntT i,j,k;
      IntT n = voxframe.Range1().Size();
      IntT m = voxframe.Range2().Size(); 
      IntT l = voxframe.Range3().Size();
      IntT n_minus_1 = n - 1;
      IntT m_minus_1 = m - 1;
      IntT l_minus_1 = l - 1;
      IntT m_l = m*l;

      // Generate the layered depth images
      SArray1dC< ImageC< Index3dC > > ldis(silimages.Size());
      SArray1dIter2C<ImageC<ByteRGBValueC>,ImageC<Index3dC> > itimages(rgbimages,ldis);
      SArray1dIterC<ImageC<ByteT> > itcam(silimages);
      IntT cam = 0;
      for (; itimages; itimages++, itcam++, cam++) {
         itimages.Data2() = GenerateLDI(itcam.Data1(),cam);
      }

      // Create a set of changed visibility surface voxels
      HSetC<Index3dC> cvsvl;
      pvoxel += m_l; // avoid first slice
      for (i = 1; i < n_minus_1; i++) {
         pvoxel += l; // avoid first row
         for (j = 1; j < m_minus_1; j++) {
            pvoxel++; // avoid first cell
            for (k = 1; k < l_minus_1; k++) {
               // Determine if this is a surface voxel
               if (*pvoxel == mask) {
                  // Check if this voxel is 6-connected to a background voxel
                  if ((*(pvoxel+1)  != mask || *(pvoxel-1)  != mask ||
                     *(pvoxel+l)  != mask || *(pvoxel-l)  != mask ||
                     *(pvoxel+m_l) != mask || *(pvoxel-m_l) != mask)) 
                  {
                     // Found a surface voxel
                     cvsvl.Insert(Index3dC(i,j,k));
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


      // Threshold for variance of pixel colours
      RealT dThresh = m_threshold*m_threshold;
      // Iterate through the changing surface set
      while (!cvsvl.IsEmpty())
      {
         Index3dC voxel = cvsvl.First();
         cvsvl.Remove(voxel);

         // Iterate through the images accumulating pixel data for this voxel
         RealT sumR=0.0, sumG=0.0, sumB=0.0;
         RealT sumSqR=0.0, sumSqG=0.0, sumSqB=0.0;
         IntT count=0, countcam=0;
         DListC< Tuple3C< ImageC<Index3dC>,Index2dC,IntT > > vislist;
         for (cam=0, itimages.First(); itimages; itimages++, cam++) 
         {
            // Get the pixel range for the voxel in this image
            IndexRange2dC pixrange;
            if (LookupVoxel(pixrange, voxel, cam))
            { 
               Array2dIter2C<ByteRGBValueC,Index3dC> itrgb(itimages.Data1(),itimages.Data2(),pixrange);
               IndexRange2dIterC itrgbpix(pixrange);
               IntT countunocc=0;
               for (; itrgb; itrgb++, itrgbpix++) 
               {
                  if (itrgb.Data2() == voxel)
                  {
                     vislist += Tuple3C< ImageC<Index3dC>,Index2dC,IntT >(itimages.Data2(),itrgbpix.Data(),cam);
                     const ByteRGBValueC& col = itrgb.Data1();
                     // Check that we have a colour to use
                     if (col.Y() > 0)
                     {
                        countunocc++;
                        RealRGBValueC c(col);
// NOR USING NORMALISED COLOUR AS WE NEED TO TELL BLACK & WHITE FROM OTHER COLOURS
//                        c /= c.Y();
                        sumR += c.Red(); sumSqR += c.Red()*c.Red();
                        sumG += c.Green(); sumSqG += c.Green()*c.Green();
                        sumB += c.Blue(); sumSqB += c.Blue()*c.Blue();
                     }
                  }
               }
               if (countunocc>0) 
               {
                  countcam++;
                  count += countunocc;
               }
            }
         }
         if (count!=0 && countcam>1) 
         {
            // Test the colour consistency of the image pixels
            RealT invCount = 1/(double)count;
            RealT varR = sumSqR*invCount - sumR*sumR*invCount*invCount;
            RealT varG = sumSqG*invCount - sumG*sumG*invCount*invCount;
            RealT varB = sumSqB*invCount - sumB*sumB*invCount*invCount;
            RealT testVar = (varR + varG + varB)/3.0;
            if (testVar>dThresh) 
            {
               // Carve!
               voxArray[voxel] = 0;
               // Remove the voxel from the ldis and add any voxels that are now visible
               DLIterC< Tuple3C< ImageC<Index3dC>,Index2dC,IntT > > itvis(vislist);
               for (; itvis; itvis++)
               {
                  Index3dC& newvoxel = itvis.Data().Data1()[itvis.Data().Data2()];
                  newvoxel = Index3dC(-1,-1,-1);
                  if ( FindFirstVoxel(newvoxel,itvis.Data().Data3(),itvis.Data().Data2()) )
                  {
                     cvsvl.Insert(newvoxel);
                  }
               }
               countCarved++;
            }
         }
      }

      // Done
      return countCarved;
   }

}}
