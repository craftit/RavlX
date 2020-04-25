// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////////////////////////////
//! author =    "Jean-Yves Guillemaut"
//! lib=RavlAAM
//! date =      "01/03/2006"
//! file="Ravl/CompVision/ActiveAppearanceModels/AAMPoseEstimate.cc"
//! rcsid="$Id: AAMPoseEstimate.cc,v 1.3 2005/07/25 16:31:07 ees1wc Exp $"

#include "Ravl/Image/AAMPoseEstimate.hh"

#include "Ravl/Assert.hh"
#include "Ravl/Pair.hh"
#include "Ravl/Vector.hh"
#include "Ravl/DP/FileFormatBinStream.hh"
#include "Ravl/DP/FileFormatStream.hh"
#include "Ravl/VirtualConstructor.hh"

#include "Ravl/PatternRec/Parameters.hh"
#include "Ravl/PatternRec/OptimiseDescent.hh"
#include "Ravl/PatternRec/OptimiseRandomUniform.hh"

#include "Ravl/PatternRec/Cost.hh"
#include "Ravl/Image/AAMPoseEstimateCostFunction.hh"

#define DODEBUG 0
#if DODEBUG
#include "Ravl/IO.hh"
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {
  using namespace RavlN;

  //: Default constructor.
  AAMPoseEstimateBodyC::AAMPoseEstimateBodyC()
  {}

  //: Load from bin stream.
  AAMPoseEstimateBodyC::AAMPoseEstimateBodyC(BinIStreamC &is)
    : RCBodyVC(is)
  {
    // eliminate header
    //StringC  header;
    //is >> header;
    is >> C0 >> CT >> ST >> CP >> SP >> CTCP >> CTSP >> STCP >> STSP >> R;
  }

  //: Load from stream.
  AAMPoseEstimateBodyC::AAMPoseEstimateBodyC(istream &is)
    : RCBodyVC(is)
  {
    // eliminate header
    //StringC header;
    //is >> header;
    is >> C0 >> CT >> ST >> CP >> SP >> CTCP >> CTSP >> STCP >> STSP >> R;
  }

  //: Save to binary stream 'out'.
  bool AAMPoseEstimateBodyC::Save(BinOStreamC &out) const {
    if(!RCBodyVC::Save(out))
      return false;
    out << C0 << CT << ST << CP << SP << CTCP << CTSP << STCP << STSP << R;
    return true;
  }

  //: Save to stream 'out'.
  bool AAMPoseEstimateBodyC::Save(ostream &out) const {
    if(!RCBodyVC::Save(out))
      return false;
    out << ' ' << C0 << ' ' << CT << ' ' << ST << ' ' << CP << ' ' << SP << ' ' << CTCP << ' ' << CTSP << ' ' << STCP << ' ' << STSP << ' ' << R;
    return true;
  }

  //: Design the statistical model of pose variation.
  //!param: samp - List of parameter vectors representing the appearances used for training.
  //!param: pan - List of ground truth pan angle values (in radians) for the given appearances.
  //!param: tilt - List of ground truth tilt angle values (in radians) for the given appearances.
  // A pose is parametrised by pan and tilt angles (both measured in radians).
  // The model assumes that each feature point moves on an sphere (i.e. pure pan and tilt rotation motion) when the head is turned.
  // This function learns the relation between a given parameter vector and a pose from the training data.
  // The parameter vector can be either the shape parameter vector or the appearance parameter vetor.
  // In the latter case, only the shape is used to infer the pose; in the former case grey level information is also used to infer the pose.
  bool AAMPoseEstimateBodyC::Design(const SampleC<VectorC> &sample, const SampleC<RealT> &pan, const SampleC<RealT> &tilt) {
    SampleIterC<VectorC> sit(sample);
    SampleIterC<RealT> tait(pan);
    SampleIterC<RealT> nait(tilt);
    if(!sit || !tait || !nait || (sample.Size() != pan.Size()) || (sample.Size() != tilt.Size()))
      return false; // No data points or incorrect data points !

    // set the size of the solution vectors
    UIntT n = sample.Size();
    UIntT dim = sample.First().Size();
    VectorC vec(dim);
    vec.Fill(0.0);
    C0 = vec.Copy();
    CT = vec.Copy();
    ST = vec.Copy();
    CP = vec.Copy();
    SP = vec.Copy();
    CTCP = vec.Copy();
    CTSP = vec.Copy();
    STCP = vec.Copy();
    STSP = vec.Copy();
    R = vec.Copy();

    // set coefficients matrix, A and B vectors
    MatrixC A(n, 9, 1.0);
    MatrixC B(dim, n); 
    IndexC ind=0;
    for(tait.First(), nait.First(), sit.First(); tait ;tait++, nait++, sit++) {
      A[ind][1] = Cos(tait.Data());
      A[ind][2] = Sin(tait.Data());
      A[ind][3] = Cos(nait.Data());
      A[ind][4] = Sin(nait.Data());
      A[ind][5] = Cos(tait.Data())*Cos(nait.Data());
      A[ind][6] = Cos(tait.Data())*Sin(nait.Data());
      A[ind][7] = Sin(tait.Data())*Cos(nait.Data());
      A[ind][8] = Sin(tait.Data())*Sin(nait.Data());
      B.SetColumn(ind, sit.Data());
      ind++;
    }

    //solve the equations
    for(UIntT i=0; i<dim; i++) {
      MatrixC A2 = A.Copy();
      VectorC b  = B.SliceRow(i).Copy();
      RealT r = 0.0;
      if(!LeastSquaresQR_IP(A2,b,r)) {
        throw ExceptionNumericalC("Dependent linear equations in PoseEstimateBodyC::Design(const SampleC<VectorC>, const SampleC<RealT>). ");
        return false;
      }
      else {
        C0[i] = b[0];
        CT[i] = b[1];
        ST[i] = b[2];
        CP[i] = b[3];
        SP[i] = b[4];
        CTCP[i] = b[5];
        CTSP[i] = b[6];
        STCP[i] = b[7];
        STSP[i] = b[8];
        R[i] = r;
      }
    }

    return true;
  }


  //: Estimate the pose of the appearance represented by parameter vector 'C'.
  //  The method first computes a least-squares solution based on the pseudo-inverse and then refines the solution by non-linear minimisation.
  PairC<RealT> AAMPoseEstimateBodyC::EstimatePose(const VectorC &C) const {

    if(C.Size() != C0.Size()) {
      throw ExceptionOperationFailedC("Invalid parameter vector size");
    }

    MatrixC Rc(C0.Size(), 8);
    Rc.SetColumn(0, CT);
    Rc.SetColumn(1, ST);
    Rc.SetColumn(2, CP);
    Rc.SetColumn(3, SP);
    Rc.SetColumn(4, CTCP);
    Rc.SetColumn(5, CTSP);
    Rc.SetColumn(6, STCP);
    Rc.SetColumn(7, STSP);

    // compute the pseudo-inverse of the matrix Ccs
    MatrixC invRc = Rc.PseudoInverse();
    VectorC x = invRc * (C-C0);

    // compute initial pose estimate
    RealT theta_0 = ATan2(x[1], x[0]);
    RealT phi_0 = ATan2(x[3],x[2]);

    // refine pose estimate by steepest-descent
    VectorC minP(-45.0*RavlConstN::pi/180,-45.0*RavlConstN::pi/180);
    VectorC maxP(45.0*RavlConstN::pi/180,45.0*RavlConstN::pi/180);
    SArray1dC<IntT> steps(2);
    steps.Fill(1);
    ParametersC opt_parameters(minP, maxP, steps);
    opt_parameters.SetConstP(VectorC(theta_0, phi_0));
    AAMPoseEstimateCostFunctionC cost(opt_parameters, C0, CT, ST, CP, SP, CTCP, CTSP, STCP, STSP, C);
    UIntT iterations = 100;
    RealT tolerance = 1e-12;
    OptimiseDescentC opt_descent(iterations,tolerance);
    RealT d_opt;
    VectorC opt_pose = opt_descent.MinimalX(cost, d_opt);
    PairC<RealT> angles(opt_pose[0], opt_pose[1]);

    // display optimum found
    //RealT d0 = cost.Cost(VectorC(theta_0, phi_0));
    //cerr << PairC<RealT>(theta_0, phi_0) << " " << d0 << endl;
    //cerr << angles << " " << d_opt << endl;

    return(angles);

  }


  //: Estimate the pose of the appearance represented by parameter vector 'C'.
  //  The method first computes a least-squares solution based on the pseudo-inverse and then refines the solution by non-linear minimisation.
  bool AAMPoseEstimateBodyC::EstimatePose(const VectorC &C, PairC<RealT> &angles) const {
    if(C.Size() != C0.Size()) {
      cerr << "Warning, expected parameter size " << C0.Size() << " and got " << C.Size() << endl;
      return false;
    }
    angles = EstimatePose(C);
    return true;
  }

  //: Outputs the profile of the cost function.
  //!param: C - Parameter vector representing the appearance for which we would like to evaluate the pose.
  //!param: fileName - Name of the output file for profile.
  // This method is useful only for testing purpose, in particular to check the convergence properties of the pose estimator.
  // Each line of the output file contains the following information: 'theta' 'pan' 'residual', for all possible pose in the range +/-45 degrees sampled evry degree.
  bool AAMPoseEstimateBodyC::GenerateProfile(const VectorC &C, StringC fileName) const {

    OStreamC fout(fileName);

    MatrixC Rc(C0.Size(), 8);
    Rc.SetColumn(0, CT);
    Rc.SetColumn(1, ST);
    Rc.SetColumn(2, CP);
    Rc.SetColumn(3, SP);
    Rc.SetColumn(4, CTCP);
    Rc.SetColumn(5, CTSP);
    Rc.SetColumn(6, STCP);
    Rc.SetColumn(7, STSP);

    VectorC minP(-45.0*RavlConstN::pi/180,-45.0*RavlConstN::pi/180);
    VectorC maxP(45.0*RavlConstN::pi/180,45.0*RavlConstN::pi/180);
    SArray1dC<IntT> steps(2);
    steps.Fill(900000);
    ParametersC opt_parameters(minP, maxP, steps);

    AAMPoseEstimateCostFunctionC cost(opt_parameters, C0, CT, ST, CP, SP, CTCP, CTSP, STCP, STSP, C);

    for(RealT theta=-45; theta<=45; theta+=0.1) {
      for(RealT phi=-45; phi<=45; phi+=0.1) {
        RealT d = cost.Cost(VectorC(theta*RavlConstN::pi/180, phi*RavlConstN::pi/180));
        fout << theta << " " << phi << " " << d << endl;
      }
    }

    return(true);

  }

  //: Returns new parameter vector representing appearance after change of pose.
  //!param: C - Parameter vector representing the original appearance.
  //!param: pan - Pan angle after change of pose.
  //!param: tilt - Tilt angle after change of pose.
  //  This function estimates the pose of the initial appearance and predicts the corresponding parameter vector in the novel pose.
  VectorC AAMPoseEstimateBodyC::Rotate(const VectorC &C, const RealT &pan, const RealT &tilt) const {
    PairC<RealT> angles = EstimatePose(C);
    VectorC r = C - (C0+CT*Cos(angles.Data1())+ST*Sin(angles.Data1())+CP*Cos(angles.Data2())+SP*Sin(angles.Data2())+CTCP*Cos(angles.Data1())*Cos(angles.Data2())+CTSP*Cos(angles.Data1())*Sin(angles.Data2())+STCP*Sin(angles.Data1())*Cos(angles.Data2())+STSP*Sin(angles.Data1())*Sin(angles.Data2()));
    //cout << "r: " << r << endl;
    VectorC Calpha = C0+CT*Cos(pan)+ST*Sin(pan)+CP*Cos(tilt)+SP*Sin(tilt)+CTCP*Cos(pan)*Cos(tilt)+CTSP*Cos(pan)*Sin(tilt)+STCP*Sin(pan)*Cos(tilt)+STSP*Sin(pan)*Sin(tilt) + r;
    return(Calpha);
  }

  //: Returns new parameter vector representing appearance after change of pose given prior information about the initial pose.
  //!param: C - Parameter vector representing the original appearance.
  //!param: pani - Pan angle before change of pose.
  //!param: tilti - Tilt angle before change of pose.
  //!param: panf - Pan angle after change of pose.
  //!param: tiltf - Tilt angle after change of pose.
  //  This function predicts the parameter vector of an appearance with known initial pose after change of pose.
  VectorC AAMPoseEstimateBodyC::Rotate(const VectorC &C, const RealT &pani, const RealT &tilti, const RealT &panf, const RealT &tiltf) const {
    PairC<RealT> angles(pani, tilti);
    VectorC r = C - (C0+CT*Cos(angles.Data1())+ST*Sin(angles.Data1())+CP*Cos(angles.Data2())+SP*Sin(angles.Data2())+CTCP*Cos(angles.Data1())*Cos(angles.Data2())+CTSP*Cos(angles.Data1())*Sin(angles.Data2())+STCP*Sin(angles.Data1())*Cos(angles.Data2())+STSP*Sin(angles.Data1())*Sin(angles.Data2()));
    //cout << "r: " << r << endl;
    VectorC Calpha = C0 + CT*Cos(panf)+ST*Sin(panf)+CP*Cos(tiltf)+SP*Sin(tiltf)+CTCP*Cos(panf)*Cos(tiltf)+CTSP*Cos(panf)*Sin(tiltf)+STCP*Sin(panf)*Cos(tiltf)+STSP*Sin(panf)*Sin(tiltf) + r;
    return(Calpha);
  }

  RAVL_INITVIRTUALCONSTRUCTOR_FULL(AAMPoseEstimateBodyC,AAMPoseEstimateC,RCHandleVC<AAMPoseEstimateBodyC>);

}
