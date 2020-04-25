// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_MOSAICBUILDER_HEADER
#define RAVLIMAGE_MOSAICBUILDER_HEADER 1
//! userlevel=Normal
//! author="Phil McLauchlan, Bill Christmas"
//! date="24/7/2002"
//! rcsid="$Id$"
//! docentry="Ravl.API.Images.Mosaic"
//! file="Ravl/Image/Processing/Tracking/MosaicBuilder.hh"
//! lib=RavlImageProc
//! example="mosaic.cc"

#include "Ravl/Index.hh"
#include "Ravl/Projection2d.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Image/ByteRGBMedian.hh"
#include "Ravl/RCHash.hh"
#include "Ravl/DP/SequenceIO.hh"
#include "Ravl/DArray1d.hh"
#include "Ravl/Image/TrackingHomog.hh"

namespace RavlImageN {
  
  typedef enum{none, onepass, twopass} autoResizeT;

  //! userlevel=Develop
  //! autoLink=on
  //: Mosaic building class body
  
  class MosaicBuilderBodyC
    : public RCBodyVC
  {
  public:
    MosaicBuilderBodyC(autoResizeT resize, bool nVerbose, OStreamC& log);
    //: Constructor
    // Initialises mosaic builder
    //!param: resize - Determines how the mosaic expands; can take values: none, onepass, twopass
    
    MosaicBuilderBodyC(
	IntT cthreshold, IntT cwidth, IntT mthreshold, IntT mwidth,
	IntT lifeTime, IntT searchSize, IntT newFreq,
	IntT borderC, IntT borderR, RealT zhomog,
	IntT cropT, IntT cropB, IntT cropL, IntT cropR,
	const Point2dC &npointTL, const Point2dC &npointTR,
	const Point2dC &npointBL, const Point2dC &npointBR,
	IntT maxFrames, const ImageC<bool>& nMask, bool nVerbose);
    //: Constructor
    // Initialises mosaic builder
    //!deprecated: use previous constructor + <code>Set...()</code> methods


    //:-
    //: These methods set various parameters for the class.  
    // They should be called before any images are processed.

    void SetTracker(IntT cthreshold, IntT cwidth, IntT mthreshold, IntT mwidth,
		    IntT lifeTime, IntT searchSize, IntT newFreq)
      { trackingHomogs.SetTracker(cthreshold,cwidth,mthreshold,mwidth,
				  lifeTime,searchSize,newFreq); }
    //: Set tracker parameters
    // Defaults are respectively: 30, 7, 20, 17, 8, 25, 1

    void SetCropRegion(IntT ncropT, IntT ncropB, IntT ncropL, IntT ncropR)
      { cropT=ncropT, cropB=ncropB, cropL=ncropL, cropR=ncropR; }
    //: Set border region that is to be ignored
    // This region is removed from the frames for tracking and building mosaic

    void SetMask(const ImageC<bool>& nmask)
      { mask = nmask; trackingHomogs.SetMask(nmask); }
    //: Set binary mask to exclude graphics regions and suchlike from the process
    // Mask coordinate system should match that of image sequence.
    // Pixels set to false are excluded from tracker and foreground.
    // Unlike SetCropRegion, the mask regions are not excluded from the mosaic.

    bool SetMask(const StringC& fileName);
    //: Set binary mask to exclude graphics regions and suchlike from the process
    // Mask coordinate system should match that of image sequence.
    // Pixels set to false are excluded from tracker and foreground.
    // Unlike SetCropRegion, the mask regions are not excluded from the mosaic.

    void SetCoordinates(const Point2dC &npointTL,
			const Point2dC &npointTR,
			const Point2dC &npointBL,
			const Point2dC &npointBR)
      { pointTL = npointTL, pointTR = npointTR,
	pointBL = npointBL, pointBR = npointBR; }
    //: Set the coordinate system of the mosaic
    // It does this by defining projected positions of corners of first frame in sequence.<br>
    // Note: the units of the arguments are in fractions of image height / width.<br>
    // Default: uses the coordinate frame of the first frame in sequence.

    void SetBorderExpansion(IndexC nborderR, IndexC nborderC)
      { borderR = nborderR, borderC = nborderC; }
    //: Set amount by which mosaic border is expanded from default.
    // Default: border is set to bounding box of projected position of first frame in sequence.

    void SetProjectiveScale(RealT imageScale, RealT mosaicScale)
      { 
	zhomog = imageScale, mosaicZHomog = mosaicScale;
	trackingHomogs.SetProjectiveScale(imageScale);
	Pmosaic = Projection2dC::I(zhomog, mosaicZHomog);
      }
    //: Set the scales (i.e. 3rd, Z component) for the projective coordinate systems
    // Should be set so that scale is commensurate with typical pixel coordinate values.  Can be set separately for video frame coordinates and mosaic coordinates . <br>
    // Default is 100, 1.

    void SetLensCorrection(RealT nK1, RealT nK2, RealT ncx_ratio, RealT ncy_ratio, RealT nfx, RealT nfy)
      { K1=nK1, K2=nK2, cx_ratio=ncx_ratio, cy_ratio=ncy_ratio, fx=nfx, fy=nfy; }
    //: Applies lens correction to frames of sequence
    //!param: K1 - cubic distortion coefficient; typical values for broadcast camera are 1..5 x 10^-7
    //!param: K2 - quintic distortion coefficient
    //!param: f1, f2 - focal lengths in pixels (default = 1)
    //!param: cx_ratio, cy_ratio - image centre coordinates as ratio of image dimensions (default: image centre)

    void SetSubsample(UIntT factor)
      { filterSubsample = factor; }
    //: Set temporal subsample factor for median filter
    // I.e. only accumulate every <code>factor</code>th frame in mosaic image.  Default value = 1.

    //:-
    //: Methods to grow the mosaic

    bool GrowMosaic(const StringC& ipFile, IntT startFrame, UIntT noOfFrames);
    //: Creates the mosaic from a sequence
    
    bool Apply(ImageC<ByteRGBValueC> &img);
    //: Add an image to the mosaic.
    // Provides all of the per-frame functionality required

    bool GenerateForeground(IntT startFrame, UIntT noOfFrames, DPOPortC<ImageC<ByteRGBValueC> >& outp, int fgThreshold);
    //: Generates foreground image sequence


    //:-
    //: Methods to get information from the mosaic builder
    // They can be called while the mosaic is being built to get partial information
    
    ImageRectangleC Frame() const
    { return mosaic.Frame(); }      
    //: Returns the frame of the current mosaic image

    ImageRectangleC Rectangle() const
    { return mosaicRect; }      
    //: Returns the rectangle of the mosaic
    // Slightly different from <code>Frame()</code> in that the rectangle may computed <i>before</i> the mosaic image is updated to fit it.

    Projection2dC GetMotion(IndexC frame) const
    //: Returns the 2D projective motion of the specified frame relative to the mosaic.
    {
      RavlAssertMsg(Parray.Size()>frame && frame>=0 && frame<=frameNo,
		    "in MosaicBuilderBodyC::GetMotion()");
      return Parray[frame];
    }
    
    DArray1dC<Projection2dC> GetMotion() const 
    //: Returns the 2D projective motions of all of the frames relative to the mosaic. 
    { return Parray; }

    const ImageC<ByteRGBValueC> GetMosaic() const;
    //: Returns the mosaic image

    const IndexRange2dC & GetCropRect() const
    //: Returns the crop rectangle
    { return cropRect; }

    //:-
    //: These methods perform smaller bits of the mosaicing process.
    // They are presented in the order in which they are used.

    bool OpenSequence(const StringC& ipFile, IntT startFrame);
    //: Open file for mosaicing and reset system

    bool PrepareFrame(ImageC<ByteRGBValueC>& img);
    //: Prepares frame for mosaicing
    // If required, removes lens distortion<br>
    // If first frame, computes border region.<br>
    // Removes unwanted border from image.

    bool FindProj(const ImageC<ByteRGBValueC> &img);
    //: Computes homography between current frame and mosaic

    bool InvolveFrame(const IndexRange2dC& rect, const Projection2dC& homog);
    //: Expand mosaic rectangle to include projected frame corners

    void ExpandMosaic();
    //: Expands mosaic to fit expanded mosaic rectangle

    void WarpFrame(const ImageC<ByteRGBValueC>& croppedImg);
    //: Warps current frame into mosaic coords and adds to mosaic
    // Returns true if expansion of mosaic was requested and needed

    Projection2dC Im2Mosaic(const ImageC<ByteRGBValueC> &img);
    //: Computes the homography between the first frame and the mosaic
    
    void SetHomogRefine(IntT searchSize)
      { updateSearchSize = searchSize; }
    //: Set search area for image matcher used to refine interframe homography
    // If searchSize = 0, this step is not used (default = 5).

    bool MatchToMosaic(const ImageC<ByteRGBValueC>& img, Projection2dC& proj);
    //: Improves inter-frame homography already calculated, by matching image to current mosaic
    // Returns false if match fails<br>

    bool GetImage(ImageC<ByteRGBValueC>& img)
      //: Returns next image from sequence
      {	
	if(!input.Get(img)) {
	  std::cerr<<"Could not read image at frame no. "<<frameNo<<endl;
	  img = ImageC<ByteRGBValueC>();
	  return false;
	}
	return true;
      }

    void Reset()
      { frameNo = 0; }
    //: Resets whole mosaicing process

  protected:
    // stored parameters
    IndexC borderC, borderR;
    autoResizeT resize;
    IntT cropT, cropB, cropL, cropR;
    Point2dC pointTL, pointTR, pointBL, pointBR;
    RealT zhomog, mosaicZHomog;
    RealT K1, K2, cx_ratio, cy_ratio, fx, fy;  // lens parameters
    UIntT filterSubsample; //median filter temporal subsample factor
    IntT updateSearchSize;  // matcher search area used for refining homography

    // stored data
    TrackingHomogC trackingHomogs; // engine that generates interframe homographies from tracked corners
    ImageC<bool> mask;  // identifies regions to be ignored for tracking
    DArray1dC<Projection2dC> Parray; // array of projections from mosaic coord frame into image coord frame (~ Psum * Pmosaic)
    IndexRange2dC cropRect;  // rectangle for cropped image
    IndexRange2dC mosaicRect;  // rectangle for mosaic
    Projection2dC Psum; // the accumulated motion as a product of the individual interframe motions
    Projection2dC Pmosaic;  // rescales the z component of a point
    ImageC<ByteRGBMedianC> mosaic;  // the median of all of the warped images (& hence contains the pixels from all of these images)
    DPISPortC<ImageC<ByteRGBValueC> > input; // i/p image stream
    IndexC frameNo;

    bool verbose;
    OStreamC log;
  };

  //! userlevel=Normal
  //! autoLink=on
  //: Mosaic building class

  // Since the mosaic generally grows as images are added to it, MosaicBuilderC has a choice of strategies to deal with this:<ol>
  // <li> The user decides in advance by how much to extend the image (using the SetBorderExpansion method).  Any growth of the mosaic beyond this will be clipped.</li>
  // <li> Each time the mosaic needs to expand, the mosaic image is copied to a new, larger image.  This is done in a single pass, so that it can handle real-time video sources, but is a bit slow.</li>
  // <li> Each time the mosaic needs to expand, the size required is noted.  In a second pass through the sequence, the mosaic is built.  This is faster than the one-pass method.</li>
  // </ol>
  
  class MosaicBuilderC
    : public RCHandleC<MosaicBuilderBodyC>
  {
  public:
    MosaicBuilderC()
      {}
    //: Default constructor.
    // Creates an invalid handle.

    MosaicBuilderC(autoResizeT resize, bool verbose=false, OStreamC log=cout)
     : RCHandleC<MosaicBuilderBodyC>(*new MosaicBuilderBodyC(resize, verbose, log))
      {}
    //: Constructor.
    //!param: resize - Determines how the mosaic expands; can take values:
    //!param:  "none" - mosaic does not expand beyond initial setting
    //!param:  "onepass" - mosaic size expands as necessary, as a single-pass operation
    //!param:  "twopass" - mosaic size expands as necessary, in two iterations
    
    MosaicBuilderC(
	IntT cthreshold, IntT cwidth, IntT mthreshold, IntT mwidth,
	IntT lifeTime, IntT searchSize, IntT newFreq,
	IntT borderC, IntT borderR, RealT zhomog,
	IntT cropT, IntT cropB, IntT cropL, IntT cropR,
	const Point2dC &npointTL, const Point2dC &npointTR,
	const Point2dC &npointBL, const Point2dC &npointBR,
	IntT maxFrames, const ImageC<bool>& nMask=ImageC<bool>(), bool verbose=false)
    : RCHandleC<MosaicBuilderBodyC>(*new MosaicBuilderBodyC(
	cthreshold, cwidth, mthreshold, mwidth, lifeTime, searchSize, newFreq,
	borderC, borderR, zhomog, cropT,  cropB,  cropL,  cropR,
	npointTL, npointTR, npointBL, npointBR, maxFrames, nMask, verbose))
    {}
    //: Deprecated constructor.
    //!deprecated: use previous constructor + <code>Set...()</code> methods
    
  protected:
    MosaicBuilderC(MosaicBuilderBodyC &bod)
      : RCHandleC<MosaicBuilderBodyC>(bod)
      {}
    //: body constructor.
  
    MosaicBuilderBodyC &Body()
      { return RCHandleC<MosaicBuilderBodyC>::Body(); }
    //: Access body.
    
    const MosaicBuilderBodyC &Body() const
      { return RCHandleC<MosaicBuilderBodyC>::Body(); }
    //: Access body.
    
  public:

    //:-
    //: These methods set various parameters for the class.  
    // They should be called before any images are processed.

    void SetTracker(IntT cthreshold,
		       IntT cwidth,
		       IntT mthreshold,
		       IntT mwidth,
		       IntT lifeTime,
		       IntT searchSize,
		       IntT newFreq)
      { Body().SetTracker(cthreshold, cwidth, mthreshold, mwidth, lifeTime, searchSize, newFreq); }
    //: Set <a href="RavlImageN.PointTrackerC.html">tracker parameters</a>
    // Defaults are respectively: 30, 7, 20, 17, 8, 25, 1

    void SetCropRegion(IntT cropT, IntT cropB, IntT cropL, IntT cropR)
      { Body().SetCropRegion(cropT, cropB, cropL, cropR); }
    //: Set border region that is to be ignored
    // This region is ignored for tracking and building mosaic

    void SetMask(const ImageC<bool>& mask)
      { Body().SetMask(mask); }
    //: Set binary mask to exclude graphics regions and suchlike from the process
    // Mask coordinate system should match that of image sequence.
    // Pixels set to false are excluded from tracker and foreground.
    // Unlike SetCropRegion, the mask regions are not excluded from the mosaic.

    bool SetMask(const StringC& fileName)
      { return Body().SetMask(fileName); }
    //: Set binary mask to exclude graphics regions and suchlike from the process
    // Mask coordinate system should match that of image sequence.
    // Pixels set to false are excluded from tracker and foreground.
    // Unlike SetCropRegion, the mask regions are not excluded from the mosaic.

    void SetCoordinates(const Point2dC &pointTL,
			const Point2dC &pointTR,
			const Point2dC &pointBL,
			const Point2dC &pointBR)
      { Body().SetCoordinates(pointTL, pointTR,	pointBL, pointBR); }
    //: Set the coordinate system of the mosaic
    // It does this by defining projected positions of corners of first frame in sequence.<br>
    // Note: the units of the arguments are in fractions of image height / width.<br>
    // Default: uses the coordinate frame of the first frame in sequence.

    void SetBorderExpansion(IndexC borderR, IndexC borderC)
      { Body().SetBorderExpansion(borderR, borderC); }
    //: Set amount by which mosaic border is expanded 
    // Default: border is set to bounding box of projected position of first frame in sequence.

    void SetProjectiveScale(RealT imageScale, RealT mosaicScale)
      { Body().SetProjectiveScale(imageScale, mosaicScale); }
    //: Set the scales (i.e. 3rd, Z component) for the projective coordinate systems
    // Should be set so that scale is commensurate with typical pixel coordinate values.  Can be set separately for video frame coordinates and mosaic coordinates . <br>
    // Default is 100, 1.

    void SetLensCorrection(RealT K1, RealT K2=0.0, RealT cx_ratio=0.5, RealT cy_ratio=0.5, RealT fx=1.0, RealT fy=1.0)
      { Body().SetLensCorrection(K1, K2, cx_ratio, cy_ratio, fx, fy); }
    //: Applies lens correction to frames of sequence
    //!param: K1 - cubic distortion coefficient; typical values for broadcast camera are 1..5 x 10^-7
    //!param: K2 - quintic distortion coefficient
    //!param: f1, f2 - focal lengths in pixels (default = 1)
    //!param: cx_ratio, cy_ratio - image centre coordinates as ratio of image dimensions (default: image centre)

    void SetSubsample(UIntT factor)
      { Body().SetSubsample(factor); }
    //: Set temporal subsample factor for median filter
    // I.e. only accumulate every <code>factor</code>th frame in mosaic image.  Default value = 1.

    void SetHomogRefine(IntT searchSize)
      { Body().SetHomogRefine(searchSize); }
    //: Set search area for image matcher used to refine interframe homography
    // If searchSize = 0, this refining step is not used (default: searchSize = 5).

    //:-
    //: Methods to grow the mosaic

    bool GrowMosaic(const StringC& ipFile, IntT startFrame, UIntT noOfFrames)
      { return Body().GrowMosaic(ipFile, startFrame, noOfFrames); }
    //: Creates the mosaic from a sequence
    
    bool Apply(ImageC<ByteRGBValueC> &img)
      { return Body().Apply(img); }
    //: Add an image to the mosaic.
    // Provides all of the per-frame functionality required
    
    bool GenerateForeground(IntT startFrame, UIntT noOfFrames, DPOPortC<ImageC<ByteRGBValueC> >& outp, int fgThreshold)
      {  return Body().GenerateForeground(startFrame, noOfFrames, outp, fgThreshold); }
    //: Generates foreground image sequence

    //:-
    //: Methods to get information from the mosaic builder
    // They can be called while the mosaic is being built to get partial information

    ImageRectangleC Frame() const
    { return Body().Frame(); }      
    //: Returns the frame of the current mosaic image

    ImageRectangleC Rectangle() const
    { return  Body().Rectangle(); }      
    //: Returns the rectangle of the mosaic
    // Slightly different from <code>Frame()</code> in that the rectangle may computed <i>before</i> the mosaic image is updated to fit it.

    Projection2dC GetMotion(IndexC frame) const
    { return Body().GetMotion(frame); }
    //: Returns the 2D projective motion of the specified frame relative to the mosaic.
    
    DArray1dC<Projection2dC> GetMotion() const 
    {return Body().GetMotion();}
    //: Returns the 2D projective motions of all of the frames relative to the mosaic. 

    const ImageC<ByteRGBValueC> GetMosaic() const
    { return Body().GetMosaic(); }
    //: Returns the mosaic image
    
    const IndexRange2dC & GetCropRect() const
    { return Body().GetCropRect(); }
    //: Returns the crop rectangle
    

    //:-
    //: These methods perform smaller bits of the mosaicing process.
    // They are presented in the order in which they are used.

    bool OpenSequence(const StringC& ipFile, IntT startFrame)
      { return Body().OpenSequence(ipFile, startFrame); }
    //: Open file for mosaicing and reset system

    bool PrepareFrame(ImageC<ByteRGBValueC>& img)
      {  return Body().PrepareFrame(img); }
    //: Prepares frame for mosaicing
    // If required, removes lens distortion<br>
    // If first frame, computes border region.<br>
    // Removes unwanted border from image.
    
    bool FindProj(const ImageC<ByteRGBValueC> &img)
      { return Body().FindProj(img); }
    //: Computes homography between current frame and mosaic

    Projection2dC Im2Mosaic(const ImageC<ByteRGBValueC> &img)
      { return Body().Im2Mosaic(img); }
    //: Computes the homography between the first frame and the mosaic
    
    bool MatchToMosaic(const ImageC<ByteRGBValueC>& img, Projection2dC& proj)
      { return Body().MatchToMosaic(img, proj); }
    //: Improves inter-frame homography already calculated, by matching image to current mosaic
    // Over an image sequence, the accumulated interframe homographies will gradually drift.  The following steps prevent this:<ul>
    // <li> The accumulated homography is used to warp the current image to the mosaic coordinates.  
    // <li> The warped image is  matched to the mosaic, which also generates the homography between the current image and the mosaic.
    // <li> The image-to-mosaic homography is used to calculate a new inter-frame homography to replace the original one.</ul>
    // Returns false if match fails<br>

    bool InvolveFrame(const IndexRange2dC& rect, const Projection2dC& homog)
      { return Body().InvolveFrame(rect, homog); }
    //: Expand mosaic rectangle to include projected frame corners
    // Returns true if expansion of mosaic was requested and needed

    void ExpandMosaic()
      { return Body().ExpandMosaic(); }
    //: Expands mosaic to fit expanded mosaic rectangle

    void WarpFrame(const ImageC<ByteRGBValueC>& croppedImg)
      { return Body().WarpFrame(croppedImg); }
    //: Warps current frame into mosaic coords and adds to mosaic

    void Reset()
      { return Body().Reset(); }
    //: Resets whole mosaicing process
  };
  
}


#endif
