// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_EDGEDETECT_HEADER
#define RAVLIMAGE_EDGEDETECT_HEADER 1
//! author="Bill Christmas"
//! userlevel=Normal
//! rcsid="$Id$"
//! lib=RavlImageProc
//! docentry="Ravl.API.Images.Edges"
//! file="Ravl/Image/Processing/Edges/EdgeDetect.hh"
//! example="doEdgeDet.cc"

#include "Ravl/Image/EdgeDeriche.hh"
#include "Ravl/Image/EdgeSobel.hh"
#include "Ravl/Image/EdgeNonMaxSuppression.hh"
#include "Ravl/Image/SqrComposition.hh"
#include "Ravl/Image/EdgeLink.hh"
#include "Ravl/Vector2d.hh"

namespace RavlImageN {

  //! userlevel=Develop
  //: Edge detector body
  // This class wraps up all the components you need to go from grey scale image
  // to a list of Edgels or pixel locations.

  class EdgeDetectBodyC 
    : public RCBodyC
  {
  protected:
    RealT minHyst,maxHyst;
    
    EdgeDericheC edgeDet;
    EdgeNonMaxSuppressionC nonMaxSup;
    SqrCompositionC sqrCompose;
    bool useDeriche;   // Use deriche edge detection, otherwise use Sobel.
    bool eightConnect; // Use eight connectivity ?
    ImageC<RealT> edgeDr;
    ImageC<RealT> edgeDc;
    ImageC<RealT> edgeMag;
    ImageC<RealT> nonMax;
    EdgeLinkC edgeMap;
    DListC<SArray1dC<EdgelC> > edgeArrayList;

  public:
    EdgeDetectBodyC();
    //: Constructor.

    void UseSobel()
    { useDeriche = false; }
    //: Use Sobel filter instead of Deriche

    void SetDeriche(RealT Alpha, RealT Omega)
    { edgeDet = EdgeDericheC(Omega,Alpha); }
    //: Set the Deriche filter parameters
    // See <a href="RavlImageN.EdgeDericheC.html#EdgeDericheCObRealT_RealTCb"> EdgeDericheC constructor </a> for default values.
    //!param: Alpha -  &alpha; controls the filter width
    //!param: Omega -  &omega; doesn't appear to be a very useful parameter -- can be set to 0.

    void SetHysteresis(RealT MinHyst, RealT MaxHyst)
    { minHyst = MinHyst; maxHyst = MaxHyst; }
    //: Set hysteresis threshold parameters
    
    bool Apply(const ImageC<RealT> &img);
    //: Apply the edge detector to image of RealT

    bool Apply(const ImageC<ByteT> &img);
    //: Apply the edge detector to image of ByteT
    DListC<DListC<EdgelC> > EdgeLists();
    //: Return edges as list of lists of connected edgels

    DListC<EdgelC> EdgeList()
    { return edgeMap.ListOfEdgels(edgeDr,edgeDc,nonMax); }
    //: Returns edges as single list of edgels

    DListC<SArray1dC<EdgelC> > EdgeArrayList()
    { return edgeArrayList; }
    //: Return edges as list of connected edgel arrays

    SArray1dC<EdgelC> EdgeArray()
    { return edgeMap.ArrayOfEdgels(edgeDr,edgeDc,edgeMag); }
    //: Return edges as single array of edgels

    ImageC<bool> EdgeImg();
    //: Return edges as boolean image

    ImageC<Vector2dC> Grad() const;
    //: Returns image of gradient vectors

    ImageC<RealT> GradHorz() const
    { return edgeDc; }
    //: Returns image of horizontal gradient

    ImageC<RealT> GradVert() const
    { return edgeDr; }
    //: Returns image of vertical gradient

    ImageC<RealT> GradMag() const
    { return edgeMag; }
    //: Returns image of gradient magnitude

  };
  
  //! userlevel=Normal
  //: Edge detector body 
  // <p>This class wraps up all the components you need to go from grey scale image
  // to a list of Edgels or pixel locations.</p>
  // <p>This class replaces the <a href="../Class/RavlImageN.EdgeDetectorC.html">EdgeDetectorC</a> class.</p>
  //!cwiz:author
  
  class EdgeDetectC
    : public RCHandleC<EdgeDetectBodyC>
  {
  public:
    EdgeDetectC() 
      : RCHandleC<EdgeDetectBodyC>(*new EdgeDetectBodyC())
    {}
    //: Constructor. 
    //!cwiz:author
    
    //:----
    //:Parameter setting

    void UseSobel() 
    { Body().UseSobel(); }
    //: Use Sobel filter instead of Deriche 
    //!cwiz:author
    
    void SetDeriche(RealT Alpha, RealT Omega=0.001) 
    { Body().SetDeriche(Alpha, Omega); }
    //: Set the Deriche filter parameters 
    // See <a href="RavlImageN.EdgeDericheC.html#EdgeDericheCObRealT_RealTCb"> EdgeDericheC constructor </a> for default values.
    //!param: Alpha -  &alpha; controls the filter width
    //!param: Omega -  &omega; doesn't appear to be a very useful parameter -- can be set to 0.
    //!cwiz:author

    void SetHysteresis(RealT MinHyst, RealT MaxHyst)
    { Body().SetHysteresis(MinHyst, MaxHyst); }
    //: Set hysteresis threshold parameters
    // (Defaults: 15, 20)
    
    
    //:----
    //: Run the edge detector

    bool Apply(const ImageC<RealT> & img)
    { return Body().Apply(img); }
    //: Apply the edge detector to image of RealT 
    //!cwiz:author
    
    bool Apply(const ImageC<ByteT> & img)
    { return Body().Apply(img); }
    //: Apply the edge detector to image of ByteT 
    //!cwiz:author
    
    
    //:----
    //: Edges in choice of formats

    DListC<DListC<EdgelC> > EdgeLists()
    { return Body().EdgeLists(); }
    //: Return edges as list of lists of connected edgels 
    //!cwiz:author
    
    DListC<EdgelC> EdgeList()
    { return Body().EdgeList(); }
    //: Returns edges as single list of edgels 
    //!cwiz:author
    
    DListC<SArray1dC<EdgelC> > EdgeArrayList()
    { return Body().EdgeArrayList(); }
    //: Return edges as list of connected edgel arrays 
    //!cwiz:author
    
    SArray1dC<EdgelC> EdgeArray()
    { return Body().EdgeArray(); }
    //: Return edges as single array of edgels 
    //!cwiz:author
    
    ImageC<bool> EdgeImg()
    { return Body().EdgeImg(); }
    //: Return edges as boolean image 
    //!cwiz:author
    
    
    //:----
    //: Gradient images

    ImageC<Vector2dC> Grad() const
    { return Body().Grad(); }
    //: Returns image of gradient vectors 
    //!cwiz:author
    
    ImageC<RealT> GradHorz() const
    { return Body().GradHorz(); }
    //: Returns image of horizontal gradient 
    //!cwiz:author
    
    ImageC<RealT> GradVert() const
    { return Body().GradVert(); }
    //: Returns image of vertical gradient 
    //!cwiz:author
    
    ImageC<RealT> GradMag() const
    { return Body().GradMag(); }
    //: Returns image of gradient magnitude 
    //!cwiz:author
    
  protected:
    EdgeDetectC(EdgeDetectBodyC &bod)
     : RCHandleC<EdgeDetectBodyC>(bod)
    {}
    //: Body constructor. 
    
    EdgeDetectBodyC& Body()
    { return static_cast<EdgeDetectBodyC &>(RCHandleC<EdgeDetectBodyC>::Body()); }
    //: Body Access. 
    
    const EdgeDetectBodyC& Body() const
    { return static_cast<const EdgeDetectBodyC &>(RCHandleC<EdgeDetectBodyC>::Body()); }
    //: Body Access. 
    
  };

}

#endif
