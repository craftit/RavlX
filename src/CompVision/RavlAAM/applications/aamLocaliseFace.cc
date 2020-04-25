// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2006, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlAAM
//! file="Ravl/CompVision/ActiveAppearanceModels/aamLocaliseFace.hh"
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
#include "Ravl/Image/AAMMultiResActiveAppearanceModel.hh"
#include "Ravl/Image/AAMFaceLocalisation.hh"

using namespace RavlN;
using namespace RavlImageN;


//: Localise face features in an image.
// This program uses an multi-resolution Active Appearance Model to localise a face in an arbitrary pose in an input image. 
// The input image to be processed are located in the directory defined by the option -d and are specified either by providing a list of file names (option "-l") or by providing the extension of the image files (option "-e").
//
// The location of the face is semi-automatic in the sense that the AAM search must be initialised with the coordinates of the eyes centre (option -gtd). 
//
// The convergence of the AAM may be difficult in the case of large pose variations; for this reason it is possible to 
// specify several possible shapes for initialisation (option -tf), typically these initialisation correspond to 
// different poses such as subject looking left, right, up or down. 
//
// The multi-resolution AAM consists of an arbitrary number of AAMs sorted in order of increasing resolution. For more information type "aamLocaliseFace -help".
int main (int argc, char** argv)
{
  OptionC cmd(argc,argv);
  FilenameC imDir = cmd.String("d","/vol/vssp/facedb/xm2fdb/images/mpeg7/xm2vts/", "Directory comtaining input images.");
  FilenameC imList = cmd.String("l","/vol/vssp/vampire/people/jean-yves/appmodel/lists/pose_half2.list", "Input file containing list of image file names to process.");
  StringC ext = cmd.String("e","ppm","Extention to search for image files (needed only if no list is provided, i.e. option -l is set to empty string \"\").");
  FilenameC GTDir = cmd.String("gtd","/vol/vssp/vampire/people/jean-yves/appmodel/eyeGT/nonfrontal/", "Directory containing ground truth eye positions to initialise search.");
  FilenameC tpDir = cmd.String("tf","/vol/vssp/vampire/people/jean-yves/appmodel/template5/", "Directory containing shape initialisation files for AAM search.");
  StringC ip = cmd.String("i","/vol/vssp/vampire/people/jean-yves/appmodel/models/pose_sm_all_half1_100x88m98_98_98.abs","Input statistical model of pose variation.");
  bool useAM = cmd.Boolean("uam",false,"Synthesize texture of corrected image using appearance model if true, otherwise compute texture by warping of the original image.");
  DListC<StringC> aamList  = cmd.List("aam", "List of AAM sorted in order of increasing resolution.");
  FilenameC  exampleFile = cmd.String("ex","/vol/vssp/vampire/people/jean-yves/appmodel/markup/frontal/000_1_1_000.xml", "Example of markup file representing an appearance (required to identify which feature points are associated to the eyes).");
  Index2dC imageSize = cmd.Index2d("os",55,51,"Dimensions of the normalised output image.");
  Point2dC leftEyePos = cmd.Point2d("le",19.0,38.0,"Coordinates of the left eye in the normalised output image.");
  Point2dC rightEyePos = cmd.Point2d("re",19.0,12.0,"Coordinates of the right eye in the normalised output image.");
  FilenameC markupDir = cmd.String("md","/vol/vssp/vampire/people/jean-yves/appmodel/markup/nonfrontal/", "Directory containing markup files (useful to initialise manually the AM or to check for suspect files).");
  FilenameC outDir = cmd.String("od","/vol/vssp/vampire/people/jean-yves/results_opt/mesh2D_5/", "Output directory for frontal images corrected based on AAM.");
  bool verbose = cmd.Boolean("v",false,"Verbose mode.");
  bool inv = cmd.Boolean("inv",false,"Inverse eyes coordinates (useful when a different convention is used for storing eyes coordinates).");

  cmd.Check();

  SampleC<AAMActiveAppearanceModelC> saam;

  for(DLIterC<StringC> it(aamList);it;it++) {
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

  // process each image
  for(UIntT i=0;i<fileList.Size();i++) {

    // fit AM to image
    ImageC<ByteT> inImage;
    AAMAppearanceC resAppear;
    StringListC strlist(fileList.Nth(i),".");
    StringC fileRoot = strlist.Nth(0);
    Point2dC leftEyeI;
    Point2dC rightEyeI;


    DListC<Tuple2C<SArray1dC<Point2dC>, PairC<Point2dC> > > initialisationList = FaceLoc.InitialisationList();

    // Generate multi-resolution AAM
    AAMMultiResActiveAppearanceModelC maam = FaceLoc.MultiResActiveAppearanceModel();

    // load current image
    Load(imDir+fileList.Nth(i),inImage);
    if(verbose) {
      Save("@X: input image",inImage);
    }

    // load GT eye centres to initialise search
    StringC posFileName = GTDir+fileRoot+".pos";
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

    FaceLoc.FitModel(inImage, eyeCentres, resAppear, useAM);

    // display converged models
    if(verbose) {
      resAppear.Draw("@X:Shape model");
      Save("@X:Appearance model",resAppear.Image());
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
