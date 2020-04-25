// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2007-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlImageProc

#include "Ravl/Image/GaborComplex.hh"
#include "Ravl/Math.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/Array2dIter3.hh"
#include "Ravl/Point2d.hh"

using namespace RavlImageN;
using namespace RavlConstN;


GaborComplexC::GaborComplexC (IntT nscale, IntT ntheta, bool isOffset)
  : Frame(0,0),
    Nscale(nscale), Ntheta(ntheta), U(0.35), sratio(2), subsample(1.0),
    sigma_factor(1.0), lambda_factor(1.0), isoffset(isOffset),
    mask(Nscale,Ntheta), init(false)
{}

Array2dC<ImageC<RealT> > GaborComplexC::Apply(const ImageC<RealT>& in)
{
  // set up filters if needed
  if (!init || (in.Frame().Size() != Frame.Size()))
    if(!Init(in.Frame())) return Array2dC<ImageC<RealT> >(0,0);

  // FFT i/p image and shift coords so that d.c. is at centre to match mask
  ImageC<ComplexC> fft_im(FFT2dC::FFTShift(fft_fwd.Apply(in)));
  fft_im.ShiftArray(-fft_im.Frame().Center());

  Array2dC<ImageC<RealT> > out(Nscale,Ntheta);  // array of modulus of results
  for (Array2dIter2C<ImageC<RealT>, ImageC<RealT> > i(mask,out); i; ++i) {
    // result = image * mask in freq domain
    ImageC<ComplexC> result(i.Data1().Frame());
    for (Array2dIter3C<ComplexC,RealT,ComplexC>
           j(result,i.Data1(),fft_im,result.Frame()); j; ++j)
      j.Data1() = j.Data2()*j.Data3();
    // Coords of result are those of mask (i.e. origin in centre)
    // NB we don't bother to rotate freqs back again as we are taking modulus
    // of complex result (so frequency shifts have no effect)
    // apply inverse FFT & take modulus of complex image
    i.Data2() = FFT2dC::Mag(fft_inv.Apply(result));
  }
  return out;
}


ImageC<RealT> GaborComplexC::FilterSpectrum() const 
{
  ImageC<RealT> fft_filt(Frame);
  fft_filt.ShiftArray(-(Frame.Center()));
  fft_filt.Fill(0.0);
  for (Array2dIterC<ImageC<RealT> > i(mask); i; ++i) {
    for (Array2dIter2C<RealT,RealT> j(fft_filt,*i,i->Frame()); j; ++j)
      j.Data1() += j.Data2();
  }
  return fft_filt;
}


bool GaborComplexC::Init(const ImageRectangleC& frame)
{
  // See Gabor.pdf in this directory for theory

  // Set up freq domain frame & coords
  Frame = ImageRectangleC(frame.Rows(), frame.Cols());
  Frame -= Frame.Size()/2;// shift range to put origin in centre
  // Initialise FFTs
  fft_fwd = FFT2dC(Frame.Size());                 // forward FFT
  fft_inv = FFT2dC(Frame.Size()/subsample, true); // inverse FFT
  // sigma is tangential filter size (sigma_theta in doc)
  RealT sigma = Sqrt(log(2.0)/2.0)/(pi*U*sin(pi/(2.0*Ntheta))) * sigma_factor; 
  // lambda is ratio of radial to tangential size, i.e. sigma_r/sigma_theta.
  RealT lambda(1.0); 
  if (isoffset) { // see documentation of explanation of these calculations
    RealT t  = Sin(pi*(Ntheta+1)/(2*Ntheta));
    // s_c is scale ratio to generate circular filters
    RealT s_c = (1.0+t-t*t + Sqrt((1+2.0*t)*(1.0-t*t)))/(t*t);
    if (sratio <= 0.0) sratio = s_c;
    else lambda = (sratio+1.0)/(sratio-1.0) * (s_c-1.0)/(s_c+1.0);
  }
  else
    if (sratio <= 0.0) sratio = (1.0+sin(pi/(2.0*Ntheta))) / (1.0-sin(pi/(2.0*Ntheta)));
    else lambda = sin(pi/(2.0*Ntheta)) * (sratio+1.0)/(sratio-1.0);
  lambda *= lambda_factor;

  // compute the filter spectral responses
  for (IntT iscale=0; iscale < Nscale; ++iscale) {
    RealT scale = pow(sratio, iscale);
    RealT offset = isoffset ? (iscale%2) * pi/(2*Ntheta) : 0;
    for (IntT itheta=0; itheta < Ntheta; ++itheta) {
      RealT theta = itheta*pi/Ntheta + offset;
      // set up mask frame & coords
      if (subsample > 1.0) { 
        // set up filter frame cropped and centred on filter centre
        // (coords rotated 90deg to give more familiar representation)
        Point2dC centreFreq(-Sin(theta)*Frame.Rows()*U/scale,
                             Cos(theta)*Frame.Cols()*U/scale);
        ImageRectangleC smallFrame(centreFreq, (int)(Frame.Rows()/subsample), (int)(Frame.Cols()/subsample));
        // if filter frame not inside image FFT frame, nudge it in
        if (!Frame.Contains(smallFrame)) {
          if (smallFrame.TRow() < Frame.TRow()) {
            smallFrame.BRow() += Frame.TRow() - smallFrame.TRow();
            smallFrame.TRow() += Frame.TRow() - smallFrame.TRow();
          }
          else if (smallFrame.BRow() > Frame.BRow()) {
            smallFrame.TRow() += Frame.BRow() - smallFrame.BRow();
            smallFrame.BRow() += Frame.BRow() - smallFrame.BRow();
          }
          if (smallFrame.LCol() < Frame.LCol()) {
            smallFrame.RCol() += Frame.LCol() - smallFrame.LCol();
            smallFrame.LCol() += Frame.LCol() - smallFrame.LCol();
          }
          else if (smallFrame.RCol() > Frame.RCol()) {
            smallFrame.LCol() += Frame.RCol() - smallFrame.RCol();
            smallFrame.RCol() += Frame.RCol() - smallFrame.RCol();
          }
          if (!Frame.Contains(smallFrame)) return false;
        }
        mask[iscale][itheta] = ImageC<RealT>(smallFrame);
      }
      // otherwise set up filter coords to match image FFT
      else mask[iscale][itheta] = ImageC<RealT>(Frame);

      // fill mask with filter spectrum
      for (Array2dIterC<RealT> i(mask[iscale][itheta]); i; ++i) {
        Index2dC p(i.Index());
        RealT r1 =  p[0]*Cos(theta)/Frame.Rows() + p[1]*Sin(theta)/Frame.Cols();
        RealT c1 = -p[0]*Sin(theta)/Frame.Rows() + p[1]*Cos(theta)/Frame.Cols();
        *i = exp(-2.0*Sqr(pi*sigma)*(Sqr(lambda*(scale*c1-U))+Sqr(scale*r1)));
      }
    }
  }
  init = true;
  return true;
}

