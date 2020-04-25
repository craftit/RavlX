// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_APPEARANCEMODEL_HEADER
#define RAVLIMAGE_APPEARANCEMODEL_HEADER 1
//! rcsid="$Id$"
//! lib=RavlAAM
//! file="Ravl/CompVision/ActiveAppearanceModels/AAMAppearanceModel.hh"
//! docentry="Ravl.API.Images.AAM"
//! author="Charles Galambos"
//! example = "aamBuildAppearanceModel.cc;aamViewShapeModel.cc"

#include "Ravl/DList.hh"
#include "Ravl/String.hh"
#include "Ravl/Array1d.hh"
#include "Ravl/Point2d.hh"
#include "Ravl/TriMesh2d.hh"
#include "Ravl/Image/AAMAppearance.hh"
#include "Ravl/Image/AAMShapeModel.hh"
#include "Ravl/Image/WarpMesh2d.hh"
#include "Ravl/Image/GaussConvolve2d.hh"

namespace RavlImageN {

  //! userlevel=Develop
  //: Statistical model of appearance.

  class AAMAppearanceModelBodyC
    : public RCBodyVC
  {
  public:
    AAMAppearanceModelBodyC(RealT nWarpSigma = 2,bool fixTextureMeanStdDev = true,bool equaliseTextureResolution = false);
    //: Constructor.
    //!param: nWarpSigma - stiffness to use in warping process.
    // Note: this parameter is relevant only in the case where thin-plate splines are used for warping.
    // In the case of a piece-wise affine warping, this parameter is ignored.
    // The warping technique to be used is defined at compilation time by the parameter RAVLAAM_USE_THINPLATEWARP
    // in the file "AAMAppearanceModel.cc".

    AAMAppearanceModelBodyC(BinIStreamC &is);
    //: Load from binary stream.

    AAMAppearanceModelBodyC(istream &is);
    //: Load from stream.

    virtual bool Save(BinOStreamC &out) const;
    //: Save to binary stream 'out'.

    virtual bool Save(ostream &out) const;
    //: Save to stream 'out'.

    virtual VectorC Parameters(const AAMAppearanceC &inst) const;
    //: Return a parameter vector representing the appearance 'inst'.

    virtual VectorC RawParameters(const AAMAppearanceC &inst) const;
    //: Generate raw parameters.
    //  The raw parameters are the parameters representing the appearance before applying PCA. They consists of the pose parameters, which describe the pose of the model instance in the image, the grey-level scaling and offset, which define the normalisation transformation for pixel intensities, the shape parameters (coordinate of the control points in normalised frame) and the texture parameters (normalised pixel grey-levels).

    virtual ImageC<ByteT> WarpToMaskShape(const AAMAppearanceC &inst) const;
    //: Warp appearance to the mean shape
    //  The input appearance 'inst' is warped such that its control points are located at the mean positions in the returned image.

    virtual ImageC<RealT> WarpToMaskShape(const ImageC<RealT> &image, const SArray1dC<Point2dC> &points) const;
    //: Warp an image and set of points to the mean shape
    // Used by ErrorVector()

    virtual ImageC<ByteT> WarpFromMaskShape(const RealRange2dC &range, const ImageC<RealT> &image, const SArray1dC<Point2dC> &points) const;
    //: Warp an appearance from normalised mask to image
    // Used by Synthesize()

    virtual SArray1dC<Point2dC> MeanShapePoints() const
    { return shape.MeanPoints(); }
    //: Get the mean shape points
    // Function extracted so that it can be overridden/expanded

    virtual bool Design(const DListC<StringC> &fileList,const StringC &dir,const StringC &mirrorFile,
                        const Index2dC &maskSize,
                        const RealT varS = 0.95, RealT varT = 0.95, RealT varC = 0.95,
                        UIntT maxS=25, UIntT maxT=120, UIntT maxC=80,
                        bool ignoreSuspect = true);
    //: Design an appearance model given some data.
    //!param: filelist    - list of appearance file names.
    //!param: dir         - name of directory containing appearance files.
    //!param: mirrorfile  - name of file containing mirror information.
    //!param: maskSize    - dimensions of the shape free image, i.e. with control points warped to mean positions.
    //!param: varS        - percentage of shape variation preserved during PCA applied to shape
    //!param: varT        - percentage of texture variation preserved during PCA applied to grey-level values
    //!param: varC        - percentage of variation preserved during PCA applied to combined shape and texture vectors
    //!param: maxS        - limit on number of parameters returned by PCA applied to shape
    //!param: maxT        - limit on number of parameters returned by PCA applied to grey-level values
    //!param: maxC        - limit on number of parameters returned by PCA applied to combined shape and texture vectors
    //!param: ignoreSuspect - If true ignore examples flagged as suspect

    virtual bool Design(const AAMShapeModelC &shapeModel,
                        SampleStreamC<AAMAppearanceC> &sample,
                        const Index2dC &maskSize,
                        RealT varT = 0.95, RealT varC = 0.95,
                        UIntT maxT=120, UIntT maxC=80);
    //: Design an appearance model given some data.
    //!param: shapeModel  - a designed shape model.
    //!param: sample      - stream of sample appearances.
    //!param: maskSize    - dimensions of the shape free image, i.e. with control points warped to mean positions.
    //!param: varT        - percentage of texture variation preserved during PCA applied to grey-level values
    //!param: varC        - percentage of variation preserved during PCA applied to combined shape and texture vectors
    //!param: maxT        - limit on number of parameters returned by PCA applied to grey-level values
    //!param: maxC        - limit on number of parameters returned by PCA applied to combined shape and texture vectors

    virtual void SetMask(const ImageC<IntT> &mask);
    //: Set shape free image.
    //  This sets up the shape free image, its dimension and area.

    UIntT Dimensions() const
    { return appearanceModel.OutputSize() + NoFixedParameters(); }
    //: Size of the parameter vector.

    virtual AAMAppearanceC Synthesize(const VectorC &dat,RealT scale = 1) const;
    //: Synthesis an appearance from a parameter vector 'dat'.
    //  'scale' is used to modify the scale of the appearance.

    virtual bool TextureImage(const VectorC &dat,ImageC<RealT> &res) const;
    //: Compute texture image 'res' (shape free image) from a parameter vector 'dat'.

    bool ErrorVector(const VectorC &parm,const ImageC<ByteT> &img,VectorC &errImg) const;
    //: Conpute error vector.
    //!param: parm   - parameter vector representing appearance.
    //!param: img    - target image for comparison.
    //!param: errImg - vector of intensity differences.
    // The error values consists of the intensity difference between target image and the texture image synthesised from the parameters parm.
    // Note that the error is measured in the shape free images for each pixel in the mask area.

    virtual bool ErrorVector(const VectorC &parm,const ImageC<RealT> &img,VectorC &errImg) const;
    //: Conpute error vector.
    //!param: parm   - parameter vector representing appearance.
    //!param: img    - target image for comparison.
    //!param: errImg - vector of intensity differences.
    // The error values consists of the intensity difference between target image and the texture image synthesised from the parameters parm.
    // Note that the error is measured in the shape free images for each pixel in the mask area.

    void MakePlausible(VectorC &parm, RealT NbSigma = 3) const;
    //: Make 'parm' a plausible parameter vector.
    //  This imposes hard limits of +/-3 std to each parameter.

    const IndexRange2dC &MaskSize() const
    { return maskSize; }
    //: Get size of mask image.

    IntT MaskArea() const
    { return maskArea; }
    //: Get number of pixels in the mask.

    const ImageC<IntT> &Mask() const
    { return mask; }
    //: Get mask image.

    const VectorC &EigenValues() const
    { return eigenValues; }
    //: Access eigen values.

    const FunctionC &AppearanceModel() const
    { return appearanceModel; }
    //: Access appearance model.

    const FunctionC &InvAppearanceModel() const
    { return invAppearanceModel; }
    //: Access inverse appearance model.

    IntT NoFixedParameters() const
    { return shape.NoFixedParameters() + (m_fixTextureMeanStdDev ? 0 : 2); }
    //: Find the number of parameters representing the pose and intensity statistics
    //  e.g. position, orientation, scale, grey-level scaling and offset.

    const VectorC &FixedMean() const
    { return fixedMean; }
    //: Get mean pose and grey-level offset and scaling

    const VectorC &PixelSize() const
    { return pixelSize; }
    //: Get size of pixel in shape free image.

    VectorC ShapeParameters(const VectorC &aamParams) const;
    //: Extract shape parameters from the appearance represented by the parameter vector 'aamParams'.

    virtual bool TextureParameters(const ImageC<RealT> &im, VectorC &res) const;
    //: Extract texture parameters 'res' from shape free image 'im'.

    const AAMShapeModelC &ShapeModel() const
    { return shape; }
    //: Access shape model.

    AAMShapeModelC &ShapeModel()
    { return shape; }
    //: Access shape model.

    const TriMesh2dC &Mesh() const
    { return warp.Mesh(); }
    //: Access mesh.
    //  The mesh defines the triangles which are affinely warped by the piece-wise affine warping.

    TriMesh2dC &Mesh()
    { return warp.Mesh(); }
    //: Access mesh.
    //  The mesh defines the triangles which are affinely warped by the piece-wise affine warping.

    bool MeanTextureAppearanceParameters(const VectorC &sParam, VectorC &aParam) const;
    //: Compute appearance parameters 'aParam' corresponding to shape parameters 'sParam' and mean texture values.

    bool MeanTextureAppearanceParameters(const SArray1dC<Point2dC> &sPoints, VectorC &aParam) const;
    //: Compute appearance parameters 'aParam' corresponding to feature points 'sPoints' and mean texture values.

    bool FixTextureMeanStdDev() const
    { return m_fixTextureMeanStdDev; }
    //: Are we using fixed values for mean and std dev of the texture image.?
  protected:

    virtual bool DesignMesh(SampleStreamC<AAMAppearanceC> &sample);
    //: Design mesh to be used for the model

    RealT warpSigma; // Stiffness used for warping (only relevant if thin-plate splines are enabled).
    AAMShapeModelC shape; // Statistical shape model.
    ImageC<IntT> mask; // Mask defining which pixels are used to define the statistical model of appearance.
    SArray1dC<Point2dC> maskPoints; // Position of control points in shape free image.
    IndexRange2dC maskSize; // Dimensions of the mask (also the dimension of the shape free image).
    IntT maskArea;     // Number of pixels in the mask.
    FunctionC appearanceModel; // Appearance model, map raw parameters to appearance parameters.
    FunctionC invAppearanceModel; // Inverse appearance model, map appearance parameters to raw parameters.
    WarpMesh2dC<ByteT> warp; // Warping to the shape free image.
    VectorC eigenValues; // Eigen values.
    VectorC fixedMean; // Pose and grey level offset and mean parameters.
    VectorC pixelSize; // Pixel size in the shape free image.
    GaussConvolve2dC<ByteT> smooth; // Gauss convolver for smoothing images.
    bool m_fixTextureMeanStdDev; // Normalise the input's image mean and std deviation, and don't add them as parameters.
    bool m_equaliseTextureResoluition;
  };

  //! userlevel=Normal
  //: Statistical model of appearance.

  class AAMAppearanceModelC
    : public RCHandleVC<AAMAppearanceModelBodyC>
  {
  public:
    AAMAppearanceModelC()
    {}
    //: Default constructor
    // Creates an invalid handle.

    AAMAppearanceModelC(RealT nWarpSigma,bool useFixedTextureStats = true)
      : RCHandleVC<AAMAppearanceModelBodyC>(*new AAMAppearanceModelBodyC(nWarpSigma,useFixedTextureStats))
    {}
    //: Constructor.
    //!param: nWarpSigma - stiffness to use in warping process.
    // Note: this parameter is relevant only in the case where thin-plate splines are used for warping.
    // In the case of a piece-wise affine warping, this parameter is ignored.
    // The warping technique to be used is defined at compilation time by the parameter RAVLAAM_USE_THINPLATEWARP
    // in the file "AAMAppearanceModel.cc".

    AAMAppearanceModelC(BinIStreamC &is);
    //: Load from binary stream.

    AAMAppearanceModelC(istream &is);
    //: Load from stream.


  protected:
    AAMAppearanceModelC(AAMAppearanceModelBodyC &bod)
      : RCHandleVC<AAMAppearanceModelBodyC>(bod)
    {}
    //: Body constructor.

    AAMAppearanceModelC(AAMAppearanceModelBodyC *bod)
      : RCHandleVC<AAMAppearanceModelBodyC>(*bod)
    {}
    //: Body ptr constructor.

    AAMAppearanceModelBodyC &Body()
    { return RCHandleVC<AAMAppearanceModelBodyC>::Body(); }
    //: Access body.

    const AAMAppearanceModelBodyC &Body() const
    { return RCHandleVC<AAMAppearanceModelBodyC>::Body(); }
    //: Access body.


  public:

    VectorC Parameters(const AAMAppearanceC &inst) const
    { return Body().Parameters(inst); }
    //: Return a parameter vector representing the appearance 'inst'.

    VectorC RawParameters(const AAMAppearanceC &inst) const
    { return Body().RawParameters(inst); }
    //: Generate raw parameters.
    //  The raw parameters are the parameters representing the appearance before applying PCA. They consists of the pose parameters, which describe the pose of the model instance in the image, the grey-level scaling and offset, which define the normalisation transformation for pixel intensities, the shape parameters (coordinate of the control points in normalised frame) and the texture parameters (normalised pixel grey-levels).

    ImageC<ByteT> WarpToMaskShape(const AAMAppearanceC &inst) const
    { return Body().WarpToMaskShape(inst); }
    //: Warp appearance to the mean shape
    //  The input appearance 'inst' is warped such that its control points are located at the mean positions in the returned image.

    bool Design(const DListC<StringC> &fileList,const StringC &dir,const StringC &mirrorFile,
                const Index2dC &newMaskSize = Index2dC(50,50),
                RealT varS = 0.95, RealT varT = 0.95, RealT varC = 0.95,
                UIntT maxS=25, UIntT maxT=120, UIntT maxC=80,
                bool ignoreSuspect = true)
    { return Body().Design(fileList,dir,mirrorFile,newMaskSize,varS,varT,varC,maxS,maxT,maxC,ignoreSuspect); }
    //: Design an appearance model given some data.
    //!param: filelist    - list of appearance file names.
    //!param: dir         - name of directory containing appearance files.
    //!param: mirrorfile  - name of file containing mirror information.
    //!param: maskSize    - dimensions of the shape free image, i.e. with control points warped to mean positions.
    //!param: varS        - percentage of shape variation preserved during PCA applied to shape
    //!param: varT        - percentage of texture variation preserved during PCA applied to grey-level values
    //!param: varC        - percentage of variation preserved during PCA applied to combined shape and texture vectors
    //!param: maxS        - limit on number of parameters returned by PCA applied to shape
    //!param: maxT        - limit on number of parameters returned by PCA applied to grey-level values
    //!param: maxC        - limit on number of parameters returned by PCA applied to combined shape and texture vectors
    //!param: ignoreSuspect - If true ignore examples flagged as suspect

    bool Design(const AAMShapeModelC &shapeModel,
                SampleStreamC<AAMAppearanceC> &sample,
                const Index2dC &maskSize,
                RealT varT = 0.95,
                RealT varC = 0.95,
                UIntT maxT = 120,
                UIntT maxC = 80)
    { return Body().Design(shapeModel, sample, maskSize, varT, varC, maxT, maxC); }
    //: Design an appearance model given some data.
    //!param: shapeModel  - a designed shape model.
    //!param: sample      - stream of sample appearances.
    //!param: maskSize    - dimensions of the shape free image, i.e. with control points warped to mean positions.
    //!param: varT        - percentage of texture variation preserved during PCA applied to grey-level values
    //!param: varC        - percentage of variation preserved during PCA applied to combined shape and texture vectors
    //!param: maxT        - limit on number of parameters returned by PCA applied to grey-level values
    //!param: maxC        - limit on number of parameters returned by PCA applied to combined shape and texture vectors

    void SetMask(const ImageC<IntT> &mask)
    { return Body().SetMask(mask); }
    //: Set shape free image.
    //  This sets up the shape free image, its dimension and area.

    UIntT Dimensions() const
    { return Body().Dimensions(); }
    //: Size of the parameter vector.

    AAMAppearanceC Synthesize(const VectorC &dat,RealT scale = 1) const
    { return Body().Synthesize(dat,scale); }
    //: Synthesis an appearance from a parameter vector 'dat'.
    //  'scale' is used to modify the scale of the appearance.

    bool TextureImage(const VectorC &dat,ImageC<RealT> &res) const
    { return Body().TextureImage(dat,res);}
    //: Compute texture image (shape free image).

    bool ErrorVector(const VectorC &parm,const ImageC<ByteT> &img,VectorC &errImg) const
    { return Body().ErrorVector(parm,img,errImg); }
    //: Conpute error vector.
    //!param: parm   - parameter vector representing appearance.
    //!param: img    - target image for comparison.
    //!param: errImg - vector of intensity differences.
    // The error values consists of the intensity difference between target image and the texture image synthesized from the parameters parm.
    // Note that the error is measured in the shape free images for each pixel in the mask area.

    bool ErrorVector(const VectorC &parm,const ImageC<RealT> &img,VectorC &errImg) const
    { return Body().ErrorVector(parm,img,errImg); }
    //: Compute error vector.
    //!param: parm   - parameter vector representing appearance.
    //!param: img    - target image for comparison.
    //!param: errImg - vector of intensity differences.
    // The error values consists of the intensity difference between target image and the texture image synthesized from the parameters parm.
    // Note that the error is measured in the shape free images for each pixel in the mask area.

    void MakePlausible(VectorC &parm, RealT NbSigma = 3) const
    { return Body().MakePlausible(parm,NbSigma); }
    //: Make 'parm' a plausible parameter vector.
    //  This imposes hard limits of +/-3 std to each parameter.

    const IndexRange2dC &MaskSize() const
    { return Body().MaskSize(); }
    //: Get size of mask image.

    IntT MaskArea() const
    { return Body().MaskArea(); }
    //: Get number of points in mask.

    const ImageC<IntT> &Mask() const
    { return Body().Mask(); }
    //: Get mask image.

    const VectorC &EigenValues() const
    { return Body().EigenValues(); }
    //: Access eigen values.

    const FunctionC &AppearanceModel() const
    { return Body().AppearanceModel(); }
    //: Access appearance model.

    const FunctionC &InvAppearanceModel() const
    { return Body().InvAppearanceModel(); }
    //: Access inverse appearance model.

    IntT NoFixedParameters() const
    { return Body().NoFixedParameters(); }
    //: Find the number of parameters which have fixed meaning.
    // offset,scale for example.

    const VectorC &FixedMean() const
    { return Body().FixedMean(); }
    //: Get mean pose and grey-level offset and scaling

    const VectorC &PixelSize() const
    { return Body().PixelSize(); }
    //: Get size of pixel in shape free image.

    VectorC ShapeParameters(const VectorC &aamParams) const
    { return Body().ShapeParameters(aamParams); }
    //: Extract shape parameters from the appearance represented by the parameter vector 'aamParams'.

    bool TextureParameters(const ImageC<RealT> &im, VectorC &res) const
    { return Body().TextureParameters(im,res); }
    //: Extract texture parameters 'res' from shape free image 'im'.

    const AAMShapeModelC &ShapeModel() const
    { return Body().ShapeModel(); }
    //: Access shape model.

    bool MeanTextureAppearanceParameters(const VectorC &sParam, VectorC &aParam) const
    { return Body().MeanTextureAppearanceParameters(sParam, aParam);}
    //: Compute appearance parameters 'aParam' corresponding to shape parameters 'sParam' and mean texture values.

    bool MeanTextureAppearanceParameters(const SArray1dC<Point2dC> &sPoints, VectorC &aParam) const
    { return Body().MeanTextureAppearanceParameters(sPoints, aParam);}
    //: Compute appearance parameters 'aParam' corresponding to feature points 'sPoints' and mean texture values.

    bool FixTextureMeanStdDev() const
    { return Body().FixTextureMeanStdDev(); }
    //: Are we using fixed values for mean and std dev of the texture image.?

  };

  inline
  BinOStreamC &operator<<(BinOStreamC &s,const AAMAppearanceModelC &ap) {
    ap.Save(s);
    return s;
  }
  //: Save appearance model to binary stream.

  inline
  BinIStreamC &operator>>(BinIStreamC &s,AAMAppearanceModelC &ap) {
    ap = AAMAppearanceModelC(s);
    return s;
  }
  //: Read appearance model from binary stream.

}


#endif
