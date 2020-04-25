// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2006, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id: aamSynthesizeFrontal.cc,v 1.3 2005/07/25 16:31:07 ees1wc Exp $"
//! lib=RavlAAM
//! file="Ravl/CompVision/ActiveAppearanceModels/aamSynthesizeFrontal.cc"
//! docentry="Ravl.API.Images.AAM"
//! userlevel="Normal"
//! author="Jean-Yves Guillemaut"

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
#include "Ravl/DList.hh"
#include "Ravl/DLIter.hh"

#include "Ravl/Image/ImagePointFeatureSet.hh"
#include "Ravl/Image/AAMAppearance.hh"
#include "Ravl/Image/AAMAppearanceUtil.hh"
#include "Ravl/Image/AAMShapeModel.hh"
#include "Ravl/Image/AAMAppearanceModel.hh"
#include "Ravl/Image/AAMActiveAppearanceModel.hh"
#include "Ravl/Image/AAMPoseEstimate.hh"
#include "Ravl/Image/AAMMultiResActiveAppearanceModel.hh"
#include "Ravl/Image/AAMFaceLocalisation.hh"
#include "Ravl/Image/AAMFaceCorrection.hh"

using namespace RavlN;
using namespace RavlImageN;

  Affine2dC ComputeSimilarityTransf(const PairC<Point2dC> & src, const PairC<Point2dC> & dest) {

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



  bool FitAM2Image(StringC fileName, StringC fileRoot, StringC xmlName, bool noXML, ImageC<ByteT> &inImage, FilenameC imDir, bool inv, bool useMarkUp, AAMAppearanceC &resAppear, FilenameC markupDir, FilenameC GTDir, bool verbose, bool useAM, Point2dC &leftEyeI, Point2dC &rightEyeI, AAMFaceLocalisationC &FaceLoc) {

    DListC<Tuple2C<SArray1dC<Point2dC>, PairC<Point2dC> > > initialisationList = FaceLoc.InitialisationList();

    // Generate multi-resolution AAM
    AAMMultiResActiveAppearanceModelC maam = FaceLoc.MultiResActiveAppearanceModel();

    // load markup file (note that the markup file is used ONLY to check whether the image is suspect or not)
    StringC xmlFileName, posFileName;
    if(xmlName.IsEmpty()){
      // markup and image files have the same name apart from the extension
      xmlFileName = markupDir+fileRoot+".xml";
      posFileName = GTDir+fileRoot+".pos";
    } else {
      // markup file name is different from image file name and must therefore be read from file list
      xmlFileName = xmlName;
      StringListC posStrlist1(xmlName,"/");
      StringListC posStrlist(posStrlist1.Last(),".");
      posFileName = GTDir+posStrlist.Nth(0)+".pos";
    }
    // skip suspect images
    ImagePointFeatureSetC fs1;
    if(!noXML) {
      if(!Load(xmlFileName,fs1)) {
        cerr << "WARNING: Failed to load file ' featureSet File \n";
        return false;
      }
      if(fs1.IsSuspect())
        return false;
    }

    // load current image
    Load(imDir+fileName,inImage);
    if(verbose) {
      Save("@X: input image",inImage);
    }

    // load GT eye centres to initialise search
    IStreamC fin(posFileName);
    fin >> leftEyeI >> rightEyeI;

    if(inv){
      RealT ftmp = leftEyeI.X();
      leftEyeI.X() = leftEyeI.Y();
      leftEyeI.Y() = ftmp;
      ftmp = rightEyeI.X();
      rightEyeI.X() = rightEyeI.Y();
      rightEyeI.Y() = ftmp;
    }
    PairC<Point2dC> eyeCentres(leftEyeI, rightEyeI);

    if(useMarkUp) {      // do not fit AAM but use manual markup
      StringListC xmlStrlist(xmlFileName,"/");
      HashC<IntT,IntT> typeMap;
      HashC<StringC,IntT> namedTypeMap;
      bool useTypeId;
      AAMAppearanceC bestAM = AAMAppearanceC(LoadFeatureFile(xmlStrlist.Last(),markupDir,typeMap,namedTypeMap,useTypeId,false,true));
      AAMAppearanceC bestAMs = AAMAppearanceC(bestAM);
      if(useAM) {
        resAppear = bestAM;
      } else {
        resAppear = bestAMs;
      }
    } else {
      FaceLoc.FitModel(inImage, eyeCentres, resAppear, useAM);
    }

    // display converged models
    if(verbose) {
      resAppear.Draw("@X:Shape model");
      Save("@X:Appearance model",resAppear.Image());
    }

    return true;
  }

//: Synthesize frontal views of faces from a single view in an arbitrary pose.
//  This program uses an multi-resolution Active Appearance Model and a statistical model of pose variation to locate a face in an arbitrary pose in an input image and synthesize a frontal view. The input image to be processed are located in the directory defined by the option -d and are specified either by providing a list of file names (option "-l") or by providing the extension of the image files (option "-e"). There are two possible modes for locating the face: manual or semi-automatic. In the manual mode, the mark-up points of some XML files are used to initialise the appearance (option -xl and -useMarkUp). In the semi-automatic mode, the AAM search is initialised with the coordinates of the eyes centre (option -gtd). The convergence of the AAM may be difficult in the case of large pose variations; for this reason it is possible to specify several possible shapes for initialisation (option -tf), typically these initialisation correspond to different poses such as subject looking left, right, up or down. The multi-resolution AAM consists of an arbitrary number of AAMs sorted in order of increasing resolution. The statistical model of pose variation must be compatible with the highest resolution AAM. The size of the output normalised image and the position of the centres of the eyes in this image are specified with the options -os, -le and -re. For more information type "aamSynthesizeFrontal -help".
int main (int argc, char** argv)
{
  OptionC cmd(argc,argv);
  FilenameC imDir = cmd.String("d","/vol/vssp/facedb/xm2fdb/images/mpeg7/xm2vts/", "Directory comtaining input images.");
  FilenameC imList = cmd.String("l","/vol/vssp/vampire/people/jean-yves/appmodel/lists/pose_half2.list", "Input file containing list of image file names to process.");
  StringC ext = cmd.String("e","ppm","Extention to search for image files (needed only if no list is provided, i.e. option -l is set to empty string \"\").");
  FilenameC GTDir = cmd.String("gtd","/vol/vssp/vampire/people/jean-yves/appmodel/eyeGT/nonfrontal/", "Directory containing ground truth eye positions to initialise search.");
  FilenameC tpDir = cmd.String("tf","/vol/vssp/vampire/people/jean-yves/appmodel/template5/", "Directory containing shape initialisation files for AAM search.");
  FilenameC xmlList = cmd.String("xl","", "Input file containing list of xml file names (only needed if xml and image names are labelled differently apart from extension).");
  bool useMarkUp = cmd.Boolean("useMarkUp",false,"Use manually marked-up points? true=yes. This desactivates the semi-automatic AAM search.");
  bool noXML = cmd.Boolean("noXML",false,"No existing XML files for this database? true=yes. This desactivates the use of XML files to cheack for suspect files.");
  StringC ip = cmd.String("i","/vol/vssp/vampire/people/jean-yves/appmodel/models/pose_sm_all_half1_100x88m98_98_98.abs","Input statistical model of pose variation.");
  bool useAM = cmd.Boolean("uam",false,"Synthesize texture of corrected image using appearance model if true, otherwise compute texture by warping of the original image.");
  DListC<StringC> aamList  = cmd.List("aam", "List of AAM sorted in order of increasing resolution.");
  FilenameC  exampleFile = cmd.String("ex","/vol/vssp/vampire/people/jean-yves/appmodel/markup/frontal/000_1_1_000.xml", "Example of markup file representing an appearance (required to identify which feature points are associated to the eyes).");
  Index2dC imageSize = cmd.Index2d("os",55,51,"Dimensions of the normalised output image.");
  Point2dC leftEyePos = cmd.Point2d("le",19.0,38.0,"Coordinates of the left eye in the normalised output image.");
  Point2dC rightEyePos = cmd.Point2d("re",19.0,12.0,"Coordinates of the right eye in the normalised output image.");
  FilenameC markupDir = cmd.String("md","/vol/vssp/vampire/people/jean-yves/appmodel/markup/nonfrontal/", "Directory containing markup files (useful to initialise manually the AM or to check for suspect files).");
  FilenameC outDir = cmd.String("od","/vol/vssp/vampire/people/jean-yves/results_opt/mesh2D_5/", "Output directory for frontal images corrected based on AAM.");
  FilenameC outDir2 = cmd.String("od2","/vol/vssp/vampire/people/jean-yves/appmodel/models/results/basic/", "Output directory for basic corrected images (geometric normalisation only).");
  FilenameC outDir3 = cmd.String("od3","/vol/vssp/vampire/people/jean-yves/results_opt/mesh2D_5_app/", "Output directory for intermediate appearance (before geometric normalization).");
  UIntT smoothSize = cmd.Int("smooth", 5, "Size of convolution kernel used for smoothing.");
  bool verbose = cmd.Boolean("v",false,"Verbose mode.");
  bool inv = cmd.Boolean("inv",false,"Inverse eyes coordinates (useful when a different convention is used for storing eyes coordinates).");
  bool useThinPlateSplines = cmd.Boolean("thinPlate",false,"Use thin plate splines for warping.");
  RealT warpSigma = cmd.Real("wsigma",2,"Stiffness thin plate spline warping.");

  cmd.Check();

  SampleC<AAMActiveAppearanceModelC> saam;

  for(DLIterC<StringC>it(aamList);it;it++) {
    // load list of appearance models
    AAMActiveAppearanceModelC aam;
    if(!Load(it.Data(),aam)) {
      cerr << "Failed to load model from '" << it.Data() << "' \n";
      return 1;
    }
    saam.Append(aam);
  }

  AAMMultiResActiveAppearanceModelC maam(saam);

  AAMFaceLocalisationC FaceLoc(maam, exampleFile, tpDir);

  // initialisation

  // load pose estimator
  BinIStreamC fin(ip);
  AAMPoseEstimateC posest(fin);

  AAMFaceCorrectionC FaceCor(posest,saam.Last().AppearanceModel());

  // load list of images
  DListC<StringC> fileList;
  if(!imList.IsEmpty())
  {
    // load list from file
    TextFileC fl(imList);
    for(UIntT i=1;i<=fl.NoLines();i++)  fileList.InsLast(fl[i].TopAndTail());
  }
  else
  {
    DirectoryC md(imDir);
    if(!md.IsValid()) {
      cerr << "Can't find directory '" << imDir << "'\n";
      return 0;
    }
    fileList = md.List("",StringC(".")+ext);
  }

  DListC<StringC> xmlFileList;
  if(!xmlList.IsEmpty()){
    // load list from file
    TextFileC xmlfl(xmlList);
    for(UIntT i=1;i<=xmlfl.NoLines();i++)
      xmlFileList.InsLast(markupDir+xmlfl[i].TopAndTail());

    // xml and image lists must be the same size
    if(xmlList.Size() != imList.Size()) {
      cerr << "ERROR: xml and image lists must be the same size.\n";
      return 1;
    }
  }

  // process each image
  for(UIntT i=0;i<fileList.Size();i++) {

    // fit AM to image
    ImageC<ByteT> inImage;
    AAMAppearanceC resAppear;
    StringListC strlist(fileList.Nth(i),".");
    StringC fileRoot = strlist.Nth(0);
    Point2dC leftEyeI;
    Point2dC rightEyeI;

    bool isCorrect;
    if(xmlFileList.IsEmpty()) {
      isCorrect = FitAM2Image(fileList.Nth(i), fileRoot, "", noXML, inImage, imDir, inv, useMarkUp, resAppear, markupDir, GTDir, verbose, useAM, leftEyeI, rightEyeI, FaceLoc);
    } else {
      isCorrect = FitAM2Image(fileList.Nth(i), fileRoot, xmlFileList.Nth(i), noXML, inImage, imDir, inv, useMarkUp, resAppear, markupDir, GTDir, verbose, useAM, leftEyeI, rightEyeI, FaceLoc);
    }
    if(!isCorrect) continue;

    // correct pose to frontal
    AAMAppearanceC frontalAppear;
    PairC<RealT> poseVals;
    FaceCor.CorrectPose(resAppear,useAM,frontalAppear,poseVals,useThinPlateSplines,warpSigma);

    Polygon2dC bounds = ConvexHull(frontalAppear.Points());
    ImageRectangleC rect1 = bounds.BoundingRectangle().IndexRange();
    rect1 = rect1.Intersection(frontalAppear.Image().Rectangle());
    ImageC<ByteT> frontalAppearIm(frontalAppear.Image(),rect1);
    if(verbose) {
      frontalAppear.Draw("@X:frontal appearance");
    }
    if(!outDir3.IsEmpty()) {
      if(!Save(outDir3+fileRoot+".pgm",frontalAppearIm)) {
        cerr << "WARNING: Failed to save appearance file " << outDir3+fileRoot+".pgm" << endl;
        return  1;
      }
    }

    // removes any asymetry left and carry out geometric normalisation
    AAMAppearanceC correctedAppear;
    ImageRectangleC outRect(imageSize.Row().V(),imageSize.Col().V());


    PairC<Point2dC> eyeCentres;
    FaceLoc.GetEyeCentres(frontalAppear, eyeCentres);

    FaceCor.GeometricNormalisation(frontalAppear, eyeCentres.Data2(), eyeCentres.Data1(), leftEyePos, rightEyePos, outRect, correctedAppear,smoothSize);
    if(verbose) {
      correctedAppear.Draw("@X:normalised frontal appearance");
    }

    if(!outDir.IsEmpty()) {
      if(!Save(outDir+fileRoot+".pgm",correctedAppear.Image())) {
        cerr << "WARNING: Failed to save corrected file " << outDir+fileRoot+".pgm" << endl;
        return  1;
      }

      // convert pose to degrees
      poseVals.Data1() *= 180.0/RavlConstN::pi;
      poseVals.Data2() *= 180.0/RavlConstN::pi;

      // save pose estimate
      OStreamC fout(outDir+fileRoot+".txt");
      fout << poseVals;

      if(verbose) {
        Save("@X: Synthesized frontal view",correctedAppear.Image());
        cerr << "Pan angle: \t" << poseVals.Data1() << " degrees" << endl;
        cerr << "Tilt angle: \t" << poseVals.Data2() << " degrees" << endl << endl;
      }
    }

    // compute frontal view from simple geometric normalisation of input image
    ImageC<ByteT> basicCorrectedImage(outRect);
    FaceCor.GeometricNormalisation(inImage, rightEyeI, leftEyeI, leftEyePos, rightEyePos, basicCorrectedImage,smoothSize);

    if(verbose) {
      Save("@X: basic corrected image",basicCorrectedImage);
    }
    if(!outDir2.IsEmpty()) {
      if(!Save(outDir2+fileRoot+".pgm",basicCorrectedImage)) {
        cerr << "WARNING: Failed to save corrected file " << outDir2+fileRoot+".pgm" << endl;
        return  1;
      }
    }

    if(verbose) {
      // wait for signal to process next model
      char tmp;
      cin >> tmp;
      cin.ignore(100,'\n');
    }

  }

  return 0;
}
