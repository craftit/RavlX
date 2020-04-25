// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_LMSMULTISCALEMOTION_HEADER
#define RAVLIMAGE_LMSMULTISCALEMOTION_HEADER  1
//! userlevel=Normal
//! author="Bill Christmas"
//! date="14/07/2000"
//! rcsid="$Id$"
//! lib=RavlImageProc
//! docentry="Ravl.API.Images.Motion.Optical Flow"
//! example=exLMSMultiScale.cc
//! file="Ravl/Image/Processing/Motion/LMSGradient/LMSMultiScaleMotion.hh"

#include "Ravl/Image/Image.hh"
#include "Ravl/Image/LMSOpticFlow.hh"
#include "Ravl/Image/ConvolveSeparable2d.hh"

namespace RavlImageN {
  
  //: Performs multiscale motion estimation
  //
  // Uses LMS optic flow and motion compensation at each level
  
  class LMSMultiScaleMotionC {
    
  public:

    LMSMultiScaleMotionC(const ConvolveSeparable2dC<RealT> &filter, const LMSOpticFlowC &flow);
    //: Constructor:
    //!param: filter - subsampling antialias filter
    //!param: flow - optical flow engine
    
    LMSMultiScaleMotionC();
    //: Default constructor.
    // Creates motion estimate with a 2:1 antialias filter.
    
    ImageC<Vector2dC> Estimate(const PairC<ImageC<RealT> > &im, IndexC Levels=3);
    // Generates optical flow field from a pair of images, using not
    // more than "Levels" levels of hierarchy

    inline LMSMultiScaleMotionC Verbose(RealT VectorScale=1, RealT VectorSubsample=1)
    { verbose=true; scale=VectorScale; vec_subsample=VectorSubsample; return *this;}
    //: Image, PostScript and data files of the o/p from each level are written
    //!param: VectorScale - rescales motion vectors and ellipses in Postscript images
    //!param: VectorSubsample - subsamples the displayed motion vectors; e.g. =4 will subsample the vectors by 4 at level 0, by 2 at level 1, full fesolution at level 2

    ImageC<Matrix2dC> MotionCov(const int reg, const RealT n);
    //: Compute the covriance matrices of the pixels according to the estimated motion. 
    
  private:
    
    
    ImageC<Vector2dC> EstimateRecursive(const PairC<ImageC<RealT> > &im, IndexC levels);
    // recursively generates optical flow field from a pair of images, using not
    // more than "levels" levels of hierarchy
    
    ConvolveSeparable2dC<RealT> filter; // subsampling filter
    bool verbose;
    RealT scale;
    RealT vec_subsample; // top-level motion vector display subsampling factor 
    RealT noise;
    IntT region;
    
    LMSOpticFlowC optical_flow;
    IndexC top_level;

    ImageC<Vector2dC> BottomLevel(const PairC<ImageC<RealT> > &im_pair, IndexC times);
    PairC<ImageC<RealT> > filtered;  // for the covariance matrix computation;
    ImageC<Vector2dC> motvec_map;
  };

}
#endif
