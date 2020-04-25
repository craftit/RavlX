// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2006, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_FACELOCALISATION_HEADER
#define RAVLIMAGE_FACELOCALISATION_HEADER 1
//! rcsid="$Id$"
//! lib=RavlAAM
//! file="Ravl/CompVision/ActiveAppearanceModels/AAMFaceLocalisation.hh"
//! docentry="Ravl.API.Images.AAM"
//! userlevel="Develop"
//! author="Jean-Yves Guillemaut"
//! example = aamLocaliseFace.cc

#include "Ravl/Image/AAMMultiResActiveAppearanceModel.hh"
#include "Ravl/OS/Filename.hh"

namespace RavlImageN {

  //! userlevel=Develop
  //: AAM-based face localisation.
  //  Use AAM to localise a face in an image. The method requires initialisation with an estimate of the eyes position.

  class AAMFaceLocalisationBodyC
    :public RCBodyVC
  {
  public:

    AAMFaceLocalisationBodyC()
    {}
    //: Default constructor.

    AAMFaceLocalisationBodyC(const AAMMultiResActiveAppearanceModelC &saam, FilenameC exampleFile);
    //: Constructor.
    //!param: saam - Multi-resolution AAM.
    //!param: exampleFile - Example of markup file representing an appearance.
    //  The example is used to learn the indices of the control points representing the eyes from the labels in the XML file. This is necessary to be able to compute the location of the centre of the eyes from an appearance or to compute the position of the appearance in the image from the position of its eye centres.

    AAMFaceLocalisationBodyC(const AAMMultiResActiveAppearanceModelC &saam, FilenameC exampleFile, FilenameC tpDir);
    //: Constructor.
    //!param: saam - Multi-resolution AAM.
    //!param: exampleFile - Example of markup file representing an appearance.
    //!param: tpDir - Name of directory containing list of shape initialisations.
    //  The example is used to learn the indices of the control points representing the eyes from the labels in the XML file. This is necessary to be able to compute the location of the centre of the eyes from an appearance or to compute the position of the appearance in the image from the position of its eye centres. A shape initialisation consists of saved mean points representing an instance of a shape. These are used to provide different initialisations for the AAM search algorithm, which increases the range of convergence of the algorithm. Typically the possible initialisation consists of mean shape corresponding to different poses such as left, right, up, down and frontal, or more if necessary.

    bool FitModel(const ImageC<ByteT> &inImage, const PairC<Point2dC> &eyeCentres, AAMAppearanceC &resAppear, bool useAM = false) const;
    //: Multi-resolution AAM search algorithm.
    //!param: inImage - Input image.
    //!param: eyeCentres - Estimate of the position of the centre of the eyes.
    //!param: resAppear - Output appearance fitted to the image.
    //!param: useAM - Synthesize texture using appearance model? Yes = true. If set to false the texture of the appearance image is obtained from the input image, i.e. the search algorithm is used only to find the location of the feature points defining the shape.
    
    bool FitModel(const ImageC<RealT> &inImage, const PairC<Point2dC> &eyeCentres, AAMAppearanceC &resAppear, bool useAM = false) const;
    //: Multi-resolution AAM search algorithm.
    //!param: inImage - Input image.
    //!param: eyeCentres - Estimate of the position of the centre of the eyes.
    //!param: resAppear - Output appearance fitted to the image.
    //!param: useAM - Synthesize texture using appearance model? Yes = true. If set to false the texture of the appearance image is obtained from the input image, i.e. the search algorithm is used only to find the location of the feature points defining the shape.
    
    bool FitModel(const ImageC<RealT> &inImage, const PairC<Point2dC> &eyeCentres, VectorC &aamParameters) const;
    //: Multi-resolution AAM search algorithm.
    //!param: inImage - Input image.
    //!param: eyeCentres - Estimate of the position of the centre of the eyes.
    //!param: aamParameters - Recovered output parameters fitting the model to the image.
    
    bool FitModel(const ImageC<ByteT> &inImage, const PairC<Point2dC> &eyeCentres, VectorC &aamParameters) const;
    //: Multi-resolution AAM search algorithm.
    //!param: inImage - Input image.
    //!param: eyeCentres - Estimate of the position of the centre of the eyes.
    //!param: aamParameters - Recovered output parameters fitting the model to the image.
    
    const AAMMultiResActiveAppearanceModelC &MultiResActiveAppearanceModel() const
    { return maam; }
    //: Get multi-resolution active appearance model.

    AAMMultiResActiveAppearanceModelC &MultiResActiveAppearanceModel()
    { return maam; }
    //: Get multi-resolution active appearance model.

    const DListC<Tuple2C<SArray1dC<Point2dC>, PairC<Point2dC> > > &InitialisationList() const
    { return initialisationList; }
    //: List of initialisation for the search.
    //  Each initialisation instance consists of an array of coordinates of the feature points defining the shape and the coordinates of the eye centres for this initialisation.

    bool GetEyeCentres(const AAMAppearanceC &app, PairC<Point2dC> &eyeCentres) const;
    //: Return coordinates of the eye centres 'eyeCentres' for the appearance defined by 'app'.
    //  This function computes the coordinates of the eye centres from the position of the other feature points.
    //  The eye centre is defined as the midpoint of the segment defined by the left and right corners of the outer contour of the eye.
    //  The eye centre IS NOT gaze-dependent.

    bool GetEyeCentres(const SArray1dC<Point2dC> &points, PairC<Point2dC> &eyeCentres) const;
    //: Return coordinates of the eye centres 'eyeCentres' for the appearance with feature points 'points'.
    //  This function computes the coordinates of the eye centres from the position of the other feature points.
    //  The eye centre is defined as the midpoint of the segment defined by the left and right corners of the outer contour of the eye.
    //  The eye centre IS NOT gaze-dependent.

    bool GetIrisCentres(const AAMAppearanceC &app, PairC<Point2dC> &irisCentres) const;
    //: Return coordinates of the iris centres 'irisCentres' for the appearance defined by 'app'.
    //  This function computes the coordinates of the iris centres from the position of the other feature points.
    //  The iris centre is defined as the centroid of the corners defining the inner contour of the eye.
    //  The iris centre IS gaze-dependent.

    bool GetIrisCentres(const SArray1dC<Point2dC> &points, PairC<Point2dC> &irisCentres) const;
    //: Return coordinates of the iris centres 'irisCentres' for the appearance with feature points 'points'.
    //  This function computes the coordinates of the iris centres from the position of the other feature points.
    //  The iris centre is defined as the centroid of the corners defining the inner contour of the eye.
    //  The iris centre is gaze-dependent.

  protected:

    bool EyeCentresInit(FilenameC exampleFile);
    //: Sets parameters 'leftEyeIDs' and 'rightEyeIDs' for locating centres of the eyes.

    bool IrisCentresInit(FilenameC exampleFile);
    //: Sets parameters 'leftIrisIDs' and 'rightIrisIDs' for locating centres of the eyes.

    bool TemplatesInit(FilenameC tpDir = "");
    //: Sets list of initiliasation 'initialisationList' for AAM search.
    // If no value is provided, the search will be initialised with the mean appearance.

    Affine2dC ComputeSimilarityTransf(const PairC<Point2dC> &src, const PairC<Point2dC> &dest) const;
    //: Compute transformation composed of a rotation, translation, and scaling which maps the two eyes from position 'src' to 'dest'.

  protected:
    AAMMultiResActiveAppearanceModelC maam;  // Multi-resolution active appearance model.
    DListC<Tuple2C<SArray1dC<Point2dC>, PairC<Point2dC> > > initialisationList;  // List of initialisations for AAM search.
    DListC<IntT> leftEyeIDs;  // Indices of the feature points used to locate the centre of the left eye.
    DListC<IntT> rightEyeIDs;  // Indices of the feature points used to locate the centre of the right eye.
    DListC<IntT> leftIrisIDs;  // Indices of the feature points used to locate the centre of the left iris.
    DListC<IntT> rightIrisIDs;  // Indices of the feature points used to locate the centre of the right iris.
  };


  //! userlevel=Normal
  //: AAM-based face localisation.
  //  Use AAM to localise a face in an image. The method requires initialisation with an estimate of the eyes position.

  class AAMFaceLocalisationC
    : public RCHandleVC<AAMFaceLocalisationBodyC>
  {
  public:

    AAMFaceLocalisationC()
    {}
    //: Default constructor 
    // Creates an invalid handle.

    AAMFaceLocalisationC(const AAMMultiResActiveAppearanceModelC &saam, FilenameC exampleFile)
      : RCHandleVC<AAMFaceLocalisationBodyC>(*new AAMFaceLocalisationBodyC(saam, exampleFile))
    {}
    //: Constructor.
    //!param: saam - Multi-resolution AAM.
    //!param: exampleFile - Example of markup file representing an appearance.
    //  The example is used to learn the indices of the control points representing the eyes from the labels in the XML file. This is necessary to be able to compute the location of the centre of the eyes from an appearance or to compute the position of the appearance in the image from the position of its eye centres.

    AAMFaceLocalisationC(const AAMMultiResActiveAppearanceModelC &saam, FilenameC exampleFile, FilenameC tpDir)
      : RCHandleVC<AAMFaceLocalisationBodyC>(*new AAMFaceLocalisationBodyC(saam, exampleFile, tpDir))
    {}
    //: Constructor.
    //!param: saam - Multi-resolution AAM.
    //!param: exampleFile - Example of markup file representing an appearance.
    //!param: tpDir - Name of directory containing list of shape initialisations.
    //  The example is used to learn the indices of the control points representing the eyes from the labels in the XML file. This is necessary to be able to compute the location of the centre of the eyes from an appearance or to compute the position of the appearance in the image from the position of its eye centres. A shape initialisation consists of saved mean points representing an instance of a shape. These are used to provide different initialisations for the AAM search algorithm, which increases the range of convergence of the algorithm. Typically the possible initialisation consists of mean shape corresponding to different poses such as left, right, up, down and frontal, or more if necessary.

  protected:
    AAMFaceLocalisationC(AAMFaceLocalisationBodyC &bod)
      : RCHandleVC<AAMFaceLocalisationBodyC>(bod)
    {}
    //: Body constructor.

    AAMFaceLocalisationC(AAMFaceLocalisationBodyC *bod)
      : RCHandleVC<AAMFaceLocalisationBodyC>(*bod)
    {}
    //: Body ptr constructor.

    AAMFaceLocalisationBodyC &Body()
    { return RCHandleVC<AAMFaceLocalisationBodyC>::Body(); }
    //: Access body.

    const AAMFaceLocalisationBodyC &Body() const
    { return RCHandleVC<AAMFaceLocalisationBodyC>::Body(); }
    //: Access body.

  public:

    bool FitModel(const ImageC<ByteT> &inImage, const PairC<Point2dC> &eyeCentres, AAMAppearanceC &resAppear, bool useAM = false) const
    {  return Body().FitModel(inImage, eyeCentres, resAppear, useAM); }
    //: Multi-resolution AAM search algorithm.
    //!param: inImage - Input image.
    //!param: eyeCentres - Estimate of the position of the centre of the eyes.
    //!param: resAppear - Output appearance fitted to the image.
    //!param: useAM - Synthesize texture using appearance model? Yes = true. If set to false the texture of the appearance image is obtained from the input image, i.e. the search algorithm is used only to find the location of the feature points defining the shape.

    bool FitModel(const ImageC<RealT> &inImage, const PairC<Point2dC> &eyeCentres, AAMAppearanceC &resAppear, bool useAM = false) const
    {  return Body().FitModel(inImage, eyeCentres, resAppear, useAM); }
    //: Multi-resolution AAM search algorithm.
    //!param: inImage - Input image.
    //!param: eyeCentres - Estimate of the position of the centre of the eyes.
    //!param: resAppear - Output appearance fitted to the image.
    //!param: useAM - Synthesize texture using appearance model? Yes = true. If set to false the texture of the appearance image is obtained from the input image, i.e. the search algorithm is used only to find the location of the feature points defining the shape.

    bool GetEyeCentres(const AAMAppearanceC &app, PairC<Point2dC> &eyeCentres) const
    {  return Body().GetEyeCentres(app, eyeCentres); }
    //: Return coordinates of the eye centres 'eyeCentres' for the appearance defined by 'app'.
    //  This function computes the coordinates of the eye centres from the position of the other feature points.
    //  The eye centre is defined as the midpoint of the segment defined by the left and right corners of the outer contour of the eye.
    //  The eye centre IS NOT gaze-dependent.

    bool GetEyeCentres(const SArray1dC<Point2dC> &points, PairC<Point2dC> &eyeCentres) const
    {  return Body().GetEyeCentres(points, eyeCentres); }
    //: Return coordinates of the eye centres 'eyeCentres' for the appearance with feature points 'points'.
    //  This function computes the coordinates of the eye centres from the position of the other feature points.
    //  The eye centre is defined as the midpoint of the segment defined by the left and right corners of the outer contour of the eye.
    //  The eye centre IS NOT gaze-dependent.

    bool GetIrisCentres(const AAMAppearanceC &app, PairC<Point2dC> &irisCentres) const
    {  return Body().GetIrisCentres(app, irisCentres); }
    //: Return coordinates of the iris centres 'irisCentres' for the appearance defined by 'app'.
    //  This function computes the coordinates of the iris centres from the position of the other feature points.
    //  The iris centre is defined as the centroid of the corners defining the inner contour of the eye.
    //  The iris centre IS gaze-dependent.

    bool GetIrisCentres(const SArray1dC<Point2dC> &points, PairC<Point2dC> &irisCentres) const
    {  return Body().GetIrisCentres(points, irisCentres); }
    //: Return coordinates of the iris centres 'irisCentres' for the appearance with feature points 'points'.
    //  This function computes the coordinates of the iris centres from the position of the other feature points.
    //  The iris centre is defined as the centroid of the corners defining the inner contour of the eye.
    //  The iris centre is gaze-dependent.

    const AAMMultiResActiveAppearanceModelC &MultiResActiveAppearanceModel() const
    { return Body().MultiResActiveAppearanceModel(); }
    //: Multi-resolution active appearance model.

    const DListC<Tuple2C<SArray1dC<Point2dC>, PairC<Point2dC> > > &InitialisationList() const
    { return Body().InitialisationList(); }
    //: List of initialisation for the search.
    //  Each initialisation instance consists of an array of coordinates of the feature points defining the shape and the coordinates of the eye centres for this initialisation.

    friend class AAMFaceLocalisationBodyC;
  };


}


#endif
