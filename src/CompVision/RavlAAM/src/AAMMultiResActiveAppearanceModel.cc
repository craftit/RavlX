// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////////////////////////////
//! file =      "Ravl/CompVision/ActiveAppearanceModels/AAMMultiresActiveAppearanceModel.cc"
//! author =    "Jean-Yves Guillemaut"
//! lib=RavlAAM
//! date =      "01/03/2006"
//! rcsid="$Id$"


#include "Ravl/Image/ImgIO.hh"
#include "Ravl/Option.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ImageConv.hh"
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
#include "Ravl/Vector.hh"
#include "Ravl/PatternRec/Sample.hh"
#include "Ravl/IO.hh"
#include "Ravl/VirtualConstructor.hh"

#include "Ravl/Image/ImagePointFeatureSet.hh"
#include "Ravl/Image/AAMAppearance.hh"
#include "Ravl/Image/AAMAppearanceUtil.hh"
#include "Ravl/Image/AAMShapeModel.hh"
#include "Ravl/Image/AAMAppearanceModel.hh"
#include "Ravl/Image/AAMActiveAppearanceModel.hh"
#include "Ravl/Image/AAMMultiResActiveAppearanceModel.hh"

#define PROJECT_TEXTURE 0

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif



namespace RavlImageN {

  //: Constructor from one AAM.
  AAMMultiResActiveAppearanceModelBodyC::AAMMultiResActiveAppearanceModelBodyC(const AAMActiveAppearanceModelC &aam1)
  {
    maam.Append(aam1);
  }

  //: Constructor from two AAMs.
  //!param: aam1 - Low resolution AAM.
  //!param: aam2 - High resolution AAM.
  AAMMultiResActiveAppearanceModelBodyC::AAMMultiResActiveAppearanceModelBodyC(const AAMActiveAppearanceModelC &aam1, const AAMActiveAppearanceModelC &aam2)
  {
    maam.Append(aam1);
    maam.Append(aam2);
  }

  //: Constructor from three AAMs.
  //!param: aam1 - Low resolution AAM.
  //!param: aam2 - Medium resolution AAM.
  //!param: aam3 - High resolution AAM.
  AAMMultiResActiveAppearanceModelBodyC::AAMMultiResActiveAppearanceModelBodyC(const AAMActiveAppearanceModelC &aam1, const AAMActiveAppearanceModelC &aam2, const AAMActiveAppearanceModelC &aam3)
  {
    maam.Append(aam1);
    maam.Append(aam2);
    maam.Append(aam3);
  }

  //: Constructor from an arbitrary number of AAMs sorted in order of increasing resolution.
  AAMMultiResActiveAppearanceModelBodyC::AAMMultiResActiveAppearanceModelBodyC(const SampleC<AAMActiveAppearanceModelC> &saam)
    : maam(saam)
  {
  }

  //: Load from bin stream.
  AAMMultiResActiveAppearanceModelBodyC::AAMMultiResActiveAppearanceModelBodyC(BinIStreamC &is)
    : RCBodyVC(is)
  {
    int version;
    is >> version;
    if(version != 1)
      throw ExceptionOutOfRangeC("AAMMultiResActiveAppearanceModelC:SAAMMultiResActiveAppearanceModelC(BinIStreamC &s), Bad version number in stream. ");

    is >> maam;
  }

  //: Load from stream.
  AAMMultiResActiveAppearanceModelBodyC::AAMMultiResActiveAppearanceModelBodyC(istream &is)
    : RCBodyVC(is)
  {
    int version;
    is >> version;
    if(version != 1)
      throw ExceptionOutOfRangeC("AAMMultiResActiveAppearanceModelC:SAAMMultiResActiveAppearanceModelC(istream &s), Bad version number in stream. ");

    is >> maam;
  }

  //: Save to binary stream 'out'.
  bool AAMMultiResActiveAppearanceModelBodyC::Save(BinOStreamC &out) const {
    if(!RCBodyVC::Save(out))
      return false;
    int version = 1;
    out << version << maam;
    return true;
  }

  //: Save to stream 'out'.
  bool AAMMultiResActiveAppearanceModelBodyC::Save(ostream &out) const {
    if(!RCBodyVC::Save(out))
      return false;
    int version = 1;
    out << ' ' << version << ' ' << ' ' << maam;
    return true;
  }

  //: Returns the optimum appearance parameters for the image 'img' given an initial estimate 'paramEstimate'.
  //  The optimum parameters are the ones which minimise the residual error between model and image measured in normalised texture frame (i.e. shape free images).
  // This function also returns the value of the corresponding residual error in 'diff'.
  VectorC AAMMultiResActiveAppearanceModelBodyC::RefinePose(const ImageC<ByteT> &img,const VectorC &paramEstimate, RealT &diff) const {

    ImageC<RealT> rimg(img.Frame());
    for(Array2dIter2C<RealT,ByteT> it(rimg,img);it;it++)
      it.Data1() = (RealT) it.Data2();

    return RefinePose(rimg, paramEstimate, diff);
  }

  //: Returns the optimum appearance parameters for the image 'img' given an initial estimate 'paramEstimate'.
  //  The optimum parameters are the ones which minimise the residual error between model and image measured in normalised texture frame (i.e. shape free images).
  // This function also returns the value of the corresponding residual error in 'diff'.
  VectorC AAMMultiResActiveAppearanceModelBodyC::RefinePose(const ImageC<RealT> &img,const VectorC &paramEstimate, RealT &diff) const {

    VectorC resParam;

    for(UIntT i=0;i<maam.Size();i++) {

      // Get statistical face models
      AAMActiveAppearanceModelC aam = maam.Nth(i);
      AAMAppearanceModelC am = aam.AppearanceModel();
      AAMShapeModelC sm = am.ShapeModel();

      VectorC initParam(am.Dimensions());

      // initialise current model
      if(i==0) {
        initParam = paramEstimate;
#if DODEBUG
        // overlay shape model on input image for displaying purpose only
        AAMAppearanceC initAMs = AAMAppearanceC(sm.Synthesize(am.ShapeParameters(initParam)),img);
        initAMs.Draw("@X:Shape model (init)");
#endif
      } else {
        AAMAppearanceModelC am_old = maam.Nth(i-1).AppearanceModel();
#if PROJECT_TEXTURE
        // Get shape free image of previous level
        ImageC<RealT> im_old(am_old.Mask().Frame());
        am_old.TextureImage(resParam,im_old);

        // Project (scale) to next level
        WarpScaleC<RealT,RealT> warpS(ImageRectangleC(am.MaskSize()));
        ImageC<RealT> im = warpS.Apply(im_old);

        // Get texture values
        VectorC ret(am.MaskArea()+am.NoFixedParameters()-sm.NoFixedParameters());
        am.TextureParameters(im, ret);

        // Generate vector of parameters
        initParam = am_old.ShapeParameters(resParam).From(0,sm.NoFixedParameters());
        initParam.Append(ret.From(0,am.NoFixedParameters()-sm.NoFixedParameters()));
        VectorC rawp = am_old.ShapeParameters(resParam).From(sm.NoFixedParameters());
        rawp.Append(ret.From(am.NoFixedParameters()-sm.NoFixedParameters()));
        initParam.Append(am.AppearanceModel().Apply(rawp));
#else
        // Generate vector corresponding to appearance with converged shape parameters and mean texture values
        VectorC sParam = am_old.ShapeParameters(resParam);
        am.MeanTextureAppearanceParameters(sParam, initParam);
#endif
      }

      // solution of current level
      resParam = aam.RefinePose(img,initParam,diff);
      am.MakePlausible(resParam);
#if DODEBUG
      AAMAppearanceC resAMi = am.Synthesize(initParam);
      Save("@X:Appearance model (before search)",resAMi.Image());
      AAMAppearanceC resAMs = AAMAppearanceC(sm.Synthesize(am.ShapeParameters(resParam)),img);
      resAMs.Draw("@X:Shape model");
      AAMAppearanceC resAM = am.Synthesize(resParam, 1.0);
      resAM.Image().ShiftArray(-resAM.Image().Frame().TopLeft());
      Save("@X:Appearance model",resAM.Image());
      //Save("/vol/vssp/vampire/people/jean-yves/results_opt/tmp/AM_level"+StringC(i+1)+".pgm",resAM.Image());
      char tmp_char;
      cin >> tmp_char;
#endif
    }

    return resParam;
  }

  RAVL_INITVIRTUALCONSTRUCTOR_FULL(AAMMultiResActiveAppearanceModelBodyC,AAMMultiResActiveAppearanceModelC,RCHandleVC<AAMMultiResActiveAppearanceModelBodyC>);
}
