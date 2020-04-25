// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! author="Charles Galambos"
//! file="Ravl/PatternRec/Modeling/GaussianMixture/GaussianMixture.cc"

#include "Ravl/PatternRec/GaussianMixture.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/SArray1dIter3.hh"
#include "Ravl/SArray1dIter4.hh"
#include "Ravl/SArray1dIter5.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/config.h"

#if RAVL_COMPILER_MIPSPRO 
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/BinStream.hh"
#pragma instantiate RavlN::GaussianMixtureBodyC* RavlN::VCLoad(RavlN::BinIStreamC&,RavlN::GaussianMixtureBodyC*)
#endif 

namespace RavlN {

  GaussianMixtureBodyC::GaussianMixtureBodyC()
  {}

  //: Constructor from an array of indexes.
  
  GaussianMixtureBodyC::GaussianMixtureBodyC(const SArray1dC<MeanCovarianceC> & prms, const SArray1dC<RealT> & wgt, bool diag)
    : params(prms), 
      weights(wgt), 
      isDiagonal(diag)
  {
    //: Lets do some checks
    if(params.Size()!=weights.Size()) 
      RavlIssueError("Gaussian Mixture parameters not of same dimensionality as mixing weights");
    
    UIntT outputSize = params.Size();
    UIntT inputSize = params[0].Mean().Size();

    if(outputSize<1) 
      RavlIssueError("Gaussian Mixture model has output dimension of < 1");

    if(inputSize<1) 
      RavlIssueError("Gaussian Mixture model has input dimension < 1");

    OutputSize(outputSize);
    InputSize(inputSize);

    //: Lets precompute stuff
    precompute();

  }

  GaussianMixtureBodyC::GaussianMixtureBodyC(const SArray1dC<VectorC> & means, const SArray1dC<MatrixRSC> & covariances, const SArray1dC<RealT> & wgt, bool diag)
    : params(means.Size()), weights(wgt), isDiagonal(diag)
  {

    //: Lets do some checks
    if((means.Size()!=covariances.Size()) || (means.Size()!=weights.Size())) 
      RavlIssueError("Gaussian Mixture parameters not of same dimension");

    // now lets build our model
    for(SArray1dIter3C<MeanCovarianceC, VectorC, MatrixRSC>it(params, means,  covariances);it;it++)
      it.Data1() = MeanCovarianceC(1, it.Data2(), it.Data3());
    
    UIntT outputSize = params.Size();
    UIntT inputSize = params[0].Mean().Size();

    if(outputSize<1) 
      RavlIssueError("Gaussian Mixture model has output dimension of < 1");

    if(inputSize<1) 
      RavlIssueError("Gaussian Mixture model has input dimension < 1");

    OutputSize(outputSize);
    InputSize(inputSize);

    //: Lets precompute stuff
    precompute();
  }



  //: Load from stream.
  GaussianMixtureBodyC::GaussianMixtureBodyC(std::istream &strm) 
    : Function1BodyC(strm) 
  { 
    strm >> params >> weights >> isDiagonal; 
    precompute(false); //: should have already regularised before writing to file, no need to do it again
  }
  
  
  //: Writes object to stream.  
  bool GaussianMixtureBodyC::Save (std::ostream &out) const {
    if(!FunctionBodyC::Save(out))
      return false;
    out << '\n' << params << '\n' << weights << '\n' << isDiagonal;
    return true;    
  }
  
  //: Load from binary stream.  
  GaussianMixtureBodyC::GaussianMixtureBodyC(BinIStreamC &strm) 
    : Function1BodyC(strm) 
  { 
    strm >> params  >> weights >> isDiagonal;     
    precompute(false); //: should of already regularised before writing to file
  }

  //: Writes object to binary stream.  
  bool GaussianMixtureBodyC::Save (BinOStreamC &out) const {
    if(!FunctionBodyC::Save(out))
      return false;
    out << params << weights << isDiagonal;
    return true;
  }
  
  //: Get vector of individual values.
  
  VectorC GaussianMixtureBodyC::GaussianValues(const VectorC &data) const {
    //: do some checks
    if(data.Size()!=InputSize()) 
      RavlIssueError("Input data of different dimension to that of model");
    
    VectorC out(OutputSize());
    for(SArray1dIter4C<MeanCovarianceC, RealT, MatrixRSC, RealT>it(params, weights, invCov, det);it;it++) {
      VectorC D = data - it.Data1().Mean();
      RealT exponent = D.Dot( it.Data3() * D);
      if(isDiagonal) { // (we actually use *log* determinant)
        out[it.Index()]  = it.Data2() * Exp(-0.5*(it.Data4() + exponent));
      } else {
        out[it.Index()]  = it.Data2() * ((1.0/(konst * Sqrt(it.Data4()))) * Exp(-0.5 * exponent));
      }
    }
    return out;
  }
  
  //: Return the density value at point X
  
  RealT GaussianMixtureBodyC::DensityValue(const VectorC & data) const { 
    RealT ret = 0;
    
    for(SArray1dIter4C<MeanCovarianceC, RealT, MatrixRSC, RealT> it(params, weights, invCov, det);it;it++) {
      VectorC D = data - it.Data1().Mean();
      RealT exponent = D.Dot( it.Data3() * D);
      if(isDiagonal) { // (we actually use *log* determinant)
        ret += it.Data2() * Exp(-0.5*(it.Data4() + exponent));
      } else {
        ret += it.Data2() * ((1.0/(konst * Sqrt(it.Data4()))) * Exp(-0.5 * D.Dot( it.Data3() * D)));
      }
    }
    return ret; 
  }
  
  //: Compute the density at a given point 
  VectorC GaussianMixtureBodyC::Apply(const VectorC & data) const  {
    VectorC ret(1);
    ret[0] = DensityValue(data);
    return ret;
  }
  
  //: Apply function to 'data'
  
  RealT GaussianMixtureBodyC::Apply1(const VectorC &data) const {
    return DensityValue(data);
  }
  
  //: Precompute the inverse matrices e.t.c..
  void GaussianMixtureBodyC::precompute(bool regularise)
  {

    //: For speed, lets precompute some stuff
    
    //: First we have to check for very small variances
    //: The smallest variance we allow
    RealT smallVariance = 0.001;
    RealT smallDeterminant = 1e-150;

    //: lets regularise our model
    //: this has effect of increasing the distance slightly in all orthogonal directions
    //: not great, bit of  a hack
    if(regularise) {
      for(SArray1dIterC<MeanCovarianceC> paramIt(params);paramIt;paramIt++)  {
	for (UIntT i=0; i<inputSize; i++)
	  paramIt.Data().Covariance()[i][i] += smallVariance;
      }
    }
    
    //: make room for the arrays
    invCov = SArray1dC<MatrixRSC>(OutputSize());
    det = SArray1dC<RealT>(OutputSize());
    if(isDiagonal) {
      konst =  (RealT)InputSize() * Log(2.0*RavlConstN::pi);
    }
    else {
      konst =  Pow(2.0* RavlConstN::pi, (RealT)InputSize()/2.0);
    }

    //: compute inverse and determinant of models
    for(SArray1dIterC<MeanCovarianceC>paramIt(params);paramIt;paramIt++)  {
      IndexC i = paramIt.Index();
      if(isDiagonal) { // we actually compute the log determinant
	det[i]=konst;
	invCov [i] = MatrixRSC(inputSize);
	invCov[i].Fill(0.0);
	for(UIntT j=0;j<inputSize;j++) {
	  invCov[i][j][j]=1.0/paramIt.Data().Covariance()[j][j];
	  det[i] += Log(paramIt.Data().Covariance()[j][j]); 
	}
      }
      else {
	invCov[i] = paramIt.Data().Covariance().NearSingularInverse(det[i]);             
        if(det[i]<smallDeterminant) {
          //: if this is called then we have a problem on one component
          //: having a very small variance.  We do try and avoid this
          //: by setting a minimum allowed variance, but it is not foolproof.
          RavlIssueError("The deteminant is too small (or negative).  Unsuitable data, perhaps use PCA.");
        } 
      }      
    }

  }


  RAVL_INITVIRTUALCONSTRUCTOR_FULL(GaussianMixtureBodyC,GaussianMixtureC,Function1C);

}
