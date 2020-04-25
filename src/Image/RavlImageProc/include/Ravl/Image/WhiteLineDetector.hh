// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef WHITELINEDETECTOR_HH
#define WHITELINEDETECTOR_HH 1

//! docentry="Ravl.API.Images.Lines"
//! lib=RavlImageProc
//! author="Bill Christmas"

#include "Ravl/Image/GaussConvolve2d.hh"
#include "Ravl/Image/EdgeDetector.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Image/PPHT.hh"
#include "Ravl/LinePP2d.hh"
#include "Ravl/RefCounter.hh"

namespace RavlImageN {

  using namespace RavlN;
  using namespace RavlImageN;
  
  //: Body for WhiteLineDetectorC
  class WhiteLineDetectorBodyC : public RavlN::RCBodyC
  {
    
  private:
    const ByteRGBValueC darkred, red, yellow, brown;
    GaussConvolve2dC<RealT> gFilter; // Gaussian filter
    EdgeDetectorC edgeDet;
    PPHoughTransformC pphtProc; // Progressive probabilistic Hough transform
    
    RealT maxAngle; // Max angle between edge pair to be ridge (degrees)
    RealT maxSep;   // Max separation between edge pair to be ridge (pixels)
    RealT overlap;  // Min overlap of edges to be ridge (fraction of edge length)
    IntT  minLen;   // Min line length to be included (currently both PPHT and ridge finding)
    ImageC<RealT> image;
    ImageC<ByteRGBValueC> canvas;
    SArray1dC<LinePP2dC> ridgeArray;

  public:
    WhiteLineDetectorBodyC();
    //: Constructor

    void SetEdgeDet(IntT blur, RealT HystLower, RealT HystUpper) {
      gFilter = GaussConvolve2dC<RealT>(blur);
      edgeDet = EdgeDetectorC(true, HystLower, HystUpper);
    }
    //: Set the edge detector parameters

    void SetHoughResolution(RealT Distance, RealT Angle)
    { pphtProc = PPHoughTransformC(Point2dC(Angle, Distance),0.9999999,90,-1,false,1.4,true); }
    //: Set the resolution of the probabilistic Hough transform

    void SetHoughParameters(RealT CWidth, IntT  MaxGap, IntT  MinLen) {
      pphtProc.SetCorridor(CWidth);
      pphtProc.SetMaxGap(MaxGap);
      pphtProc.SetMinLineLength(MinLen);
      minLen = MinLen;
    }
    //: Set the line parameters of the probabilistic Hough transform
    
    void SetRidgeParameters(RealT MaxAngle,  RealT MaxSep, RealT Overlap)
    { maxAngle = MaxAngle; maxSep = MaxSep; overlap = Overlap; }
    //: Set the parameters for finding the white line from the Hough o/p

    SArray1dC<LinePP2dC> Apply(const ImageC<RealT> &img);
    //: Returns white lines found in <code>img</code> 

    ImageC<ByteRGBValueC> Image()
    { return canvas; }
    //: Returns input image with lines overlaid
    // Yellow lines are candidate edges; red lines are detected white lines

    SArray1dC<RealT> MeanStrength();
    //: Returns mean grey-level value of line

    SArray1dC<RealT> MedianStrength();
    //: Returns median grey-level value of line
  };
  
  //! userlevel=Normal
  //: Detects straight white lines in images
  // Typical use would be for detecting court or pitch markings in tennis or football.
  
  class WhiteLineDetectorC
    : public RCHandleC<WhiteLineDetectorBodyC>
  {
  public:
    WhiteLineDetectorC() 
      : RCHandleC<WhiteLineDetectorBodyC>(*new WhiteLineDetectorBodyC())
    {}
    //: Constructor 
    
    void SetEdgeDet(IntT Blur,RealT HystLower,RealT HystUpper) 
    { Body().SetEdgeDet(Blur,HystLower,HystUpper); }
    //: Sets the edge detector parameters 
    //!param: Blur - Edge detector: image blur mask width using <a href="RavlImageN.GaussConvolve2dC.html">GaussConvolve2dC</a> (default: 5)
    //!param: HystLower - Edge detector: lower hysteresis threshold (default: 0.3)
    //!param: HystUpper - Edge detector: upper hysteresis threshold (default: 1.0)
    
    void SetHoughResolution(RealT Distance,RealT Angle) 
    { Body().SetHoughResolution(Distance,Angle); }
    //: Sets the resolution of the probabilistic Hough transform 
    //!param: Distance - Position resolution (pixels) (default: 1.0)
    //!param: Angle - Angle resolution (radians) (default: 0.01)
    
    void SetHoughParameters(RealT CWidth,IntT MaxGap,IntT MinLen) 
    { Body().SetHoughParameters(CWidth,MaxGap,MinLen); }
    //: Sets the line parameters of the probabilistic Hough transform 
    //!param: CWidth - Line corridor width (pixels) (default: 4)
    //!param: MaxGap - Maxium size of gap in line (pixels) (default: 25)
    //!param: MinLen - Minimum line length to be included (pixels) (default: 50)
    
    void SetRidgeParameters(RealT MaxAngle,RealT MaxSep,RealT Overlap) 
    { Body().SetRidgeParameters(MaxAngle,MaxSep,Overlap); }
    //: Sets the parameters for finding the white line from the Hough o/p 
    //!param: MaxAngle - Max angle between edge pair to be ridge (degrees) (default: 1.0)
    //!param: MaxSep   - Max separation between edge pair to be ridge (pixels) (default: 6)
    //!param: Overlap  - Min overlap of edges to be ridge (fraction of edge length) (default: 0.5)
    
    SArray1dC<LinePP2dC> Apply(const ImageC<RealT> & img) 
    { return Body().Apply(img); }
    //: Returns white lines found in <code>img</code> 
    
    ImageC<ByteRGBValueC> Image()
    { return Body().Image(); }
    //: Returns input image with lines overlaid
    // Yellow lines are candidate edges; red lines are detected white lines

    SArray1dC<RealT> MeanStrength()
    { return Body().MeanStrength(); }
    //: Returns mean grey-level value of line

    SArray1dC<RealT> MedianStrength()
    { return Body().MedianStrength(); }
    //: Returns median grey-level value of line

  protected:
    WhiteLineDetectorC(WhiteLineDetectorBodyC &bod)
     : RCHandleC<WhiteLineDetectorBodyC>(bod)
    {}
    //: Body constructor. 
    
    WhiteLineDetectorBodyC& Body()
    { return static_cast<WhiteLineDetectorBodyC &>(RCHandleC<WhiteLineDetectorBodyC>::Body()); }
    //: Body Access. 
    
    const WhiteLineDetectorBodyC& Body() const
    { return static_cast<const WhiteLineDetectorBodyC &>(RCHandleC<WhiteLineDetectorBodyC>::Body()); }
    //: Body Access. 
    
  };
  
  
  
}

#endif
