// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_SHAPEMODEL_HEADER
#define RAVLIMAGE_SHAPEMODEL_HEADER 1
//! rcsid="$Id$"
//! lib=RavlAAM
//! file="Ravl/CompVision/ActiveAppearanceModels/AAMShapeModel.hh"
//! docentry="Ravl.API.Images.AAM"
//! userlevel="Develop"
//! author="Charles Galambos"
//! example = "aamBuildShapeModel.cc;aamViewShapeModel.cc"

#include "Ravl/Image/AAMAppearance.hh"
#include "Ravl/PatternRec/Function.hh"
#include "Ravl/PatternRec/SampleStream.hh"
#include "Ravl/Vector2d.hh"

namespace RavlImageN {

  //! userlevel=Develop
  //: Statistical shape model.

  class AAMShapeModelBodyC
    : public RCBodyVC
  {
  public:
    AAMShapeModelBodyC()
      : m_verbose(false), // Write error messages to standard out.
        nPoints(0) // Number of control points in the model.
    {}
    //: Default constructor.

    AAMShapeModelBodyC(BinIStreamC &is);
    //: Constructor.
    //  Load from bin stream.

    AAMShapeModelBodyC(istream &is);
    //: Constructor.
    //  Load from stream.

    void SetVerbose(bool verboseMode)
    { m_verbose = verboseMode; }
    //: If set to true designer will print error messages to std::cerr

    virtual bool Save(BinOStreamC &out) const;
    //: Save to binary stream 'out'.

    virtual bool Save(ostream &out) const;
    //: Save to stream 'out'.

    virtual bool ComputeMean(const SampleC<AAMAppearanceC> &sample);
    //: Compute mean control points for the list of appearance provided.

    virtual bool ComputeMean(SampleStreamC<AAMAppearanceC> &sample);
    //: Compute mean control points for the list of provided appearances

    virtual bool RawParameters(const AAMAppearanceC &inst,VectorC &fixedParams,VectorC &freeParams) const;
    //: Generate raw parameters.
    //!param: inst        - input appearance for which we would like to compute the parameters.
    //!param: fixedParams - output pose parameters (e.g. pose, scale, orientation).
    //!param: freeParams  - output normalised control point coordinates. This vector consists of the concatenation of the X and Y coordinates of all control points in a normalised frame.
    //  The raw parameters are the parameters representing the shape before applying PCA. They consists of the pose parameters, which describe the pose of the model instance in the image, and the shape parameters, which describe its shape.

    virtual void RawProject(const VectorC &fixedParams,const VectorC &freeParams,SArray1dC<Point2dC> &out) const;
    //: Generate control points defining an appearance from the raw parameters.
    //!param: fixedParams - input pose parameters (e.g. pose, scale, orientation).
    //!param: freeParams  - input normalised control point coordinates. This vector consists of the concatenation of the X and Y coordinates of all control points in a normalised frame.
    //!param: out         - ouput control points

    virtual IntT NoFixedParameters() const;
    //: Return number of parameters describing the pose
    //  These parameters include e.g. the position, scale and orientation of the model instance

    VectorC Parameters(const AAMAppearanceC &inst) const;
    //: Return a parameter vector representing the appearance 'inst'.

    bool Design(const SampleC<AAMAppearanceC> &sample,RealT variation = 0.95,UIntT maxP=50);
    //: Design a shape model given some data.
    //!param: sample    - list of appearances for training the model.
    //!param: variation - percentage of variation preserved during PCA.
    //!param: maxP      - limit on number of parameters contained in the shape model.

    bool Design(SampleStreamC<AAMAppearanceC> &sample,RealT variation = 0.95,UIntT maxP=50);
    //: Design a shape model given some data.
    //!param: sample    - list of appearances for training the model.
    //!param: variation - percentage of variation preserved during PCA.
    //!param: maxP      - limit on number of parameters contained in the shape model.

    UIntT Dimensions() const
    { return shapeModel.OutputSize() + NoFixedParameters(); }
    //: Size of the parameter vector.

    const SArray1dC<Point2dC> MeanPoints() const
    { return meanPoints.Copy(); }
    //: Return mean control point positions (in normalised frame) for shape model.

    void TransformMeanPoints(const Point2dC &offset, const Vector2dC &scale);
    //: Transform mean points by shifting by offset and scaling

    SArray1dC<Point2dC> Synthesize(const VectorC &parm) const;
    //: Synthesis a control point set from a parameter vector.

    const VectorC &EigenValues() const
    { return eigenValues; }
    //: Access eigen values.

    const VectorC &FixedMean() const
    { return fixedMean; }
    //: Return mean pose parameters for the shape model.

    void MakePlausible(VectorC &parm, RealT NbSigma = 3) const;
    //: Make 'parm' a plausible parameter vector.
    //  This imposes hard limits of +/-3 std to each parameter.

    bool P2PError(const VectorC &parm,const SArray1dC<Point2dC> &points,VectorC &errVec) const;
    //: Return vector of point to point errors between shape represented by vector 'parm' and target shape defined by 'points'.

  protected:
    bool m_verbose; // Write error messages to standard out.

    UIntT nPoints; // Number of control points in the model.

    FunctionC shapeModel;   // Shape model, map control point location to parameters.
    FunctionC invShapeModel;// Inverse shape model, map parameters to control point location

    SArray1dC<Point2dC> meanPoints; // Mean control point positions (in normalised frame).
    VectorC fixedMean; // Mean pose.

    VectorC eigenValues;   // eigenValues from PCA.
  };

  //! userlevel=Normal
  //: Statistical shape model.

  class AAMShapeModelC
    : public RCHandleVC<AAMShapeModelBodyC>
  {
  public:
    AAMShapeModelC()
    {}
    //: Default constructor.
    // Creates an invalid handle.

    AAMShapeModelC(BinIStreamC &is);
    //: Binary stream constructor.

    AAMShapeModelC(istream &is);
    //: Stream constructor.

    AAMShapeModelC(bool)
      : RCHandleVC<AAMShapeModelBodyC>(*new AAMShapeModelBodyC())
    {}
    //: Construct a shape model.

  protected:
    AAMShapeModelC(AAMShapeModelBodyC &bod)
      : RCHandleVC<AAMShapeModelBodyC>(bod)
    {}
    //: Body constructor.

    AAMShapeModelC(AAMShapeModelBodyC *bod)
      : RCHandleVC<AAMShapeModelBodyC>(*bod)
    {}
    //: Body ptr constructor.

    AAMShapeModelBodyC &Body()
    { return RCHandleVC<AAMShapeModelBodyC>::Body(); }
    //: Access body.

    const AAMShapeModelBodyC &Body() const
    { return RCHandleVC<AAMShapeModelBodyC>::Body(); }
    //: Access body.

  public:

    void SetVerbose(bool verboseMode)
    { Body().SetVerbose(verboseMode); }
    //: If set to true designer will print error messages to std::cerr

    bool RawParameters(const AAMAppearanceC &inst,VectorC &fixedParams,VectorC &freeParams) const
    { return Body().RawParameters(inst,fixedParams,freeParams); }
    //: Generate raw parameters.
    //!param: inst        - input appearance for which we would like to compute the parameters.
    //!param: fixedParams - output pose parameters (e.g. pose, scale, orientation).
    //!param: freeParams  - output normalised control point coordinates. This vector consists of the concatenation of the X and Y coordinates of all control points in a normalised frame.
    //  The raw parameters are the parameters representing the shape before compression using PCA. They consists of the pose parameters, which describe the pose of the model instance in the image, and the shape parameters, which describe its shape.

    VectorC Parameters(const AAMAppearanceC &inst) const
    { return Body().Parameters(inst); }
    //: Return a parameter vector representing the appearance 'inst'.

    bool Design(const SampleC<AAMAppearanceC> &sample,RealT variation = 0.95,UIntT maxP=25)
    { return Body().Design(sample,variation,maxP); }
    //: Design a shape model given some data.
    //!param: sample    - list of appearances for training the model.
    //!param: variation - percentage of variation preserved during PCA.
    //!param: maxP      - limit on number of parameters contained in the shape model.

    bool Design(SampleStreamC<AAMAppearanceC> &sample,RealT variation = 0.95,UIntT maxP=25)
    { return Body().Design(sample,variation,maxP); }
    //: Design a shape model given some data.
    //!param: sample    - list of appearances for training the model.
    //!param: variation - percentage of variation preserved during PCA.
    //!param: maxP      - limit on number of parameters contained in the shape model.

    UIntT Dimensions() const
    { return Body().Dimensions(); }
    //: Size of the parameter vector.

    SArray1dC<Point2dC> MeanPoints() const
    { return Body().MeanPoints(); }
    //: Return mean control point positions (in normalised frame) for shape model.

    void TransformMeanPoints(const Point2dC &offset, const Vector2dC &scale)
    { Body().TransformMeanPoints(offset, scale); }
    //: Transform mean points by shifting by offset and scaling

    SArray1dC<Point2dC> Synthesize(const VectorC &parm) const
    { return Body().Synthesize(parm); }
    //: Synthesis a control point set from a parameter vector.

    const VectorC &EigenValues() const
    { return Body().EigenValues(); }
    //: Access eigen values.

    IntT NoFixedParameters() const
    { return Body().NoFixedParameters(); }
    //: Return number of parameters describing the pose
    //  These parameters include e.g. the position, scale and orientation of the model instance

    const VectorC &FixedMean() const
    { return Body().FixedMean(); }
    //: Return mean pose parameters for the shape model.

    void MakePlausible(VectorC &parm, RealT NbSigma = 3) const
    { return Body().MakePlausible(parm,NbSigma); }
    //: Make 'parm' a plausible parameter vector.
    //  This imposes hard limits of +/-3 std to each parameter.

    bool P2PError(const VectorC &parm,const SArray1dC<Point2dC> &points,VectorC &errVec) const
    { return Body().P2PError(parm, points, errVec);}
    //: Return vector of point to point errors between shape represented by vector 'parm' and target shape defined by 'points'.
    //: Returns point to point error between shape modelled by vector parm and target shape defined by points
  };

  inline
  BinOStreamC &operator<<(BinOStreamC &s,const AAMShapeModelC &ap) {
    ap.Save(s);
    return s;
  }
  //: Save shape model to binary stream.

  inline
  BinIStreamC &operator>>(BinIStreamC &s,AAMShapeModelC &ap) {
    ap = AAMShapeModelC(s);
    return s;
  }
  //: Read shape model from binary stream.

}


#endif
