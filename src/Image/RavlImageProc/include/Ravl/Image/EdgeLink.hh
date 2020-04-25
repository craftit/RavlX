// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_EDGELINK_HEADER
#define RAVLIMAGE_EDGELINK_HEADER 1
///////////////////////////////////////////////////////
//! rcsid="$Id$"
//! authors="George Matas, Radek Marik, Charles Galambos"
//! date="04/07/2000"
//! docentry="Ravl.API.Images.Edges"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Edges/EdgeLink.hh"

#include "Ravl/Image/Image.hh"
#include "Ravl/Image/Edgel.hh"
#include "Ravl/DList.hh"
#include "Ravl/StdConst.hh"

namespace RavlImageN {
  
  using namespace RavlN;
  
  enum EdgeStateT { EDGE_PROC = 0,EDGE_UNPROC,EDGE_JUNCT,EDGE_INSTRING };
  enum FBOrientationT { FB_FORWARD,FB_BACKWARD };

  //! userlevel=Normal
  //: Engine for linking edges into chains.

  // The edge information is stored internally in an ImageC<ByteT> object.
  // Although this has been made public, its use is deprecated and the interpretation of the
  // pixel values is obscure.
  
  class EdgeLinkC
    : public ImageC<ByteT>
  {
  public:
    EdgeLinkC()
      : edgeCount(0)
    {}
    //: Default constructor.
    
    EdgeLinkC(const IndexRange2dC &rect)
      : ImageC<ByteT>(rect),
	edgeCount(0)
    {}
    //: Constructor.
    
    EdgeLinkC(const ImageC<ByteT> &x)
      : ImageC<ByteT>(x),
	edgeCount(0)
    {}
    //: Construct from ByteT image.
    
    void LabelContour(const Index2dC &start);
    //: Label contours.
    
    DListC<DListC<Index2dC> > LinkEdges(bool clearDir = true);
    //: Generate a set of edge lists.
    // If clearDir is true information about pixel neighbours is cleared from
    // the image after processing is complete. Set to false if you are not using
    // the edge image and want to save some time.
    
    DListC<SArray1dC<EdgelC> > LinkEdgels(const ImageC<RealT> & inDrIm, 
                                          const ImageC<RealT> & inDcIm,  
                                          const ImageC<RealT> & inGrad,
                                          bool clearDir = true
                                          );
    //: Generate a set of linked edgels
    // If clearDir is true information about pixel neighbours is cleared from
    // the image after processing is complete. Set to false if you are not using
    // the edge image and want to save some time.
    
    SArray1dC<Index2dC> ListEdges();
    //: List edge positions in the image
    
    SArray1dC<EdgelC> ArrayOfEdgels(const ImageC<RealT> & inDrIm, 
				    const ImageC<RealT> & inDcIm,  
				    const ImageC<RealT> & inGrad) const;
    //: Generate an array of all edgels in image
    
    DListC<EdgelC> ListOfEdgels(const ImageC<RealT> & inDrIm, 
				const ImageC<RealT> & inDcIm,  
				const ImageC<RealT> & inGrad) const;
    //: Generate a list of all edgels in image
    
  protected:
    static NeighbourOrderT GetDir(ByteT val, FBOrientationT oo) 
    { return (NeighbourOrderT)((val >> ((oo==FB_FORWARD) ? 2 : 5)) & 7); }
    
    static void PutDir(ByteT &dest,NeighbourOrderT dir,FBOrientationT  oo) 
    { dest |= (dir << ((oo == FB_FORWARD) ? 2 : 5)); }
    
    void PutDir(const Index2dC &pxl,NeighbourOrderT dir,FBOrientationT  oo) 
    { (*this)[pxl] |= (dir << ((oo == FB_FORWARD) ? 2 : 5)); }
    
    static EdgeStateT GetState(ByteT value)
    { return (EdgeStateT) (value & 3); }
    //: Get the state of a pixel, ignoring any direction information.
    
    EdgeStateT GetState(const Index2dC &at)
    { return (EdgeStateT) ((*this)[at] & 3); }
    //: Get the state of a pixel, ignoring any direction information.
    
    static void PutState(ByteT & pixel, EdgeStateT state)
    { pixel = (pixel & ~3)|state; }
    //: Change the state of of a pixel.
    
    void PutState(const Index2dC &loc, EdgeStateT state) {
      ByteT &val = (*this)[loc];
      val = (val & ~3)|state; 
    }
    //: Change the state of of a pixel.
    
    bool IsEdge(const Index2dC &pxl) 
    { return GetState((*this)[pxl]) != EDGE_PROC; }
    //: Is this an edge ?
    
    bool IsNotExpanded(const Index2dC &pxl) 
    { return GetState((*this)[pxl]) == EDGE_UNPROC; }
    //: Has this edge been expanded yet ?
    
    UIntT edgeCount; // Count of edges in the map.
  };
  
  EdgeLinkC HysteresisThreshold(const ImageC<RealT> &img,RealT maxThr,RealT minThr);
}


#endif
