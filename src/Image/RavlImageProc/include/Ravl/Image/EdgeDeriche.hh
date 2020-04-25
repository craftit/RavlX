// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_EDGEDERICHE_HEADER
#define RAVLIMAGE_EDGEDERICHE_HEADER 1
///////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Image/Processing/Edges/EdgeDeriche.hh"
//! authors="George Matas, Radek Marik and Charles Galambos"
//! docentry="Ravl.API.Images.Edges"
//! lib=RavlImageProc

#include "Ravl/Image/Image.hh"
#include "Ravl/RefCounter.hh"
#include "Ravl/Tuple2.hh"

namespace RavlImageN {
#if RAVL_VISUALCPP_NAMESPACE_BUG
  using namespace RavlN;
  using RavlN::RCBodyC;
  using RavlN::RCHandleC;
#endif
  
  class EdgeDericheC;

  //! userlevel=Develop
  //: Deriche edge filter.
  
  class EdgeDericheBodyC  
    : public RCBodyC
  {
  public:
    EdgeDericheBodyC(RealT omega,RealT alpha);
    //: Constructor.
    
    bool Apply(const ImageC<RealT> &inImg,ImageC<RealT> &outVert,ImageC<RealT> &outHorz) const;
    //: Apply operator.

    bool Apply(const ImageC<RealT> &inImg,ImageC<TFVectorC<RealT,2> > &out) const;
    //: Apply Deriche operator to 'img', put result in 'out'
    // This is a little slower than the Apply(inImg,outVert,outHorz) for the moment,
    // this will be fixed eventually.

    ImageC<RealT> EdgeMagnitude(const ImageC<RealT> &inImg) const;
    //: Calculate the edge magnitude only
    
  protected:
    RealT omega,alpha; // User parameters.
    
    bool Init();
    //: Compute filter parameters.
    
    bool YDer(ImageC<RealT> &im,ImageC<RealT> &derv) const;
  
    bool YDer1p(ImageC<RealT> &im,ImageC<RealT> &a2) const;
    bool YDer1n(ImageC<RealT> &im,ImageC<RealT> &a3) const;
    bool YDer2p(ImageC<RealT> &im,ImageC<RealT> &a2,ImageC<RealT> &a3) const;
    bool YDer2n(ImageC<RealT> &im,ImageC<RealT> &a2,ImageC<RealT> &a4) const;
    
    bool XDer(ImageC<RealT> &im,ImageC<RealT> &derv) const;
    
    bool XDer1p(ImageC<RealT> &im,ImageC<RealT> &a2) const;
    bool XDer1n(ImageC<RealT> &im,ImageC<RealT> &a3) const;
    bool XDer2p(ImageC<RealT> &im,ImageC<RealT> &a2,ImageC<RealT> &a3) const;
    bool XDer2n(ImageC<RealT> &im,ImageC<RealT> &a2,ImageC<RealT> &a4) const;
    
    // Parameters.
    RealT cst,cst0,cst1,cst2;
    RealT ad1,ad2,an1,an2,an3,an4,an11;
    
    friend class EdgeDericheC;  
  };
  
  
  //! userlevel=Normal
  //: Deriche edge filter.

  // <p>R. Deriche, "Using Canny's Criteria to Derive a Recursively
  // Implemented Optimal Edge Detector"; in:
  // International Journal of Comp. Vision, vol.1, no. 2, pp. 167-187, 1987.</p>
  //
  // <p>The sign convention is: a +ve gradient computed if image intensity is
  // increasing in +ve direction in coordinate system.</p>
  //
  // <p>A vertical (horizontal) edge is defined as an edge in which the
  // <i>gradient is changing</i> in a vertical (horizontal) direction.</p>
  
  class EdgeDericheC 
    : public RCHandleC<EdgeDericheBodyC>
  { 
  public:
    EdgeDericheC(RealT omega = 0.001 ,RealT alpha = 2.0)
      : RCHandleC<EdgeDericheBodyC>(*new EdgeDericheBodyC(omega,alpha))
    {}
    //: Constructor.
    // Decreasing alpha <i>increases</i> the amount of filtering.
  protected:
    EdgeDericheC(EdgeDericheBodyC &bod)
      : RCHandleC<EdgeDericheBodyC>(bod)
    {}
    //: Body constructor.
    
    EdgeDericheBodyC &Body()
    { return static_cast<EdgeDericheBodyC &>(RCHandleC<EdgeDericheBodyC>::Body()); }
    //: Access body.
    
    const EdgeDericheBodyC &Body() const
    { return static_cast<const EdgeDericheBodyC &>(RCHandleC<EdgeDericheBodyC>::Body()); }
    //: Access body.


  public:
    
    bool Apply(const ImageC<RealT> &inImg,ImageC<RealT> &vert,ImageC<RealT> &horz) const
    { return Body().Apply(inImg,vert,horz); }
    //: Apply Deriche operator to "img" put the vertical and horizontal edge results into "vert" and "horz" respectively.
    
    bool Apply(const ImageC<RealT> &inImg,ImageC<TFVectorC<RealT,2> > &out) const
    { return Body().Apply(inImg,out); }
    //: Apply Deriche operator to "img", put result in "out"
    // This is slightly slower than the Apply(inImg,vert,horz) for the moment,
    // but it might be fixed eventually.
    
    ImageC<RealT> EdgeMagnitude(const ImageC<RealT> &inImg) const
    { return Body().EdgeMagnitude(inImg); }
    //: Calculate the edge magnitude only
    
  protected:
    
    bool YDer1p(ImageC<RealT> &im,ImageC<RealT> &a2) const
    { return Body().YDer1p(im,a2); }
    
    bool YDer1n(ImageC<RealT> &im,ImageC<RealT> &a3) const
    { return Body().YDer1n(im,a3); }
    
    bool YDer2p(ImageC<RealT> &im,ImageC<RealT> &a2,ImageC<RealT> &a3) const
    { return Body().YDer2p(im,a2,a3); }
    
    bool YDer2n(ImageC<RealT> &im,ImageC<RealT> &a2,ImageC<RealT> &a4) const
    { return Body().YDer2n(im,a2,a4); }
    
    bool XDer1p(ImageC<RealT> &im,ImageC<RealT> &a2) const
    { return Body().XDer1p(im,a2); }
    
    bool XDer1n(ImageC<RealT> &im,ImageC<RealT> &a3) const
    { return Body().XDer1n(im,a3); }
    
    bool XDer2p(ImageC<RealT> &im,ImageC<RealT> &a2,ImageC<RealT> &a3) const
    { return Body().XDer2p(im,a2,a3); }
    
    bool XDer2n(ImageC<RealT> &im,ImageC<RealT> &a2,ImageC<RealT> &a4) const
    { return Body().XDer2n(im,a2,a4); }

    bool YDer(ImageC<RealT> &im,ImageC<RealT> &derv) const
    { return Body().YDer(im,derv); }
    
    bool XDer(ImageC<RealT> &im,ImageC<RealT> &derv) const
    { return Body().XDer(im,derv); }  
    
    friend class EdgeDericheBodyC;
  };

}

#endif
