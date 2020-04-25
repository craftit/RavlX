// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Container/Graph/HEMeshBase.cc"

#include "Ravl/HEMeshBase.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/Hash.hh"
#include "Ravl/HSet.hh"
#include "Ravl/IntrDLIter.hh"
#include "Ravl/HEMeshBaseFaceIter.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  //: Destructor.
  // Needed to ensure faces are removed before vertices.
  
  HEMeshBaseBodyC::~HEMeshBaseBodyC() {
    ONDEBUG(cerr << "HEMeshBaseBodyC::~HEMeshBaseBodyC(). \n");
    while(!faces.IsEmpty()) {
      HEMeshBaseFaceBodyC *face = &faces.First();
      face->DestroyFace();
      delete face;
    }
    vertices.Empty(); 
  }
  
  //: Insert face defined by vertices.
  
  HEMeshBaseFaceC HEMeshBaseBodyC::InsertFace(HEMeshBaseFaceC &face,
					      const SArray1dC<HEMeshBaseVertexC> &vertices,
					      HashC<Tuple2C<HEMeshBaseVertexC,HEMeshBaseVertexC> , HEMeshBaseEdgeC> &edgeTab) {
    RavlAssert(vertices.Size() > 2);
    RavlAssert(face.IsValid());
    ONDEBUG(cerr << "Inserting face " << face.Hash() << "\n"); 
    faces.InsLast(face.Body());
    
    SArray1dIterC<HEMeshBaseVertexC> it(vertices);
    HEMeshBaseVertexC lastVert = *it;
    HEMeshBaseVertexC firstVert = *it;
    it++;
    HEMeshBaseEdgeC firstEdge;
    for(;it;it++) {
      RavlAssert(it->IsValid());
      HEMeshBaseEdgeC newEdge = NewEdge(*it,face);
      RavlAssert(newEdge.Face().IsValid());
      if(!firstEdge.IsValid())
	firstEdge = newEdge;
      else
	firstEdge.LinkBefore(newEdge);
      HEMeshBaseEdgeC opedge = edgeTab[Tuple2C<HEMeshBaseVertexC,HEMeshBaseVertexC>(*it,lastVert)];
      if(opedge.IsValid()) {
	RavlAssertMsg(!opedge.HasPair(),"Can't represent nonplanar meshes.");
	opedge.SetPair(newEdge);
	newEdge.SetPair(opedge);
      }
      HEMeshBaseEdgeC &me = edgeTab[Tuple2C<HEMeshBaseVertexC,HEMeshBaseVertexC>(lastVert,*it)];
      RavlAssert(!me.IsValid());
      me = newEdge;
      ONDEBUG(cerr << "Linking " << lastVert.Hash() << " to " << it->Hash() << "\n");
      it->SetEdge(me.Body()); // Make sure vertex has a valid edge.
      lastVert = *it;
    }
    HEMeshBaseEdgeC newEdge = NewEdge(firstVert,face);
    RavlAssert(newEdge.Face().IsValid());
    firstEdge.LinkBefore(newEdge);
    HEMeshBaseEdgeC opedge = edgeTab[Tuple2C<HEMeshBaseVertexC,HEMeshBaseVertexC>(firstVert,lastVert)];
    if(opedge.IsValid()) {
      RavlAssertMsg(!opedge.HasPair(),"Can't represent nonplanar meshes.");
      opedge.SetPair(newEdge);
      newEdge.SetPair(opedge);
    }
    edgeTab[Tuple2C<HEMeshBaseVertexC,HEMeshBaseVertexC>(lastVert,firstVert)] = newEdge;
    firstVert.SetEdge(newEdge.Body()); // Make sure vertex has a valid edge.
    ONDEBUG(cerr << "Linking " << lastVert.Hash() << " To " << firstVert.Hash() << "\n");
    
    face.SetEdge(firstEdge);
    return face;
  }
  
  //: Insert a vertex on an edge.
  
  bool HEMeshBaseBodyC::InsertVertexInEdge(HEMeshBaseVertexC vert,HEMeshBaseEdgeC edge) {
    RavlAssert(vert.IsValid());
    RavlAssert(edge.IsValid());
    
    HEMeshBaseEdgeC edge1 = NewEdge(vert,edge.Face());
    edge.LinkBefore(edge1);
    vert.SetEdge(edge1.Body());
    
    if(edge.HasPair()) {
      HEMeshBaseEdgeC edgep = edge.Pair();
      HEMeshBaseEdgeC edge2 = NewEdge(vert,edgep.Face());
      edgep.LinkBefore(edge2);
      
      edge.SetPair(edge2);
      edge2.SetPair(edge);
      
      edge1.SetPair(edgep);
      edgep.SetPair(edge1);
    }
    
    return true;
  }

  //: Insert a vertex on an edge, assuming and maintaing a triangular mesh.
  
  bool HEMeshBaseBodyC::InsertVertexInEdgeTri(HEMeshBaseVertexC vert,HEMeshBaseEdgeC edge) {
    ONDEBUG(cerr << "HEMeshBaseBodyC::InsertVertexInEdgeTri(), Inserting vertex " << vert.Hash() << "\n"); 
    RavlAssert(vert.IsValid());
    RavlAssert(edge.IsValid());
    
    HEMeshBaseEdgeC edge1 = NewEdge(vert,edge.Face());
    edge.LinkBefore(edge1);
    vert.SetEdge(edge1.Body());
    
    HEMeshBaseEdgeC edge2;
    HEMeshBaseEdgeC edgep;
    if(edge.HasPair()) {
      edgep = edge.Pair();
      edge2 = NewEdge(vert,edgep.Face());
      edgep.LinkBefore(edge2);

      edge.SetPair(edge2);
      edge2.SetPair(edge);
      
      edge1.SetPair(edgep);
      edgep.SetPair(edge1);
    }
    
    SplitFace(edge1,edge.Next());
    if(edge.HasPair())
      SplitFace(edge2,edgep.Next());
    
    return true;
  }
  
  
  //: Insert a vertex into a face, link all vertexes already in the face to it.
  
  bool HEMeshBaseBodyC::InsertVertexInFace(HEMeshBaseVertexC vert,HEMeshBaseFaceC face) {
    ONDEBUG(cerr << "HEMeshBaseBodyC::InsertVertexInFace(), Called.  Vert=" << vert.Hash() << " Face=" << face.Hash() << "\n");
    // Put in first link to new vertex.
    
    HEMeshBaseEdgeC lastEdge,firstEdge;
    HEMeshBaseEdgeC nextEdge;
    HEMeshBaseEdgeC endEdge = face.Edge();
    HEMeshBaseEdgeC thisEdge = face.Edge();
    HEMeshBaseVertexC lastVert = thisEdge.Prev().Vertex();
    for(;thisEdge.IsValid();thisEdge = nextEdge) {
      RavlAssert(thisEdge.Face() == face);
      ONDEBUG(cerr << "HEMeshBaseBodyC::InsertVertexInFace(), Fixing edge " << thisEdge.Hash() << "\n");
      // Sort out next edge to process.
      nextEdge = thisEdge.Next();
      if(nextEdge == endEdge)
	nextEdge.Invalidate();
      
      // Create next face.
      //ONDEBUG(cerr << "Edge :" << thisEdge.Hash() << "\n");
      HEMeshBaseFaceC newFace;
      if(thisEdge != endEdge) { // Create a new face ?
	newFace = NewFace();
	faces.InsLast(newFace.Body());
      } else 
	newFace = face; // Use exiting face.
      newFace.SetEdge(thisEdge);
      
      thisEdge.SetSelfPointing();
      thisEdge.SetFace(newFace);
      HEMeshBaseEdgeC edge1 = NewEdge(vert,newFace);
      HEMeshBaseEdgeC edge2 = NewEdge(lastVert,newFace);
      if(lastEdge.IsValid()) {
	lastEdge.SetPair(edge2);
	edge2.SetPair(lastEdge);
      } else
	firstEdge = edge2;
      thisEdge.LinkAfter(edge1);
      thisEdge.LinkBefore(edge2); 
      lastEdge = edge1;
      lastVert = thisEdge.Vertex();
    }
    firstEdge.SetPair(lastEdge);
    lastEdge.SetPair(firstEdge);
    vert.SetEdge(lastEdge.Body());
    
    return true;
  }
  
  //: Twist an edge that lies between two faces.
  // Both 'from' and 'to' must be one of the faces adjacent to 'edge'.
  
  bool HEMeshBaseBodyC::TwistEdge(HEMeshBaseEdgeC edge,HEMeshBaseEdgeC vertFrom,HEMeshBaseEdgeC vertTo) {
    ONDEBUG(cerr << "HEMeshBaseBodyC::TwistEdge(), Called.  edge=" << edge.Hash() << " From=" << vertFrom.Hash() << " To=" << vertTo.Hash() << "\n");
    RavlAssert(vertFrom != vertTo);
    RavlAssert(edge != vertFrom);
    RavlAssert(edge != vertTo);
    
    RavlAssert(edge.HasPair());
    HEMeshBaseEdgeC edgep = edge.Pair();
    
    RavlAssert(vertFrom.Next() != vertTo);
    RavlAssert(vertFrom.Prev() != vertTo);
    RavlAssertMsg(vertFrom.Vertex() != vertTo.Vertex(),"HEMeshBaseBodyC::TwistEdge(), Called with identical source and destination vertices. ");
    
    RavlAssert(edgep != vertFrom);
    RavlAssert(edgep != vertTo);
    RavlAssert(!(vertFrom.Face() == vertTo.Face()));
    RavlAssert(vertFrom.Face() == edge.Face() || vertFrom.Face() == edgep.Face());
    RavlAssert(vertTo.Face() == edge.Face() || vertTo.Face() == edgep.Face());


    // Make sure vertex's don't refer directly to these edges.
    
    edge.Body().CorrectVertexEdgePtr();
    edgep.Body().CorrectVertexEdgePtr();
    
    //RavlAssert(CheckMesh(true));
    
    // Take edge out and loop all remaining edge's together.
    
    edgep.Body().CutPaste(edge.Body().Next(),edge.Body()); // Note: this removes 'edge' and leaves it self pointing.
    edgep.Unlink(); // Take out the edges we're moving.
    
    RavlAssert(edge.Body().IsSelfPointing());
    
    // Set new vertex's for edge.
    
    edge.Body().SetVertex(vertTo.Vertex().Body());
    //vertTo.Vertex().SetEdge(edge.Body());
    
    edgep.Body().SetVertex(vertFrom.Vertex().Body());
    //vertFrom.Vertex().SetEdge(edgep.Body());
    
    edge.Body().CutPaste(vertTo.Body().Next() ,vertFrom.Next().Body());
    vertTo.Body().LinkAft(edgep.Body());
    
    // Go around new faces correcting the face pointers. 
    
    HEMeshBaseFaceC face = edge.Face();
    face.SetEdge(edge);
    HEMeshBaseEdgeC at;
    for(at = edge.Next();at != edge;at = at.Next()) {
      ONDEBUG(cerr << " f1 edge " << at.Hash() << "\n");
      at.SetFace(face);
    }
    
    face = edgep.Face();
    face.SetEdge(edgep);
    for(at = edgep.Next();at != edgep;at = at.Next()) {
      ONDEBUG(cerr << " f2 edge " << at.Hash() << "\n");
      at.SetFace(face);
    }
    
    return true;
  }
  
  //: Split a face with a new edge.
  // Edges from and to must be on the same face. The new edge is placed
  // between the vertex's they point to.  The new edge is returned.
  
  HEMeshBaseEdgeC HEMeshBaseBodyC::SplitFace(HEMeshBaseEdgeC from,HEMeshBaseEdgeC to) {
    ONDEBUG(cerr << "HEMeshBaseBodyC::SplitFace(). From:" << from.Hash() << " To:" << to.Hash() << "\n"); 
    RavlAssert(from.IsValid());
    RavlAssert(to.IsValid());
    RavlAssert(from.Face() == to.Face());
    RavlAssert(to.Next() != from);
    RavlAssert(from.Next() != to);
    
    HEMeshBaseFaceC face1 = to.Face();
    HEMeshBaseFaceC face2 = NewFace();
    faces.InsLast(face2.Body());
    
    HEMeshBaseEdgeC edge1 = NewEdge(from.Vertex(),face1);
    HEMeshBaseEdgeC edge2 = NewEdge(to.Vertex(),face2);
    edge1.SetPair(edge2);
    edge2.SetPair(edge1);
    face2.SetEdge(edge2);
    face1.SetEdge(edge1); // Make sure face1 is pointing to a valid edge.
    
    edge2.Body().CutPaste(to.Body().Next(),from.Body().Next()); 
    to.LinkAfter(edge1);
    
    for(HEMeshBaseEdgeC at = edge2.Next();at != edge2;at = at.Next()) {
      ONDEBUG(cerr << "HEMeshBaseBodyC::SplitFace() Setting face for edge " << at.Hash() <<" to " << face2.Hash() << "\n"); 
      at.SetFace(face2);
    }
    //RavlAssert(CheckMesh(true));
    return edge2;
  }
  
  //: Remove an edge from between two faces, merge the faces into one.
  
  bool HEMeshBaseBodyC::OpenEdge(HEMeshBaseEdgeC edge) {
    ONDEBUG(cerr << "HEMeshBaseBodyC::OpenEdge() Called on " << edge.Hash() << "\n");
    RavlAssert(edge.IsValid());
    
    HEMeshBaseFaceC face1 = edge.Face();
    
    if(!edge.HasPair()) { // Just delete the face.
      delete &(face1.Body());
      face1.Invalidate();
      return true;
    }
    HEMeshBaseEdgeC edgep = edge.Pair();
    HEMeshBaseFaceC face2 = edgep.Face();
    
    // Prepair edges for removal.
    edge.Body().CorrectVertexEdgePtr();
    edgep.Body().CorrectVertexEdgePtr();
    
    if(face2 == face1) { // Same face either side of the edge ?
      delete &(edge.Body());
      delete &(edgep.Body());
      return true;
    }
    
    // Change all the face pointers for face we're going to remove.
    for(HEMeshBaseEdgeC at = edgep.Next();at != edgep;at = at.Next())
      at.SetFace(face1);
    
    edge.Body().CutPaste(edgep.Next().Body(),edgep.Body());
    face1.SetEdge(edge.Next()); // Make sure face uses a valid edge.
    
    // Delete stuff
    
    delete &(edge.Body());
    delete &(edgep.Body());
    
    ONDEBUG(cerr << "Deleting face " << face2.Hash() << "\n");
    face2.Body().edge = 0;
    delete &(face2.Body());
    
    return true;
  }

  //: Check mesh structure is consistant.
  // Returns false if an inconsistancy is found.
  
  bool HEMeshBaseBodyC::CheckMesh(bool canBeOpen) const {
    bool ret = true;
    if(!canBeOpen || 1) {
      // Can't do this check properly on an open mesh.
      ONDEBUG(cerr << "HEMeshBaseBodyC::CheckMesh(), Checking vertexes. \n");
      for(IntrDLIterC<HEMeshBaseVertexBodyC> vit(vertices);vit;vit++) {
	ONDEBUG(cerr << " Checking vertex " << HEMeshBaseVertexC(*vit).Hash() << " Edge=" << HEMeshBaseVertexC(*vit).FirstEdge().Hash() << "\n");
	for(HEMeshBaseVertexEdgeIterC it(*vit);it;it++) {
	  if(it->Vertex() == *vit) {
	    std::cerr << "HEMeshBaseBodyC::CheckMesh(), Warning: Zero area face with edge " << (*it).Hash() << " Vertex=" << it->Vertex().Hash() << "\n";
	    ret = false;
	  }
	  HEMeshBaseEdgeC edge = *it;
	  if(!edge.HasPair())
	    break; // Can only check edge's with pairs.
	  if(edge.Prev().Vertex() != edge.Pair().Vertex()) {
	    ret = false;
	    std::cerr << "HEMeshBaseBodyC::CheckMesh(), Mis-matched edge pair vertex's. \n";
	  }
	}
      }
    }
    ONDEBUG(cerr << "HEMeshBaseBodyC::CheckMesh(), Checking faces. \n");
    HSetC<HEMeshBaseFaceC> faceDone;
    HSetC<HEMeshBaseEdgeC> edgeDone; // Each edge should only be seen once.
    for(IntrDLIterC<HEMeshBaseFaceBodyC> fit(faces);fit;fit++) {
      HEMeshBaseFaceC curFace(*fit);
      ONDEBUG(cerr << " Checking face " << curFace.Hash() << "\n");
      if(faceDone.IsMember(curFace)) {
	cerr << "HEMeshBaseBodyC::CheckMesh(), Error: Infinite loop in face links. \n";
	ret = false;
	break;
      }
      faceDone += curFace;
      HSetC<HEMeshBaseVertexC> vertSeen;
      for(HEMeshBaseFaceEdgeIterC efit(*fit);efit;efit++) {
	HEMeshBaseEdgeC curEdge(*efit);
	if(curEdge.Next().Prev() != curEdge) {
	  std::cerr << "HEMeshBaseBodyC::CheckMesh(), Error: Bad next pointer in edge. \n";
	  ret = false;
	  break;	  
	}
	if(curEdge.Prev().Next() != curEdge) {
	  std::cerr << "HEMeshBaseBodyC::CheckMesh(), Error: Bad prev pointer in edge. \n";
	  ret = false;
	  break;
	}
	
	ONDEBUG(cerr << "  Checking Edge " << curEdge.Hash() << " Vertex=" <<  curEdge.Vertex().Hash());
#if DODEBUG
	if(curEdge.HasPair())
	  ONDEBUG(cerr << " Pair=" << curEdge.Pair().Hash());
	ONDEBUG(cerr << "\n");
#endif
	if(edgeDone.IsMember(curEdge)) {
	  std::cerr << "HEMeshBaseBodyC::CheckMesh(), Error: Infinite loop in edge links. \n";
	  ret = false;
	  break;
	}
	if(!curEdge.Vertex().IsValid()) {
	  std::cerr << "HEMeshBaseBodyC::CheckMesh(), Error: Edge with invalid vertex. \n";
	  ret = false;
	} else {
	  if(!curEdge.Vertex().FirstEdge().IsValid()) {
	    std::cerr <<"HEMeshBaseBodyC::CheckMesh(), Error: Vertex has invalid edge pointer. \n";
	    ret = false;
	  }
	  if(vertSeen.IsMember(curEdge.Vertex())) {
	    std::cerr << "HEMeshBaseBodyC::CheckMesh(), Error: Duplicate vertex in face. \n";
	    ret = false;
	  }
	  vertSeen += curEdge.Vertex();
	}
	edgeDone += curEdge;
	if(efit->Face() != *fit) {
	  std::cerr << "HEMeshBaseBodyC::CheckMesh(), Error: Bad face pointer found. " << HEMeshBaseFaceC(efit->Face()).Hash() << "\n";
	  ret = false;
	  //return false;
	}
	if(efit->HasPair()) {
	  if(!efit->Pair().HasPair()) {
	    std::cerr << "HEMeshBaseBodyC::CheckMesh(), Error: Half paired edge found.  \n";
	    ret = false;
	    //return false;
	  }
	  if(!efit->Pair().HasFace()) {
	    std::cerr << "HEMeshBaseBodyC::CheckMesh(), Error: Adjacent edge has no face.  \n";
	    ret = false;
	  }
	  if(*efit != efit->Pair().Pair()) {
	    if(*efit == efit->Pair())
	      std::cerr << "HEMeshBaseBodyC::CheckMesh(), Error: Self paired edge found.  \n";
	    else
	      std::cerr << "HEMeshBaseBodyC::CheckMesh(), Error: Mis-paired edge found.  \n";
	    ret = false;
	    //	    return false;
	  }
	  if(curEdge.Pair().Vertex() != curEdge.Prev().Vertex()) {
	    std::cerr << "HEMeshBaseBodyC::CheckMesh(), Error: Last vertex doesn't match egde pairs vertex. Other edge=" << curEdge.Pair().Hash() << "\n";
	    ret = false;
	  }
	  // Paranoia... double check 
	  if(curEdge.Pair().Prev().Vertex() != curEdge.Vertex()) {
	    std::cerr << "HEMeshBaseBodyC::CheckMesh(), Error: Pair's previous vertex doesn't match edge's vertex. Other edge=" << curEdge.Pair().Hash() << "\n";
	    ret = false;
	  }
	} else {
	  if(!canBeOpen) {
	    std::cerr << "HEMeshBaseBodyC::CheckMesh(), Error: Open face found. \n";
	    ret = false;
	    //return false;
	  }
	}	
      }
    }
    return ret;
  }

  //: Insert a new vertex into the mesh.
  
  HEMeshBaseVertexC HEMeshBaseBodyC::InsertVertex() {
    HEMeshBaseVertexC ret = NewVertex();
    vertices.InsLast(ret.Body());
    return ret;
  }

  //: Create a new face.
  
  HEMeshBaseFaceC HEMeshBaseBodyC::NewFace() {
    ONDEBUG(cerr << "HEMeshBaseBodyC::NewFace(), Called. \n");
    return HEMeshBaseFaceC(true);
  }
  
  //: Create a new face.
  
  HEMeshBaseVertexC HEMeshBaseBodyC::NewVertex() {
    ONDEBUG(cerr << "HEMeshBaseBodyC::NewVertex(), Called. \n");
    return HEMeshBaseVertexC(true);
  }
  
  //: Create a new face.
  
  HEMeshBaseEdgeC HEMeshBaseBodyC::NewEdge(HEMeshBaseVertexBodyC &vert,HEMeshBaseFaceBodyC &face) { 
    ONDEBUG(cerr << "HEMeshBaseBodyC::NewEdge(), Called. \n");
    return HEMeshBaseEdgeC(vert,face); 
  }
  
  //: Remove a vertex and assoicated faces from the mesh.
  // Note: This does not close the resulting face, so a hole will be left in the mesh.
  
  bool HEMeshBaseBodyC::DeleteVertex(HEMeshBaseVertexC vert,bool leaveHole) {
    RavlAssert(vert.IsValid());
    ONDEBUG(cerr << "HEMeshBaseBodyC::DeleteVertex(), Vert=" << vert.Hash() << " HasEdge=" << vert.HasEdge() << " \n");
    if(!leaveHole) {
      // Would be probably faster to write a dedicated routine to do this,
      // will do on request.
      while(1) {
	HEMeshBaseEdgeC firstEdge = vert.FirstEdge();
	if(!firstEdge.IsValid())
	  break;
	//cerr << "Deleting edge:" << firstEdge.Hash() << "\n";
	OpenEdge(firstEdge);
      }
      // Now it not connected delete it.
      delete &vert.Body();
      return true;
    }
    DListC<HEMeshBaseFaceC> delFaces;
    
    for(HEMeshBaseFaceIterC fit(faces);fit;fit++) {
      for(HEMeshBaseFaceEdgeIterC eit(*fit);eit;eit++) {
	if(eit->Vertex() == vert) {
	  delFaces.InsLast(*fit);
	  break;
	}
      }
    }
    
    //ONDEBUG(cerr << "HEMeshBaseBodyC::DeleteVertex(), Deleting Faces=" << delFaces.Size() << " Total=" << faces.Size() << "\n");
    for(DLIterC<HEMeshBaseFaceC> it2(delFaces);it2;it2++) {
      //ONDEBUG(cerr << "HEMeshBaseBodyC::DeleteVertex(), Face=" << it2.Data().Hash() << " Valid=" << it2.Data().IsValid() << " Face=" << ((void *) &(it2->Body())) << "\n");
      delete &(it2->Body());
    }
    delete &vert.Body();

    // Tidy up any dangling vertex to edge pointers.
    //ONDEBUG(cerr << "HEMeshBaseBodyC::DeleteVertex(), Total left=" << faces.Size() << "\n");
    
    for(HEMeshBaseFaceIterC fit(faces);fit;fit++) {
      for(HEMeshBaseFaceEdgeIterC eit(*fit);eit;eit++)
	eit->Vertex().SetEdge((*(eit)).Body());
    }
    
    return true;
  }

  
}
