// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_AAMPOSEESTIMATE_HEADER
#define RAVLIMAGE_AAMPOSEESTIMATE_HEADER 1
//////////////////////////////////////////////////////////////////////////////
//! author =    "Jean-Yves Guillemaut"
//! lib=RavlAAM
//! date =      "01/03/2006"
//! rcsid="$Id: AAMPoseEstimate.hh,v 1.3 2005/07/25 16:31:07 ees1wc Exp $"
//! userlevel = Normal
//! file="Ravl/CompVision/ActiveAppearanceModels/AAMPoseEstimate.hh"
//! docentry =  "Ravl.API.Images.AAM"
//! example = aamBuildPoseEstimator.cc

#include "Ravl/RefCounter.hh"
#include "Ravl/RCHandleV.hh"
#include "Ravl/PatternRec/SampleIter.hh"
#include "Ravl/DP/FileFormatBinStream.hh"
#include "Ravl/DP/FileFormatStream.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/LeastSquares.hh"
#include "Ravl/Matrix.hh"

namespace RavlImageN {
  using namespace RavlN;

  //! userlevel=Develop
  //: Statistical model of pose variation.
  //  Let us consider a parameter vector 'C' defining an appearance with pose parametrised by the pan angle 'theta' and the tilt angle 'phi'. The parameter vector 'C' and the pose values ('theta', 'phi') are related by:
  //  <p><i>C = C0 + CT cos(theta) + ST sin(theta) + CP cos(phi) + SP sin(phi) + CTCP cos(theta) cos(phi) + CTSP cos(theta) sin(phi) + STCP sin(theta) cos(phi) + STSP sin(theta) sin(phi) + R</i>,</p>
  //  where 'C0', 'CT', 'ST', 'CP', 'SP', 'CTCP', 'CTSP', 'STCP', 'STSP' are vectors defining the statitical model of pose variation and 'R' is the residual vector carrying the identity of the person.

  class AAMPoseEstimateBodyC
  : public RCBodyVC
  {
  public:
    AAMPoseEstimateBodyC();
    //: Default constructor.

    AAMPoseEstimateBodyC(BinIStreamC &is);
    //: Load from bin stream.

    AAMPoseEstimateBodyC(istream &is);
    //: Load from stream.

    virtual bool Save(BinOStreamC &out) const;
    //: Save to binary stream 'out'.

    virtual bool Save(ostream &out) const;
    //: Save to stream 'out'.

    bool Design(const SampleC<VectorC> &samp, const SampleC<RealT> &pan, const SampleC<RealT> &tilt);
    //: Design the statistical model of pose variation.
    //!param: samp - List of parameter vectors representing the appearances used for training.
    //!param: pan - List of ground truth pan angle values (in radians) for the given appearances.
    //!param: tilt - List of ground truth tilt angle values (in radians) for the given appearances.
    // A pose is parametrised by pan and tilt angles (both measured in radians).
    // The model assumes that each feature point moves on an sphere (i.e. pure pan and tilt rotation motion) when the head is turned.
    // This function learns the relation between a given parameter vector and a pose from the training data.
    // The parameter vector can be either the shape parameter vector or the appearance parameter vetor.
    // In the latter case, only the shape is used to infer the pose; in the former case grey level information is also used to infer the pose.

    PairC<RealT> EstimatePose(const VectorC &C) const;
    //: Estimate the pose of the appearance represented by parameter vector 'C'.
    //  The method first computes a least-squares solution based on the pseudo-inverse and then refines the solution by non-linear minimisation.

    bool EstimatePose(const VectorC &C, PairC<RealT> &angles) const;
    //: Estimate the pose of the appearance represented by parameter vector 'C'.
    //  The method first computes a least-squares solution based on the pseudo-inverse and then refines the solution by non-linear minimisation.

    bool GenerateProfile(const VectorC &C, StringC fileName) const;
    //: Outputs the profile of the cost function.
    //!param: C - Parameter vector representing the appearance for which we would like to evaluate the pose.
    //!param: fileName - Name of the output file for profile.
    // This method is useful only for testing purpose, in particular to check the convergence properties of the pose estimator.
    // Each line of the output file contains the following information: 'theta' 'pan' 'residual', for all possible pose in the range +/-45 degrees sampled evry degree.

    VectorC Rotate(const VectorC &C, const RealT &pan, const RealT &tilt) const;
    //: Returns new parameter vector representing appearance after change of pose.
    //!param: C - Parameter vector representing the original appearance.
    //!param: pan - Pan angle after change of pose.
    //!param: tilt - Tilt angle after change of pose.
    //  This function estimates the pose of the initial appearance and predicts the corresponding parameter vector in the novel pose.

    VectorC Rotate(const VectorC &C, const RealT &pani, const RealT &tilti, const RealT &panf, const RealT &tiltf) const;
    //: Returns new parameter vector representing appearance after change of pose given prior information about the initial pose.
    //!param: C - Parameter vector representing the original appearance.
    //!param: pani - Pan angle before change of pose.
    //!param: tilti - Tilt angle before change of pose.
    //!param: panf - Pan angle after change of pose.
    //!param: tiltf - Tilt angle after change of pose.
    //  This function predicts the parameter vector of an appearance with known initial pose after change of pose.

    VectorC RotateToFrontal(const VectorC &C) const {
      return Rotate(C, 0.0, 0.0);
    }
    //: Returns new parameter vector representing appearance in frontal pose.
    //!param: C - Parameter vector representing the original appearance.
    //  This function estimates the pose of the initial appearance and predicts the corresponding parameter vector in frontal pose.

    VectorC RotateToFrontal(const VectorC &C, const RealT &pani, const RealT &tilti) const {
      return Rotate(C, pani, tilti, 0.0, 0.0);
    }
    //: Returns new parameter vector representing appearance in frontal pose given prior information about the initial pose.
    //!param: C - Parameter vector representing the original appearance.
    //!param: pani - Pan angle before change of pose.
    //!param: tilti - Tilt angle before change of pose.
    //  This function predicts the frontal pose parameter vector of an appearance with known initial pose.

  protected:
    VectorC C0;
    VectorC CT;
    VectorC ST;
    VectorC CP;
    VectorC SP;
    VectorC CTCP;
    VectorC CTSP;
    VectorC STCP;
    VectorC STSP;
    VectorC R;

  };

  //! userlevel=Normal
  //: Statistical model of pose variation.
  //  Let us consider a parameter vector 'C' defining an appearance with pose parametrised by the pan angle 'theta' and the tilt angle 'phi'. The parameter vector 'C' and the pose values ('theta', 'phi') are related by:
  //  <p><i>C = C0 + CT cos(theta) + ST sin(theta) + CP cos(phi) + SP sin(phi) + CTCP cos(theta) cos(phi) + CTSP cos(theta) sin(phi) + STCP sin(theta) cos(phi) + STSP sin(theta) sin(phi) + R</i>,</p>
  //  where 'C0', 'CT', 'ST', 'CP', 'SP', 'CTCP', 'CTSP', 'STCP', 'STSP' are vectors defining the statitical model of pose variation and 'R' is the residual vector carrying the identity of the person.

  class AAMPoseEstimateC
      : public RCHandleVC<AAMPoseEstimateBodyC>
  {
  public:
    AAMPoseEstimateC()
      : RCHandleVC<AAMPoseEstimateBodyC> (*new AAMPoseEstimateBodyC() )
    {}
    //: Default constructor.

    AAMPoseEstimateC(BinIStreamC &is);
    //: Binary stream constructor.

    AAMPoseEstimateC(istream &is);
    //: Stream constructor.

  protected:
    AAMPoseEstimateC(AAMPoseEstimateBodyC &bod)
      : RCHandleVC<AAMPoseEstimateBodyC>(bod)
    {}
    //: Body constructor.

    AAMPoseEstimateC(AAMPoseEstimateBodyC *bod)
      : RCHandleVC<AAMPoseEstimateBodyC>(*bod)
    {}
    //: Body ptr constructor.

    AAMPoseEstimateBodyC &Body()
    { return RCHandleVC<AAMPoseEstimateBodyC>::Body(); }
    //: Access body.

    const AAMPoseEstimateBodyC &Body() const
    { return RCHandleVC<AAMPoseEstimateBodyC>::Body(); }
    //: Access body.

  public:
   bool Design(const SampleC<VectorC> &samp, const SampleC<RealT> &pan, const SampleC<RealT> &tilt)
     { return Body().Design(samp, pan, tilt); }
    //: Design the statistical model of pose variation.
    //!param: samp - List of parameter vectors representing the appearances used for training.
    //!param: pan - List of ground truth pan angle values (in radians) for the given appearances.
    //!param: tilt - List of ground truth tilt angle values (in radians) for the given appearances.
    // A pose is parametrised by pan and tilt angles (both measured in radians).
    // The model assumes that each feature point moves on an sphere (i.e. pure pan and tilt rotation motion) when the head is turned.
    // This function learns the relation between a given parameter vector and a pose from the training data.
    // The parameter vector can be either the shape parameter vector or the appearance parameter vetor.
    // In the latter case, only the shape is used to infer the pose; in the former case grey level information is also used to infer the pose.

   PairC<RealT> EstimatePose(const VectorC &C) const
     { return Body().EstimatePose(C); }
    //: Estimate the pose of the appearance represented by parameter vector 'C'.
    //  The method first computes a least-squares solution based on the pseudo-inverse and then refines the solution by non-linear minimisation.

    bool EstimatePose(const VectorC &C, PairC<RealT> &angles) const
    { return Body().EstimatePose(C, angles); }
    //: Estimate the pose of the appearance represented by parameter vector 'C'.
    //  The method first computes a least-squares solution based on the pseudo-inverse and then refines the solution by non-linear minimisation.

   bool GenerateProfile(const VectorC &C, StringC fileName) const
     { return Body().GenerateProfile(C, fileName); }
    //: Outputs the profile of the cost function.
    //!param: C - Parameter vector representing the appearance for which we would like to evaluate the pose.
    //!param: fileName - Name of the output file for profile.
    // This method is useful only for testing purpose, in particular to check the convergence properties of the pose estimator.
    // Each line of the output file contains the following information: 'theta' 'pan' 'residual', for all possible pose in the range +/-45 degrees sampled evry degree.

   VectorC Rotate(const VectorC &C, const RealT &pan, const RealT &tilt) const
     { return Body().Rotate(C, pan, tilt); }
    //: Returns new parameter vector representing appearance after change of pose.
    //!param: C - Parameter vector representing the original appearance.
    //!param: pan - Pan angle after change of pose.
    //!param: tilt - Tilt angle after change of pose.
    //  This function estimates the pose of the initial appearance and predicts the corresponding parameter vector in the novel pose.

   VectorC Rotate(const VectorC &C, const RealT &pani, const RealT &tilti, const RealT &panf, const RealT &tiltf) const
     { return Body().Rotate(C, pani, tilti, panf, tiltf); }
    //: Returns new parameter vector representing appearance after change of pose given prior information about the initial pose.
    //!param: C - Parameter vector representing the original appearance.
    //!param: pani - Pan angle before change of pose.
    //!param: tilti - Tilt angle before change of pose.
    //!param: panf - Pan angle after change of pose.
    //!param: tiltf - Tilt angle after change of pose.
    //  This function predicts the parameter vector of an appearance with known initial pose after change of pose.

   VectorC RotateToFrontal(const VectorC &C) const
     { return Body().RotateToFrontal(C); }
    //: Returns new parameter vector representing appearance in frontal pose.
    //!param: C - Parameter vector representing the original appearance.
    //  This function estimates the pose of the initial appearance and predicts the corresponding parameter vector in frontal pose.

   VectorC RotateToFrontal(const VectorC &C, const RealT &pani, const RealT &tilti) const
     { return Body().RotateToFrontal(C, pani, tilti); }
    //: Returns new parameter vector representing appearance in frontal pose given prior information about the initial pose.
    //!param: C - Parameter vector representing the original appearance.
    //!param: pani - Pan angle before change of pose.
    //!param: tilti - Tilt angle before change of pose.
    //  This function predicts the frontal pose parameter vector of an appearance with known initial pose.

  };

  inline
  BinOStreamC &operator<<(BinOStreamC &s,const AAMPoseEstimateC &posest) {
    posest.Save(s);
    return s;
  }
  //: Save statistical model of pose variation to binary stream.

  inline
  BinIStreamC &operator>>(BinIStreamC &s,AAMPoseEstimateC &posest) {
    posest = AAMPoseEstimateC(s);
    return s;
  }
  //: Read statistical model of pose variation from binary stream.

}

#endif
