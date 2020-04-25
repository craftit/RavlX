// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DESIGNMEANSHIFTCLUSTER_HEADER
#define RAVL_DESIGNMEANSHIFTCLUSTER_HEADER 1
//! author="Charles Galambos"
//! userlevel=Normal
//! lib=RavlPatternRec
//! docentry="Ravl.API.Pattern Recognition.Cluster;Ravl.API.Pattern Recognition.Classifier.DesignClassifier"
//! file="Ravl/PatternRec/Cluster/DesignMeanShiftCluster.hh"
//! example=exDesignMeanShiftCluster.cc

#include "Ravl/PatternRec/DesignCluster.hh"
#include "Ravl/PatternRec/DistanceSqrEuclidean.hh"

namespace RavlN {
  using namespace RavlN;
  
  //! userlevel=Develop
  //: Design a mean shift classifier / clustering algorithm
  
  class DesignMeanShiftClusterBodyC
    : public DesignClusterBodyC
  {
  public:
    DesignMeanShiftClusterBodyC(RealT nk,RealT ntermiter = 1.0,const DistanceC &distanceMetric = DistanceSqrEuclideanC());
    //: Constructor.
    // k is the size of the kernel
    // ntermiter is the minimum distance the cluster centre is moved before terminating iteration.
    
    DesignMeanShiftClusterBodyC(istream &strm);
    //: Load from stream.
    
    DesignMeanShiftClusterBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (ostream &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual FunctionC Apply(const SampleC<VectorC> &in);
    //: Create a classifier.
    
    virtual FunctionC Apply(const SampleC<VectorC> &in,const SampleC<RealT> &weight);
    //: Create function from the given data, and sample weights.
    
    virtual SArray1dC<MeanCovarianceC> Cluster(const SampleC<VectorC> &in);
    //: Compute cluster means.
    //!param: in - Array of samples to cluster.
    //!return: Array of point centres
    
    virtual SArray1dC<MeanCovarianceC> Cluster(const SampleC<VectorC> &in, const SampleC<RealT> & weights);
    //: Compute cluster means.
    //!param: in - Array of samples to cluster.
    //!param: weights - Weights of samples
    //!return: Array of point centres

    virtual SArray1dC<MeanCovarianceC> Cluster(const SampleC<VectorC> &in,SampleC<UIntT> &labels);
    //: Compute cluster means, and labels for all the samples
    //!param: in - Sample of vectors to cluster
    //!param: labels - Sample used to store the membership of each sample in the form of an index into the returned array
    //!return: Array of clusters.
    
    virtual SArray1dC<MeanCovarianceC> Cluster(const SampleC<VectorC> &in, const SampleC<RealT> & weights, SampleC<UIntT> &labels);
      //: Compute cluster means, and labels for all the samples
      //!param: in - Sample of vectors to cluster
      //!param: labels - Sample used to store the membership of each sample in the form of an index into the returned array
      //!return: Array of clusters.

    RealT K() const
    { return k; }
    //: Access size of kernel
    
  protected:
    virtual DListC<VectorC> FindMeans(const SampleC<VectorC> &in,SampleC<UIntT> &labels);
    //: Find means for 'in'.
    //!param: in - Set of vectors to process, must be all of the same length.
    //!param: labels - Data set of labels in which the member ship of each sample is stored
    //!return: Set of cluster centres
    
    virtual DListC<VectorC> FindMeans(const SampleC<VectorC> &in,const SampleC<RealT> &weights,SampleC<UIntT> &labels);
    //: Find weighted means for 'in'.
    //!param: in - Set of vectors to process, must be all of the same length.
    //!param: labels - Data set of labels in which the member ship of each sample is stored
    //!param: weights - Weights for each of the samples.
    //!return: Set of cluster centres
    
    DistanceC distance;
    RealT k;
    RealT termiter;
  };

  //! userlevel=Normal
  //: Design a mean shift classifier / clustering algorithm

  class DesignMeanShiftClusterC 
    : public  DesignClusterC
  {
  public:
    DesignMeanShiftClusterC()
    {}
    //: Default constructor.
    // Creates an invalid constructor.
    
    DesignMeanShiftClusterC(RealT k,RealT ntermiter = 1.0,const DistanceC &distanceMetric = DistanceSqrEuclideanC())
      : DesignClusterC(*new DesignMeanShiftClusterBodyC(k,ntermiter,distanceMetric))
    {}
    //: Constructor.
    //!param: k - size of the kernel
    //!param: ntermiter - minimum distance the cluster centre is moved before terminating iteration.
    
    DesignMeanShiftClusterC(std::istream &strm);
    //: Load from stream.
    
    DesignMeanShiftClusterC(BinIStreamC &strm);
    //: Load from binary stream. //: Compute cluster means.
    
  protected:
    DesignMeanShiftClusterC(DesignMeanShiftClusterBodyC &bod)
      : DesignClusterC(bod)
    {}
    //: Body constructor.

    DesignMeanShiftClusterC(DesignMeanShiftClusterBodyC *bod)
      : DesignClusterC(bod)
    {}
    //: Body ptr constructor.
    
    DesignMeanShiftClusterBodyC &Body()
    { return static_cast<DesignMeanShiftClusterBodyC &>(DesignerC::Body()); }
    //: Access body.

    const DesignMeanShiftClusterBodyC &Body() const
    { return static_cast<const DesignMeanShiftClusterBodyC &>(DesignerC::Body()); }
    //: Access body.
    
  public:
    RealT K() const
    { return Body().K(); }
    //: Access size of kernel
    
    using DesignClusterC::Cluster;

    SArray1dC<MeanCovarianceC> Cluster(const SampleC<VectorC> &in,SampleC<UIntT> &labels)
    { return Body().Cluster(in,labels); }
    //: Compute cluster means, and labels for all the samples
    //!param: in - Sample of vectors to cluster
    //!param: labels - Sample used to store the membership of each sample in the form of an index into the returned array
    //!return: Array of clusters.
    
    SArray1dC<MeanCovarianceC> Cluster(const SampleC<VectorC> &in, const SampleC<RealT> & weights, SampleC<UIntT> &labels)
      { return Body().Cluster(in, weights, labels); }
      //: Compute cluster means, and labels for all the samples
      //!param: in - Sample of vectors to cluster
      //!param: labels - Sample used to store the membership of each sample in the form of an index into the returned array
      //!return: Array of clusters.


  };

}


#endif
