// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DESIGNGAUSSIANMIXTURE_HEADER
#define RAVL_DESIGNGAUSSIANMIXTURE_HEADER 1
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! author="Kieron Messer"
//! docentry="Ravl.API.Pattern Recognition.Expectation Maximization"
//! example=exDesignGaussianMixture.cc
//! file="Ravl/PatternRec/Modeling/GaussianMixture/DesignGaussianMixture.hh"

#include "Ravl/PatternRec/DesignFunctionUnsupervised.hh"
#include "Ravl/PatternRec/GaussianMixture.hh"
#include "Ravl/PatternRec/SampleVector.hh"
#include "Ravl/PatternRec/DesignCluster.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Computes normal parameters of a set of unlabelled data using EM algorithm
  // This function estimates a PDF as a Gaussian mixture.
  // It can be constructed from a set of data points.  The parameters
  // of the gaussians are then estimated using the Expectation Maximisation
  // algorithm (Dempster et al (1977) J. Royal Statistical Society Series B, 39:1-38).
  
  class DesignGaussianMixtureBodyC
    : public DesignFunctionUnsupervisedBodyC
  {
  public:
    DesignGaussianMixtureBodyC();
    //: Default constructor.
    
    DesignGaussianMixtureBodyC(UIntT mixes, bool isDiagonal,const DesignClusterC &initCluster = DesignClusterC());
    //: Number of centres for the EM algorithm
    
    DesignGaussianMixtureBodyC(std::istream &strm);
    //: Load from stream.
    
    DesignGaussianMixtureBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (std::ostream &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual FunctionC Apply(const SampleC<VectorC> &in);
    //: Build the EM model from the supplied data
        
  protected:
    FunctionC ApplyWorkSpace(const SampleC<VectorC> &in);
    //: Build the EM model from the supplied data
    // Uses workspace approach. Faster for small datasets.
    
    FunctionC ApplyIterative(const SampleC<VectorC> &in);
    //: Build the EM model from the supplied data
    // Uses iterative approach.  Slower, but better for large datasets.
    
    GaussianMixtureC InitModel(const SampleVectorC & in);
    //: General initialisating function.
    
    GaussianMixtureC InitRandom(const SampleVectorC & in);
    //: initialise the mixture model

    GaussianMixtureC InitKMeans(const SampleVectorC & in);
    //: initialise the mixture model

    UIntT mixes;
    //: The number of mixtures to consider
    
    bool isDiagonal;
    //: Do you want to compute a full covariance matrix or diagonal
    
    DesignClusterC initCluster;
    //: Clustering code use to initialise guassians.
    
  };
  
  //! userlevel=Normal
  //: Computes normal parameters of a set of unlabelled data using EM algorithm
  // This function estimates a PDF as a Gaussian mixture.
  // It can be constructed from a set of data points.  The parameters
  // of the gaussians are then estimated using the Expectation Maximisation
  // algorithm (Dempster et al (1977) J. Royal Statistical Society Series B, 39:1-38).
  
  class DesignGaussianMixtureC
    : public DesignFunctionUnsupervisedC
  {
  public:
    DesignGaussianMixtureC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    DesignGaussianMixtureC(UIntT mixes, bool isDiagonal=false,const DesignClusterC &initCluster = DesignClusterC())
      : DesignFunctionUnsupervisedC(*new DesignGaussianMixtureBodyC(mixes, isDiagonal,initCluster))
    {}
    //: How many centres and do we want to use diagonal covariances
    
  protected:
    DesignGaussianMixtureC(DesignGaussianMixtureBodyC &bod)
      : DesignFunctionUnsupervisedC(bod)
    {}
    //: Body constructor.
    
    DesignGaussianMixtureBodyC &Body()
    { return static_cast<DesignGaussianMixtureBodyC &>(DesignerC::Body()); }
    //: Access body.
    
    const DesignGaussianMixtureBodyC &Body() const
    { return static_cast<const DesignGaussianMixtureBodyC &>(DesignerC::Body()); }
    //: Access body.
    
  public:    
  };

}



#endif
