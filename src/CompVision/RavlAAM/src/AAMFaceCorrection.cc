// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2006-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////////////////////////////
//! author =    "Jean-Yves Guillemaut"
//! lib=RavlAAM
//! date =      "01/03/2006"
//! rcsid="$Id: AAMFaceCorrection.cc,v 1.3 2005/07/25 16:31:07 ees1wc Exp $"
//! file="Ravl/CompVision/ActiveAppearanceModels/AAMFaceCorrection.cc"

#include "Ravl/OS/Filename.hh"
#include "Ravl/OS/Directory.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/Option.hh"
#include "Ravl/StringList.hh"
#include "Ravl/PatternRec/SampleIter.hh"
#include "Ravl/Image/Reflect.hh"
#include "Ravl/Image/HistogramEqualise.hh"
#include "Ravl/Array2dIter3.hh"
#include "Ravl/Array2d.hh"
#include "Ravl/PointSet2d.hh"
#include "Ravl/Index2d.hh"
#include "Ravl/Image/ImagePointFeatureSet.hh"
#include "Ravl/Image/AAMShapeModel.hh"
#include "Ravl/Image/AAMScaleRotationShapeModel.hh"
#include "Ravl/Image/AAMAppearanceUtil.hh"
#include "Ravl/Image/AAMAppearance.hh"
#include "Ravl/Image/AAMAppearanceModel.hh"
#include "Ravl/Image/AAMActiveAppearanceModel.hh"

#include "Ravl/Image/AAMPoseEstimate.hh"
#include "Ravl/Image/AAMFaceCorrection.hh"

#include "Ravl/Image/WarpThinPlateSpline.hh"
#include "Ravl/DelaunayTriangulation2d.hh"
#include "Ravl/Text/TextFile.hh"
#include "Ravl/Polygon2d.hh"
#include "Ravl/Image/WarpAffine.hh"
#include "Ravl/IO.hh"
#include "Ravl/Image/DrawLine.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {

  //: Constructor.
  AAMFaceCorrectionBodyC::AAMFaceCorrectionBodyC(const AAMPoseEstimateC &posestIn, const AAMAppearanceModelC &amIn)
    : posest(posestIn),
      am(amIn)
  {
  }

  //: Synthesize a frontal appearance of a person given an appearance in an arbitrary pose.
  //!param: appear - Input appearance.
  //!param: useAM - Use appearance model to synthesize grey-level values of corrected appearance? true = yes. Alternatively (if variable set to false), the grey-level values are obtained by warping of the input image. The latter setting is usually more accurate.
  //!param: frontalAppear - Returned frontal appearance.
  //!param: poseVals - Returned estimate of the pose (pan and tilt angles in radians) of the input appearance.
  //!param: useThinPlateSplines - Use thin-plate splines for warping? true = yes. The alternative method (if variable set to false) is a piece-wise affine warping. The piece-wise affine warping is faster.
  //!param: warpSigma - Stiffness (only useful in the case where the thin-plate spline warping is used).
  bool AAMFaceCorrectionBodyC::CorrectPose(const AAMAppearanceC &appear,bool useAM, AAMAppearanceC &frontalAppear, PairC<RealT> &poseVals, bool useThinPlateSplines, RealT warpSigma) const {

    AAMShapeModelC sm = am.ShapeModel();



    // Get parameters of original appearance.
    VectorC param;
    UIntT nb_fixed_params;
    if(useAM) {
      param = am.Parameters(appear);
      nb_fixed_params = am.NoFixedParameters();
    }
    else {
      param = sm.Parameters(appear);
      nb_fixed_params = sm.NoFixedParameters();
    }

    // Get pose.
    VectorC vec = param.From(nb_fixed_params);
    poseVals = posest.EstimatePose(vec);

    // Correct parameters affected by rotation.
    VectorC frontal_vec  = posest.RotateToFrontal(vec);

    //  Generate full parameter vectors by copying the fixed mean params...
    VectorC frontalParam = param.From(0,nb_fixed_params);
    if(sm.NoFixedParameters()==4) {
      frontalParam[3] = 0;  // remove roll in the case of AAMScaleRotationShapeModel
    }
    // ... and copying the rest of the parameters
    frontalParam.Append(frontal_vec);

#if 1
    // Make plausible if required.
    if(useAM) {
      am.MakePlausible(frontalParam);
    }
    else {
      sm.MakePlausible(frontalParam);
    }
#endif

    if(useAM) {
      frontalAppear = am.Synthesize(frontalParam);
    }
    else {
      SArray1dC<Point2dC> newpnts = sm.Synthesize(frontalParam);
      Polygon2dC bounds = ConvexHull(newpnts);
      ImageRectangleC rect1 = bounds.BoundingRectangle().IndexRange();
      // expand the image (useful in case of future warping)
      rect1 = rect1.Expand(Floor(0.15*Max(rect1.Rows(),rect1.Cols())));
      ImageC<ByteT> original= appear.Image();
      // make sure the dimension does not exceed the dimensions of the image
      rect1 = rect1.Intersection(original.Rectangle());

      ImageC<ByteT> out_frontal(rect1);

      if(useThinPlateSplines) {
        WarpThinPlateSplineC<ByteT> warp(warpSigma);
        warp.Apply(original,Array1dC<Point2dC>(appear.Points()),newpnts,out_frontal);
      }
      else {
        // add the four corners of the frame to the list of points
        // this is to force the filling of the background which do not contain feature points
        SArray1dC<Point2dC> corner_points(4);
        corner_points[0] = Point2dC(rect1.TopLeft());
        corner_points[1] = Point2dC(rect1.TopRight());
        corner_points[2] = Point2dC(rect1.BottomLeft());
        corner_points[3] = Point2dC(rect1.BottomRight());
        SArray1dC<Point2dC> newpnts_expanded = newpnts.Join(corner_points);
        SArray1dC<Point2dC> orgpnts_expanded = appear.Points().Join(corner_points);
        TriMesh2dC mesh = DelaunayTriangulation(newpnts_expanded).TriMesh();
        WarpMesh2dC<ByteT> warp;
        warp.CreateWarpMap(mesh,original.Frame());
        warp.Apply(original,orgpnts_expanded,out_frontal);

#if 0
        TriMesh2dC mesh2 = DelaunayTriangulation(newpnts).TriMesh();
        ImageC<ByteT> im2(out_frontal.Frame());
        for(Array2dIter2C<ByteT, ByteT> it2(out_frontal,im2); it2; it2++)
          it2.Data2() = it2.Data1();
        SArray1dC<Point2dC> vertices2 = mesh2.Vertices();
        SArray1dC<Index3dC> faces2 = mesh2.Faces();
        for(SArray1dIterC<Index3dC> faces2_it(faces2); faces2_it; faces2_it++) {
          Index2dC I(vertices2[faces2_it.Data().I()].X(), vertices2[faces2_it.Data().I()].Y());
          Index2dC J(vertices2[faces2_it.Data().J()].X(), vertices2[faces2_it.Data().J()].Y());
          Index2dC K(vertices2[faces2_it.Data().K()].X(), vertices2[faces2_it.Data().K()].Y());
          DrawLine(im2, (ByteT) 255, I, J);
          DrawLine(im2, (ByteT) 255, J, K);
          DrawLine(im2, (ByteT) 255, K, I);
        }
        Save("@X: final mesh", im2);

        ImageC<ByteT> im1(original.Frame());
        for(Array2dIter2C<ByteT, ByteT> it1(im1,original); it1; it1++)
          it1.Data1() = it1.Data2();
        SArray1dC<Point2dC> vertices1 = appear.Points();
        SArray1dC<Index3dC> faces1 = mesh2.Faces();
        for(SArray1dIterC<Index3dC> faces1_it(faces1); faces1_it; faces1_it++) {
          Index2dC I(vertices1[faces1_it.Data().I()].X(), vertices1[faces1_it.Data().I()].Y());
          Index2dC J(vertices1[faces1_it.Data().J()].X(), vertices1[faces1_it.Data().J()].Y());
          Index2dC K(vertices1[faces1_it.Data().K()].X(), vertices1[faces1_it.Data().K()].Y());
          DrawLine(im1, (ByteT) 255, I, J);
          DrawLine(im1, (ByteT) 255, J, K);
          DrawLine(im1, (ByteT) 255, K, I);
        }
        Save("@X: original mesh", im1);
#endif

     }

      out_frontal = ImageC<ByteT>(out_frontal,rect1);
      frontalAppear = AAMAppearanceC(newpnts,out_frontal);
    }

    return true;
  }

  //: Synthesize a frontal appearance of a person given an appearance in an arbitrary pose for which a pose estimate is provided.
  //!param: appear - Input appearance.
  //!param: useAM - Use appearance model to synthesize grey-level values of corrected appearance? true = yes. Alternatively (if variable set to false), the grey-level values are obtained by warping of the input image. The latter setting is usually more accurate.
  //!param: frontalAppear - Returned frontal appearance.
  //!param: poseVals - Returned estimate of the pose (pan and tilt angles in radians) of the input appearance.
  //!param: poseVali - Initial estimate of the pose (pan and tilt angles in radians) of the input appearance.
  //!param: useThinPlateSplines - Use thin-plate splines for warping? true = yes. The alternative method (if variable set to false) is a piece-wise affine warping. The piece-wise affine warping is faster.
  //!param: warpSigma - Stiffness (only useful in the case where the thin-plate spline warping is used).
  bool AAMFaceCorrectionBodyC::CorrectPose(const AAMAppearanceC &appear,bool useAM, AAMAppearanceC &frontalAppear, PairC<RealT> &poseVals, const PairC<RealT> &poseVali, bool useThinPlateSplines, RealT warpSigma) const {

    AAMShapeModelC sm = am.ShapeModel();

    // Get parameters of original appearance.
    VectorC param;
    UIntT nb_fixed_params;
    if(useAM) {
      param = am.Parameters(appear);
      nb_fixed_params = am.NoFixedParameters();
    }
    else {
      param = sm.Parameters(appear);
      nb_fixed_params = sm.NoFixedParameters();
    }

    // Get pose.
    VectorC vec = param.From(nb_fixed_params);
    poseVals = posest.EstimatePose(vec);

    // Correct parameters affected by rotation.
    VectorC frontal_vec  = posest.RotateToFrontal(vec, poseVali.Data1(), poseVali.Data2());

    //  Generate full parameter vectors by copying the fixed mean params...
    VectorC frontalParam = param.From(0,nb_fixed_params);
    if(sm.NoFixedParameters()==4) {
      frontalParam[3] = 0;  // remove roll in the case of AAMScaleRotationShapeModel
    }
    // ... and copying the rest of the parameters
    frontalParam.Append(frontal_vec);

#if 1
    // Make plausible if required.
    if(useAM) {
      am.MakePlausible(frontalParam);
    }
    else {
      sm.MakePlausible(frontalParam);
    }
#endif

    if(useAM) {
      frontalAppear = am.Synthesize(frontalParam);
    }
    else {
      SArray1dC<Point2dC> newpnts = sm.Synthesize(frontalParam);
      Polygon2dC bounds = ConvexHull(newpnts);
      ImageRectangleC rect1 = bounds.BoundingRectangle().IndexRange();
      // expand the image (useful in case of future warping)
      rect1 = rect1.Expand(Floor(0.15*Max(rect1.Rows(),rect1.Cols())));
      ImageC<ByteT> original= appear.Image();
      // make sure the dimension does not exceed the dimensions of the image
      rect1 = rect1.Intersection(original.Rectangle());

      ImageC<ByteT> out_frontal(rect1);

      if(useThinPlateSplines) {
        WarpThinPlateSplineC<ByteT> warp(warpSigma);
        warp.Apply(original,Array1dC<Point2dC>(appear.Points()),newpnts,out_frontal);
      }
      else {
        // add the four corners of the frame to the list of points
        // this is to force the filling of the background which do not contain feature points
        SArray1dC<Point2dC> corner_points(4);
        corner_points[0] = Point2dC(rect1.TopLeft());
        corner_points[1] = Point2dC(rect1.TopRight());
        corner_points[2] = Point2dC(rect1.BottomLeft());
        corner_points[3] = Point2dC(rect1.BottomRight());
        SArray1dC<Point2dC> newpnts_expanded = newpnts.Join(corner_points);
        SArray1dC<Point2dC> orgpnts_expanded = appear.Points().Join(corner_points);
        TriMesh2dC mesh = DelaunayTriangulation(newpnts_expanded).TriMesh();
        WarpMesh2dC<ByteT> warp;
        warp.CreateWarpMap(mesh,original.Frame());
        warp.Apply(original,orgpnts_expanded,out_frontal);
      }

      out_frontal = ImageC<ByteT>(out_frontal,rect1);
      frontalAppear = AAMAppearanceC(newpnts,out_frontal);
    }

    return true;
  }

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
  bool AAMFaceCorrectionBodyC::GeometricNormalisation(const AAMAppearanceC &inAppear,
                                                             const Point2dC &leftEye, const Point2dC &rightEye,
                                                             const Point2dC &nLeftEye, const Point2dC &nRightEye,
                                                             ImageRectangleC &outRect,
                                                             AAMAppearanceC &outAppear,
                                                             UIntT smoothingSize
                                                             ) const
  {
    Affine2dC Aff;
    Vector2dC d1 = nRightEye - nLeftEye;
    Vector2dC d2 = rightEye - leftEye;
    RealT rot = d2.Angle() - d1.Angle();      // Sort out rotation.
    RealT scale = d2.Modulus() / d1.Modulus();// Sort out scaling.
    // Compose rotation/scale matrix.
    Matrix2dC rotm = Matrix2dC(Cos(rot) * scale,-Sin(rot) * scale, Sin(rot) * scale,Cos(rot) * scale);
    // Calculate translation.
    Point2dC cent= ((leftEye + rightEye)/2);
    Point2dC dcent = rotm * (nLeftEye + nRightEye)/2;
    Vector2dC off = (cent - dcent);
    Aff = Affine2dC(rotm,off);

    WarpAffineC<ByteT> affWarp(outRect, Aff);
    ImageC<ByteT> out_frontal_corrected;

    // first smooth image
    GaussConvolve2dC<ByteT> smooth(smoothingSize);
    ImageC<ByteT> im = smooth.Apply(inAppear.Image());

    // then warp it
    affWarp.Apply(im,out_frontal_corrected);

    DListC<Point2dC> srcPoints;
    for(SArray1dIterC<Point2dC> it(inAppear.Points());it;it++) {
      srcPoints.InsLast(*it);
    }
    PointSet2dC srcSet(srcPoints);
    PointSet2dC destSet=Aff.Inverse()*srcSet;
    SArray1dC<Point2dC> destPoints(destSet.Size());
    destSet.First();
    for(SArray1dIterC<Point2dC> it(destPoints);it;it++) {
      *it = destSet.PopFirst();
    }

    outAppear = AAMAppearanceC(destPoints,out_frontal_corrected);
    outAppear.Image().ShiftArray(-outAppear.Image().Frame().TopLeft());

    return true;
  }

  //: Apply geometric normalisation to an image with known eye centres.
  //!param: inImage - Input image (non normalised).
  //!param: leftEye - Coordinates of the left eye in input image.
  //!param: rightEye - Coordinates of the right eye in input image.
  //!param: nLeftEye - Coordinates of the left eye after normalisation.
  //!param: nRightEye - Coordinates of the right eye after normalisation.
  //!param: outImage - Output image (normalised).
  //!param: smoothingSize - Size of the Gaussian convolution kernel used for smoothing the input image before warping.
  // The normalisation consists in applying a transformation composed of a rotation, translation and scaling in order to map the eye centres to the required positions.
  bool AAMFaceCorrectionBodyC::GeometricNormalisation(const ImageC<ByteT> &inImage,
                                                            const Point2dC &leftEye, const Point2dC &rightEye,
                                                            const Point2dC &nLeftEye, const Point2dC &nRightEye,
                                                            ImageC<ByteT> &outImage, UIntT smoothingSize
                                                            ) const
  {
    // smooth image
    GaussConvolve2dC<ByteT> smooth(smoothingSize);
    ImageC<ByteT> inImage2 = smooth.Apply(inImage);

    Vector2dC d1 = nRightEye - nLeftEye;
    Vector2dC d2 = rightEye - leftEye;
    RealT rot = d2.Angle() - d1.Angle();      // Sort out rotation.
    RealT scale = d2.Modulus() / d1.Modulus();// Sort out scaling.
    // Compose rotation/scale matrix.
    Matrix2dC rotm = Matrix2dC(Cos(rot) * scale,-Sin(rot) * scale, Sin(rot) * scale,Cos(rot) * scale);
    // Calculate translation.
    Point2dC cent= ((leftEye + rightEye)/2);
    Point2dC dcent = rotm * (nLeftEye + nRightEye)/2;
    Vector2dC off = (cent - dcent);
    Affine2dC  Aff = Affine2dC(rotm,off);

    WarpAffineC<ByteT> affWarp(outImage.Rectangle(), Aff);
    affWarp.Apply(inImage2,outImage);

    return true;
  }

}
