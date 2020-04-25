// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2003-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlPatternRec

#include "Ravl/PatternRec/DesignGaussianMixture.hh"
#include "Ravl/PatternRec/Function.hh"

using namespace RavlN;

// This trivial example generates a normally distributed sample, and then fits
// a single Gaussian to it.

int main(int argc, char **argv) 
{
  IntT SampleSize = 1000;
  SampleC<VectorC> Samples(SampleSize);
  
  for (RealT counter=0; counter<SampleSize; ++counter){
    VectorC sample(1);
    sample[0] = RandomGauss();
    Samples.Append(sample);
  }
  
  DesignGaussianMixtureC gmm(1,true);
  GaussianMixtureC func = gmm.Apply(Samples);
  
  MeanCovarianceC mc = func.MeanCovariances()[0];
  std::cout << "Means:" << mc.Mean() << std::endl;
  std::cout << "Covs:" << mc.Covariance() << std::endl;
  return 0;
}




