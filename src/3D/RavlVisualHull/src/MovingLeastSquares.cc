// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006-14, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#include "Ravl/IO.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/DList.hh"
#include "Ravl/HashIter.hh"
#include "Ravl/Array3dIter.hh"
#include "Ravl/IndexRange3dIter.hh"
//#include "../../MeshToolkit/MeshProc/MeshProc.hh"
//! lib=RavlVisualHull

#include "Ravl/Voxels/VoxelSurface.hh"
#include "Ravl/VisualHull/MovingLeastSquares.hh"

namespace RavlN { namespace VisualHullN {

  static Vector3dC PlaneNormal(const DListC<Vector3dC>& points, Vector3dC hint)
  {
    MatrixC M(3,3);
    M.Fill(0.0);
    UIntT count = 0;
    Vector3dC cog(0,0,0);
    DLIterC<Vector3dC> it(points);
    for (; it; it++)
    {
       const Vector3dC& p = it.Data();
       cog += p;
       count++;
       M[0][0] += p[0]*p[0];
       M[0][1] += p[0]*p[1];
       M[0][2] += p[0]*p[2];
       M[1][0] += p[1]*p[0];
       M[1][1] += p[1]*p[1];
       M[1][2] += p[1]*p[2];
       M[2][0] += p[2]*p[0];
       M[2][1] += p[2]*p[1];
       M[2][2] += p[2]*p[2];
    }
    if (count == 0)
    {
       return Vector3dC(0,0,0);
    }
    cog /= (RealT)count;
    M /= (RealT)count;
    // Remove the mean
    M[0][0] -= cog[0]*cog[0];
    M[0][1] -= cog[0]*cog[1];
    M[0][2] -= cog[0]*cog[2];
    M[1][0] -= cog[1]*cog[0];
    M[1][1] -= cog[1]*cog[1];
    M[1][2] -= cog[1]*cog[2];
    M[2][0] -= cog[2]*cog[0];
    M[2][1] -= cog[2]*cog[1];
    M[2][2] -= cog[2]*cog[2];
    // Find the axes
    MatrixC U;
    VectorC d = EigenVectors(M,U);
    // Find the maximum eigenvalue
    IntT min = (d[2]<d[1]) ? ( d[2]<d[0] ? 2 : 0 ) : ( d[1]<d[0] ? 1 : 0 );
    // Set the normal along the principle eigenvector
    Vector3dC n(U[0][min], U[1][min], U[2][min]);
    // Set the orientation
    if (hint.Dot(n) < 0.0)
    {
       n *= -1.0;
    }
    return n.Unit();
  }


   // Extract the surface points from a voxel set
   // Returns the surface points as an array of vertices
   // Returns a lookup table to match voxels to the array
   IntT 
      MovingLeastSquaresC::ExtractSurface(SArray1dC<VertexC>& surfacepts, HashC<Index3dC, IntT>& surface) const
   {
      const Array3dC<ByteT>& arr = m_voxels.Array();
      Matrix3dC RT = m_voxels.R().T();

      // Create the set of surface points
      IntT size = 0;
      surface.Empty();
      IndexRange3dC frame = arr.Frame().Erode();
      Array3dIterC<ByteT> ita(arr,frame);
      IndexRange3dIterC iti(frame);
      for (; ita; ita++, iti++)
      {
         if (ita.Data())
         {
            const Index3dC& vox = iti.Data();
            // Use a 6-connected neighbourhood to determine the surface set
            Index3dC n1(vox.I()-1,vox.J(),vox.K());
            Index3dC n2(vox.I()+1,vox.J(),vox.K());
            Index3dC n3(vox.I(),vox.J()-1,vox.K());
            Index3dC n4(vox.I(),vox.J()+1,vox.K());
            Index3dC n5(vox.I(),vox.J(),vox.K()-1);
            Index3dC n6(vox.I(),vox.J(),vox.K()+1);
            // Is this a surface voxel
            if (!arr[n1] || !arr[n2] || !arr[n3] || !arr[n4] || !arr[n5] || !arr[n6])
            {
               // Add to the surface set
               surface[vox] = 0;
               size++;
            }
         }
      }
      // Create the array of vertices
      surfacepts = SArray1dC<VertexC>(size);
      HashIterC<Index3dC, IntT> itsurface(surface);
      SArray1dIterC<VertexC> itpts(surfacepts);
      IntT v = 0;
      for (; itsurface; itsurface++, itpts++, v++)
      {
         itpts.Data() = VertexC( m_voxels.WorldPosition(itsurface.Key()) );
         itsurface.Data() = v;
      }

      // Compute the normal for the surface points
      for (itpts.First(), itsurface.First(); itsurface; itsurface++, itpts++)
      {
         const Index3dC& vox = itsurface.Key();
         // Collect the surface points in the neighbourhood
         DListC<Vector3dC> pts;
         // Estimate the normal from the voxel set
         Vector3dC norm(0,0,0);
         IndexRange3dIterC itn( IndexRange3dC(vox,2).ClipBy(arr.Frame()) );
         for (; itn; itn++)
         {
            const Index3dC& nn = itn.Data();
            if (surface.Lookup(nn,v))
            {
               pts += surfacepts[v].Position();
            }
            if (arr[nn])
            {
               norm -= Vector3dC(nn.I() - vox.I(), nn.J() - vox.J(), nn.K() - vox.K());
            }
         }
         itpts.Data().Normal() = PlaneNormal(pts, RT * norm);
      }

      return size;
   }
/*
   // Extract the surface points from a voxel set
   // Returns the index and corresponding position + normal
   IntT 
      MovingLeastSquaresC::ExtractSurface(HSetC<Index3dC>& surfacepts) const
   {
      IntT size = 0;
      surfacepts.Empty();
      const Array3dC<ByteT>& arr = m_voxels.Array();

      IndexRange3dC frame = arr.Frame().Erode();
      Array3dIterC<ByteT> ita(arr,frame);
      IndexRange3dIterC iti(frame);
      for (; ita; ita++, iti++)
      {
         if (ita.Data())
         {
            const Index3dC& vox = iti.Data();
            // Use a 6-connected neighbourhood to determine the surface set
            Index3dC n1(vox.I()-1,vox.J(),vox.K());
            Index3dC n2(vox.I()+1,vox.J(),vox.K());
            Index3dC n3(vox.I(),vox.J()-1,vox.K());
            Index3dC n4(vox.I(),vox.J()+1,vox.K());
            Index3dC n5(vox.I(),vox.J(),vox.K()-1);
            Index3dC n6(vox.I(),vox.J(),vox.K()+1);
            // Is this a surface voxel
            if (!arr[n1] || !arr[n2] || !arr[n3] || !arr[n4] || !arr[n5] || !arr[n6])
            {
               surfacepts.Insert(vox);
               size++;
            }
         }
      }

      return size;
   }

   VertexC
      MovingLeastSquaresC::SurfaceVertex(const Index3dC& vox, const HSetC<Index3dC>& surfacepts) const
   {
      const Array3dC<ByteT>& arr = m_voxels.Array();

      // Collect the surface points in the neighbourhood
      DListC<Vector3dC> pts;
      // Estimate the normal from the voxel set
      Vector3dC norm(0,0,0);
      IndexRange3dIterC itn( IndexRange3dC(vox,2).ClipBy(arr.Frame()) );
      for (; itn; itn++)
      {
         const Index3dC& nn = itn.Data();
         if (surfacepts.Contains(nn))
         {
            pts += m_voxels.WorldPosition(nn);
         }
         if (arr[nn])
         {
            norm -= Vector3dC(nn.I() - vox.I(), nn.J() - vox.J(), nn.K() - vox.K());
         }
      }
      norm = m_voxels.R().TMul(norm);
      Vector3dC n = PlaneNormal(pts, norm);
      Vector3dC p = m_voxels.WorldPosition(vox);

      return VertexC(p,n);
   }
*/

   static bool FloatGrEq(const FloatT &f1, const FloatT &f2)
   {
      return f1 >= f2;
   }

   // Extract the depth to the surface in the volume
   FloatVoxelGridC
      MovingLeastSquaresC::ExtractDepth(
         IntT supportSize,
         bool filter,
         bool verbose) const
   {
      const Array3dC<ByteT>& arr = m_voxels.Array();
      IndexRange3dC frame = arr.Frame();
      RealT voxelSize = m_voxels.VoxelSize();
      FloatVoxelGridC ret(m_voxels.R(), m_voxels.t(), voxelSize, arr.Range1().Size(), 0.0f);
      Array3dC<FloatT>& deptharr = ret.Array();

      if (verbose)
      {
         cerr << "Extracting surface voxels...\n";
      }
      // Derive the set of surface points
      SArray1dC<VertexC> surfaceVerts;
      HashC<Index3dC, IntT> surface;
      ExtractSurface(surfaceVerts, surface);

      if (verbose)
      {
         cerr << "MLS...\n";
      }
      // Pre-allocate matrices for MLS
      MatrixC ATA(6,6);
      VectorC ATb(6);
      // Use pointer access for speed
      RealT* pATA = &ATA[0][0];
      RealT* pATb = &ATb[0];
      // Pre-allocate fixed size arrays to collect surface points
      IntT rangeSize = (IntT)Pow(2*supportSize+1,3);
      SArray1dC<Vector3dC> pts(rangeSize);
      // Pre-calculate the maximum voxel distance
      RealT minDist0 = Pow(supportSize*voxelSize,2) * 1e6;
      // Define a place holder for the set of surface points to use in MLS
      SArray1dC< HashC<Index2dC, DListC<IntT> > > surfacepts(frame.Range1());
      // Initialise depth according to the voxel occupancy
      IntT mini = frame.MinI().V() + supportSize;
      IntT maxi = frame.MaxI().V() + supportSize;
      IntT ilimit = frame.MaxI().V();
      Array3dIterC<ByteT> ita(arr);
      Array3dIterC<FloatT> itr(deptharr);
      IntT nextProgress = 10;
      IntT i = frame.MinI().V();
      for (; i <= maxi; i++)
      {
         if (verbose)
         {
            RealT progress = (RealT)(i - mini) * 100.0 / (RealT)(maxi - mini);
            if (progress > nextProgress)
            {
               cerr << nextProgress << "% ";
               nextProgress += 10;
            }
         }

         if (i <= ilimit)
         {
            // Initialise the depth array in this layer and accumulate surface points
            IntT j = frame.MinJ().V();
            for (; j <= frame.MaxJ().V(); j++)
            {
               IntT k = frame.MinK().V();
               for (; k <= frame.MaxK().V(); k++, ita++, itr++)
               {
                  Index3dC vox(i,j,k);
                  // Initialise the depth
                  if (ita.Data())
                  {
                     itr.Data() = 1e6;
                  }
                  else
                  {
                     itr.Data() = -1e6;
                  }
                  // Check to see if this is a surface point
                  if (ita.Data())
                  {
                     IntT v = 0;
                     if (surface.Lookup(vox,v))
                     {
                        // Add this surface point to the neighbourhood
                        IndexRange3dC framen(vox,supportSize);
                        framen.ClipBy(frame);
                        IndexRange3dIterC itn(framen);
                        for (; itn; itn++)
                        {
                           const Index3dC& voxn = itn.Data();
                           DListC<IntT>& vlist = surfacepts[voxn.I()][ Index2dC(voxn.J(),voxn.K()) ];
                           vlist += v;
                        }
                     }
                  }
               }
            }
         }
         // Perform MLS when all vertices have been accumulated in a slab
         if (i >= mini)
         {
            IntT ii = i - supportSize;
            // Compute MLS depth at all points in the slab 
            HashC<Index2dC, DListC<IntT> >& slab = surfacepts[ii];
            HashIterC<Index2dC, DListC<IntT> > itsurface(slab);
            for (; itsurface; itsurface++)
            {
               Index3dC svox(ii, itsurface.Key().Row().V(), itsurface.Key().Col().V());

               IntT voxRange0 = supportSize;
               IntT surfsize = (IntT)Pow(voxRange0*2+1,2) / 2;

               // Find the closest surface point to get the normal
               RealT minDist = minDist0;
               IntT vCount = 0;
               Vector3dC norm(0,0,0);
               Vector3dC r = m_voxels.WorldPosition(svox);
               DLIterC<IntT> its(itsurface.Data());
               for (; its; its++, vCount++)
               {
                  const VertexC& vert = surfaceVerts[ its.Data() ];
                  RealT d = vert.Position().SqrEuclidDistance(r);
                  if (d<minDist)
                  {
                     minDist = d;
                     norm = vert.Normal();
                  }
               }
               // Check there are sufficient vertices for MLS
               // Check that the voxel lies inside the depth layer
               if (vCount<surfsize)
               {
                  continue;
               }

               // Prune down vertices within the voxelrange to points where the normal agrees 
               // And compute the average normal for the set
               Vector3dC n(0,0,0);
               IntT pCount = 0;
               for (its.First(); its; its++)
               {
                  const VertexC& vert = surfaceVerts[ its.Data() ];
                  if (vert.Normal().Dot(norm) > 0.0)
                  {
                     pts[pCount++] = vert.Position();
                     n += vert.Normal();
                  }
               }
               // Check there are sufficient vertices for MLS
               if (pCount<surfsize) 
               {
                  continue;
               }
               n.MakeUnit();

               // Create a basis for the reference plane for MLS
               Vector3dC y = ( Abs(n.X()) > Abs(n.Y()) ) ?
                  ( ( Abs(n.X()) > Abs(n.Z()) ) ? Vector3dC(0,0,1) : Vector3dC(0,1,0) ) : 
               ( ( Abs(n.Y()) > Abs(n.Z()) ) ? Vector3dC(1,0,0) : Vector3dC(0,1,0) ) ;
               y = (n.Cross(y.Cross(n))).Unit();
               Vector3dC x = (y.Cross(n)).Unit();

               // Now fit a quadratic to the point set
               // Reset
               UIntT row, col;
               for(row=0; row<6; row++) 
               {
                  pATb[row] = 0.0;
                  for(col=row; col<6; col++)
                  {
                     pATA[row*6 + col] = 0.0;
                  }
               }    

               SArray1dIterC<Vector3dC> itp(pts);
               IntT v;
               for (v=0; v<pCount; v++, itp++)
               {
                  const Vector3dC& p = itp.Data();
                  Vector3dC off = p-r;
                  RealT f = n.Dot(off);
                  RealT xval = off.Dot(x);
                  RealT yval = off.Dot(y);
                  RealT x2 = xval*xval;
                  RealT xy = xval*yval;
                  RealT y2 = yval*yval;
                  RealT xy2 = xval*y2;
                  RealT x2y = x2*yval;
                  RealT x2y2 = xy*xy;
                  pATA[0] += 1;
                  pATA[1] += xval;
                  pATA[2] += yval;
                  pATA[3] += x2;
                  pATA[4] += xy;
                  pATA[5] += y2;
                  pATA[7] += x2;
                  pATA[8] += xy;
                  pATA[9] += x2*xval;
                  pATA[10] += x2y;
                  pATA[11] += xy2;
                  pATA[14] += y2;
                  pATA[15] += x2y;
                  pATA[16] += xy2;
                  pATA[17] += y2*yval;
                  pATA[21] += x2*x2;
                  pATA[22] += x2y*xval;
                  pATA[23] += x2y2;
                  pATA[28] += x2y2;
                  pATA[29] += xy*y2;
                  pATA[35] += y2*y2;
                  pATb[0] += f;
                  pATb[1] += xval*f;
                  pATb[2] += yval*f;
                  pATb[3] += x2*f;
                  pATb[4] += xy*f;
                  pATb[5] += y2*f;
               }
               // Make symmetric
               for(row=0; row<6; row++) 
               {
                  for(col=row+1; col<6; col++)
                  {
                     ATA[col][row] = ATA[row][col];
                  }
               }    
               if ( !SolveIP(ATA,ATb) ) 
               {
                  break;
               }

               deptharr[svox] = (FloatT)(ATb[0]);
            }

            // Now release the slab
            slab.Empty();
         }
      }

      if (verbose)
      {
        cerr << "100%\n";
      }

      // Filter the result to check for spurious depths
      if (filter)
      {
         if (verbose)
         {
            cerr << "Filtering...\n";
         }
         IndexRange3dC frame = arr.Frame().Erode();
         IndexRange3dIterC itvox(frame);
         itr = Array3dIterC<FloatT>(deptharr, frame);
         for (; itr; itr++, itvox++)
         {
            FloatT& depth = itr.Data();
            if (depth<1e5 && depth>-1e5)
            {
               SArray1dC<FloatT> neighbours(27);
               Array3dIterC<FloatT> itrr(deptharr, IndexRange3dC(itvox.Data(),1));
               for (IntT idx = 0; itrr; itrr++, idx++)
               {
                  neighbours[idx] = itrr.Data();
               }
               // Median filter
               neighbours.QuickSort(FloatGrEq);
               FloatT median = neighbours[13];
               if (Sign(median) != Sign(depth))
               {
                  depth = median;
               }
            }
         }
      }

      return ret;
   }

   // Extract the depth using MLS and reconstruct the surface
   TriMeshC
   ExtractVoxelSurfaceMLS(
      const ByteVoxelGridC& voxels, 
      IntT supportSize,
      bool filter,
      bool verbose)
   {
      MovingLeastSquaresC mls(voxels);
      FloatVoxelGridC depth = mls.ExtractDepth(supportSize, filter, verbose);
      if (verbose)
      {
         cerr << "Extracting mesh...\n";
      }
      return ExtractVoxelSurface(depth);
   }

}}
