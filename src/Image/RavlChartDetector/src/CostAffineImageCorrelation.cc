// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlChartDetector
//! file="Ravl/Image/Processing/ChartDetector/CostAffineImageCorrelation.cc"

//! author="Charles Galambos"

#include "Ravl/Image/CostAffineImageCorrelation.hh"
#include "Ravl/Image/WarpAffine.hh"
#include "Ravl/Image/NormalisedCorrelation.hh"
#include "Ravl/IO.hh"
#include "Ravl/OS/Date.hh"

namespace RavlImageN {
  //: Constructor.
  //!param:refImage - Image of which we're try to get a match.
  //!param:sceneImage - Scene in which we're searching.
  //!param:est - Initial estimate of transform.
  //!param:range - Range of parameter values over which to search. If null vector will be set automaticly.
  
  
  CostAffineImageCorrelationBodyC::CostAffineImageCorrelationBodyC(const ImageC<ByteT> &nrefImage,
								   const ImageC<ByteT> &nsceneImage,
								   const Affine2dC &est,
								   const VectorC &range) 
    : refImage(nrefImage),
      sceneImage(nsceneImage),
      context(est)
  {
    ParametersC xyz(6);
    if(range.Size() > 0) {
      VectorC start(6);
      start.Fill(0);
      start[2] = 1.0;
      start[5] = 1.0;
      for(int i = 0;i < 6;i++)
	xyz.Setup(i,start[i]-range[i],start[i]+range[i],1000,start[i]);
    } else {
      xyz.Setup(0,-15,15,1000,0.0);
      xyz.Setup(1,-15,15,1000,0.0);
      xyz.Setup(2, 0.7,1.3,1000,1.0);
      xyz.Setup(3,-0.3,0.3,1000,0.0);
      xyz.Setup(4,-0.3,0.3,1000,0.0);
      xyz.Setup(5, 0.7,1.3,1000,1.0);      
    }
    SetParameters(xyz);
  }
  
  //: Convert affine transform to vector form.
  
  VectorC CostAffineImageCorrelationBodyC::Affine2Vector(const Affine2dC &affine) const {
    RavlAssert(0); // Not working, need to account for context.
    VectorC ret(6);
    Affine2dC aff = affine;
    ret[0] = aff.Translation()[0];
    ret[1] = aff.Translation()[1];
    ret[2] = aff.SRMatrix()[0][0];
    ret[3] = aff.SRMatrix()[0][1];
    ret[4] = aff.SRMatrix()[1][0];
    ret[5] = aff.SRMatrix()[1][1];
    return ret;
  }
  
  //: Convert vector to affine transform.
  
  Affine2dC CostAffineImageCorrelationBodyC::Vector2Affine(const VectorC &data) const {
    Vector2dC translation (data[0], data[1]);
    Matrix2dC srMatrix (data[2], data[3], data[4], data[5]);
    return context * Affine2dC(srMatrix, translation);
  }
  
  //: Determines cost of X
  
  RealT CostAffineImageCorrelationBodyC::Cost (const VectorC &x) const {
    //cerr << "X=" << x << "\n";
    WarpAffineC<ByteT> warp(refImage.Frame(),Vector2Affine(x));
    warp.SetMidPixelCorrection(false);
    ImageC<ByteT> result = warp.Apply(sceneImage);
    //RavlN::Save("@X:Ref",refImage);
    //RavlN::Save("@X:Opt",result);
    RealT cost =-NormalisedCorrelation(refImage,result);
    //cerr << "Cost=" << cost << "\n";
    return cost;
  }

  //: Evaluate cost function at X
  
  VectorC CostAffineImageCorrelationBodyC::Apply(const VectorC &X) const {
    VectorC ret(1);
    ret[0] = Cost(X);
    return ret;
  }
  
  //: Calculate Jacobian matrix at X
  
  MatrixC CostAffineImageCorrelationBodyC::Jacobian (const VectorC &X) const {
    RavlAssert(X.Size() == inputSize);
    MatrixC J (outputSize,inputSize);
    VectorC dX (inputSize);
    dX.Fill(0);
    for(UIntT index = 0;index < inputSize;index++) {
      dX[index] = 1e-6;
      VectorC temp = Apply(X+dX) - Apply(X-dX);
      temp /= 2e-6;
      J.SetColumn (index,temp);
      dX[index] = 0;
    }
    return J;
  }
  
}
