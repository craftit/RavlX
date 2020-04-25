// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! author="Bill Christmas"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Edges/EdgeDetect.cc"

#include "Ravl/Image/EdgeDetect.hh"
#include "Ravl/Image/EdgeSobel.hh"
#include "Ravl/Image/ImageConv.hh"
#include "Ravl/Image/ImageConv.hh"
#include "Ravl/CDLIter.hh"
#include "Ravl/SArray1dIter.hh"

namespace RavlImageN {

  //: Constructor.
  
  EdgeDetectBodyC::EdgeDetectBodyC() 
    : minHyst(15),
      maxHyst(20),
      useDeriche(true),
      eightConnect(false)
  {}
  

  //: Apply the edge detector to image of RealT

  bool EdgeDetectBodyC::Apply(const ImageC<RealT> & img) {
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
    
    edgeArrayList = edgeMap.LinkEdgels(edgeDr, edgeDc, edgeMag, false);
    /* N.B. LinkEdgels() is put here because it must only be called once per Apply() for
     * some reason.  Something to do with it being a non-const method I guess.
     * If called repeatedly, it just returns an empty list on 2nd & following calls.
     */
    return true;
  }


    //: Apply the edge detector to image of ByteT

  bool EdgeDetectBodyC::Apply(const ImageC<ByteT> & img) {
    return Apply(ByteImageCT2DoubleImageCT(img));
  }


  //: Return edges as list of lists of connected edgels

  DListC<DListC<EdgelC> > EdgeDetectBodyC::EdgeLists()
  {
    DListC<DListC<EdgelC> > edgeLists;
    for(ConstDLIterC<DListC<Index2dC> > itStr(edgeMap.LinkEdges());itStr;itStr++) {
      DListC<EdgelC> str;
      for(ConstDLIterC<Index2dC> it(*itStr);it;it++) {
	str.InsLast(EdgelC(*it,edgeDr[*it],edgeDc[*it],edgeMag[*it]));
      }
      edgeLists.InsLast(str);
    }
    return edgeLists;
  }


  //: Return edges as boolean image

  ImageC<bool> EdgeDetectBodyC::EdgeImg() {
    ImageC<bool> edgeImg = ImageC<bool>(edgeMap.Frame());
    edgeImg.Fill(false);
    for (DLIterC<SArray1dC<EdgelC> > il(this->EdgeArrayList()); il; ++il) {
      for (SArray1dIterC<EdgelC> ia(*il); ia; ++ia) {
        edgeImg[ia->At()] = true;
      }
    }
    return edgeImg;
  }

}
