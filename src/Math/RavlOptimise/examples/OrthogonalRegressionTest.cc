// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlOptimise
//! docentry="Ravl.API.Pattern Recognition.Optimisation2.Cases"
//! userlevel=Normal
//! file="Ravl/Math/Optimisation/OrthogonalRegressionTest.cc"

#include "Ravl/LevenbergMarquardt.hh"
#include "Ravl/StateVectorLine2d.hh"
#include "Ravl/ObservationLine2dPoint.hh"
#include "Ravl/ObsVectorBiGaussian.hh"
#include "Ravl/Random.hh"
#include "Ravl/EntryPnt.hh"
#include "Ravl/OptimiseOrthogonalRegression.hh"

// RANSAC headers
#include "Ravl/Ransac.hh"
#include "Ravl/FitLine2dPoints.hh"
#include "Ravl/EvaluateNumInliers.hh"

using namespace RavlN;

#define ZHOMOG 100.0
#define RANSAC_ITERATIONS 100

// Initialisation function for 2D line state vector
static const StateVectorLine2dC
 InitialiseLine2d ( DListC<ObservationC> &obsList, // observation list
		    RealT zh, // 3rd homogeneous plane coordinate
		    DListC<ObservationC> &compatibleObsList ) // output list of compatible observations
{
  UIntT iteration;
  ObservationListManagerC obsManager(obsList);
  FitLine2dPointsC fitter(zh);
  EvaluateNumInliersC evaluator(3.0, 10.0);
  
  // use RANSAC to fit line
  RansacC ransac(obsManager, fitter, evaluator);

  // select and evaluate the given number of samples
  for ( iteration=0; iteration < RANSAC_ITERATIONS; iteration++ )
    ransac.ProcessSample(2);

  // select observations compatible with solution
  compatibleObsList = evaluator.CompatibleObservations(ransac.GetSolution(),
							 obsList);
  return ransac.GetSolution();
}

static void
 PrintInlierFlags(DListC<ObservationC> obsList)
{
  cout << "Inlier flags: ";
  for(DLIterC<ObservationC> it(obsList);it;it++) {

    // get observation vector/inverse covariance object
    ObservationC& obs(it.Data());
    const ObsVectorBiGaussianC& obsVec = obs.GetObsVec();

    // ignore observations that are not robust
    if(!obsVec.IsValid()) {
      cout << "N";
      continue;
    }

    if(obsVec.Outlier()) cout << "0";
    else cout << "1";
  }
    
  cout << endl;
}

// number of points
#define NPOINTS 100

// ground-truth line coefficients lx,ly,lz
#define LX_TRUE 2.0
#define LY_TRUE 3.0
#define LZ_TRUE 4.0

// noise standard deviation
#define SIGMA 0.1

// outlier noise standard deviation
#define OUTLIER_SIGMA 1.0

static void
 PrintBestEstimate ( SArray1dC<Vector2dC> coords )
{
  RealT Sx=0.0, Sy=0.0, Sxx=0.0, Sxy=0.0, Syy=0.0;
  UIntT n=0;

  // compute solution by orthogonal regression
  for(SArray1dIterC<Vector2dC> it(coords);it;it++) {
    Vector2dC xy = it.Data();

    Sx += xy[0]; Sy += xy[1];
    Sxx += xy[0]*xy[0]; Sxy += xy[0]*xy[1]; Syy += xy[1]*xy[1];
    n++;
  }

  // compute sums normalized for centroid
  RealT nr = (RealT) n;
  RealT p = Sxx - Sx*Sx/nr;
  RealT q = Sxy - Sx*Sy/nr;
  RealT r = Syy - Sy*Sy/nr;

  // compute smallest non-zero eigenvalue of covariance
  // matrix (p q 0)
  //        (q r 0)
  //        (0 0 0)
  RealT lambda = 0.5*(p + r - sqrt((p-r)*(p-r) + 4.0*q*q));

  // compute line parameters lx*x + ly*y + lz = 0
  RealT lx = q;
  RealT ly = lambda - p;
  RealT lz = -(lx*Sx + ly*Sy)/nr;
  RealT s = sqrt(lx*lx + ly*ly);

  // normalise to lx*lx + ly*ly = 1 and convert to lx*x + ly*y + lz*zh = 0
  lx /= s;
  ly /= s;
  lz /= ZHOMOG*s;

  cout << "Orthogonal regression: lx=" << lx << " ly=" << ly << " lz=" << lz;
  cout << endl;
}

static bool
 TestLine2dFit()
{
  SArray1dC<Vector2dC> coords(NPOINTS);
  DListC<ObservationC> obsList;

  // build arrays of x & y coordinates
  IntT i = 0;
  for(SArray1dIterC<Vector2dC> it(coords);it;i++, it++) {
    // x-coordinates evenly spaced
    RealT x = (RealT) i;

    // construct y = a*x^2 + b*y + c + w with added Gaussian noise w
    it.Data() = Vector2dC ( x + SIGMA*RandomGauss(),
			    -(LX_TRUE*x + LZ_TRUE*ZHOMOG)/LY_TRUE
			    + SIGMA*RandomGauss() );
  }
  
  // construct point observations and create list
  i=0;
  MatrixRSC Ni(2);
  Ni.Fill(0.0);
  Ni[0][0] = Ni[1][1] = 1.0/(SIGMA*SIGMA);
  for(SArray1dIterC<Vector2dC> it(coords);it;it++, i++)
    obsList.InsLast(ObservationLine2dPointC(it.Data(), Ni));

  // list of compatible observations
  DListC<ObservationC> compatibleObsList;

  // fit the line parameters using RANSAC
  StateVectorLine2dC stateVecInit = InitialiseLine2d(obsList,ZHOMOG,
						       compatibleObsList);

  // initialise Levenberg-Marquardt algorithm
  LevenbergMarquardtC lm = LevenbergMarquardtC(stateVecInit,obsList);

  cout << "Line 2D fitting test: Initial residual=" << lm.GetResidual();
  cout << endl;

  // apply iterations
  RealT lambda = 10.0;
  for ( i = 0; i < 10; i++ ) {
    bool accepted = lm.Iteration(obsList, lambda);
    if ( accepted )
      // iteration succeeded in reducing the residual
      lambda /= 10.0;
    else
      // iteration failed to reduce the residual
      lambda *= 10.0;

    StateVectorLine2dC sv = lm.GetSolution();
    cout << " a=" << sv.GetLx() << " b=" << sv.GetLy() << " c=" << sv.GetLz();
    cout << " Accepted=" << accepted << " Residual=" << lm.GetResidual();
    cout << " DOF=" << obsList.Size()-2 << endl;
  }

  StateVectorLine2dC sv = lm.GetSolution();
  cout << "Final solution: lx=" << sv.GetLx() << " ly=" << sv.GetLy() << " lz=" << sv.GetLz() << endl;
  PrintBestEstimate(coords);
  cout << endl;

  return true;
}

static bool
 TestLine2dRobustFit()
{
  SArray1dC<Vector2dC> coords(NPOINTS);
  DListC<ObservationC> obsList;
  StateVectorLine2dC sv;

  // build arrays of x & y coordinates
  IntT i = 0;
  for(SArray1dIterC<Vector2dC> it(coords);it;i++, it++) {
    // x-coordinates evenly spaced
    RealT x = (RealT) i;

    if ( i >= 0 && i < 10 )
      // outlier point constructed using uniform distribution over x/y range
      it.Data() = Vector2dC ( Random1()*((RealT)NPOINTS),
			      -(Random1()*LX_TRUE*((RealT)NPOINTS) + LZ_TRUE*ZHOMOG)/LY_TRUE );
    else
      // construct y = a*x^2 + b*y + c with added Gaussian noise on x & y
      it.Data() = Vector2dC ( x + SIGMA*RandomGauss(),
			      -(LX_TRUE*x + LZ_TRUE*ZHOMOG)/LY_TRUE
			      + SIGMA*RandomGauss() );

#if 0
    RealT s2 = LX_TRUE*LX_TRUE + LY_TRUE*LY_TRUE;
    cout << "x=" << it.Data()[0] << " y=" << it.Data()[1] << " Distance^2 = " << Sqr(LX_TRUE*it.Data()[0] + LY_TRUE*it.Data()[1] + LZ_TRUE*ZHOMOG)/s2 << endl;
#endif
  }
  
  // construct point observations and create list
  i=0;
  MatrixRSC Ni(2);
  Ni.Fill(0.0);
  Ni[0][0] = Ni[1][1] = 1.0/(SIGMA*SIGMA);
  for(SArray1dIterC<Vector2dC> it(coords);it;it++, i++)
    obsList.InsLast(ObservationLine2dPointC(it.Data(), Ni,
					     Sqr(OUTLIER_SIGMA/SIGMA), 5.0));

  // list of compatible observations
  DListC<ObservationC> compatibleObsList;

  // fit the line parameters using RANSAC
  StateVectorLine2dC stateVecInit = InitialiseLine2d(obsList,ZHOMOG,
						     compatibleObsList);

  // initialise Levenberg-Marquardt algorithm
  LevenbergMarquardtC lm = LevenbergMarquardtC(stateVecInit,compatibleObsList);

  cout << "Line 2D robust fitting test: Initial residual=" << lm.GetResidual();
  cout << endl;
  cout << "Selected " << compatibleObsList.Size() << " observations using RANSAC" << endl;
  sv = lm.GetSolution();
  cout << "RANSAC solution: lx=" << sv.GetLx() << " ly=" << sv.GetLy() << " lz=" << sv.GetLz() << endl;
  PrintInlierFlags(obsList);

  // apply iterations
  RealT lambda = 10.0;
  for ( i = 0; i < 10; i++ ) {
    bool accepted = lm.Iteration(compatibleObsList, lambda);
    if ( accepted )
      // iteration succeeded in reducing the residual
      lambda /= 10.0;
    else
      // iteration failed to reduce the residual
      lambda *= 10.0;

    sv = lm.GetSolution();
    cout << " lx=" << sv.GetLx() << " ly=" << sv.GetLy() << " lz=" << sv.GetLz();
    cout << " Accepted=" << accepted << " Residual=" << lm.GetResidual();
    cout << " DOF=" << compatibleObsList.Size()-2 << endl;
  }

  sv = lm.GetSolution();
  cout << "Final solution: lx=" << sv.GetLx() << " ly=" << sv.GetLy() << " lz=" << sv.GetLz() << endl;
  RealT s = sqrt(LX_TRUE*LX_TRUE + LY_TRUE*LY_TRUE);
  cout << "Ground truth: lx=" << LX_TRUE/s << " ly=" << LY_TRUE/s << " lz=" << LZ_TRUE/s << endl;
  PrintInlierFlags(obsList);
  cout << endl;

  // Test shrink-wrapped function
  cout << endl;
  cout << "Testing shrink-wrap function" << endl;
  DListC<Point2dObsC> matchList;
  for(DLIterC<ObservationC> it(obsList);it;it++) {
    ObservationLine2dPointC obs = it.Data();
    Vector2dC z(obs.GetZ()[0],obs.GetZ()[1]);
    matchList.InsLast(Point2dObsC(z,obs.GetNi()));
  }
  
  sv = OptimiseOrthogonalRegression ( matchList,
				      Sqr(OUTLIER_SIGMA/SIGMA),
				      5.0, RANSAC_ITERATIONS,
				      3.0, 10.0, 20, 100.0, 0.1 );
  cout << "Final solution: lx=" << sv.GetLx() << " ly=" << sv.GetLy() << " lz=" << sv.GetLz() << endl;
  PrintInlierFlags(obsList);
  cout << endl;

  return true;
}

int Main(int nargs,char **argv)
{
  if ( !TestLine2dFit() ) {
    cout << "TestLine2dFit() failed" << endl;
    return 1;
  }

  if ( !TestLine2dRobustFit() ) {
    cout << "TestLine2dRobustFit() failed" << endl;
    return 1;
  }

  return 0;
}

RAVL_ENTRY_POINT(Main);
