// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_EDGEDETECTOR_HEADER
#define RAVLIMAGE_EDGEDETECTOR_HEADER 1
//! author="Charles Galambos"
//! userlevel=Obsolete
//! rcsid="$Id$"
//! lib=RavlImageProc
//! docentry="Ravl.API.Images.Edges"
//! file="Ravl/Image/Processing/Edges/EdgeDetector.hh"
//! example="doEdgeDet.cc"

#include "Ravl/Image/EdgeDeriche.hh"
#include "Ravl/Image/EdgeSobel.hh"
#include "Ravl/Image/EdgeNonMaxSuppression.hh"
#include "Ravl/Image/SqrComposition.hh"
#include "Ravl/Image/EdgeLink.hh"
#include "Ravl/Image/EdgeSobel.hh"

namespace RavlImageN {

  //! userlevel=Obsolete
  //: Edge detector body
  // This class wraps up all the components you need to go from grey scale image
  // to a list of Edgels or pixel locations.
  
  class EdgeDetectorBodyC 
    : public RCBodyC
  {
  public:
    EdgeDetectorBodyC(bool useDeriche,RealT maxHyst,RealT minHyst);
    //: Constructor.
    
    bool Apply(const ImageC<RealT> &img,EdgeLinkC &edgeMap,ImageC<RealT> &edgeDr,ImageC<RealT> &edgeDc,ImageC<RealT> &edgeMag) const;
    //: Apply the edge detector to 'img', generate an edge map image.

    bool Apply(const ImageC<RealT> &img,SArray1dC<EdgelC> &edges) const;
    //: Apply the edge detector to 'img', generate an array of edgels.

    SArray1dC<EdgelC> PApply(const ImageC<RealT> &img) const {
      SArray1dC<EdgelC> ret;
      Apply(img,ret);
      return ret;
    }
    //: Apply the edge detector to 'img', returning an array of edgels.

    bool Apply(const ImageC<RealT> &img,DListC<EdgelC> &edges) const;
    //: Apply the edge detector to 'img', generate a list of edgels.
    
    DListC<EdgelC> LApply(const ImageC<RealT> &img) const {
      DListC<EdgelC> ret;
      Apply(img,ret);
      return ret;
    }
    //: Apply the edge detector to 'img', returning a list of edgels.
    
    bool Apply(const ImageC<RealT> &img,DListC<SArray1dC<EdgelC> > &edgeLists) const;
    //: Apply the edge detector to 'img', generate an array of ordered edgels list.

    bool Apply(const ImageC<ByteT> &img,EdgeLinkC &edgeMap,ImageC<RealT> &edgeDr,ImageC<RealT> &edgeDc,ImageC<RealT> &edgeMag) const;
    //: Apply the edge detector to 'img', generate an edge map image.
    
    bool Apply(const ImageC<ByteT> &img,DListC<SArray1dC<EdgelC> > &edgeLists) const;
    //: Apply the edge detector to 'img', generate an array of ordered edgels lists.
    
    bool Apply(const ImageC<ByteT> &img,DListC<DListC<EdgelC> > &edgeLists) const;
    //: Apply the edge detector to 'img', generate list of ordered edgels lists.

    bool Apply(const ImageC<ByteT> &img, ImageC<bool> &edgeImg) const;
    //: Apply the edge detector to 'img', generate a boolean image of edges.

    void SetDeriche(RealT Omega, RealT Alpha)
    { edgeDet = EdgeDericheC(Omega,Alpha); }
    //: Set the Deriche filter parameters
    
  protected:
    RealT minHyst,maxHyst;
    
    EdgeDericheC edgeDet;
    EdgeNonMaxSuppressionC nonMaxSup;
    SqrCompositionC sqrCompose;
    bool useDeriche;   // Use deriche edge detection, otherwise use Sobel.
    bool eightConnect; // Use eight connectivity ?
  };
  
  //! userlevel=Obsolete
  //: Edge detector.
  // This class wraps up all the components you need to go from grey scale image
  // to a list of Edgels or pixel locations.
  
  class EdgeDetectorC 
    : public RCHandleC<EdgeDetectorBodyC>
  {
  public:
    //:----
    //: Constructors

    EdgeDetectorC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    EdgeDetectorC(bool useDeriche,RealT maxHyst = 20,RealT minHyst= 15)
      : RCHandleC<EdgeDetectorBodyC>(*new EdgeDetectorBodyC(useDeriche,minHyst,maxHyst))
    {}
    //: Constructor.
    // If useDeriche is true, uses the <a href="RavlImageN.EdgeDericheC.html">Deriche</a> edge detector; if false, uses the Sobel edge detector.<br>
    // Default Deriche parameters are as in EdgeDericheC <a href="RavlImageN.EdgeDericheC.html#EdgeDericheCObRealT_RealTCb">constructor</a>.
    // Note order of threshold arguments (for backwards compatibility).
    
  protected:
    
  public:
    //:----
    //: Apply methods: apply the edge detector to "img" of <code>RealT</code> pixels

    bool Apply(const ImageC<RealT> &img,EdgeLinkC &edgeMap,ImageC<RealT> &edgeDx,ImageC<RealT> &edgeDy,ImageC<RealT> &edgeMag) const
    { return Body().Apply(img,edgeMap,edgeDx,edgeDy,edgeMag); }
    //: Generates an edge link image.
    // Edge images are also returned.
    
    bool Apply(const ImageC<RealT> &img,EdgeLinkC &edgeMap) const { 
      ImageC<RealT> edgeDx;
      ImageC<RealT> edgeDy;
      ImageC<RealT> edgeMag;
      return Body().Apply(img,edgeMap,edgeDx,edgeDy,edgeMag); 
    }
    //: Generates an edge link image.

    bool Apply(const ImageC<RealT> &img,SArray1dC<EdgelC> &edges) const
    { return Body().Apply(img,edges); }
    //: Generates an array of edgels.
    
    SArray1dC<EdgelC> PApply(const ImageC<RealT> &img) const
    { return Body().PApply(img); }
    //: Returns an array of edgels.
    
    bool Apply(const ImageC<RealT> &img,DListC<EdgelC> &edges) const
    { return Body().Apply(img,edges); }
    //: Generates a list of edgels.

    DListC<EdgelC> LApply(const ImageC<RealT> &img) 
    { return Body().LApply(img); }
    //: Returns a list of edgels.
    
    bool Apply(const ImageC<RealT> &img,DListC<SArray1dC<EdgelC> > &edgeLists) const
    { return Body().Apply(img,edgeLists); }
    //: Generates an array of ordered edgels list.
        
    //:----
    //: Apply methods: apply the edge detector to "img" of <code>ByteT</code> pixels

    bool Apply(const ImageC<ByteT> &img,EdgeLinkC &edgeMap,ImageC<RealT> &edgeDx,ImageC<RealT> &edgeDy,ImageC<RealT> &edgeMag) const
    { return Body().Apply(img,edgeMap,edgeDx,edgeDy,edgeMag); }
    //: Generate an edge link image.
    // Edge images are also returned.

    bool Apply(const ImageC<ByteT> &img,DListC<SArray1dC<EdgelC> > &edgeLists) const
    { return Body().Apply(img,edgeLists); }
    //: Generates a list of edgel arrays
    
    bool Apply(const ImageC<ByteT> &img,DListC<DListC<EdgelC> > &edgeLists) const
    { return Body().Apply(img,edgeLists); }
    //: Generates a list of edgel lists.
    
    bool Apply(const ImageC<ByteT> &img, ImageC<bool> &edgeImg) const
    { return Body().Apply(img,edgeImg); }
    //: Apply the edge detector to 'img', generate a boolean image of edges.

    void SetDeriche(RealT Omega, RealT Alpha)
    { Body().SetDeriche(Omega,Alpha); }
    //: Set the Deriche filter parameters
    //!param: Alpha -  &alpha; controls the filter width
    //!param: Omega -  &omega; doesn't appear to be a very useful parameter -- can be set to 0.  Is 1st argument only because of bug in code.
  };
  
}

#endif
