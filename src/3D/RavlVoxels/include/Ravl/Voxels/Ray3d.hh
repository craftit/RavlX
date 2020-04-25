// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006-14, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_VOXEL_RAY3D_HEADER
#define RAVL_VOXEL_RAY3D_HEADER 1
//! lib=RavlVoxels

//
// Ray3dC provides a simple class to represent a ray in 3D given an origin and direction.
//

#include "Ravl/Vector3d.hh"

namespace RavlN { namespace VoxelsN {

  //:3D Ray class
  class Ray3dC
  {
  public:
    //:-
    // CONSTRUCTION/DESTRUCTION /////////////////////////////////////////////

    Ray3dC() {}
    //: Default Constructor

    Ray3dC(const Vector3dC& origin, const Vector3dC& direction, bool normalise = true)
      : m_origin(origin), m_direction(direction)
    {
       if (normalise)
       {
          m_direction.MakeUnit();
       }
    }
    //: Constructor from an origin and direction
    // NOTE: normalization is required for the distance scale to be correct, 
    // the user can supply a normalized direction vector

  public:
    //:-
    // ACCESS ///////////////////////////////////////////////////////////////

    Vector3dC& Origin() 
    { return m_origin; }
    //: Origin of the ray

    const Vector3dC& Origin() const 
    { return m_origin; }
    //: Origin of the ray

    Vector3dC& Direction()
    { return m_direction; }
    //: Direction of the ray

    const Vector3dC& Direction() const 
    { return m_direction; }
    //: Direction of the ray

  public:
    //:-
    // FUNCTIONS ////////////////////////////////////////////////////////////

    Vector3dC Point(RealT distance) const 
    { 
       return (m_origin + m_direction*distance); 
    }
    // Return a point along the ray by scaling the direction vector
    // This requires the direction vector to be normalized for the scale to be correct.

    RealT Distance2Point(const Vector3dC& point) const
    {	
       Vector3dC offset = (point - m_origin);
       Vector3dC rayOffset = m_direction * (offset.Dot(m_direction));
       Vector3dC difference = (offset-rayOffset);
       return difference.Magnitude(); 
    }
    //: Return the perpendicular distance from a point to the ray
    // This requires the direction vector to be normalized for the scale to be correct.

    Vector3dC ClosestPoint(const Vector3dC& point) const
    {
       Vector3dC offset = (point - m_origin);
       RealT distance = (offset.Dot(m_direction));
       return (m_origin + m_direction*distance); 
    }
    //: Return the closest point on the ray to a given point
    
    bool IntersectPlane(Vector3dC& point, const Vector3dC& planeN, const RealT planeOffset) const
    {	
       // Check that the line does not lie in the plane
       if (m_direction.Dot(planeN)<10e-6) 
       {
          return false;
       }
	    point = m_origin + m_direction * ( (planeOffset - m_origin.Dot(planeN)) / (m_direction.Dot(planeN)) ) ;
	    return true; 
    }
    //: Calculate the intersection between the ray and a plane oPoint.oPlaneN = dPlaneOffset

  protected:
    //:-
    // DATA /////////////////////////////////////////////////////////////////

    Vector3dC m_origin, m_direction;
    // Point on the ray and normalised direction

  };

}}

#endif
