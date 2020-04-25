// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2006, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_FACECORRECTION_HEADER
#define RAVLIMAGE_FACECORRECTION_HEADER 1
//! rcsid="$Id: AAMFaceCorrection.hh,v 1.3 2005/07/25 16:31:07 ees1wc Exp $"
//! lib=RavlAAM
//! docentry="Ravl.API.Images.AAM"
//! userlevel="Develop"
//! file="Ravl/CompVision/ActiveAppearanceModels/AAMFaceCorrection.hh"
//! author="Jean-Yves Guillemaut"
//! example = aamSynthesizeFrontal.cc

#include "Ravl/RCBodyV.hh"
#include "Ravl/Image/AAMPoseEstimate.hh"
#include "Ravl/Image/AAMAppearanceModel.hh"

namespace RavlImageN {

  //! userlevel=Develop
  //: AAM-based face pose correction.

  class AAMFaceCorrectionBodyC
    :public RCBodyVC
  {
  public:

    AAMFaceCorrectionBodyC(const AAMPoseEstimateC &posestIn, const AAMAppearanceModelC &amIn);
    //: Constructor.

    bool CorrectPose(const AAMAppearanceC &appear,bool useAM, AAMAppearanceC &frontalAppear, PairC<RealT> &poseVals, bool useThinPlateSplines = true, RealT warpSigma=2) const;
    //: Synthesize a frontal appearance of a person given an appearance in an arbitrary pose.
    //!param: appear - Input appearance.
    //!param: useAM - Use appearance model to synthesize grey-level values of corrected appearance? true = yes. Alternatively (if variable set to false), the grey-level values are obtained by warping of the input image. The latter setting is usually more accurate.
    //!param: frontalAppear - Returned frontal appearance.
    //!param: poseVals - Returned estimate of the pose (pan and tilt angles in radians) of the input appearance.
    //!param: useThinPlateSplines - Use thin-plate splines for warping? true = yes. The alternative method (if variable set to false) is a piece-wise affine warping. The piece-wise affine warping is faster.
    //!param: warpSigma - Stiffness (only useful in the case where the thin-plate spline warping is used).

    bool CorrectPose(const AAMAppearanceC &appear,bool useAM, AAMAppearanceC &frontalAppear, PairC<RealT> &poseVals, const PairC<RealT> &poseVali, bool useThinPlateSplines = true, RealT warpSigma=2) const;
    //: Synthesize a frontal appearance of a person given an appearance in an arbitrary pose for which a pose estimate is provided.
    //!param: appear - Input appearance.
    //!param: useAM - Use appearance model to synthesize grey-level values of corrected appearance? true = yes. Alternatively (if variable set to false), the grey-level values are obtained by warping of the input image. The latter setting is usually more accurate.
    //!param: frontalAppear - Returned frontal appearance.
    //!param: poseVals - Returned estimate of the pose (pan and tilt angles in radians) of the input appearance.
    //!param: poseVali - Initial estimate of the pose (pan and tilt angles in radians) of the input appearance.
    //!param: useThinPlateSplines - Use thin-plate splines for warping? true = yes. The alternative method (if variable set to false) is a piece-wise affine warping. The piece-wise affine warping is faster.
    //!param: warpSigma - Stiffness (only useful in the case where the thin-plate spline warping is used).

    bool GeometricNormalisation(const AAMAppearanceC &inAppear, const Point2dC &leftEye, const Point2dC &rightEye, const Point2dC &nLeftEye, const Point2dC &nRightEye, ImageRectangleC &outRect, AAMAppearanceC &outAppear, UIntT smoothingSize = 5) const;
    //: Apply geometric normalisation to an appearance.
    //!param: inAppear - Input appearance (non-normalised).
    //!param: leftEyeIDs - IDs of the left inner eye feature points used for locating left eye centre.
    //!param: rightEyeIDs - IDs of the right inner eye feature points used for locating right eye centre.
    //!param: nLeftEye - Coordinates of the left eye after normalisation.
    //!param: nRightEye - Coordinates of the right eye after normalisation.
    //!param: outRect - Frame dimensions for the normalised appearance.
    //!param: outAppear - Output appearance (normalised).
    //!param: smoothingSize - Size of the Gaussian convolution kernel used for smoothing the input image before warping.
    // The normalisation consists in applying a transformation composed of a rotation, translation and scaling in order to map the eye centres to the required positions. The eye centres in the input appeconst arance are located at the centroid of the set of feature points whose identities are provided in 'leftEyeIDs' and 'rightEyeIDs'.

    bool GeometricNormalisation(const ImageC<ByteT> &inImage, const Point2dC &leftEye, const Point2dC &rightEye, const Point2dC &nLeftEye, const Point2dC &nRightEye, ImageC<ByteT> &outImage, UIntT smoothingSize = 5) const;
    //: Apply geometric normalisation to an image with known eye centres.
    //!param: inImage - Input image (non normalised).
    //!param: leftEye - Coordinates of the left eye in input image.
    //!param: rightEye - Coordinates of the right eye in input image.
    //!param: nLeftEye - Coordinates of the left eye after normalisation.
    //!param: nRightEye - Coordinates of the right eye after normalisation.
    //!param: outImage - Output image (normalised).
    //!param: smoothingSize - Size of the Gaussian convolution kernel used for smoothing the input image before warping.
    // The normalisation consists in applying a transformation composed of a rotation, translation and scaling in order to map the eye centres to the required positions.

  protected:
    AAMPoseEstimateC posest;
    AAMAppearanceModelC am;
  };


  //! userlevel=Normal
  //: AAM-based face pose correction.

  class AAMFaceCorrectionC
    : public RCHandleVC<AAMFaceCorrectionBodyC>
  {
  public:

    AAMFaceCorrectionC()
    {}
    //: Default constructor 
    // Creates an invalid handle.

    AAMFaceCorrectionC(const AAMPoseEstimateC &posestIn, const AAMAppearanceModelC &amIn)
      : RCHandleVC<AAMFaceCorrectionBodyC>(*new AAMFaceCorrectionBodyC(posestIn, amIn))
    {}
    //: Constructor.


  protected:
    AAMFaceCorrectionC(AAMFaceCorrectionBodyC &bod)
      : RCHandleVC<AAMFaceCorrectionBodyC>(bod)
    {}
    //: Body constructor.

    AAMFaceCorrectionC(AAMFaceCorrectionBodyC *bod)
      : RCHandleVC<AAMFaceCorrectionBodyC>(*bod)
    {}
    //: Body ptr constructor.

    AAMFaceCorrectionBodyC &Body()
    { return RCHandleVC<AAMFaceCorrectionBodyC>::Body(); }
    //: Access body.

    const AAMFaceCorrectionBodyC &Body() const
    { return RCHandleVC<AAMFaceCorrectionBodyC>::Body(); }
    //: Access body.

  public:

    bool CorrectPose(const AAMAppearanceC &appear,bool useAM, AAMAppearanceC &frontalAppear, PairC<RealT> &poseVals, bool useThinPlateSplines = true, RealT warpSigma=2) const
    {  return Body().CorrectPose(appear, useAM, frontalAppear, poseVals, useThinPlateSplines, warpSigma);}
    //: Synthesize a frontal appearance of a person given an appearance in an arbitrary pose.
    //!param: appear - Input appearance.
    //!param: useAM - Use appearance model to synthesize grey-level values of corrected appearance? true = yes. Alternatively (if variable set to false), the grey-level values are obtained by warping of the input image. The latter setting is usually more accurate.
    //!param: frontalAppear - Returned frontal appearance.
    //!param: poseVals - Returned estimate of the pose (pan and tilt angles in radians) of the input appearance.
    //!param: useThinPlateSplines - Use thin-plate splines for warping? true = yes. The alternative method (if variable set to false) is a piece-wise affine warping. The piece-wise affine warping is faster.
    //!param: warpSigma - Stiffness (only useful in the case where the thin-plate spline warping is used).

    bool CorrectPose(const AAMAppearanceC &appear,bool useAM, AAMAppearanceC &frontalAppear, PairC<RealT> &poseVals, const PairC<RealT> &poseVali, bool useThinPlateSplines = true, RealT warpSigma=2) const
    {  return Body().CorrectPose(appear, useAM, frontalAppear, poseVals, poseVali, useThinPlateSplines, warpSigma);}
    //: Synthesize a frontal appearance of a person given an appearance in an arbitrary pose for which a pose estimate is provided.
    //!param: appear - Input appearance.
    //!param: useAM - Use appearance model to synthesize grey-level values of corrected appearance? true = yes. Alternatively (if variable set to false), the grey-level values are obtained by warping of the input image. The latter setting is usually more accurate.
    //!param: frontalAppear - Returned frontal appearance.
    //!param: poseVals - Returned estimate of the pose (pan and tilt angles in radians) of the input appearance.
    //!param: poseVali - Initial estimate of the pose (pan and tilt angles in radians) of the input appearance.
    //!param: useThinPlateSplines - Use thin-plate splines for warping? true = yes. The alternative method (if variable set to false) is a piece-wise affine warping. The piece-wise affine warping is faster.
    //!param: warpSigma - Stiffness (only useful in the case where the thin-plate spline warping is used).

    bool GeometricNormalisation(const AAMAppearanceC &inAppear, const Point2dC &leftEye, const Point2dC &rightEye, const Point2dC &nLeftEye, const Point2dC &nRightEye, ImageRectangleC &outRect, AAMAppearanceC &outAppear, UIntT smoothingSize = 5) const
    {  return Body().GeometricNormalisation(inAppear, leftEye, rightEye, nLeftEye, nRightEye, outRect, outAppear, smoothingSize); }
    //: Apply geometric normalisation to an appearance.
    //!param: inAppear - Input appearance (non-normalised).
    //!param: leftEyeIDs - IDs of the left inner eye feature points used for locating left eye centre.
    //!param: rightEyeIDs - IDs of the right inner eye feature points used for locating right eye centre.
    //!param: nLeftEye - Coordinates of the left eye after normalisation.
    //!param: nRightEye - Coordinates of the right eye after normalisation.
    //!param: outRect - Frame dimensions for the normalised appearance.
    //!param: outAppear - Output appearance (normalised).
    //!param: smoothingSize - Size of the Gaussian convolution kernel used for smoothing the input image before warping.
    // The normalisation consists in applying a transformation composed of a rotation, translation and scaling in order to map the eye centres to the required positions. The eye centres in the input appearance are located at the centroid of the set of feature points whose identities are provided in 'leftEyeIDs' and 'rightEyeIDs'.

    bool GeometricNormalisation(const ImageC<ByteT> &inImage, const Point2dC &leftEye, const Point2dC &rightEye, const Point2dC &nLeftEye, const Point2dC &nRightEye, ImageC<ByteT> &outImage, UIntT smoothingSize = 5) const
    {  return Body().GeometricNormalisation(inImage, leftEye, rightEye, nLeftEye, nRightEye, outImage, smoothingSize); }
    //: Apply geometric normalisation to an image with known eye centres.
    //!param: inImage - Input image (non normalised).
    //!param: leftEye - Coordinates of the left eye in input image.
    //!param: rightEye - Coordinates of the right eye in input image.
    //!param: nLeftEye - Coordinates of the left eye after normalisation.
    //!param: nRightEye - Coordinates of the right eye after normalisation.
    //!param: outImage - Output image (normalised).
    //!param: smoothingSize - Size of the Gaussian convolution kernel used for smoothing the input image before warping.
    // The normalisation consists in applying a transformation composed of a rotation, translation and scaling in order to map the eye centres to the required positions.

    friend class AAMFaceCorrectionBodyC;
  };

}


#endif
