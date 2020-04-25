// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2007, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef GABORCOMPLEXC
#define GABORCOMPLEXC 1
//! docentry="Ravl.API.Images.Filtering"
//! author="Bill Christmas"
//! example="exGaborComplex.cc"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Filters/Gabor/GaborComplex.hh"

#include "Ravl/Image/Image.hh"
#include "Ravl/Array2d.hh"
#include "Ravl/FFT2d.hh"

namespace RavlImageN {
using namespace RavlN;

  //! userlevel=Normal
  //! example = exGaborComplex.cc

  //: Gabor filters

  // <p>This class generates a set of complex Gabor filters of varying
  // frequency and orientation.  For more detail, see <a href=
  // "../../../html/Gabor.pdf">complex Gabor filter theory</a>.</p>
  //
  // <p>Remember to ensure that both input and outout image sizes are highly
  // composite to ensure an efficient FFT.</p>


  class GaborComplexC {
    
  public:

  GaborComplexC (IntT nscale, IntT ntheta, bool isOffset=false);
  //: Constructor on size of image, no. of orientations, no. of scales
  //!param: isOffset - if true, every other scale is offset by &frac12; angular spacing 

  Array2dC<ImageC<RealT> > Apply(const ImageC<RealT>& in_img);
  //: Filters image <code>in_img</code> using Gabor filter bank
  // Returns the modulus of the complex images<br>
  // Array is organised as: no. of angles &times; no. of scales

  void SetFreq(RealT freq)
  { U = freq; init=false; }
  //: Sets centre frequency magnitude of highest frequency filters
  // Centre frequency is expressed as fraction of sampling frequency<br>
  // Default is 0.35, chosen as compromise between clipping the response and covering the whole spectrum. 

  void SetRadialScale(RealT ratio)
    { sratio = ratio; init = false;}
  //: Sets the ratio of centre frequencies of successive scales
  // <code>ratio</code> should be &gt; 1.<br> 
  // If <code>ratio</code> &lt; 0, ratio is instead chosen to make filters circular.<br>
  // Default is 2

  void SetSubsample(RealT factor)
    { subsample = factor; init = false;}
  //: Subsamples final image to speed up the inverse FFTs
  // Default: no subsampling.<br>
  // Subsampling is performend by cropping the filtered spectrum by <code>factor</code>, centred on filter centre.
  // The result is that the output image is subsampled by <code>factor</code> (>=1).<br>
  // Where part of the cropped would lie partly outside the spectrum, the cropped region is shifted to avoid this. <br>
  // <b>N.B.</b> Depending on the arrangement of filters in the spectrum and the subsampling factor, some areas of the spectrum may be omitted entirely.  Use the <code>FilterSpectrum()</code> method to view the spectral coverage

  void AdjustSigma(RealT factor)
    { sigma_factor = factor; init=false; }
  //: Multiplies size of filter in <i>spatial</i> domain by <code>factor</code>

  void AdjustLambda(RealT factor)
    { lambda_factor = factor; init=false; }
  //: Adjusts &sigma;_r so that  ratio of radial to tangential filter widths in <i>spatial</i> domain = <code>factor</code>
  // This is in addition to the adjustment to &sigma;_r provided by <code>AdjustSigma()</code>

  const Array2dC<ImageC<RealT> > Mask() const
    { return mask; }
  //: Const access to frequency masks as 2D array
  // Array is organised as: no. of angles &times; no. of scales

  ImageC<RealT> FilterSpectrum() const;
  //: Returns an image of all of the filter spectra superimposed.

 protected:

  ImageRectangleC Frame;// FFT size, with centred coord system
  FFT2dC fft_fwd;       // forward FFT
  FFT2dC fft_inv;       // inverse FFT
  IntT Nscale;          // no. of filter scales
  IntT Ntheta;          // no. of filter angles
  RealT U;     // offset of prototype filter centre frequency as proportion of sampling frequency
  RealT sratio;         // ratio of filter scales: consecutive sigma_r's
  RealT subsample;      // output image is subsampled by this factor (>=1)
  RealT sigma_factor;   // manual adjustment to sigma
  RealT lambda_factor;  // manual adjustment to lambda
  bool isoffset;        // if true, alternate scales are offset by 1/2 filter separation
  Array2dC<ImageC<RealT> > mask; // frequency domain filter kernels
  bool init;

  bool Init(const ImageRectangleC& frame);
  //: Initialises frequency domain masks
  // Called each time a parameter is altered or image size changes

};

}

#endif
