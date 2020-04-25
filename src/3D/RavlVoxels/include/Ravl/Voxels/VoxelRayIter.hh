// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006-14, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_VOXEL_VOXELRAYITER_HEADER
#define RAVL_VOXEL_VOXELRAYITER_HEADER 1
//! lib=RavlVoxels

// VoxelRayIterC is an iterator for voxels along a ray in a VoxelGridC
// A ray is intersected with the voxel volume and the iterator moves along adjacent voxels on the ray.
// Use:
//    VoxelRayIterC it(voxelset, ray);
//    for (; it; it++)
//    {
//       Index3dC index = it.Data();
//    }

#include "Ravl/Voxels/VoxelGrid.hh"
#include "Ravl/Voxels/Ray3d.hh"

namespace RavlN { namespace VoxelsN {

  class VoxelRayIterC 
  {
  public:
    //:-
    // CONSTRUCTION/DESTRUCTION /////////////////////////////////////////////
    VoxelRayIterC()
      : at(0,0,0), m_start(0,0,0), m_delta(0,0,0), m_range(-1,-1,-1)
    {}
    //: Default constructor.

    template<class DataTypeT>
    VoxelRayIterC(const VoxelGridC<DataTypeT>& voxels, const Ray3dC& ray)
      : at(0,0,0), m_start(0,0,0), m_delta(0,0,0), m_range(-1,-1,-1)
    { 
       IntersectVoxelSet(voxels, ray, voxels.Array().Frame()); 
       First(); 
    }
    //:  Construct from a voxel set and ray in world coordinates

    template<class DataTypeT>
    VoxelRayIterC(const VoxelGridC<DataTypeT>& voxels, const Ray3dC& ray, const IndexRange3dC& subRegion)
      : at(0,0,0), m_start(0,0,0), m_delta(0,0,0), m_range(-1,-1,-1)
    { 
       IntersectVoxelSet(voxels, ray, subRegion); 
       First(); 
    }
    //:  Construct from a voxel set and ray over a specific range in the voxel set

    template<class DataTypeT>
    VoxelRayIterC(const VoxelGridC<DataTypeT>& voxels, const Index3dC& start, const Index3dC& end);
    //:  Construct from a voxel set and start and end voxel indices
    // Note the start point must lie within the set for this to be a valid iterator

  public:
    //:-
    // ACCESS ///////////////////////////////////////////////////////////////
    
    bool First() {
      m_at = m_start;
      at = Index3dC(IndexC(m_start.X()+0.5),IndexC(m_start.Y()+0.5),IndexC(m_start.Z()+0.5));
      return IsElm();
    }
    //: Goto first index.
    
    const Index3dC& Data() const
    { 
       return at; 
    }
    //: Access current position.
    
    const Index3dC& operator*() const
    { 
       return at; 
    }
    //: Access current position.

    const Index3dC* operator->() const
    { 
       return &at; 
    }
    //: Access current position.
    
    RealT Distance() const
    {
       return (sqrt(m_at.SqrEuclidDistance(m_rayO))*m_voxelSize); 
    }
    //: Access the current distance along the ray

    void Next() 
    {
      m_at += m_delta;
      at = Index3dC(IndexC(m_at.X()+0.5),IndexC(m_at.Y()+0.5),IndexC(m_at.Z()+0.5));
    }
    //: Goto next index.
    // Returns true if on the same line.
    
    void operator++(int)
    { 
       Next(); 
    }
    //: Goto next index.
    
    bool IsElm() const
    { 
       return m_range.Contains(at); 
    }
    //: At a valid index
    
    operator bool() const
    { 
       return IsElm(); 
    }
    //: At a valid index

  protected:
    //:-
    // FUNCTIONS ////////////////////////////////////////////////////////////
    
     template<class DataTypeT>
     bool IntersectVoxelSet(const VoxelGridC<DataTypeT>& voxels, const Ray3dC& ray, const IndexRange3dC& subRegion);
    // Intersect the ray with the voxel set and generate the iterator
    
  protected:
    //:-
    // DATA /////////////////////////////////////////////////////////////////

    Index3dC at;            // The current voxel
    Vector3dC m_rayO;      // The origin of the ray in voxel index coordinates
    Vector3dC m_at;        // The current voxel in voxel index coordinates
    Vector3dC m_start;     // The start point for iteration in voxel index coordinates
    Vector3dC m_delta;     // The step size in iteration in voxel index coordinates
    IndexRange3dC m_range; // The voxel range
    RealT m_voxelSize;     // The size of the voxels
  };

  // Construct from a voxel set and start and end voxel indices
  // Note the start point must lie within the set for this to be a valid iterator
  template<class DataTypeT>
  VoxelRayIterC::VoxelRayIterC(const VoxelGridC<DataTypeT>& voxels, const Index3dC& start, const Index3dC& end)
    : at(0,0,0), m_start(0,0,0), m_delta(0,0,0), m_range(-1,-1,-1)
  {
    m_start = Vector3dC(start.I(),start.J(),start.K());
    m_rayO = m_start;
    m_range = voxels.Array().Frame();
    // Set the voxel index step
    IntT iX=end.I().V()-start.I().V(), iY=end.J().V()-start.J().V(), iZ=end.K().V()-start.K().V();
    RealT dNXabs=(RealT)abs(iX), dNYabs=(RealT)abs(iY), dNZabs=(RealT)abs(iZ);
    RealT dBase = (dNXabs>dNYabs && dNXabs>dNZabs) ? dNXabs : 
                  ( (dNYabs>dNXabs && dNYabs>dNZabs) ? dNYabs : dNZabs );
    m_delta = Vector3dC((RealT)iX/dBase, (RealT)iY/dBase, (RealT)iZ/dBase);
    m_voxelSize = voxels.voxelSize();
    First();
  }


  // Intersect the ray with the voxel set and generate the iterator
  template<class DataTypeT>
  bool VoxelRayIterC::IntersectVoxelSet(const VoxelGridC<DataTypeT>& voxels, const Ray3dC& ray, const IndexRange3dC& subRegion)
  {
    // Set the range for the ray in the voxel set
    IndexRange3dC oRange = subRegion;
    oRange.ClipBy(voxels.Array().Frame());
    // Transform the ray into the voxel coordinate frame
    Vector3dC voxelO = voxels.R()*ray.Origin() + voxels.t();
    Vector3dC voxelN = voxels.R()*ray.Direction();

    // Clip the ray against each pair of bounding planes
    RealT dStart=0.0; // Find the intersection into the set only
    RealT voxelSize = voxels.VoxelSize();

    // Clip against +/- x
    if (fabs(voxelN.X()) > 1E-6) {
      IndexC oMinXIndex = oRange.Range1().Min();
      IndexC oMaxXIndex = oRange.Range1().Max();
      RealT dXmin = ((RealT)oMinXIndex.V())*voxelSize;
      RealT dXmax = ((RealT)oMaxXIndex.V())*voxelSize;
      RealT dMinIntersect = (dXmin-voxelO.X())/voxelN.X();
      RealT dMaxIntersect = (dXmax-voxelO.X())/voxelN.X();
      if (voxelN.X()>0.0) {
        if (dMaxIntersect<0.0) return false; // Ray starts outside bounds
        if (dMinIntersect>dStart) dStart = dMinIntersect;
      } else {
        if (dMinIntersect<0.0) return false; // Ray starts outside bounds
        if (dMaxIntersect>dStart) dStart = dMaxIntersect;
      }
    }

    // Clip against +/- y
    if (fabs(voxelN.Y()) > 1E-6) {
      IndexC oMinYIndex = oRange.Range2().Min();
      IndexC oMaxYIndex = oRange.Range2().Max();
      RealT dYmin = ((RealT)oMinYIndex.V())*voxelSize;
      RealT dYmax = ((RealT)oMaxYIndex.V())*voxelSize;
      RealT dMinIntersect = (dYmin-voxelO.Y())/voxelN.Y();
      RealT dMaxIntersect = (dYmax-voxelO.Y())/voxelN.Y();
      if (voxelN.Y()>0.0) {
        if (dMaxIntersect<0.0) return false; // Ray starts outside bounds
        if (dMinIntersect>dStart) dStart = dMinIntersect;
      } else {
        if (dMinIntersect<0.0) return false; // Ray starts outside bounds
        if (dMaxIntersect>dStart) dStart = dMaxIntersect;
      }
    }

    // Clip against +/- z
    if (fabs(voxelN.Z()) > 1E-6) {
      IndexC oMinZIndex = oRange.Range3().Min();
      IndexC oMaxZIndex = oRange.Range3().Max();
      RealT dZmin = ((RealT)oMinZIndex.V())*voxelSize;
      RealT dZmax = ((RealT)oMaxZIndex.V())*voxelSize;
      RealT dMinIntersect = (dZmin-voxelO.Z())/voxelN.Z();
      RealT dMaxIntersect = (dZmax-voxelO.Z())/voxelN.Z();
      if (voxelN.Z()>0.0) {
        if (dMaxIntersect<0.0) return false; // Ray starts outside bounds
        if (dMinIntersect>dStart) dStart = dMinIntersect;
      } 
      else 
      {
        if (dMinIntersect<0.0) return false; // Ray starts outside bounds
        if (dMaxIntersect>dStart) dStart = dMaxIntersect;
      }
    }

    // Set the start point in voxel index coordinates
    Vector3dC startN(voxelN*dStart);
    m_start = (voxelO + startN)/voxelSize;
    // Set the origin of the ray in voxel index coordinates

    m_rayO = voxelO/voxelSize;
    // Set the voxel size
    m_voxelSize = voxelSize;
    // Set the feasible range in voxel index coordinates
    m_range = oRange;
    // Set-up how to iterate in the voxel set
    RealT dNXabs=fabs(voxelN.X()), dNYabs=fabs(voxelN.Y()), dNZabs=fabs(voxelN.Z());
    RealT dBase = (dNXabs>dNYabs && dNXabs>dNZabs) ? dNXabs : ( (dNYabs>dNXabs && dNYabs>dNZabs) ? dNYabs : dNZabs );
    m_delta = Vector3dC(voxelN.X()/dBase, voxelN.Y()/dBase, voxelN.Z()/dBase);

    // Done
    return true;
  }

}}


#endif
