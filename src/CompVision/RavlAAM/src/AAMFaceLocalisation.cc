// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////////////////////////////
//! file =      "Ravl/CompVision/ActiveAppearanceModels/AAMFaceLocalisation.cc"
//! author =    "Jean-Yves Guillemaut"
//! lib=RavlAAM
//! date =      "01/03/2006"

#include "Ravl/Image/ImgIO.hh"
#include "Ravl/Option.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ImageConv.hh"
#include "Ravl/IO.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/DP/FileFormatIO.hh"
#include "Ravl/Array1d.hh"
#include "Ravl/Array1dIter.hh"
#include "Ravl/Point2d.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/Affine2d.hh"
#include "Ravl/OS/Directory.hh"
#include "Ravl/StringList.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/PatternRec/SampleIter.hh"
#include "Ravl/PatternRec/FuncMeanProjection.hh"
#include "Ravl/PatternRec/DistanceEuclidean.hh"
#include "Ravl/Array2dIter3.hh"
#include "Ravl/Text/TextFile.hh"
#include "Ravl/Image/DrawCross.hh"
#include "Ravl/Image/DrawCircle.hh"
#include "Ravl/Image/ImageRectangle.hh"
#include "Ravl/Image/WarpScale.hh"
#include "Ravl/Sums1d2.hh"
#include "Ravl/SumsNd2.hh"
#include "Ravl/Vector.hh"
#include "Ravl/Vector2d.hh"
#include "Ravl/Affine2d.hh"
#include "Ravl/Image/GaussConvolve2d.hh"
#include "Ravl/Image/HistogramEqualise.hh"
#include "Ravl/Polygon2d.hh"
#include "Ravl/Tuple2.hh"

#include "Ravl/Image/ImagePointFeatureSet.hh"
#include "Ravl/Image/AAMAppearance.hh"
#include "Ravl/Image/AAMAppearanceUtil.hh"
#include "Ravl/Image/AAMShapeModel.hh"
#include "Ravl/Image/AAMAppearanceModel.hh"
#include "Ravl/Image/AAMActiveAppearanceModel.hh"
#include "Ravl/Image/AAMMultiResActiveAppearanceModel.hh"
#include "Ravl/Image/AAMFaceLocalisation.hh"

namespace RavlImageN {

  //: Constructor.
  //!param: saam - Multi-resolution AAM.
  //!param: exampleFile - Example of markup file representing an appearance.
  //  The example is used to learn the indices of the control points representing the eyes from the labels in the XML file. This is necessary to be able to compute the location of the centre of the eyes from an appearance or to compute the position of the appearance in the image from the position of its eye centres.
  AAMFaceLocalisationBodyC::AAMFaceLocalisationBodyC(const AAMMultiResActiveAppearanceModelC &saam, FilenameC exampleFile)
    : maam(saam)
  {
    EyeCentresInit(exampleFile);
    IrisCentresInit(exampleFile);
    TemplatesInit();
  }

  //: Constructor.
  //!param: saam - Multi-resolution AAM.
  //!param: exampleFile - Example of markup file representing an appearance.
  //!param: tpDir - Name of directory containing list of shape initialisations.
  //  The example is used to learn the indices of the control points representing the eyes from the labels in the XML file. This is necessary to be able to compute the location of the centre of the eyes from an appearance or to compute the position of the appearance in the image from the position of its eye centres. A shape initialisation consists of saved mean points representing an instance of a shape. These are used to provide different initialisations for the AAM search algorithm, which increases the range of convergence of the algorithm. Typically the possible initialisation consists of mean shape corresponding to different poses such as left, right, up, down and frontal, or more if necessary.
  AAMFaceLocalisationBodyC::AAMFaceLocalisationBodyC(const AAMMultiResActiveAppearanceModelC &saam, FilenameC exampleFile, FilenameC tpDir)
    : maam(saam)
  {
    EyeCentresInit(exampleFile);
    IrisCentresInit(exampleFile);
    TemplatesInit(tpDir);
  }

  //: Sets parameters 'leftEyeIDs' and 'rightEyeIDs' for locating centres of the eyes.
  bool AAMFaceLocalisationBodyC::EyeCentresInit(FilenameC exampleFile)
  {
    // determine mark-ups related to the outer eye for eye centre estimation
    // Warning: left and right sides have been swapped in mark-up file
    // i.e. the true left eye is called right because appears on the right handside in the image and vice versa
    ImagePointFeatureSetC fs;
    if(!Load(exampleFile,fs)) {
      cerr << "WARNING: Failed to load file ' featureSet File \n";
      return  1;
    }
    for(HashIterC<IntT, ImagePointFeatureC> it(fs.FeatureIterator());it;it++ ) {
      ImagePointFeatureC feat= it.Data();
      if(feat.Description().contains("Left Outer Eye-0") || feat.Description().contains("Left Outer Eye-4"))
        leftEyeIDs.InsLast(feat.TypeID()-1);
      else if(feat.Description().contains("Right Outer Eye-0") || feat.Description().contains("Right Outer Eye-4"))
        rightEyeIDs.InsLast(feat.TypeID()-1);
    }

    return true;
  }

  //: Sets parameters 'leftIrisIDs' and 'rightIrisIDs' for locating centres of the eyes.
  bool AAMFaceLocalisationBodyC::IrisCentresInit(FilenameC exampleFile)
  {
    // determine mark-ups related to the inner eye for iris centre estimation
    // Warning: left and right sides have been swapped in mark-up file
    // i.e. the true left eye is called right because appears on the right handside in the image and vice versa
    ImagePointFeatureSetC fs;
    if(!Load(exampleFile,fs)) {
      cerr << "WARNING: Failed to load file ' featureSet File \n";
      return  1;
    }
    for(HashIterC<IntT, ImagePointFeatureC> it(fs.FeatureIterator());it;it++ ) {
      ImagePointFeatureC feat= it.Data();
      if(feat.Description().contains("Left Inner Eye"))
        leftIrisIDs.InsLast(feat.TypeID()-1);
      else if(feat.Description().contains("Right Inner Eye"))
        rightIrisIDs.InsLast(feat.TypeID()-1);
    }

    return true;
  }

  //: Sets list of initiliasation 'initialisationList' for AAM search.
  // If no value is provided, the search will be initialised with the mean appearance.
  bool AAMFaceLocalisationBodyC::TemplatesInit(FilenameC tpDir)
  {
    // load list of templates
    DListC<StringC> tpList;
    if(!tpDir.IsEmpty()) {
      DirectoryC md2(tpDir);
      if(!md2.IsValid()) {
        cerr << "Can't find directory '" << tpDir << "'\n";
        return 0;
      }
      tpList = md2.List("",StringC(".abs"));
    }

    // create list of mean shapes and the associated eye centres
    if(!tpList.IsEmpty()) {
      for(UIntT k=0;k<tpList.Size();k++) {

        SArray1dC<Point2dC> meanPoints;
        StringC tpFile = tpDir+tpList.Nth(k);

        if(!Load(tpFile,meanPoints)) {
          cerr << "Failed to load mean point file from '" << tpFile << "' \n";
          return 1;
        }

        // get the eye centres positions
        PairC<Point2dC> eyeCentresT;
        GetEyeCentres(meanPoints, eyeCentresT);

        Tuple2C<SArray1dC<Point2dC>, PairC<Point2dC> > temp_init(meanPoints, eyeCentresT);
        initialisationList.InsLast(temp_init);
      }
    } else {
      // create mean shape
      SArray1dC<Point2dC> meanPoints = maam.Last().AppearanceModel().ShapeModel().MeanPoints();

      // get the eye centres positions
      PairC<Point2dC> eyeCentres;
      GetEyeCentres(meanPoints, eyeCentres);

      Tuple2C<SArray1dC<Point2dC>, PairC<Point2dC> > temp_init(meanPoints, eyeCentres);
      initialisationList.InsLast(temp_init);
    }

    return true;
  }

  //: Return coordinates of the eye centres 'eyeCentres' for the appearance defined by 'app'.
  //  This function computes the coordinates of the eye centres from the position of the other feature points.
  //  The eye centre is defined as the midpoint of the segment defined by the left and right corners of the outer contour of the eye.
  bool AAMFaceLocalisationBodyC::GetEyeCentres(const AAMAppearanceC &app, PairC<Point2dC> &eyeCentres) const
  {
    return GetEyeCentres(app.Points(), eyeCentres);
  }

  //: Return coordinates of the eye centres 'eyeCentres' for the appearance with feature points 'points'.
  //  This function computes the coordinates of the eye centres from the position of the other feature points.
  //  The eye centre is defined as the midpoint of the segment defined by the left and right corners of the outer contour of the eye.
  bool AAMFaceLocalisationBodyC::GetEyeCentres(const SArray1dC<Point2dC> &points, PairC<Point2dC> &eyeCentres) const
  {
    // get the eye centres positions
    Point2dC leftEye(0.0,0.0), rightEye(0.0,0.0);

    for(DLIterC<IntT>It(leftEyeIDs);It.IsElm();It.Next())
      leftEye += points[It.Data()];
    leftEye /= (RealT) leftEyeIDs.Size();

    for(DLIterC<IntT>It(rightEyeIDs);It.IsElm();It.Next())
      rightEye += points[It.Data()];
    rightEye /= (RealT) rightEyeIDs.Size();

    eyeCentres = PairC<Point2dC>(leftEye, rightEye);

    return true;
  }

  bool AAMFaceLocalisationBodyC::GetIrisCentres(const AAMAppearanceC &app, PairC<Point2dC> &irisCentres) const
  {
    return GetIrisCentres(app.Points(), irisCentres);
  }

  bool AAMFaceLocalisationBodyC::GetIrisCentres(const SArray1dC<Point2dC> &points, PairC<Point2dC> &irisCentres) const
  {
    // get the iris centres positions
    Point2dC leftIris(0.0,0.0), rightIris(0.0,0.0);

    for(DLIterC<IntT>It(leftIrisIDs);It.IsElm();It.Next())
      leftIris += points[It.Data()];
    leftIris /= (RealT) leftIrisIDs.Size();

    for(DLIterC<IntT>It(rightIrisIDs);It.IsElm();It.Next())
      rightIris += points[It.Data()];
    rightIris /= (RealT) rightIrisIDs.Size();

    irisCentres = PairC<Point2dC>(leftIris, rightIris);

    return true;
  }

  //: Multi-resolution AAM search algorithm.
  //!param: inImage - Input image.
  //!param: eyeCentres - Estimate of the position of the centre of the eyes.
  //!param: resAppear - Output appearance fitted to the image.
  //!param: useAM - Synthesize texture using appearance model? Yes = true. If set to false the texture of the appearance image is obtained from the input image, i.e. the search algorithm is used only to find the location of the feature points defining the shape.
  bool AAMFaceLocalisationBodyC::FitModel(const ImageC<ByteT> &inImage, const PairC<Point2dC> &eyeCentres, AAMAppearanceC &resAppear, bool useAM) const
  {
    ImageC<RealT> rImage(inImage.Frame());
    for(Array2dIter2C<RealT,ByteT> it(rImage,inImage);it;it++)
      it.Data1() = (RealT) it.Data2();
    
    return FitModel(rImage, eyeCentres, resAppear, useAM);
  }

  //: Multi-resolution AAM search algorithm.
  //!param: inImage - Input image.
  //!param: eyeCentres - Estimate of the position of the centre of the eyes.
  //!param: aamParameters - Recovered output parameters fitting the model to the image.
  
  bool AAMFaceLocalisationBodyC::FitModel(const ImageC<ByteT> &inImage, const PairC<Point2dC> &eyeCentres, VectorC &aamParameters) const 
  {
    ImageC<RealT> rImage(inImage.Frame());
    for(Array2dIter2C<RealT,ByteT> it(rImage,inImage);it;it++)
      it.Data1() = (RealT) it.Data2();
    
    return FitModel(rImage,eyeCentres,aamParameters);
  }

  //: Multi-resolution AAM search algorithm.
  //!param: inImage - Input image.
  //!param: eyeCentres - Estimate of the position of the centre of the eyes.
  //!param: aamParameters - Recovered output parameters fitting the model to the image.
  //!param: useAM - Synthesize texture using appearance model? Yes = true. If set to false the texture of the appearance image is obtained from the input image, i.e. the search algorithm is used only to find the location of the feature points defining the shape.
  
  bool AAMFaceLocalisationBodyC::FitModel(const ImageC<RealT> &inImage, const PairC<Point2dC> &eyeCentres, VectorC &aamParameters) const {
    
    Point2dC leftEyeI = eyeCentres.Data1();
    Point2dC rightEyeI = eyeCentres.Data2();

    AAMAppearanceModelC am1 = maam.First().AppearanceModel();
    AAMAppearanceModelC am3 = maam.Last().AppearanceModel();
    AAMShapeModelC sm = am3.ShapeModel();

    // fit AAM trying different templates for initialisation
    VectorC bestParam;
    RealT bestRes = 1e10;

    for(UIntT k=0;k<initialisationList.Size();k++) {

      // transform template shape so eye centres coincide with GT initialisation
      SArray1dC<Point2dC> meanPoints = initialisationList.Nth(k).Data1();
      PairC<Point2dC> eyeCentres_i = initialisationList.Nth(k).Data2();
      Affine2dC T = ComputeSimilarityTransf(eyeCentres_i, PairC<Point2dC>(leftEyeI,rightEyeI));
      SArray1dC<Point2dC> initPoints(meanPoints.Size());
      for(UIntT i=0;i<meanPoints.Size();i++) {
        initPoints[i] = T*meanPoints[i];
       }

      // compute the initial parameters associated with the corresponding appearance
      VectorC aParam;
      am1.MeanTextureAppearanceParameters(initPoints, aParam);

      // do multiresolution search
      VectorC resParam;
      RealT residual;
      resParam = maam.RefinePose(inImage,aParam,residual);

      // keep best fit
      if(residual<bestRes) {
        bestRes = residual;
        bestParam = resParam;
      }
    }
    aamParameters = bestParam;
    
    return true;
  }
  
  
  //: Multi-resolution AAM search algorithm.
  //!param: inImage - Input image.
  //!param: eyeCentres - Estimate of the position of the centre of the eyes.
  //!param: resAppear - Output appearance fitted to the image.
  //!param: useAM - Synthesize texture using appearance model? Yes = true. If set to false the texture of the appearance image is obtained from the input image, i.e. the search algorithm is used only to find the location of the feature points defining the shape.
  
  bool AAMFaceLocalisationBodyC::FitModel(const ImageC<RealT> &inImage, const PairC<Point2dC> &eyeCentres, AAMAppearanceC &resAppear, bool useAM) const
  {
    VectorC bestParam;
    
    if(!FitModel(inImage,eyeCentres,bestParam))
      return false;

    AAMAppearanceModelC am3 = maam.Last().AppearanceModel();
    AAMShapeModelC sm = am3.ShapeModel();
    
    // define converged appearance (either from shape+image or from appearance model)
    if(useAM) {
      resAppear = AAMAppearanceC(am3.Synthesize(bestParam, 1.0));
    } else {
      resAppear = AAMAppearanceC(sm.Synthesize(am3.ShapeParameters(bestParam)),inImage);
    }

    return true;
  }

  //: Compute transformation composed of a rotation, translation, and scaling which maps the two eyes from position 'src' to 'dest'.
  Affine2dC AAMFaceLocalisationBodyC::ComputeSimilarityTransf(const PairC<Point2dC> & src,
                                                                    const PairC<Point2dC> & dest
                                                                    ) const
  {
    Point2dC src1(src.A()), src2(src.B()), dest1(dest.A()), dest2(dest.B());
    Vector2dC d1 = src1 - src2;
    Vector2dC d2 = dest1 - dest2;
    RealT rot = d2.Angle() - d1.Angle();      // Sort out rotation.
    RealT scale = d2.Modulus() / d1.Modulus();// Sort out scaling.

    // Compose rotation/scale matrix.
    Matrix2dC rotm = Matrix2dC(Cos(rot) * scale,-Sin(rot) * scale, Sin(rot) * scale,Cos(rot) * scale);
    // Calculate translation.
    Point2dC cent= (dest1 + dest2)/2;
    Point2dC dcent = rotm * (src1 + src2)/2;
    Vector2dC off = (cent - dcent);
    return Affine2dC(rotm,off);
  }


}
