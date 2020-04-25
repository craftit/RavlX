// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=Ravl3D
//! file="Ravl/3D/Mesh/HEMesh.cc"

#include "Ravl/3D/HEMesh.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/Hash.hh"
#include "Ravl/IntrDLIter.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace Ravl3DN {

  //: Destructor.
  // Needed to ensure faces are removed before vertices.
  
  HEMeshBodyC::~HEMeshBodyC() {
    while(!faces.IsEmpty()) {
      HEMeshFaceBodyC *face = &faces.First();
      face->DestroyFace();
      delete face;
    }
    vertices.Empty(); 
  }
  
  //: Insert face defined by vertices.
  
  HEMeshFaceC HEMeshBodyC::InsertFace(const SArray1dC<HEMeshVertexC> &vertices,HashC<Tuple2C<HEMeshVertexC,HEMeshVertexC> , HEMeshEdgeC> &edgeTab) {
    RavlAssert(vertices.Size() > 2);
    HEMeshFaceC face(true);
    ONDEBUG(cerr << "Inserting face " << face.Hash() << "\n"); 
    faces.InsLast(face.Body());
    
    SArray1dIterC<HEMeshVertexC> it(vertices);
    HEMeshVertexC lastVert = *it;
    HEMeshVertexC firstVert = *it;
    it++;
    HEMeshEdgeC firstEdge;
    for(;it;it++) {
      RavlAssert(it->IsValid());
      HEMeshEdgeC newEdge(it->Body(),face.Body());
      if(!firstEdge.IsValid())
	firstEdge = newEdge;
      else
	firstEdge.LinkBefore(newEdge);
      HEMeshEdgeC opedge = edgeTab[Tuple2C<HEMeshVertexC,HEMeshVertexC>(*it,lastVert)];
      if(opedge.IsValid()) {
	RavlAssertMsg(!opedge.HasPair(),"Can't represent nonplanar meshes.");
	opedge.SetPair(newEdge);
	newEdge.SetPair(opedge);
      }
      HEMeshEdgeC &me = edgeTab[Tuple2C<HEMeshVertexC,HEMeshVertexC>(lastVert,*it)];
      RavlAssert(!me.IsValid());
      me = newEdge;
      ONDEBUG(cerr << "Linking " << lastVert.Hash() << " To " << it->Hash() << "\n");
      it->SetEdge(me.Body()); // Make sure vertex has a valid edge.
      lastVert = *it;
    }
    HEMeshEdgeC newEdge(firstVert.Body(),face.Body());
    firstEdge.LinkBefore(newEdge);
    HEMeshEdgeC opedge = edgeTab[Tuple2C<HEMeshVertexC,HEMeshVertexC>(firstVert,lastVert)];
    if(opedge.IsValid()) {
      RavlAssertMsg(!opedge.HasPair(),"Can't represent nonplanar meshes.");
      opedge.SetPair(newEdge);
      newEdge.SetPair(opedge);
    }
    edgeTab[Tuple2C<HEMeshVertexC,HEMeshVertexC>(lastVert,firstVert)] = newEdge;
    firstVert.SetEdge(newEdge.Body()); // Make sure vertex has a valid edge.
    ONDEBUG(cerr << "Linking " << lastVert.Hash() << " To " << firstVert.Hash() << "\n");
    
    face.SetEdge(firstEdge);
    return face;
  }
  
  //: Insert a vertex on an edge.
  
  HEMeshVertexC HEMeshBodyC::InsertVertexOnEdge(HEMeshEdgeC edge) {
    HEMeshVertexC ret;
    //vertices.InsLast(nvert.Body());
    RavlAssertMsg(0,"HEMeshBodyC::InsertVertexOnEdge(), Not Implemented.");
    return ret;
  }

  //: Check mesh structure is consistant.
  // Returns false if an inconsistancy is found.
  
  bool HEMeshBodyC::CheckMesh(bool canBeOpen) const {
    bool ret = true;
    if(!canBeOpen) {
      // Can't do this check properly on an open mesh.
      ONDEBUG(cerr << "HEMeshBodyC::CheckMesh(), Checking vertexes. \n");
      for(IntrDLIterC<HEMeshVertexBodyC> vit(vertices);vit;vit++) {
	if (vit->HasEdge()) {
	  for(HEMeshVertexEdgeIterC it(*vit);it;it++) {
	    if(it->SourceVertex() != *vit) {
	      cerr << "HEMeshBodyC::CheckMesh(), Incorrect vertex pointer. \n";
	      ret = false;
	      //return false;
	    }
	    if(it->Vertex() == *vit) {
	      cerr << "HEMeshBodyC::CheckMesh(), Zero area face. \n";
	      ret = false;
	      //return false;
	    }
	  }
	}
      }
    }
    ONDEBUG(cerr << "HEMeshBodyC::CheckMesh(), Checking faces. \n");
    for(IntrDLIterC<HEMeshFaceBodyC> fit(faces);fit;fit++) {
      for(HEMeshFaceEdgeIterC efit(*fit);efit;efit++) {
	if(efit->Face() != *fit) {
	  cerr << "HEMeshBodyC::CheckMesh(), Bad face pointer found. \n";
	  ret = false;
	  //return false;
	}
	if(efit->HasPair()) {
	  if(!efit->Pair().HasPair()) {
	    cerr << "HEMeshBodyC::CheckMesh(), Half paired edge found.  \n";
	    ret = false;
	    //return false;
	  }
	  if(*efit != efit->Pair().Pair()) {
	    if(*efit == efit->Pair())
	      cerr << "HEMeshBodyC::CheckMesh(), Self paired edge found.  \n";
	    else
	      cerr << "HEMeshBodyC::CheckMesh(), Mis-paired edge found.  \n";
	    ret = false;
	    //	    return false;
	  }
	} else {
	  if(!canBeOpen) {
	    cerr << "HEMeshBodyC::CheckMesh(), Open face found. \n";
	    ret = false;
	    //return false;
	  }
	}
      }
    }
    return ret;
  }

  //: Write to binary stream.
  // NOT IMPLEMENTED.
  
  BinOStreamC &operator<<(BinOStreamC &strm,const HEMeshC &obj) {
    RavlAssertMsg(0,"not implemented");
    return strm;
  }
  
  //: Read from binary stream.
  // NOT IMPLEMENTED.
  
  BinIStreamC &operator>>(BinIStreamC &strm,HEMeshC &obj) {
    RavlAssertMsg(0,"not implemented");
    return strm;
  }
  
}
