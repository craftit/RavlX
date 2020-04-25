// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006-14, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_VOXEL_VOXELSURFACE_HEADER
#define RAVL_VOXEL_VOXELSURFACE_HEADER 1
//! lib=RavlVoxels

//
// VoxelSurfaceC implements the Marching Cubes algorithm on a voxel set
//
// VoxelSurfaceC holds a reference to the voxel set for surface extraction.
//

#include "Ravl/Hash.hh"
#include "Ravl/3D/TriMesh.hh"
#include "Ravl/Voxels/VoxelSet.hh"

namespace RavlN { namespace VoxelsN {

   using Ravl3DN::TriMeshC;

   template <class DataTypeT>
   TriMeshC ExtractVoxelSurface(const VoxelSetC<DataTypeT>& voxels);
   // Polygonize the voxel surface using marching cubes

   //: Class for construction of a trimesh for the surface of a voxel set using "Marching Cubes"
   template <class DataTypeT>
   class VoxelSurfaceC
   {
   public:
      //:-
      // CONSTRUCTION/DESTRUCTION /////////////////////////////////////////////

      VoxelSurfaceC(const VoxelSetC<DataTypeT>& voxels)
         : m_voxels(voxels)
      { }
      //: Default constructor

   public:
      //:-
      // SURFACE EXTRACTION ///////////////////////////////////////////////////

      TriMeshC ExtractSurface();
      //: extract a surface mesh from the voxel set as the voxels.OccupiedThreshold() level set
      // Note internal regions are assumed to have a value > OccupiedThreshold

   protected:
      void Polygonize(const Index3dC& i000, DListC<Vector3dC>& verts, DListC<UIntT>& faceIndices, UIntT& currentVIndex, HashC<Index3dC,UIntT>& vlookup);
      VoxelSetC<DataTypeT> m_voxels; 
   };

   struct MarchingCubes
   {
      static const int EdgeTable[256];    // Look-up for intersected edges 
      static const int TriTable[256][16]; // Look-up for triangle faces
   };

   // Lookup tables for marching cubes

   //: Polygonize a cube
   template <class DataTypeT>
      void 
      VoxelSurfaceC<DataTypeT>::Polygonize(const Index3dC& i000, DListC<Vector3dC>& verts, DListC<UIntT>& faceIndices, UIntT& currentVIndex, HashC<Index3dC,UIntT>& vlookup)
   {
      RealT thresh = (RealT)m_voxels.OccupiedThreshold();
      Index3dC cubeverts[12];
      Vector3dC fnverts[12];

      // Get the dual cube
      IntT i = i000.I().V(), j = i000.J().V(), k = i000.K().V();
      Index3dC i001(i,j,k+1);
      Index3dC i010(i,j+1,k);
      Index3dC i011(i,j+1,k+1);
      Index3dC i100(i+1,j,k);
      Index3dC i101(i+1,j,k+1);
      Index3dC i110(i+1,j+1,k);
      Index3dC i111(i+1,j+1,k+1);
      // Get the data for the dual cube
      RealT ijk    = (RealT)m_voxels.GetVoxel(i000);
      RealT ijk1   = (RealT)m_voxels.GetVoxel(i001);
      RealT ij1k   = (RealT)m_voxels.GetVoxel(i010);
      RealT ij1k1  = (RealT)m_voxels.GetVoxel(i011);
      RealT i1jk   = (RealT)m_voxels.GetVoxel(i100);
      RealT i1jk1  = (RealT)m_voxels.GetVoxel(i101);
      RealT i1j1k  = (RealT)m_voxels.GetVoxel(i110);
      RealT i1j1k1 = (RealT)m_voxels.GetVoxel(i111);
      
      // Check occupancy
      IntT iLookup = 0;
      if (ijk <= thresh) iLookup |= 1;  // i,j,k
      if (i1jk <= thresh) iLookup |= 2;  // i+1,j,k
      if (i1jk1 <= thresh) iLookup |= 4;  // i+1,j,k+1
      if (ijk1 <= thresh) iLookup |= 8;  // i,j,k+1
      if (ij1k <= thresh) iLookup |= 16; // i,j+1,k
      if (i1j1k <= thresh) iLookup |= 32; // i+1,j+1,k
      if (i1j1k1 <= thresh) iLookup |= 64; // i+1,j+1,k+1
      if (ij1k1 <= thresh) iLookup |= 128;// i,j+1,k+1
      // Look-up the edge intersections
      int edges = MarchingCubes::EdgeTable[iLookup];
      if (edges==0) return;

      // Interpolate the function values to find the zero crossing
      int i2=i*2, j2=j*2, k2=k*2;
      if (edges & 1) { // i+?,j,k
         RealT f0 = ijk - thresh; // i,j,k
         RealT f1 = i1jk - thresh; // i+1,j,k
         RealT r = f0/(f0-f1);
         fnverts[0] = Vector3dC(i+r,j,k);
         IntT n = r == 0 ? 0 : r == 1 ? 2 : 1;
         cubeverts[0] =  Index3dC(i2+n,j2  ,k2);
      }
      if (edges & 2) { // i+1,j,k+?
         RealT f0 = i1jk - thresh; // i+1,j,k
         RealT f1 = i1jk1 - thresh; // i+1,j,k+1
         RealT r = f0/(f0-f1);
         fnverts[1] = Vector3dC(i+1,j,k+r);
         IntT n = r == 0 ? 0 : r == 1 ? 2 : 1;
         cubeverts[1] =  Index3dC(i2+2,j2  ,k2+n);
      }
      if (edges & 4) { // i+?,j,k+1
         RealT f0 = ijk1 - thresh; // i,j,k+1
         RealT f1 = i1jk1 - thresh; // i+1,j,k+1
         RealT r = f0/(f0-f1);
         fnverts[2] = Vector3dC(i+r,j,k+1);
         IntT n = r == 0 ? 0 : r == 1 ? 2 : 1;
         cubeverts[2] =  Index3dC(i2+n,j2  ,k2+2);
      }
      if (edges & 8) { // i,j,k+?
         RealT f0 = ijk - thresh; // i,j,k
         RealT f1 = ijk1 - thresh; // i,j,k+1
         RealT r = f0/(f0-f1);
         fnverts[3] = Vector3dC(i,j,k+r);
         IntT n = r == 0 ? 0 : r == 1 ? 2 : 1;
         cubeverts[3] =  Index3dC(i2  ,j2  ,k2+n);
      }
      if (edges & 16){ // i+?,j+1,k
         RealT f0 = ij1k - thresh; // i,j+1,k
         RealT f1 = i1j1k - thresh; // i+1,j+1,k
         RealT r = f0/(f0-f1);
         fnverts[4] = Vector3dC(i+r,j+1,k);
         IntT n = r == 0 ? 0 : r == 1 ? 2 : 1;
         cubeverts[4] =  Index3dC(i2+n,j2+2,k2);
      }
      if (edges & 32) { // i+1,j+1,k+?
         RealT f0 = i1j1k - thresh; // i+1,j+1,k
         RealT f1 = i1j1k1 - thresh; // i+1,j+1,k+1
         RealT r = f0/(f0-f1);
         fnverts[5] = Vector3dC(i+1,j+1,k+r);
         IntT n = r == 0 ? 0 : r == 1 ? 2 : 1;
         cubeverts[5] =  Index3dC(i2+2,j2+2,k2+n);
      }
      if (edges & 64) { // i+?,j+1,k+1
         RealT f0 = ij1k1 - thresh; // i,j+1,k+1
         RealT f1 = i1j1k1 - thresh; // i+1,j+1,k+1
         RealT r = f0/(f0-f1);
         fnverts[6] = Vector3dC(i+r,j+1,k+1);
         IntT n = r == 0 ? 0 : r == 1 ? 2 : 1;
         cubeverts[6] =  Index3dC(i2+n,j2+2,k2+2);
      }
      if (edges & 128) { // i,j+1,k+?
         RealT f0 = ij1k - thresh; // i,j+1,k
         RealT f1 = ij1k1 - thresh; // i,j+1,k+1
         RealT r = f0/(f0-f1);
         fnverts[7] = Vector3dC(i,j+1,k+r);
         IntT n = r == 0 ? 0 : r == 1 ? 2 : 1;
         cubeverts[7] =  Index3dC(i2  ,j2+2,k2+n);
      }
      if (edges & 256) { // i,j+?,k
         RealT f0 = ijk - thresh; // i,j,k
         RealT f1 = ij1k - thresh; // i,j+1,k
         RealT r = f0/(f0-f1);
         fnverts[8] = Vector3dC(i,j+r,k);
         IntT n = r == 0 ? 0 : r == 1 ? 2 : 1;
         cubeverts[8] =  Index3dC(i2  ,j2+n,k2);
      }
      if (edges & 512) { // i+1,j+?,k
         RealT f0 = i1jk - thresh; // i+1,j,k
         RealT f1 = i1j1k - thresh; // i+1,j+1,k
         RealT r = f0/(f0-f1);
         fnverts[9] = Vector3dC(i+1,j+r,k);
         IntT n = r == 0 ? 0 : r == 1 ? 2 : 1;
         cubeverts[9] =  Index3dC(i2+2,j2+n,k2);
      }
      if (edges & 1024) { // i+1,j+?,k+1
         RealT f0 = i1jk1 - thresh; // i+1,j,k+1
         RealT f1 = i1j1k1 - thresh; // i+1,j+1,k+1
         RealT r = f0/(f0-f1);
         fnverts[10] = Vector3dC(i+1,j+r,k+1);
         IntT n = r == 0 ? 0 : r == 1 ? 2 : 1;
         cubeverts[10] =  Index3dC(i2+2,j2+n,k2+2);
      }
      if (edges & 2048) { // i,j+?,k+1
         RealT f0 = ijk1 - thresh; // i,j,k+1
         RealT f1 = ij1k1 - thresh; // i,j+1,k+1
         RealT r = f0/(f0-f1);
         fnverts[11] = Vector3dC(i,j+r,k+1);
         IntT n = r == 0 ? 0 : r == 1 ? 2 : 1;
         cubeverts[11] =  Index3dC(i2  ,j2+n,k2+2);
      }

      // Create the triangle faces
      const int* piTriList = &(MarchingCubes::TriTable[iLookup][0]);
      
      
      for (; (*piTriList)!=-1; piTriList+=3) 
      {
         if (cubeverts[*(piTriList)]   != cubeverts[*(piTriList+1)] &&
             cubeverts[*(piTriList)]   != cubeverts[*(piTriList+2)] &&
             cubeverts[*(piTriList+1)] != cubeverts[*(piTriList+2)] )
         {
            
             if (fnverts[*(piTriList)]   == fnverts[*(piTriList+1)] ||
                 fnverts[*(piTriList)]   == fnverts[*(piTriList+2)] ||
                 fnverts[*(piTriList+1)] == fnverts[*(piTriList+2)] )
                 {
                    cout << "not caught error" << endl;
                    continue;
                 }
            int v;
            for (v=2; v>=0; v--) 
            {
               Index3dC vindex3d = cubeverts[*(piTriList+v)];
               UIntT vindex;
               if (vlookup.Lookup(vindex3d,vindex)) 
               {
                  // Vertex already created
                  faceIndices += vindex;
               } 
               else 
               {
                  // Create a vertex	      
                  Vector3dC vpos = m_voxels.WorldPosition(fnverts[*(piTriList+v)]);
                  verts += vpos;
                  faceIndices += currentVIndex;
                  vlookup.Update(vindex3d,currentVIndex);
                  currentVIndex++;
               }
            }
         }
      }
   }

   //: extract a surface mesh from the voxel set as the voxels.OccupiedThreshold() level set
   template <class DataTypeT>
      TriMeshC 
      VoxelSurfaceC<DataTypeT>::ExtractSurface()
   {
      DListC<Vector3dC> verts;          // List of vertices
      DListC<UIntT> faceIndices;        // List of indices for faces
      UIntT currentVIndex = 0;          // Current vertex index
      HashC<Index3dC,UIntT> vlookup;    // Track vertices created

      // Iterate over the voxel set
      VoxelSetIterC<DataTypeT> voxit = m_voxels.Iterator();
      for (; voxit; voxit++)
      {
         const Index3dC& vox = voxit.Index();
         Polygonize(vox, verts, faceIndices, currentVIndex, vlookup);
         // Check for boundaries on the voxel set
         Index3dC n1(vox.I()-1, vox.J(), vox.K());
         Index3dC n2(vox.I(), vox.J()-1, vox.K());
         Index3dC n3(vox.I(), vox.J(), vox.K()-1);
         bool b1 = m_voxels.IsElm(n1);
         bool b2 = m_voxels.IsElm(n2);
         bool b3 = m_voxels.IsElm(n3);
         if (!b1)
         {
            Polygonize(n1, verts, faceIndices, currentVIndex, vlookup);
         }
         if (!b2)
         {
            Polygonize(n2, verts, faceIndices, currentVIndex, vlookup);
         }
         if (!b3)
         {
            Polygonize(n3, verts, faceIndices, currentVIndex, vlookup);
         }
         if (!b1 && !b2)
         {
            Index3dC n(vox.I()-1, vox.J()-1, vox.K());
            if (!m_voxels.IsElm(n))
            {
               Polygonize(n, verts, faceIndices, currentVIndex, vlookup);
            }
         }
         if (!b2 && !b3)
         {
            Index3dC n(vox.I(), vox.J()-1, vox.K()-1);
            if (!m_voxels.IsElm(n))
            {
               Polygonize(n, verts, faceIndices, currentVIndex, vlookup);
            }
         }
         if (!b1 && !b3)
         {
            Index3dC n(vox.I()-1, vox.J(), vox.K()-1);
            if (!m_voxels.IsElm(n))
            {
               Polygonize(n, verts, faceIndices, currentVIndex, vlookup);
            }
         }
         if (!b1 && !b2 && !b3)
         {
            Index3dC n(vox.I()-1, vox.J()-1, vox.K()-1);
            if (!m_voxels.IsElm(n))
            {
               Polygonize(n, verts, faceIndices, currentVIndex, vlookup);
            }
         }
      }

      // Now generate a mesh from the list of vertices and face indices
      TriMeshC ret(verts,faceIndices);
      return ret;
   }

   // Polygonize the voxel surface using marching cubes
   template <class DataTypeT>
   TriMeshC ExtractVoxelSurface(const VoxelSetC<DataTypeT>& voxels)
   {
      VoxelSurfaceC<DataTypeT> surf(voxels);
      return surf.ExtractSurface();
   }

}}

#endif 
