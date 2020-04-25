// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_COSTAFFINEIMAGECORRELATION_HEADER
#define RAVLIMAGE_COSTAFFINEIMAGECORRELATION_HEADER 1
//! author="Charles Galambos"
//! docentry="Ravl.API.Images.Correlation"
//! rcsid="$Id$"
//! lib=RavlChartDetector
//! file="Ravl/Image/Processing/ChartDetector/CostAffineImageCorrelation.hh"

#include "Ravl/PatternRec/Cost.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Affine2d.hh"

namespace RavlImageN {

  //! userlevel=Develop
  //: Cost function for affine correlation of two images.
  
  class CostAffineImageCorrelationBodyC
    : public CostBodyC
  {
  public:
    CostAffineImageCorrelationBodyC(const ImageC<ByteT> &refImage,const ImageC<ByteT> &sceneImage,const Affine2dC &est,const VectorC & range = VectorC ());
    //: Constructor.
    //!param:refImage - Image of which we're try to get a match.
    //!param:sceneImage - Scene in which we're searching.
    //!param:est - Initial estimate of transform.
    //!param:range - Range of parameter values over which to search. If null vector will be set automaticly.
    // Parameter vector consists of row,col,sr[0][0],sr[0][1],sr[1][0],sr[1][1] where sr is the scale rotation
    // matrix used in Affine2dC.
    
    virtual VectorC Apply (const VectorC &X) const;
    //: Evaluate cost function at X
    
    virtual RealT Cost (const VectorC &X) const;
    //: Determines cost of X
    
    virtual MatrixC Jacobian (const VectorC &X) const;
    //: Calculate Jacobian matrix at X
    
    VectorC Affine2Vector(const Affine2dC &affine) const;
    //: Convert affine transform to vector form.
    
    Affine2dC Vector2Affine(const VectorC &vec) const;
    //: Convert vector to affine transform.
    
  protected: 
    ImageC<ByteT> refImage;
    ImageC<ByteT> sceneImage;
    Affine2dC context;
  };
  
  //! userlevel=Normal
  //: Cost function for affine correlation of two images. 
  // Used to compute affine image correlations in the <a href="../Tree/Ravl.API.Images.Chart_Detector.html">chart detector</a>.

  class CostAffineImageCorrelationC
    : public CostC
  {
  public:
    CostAffineImageCorrelationC(const ImageC<ByteT> & refImage,const ImageC<ByteT> & sceneImage,const Affine2dC & est,const VectorC & range = VectorC ()) 
      : CostC(*new CostAffineImageCorrelationBodyC(refImage,sceneImage,est,range))
    {} 
    //: Constructor.
    //!param:refImage - Image of which we're try to get a match.
    //!param:sceneImage - Scene in which we're searching.
    //!param:est - Initial estimate of transform.
    //!param:range - Range of parameter values over which to search. If null vector will be set automaticly.
    // Parameter vector consists of row,col,sr[0][0],sr[0][1],sr[1][0],sr[1][1] where sr is the scale rotation
    // matrix used in Affine2dC.
    
    VectorC Apply(const VectorC & X) const
    { return Body().Apply(X); }
    //: Evaluate cost function at X 
    
    RealT Cost(const VectorC & X) const
    { return Body().Cost(X); }
    //: Determines cost of X 
    
    MatrixC Jacobian(const VectorC & X) const
    { return Body().Jacobian(X); }
    //: Calculate Jacobian matrix at X 
    
    VectorC Affine2Vector(const Affine2dC & affine) const
    { return Body().Affine2Vector(affine); }
    //: Convert affine transform to vector form. 
    
    Affine2dC Vector2Affine(const VectorC & vec) const
    { return Body().Vector2Affine(vec); }
    //: Convert vector to affine transform. 
    
  protected:
    CostAffineImageCorrelationC(CostC &bod)
     : CostC(bod)
    {}
    //: Body constructor. 
    
    CostAffineImageCorrelationBodyC& Body()
    { return static_cast<CostAffineImageCorrelationBodyC &>(CostC::Body()); }
    //: Body Access. 
    
    const CostAffineImageCorrelationBodyC& Body() const
    { return static_cast<const CostAffineImageCorrelationBodyC &>(CostC::Body()); }
    //: Body Access. 
    
  };
  
}


#endif
