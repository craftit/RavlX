// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! file="Ravl/Image/Processing/Tracking/MosaicBuilder.cc"
//! lib=RavlImageProc

#include "Ravl/Array2dIter2.hh"
#include "Ravl/Array2dIter3.hh"
#include "Ravl/Projection2d.hh"
#include "Ravl/Image/MosaicBuilder.hh"
#include "Ravl/Image/ImageMatcher.hh"
#include "Ravl/Image/ForegroundSep.hh"
#include "Ravl/Image/WarpProjective.hh"
#include "Ravl/Image/RemoveDistortion.hh"
#include "Ravl/Image/ImageConv.hh"
#include "Ravl/Image/Erode.hh"
#include "Ravl/Image/Dilate.hh"
#include "Ravl/Image/RealRGBValue.hh"

#include "Ravl/IO.hh"
#include "Ravl/Image/ImgIO.hh"
#define DODEBUG 0
#if DODEBUG
#include "Ravl/Image/DrawCross.hh"
#include "Ravl/Image/DrawFrame.hh"
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {
  using namespace RavlN;

  //: Constructor for mosaic builder
  MosaicBuilderBodyC::MosaicBuilderBodyC(autoResizeT nresize, bool nVerbose, OStreamC& Log)
    : resize(nresize),
      cropT(0), cropB(0), cropL(0), cropR(0),
      pointTL(0.0, 0.0), pointTR(0.0, 1.0),
      pointBL(1.0, 0.0), pointBR(1.0, 1.0),
      zhomog(100), mosaicZHomog(1),
      K1(0), K2(0), cx_ratio(0.5), cy_ratio(0.5), fx(1.0), fy(1.0),
      filterSubsample(1), updateSearchSize(5), 
      trackingHomogs(nVerbose), Parray(0), frameNo(0),
      verbose(nVerbose),
      log(Log)
  { SetProjectiveScale(zhomog, mosaicZHomog); }

  //: Deprecated constructor
  MosaicBuilderBodyC::MosaicBuilderBodyC(
	IntT cthreshold, IntT cwidth, IntT mthreshold, IntT mwidth,
	IntT lifeTime, IntT searchSize, IntT newFreq,
	IntT nborderC, IntT nborderR, RealT nzhomog,
	IntT ncropT, IntT ncropB, IntT ncropL, IntT ncropR,
	const Point2dC &npointTL, const Point2dC &npointTR,
	const Point2dC &npointBL, const Point2dC &npointBR,
	IntT nmaxFrames, const ImageC<bool>& nMask, bool nVerbose)
  : borderC(nborderC), borderR(nborderR), resize(none),
    cropT(ncropT), cropB(ncropB), cropL(ncropL), cropR(ncropR),
    pointTL(npointTL), pointTR(npointTR), pointBL(npointBL), pointBR(npointBR),
    zhomog(nzhomog), mosaicZHomog(1), 
    K1(0), K2(0), cx_ratio(0.5), cy_ratio(0.5), fx(1), fy(1),
    filterSubsample(1), trackingHomogs(nVerbose), Parray(0),
    frameNo(0),
    verbose(nVerbose)
  { SetProjectiveScale(zhomog, mosaicZHomog); }


  //: Set binary mask to exclude regions from tracker
  bool MosaicBuilderBodyC::SetMask(const StringC& fileName) {
    if (!Load(fileName, mask)) {
      std::cerr << "Failed to load file: " << fileName << std::endl;
      return false;
    }
    trackingHomogs.SetMask(mask);
    return true;
  }


  //: Creates the mosaic from a sequence
  bool MosaicBuilderBodyC::GrowMosaic(const StringC& ipFile, IntT startFrame, UIntT noOfFrames) {
    // open i/p video stream
    if (!OpenSequence(ipFile, startFrame)) exit (-1);
    // Compute homographies & warp frames
    ImageC<ByteRGBValueC> img;
    if (resize == twopass) { // two-pass version
      for (; frameNo < noOfFrames; ++frameNo) {
	if (verbose)  log << "Tracking frame " << frameNo << std::endl;
	if (!GetImage(img))  break;
	if (!PrepareFrame(img)) break; 
	if (!FindProj(img)) break;
	InvolveFrame(img.Rectangle(), Parray[frameNo]);
      }
      mosaic = ImageC<ByteRGBMedianC>(mosaicRect);
      input.Seek(startFrame);
      for (frameNo = 0; frameNo < Parray.Size(); ++frameNo) {
	if (!GetImage(img))  break;
	if (frameNo % filterSubsample == 0) {
	  if (verbose)  log << "Warping frame " << frameNo << std::endl;
	  if (!PrepareFrame(img)) break;
	  WarpFrame(img);
	}
      }
    }
    else { // one-pass version
      while(frameNo < noOfFrames) {
	if (verbose)  log << "Processing frame " << frameNo << std::endl;
	if (!GetImage(img))  break;
	if (!Apply(img)) break;
      }
    }
    return true;
  }


  //: Add an image to the mosaic.
  bool MosaicBuilderBodyC::Apply(ImageC<ByteRGBValueC> &img)
{
    if (!PrepareFrame(img)) return false; 
    if (!FindProj(img)) return false;  // process next image to get projection info
    if (frameNo % filterSubsample == 0) {
      // If we are resizing the mosaic as we go, and mosaic rectangle needs 
      // expanding, then we need to copy mosaic to a new, bigger image.
      if (InvolveFrame(img.Rectangle(),Parray[frameNo]) && (resize==onepass)) {
	if (verbose)  log << "Mosaic was expanded to " << img.Rectangle() << std::endl;
	ExpandMosaic();
      }
      WarpFrame(img);
    }

#if DODEBUG
    ImageC<ByteRGBValueC> timg(img.Copy());

    // Draw red boxes around the corners.
    ByteRGBValueC val(255,0,0);
    for(HashIterC<UIntT,PointTrackC> cit(corners);cit;cit++) {
      DrawCross(timg,val,cit->Location());
    }
    
    // Draw green crosses around the selected corners
    val = ByteRGBValueC(0,255,0);
    for(DLIterC<ObservationC> oit(compatibleObsList);oit;oit++) {
      const VectorC &z=oit->GetZ();
      Point2dC p(z[0],z[1]);
      IndexRange2dC rect(p,5,5);
      DrawFrame(timg,val,rect);
    }

    // Write the image with tracks out.
    RavlN::Save("@X:Matches",timg);
#endif
    frameNo++;
    return true;
  }


  //: Open file for mosaicing
  bool MosaicBuilderBodyC::OpenSequence(const StringC& ipFile, IntT startFrame) {
    if(!OpenISequence(input,ipFile)) {
      std::cerr << "Failed to open input sequence `" << ipFile << "'" << std::endl;
      return false;
    }
    if (startFrame > 0)  input.Seek(startFrame);
    return true;
  }
  

  //: Gets next frame
  bool MosaicBuilderBodyC::PrepareFrame(ImageC<ByteRGBValueC>& img) {
    if (K1!=0.0 || K2!=0.0) {
      ImageC<ByteRGBValueC> linearImg(img.Copy());
      RemoveDistortionC<ByteRGBValueC,ByteRGBValueC> distortionRemover(
	cx_ratio*(RealT)img.Rows(), cy_ratio*(RealT)img.Cols(),
	fx, fy, K1, K2);
      distortionRemover.Apply(linearImg,img);
    }
    if (frameNo == 0) {
      // set frame from first image
      cropRect = IndexRange2dC(img.Frame());
      // adjust frame for cropping region
      cropRect.TRow() += cropT; cropRect.BRow() -= cropB;
      cropRect.LCol() += cropL; cropRect.RCol() -= cropR;
    }
    if (!cropRect.IsValid()) {
      std::cerr<<"Cropped image rectangle too small at frame no. "<<frameNo<<endl;
      img = ImageC<ByteRGBValueC>();
      return false;
    }
    img = ImageC<ByteRGBValueC>(img, cropRect);
    return true;
  }


  //: computes homography between current frame and mosaic
  bool MosaicBuilderBodyC::FindProj(const ImageC<ByteRGBValueC> &img) {
    // frame 0 is a special case
    if (frameNo == 0)  {
      trackingHomogs.Reset(RGBImageCT2ByteImageCT(img));
      Projection2dC mosaicHomog(Im2Mosaic(img));
      Parray.Append(mosaicHomog);
      if (verbose) log << "Mosaic homography:\n"<< Parray[0]<<endl;
      return mosaicHomog.IsValid();
    }
    else {
      Projection2dC P = trackingHomogs.Apply(RGBImageCT2ByteImageCT(img));
      // accumulate homography
      Psum = P*Psum;
      Projection2dC Pacc(Psum*Pmosaic);
      MatchToMosaic(img, Pacc);
      Parray.Append(Pacc);
      return P.IsValid();
    }
  }


  //: Expand mosaic rectangle to include projected frame corners
  bool MosaicBuilderBodyC::InvolveFrame(const IndexRange2dC& rect, const Projection2dC& homog) {
    Projection2dC warp(homog.Inverse());
    if (frameNo == 0) { 
      // initialise rectangle at pixel guaranteed in projected image
      mosaicRect = IndexRange2dC(warp.Project(rect.Center()), 1); 
    }
    IndexRange2dC oldRect = mosaicRect;
    mosaicRect.Involve(warp.Project(rect.TopLeft()));
    mosaicRect.Involve(warp.Project(rect.TopRight()));
    mosaicRect.Involve(warp.Project(rect.BottomLeft()));
    mosaicRect.Involve(warp.Project(rect.BottomRight()));
    if (frameNo == 0) {
      mosaicRect.TRow() -= borderR;
      mosaicRect.LCol() -= borderC;
      mosaicRect.BRow() += borderR;
      mosaicRect.RCol() += borderC;
    }
    return (oldRect != mosaicRect);
  }

  
  //: Expands mosaic to fit expanded mosaic rectangle
  void MosaicBuilderBodyC::ExpandMosaic() {
    // Create expanded mosaic with no median info in it
    ImageC<ByteRGBMedianC> newMosaic(mosaicRect);
    if (mosaic.IsValid()) {
      for(Array2dIter2C<ByteRGBMedianC,ByteRGBMedianC> it(mosaic, newMosaic, mosaic.Rectangle()); it;it++ )
	it.Data2() = it.Data1();
    }
    mosaic = newMosaic;
  }

  //: Warps current frame into mosaic coords
  void MosaicBuilderBodyC::WarpFrame(const ImageC<ByteRGBValueC>& img) {
    if (frameNo == 0) {
      mosaic = ImageC<ByteRGBMedianC>(mosaicRect);
    }
    WarpProjectiveC<ByteRGBValueC,ByteRGBMedianC,PixelMixerRecursiveC<ByteRGBValueC,ByteRGBMedianC> > pwarp(mosaic.Rectangle(),Parray[frameNo].Inverse(),false);
    pwarp.Apply(img,mosaic);
  }


  // Generates foreground image sequence
  bool MosaicBuilderBodyC::GenerateForeground(IntT startFrame, UIntT noOfFrames, DPOPortC<ImageC<ByteRGBValueC> >& outp, int fgThreshold) {
    // find start of sequence
    input.Seek(startFrame);
    // set up foreground separator
    ForegroundSepC fgSep(GetMosaic(), fgThreshold);
    if (mask.IsValid()) fgSep.SetMask(mask);  // exclude unwanted regions
    ImageC<ByteRGBValueC> img;
    // compute foreground segmented image
    for(frameNo = 0; frameNo < noOfFrames; frameNo++) {

      // Read an image from the input, crop, & correct for lens distortion
      if(!input.Get(img))  break;
      PrepareFrame(img) ;
      // Separate out the f/g
      Projection2dC homog (GetMotion(frameNo));
      img = fgSep.Apply(img, homog);
      // Write an image out.
      outp.Put(img);
    }
    return true;
  }

  //: Computes the homography between the first frame and the mosaic
  Projection2dC MosaicBuilderBodyC::Im2Mosaic(const ImageC<ByteRGBValueC> &img) {    
    // initialise accumulated motion Psum by solving for transformation
    // from mosaic coords to image coords
    //  p1 is array of frame corners
    //  p2 is array of corners of reference frame in the mosaic coords
    SArray1dC<Vector2dC> p1(4), p2(4);
    MatrixC A(8,8);
    A.Fill(0.0);
    VectorC b(8);
    
    p1[0][0] =               0.0; p1[0][1] =               0.0;
    p1[1][0] =               0.0; p1[1][1] = (RealT)img.Cols();
    p1[2][0] = (RealT)img.Rows(); p1[2][1] =               0.0;
    p1[3][0] = (RealT)img.Rows(); p1[3][1] = (RealT)img.Cols();
    p2[0][0] = pointTL[0] * (RealT)img.Rows();
    p2[0][1] = pointTL[1] * (RealT)img.Cols();
    p2[1][0] = pointTR[0] * (RealT)img.Rows();
    p2[1][1] = pointTR[1] * (RealT)img.Cols();
    p2[2][0] = pointBL[0] * (RealT)img.Rows();
    p2[2][1] = pointBL[1] * (RealT)img.Cols();
    p2[3][0] = pointBR[0] * (RealT)img.Rows();
    p2[3][1] = pointBR[1] * (RealT)img.Cols();
    
    for(IntT i=0; i < 4; i++) {
      A[i*2][0] = p1[i][0]*zhomog; 
      A[i*2][1] = p1[i][1]*zhomog;
      A[i*2][2] = zhomog*zhomog;
      A[i*2][6] = -p1[i][0]*p2[i][0];
      A[i*2][7] = -p1[i][1]*p2[i][0];
      b[i*2] = zhomog*p2[i][0];
      A[i*2+1][3] = p1[i][0]*zhomog;
      A[i*2+1][4] = p1[i][1]*zhomog;
      A[i*2+1][5] = zhomog*zhomog;
      A[i*2+1][6] = -p1[i][0]*p2[i][1];
      A[i*2+1][7] = -p1[i][1]*p2[i][1];
      b[i*2+1] = zhomog*p2[i][1];
    }
    
    // solve for solution vector
    SolveIP(A,b);
    Psum = Projection2dC(Matrix3dC(b[0], b[1], b[2],
		     b[3], b[4], b[5],
		     b[6], b[7], 1.0), zhomog, zhomog);
    Psum = Psum.Inverse();
    return Psum*Pmosaic;    
    // So at this point, the result will transform a point from mosaic coords to frame 0 coords, using the "mosaic" coordinate system
  }
  
  //: Improves inter-frame homography already calculated, by matching image to mosaic
  bool MosaicBuilderBodyC::MatchToMosaic(const ImageC<ByteRGBValueC>& img, Projection2dC& proj) {
    if (updateSearchSize == 0) return false;
    try  {
      // use proj as estimate to match image with current mosaic & update homog
      if (verbose) log << "Unrefined homography:\n" << proj << std::endl;
      ImageMatcherC matchUpdate(RGBImageCT2ByteImageCT(GetMosaic()),
				0, 0, 0, 0, 17, updateSearchSize, 20,
				mosaicZHomog, zhomog);
      Projection2dC projCopy(proj);
      if(!matchUpdate.Apply(RGBImageCT2ByteImageCT(img), projCopy)) {
	return false;
      }
      proj = projCopy;
      if (verbose) log << "Refined homography:\n" << proj << std::endl;
    }
    catch (...) {
      std::cerr << "Exception caught in mosaic image matcher." << std::endl;
      return false;
    }
    return true;
  }


  //: Returns the mosaic image
  const ImageC<ByteRGBValueC> MosaicBuilderBodyC::GetMosaic() const { 
    return ByteRGBMedianImageC2ByteRGBImageCT(mosaic);
  }



}
