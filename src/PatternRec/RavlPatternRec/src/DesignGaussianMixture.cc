// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Modeling/GaussianMixture/DesignGaussianMixture.cc"

#include "Ravl/PatternRec/DesignGaussianMixture.hh"
#include "Ravl/PatternRec/DesignKMeans.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/PatternRec/SampleVector.hh"
#include "Ravl/Collection.hh"
#include "Ravl/PatternRec/SampleIter.hh"
#include "Ravl/PatternRec/DistanceEuclidean.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/SArray1dIter3.hh"
#include "Ravl/config.h"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

#if RAVL_COMPILER_MIPSPRO
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/SArray1dIter3.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/Vector.hh"
#include "Ravl/MatrixRS.hh"
#include "Ravl/DP/Entity.hh"
#pragma instantiate RavlN::SArray1dIter3C<RavlN::VectorC,RavlN::MatrixRSC,double>
#pragma instantiate RavlN::GaussianMixtureBodyC* RavlN::VCLoad(std::istream&,RavlN::GaussianMixtureBodyC*)
#pragma instantiate RavlN::SArray1dIter2C<RavlN::VectorC,RavlN::MatrixRSC>
#pragma instantiate RavlN::SArray1dIterC<RavlN::MatrixRSC>
#pragma instantiate RavlN::SArray1dIter2C<RavlN::MeanCovarianceC,RavlN::VectorC>
#pragma instantiate RavlN::BufferAccessIter3C<RavlN::VectorC,RavlN::MatrixRSC,double>
#pragma instantiate RavlN::SArray1dIter2C<RavlN::VectorC,double>
#pragma instantiate RavlN::SArray1dIter2C<RavlN::MatrixRSC,double>
//#pragma instantiate RavlN::DPEntityC
#endif 

namespace RavlN {

  DesignGaussianMixtureBodyC::DesignGaussianMixtureBodyC()
    : mixes(1),
      isDiagonal(false)
  {}

  //: Default constructor.
  
  DesignGaussianMixtureBodyC::DesignGaussianMixtureBodyC(UIntT mx, bool diag,const DesignClusterC &nInitCluster)
    : DesignFunctionUnsupervisedBodyC(),
      mixes(mx),
      isDiagonal(diag),
      initCluster(nInitCluster)
  {}
  
  //: Load from stream.
  
  DesignGaussianMixtureBodyC::DesignGaussianMixtureBodyC(std::istream &strm) 
    : DesignFunctionUnsupervisedBodyC(strm)
  {
    RavlIssueError("not implemented");
  }
  
  //: Load from binary stream.
  
  DesignGaussianMixtureBodyC::DesignGaussianMixtureBodyC(BinIStreamC &strm)
    : DesignFunctionUnsupervisedBodyC(strm)
  {
    RavlIssueError("not implemented");
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DesignGaussianMixtureBodyC::Save(std::ostream &out) const {
    if(!DesignFunctionUnsupervisedBodyC::Save(out))
      return false;
    RavlIssueError("not implemented");
    return true;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DesignGaussianMixtureBodyC::Save(BinOStreamC &out) const {
    if(!DesignFunctionUnsupervisedBodyC::Save(out))
      return false;
    RavlIssueError("not implemented");
    return true;
  }


  FunctionC DesignGaussianMixtureBodyC::Apply(const SampleC<VectorC> &in) {
    UIntT size = in.Size();
    if(size < 10000)
      return ApplyWorkSpace(in);
    return ApplyIterative(in);
  }
  
  
  //: Build the EM model from the supplied data
  // Uses workspace approach. Faster for small datasets.
  
  FunctionC DesignGaussianMixtureBodyC::ApplyWorkSpace(const SampleC<VectorC> &in) {
    ONDEBUG(std::cerr << "DesignGaussianMixtureBodyC::ApplyWorkSpace(), Called with " << in.Size() << " samples. \n");
    if(in.IsEmpty()) // Empty sample ?
      return FunctionC(); 
    //: set some variables
    
    UIntT d = in.First().Size();
    RealT epsilon = 0.0001;
    RealT likelihood, prev_likelihood=0.0;
    RealT err_res = 2.0/(RealT)in.Size();
    bool done=false;
    UIntT iter=0;
    
    //: Get a good first guess by using the KMeans classifier
    // Note, this may update 'mixes'.
    GaussianMixtureC gm = InitModel(in);
    
    //: lets set up some workspace
    MatrixC hij(in.Size(), mixes); 
    VectorC emptyVec(d);
    MatrixRSC emptyMat(d);
    VectorC emptyWeights(mixes);
    emptyVec.Fill(0.0);
    emptyMat.Fill(0.0);
    emptyWeights.Fill(0.0);
    VectorC sum(mixes);

 
    //: now lets loop through until criterion reached
    while(!done) {

      //: Lets do the expectation step
      likelihood=0.0;
      IntT count=0;
      for(SampleIterC<VectorC> it(in);it;it++) {
	VectorC post = gm.GaussianValues(*it);
	RealT psum = post.Sum();
	if(psum>0.0) {
	  post/=psum;
	  likelihood+=Log(psum);
	} else post.Fill(0.0);
	hij.SetRow(count,  post);
	count++;
      }
      likelihood=(-1.0 * likelihood);

      //: Lets set up the workspace for the new parameters
      SArray1dC<VectorC> nmeans(mixes);  
      SArray1dC<MatrixRSC> ncovs(mixes);
      SArray1dC<RealT>nweights = emptyWeights.Copy();
      for(SArray1dIter3C<VectorC, MatrixRSC, RealT>itp(nmeans, ncovs, nweights);itp;itp++) {
	itp.Data1() = emptyVec.Copy();
	itp.Data2() = emptyMat.Copy();
      }
      
      // Find new mean values
      count = 0;
      SampleIterC<VectorC> itm(in);
      for(;itm;itm++) {
	for(SArray1dIterC<VectorC>meanIt(nmeans);meanIt;meanIt++) meanIt.Data() += (itm.Data() * hij[count][meanIt.Index()]);
	count++;
      }
      
      //: calculate the sum of posterior probs
      UIntT i;
      for(i=0;i<mixes;i++) sum[i]=hij.SliceColumn(i).Sum();
      
      //: get the new means
      for(i=0;i<mixes;i++) nmeans[i] /= sum[i];
      
      count = 0;
      for(itm.First();itm;itm++) {	
	for(SArray1dIter2C<VectorC, MatrixRSC>paramIt(nmeans, ncovs);paramIt;paramIt++) {
	  IndexC ind = paramIt.Index();
	  VectorC D = itm.Data() - paramIt.Data1();
	  MatrixRSC temp = D.OuterProduct();
	  temp *= hij[count][ind];
	  if(!isDiagonal) paramIt.Data2() += temp; // update full covariance matrix
	  else paramIt.Data2().AddDiagonal(temp.Diagonal()); // only update diagonal
	}
	count++;
      }
      
      //: get the new covariances
      for(SArray1dIterC<MatrixRSC>covIt(ncovs);covIt;covIt++) covIt.Data() /= sum[covIt.Index()];    
      
      //: now lets compute the weights (carefully)
      for(i=0;i<mixes;i++) { 
	nweights[i] = sum[i] / (RealT)in.Size();
	
	//: if we have a small weight we just set that component to have zero effect
	if(nweights[i]<err_res) {
	  ncovs[i].Fill(0.0);
	  nweights[i]=0.0;
	  for (UIntT j = 0; j <d; j++)
	    ncovs[i][j][j] = 1e-3; //: do this so pre-compute and evaluate work
	}
      }
    
      nweights /= nweights.Sum(); //: just make sure they add to one we can lose precision
      
      //: we can stop if change in likelihood is neglible
      RealT change = Abs((likelihood - prev_likelihood)/likelihood);
      ONDEBUG(std::cerr << "likelihood: " << likelihood << " change " << change << endl);
      if(change < epsilon) done=true;
      else prev_likelihood = likelihood;   
      
      // finally we can update the model 
      gm = GaussianMixtureC(nmeans, ncovs, nweights, isDiagonal);
      if(iter++==100) {
	cerr << "maximum iterations reached.  trying again.";
	InitRandom(in);
      }
      
      if(iter==200) {
	cerr << "change in log-likelihood never good enough: " << change << std::endl;
	done=true;
      }
    }
    
    return gm;    
  }
  
  //: Build the EM model from the supplied data
  // Uses iterative approach.  Slower, but better for large datasets.
  
  FunctionC DesignGaussianMixtureBodyC::ApplyIterative(const SampleC<VectorC> &data) {
    if(data.IsEmpty()) // Empty sample ?
      return FunctionC(); 
    RealT epsilon = 0.0001;
    RealT likelihood, prev_likelihood=0.0;
    SArray1dC<VectorC> prob(data.Size());
    SampleIterC<VectorC> DataIt(data);
    SampleIterC<VectorC> ProbIt(prob);
    bool done=false;
    IntT iter=0;
    UIntT d = data.First().Size();

    GaussianMixtureC gm = InitModel(data);
    VectorC emptyVec(d);
    MatrixRSC emptyMat(d);
    VectorC emptyWeights(mixes);
    emptyVec.Fill(0.0);
    emptyMat.Fill(0.0);
    emptyWeights.Fill(0.0);
  
    while(!done) {
      UIntT count=0;
      likelihood=0.0;
    
      //: first lets do expectation step
      //-----------------------------------------
      for(DataIt.First();DataIt.IsElm();DataIt.Next()) {
	VectorC post = gm.GaussianValues(DataIt.Data());
	RealT sum = post.Sum();
	if(sum>0.0) {
	  post/=sum;
	  likelihood += Log(sum);
	}
	else post.Fill(0.0);
	prob[count]=post;
	count++;
      }
      likelihood=(-1.0 * likelihood);

      //: Lets set up the workspace for the new parameters
      //-------------------------------------------------------------------------
      SArray1dC<VectorC> means(mixes);  
      SArray1dC<MatrixRSC> covs(mixes);
      SArray1dC<RealT>weights = emptyWeights.Copy();
      for(SArray1dIter2C<VectorC, MatrixRSC>it(means, covs);it;it++) {
	it.Data1() = emptyVec.Copy();
	it.Data2() = emptyMat.Copy();
      }

      //: lets do maximisation step
      //----------------------------------------------------------
      //: first compute update to weights and mean
      for(DataIt.First(), ProbIt.First();DataIt.IsElm();DataIt.Next(), ProbIt.Next()) {
	for(SArray1dIter2C<VectorC, RealT>it(means, weights);it;it++) {
	  it.Data2() += ProbIt.Data()[it.Index()];  //: this computes sum of \sum P(j|x)
	  it.Data1()+= (DataIt.Data() * ProbIt.Data()[it.Index()]);  //: this computes \sum P(j|x) x
	}      
      }
  
      //: compute the mean
      for(SArray1dIter2C<VectorC, RealT>itm(means, weights);itm;itm++) itm.Data1()/=itm.Data2();
  
      //: now we have mean we compute updates for covariance matrix
      for(DataIt.First(), ProbIt.First();DataIt.IsElm();DataIt.Next(), ProbIt.Next()) {
	for(SArray1dIter2C<VectorC, MatrixRSC>it(means, covs);it;it++) {
	  VectorC dif = DataIt.Data() - it.Data1();
	  it.Data2() += (dif.OuterProduct() * ProbIt.Data()[it.Index()]);  //: this computes \sum P(j|x) (x-mn)(x-mn)^T     
	}
      }
  
      //: compute the covariance and weights
      for(SArray1dIter2C<MatrixRSC, RealT>itc(covs, weights);itc;itc++) {
	if(!isDiagonal) {
	  itc.Data1() /= itc.Data2();
	} else {
	  for(UIntT i=0;i<d;i++) for(UIntT j=0;j<d;j++) {
	    if(i==j) itc.Data1()[i][j] /=itc.Data2(); 
	    else itc.Data1()[i][j]=0.0;
	  }
	}
	itc.Data2() /= (RealT)data.Size();
      }

      //: we can stop early if change in likelihood is neglible
      RealT change = Abs((likelihood - prev_likelihood)/likelihood);
      //cout << "likelihood: " << likelihood << " change " << change << std::endl;
      if((change < epsilon)&&(iter > 10)) done=true;
      else prev_likelihood = likelihood;   
    
      // finally we can update the model 
      gm = GaussianMixtureC(means, covs, weights, isDiagonal);
    
      if(iter++==100) {
	cerr << "maximum iterations reached.  trying again.";
	cerr << "change in log-likelihood never good enough: " << change << std::endl;
	cerr << "returning model anyway." << std::endl;
	done=true;
      }
    
    }
  
    return gm;
    
  }


  //: General initialisating function.
  
  GaussianMixtureC DesignGaussianMixtureBodyC::InitModel(const SampleVectorC & data) {
    if(!initCluster.IsValid())
      initCluster = DesignKMeansC(mixes); // Default to k-means
    SArray1dC<MeanCovarianceC> params = initCluster.Cluster(data);
    mixes=params.Size();
    ONDEBUG(std::cerr << "DesignGaussianMixtureBodyC::InitModel(), Mixtures=" << mixes << " Model=" << params << "\n");
    SArray1dC<RealT> weights(mixes);
    for(SArray1dIterC<RealT> it(weights);it;it++) 
      *it=1.0/(RealT)mixes;
    //: finally lets construct our model
    GaussianMixtureC gm(params, weights, isDiagonal);
    return gm;
  }
  
  GaussianMixtureC
  DesignGaussianMixtureBodyC::InitRandom(const SampleVectorC & data)
  {
    //: Get dimension of data
    UIntT d = data.First().Size();
  
    //: set the means to random vectors of the data
    SArray1dC<VectorC>vecs(mixes);
    for(SArray1dIterC<VectorC>it(vecs);it;it++) 
      *it = data.Nth(Floor(Random1()*(RealT)data.Size()));

    //: set the covariances to the minimum euclidean distances between these mean 
    DistanceEuclideanC euc;
    RealT minDist=1e20;
    for(UIntT i=0;i<mixes;i++) 
      for(UIntT j=0;j<mixes;j++) 
	if(i!=j) {
	  RealT dist = euc.Measure(vecs[i], vecs[j]);
	  if(dist<minDist) minDist=dist;
	}
    
  
    VectorC diag(d);
    diag.Fill(minDist);
    MatrixRSC cov(d);
    cov.SetDiagonal(diag);
  
    SArray1dC<MeanCovarianceC>arr(mixes);
    for(SArray1dIter2C<MeanCovarianceC, VectorC>itmc(arr, vecs);itmc;itmc++)
      itmc.Data1() = MeanCovarianceC(1, itmc.Data2(), cov.Copy());
  
    SArray1dC<RealT>weights(mixes);
    for(SArray1dIterC<RealT>itw(weights);itw;itw++) *itw = 1.0/(RealT)mixes;

    //: finally lets construct our model
    GaussianMixtureC gm(arr, weights, isDiagonal);

    return gm; 
  }  
  
  GaussianMixtureC DesignGaussianMixtureBodyC::InitKMeans(const SampleVectorC & data) {
    DesignKMeansC ms(mixes);
    SArray1dC<MeanCovarianceC> params = ms.Cluster(data);
    SArray1dC<RealT> weights(mixes);
    for(SArray1dIterC<RealT>it(weights);it;it++) *it=1.0/(RealT)mixes;
    //: finally lets construct our model
    GaussianMixtureC gm(params, weights, isDiagonal);
    return gm; 
  }  


}

  
