// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FLINEPP_HEADER
#define RAVL_FLINEPP_HEADER 1
//! rcsid="$Id$"
//! lib=RavlMath
//! docentry="Ravl.API.Math.Geometry"
//! date="13/9/2002"
//! author="Charles Galambos"
//! file="Ravl/Math/Geometry/Euclidean/FLinePP.hh"

#include "Ravl/FPoint.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: Line in N dimensional space.
  
  template<unsigned int N>
  class FLinePPC {
  public:
    FLinePPC()
    {}
    //: Default constructor.
    // The line is undefined.
    
    FLinePPC(const FPointC<N> &start,const FPointC<N> &end)
    { point[0] = start; point[1] = end; }
    //: Construct from two points.
    
    FLinePPC(const FPointC<N> &start,const FVectorC<N> &direction)
    { point[0] =start; point[1] = start + direction; }
    //: Construct from a start point and a direction.
    
    const FPointC<N> & FirstPoint() const
    { return point[0]; }
    //: Returns the start point of the line segment.
    
    const FPointC<N> & SecondPoint() const
    { return point[1]; }
    //: Returns the end point of the line segment.

    FPointC<N> & FirstPoint()
    { return point[0]; }
    //: Returns the start point of the line segment.
    
    FPointC<N> & SecondPoint()
    { return point[1]; }
    //: Returns the end point of the line segment.
    
    FPointC<N> MidPoint() const
    { return (point[1] + point[0])/2.0; }
    //: Returns the mid point of the line segment.
    
    const FPointC<N> & P1() const
    { return point[0]; }
    //: Returns the start point of the line segment. 
    // It is equivalent to the function FirstPoint().
    
    const FPointC<N> & P2() const
    { return point[1]; }
    // Returns the start point of the line segment. 
    //: It is equivalent to the function SecondPoint().
    
    FPointC<N> & P1()
    { return point[0]; }
    //: Returns the start point of the line segment. 
    // It is equivalent to the function FirstPoint().
    
    FPointC<N> & P2()
    { return point[1]; }
    //: Returns the start point of the line segment. 
    // It is equivalent to the function SecondPoint().
    
    const FPointC<N> & operator[](const UIntT i) const {
      RavlAssertMsg(i == 0 || i == 1,"Index out of range 0..1"); 
      return point[i];
    }
    //: Returns the ith point.

    FPointC<N> & operator[](const UIntT i) {
      RavlAssertMsg(i == 0 || i == 1,"Index out of range 0..1"); 
      return point[i];
    }
    //: Returns the ith point.
    
    //:-------------------------
    // Geometrical computations.
    
    FLinePPC<N> operator+(const FVectorC<N> & v) const
    { return FLinePPC<N>(FPointC<N>(P1()+v),FPointC<N>(P2()+v)); }
    //: Returns the line segment translated into the new position.
    
    FLinePPC<N> & operator+=(const FVectorC<N> & v) {
      point[0] += v;
      point[1] += v;
      return *this;
    }
    //: Moves the line segment into the new position.
    // The operator is equivalent to the member function Translate().
    
    FLinePPC<N> & Translate(const FVectorC<N> & v)
    { return operator+=(v); }
    //: Moves the line segment into the new position.
    // The member function is equivalent to the operator+=.
    
    void Swap() {
      FPointC<N> tmp = point[0];
      point[0] = point[1];
      point[1] = tmp;
    }
    //: Swaps the end points of this
    
    FLinePPC<N> Swapped() const
    { return FLinePPC<N>(P2(), P1()); }
    //: Returns a line with swapped endpoints
    
    FVectorC<N> Vector() const
    { return point[1] - point[0]; } 
    //: Returns the direction of the line segment as a free vector.
    // The magnitude of the vector is the length of the line segment.
    
    FLinePPC<N> & FixStart(const FPointC<N> & p) {
      FVectorC<N> vec = point[1] - point[0];
      point[0] = p;
      point[1] = p + vec;
      return *this;
    }
    //: Translates the line segment to start in the point 'p'.
    
    FLinePPC<N> & FixEnd(const FPointC<N> & p) {
      FVectorC<N> vec = point[1] - point[0];
      point[0] = p - vec;
      point[1] = p;
      return *this;
    }
    //: Translates the line segment to end in the point 'p'.
    
    RealT Length() const
    { return point[0].EuclidDistance(point[1]); }
    //: Returns the length of the line in euclidian space.
    
    FPointC<N> Point(const RealT t) const
    { return FirstPoint() + Vector() * t; }
    //: Returns the point of the line: FirstPoint() + t * Vector().
    
    RealT ParClosest(const FPointC<N> &pnt) const {
      TFVectorC<RealT,N> v = Vector();
      RealT l2 = v.SumOfSqr();
      if (l2 == 0.0) throw ExceptionC("FLinePPC::ParClosest(): Cannot find line parameter for zero-length line");
      return v.Dot(pnt - point[0]) / l2;
    }
    //: Returns the parameter of the closest point on the line to 'pnt'.
    // Where 0 is at the start point and 1 is at the end. 
    
  protected:
    FPointC<N> point[2];
  };

  template<unsigned int N>
  inline
  ostream &operator<<(ostream &s,const FLinePPC<N> &dat) {
    s << dat.P1() << ' ' << dat.P2();
    return s;
  }
  
  template<unsigned int N>
  inline
  istream &operator>>(istream &s,FLinePPC<N> &dat) {
    s >> dat.P1() >> dat.P2();
    return s;
  }
  
  template<unsigned int N>
  inline
  BinOStreamC &operator<<(BinOStreamC &s,const FLinePPC<N> &dat) {
#if !defined VISUAL_CPP   
	  s << dat.P1() << dat.P2();
#else
	 RavlIssueError("BinOStreamC for FLinePP and VisualCPP needs fixing\n");
#endif
    return s;
  }
  
  template<unsigned int N>
  inline
  BinIStreamC &operator>>(BinIStreamC &s,FLinePPC<N> &dat) {
#if !defined VISUAL_CPP
    s >> dat.P1() >> dat.P2();
#else 
	RavlIssueError("BinIStreamC for FLinePP and VisualCPP needs fixing\n");
#endif
	return s;
  }
  
  
}


#endif
