// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! author="Charles Galambos"
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Euclidean/2D/DelaunayTriangulation2d.cc"

#include "Ravl/DelaunayTriangulation2d.hh"
#include "Ravl/THEMeshFaceIter.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/Array1dIter3.hh"
#include "Ravl/Array1dIter2.hh"
#include "Ravl/Array1dIter.hh"
#include "Ravl/Circle2d.hh"
#include "Ravl/HSet.hh"
#include "Ravl/LinePP2d.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  static bool testLegal(const Point2dC &pi,const Point2dC &pj,const Point2dC &pk,const Point2dC &pr) {
    Circle2dC cir;
    //ONDEBUG(cerr << "testLegal(), Fit. i=" << pi << " j=" << pj << " k=" << pk << " r=" << pr);
    if(!cir.Fit(pi,pj,pk)) {
      //ONDEBUG(cerr << " Fit failed. \n");
      return true;
    }
    //ONDEBUG(cerr << " IsInside=" << cir.IsInside(pr) << " \n");
    return !cir.IsInside(pr);
  }
  
  static void LegaliseEdge(HEMesh2dC &mesh,THEMeshEdgeC<Point2dC> edge) {
    ONDEBUG(cerr << "LegaliseEdge(), Called. Edge=" << edge.Hash() << " \n");
    
    if(!edge.HasPair())
      return ; // Must be legal, nothing to change it with!
    
    THEMeshVertexC<Point2dC> pi = edge.Prev().Vertex();
    THEMeshVertexC<Point2dC> pj = edge.Vertex();    
    THEMeshVertexC<Point2dC> pr = edge.Next().Vertex(); // Find point being inserted.
    THEMeshVertexC<Point2dC> pk = edge.Pair().Next().Vertex();
    
    if(testLegal(pi.Data(),pj.Data(),pk.Data(),pr.Data()))
      return ; // Edge is legal, we're done.
    
    // Twist edge in the face.
    mesh.TwistEdge(edge,edge.Next(),edge.Pair().Next());
    RavlAssert(mesh.CheckMesh(true));
    
    // Check resulting triangles are ok.
    RavlAssert(edge.Next().Next().Vertex() == pr);
    
    THEMeshEdgeC<Point2dC> tmp = edge.Pair().Prev();
    LegaliseEdge(mesh,edge.Next());
    
    RavlAssert(tmp.Next().Vertex() == pr);
    LegaliseEdge(mesh,tmp);
  }
  
  //: Find face containing point 'pnt'.
  // Returns an invalid handle if none found.
  
  static THEMeshFaceC<Point2dC> FindFace(HEMesh2dC &mesh,HEMeshBaseEdgeC &me,const Point2dC &pnt) {
    ONDEBUG(cerr << "FindFace(), Point= " << pnt << "\n");
    for(THEMeshFaceIterC<Point2dC> it(mesh.Faces());it;it++) {
      //ONDEBUG(cerr << "FindFace(), Face= " << (*it).Hash() << "\n");      
      RavlAssert((*it).IsValid());
      THEMeshFaceEdgeIterC<Point2dC> eit(*it);
      for(;eit;eit++) {
	LinePP2dC line((*eit).Prev().Vertex().Data(),eit->Vertex().Data());
	//ONDEBUG(cerr << "FindFace(), " << (*eit).Prev().Vertex().Data() << " " << eit->Vertex().Data() << " -> " << !line.IsPointToRightOn(pnt) << "\n");
	if(!line.IsPointToRightOn(pnt))
	  break;
	if(line.IsPointIn(pnt)) {
	  ONDEBUG(cerr << "Point is on a line. Point=" << pnt << " Line=" << line << "\n");
	  me = *eit;
	  return *it;
	}
      }
      if(!(eit)) { 
	//ONDEBUG(cerr << "FindFace(), Found!\n");
	return *it; // Found it !
      } //else
	//ONDEBUG(cerr << "FindFace(), Reject. \n");
	// Keep trying....
    }
    return THEMeshFaceC<Point2dC>();
  }
  
  //: Create a delaunay triangulation of the given set of points.
  
  HEMesh2dC DelaunayTriangulation(const SArray1dC<Point2dC> &points) { 
    Array1dC<IntT> idMap;
    return DelaunayTriangulation(Array1dC<Point2dC>(points),idMap); 
  }
  
  
  //: Create a delaunay triangulation of the given set of points.
  // The vertices in the created mesh are in same order as the
  // appear in 'points'.
  
  HEMesh2dC DelaunayTriangulation(const Array1dC<Point2dC> &points) {
    Array1dC<IntT> idMap;
    return DelaunayTriangulation(points,idMap);
  }
  
  
  //: Create a delaunay triangulation of the given set of points.
  
  HEMesh2dC DelaunayTriangulation(const Array1dC<Point2dC> &points,Array1dC<IntT> &idMap) {
    HEMesh2dC ret(true);
    ONDEBUG(cerr << "DelaunayTriangulation(), Called. \n");
    
    Array1dIterC<Point2dC> xit(points);
    if(!xit)
      return ret;
    RealRange2dC box(*xit,0);
    for(xit++;xit;xit++)
      box.Involve(*xit);
    
    Point2dC off((box.RowRange().Size()+1) * 100,(box.ColRange().Size() + 1) * 100 );
    
    // Create initial face.
    
    SArray1dC<HEMeshBaseVertexC> tempFace(3);
    tempFace[0] = ret.InsertVertex(box.Origin() - off);
    tempFace[1] = ret.InsertVertex(Point2dC(box.End()[0],box.End()[1] + off[1]));
    tempFace[2] = ret.InsertVertex(Point2dC(box.End()[0] + off[0],box.End()[1]));
    
    SizeC vertIds = ret.NumberOfVertexIds();
    idMap = Array1dC<IntT>(vertIds,vertIds+points.Size());
    
    HashC<Tuple2C<HEMeshBaseVertexC,HEMeshBaseVertexC> , HEMeshBaseEdgeC> edgeTab;
    ret.InsertFace(tempFace,edgeTab); // Insert initial face.
    
    SArray1dC<HEMeshBaseVertexC> vertices(points.Size());
    // Start inserting points,
    
    for(Array1dIter2C<Point2dC,HEMeshBaseVertexC> it(points,vertices);it;it++) {
      // Insert new vertex.
      THEMeshVertexC<Point2dC> vertex = ret.InsertVertex(it.Data1());
      idMap[vertex.Id()] = it.Index().V();
      
      it.Data2() = vertex;
      HEMeshBaseEdgeC me;
      THEMeshFaceC<Point2dC> face = FindFace(ret,me,it.Data1());
#if 0
      if(!face.IsValid())
	face = firstFace;
#else
      RavlAssert(face.IsValid());
#endif
      if(!me.IsValid()) { // Insert vertex in face.
	ONDEBUG(cerr << "HEMesh2dC DelaunayTriangulation(), Inserting vertex in face. \n");
	RavlAssert(face.Sides() == 3);
	THEMeshFaceEdgeIterC<Point2dC> eit(face);
	THEMeshEdgeC<Point2dC> e1 = *eit; eit++;
	THEMeshEdgeC<Point2dC> e2 = *eit; eit++;
	THEMeshEdgeC<Point2dC> e3 = *eit; 
	
	ret.InsertVertexInFace(vertex,face);
	
	RavlAssert(ret.CheckMesh(true));
	
	LegaliseEdge(ret,e1);	
	LegaliseEdge(ret,e2);
	LegaliseEdge(ret,e3);
	
      } else { // Insert vertex on edge.
	ONDEBUG(cerr << "HEMesh2dC DelaunayTriangulation(), Inserting vertex on edge. \n");
	
	ret.InsertVertexInEdgeTri(vertex,me);
	
	RavlAssert(ret.CheckMesh(true));
	
	THEMeshVertexEdgeIterC<Point2dC> it(vertex);
	RavlAssert(it);
	THEMeshEdgeC<Point2dC> e1 = (*it).Prev(); it++;
	THEMeshEdgeC<Point2dC> e2 = (*it).Prev(); it++;
	THEMeshEdgeC<Point2dC> e3 = (*it).Prev(); it++;
	THEMeshEdgeC<Point2dC> e4 = (*it).Prev(); 
	
	LegaliseEdge(ret,e1);
	LegaliseEdge(ret,e2);
	LegaliseEdge(ret,e3);
	LegaliseEdge(ret,e4);
      }
    }
    RavlAssert(ret.CheckMesh(true));
#if 1
    for(int i = 0;i < 3;i++) {
      ret.DeleteVertex(tempFace[i],true);
      RavlAssert(ret.CheckMesh(true));
    }
#endif
    return ret;
  }

  //: Test if mesh is a delaunay triangulation. 
  
  bool IsDelaunayTriangulation(const HEMesh2dC &mesh) {
    ONDEBUG(cerr << "IsDelaunayTriangulation(), Called. \n");
    for(THEMeshFaceIterC<Point2dC> fit(mesh.Faces());fit;fit++) {
      for(THEMeshFaceEdgeIterC<Point2dC> eit(*fit);eit;eit++) {
	if(!eit.Data().HasPair()) // On the edge of the mesh ?
	  continue;
	THEMeshEdgeC<Point2dC> edge = eit.Data();
	if(edge.Vertex() > edge.Next().Vertex()) { // Only have to check one edge from each pair.
	  THEMeshVertexC<Point2dC> pi = edge.Prev().Vertex();
	  THEMeshVertexC<Point2dC> pj = edge.Vertex();
	  THEMeshVertexC<Point2dC> pr = edge.Next().Vertex(); // Find point being inserted.
	  THEMeshVertexC<Point2dC> pk = edge.Pair().Next().Vertex();
	  if(!testLegal(pi.Data(),pj.Data(),pk.Data(),pr.Data()))
	    return false;
	}
      }
    }
    return true;
  }

  
}
