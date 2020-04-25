// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlImageProc
//! rcsid="$Id$"
//! authors="George Matas, Radek Marik and Charles Galambos"
//! file="Ravl/Image/Processing/Edges/EdgeLink.cc"

#include "Ravl/Stack.hh"
#include "Ravl/Image/EdgeLink.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/Image/DrawFrame.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {

  EdgeLinkC HysteresisThreshold(const ImageC<RealT> &img,RealT maxThr,RealT minThr) {
    EdgeLinkC ret(img.Rectangle());
    Array2dIter2C<ByteT,RealT> it(ret,img);
    for(;it;it++)
      it.Data1() = (it.Data2() <= minThr) ? EDGE_PROC : EDGE_UNPROC;
    
    DrawFrame(ret,(ByteT) EDGE_PROC,ret.Frame());
    
    for(it.First();it;it++)
      if((it.Data2() > maxThr) && (((EdgeStateT) it.Data1()) == EDGE_UNPROC))
	ret.LabelContour(it.Index());
    
#if 1
    for(Array2dIterC<ByteT> it(ret);it;it++)
      if(*it == (UByteT)EDGE_UNPROC)
	*it = EDGE_PROC;
#endif
    
    return ret;
  }
  
  
  void EdgeLinkC::LabelContour(const Index2dC &spxl) {
    Index2dC pxl(spxl);
    StackC<Index2dC> stack;
    
    stack.Push(pxl);
    PutState(pxl,EDGE_INSTRING); // to avoid loops
    edgeCount++;
    NeighbourOrderT dir[8];
    
    while (!stack.IsEmpty()) {
      pxl = stack.Pop();
      IntT neighbours = 0;
      bool dr = true, ur = true, ul = true, dl = true;
      
      if (IsEdge(pxl.DownN()))  { dir[neighbours++]=NEIGH_DOWN;  dr = dl = false; }
      if (IsEdge(pxl.RightN())) { dir[neighbours++]=NEIGH_RIGHT; dr = ur = false; }
      if (IsEdge(pxl.UpN()))    { dir[neighbours++]=NEIGH_UP;    ur = ul = false; }
      if (IsEdge(pxl.LeftN()))  { dir[neighbours++]=NEIGH_LEFT;  ul = dl = false; }
      
      if (dr && IsEdge(pxl.LowerRightN()))   dir[neighbours++]=NEIGH_DOWN_RIGHT;
      if (ur && IsEdge(pxl.UpperRightN()))   dir[neighbours++]=NEIGH_UP_RIGHT;
      if (ul && IsEdge(pxl.UpperLeftN()))    dir[neighbours++]=NEIGH_UP_LEFT;
      if (dl && IsEdge(pxl.LowerLeftN()))    dir[neighbours++]=NEIGH_DOWN_LEFT;
      
      // put all neighbours not expanded yet stored in dir on stack.
      for(IntT k=0; k < neighbours; k++) {
	Index2dC pxlNext(pxl.Neighbour(dir[k]));
	if (IsNotExpanded(pxlNext)) {
	  stack.Push(pxlNext);
	  PutState(pxlNext,EDGE_INSTRING);
	  edgeCount++;
	}
      }
      
      // label edge point according to the number of neighbours.
      ByteT &val = (*this)[pxl];
      if(neighbours != 2) {
	//ONDEBUG(cerr << "Junction at " << pxl << " Neigh=" << neighbours << "\n");
	PutState(val,EDGE_JUNCT);
      } else {
	//ONDEBUG(cerr << "Edge     at " << pxl << "\n");
	PutDir(val, dir[0], FB_FORWARD);
	PutDir(val, dir[1], FB_BACKWARD);
      }
    }
  }

  //: List all edges in image
  
  SArray1dC<Index2dC> EdgeLinkC::ListEdges() {
    ONDEBUG(cerr << "EdgeLinkC::ListEdges(), Called. Max edges=" << edgeCount << "\n");
    SArray1dC<Index2dC> ret(edgeCount);
    if (edgeCount == 0)
      return ret;
    SArray1dIterC<Index2dC> rit(ret);
    for(Array2dIterC<ByteT> it(*this);it;it++) {
      if(GetState(*it) == EDGE_PROC) 
	continue;
      RavlAssert(rit);
      *rit = it.Index();
      rit++;
    }
    return ret;
  }

  //: List all edgels in image
  
  SArray1dC<EdgelC> EdgeLinkC::ArrayOfEdgels(const ImageC<RealT> & inDrIm, 
					     const ImageC<RealT> & inDcIm,  
					     const ImageC<RealT> & inGrad) const {
    ONDEBUG(cerr << "EdgeLinkC::ArrayOfEdgels() Called. Max edges=" << edgeCount << " Area=" << inGrad.Frame().Area() << "\n");
    SArray1dC<EdgelC> ret(edgeCount);
    if (edgeCount == 0)
      return ret;
    SArray1dIterC<EdgelC> rit(ret);
    for(Array2dIterC<ByteT> it(*this);it;it++) {
      if(GetState(*it) == EDGE_PROC)
	continue;
      Index2dC at = it.Index();
      RavlAssert(rit);
      *rit = EdgelC(at,inDcIm[at],inDrIm[at],inGrad[at]);
      rit++;
    }
    RavlAssert(!rit);
    //ONDEBUG(cerr << "EdgeLinkC::ListEdgels(), Processed=" << rit.Index() << "\n");
    return ret;
  }

  //: Generate a list of all edgels in image
  
  DListC<EdgelC> EdgeLinkC::ListOfEdgels(const ImageC<RealT> & inDrIm, 
					 const ImageC<RealT> & inDcIm,  
					 const ImageC<RealT> & inGrad) const {
    DListC<EdgelC> ret;
    for(Array2dIterC<ByteT> it(*this);it;it++) {
      if(GetState(*it) == EDGE_PROC)
	continue;
      Index2dC at = it.Index();
      ret.InsLast(EdgelC(at,inDcIm[at],inDrIm[at],inGrad[at]));
    }
    return ret;
  }


  //: Generate a set of linked edgels
  
  DListC<SArray1dC<EdgelC> > EdgeLinkC::LinkEdgels(const ImageC<RealT> & inDrIm, 
                                                   const ImageC<RealT> & inDcIm,  
                                                   const ImageC<RealT> & inGrad,
                                                   bool clearDir) {
    DListC<SArray1dC<EdgelC> > strings;
    
    SArray1dC<Index2dC> forward(edgeCount);
    SArray1dC<Index2dC> backward(edgeCount);
    
    Array2dIterC<ByteT> it(*this);
    for(;it;it++) {
      if(GetState(*it) != EDGE_INSTRING) 
	continue;
      Index2dC pxl = it.Index();
      DListC<Index2dC> string;
      int fi = 1;
      forward[0] = pxl;
      int bi = 0;
      
      PutState(*it, EDGE_PROC);
      // Go both ways along pixel chain.
      for(IntT dir_num=0; dir_num<=1; dir_num++) {
	NeighbourOrderT dir = GetDir(*it,(FBOrientationT) dir_num);
	Index2dC pxlNext = pxl.Neighbour(dir);
	// Step along pixel chain. 
	while(1) {
	  ByteT &pval = (*this)[pxlNext];
	  if(GetState(pval) != EDGE_INSTRING)
	    break;
	  PutState(pval,EDGE_PROC);
	  if (dir_num == 0)
	    forward[fi++] = pxlNext;
	  else
	    backward[bi++] = pxlNext;
	  NeighbourOrderT old_dir = RavlN::Reverse(dir);
	  dir = GetDir(pval, FB_FORWARD);
	  if (dir == old_dir) // Check we're not going back.
	    dir = GetDir(pval, FB_BACKWARD);
	  pxlNext.Step(dir);
	}
	
	// Append the junction point.
	// It can be EDGE_PROC if string is closed.
	if (GetState(pxlNext) == EDGE_JUNCT) {
	  if (dir_num==0) {
	    forward[fi++] = pxlNext;
          } else
	    backward[bi++] = pxlNext;
	}
      }
      // Generate edgel's
      
      SArray1dC<EdgelC> newEdges(bi+fi);
      IntT rawPos = 0;
      for(bi--;bi >= 0;bi--) {
        Index2dC at = backward[bi];
        newEdges[rawPos++] = EdgelC(at,inDcIm[at],inDrIm[at],inGrad[at]);
      }
      
      for(int i = 0;i < fi;i++) {
        Index2dC at = forward[i];
        newEdges[rawPos++] = EdgelC(at,inDcIm[at],inDrIm[at],inGrad[at]);
      }
      
      strings.InsLast(newEdges); 
    }
    if(clearDir) {
      // the upper six bits contain information about neighbouring edge direction
      // remove the information                                                
      for(it.First();it;it++) {
        // Remove the information about
        // the edge direction.
        *it &= 3;
        if (GetState(*it) == EDGE_JUNCT)
          PutState(*it,EDGE_PROC);
      }
    }
    
    return strings;
  }


  
  DListC<DListC<Index2dC> >  EdgeLinkC::LinkEdges(bool clearDir) {
    DListC<DListC<Index2dC> > strings;
    Array2dIterC<ByteT> it(*this);
    for(;it;it++) {
      if(GetState(*it) != EDGE_INSTRING) 
	continue;
      Index2dC pxl = it.Index();
      DListC<Index2dC> string;
      string.InsFirst(pxl);
      PutState(*it, EDGE_PROC);
      // Go both ways along pixel chain.
      for(IntT dir_num=0; dir_num<=1; dir_num++) {
	NeighbourOrderT dir = GetDir(*it,(FBOrientationT) dir_num);
	Index2dC pxlNext = pxl.Neighbour(dir);
	// Step along pixel chain. 
	while(1) {
	  ByteT &pval = (*this)[pxlNext];
	  if(GetState(pval) != EDGE_INSTRING)
	    break;
	  PutState(pval,EDGE_PROC);
	  if (dir_num == 0)
	    string.InsFirst(pxlNext);
	  else
	    string.InsLast(pxlNext);
	  NeighbourOrderT old_dir = RavlN::Reverse(dir);
	  dir = GetDir(pval, FB_FORWARD);
	  if (dir == old_dir) // Check we're not going back.
	    dir = GetDir(pval, FB_BACKWARD);
	  pxlNext.Step(dir);
	}
	
	// Append the junction point.
	// It can be EDGE_PROC if string is closed.
	if (GetState(pxlNext) == EDGE_JUNCT) {
	  if (dir_num==0)   
	    string.InsFirst(pxlNext);
	  else
	    string.InsLast(pxlNext);
	}
      }
      strings.InsLast(string); 
    }
    if(clearDir) {
      // the upper six bits contain information about neighbouring edge direction
      // remove the information                                                
      for(it.First();it;it++) {
        // Remove the information about
        // the edge direction.
        *it &= 3;
        if (GetState(*it) == EDGE_JUNCT)
          PutState(*it,EDGE_PROC);
      }
    }
    
    return strings;
  }
}
