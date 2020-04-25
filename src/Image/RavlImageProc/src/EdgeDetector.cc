// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! author="Charles Galambos"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Edges/EdgeDetector.cc"

#include "Ravl/Image/EdgeDetector.hh"
#include "Ravl/Image/EdgeSobel.hh"
#include "Ravl/Image/ImageConv.hh"
#include "Ravl/CDLIter.hh"
#include "Ravl/SArray1dIter.hh"

namespace RavlImageN {

  //: Constructor.
  
  EdgeDetectorBodyC::EdgeDetectorBodyC(bool nuseDeriche,RealT nmaxHyst,RealT nminHyst) 
    : minHyst(nminHyst),
      maxHyst(nmaxHyst),
      useDeriche(nuseDeriche),
      eightConnect(false)
  { 
  }
  
  //: Apply the edge detector to 'img', generate an edge link image.
  
  bool EdgeDetectorBodyC::Apply(const ImageC<RealT> &img,EdgeLinkC &edgeMap,ImageC<RealT> &edgeDr,ImageC<RealT> &edgeDc,ImageC<RealT> &nonMax) const {     
    ImageC<RealT> edgeMag;
    if(useDeriche)
      edgeDet.Apply(img,edgeDr,edgeDc);
    else {
      EdgeSobelC<RealT,RealT> ed;
      ed.Apply(img,edgeDr,edgeDc);
    }
    
    sqrCompose.Apply(edgeDr,edgeDc,edgeMag);
    RavlAssert(edgeDr.Frame().Area() > 0);
    RavlAssert(edgeDc.Frame().Area() > 0);
    RavlAssert(edgeMag.Frame().Area() > 0);
    
    RealT mean;
    IntT edgels;
    nonMaxSup.Apply(edgeDr,edgeDc,edgeMag,nonMax,mean,edgels);
    
    edgeMap = HysteresisThreshold(nonMax,maxHyst,minHyst);
    
    return true;
  }

  //: Apply the edge detector to 'img', returning a list of edgels.
  
  bool EdgeDetectorBodyC::Apply(const ImageC<RealT> &img,SArray1dC<EdgelC> &edges) const {
    ImageC<RealT> edgeDr;
    ImageC<RealT> edgeDc;
    ImageC<RealT> edgeMag;
    EdgeLinkC edgeMap;
    if(!Apply(img,edgeMap,edgeDr,edgeDc,edgeMag))
      return false;    
    edges = edgeMap.ArrayOfEdgels(edgeDr,edgeDc,edgeMag);
    return true;
  }

  //: Apply the edge detector to 'img', generate a list of edgels.
  
  bool EdgeDetectorBodyC::Apply(const ImageC<RealT> &img,DListC<EdgelC> &edges) const {
    ImageC<RealT> edgeDr;
    ImageC<RealT> edgeDc;
    ImageC<RealT> nonMax;
    EdgeLinkC edgeMap;
    if(!Apply(img,edgeMap,edgeDr,edgeDc,nonMax))
      return false;    
    edges = edgeMap.ListOfEdgels(edgeDr,edgeDc,nonMax);
    return true;
  }

  //: Apply the edge detector to 'img', generate an array of ordered edgels list.
  
  bool EdgeDetectorBodyC::Apply(const ImageC<RealT> &img,DListC<SArray1dC<EdgelC> > &edgeLists) const {
    ImageC<RealT> edgeDr;
    ImageC<RealT> edgeDc;
    ImageC<RealT> edgeMag;
    EdgeLinkC edgeMap;
    if(!Apply(img,edgeMap,edgeDr,edgeDc,edgeMag))
      return false;    
    
    edgeLists =  edgeMap.LinkEdgels(edgeDr,
                                    edgeDc,
                                    edgeMag,
                                    false
                                    );
    return true;
  }
    

  //: Apply the edge detector to 'img', generate an edge map image.
  
  bool EdgeDetectorBodyC::Apply(const ImageC<ByteT> &img,EdgeLinkC &edgeMap,ImageC<RealT> &edgeDr,ImageC<RealT> &edgeDc,ImageC<RealT> &nonMax) const {
    ImageC<RealT> edgeMag;
    if(useDeriche) {
      ImageC<RealT> rimg = ByteImageCT2DoubleImageCT(img);
      edgeDet.Apply(rimg,edgeDr,edgeDc);
    } else {
      EdgeSobelC<ByteT,RealT> ed;
      ed.Apply(img,edgeDr,edgeDc);
    }
    
    sqrCompose.Apply(edgeDr,edgeDc,edgeMag);
    RavlAssert(edgeDr.Frame().Area() > 0);
    RavlAssert(edgeDc.Frame().Area() > 0);
    RavlAssert(edgeMag.Frame().Area() > 0);
    
    RealT mean;
    IntT edgels;
    nonMaxSup.Apply(edgeDr,edgeDc,edgeMag,nonMax,mean,edgels);
    
    edgeMap = HysteresisThreshold(nonMax,maxHyst,minHyst);
    
    return true;    
  }
  
  //: Apply the edge detector to 'img', generate an array of ordered edgels list.
  
  bool EdgeDetectorBodyC::Apply(const ImageC<ByteT> &img,DListC<SArray1dC<EdgelC> > &edgeLists) const {
    ImageC<RealT> edgeDr;
    ImageC<RealT> edgeDc;
    ImageC<RealT> edgeMag;
    EdgeLinkC edgeMap;
    if(!Apply(img,edgeMap,edgeDr,edgeDc,edgeMag))
      return false;    
    
    edgeLists =  edgeMap.LinkEdgels(edgeDr,
                                    edgeDc,
                                    edgeMag,
                                    false
                                    );
    return true;
  }

  //: Apply the edge detector to 'img', generate a list of ordered edgels lists.
  
  bool EdgeDetectorBodyC::Apply(const ImageC<ByteT> &img,DListC<DListC<EdgelC> > &edgeLists) const {
    ImageC<RealT> edgeDr;
    ImageC<RealT> edgeDc;
    ImageC<RealT> edgeMag;
    EdgeLinkC edgeMap;
    if(!Apply(img,edgeMap,edgeDr,edgeDc,edgeMag))
      return false;    
    
    edgeLists.Empty();
    
    // Convert to edgel list.
    
    DListC<DListC<Index2dC> > rawEdges = edgeMap.LinkEdges();
    for(DLIterC<DListC<Index2dC> > itStr(rawEdges);itStr;itStr++) {
      DListC<EdgelC> str;
      for(DLIterC<Index2dC> it(*itStr);it;it++) {
	str.InsLast(EdgelC(*it,edgeDr[*it],edgeDc[*it],edgeMag[*it]));
      }
      edgeLists.InsLast(str);
    }
    return true;
  }


  //: Apply the edge detector to 'img', generate a boolean image of edges.

  bool EdgeDetectorBodyC::Apply(const ImageC<ByteT> &img, ImageC<bool> &edgeImg) const {
    DListC<SArray1dC<EdgelC> > edgeLists;
    if(!Apply(img,edgeLists))  return false;
    edgeImg = ImageC<bool>(img.Frame());
    edgeImg.Fill(false);
    for (ConstDLIterC<SArray1dC<EdgelC> > il(edgeLists); il; ++il) {
      for (SArray1dIterC<EdgelC> ia(*il); ia; ++ia) {
        edgeImg[ia->At()] = true;
      }
    }
    return true;
  }

}
