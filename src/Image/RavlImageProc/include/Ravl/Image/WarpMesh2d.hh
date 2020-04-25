// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_WARPMESH2D_HEADER
#define RAVLIMAGE_WARPMESH2D_HEADER 1
//! rcsid="$Id$"
//! lib=RavlImageProc
//! docentry="Ravl.API.Images.Scaling and Warping"
//! example=exWarpMesh.cc
//! author="Charles Galambos"
//! file="Ravl/Image/Processing/Filters/Warp/WarpMesh2d.hh"

#include "Ravl/TriMesh2d.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Affine2d.hh"
#include "Ravl/Image/PixelMixer.hh"
#include "Ravl/Image/BilinearInterpolation.hh"
#include "Ravl/Image/DrawFrame.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/Array1dIter2.hh"
#include "Ravl/Array1dIter.hh"

namespace RavlImageN {

  using namespace RavlImageN;
  using namespace RavlN;
  
  //:-
  
  //! userlevel=Develop
  //: Warp an image with an affine mesh.
  
  class WarpMesh2dBaseC 
  {
  public:
    WarpMesh2dBaseC(bool nFillBackground = true)
      : fillBackground(nFillBackground)
    {}
    //: Default constructor.
    
    WarpMesh2dBaseC(const TriMesh2dC &homeMesh,bool nFillBackground = true);
    //: Constructor.

    WarpMesh2dBaseC(const TriMesh2dC &homeMesh,const IndexRange2dC &rng,bool nFillBackground = true);
    //: Constructor.
    
    void CreateWarpMap(const TriMesh2dC &homeMesh,const IndexRange2dC &rng);
    //: Create a map of faces in the mesh.
    
    bool ComputeTransforms(const SArray1dC<Point2dC> &baseMesh,const SArray1dC<Point2dC> &newMeshPositions);
    //: Compute transforms.
    // Returns true if all transforms are defined.
    
    const TriMesh2dC &Mesh() const
    { return homeMesh; }
    //: Access mesh.
    
    TriMesh2dC &Mesh()
    { return homeMesh; }
    //: Access mesh.
    
    bool FillBackground() const
    { return fillBackground; }
    //: Are we filling in the background ?

    bool &FillBackground()
    { return fillBackground; }
    //: Are we filling in the background ?
    
    const IndexRange2dC WarpMapFrame() const
    { return warpMap.Frame(); }
    //: Access warp map frame.
    
    const ImageC<Affine2dC *> &WarpMap()
    { return warpMap; }
    // Map of pixels to faces.
    
    const SArray1dC<Affine2dC> &TransformMap()
    { return transforms; }
    // Affine transform for each face.
    
  protected:
    TriMesh2dC homeMesh;
    ImageC<Affine2dC *> warpMap; // Map of pixels to faces.
    SArray1dC<Affine2dC> transforms; // Affine transform for each face.
    bool fillBackground;
  };
  
  ostream & operator<<(ostream & outS, const WarpMesh2dBaseC & mesh);
  //: Write to stream.
  
  istream & operator>>(istream & inS,WarpMesh2dBaseC & mesh);
  //: Read from stream.
  
  BinOStreamC & operator<<(BinOStreamC & outS, const WarpMesh2dBaseC & mesh);
  //: Write to binary stream.
  
  BinIStreamC & operator>>(BinIStreamC & inS,WarpMesh2dBaseC & mesh);
  //: Read from binary stream.
  
  //:-
  
  //! userlevel=Normal
  //: Warp an image with an affine mesh.
  
  template<class InT,class OutT = InT,class MixerT = PixelMixerAssignC<OutT,OutT> >
  class WarpMesh2dC 
    : public WarpMesh2dBaseC
  {
  public:
    WarpMesh2dC(bool nFillBackground = true)
      : WarpMesh2dBaseC(nFillBackground)
    {}
    //: Default constructor.
    
    WarpMesh2dC(const WarpMesh2dBaseC &warpInfo)
      : WarpMesh2dBaseC(warpInfo)
    {}
    //: Build new warper using parameters.
    
    WarpMesh2dC(const TriMesh2dC &aHomeMesh,bool nFillBackground = true,const MixerT &nMixer = MixerT())
      : WarpMesh2dBaseC(aHomeMesh,nFillBackground),
	mixer(nMixer)
    {}
    //: Constructor.

    WarpMesh2dC(const TriMesh2dC &aHomeMesh,const IndexRange2dC &orng,bool nFillBackground = true,const MixerT &nMixer = MixerT())
      : WarpMesh2dBaseC(aHomeMesh,orng,nFillBackground),
	mixer(nMixer)
    {}
    //: Constructor.
    
    bool Apply(const ImageC<InT> &inImg,const SArray1dC<Point2dC> &newPnts,ImageC<OutT> &outImg) {
      if(outImg.Frame() != warpMap.Frame())
	outImg = ImageC<OutT>(warpMap.Frame());
      ComputeTransforms(homeMesh.Vertices(),newPnts);
      Point2dC at,pat;
      RealRange2dC irng(inImg.Frame());
      irng = irng.Expand(-1.1); // There's an off by a bit error somewhere in here...
      for(Array2dIter2C<OutT,Affine2dC *> it(outImg,warpMap);it;) {
	at = Point2dC(it.Index());
	do {
	  if(it.Data2() != 0) {
	    pat = (*it.Data2()) * at;
	    if(irng.Contains(pat)) {
	      OutT val;
	      BilinearInterpolation(inImg,pat,val);
	      mixer(it.Data1(),val);
	    } else {
	      if(fillBackground)
		SetZero(it.Data1());
	    }
	  } else {
	    if(fillBackground)
	      SetZero(it.Data1());
	  }
	  at[1]++;
	} while(it.Next());
      }
      return true;
    }
    //: Warp an image to align newPnts with the ones in the home mesh.

    bool InvApply(const ImageC<InT> &inImg,const SArray1dC<Point2dC> &newPnts,ImageC<OutT> &outImg) {
      //if(outImg.Frame() != warpMap.Frame())
      //	outImg = ImageC<OutT>(warpMap.Frame());
      ComputeTransforms(homeMesh.Vertices(),newPnts);
      Point2dC at;
      Array2dIter2C<InT,Affine2dC *> it(inImg,warpMap);
      
#if 0
      for(;it;) {
	at = Point2dC(it.Index());
	do {
	  if(it.Data2() != 0) {
	    Index2dC pat((*it.Data2()) * at);
	    if(outImg.Frame().Contains(pat))
	      outImg[pat] = it.Data1();
	    
	    pat.Left();
	    if(outImg.Frame().Contains(pat))
	      outImg[pat] = it.Data1();
	    
	    pat.Up();
	    if(outImg.Frame().Contains(pat))
	      outImg[pat] = it.Data1();
	    
	    pat.Right();
	    if(outImg.Frame().Contains(pat))
	      outImg[pat] = it.Data1();
	  }
	  at[1]++;
	} while(it.Next());
      }
#else      
      Array1dC<Index2dC> lastRow(inImg.Frame().Range2());
      Array1dC<Index2dC> curRow(inImg.Frame().Range2());

      // Do the first row.
      
      at = Point2dC(it.Index());
      Array1dIterC<Index2dC> ipit(lastRow);
      do {
	if(it.Data2() != 0)
	  *ipit = Index2dC((*it.Data2()) * at);
	else
	  *ipit = Index2dC(-1,-1);
	at[1]++;
	ipit++;
      } while(it.Next());
      
      // Do rest of rows.
      
      for(;it;) {
	at = Point2dC(it.Index());
	Array1dIter2C<Index2dC,Index2dC> pit(curRow,lastRow);
	// Do first pixel in row.
	if(it.Data2() != 0)
	  pit.Data1() = Index2dC((*it.Data2()) * at);
	else
	  pit.Data1() = Index2dC(-1,-1);
	Index2dC lr1 = pit.Data2();
	Index2dC lr2 = pit.Data1();
	pit++;
	at[1]++;
	it.Next();
	do {
	  if(it.Data2() != 0) {
	    pit.Data1() = Index2dC((*it.Data2()) * at);
	    IndexRange2dC srng(pit.Data1(),0);
	    srng.Involve(lr1);
	    srng.Involve(lr2);
	    srng.Involve(pit.Data2());
	    if(srng.Origin()[0] > -1) {
	      srng.ClipBy(outImg.Frame());
	      for(BufferAccess2dIterC<OutT> dit(outImg,srng);dit;dit++)
		*dit = it.Data1();
	    }
	    lr1 = pit.Data2();
	    lr2 = pit.Data1();
	  } else {
	    pit.Data1() = Index2dC(-1,-1);
	    lr1 = Index2dC(-1,-1);
	    lr2 = Index2dC(-1,-1);
	  }
	  at[1]++;
	  pit++;
	} while(it.Next());
	Swap(lastRow,curRow);
      }
#endif
      return true;
    }
    //: Warp an image to align home mesh with the ones in the newPnts
    // This is a crude implementation, for visualisation only.
    
  protected:
    MixerT mixer;
  };

  template<class InT,class OutT,class MixerT>
  ostream & operator<<(ostream & s, const WarpMesh2dC<InT,OutT,MixerT> & mesh) {
    s << ((WarpMesh2dBaseC &) mesh);
    return s;    
  }
  //: Write to stream.
  
  template<class InT,class OutT,class MixerT>
  istream & operator>>(istream & s,WarpMesh2dC<InT,OutT,MixerT> & mesh) {
    s >> ((WarpMesh2dBaseC &) mesh);
    return s;
  }
  //: Read from stream.
  
  template<class InT,class OutT,class MixerT>
  BinOStreamC & operator<<(BinOStreamC & s, const WarpMesh2dC<InT,OutT,MixerT> & mesh) {
    s << ((WarpMesh2dBaseC &) mesh);
    return s;
  }
  //: Write to binary stream.
  
  template<class InT,class OutT,class MixerT>
  BinIStreamC & operator>>(BinIStreamC & s,WarpMesh2dC<InT,OutT,MixerT> & mesh) {
    s >> ((WarpMesh2dBaseC &) mesh);
    return s;
  }
  //: Read from binary stream.
  
}

#endif
