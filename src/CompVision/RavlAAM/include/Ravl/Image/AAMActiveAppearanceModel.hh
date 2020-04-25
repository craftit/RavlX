// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_ACTIVEAPPEARANCEMODEL_HEADER
#define RAVLIMAGE_ACTIVEAPPEARANCEMODEL_HEADER 1
//! rcsid="$Id$"
//! lib=RavlAAM
//! file="Ravl/CompVision/ActiveAppearanceModels/AAMActiveAppearanceModel.hh"
//! docentry="Ravl.API.Images.AAM"
//! userlevel="Develop"
//! author="Charles Galambos"
//! example = "aamBuildActiveAppearanceModel.cc;aamCombineActiveAppearanceModel.cc"

#include "Ravl/Image/AAMAppearanceModel.hh"
#include "Ravl/PatternRec/Function.hh"
#include "Ravl/Image/GaussConvolve2d.hh"

namespace RavlImageN {

  //! userlevel=Develop
  //: Active Appearance Model (AAM).

  class AAMActiveAppearanceModelBodyC
    :public RCBodyVC
  {
  public:
    AAMActiveAppearanceModelBodyC();
    //: Constructor.

    AAMActiveAppearanceModelBodyC(BinIStreamC &is);
    //: Load from bin stream.

    AAMActiveAppearanceModelBodyC(istream &is);
    //: Load from stream.

    virtual bool Save(BinOStreamC &out) const;
    //: Save to binary stream 'out'.

    virtual bool Save(ostream &out) const;
    //: Save to stream 'out'.

    VectorC RefinePose(const ImageC<ByteT> &img, const VectorC &paramEstimate) const;
    //: Returns the optimum appearance parameters for the image 'img' given an initial estimate 'paramEstimate'.
    //  The optimum parameters are the ones which minimise the residual error between model and image measured in normalised texture frame (i.e. shape free images).

    VectorC RefinePose(const ImageC<ByteT> &img, const VectorC &paramEstimate,RealT &diff) const;
    //: Returns the optimum appearance parameters for the image 'img' given an initial estimate 'paramEstimate'.
    //  The optimum parameters are the ones which minimise the residual error between model and image measured in normalised texture frame (i.e. shape free images).
    // This function also returns the value of the corresponding residual error in 'diff'.

    VectorC RefinePose(const ImageC<RealT> &img, const VectorC &paramEstimate,RealT &diff) const;
    //: Returns the optimum appearance parameters for the image 'img' given an initial estimate 'paramEstimate'.
    //  The optimum parameters are the ones which minimise the residual error between model and image measured in normalised texture frame (i.e. shape free images).
    // This function also returns the value of the corresponding residual error in 'diff'.

    bool Design(const AAMAppearanceModelC &apm,
                const DListC<StringC> &fileList,
                const StringC &dir,
                const StringC &mirrorFile,
                UIntT incrSize,
                bool ignoreSuspect = true
                );
    //: Train the AAM given some data and an existing appearance model.
    //!param: apm        - input appearance model.
    //!param: fileList   - list of file names for training.
    //!param: dir        - name of directory containing training images.
    //!param: mirrorFile - name of mirror file to use for mirror appearances (if an empty string is provided, mirror images will not be considered).
    //!param: incrSize   - half number of displacements for each parameter when perturbing the model.
    //!param: ignoreSuspect - Ignore suspect markups ?
    // This functions considers each training appearance successively and perturbs the different parameters in order to analyse the effect of errors in the parameters on the residual error. The range of displacement is +/-0.5 standard deviation for each parameter. A value of n for 'incrSize' means that there will be 2n displacements uniformly spread on the interval [-0.5std,+0.5std] for each parameter.

    bool Design(const AAMAppearanceModelC &apm,
                SampleStreamC<AAMAppearanceC> &sample,
                UIntT incrSize
                );
    //: Train the AAM given some data and an existing appearance model.
    //!param: apm        - input appearance model.
    //!param: sample     - list of sample appearances
    //!param: incrSize   - half number of displacements for each parameter when perturbing the model.
    // This functions considers each training appearance successively and perturbs the different parameters in order to analyse the effect of errors in the parameters on the residual error. The range of displacement is +/-0.5 standard deviation for each parameter. A value of n for 'incrSize' means that there will be 2n displacements uniformly spread on the interval [-0.5std,+0.5std] for each parameter.

    bool PreDesign(const AAMAppearanceModelC &apm,
                   const DListC<StringC> &fileList,
                   const StringC &dir,
                   const StringC &mirrorFile,
                   UIntT incrSize,
                   const StringC &op,
                   bool ignoreSuspect = true
                   );
    //: Pre-process some data before final design of the AAM.
    //!param: apm        - input appearance model.
    //!param: fileList   - list of file names for training.
    //!param: dir        - name of directory containing training images.
    //!param: mirrorFile - name of mirror file to use for mirror appearances (if an empty string is provided, mirror images will not be considered).
    //!param: incrSize   - half number of displacements for each parameter when perturbing the model.
    //!param: op         - name of output file containing results of training for this list of file.
    //!param: ignoreSuspect - Ignore suspect markups ?

    // This functions considers each training appearance successively and perturbs the different parameters in order to analyse the effect of errors in the parameters on the residual error. The range of displacement is +/-0.5 standard deviation for each parameter. A value of n for 'incrSize' means that there will be 2n displacements uniformly spread on the interval [-0.5std,+0.5std] for each parameter.
    // Note: contrary to the Design method, PreDesign does not produce a complete appearance model. PreDesign needs to be followed by PostDesign in order to obtain the AAM. PreDesign allows to split the training of the AAM (which is usually computer intensive because of the number of files to process) into a large number of smaller jobs which can be run in parallel.

    bool PreDesign(const AAMAppearanceModelC &apm,
                   SampleStreamC<AAMAppearanceC> &sample,
                   UIntT incrSize,
                   const StringC &op
                   );
    //: Pre-process some data before final design of the AAM.
    //!param: apm        - input appearance model.
    //!param: sample     - list of sample appearances.
    //!param: incrSize   - half number of displacements for each parameter when perturbing the model.
    //!param: op         - name of output file containing results of training for this list of file.

    // This functions considers each training appearance successively and perturbs the different parameters in order to analyse the effect of errors in the parameters on the residual error. The range of displacement is +/-0.5 standard deviation for each parameter. A value of n for 'incrSize' means that there will be 2n displacements uniformly spread on the interval [-0.5std,+0.5std] for each parameter.
    // Note: contrary to the Design method, PreDesign does not produce a complete appearance model. PreDesign needs to be followed by PostDesign in order to obtain the AAM. PreDesign allows to split the training of the AAM (which is usually computer intensive because of the number of files to process) into a large number of smaller jobs which can be run in parallel.

    bool PostDesign(const AAMAppearanceModelC &apm,const DListC<StringC> &fileList,const StringC &dir);
    //: Terminate the design of AAM by merging results of PreDesign.
    //!param: apm        - input appearance model.
    //!param: fileList   - list of names of files containing the results of the pre-design.
    //!param: dir        - name of directory containg results of the pre-design.

    AAMAppearanceModelC &AppearanceModel()
    { return appearanceModel; }
    //: Access appearance model.

    const AAMAppearanceModelC &AppearanceModel() const
    { return appearanceModel; }
    //: Access appearance model.

    const AAMShapeModelC &ShapeModel() const
    { return appearanceModel.ShapeModel(); }
    //: Access shape model.

    VectorC ShapeParameters(const VectorC &aamParams) const
    { return appearanceModel.ShapeParameters(aamParams); }
    //: Extract shape parameters from the appearance represented by the parameter vector 'aamParams'.

    const SArray1dC<Point2dC> &RefrenceHome() const
    { return refHome; }
    //: Access home position of reference points (i.e. position of the control points in mean shape).

    SArray1dC<Point2dC> &RefrenceHome()
    { return refHome; }
    //: Access home position of reference points (i.e. position of the control points in mean shape).

    UIntT Dimensions() const
    { return appearanceModel.Dimensions(); }
    //: Size of the parameter vector.

  protected:
    SArray1dC<Point2dC> refHome;         // Home position of reference points in shape free image.
    AAMAppearanceModelC appearanceModel; // Statistical model of appearance.
    FunctionC refiner;                   // Predicts correction to apply to the model parameters given residual.
    GaussConvolve2dC<RealT> smooth;      // Gauss convolver for smoothing input images.
  };


  //! userlevel=Normal
  //: Active Appearance Model (AAM).

  class AAMActiveAppearanceModelC
    : public RCHandleVC<AAMActiveAppearanceModelBodyC>
  {
  public:

    AAMActiveAppearanceModelC(BinIStreamC &is);
    //: Load from bin stream.

    AAMActiveAppearanceModelC(istream &is);
    //: Load from stream.

    AAMActiveAppearanceModelC()
      : RCHandleVC<AAMActiveAppearanceModelBodyC>(*new AAMActiveAppearanceModelBodyC())
    {}
    //: Constructor.


  protected:
    AAMActiveAppearanceModelC(AAMActiveAppearanceModelBodyC &bod)
      : RCHandleVC<AAMActiveAppearanceModelBodyC>(bod)
    {}
    //: Body constructor.

    AAMActiveAppearanceModelC(AAMActiveAppearanceModelBodyC *bod)
      : RCHandleVC<AAMActiveAppearanceModelBodyC>(*bod)
    {}
    //: Body ptr constructor.

    AAMActiveAppearanceModelBodyC &Body()
    { return RCHandleVC<AAMActiveAppearanceModelBodyC>::Body(); }
    //: Access body.

    const AAMActiveAppearanceModelBodyC &Body() const
    { return RCHandleVC<AAMActiveAppearanceModelBodyC>::Body(); }
    //: Access body.

  public:

    VectorC RefinePose(const ImageC<ByteT> &img, const VectorC &paramEstimate) const
    { return Body().RefinePose(img,paramEstimate); }
    //: Returns the optimum appearance parameters for the image 'img' given an initial estimate 'paramEstimate'.
    //  The optimum parameters are the ones which minimise the residual error between model and image measured in normalised texture frame (i.e. shape free images).

    VectorC RefinePose(const ImageC<ByteT> &img, const VectorC &paramEstimate, RealT &diff) const
    { return Body().RefinePose(img,paramEstimate,diff); }
    //: Returns the optimum appearance parameters for the image 'img' given an initial estimate 'paramEstimate'.
    //  The optimum parameters are the ones which minimise the residual error between model and image measured in normalised texture frame (i.e. shape free images).
    // This function also returns the value of the corresponding residual error in 'diff'.

    VectorC RefinePose(const ImageC<RealT> &img, const VectorC &paramEstimate, RealT &diff) const
    { return Body().RefinePose(img,paramEstimate,diff); }
    //: Returns the optimum appearance parameters for the image 'img' given an initial estimate 'paramEstimate'.
    //  The optimum parameters are the ones which minimise the residual error between model and image measured in normalised texture frame (i.e. shape free images).
    // This function also returns the value of the corresponding residual error in 'diff'.

    bool Design(const AAMAppearanceModelC &apm, const DListC<StringC> &fileList, const StringC &dir, const StringC &mirrorFile, UIntT incrSize,bool ignoreSuspect = true)
    { return Body().Design(apm,fileList,dir,mirrorFile,incrSize,ignoreSuspect); }
    //: Train the AAM given some data and an existing appearance model.
    //!param: apm        - input appearance model.
    //!param: fileList   - list of file names for training.
    //!param: dir        - name of directory containing training images.
    //!param: mirrorFile - name of mirror file to use for mirror appearances (if an empty string is provided, mirror images will not be considered).
    //!param: incrSize   - half number of displacements for each parameter when perturbing the model.

    // This functions considers each training appearance successively and perturbs the different parameters in order to analyse the effect of errors in the parameters on the residual error. The range of displacement is +/-0.5 standard deviation for each parameter. A value of n for 'incrSize' means that there will be 2n displacements uniformly spread on the interval [-0.5std,+0.5std] for each parameter.

    bool Design(const AAMAppearanceModelC &apm, SampleStreamC<AAMAppearanceC> &sample, UIntT incrSize)
    { return Body().Design(apm, sample, incrSize); }
    //: Train the AAM given some data and an existing appearance model.
    //!param: apm        - input appearance model.
    //!param: sample     - list of sample appearances.
    //!param: incrSize   - half number of displacements for each parameter when perturbing the model.

    // This functions considers each training appearance successively and perturbs the different parameters in order to analyse the effect of errors in the parameters on the residual error. The range of displacement is +/-0.5 standard deviation for each parameter. A value of n for 'incrSize' means that there will be 2n displacements uniformly spread on the interval [-0.5std,+0.5std] for each parameter.

    bool PreDesign(const AAMAppearanceModelC &apm,
                   const DListC<StringC> &fileList,
                   const StringC &dir,
                   const StringC &mirrorFile,
                   UIntT incrSize,
                   const StringC &op,
                   bool ignoreSuspect = true
                   )
    { return Body().PreDesign(apm,fileList,dir,mirrorFile,incrSize,op,ignoreSuspect); }
    //: Pre-process some data before final design of the AAM.
    //!param: apm        - input appearance model.
    //!param: fileList   - list of file names for training.
    //!param: dir        - name of directory containing training images.
    //!param: mirrorFile - name of mirror file to use for mirror appearances (if an empty string is provided, mirror images will not be considered).
    //!param: incrSize   - half number of displacements for each parameter when perturbing the model.
    //!param: op         - name of output file containing results of training for this list of file.
    //!param: ignoreSuspect - Ignore suspect markups ?

    // This functions considers each training appearance successively and perturbs the different parameters in order to analyse the effect of errors in the parameters on the residual error. The range of displacement is +/-0.5 standard deviation for each parameter. A value of n for 'incrSize' means that there will be 2n displacements uniformly spread on the interval [-0.5std,+0.5std] for each parameter.
    // Note: contrary to the Design method, PreDesign does not produce a complete appearance model. PreDesign needs to be followed by PostDesign in order to obtain the AAM. PreDesign allows to split the training of the AAM (which is usually computer intensive because of the number of files to process) into a large number of smaller jobs which can be run in parallel.

    bool PreDesign(const AAMAppearanceModelC &apm,
                   SampleStreamC<AAMAppearanceC> &sample,
                   UIntT incrSize,
                   const StringC &op
                   );
    //: Pre-process some data before final design of the AAM.
    //!param: apm        - input appearance model.
    //!param: sample     - list of sample appearances.
    //!param: incrSize   - half number of displacements for each parameter when perturbing the model.
    //!param: op         - name of output file containing results of training for this list of file.

    // This functions considers each training appearance successively and perturbs the different parameters in order to analyse the effect of errors in the parameters on the residual error. The range of displacement is +/-0.5 standard deviation for each parameter. A value of n for 'incrSize' means that there will be 2n displacements uniformly spread on the interval [-0.5std,+0.5std] for each parameter.
    // Note: contrary to the Design method, PreDesign does not produce a complete appearance model. PreDesign needs to be followed by PostDesign in order to obtain the AAM. PreDesign allows to split the training of the AAM (which is usually computer intensive because of the number of files to process) into a large number of smaller jobs which can be run in parallel.

    bool PostDesign(const AAMAppearanceModelC &apm,const DListC<StringC> &fileList,const StringC &dir)
    { return Body().PostDesign(apm,fileList,dir); }
    //: Terminate the design of AAM by merging results of PreDesign.
    //!param: apm        - input appearance model.
    //!param: fileList   - list of names of files containing the results of the pre-design.
    //!param: dir        - name of directory containg results of the pre-design.

    const AAMAppearanceModelC &AppearanceModel() const
    { return Body().AppearanceModel(); }
    //: Access appearance model.

    AAMAppearanceModelC &AppearanceModel()
    { return Body().AppearanceModel(); }
    //: Access appearance model.

    const AAMShapeModelC &ShapeModel() const
    { return Body().ShapeModel(); }
    //: Access shape model.

    const SArray1dC<Point2dC> &RefrenceHome() const
    { return Body().RefrenceHome(); }
    //: Access home position of reference points (i.e. position of the control points in mean shape).

    SArray1dC<Point2dC> &RefrenceHome()
    { return Body().RefrenceHome(); }
    //: Access home position of reference points (i.e. position of the control points in mean shape).

    VectorC ShapeParameters(const VectorC &aamParams) const
    { return Body().ShapeParameters(aamParams); }
    //: Extract shape parameters from the appearance represented by the parameter vector 'aamParams'.

    UIntT Dimensions() const
    { return Body().Dimensions(); }
    //: Size of the parameter vector.

    friend class AAMActiveAppearanceModelBodyC;
  };

  inline
  BinOStreamC &operator<<(BinOStreamC &s,const AAMActiveAppearanceModelC &aam) {
    aam.Save(s);
    return s;
  }
  //: Save active appearance model to binary stream.

  inline
  BinIStreamC &operator>>(BinIStreamC &s,AAMActiveAppearanceModelC &aam) {
    aam = AAMActiveAppearanceModelC(s);
    return s;
  }
  //: Read active appearance model from binary stream.

}


#endif
