// This file is part of RAVL, Recognition And Vision Library 
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlMath

// Based on code submitted to flipcode by John W. Ratcliff (jratcliff@verant.com) July 22, 2000

#include "Ravl/Triangulate2d.hh"
#include "Ravl/Collection.hh"

namespace RavlN {
  static const RealT g_epsilon=1e-12;
  
  //! Compute area of polygon
  
  RealT PolygonArea(const SArray1dC<Point2dC> &polygon) {
    const int n = polygon.Size();
    RealT a=0.0;
    for(int p=n-1,q=0; q<n; p=q++)
      a += polygon[p].Row()*polygon[q].Col() - polygon[q].Row()*polygon[p].Col();
    return a*0.5f;
  }
  
  // InsideTriangle decides if a point P is Inside of the triangle
  // defined by A, B, C.
  
  static bool InsideTriangle(RealT Ax, RealT Ay,
			     RealT Bx, RealT By,
			     RealT Cx, RealT Cy,
			     RealT Px, RealT Py)
    
  {
    RealT ax, ay, bx, by, cx, cy, apx, apy, bpx, bpy, cpx, cpy;
    RealT cCROSSap, bCROSScp, aCROSSbp;
    
    ax = Cx - Bx;  ay = Cy - By;
    bx = Ax - Cx;  by = Ay - Cy;
    cx = Bx - Ax;  cy = By - Ay;
    apx= Px - Ax;  apy= Py - Ay;
    bpx= Px - Bx;  bpy= Py - By;
    cpx= Px - Cx;  cpy= Py - Cy;
    
    aCROSSbp = ax*bpy - ay*bpx;
    cCROSSap = cx*apy - cy*apx;
    bCROSScp = bx*cpy - by*cpx;
    
    return ((aCROSSbp >= 0.0f) && (bCROSScp >= 0.0f) && (cCROSSap >= 0.0f));
  }
  
  static bool Snip(const SArray1dC<Point2dC> &polygon,int u,int v,int w,int n,const SArray1dC<int> &V)
  {
    int p;
    RealT Ax, Ay, Bx, By, Cx, Cy, Px, Py;
    
    Ax = polygon[V[u]].Row();
    Ay = polygon[V[u]].Col();
    
    Bx = polygon[V[v]].Row();
    By = polygon[V[v]].Col();
    
    Cx = polygon[V[w]].Row();
    Cy = polygon[V[w]].Col();
    
    if ( g_epsilon > (((Bx-Ax)*(Cy-Ay)) - ((By-Ay)*(Cx-Ax))) ) return false;
    
    for (p=0;p<n;p++) {
      if( (p == u) || (p == v) || (p == w) ) 
	continue;
      Px = polygon[V[p]].Row();
      Py = polygon[V[p]].Col();
      if (InsideTriangle(Ax,Ay,Bx,By,Cx,Cy,Px,Py)) 
	return false;
    }
    
    return true;
  }
  
  //! Triangulate a polygon given by the set of points in 'polygon'
  //! Returned are sets of 3 indexs into the original array.
  
  bool PolygonTriangulation(const SArray1dC<Point2dC> &polygon,SArray1dC<Index3dC> &triangles) {
    int n = polygon.Size();
    triangles = SArray1dC<Index3dC>();
    
    // We need 3 or more points to form a triangulation.
    if ( n < 3 ) return false;
    
    CollectionC<Index3dC> result(polygon.Size() - 2);
    
    SArray1dC<int> V(n);
    
    // We want a counter-clockwise polygon in V
    
    if ( 0.0 < PolygonArea(polygon) )
      for (int v=0; v<n; v++) V[v] = v;
    else
      for(int v=0; v<n; v++) V[v] = (n-1)-v;
    
    int nv = n;
    
    //  remove nv-2 Vertices, creating 1 triangle every time.
    int count = 2*nv;   // error detection.
    
    for(int m=0, v=nv-1; nv>2; ) {
      // if we loop, it is probably a non-simple polygon 
      if (0 >= (count--)) {
	// Triangulate: ERROR - probable bad polygon!
	return true;
      }
      
      // three consecutive vertices in current polygon, <u,v,w> 
      int u = v  ; if (nv <= u) u = 0;     // previous 
      v = u+1; if (nv <= v) v = 0;         // new v    
      int w = v+1; if (nv <= w) w = 0;     // next     
      
      if ( Snip(polygon,u,v,w,nv,V) ) {
	int a,b,c,s,t;
	
	// true names of the vertices 
	a = V[u]; 
	b = V[v]; 
	c = V[w];
	
	// output Triangle 
	result.Append(Index3dC(a,b,c));
      
	m++;
	
	// remove v from remaining polygon 
	for(s=v,t=v+1;t<nv;s++,t++) 
	  V[s] = V[t]; 
	
	nv--;
	
	// resest error detection counter 
	count = 2*nv;
      }
    }
    
    triangles = result.Array();
    return true;
  }
  
}
