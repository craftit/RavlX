// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DESIGNKMEANS_HEADER
#define RAVL_DESIGNKMEANS_HEADER 1
//! author="Charles Galambos"
//! lib=RavlPatternRec
//! docentry="Ravl.API.Pattern Recognition.Cluster;Ravl.API.Pattern Recognition.Classifier.DesignClassifier"
//! example=exKMeansCluster.cc
//! file="Ravl/PatternRec/Cluster/DesignKMeans.hh"

#include "Ravl/PatternRec/DesignCluster.hh"
#include "Ravl/PatternRec/DistanceSqrEuclidean.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Design a k means classifier.
  
  class DesignKMeansBodyC
    : public DesignClusterBodyC
  {
  public:
    DesignKMeansBodyC(UIntT nk,const DistanceC &distanceMetric = DistanceSqrEuclideanC());
    //: Constructor.
    
    DesignKMeansBodyC(istream &strm);
    //: Load from stream.
    
    DesignKMeansBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (ostream &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor

    virtual FunctionC Apply(const SampleC<VectorC> &in);
    //: Create a classifier.
    
    virtual FunctionC Apply(const SampleC<VectorC> &in,const SampleC<RealT> &weight);
    //: Create function from the given data, and sample weights.
    
    SArray1dC<MeanCovarianceC> Cluster(const SampleC<VectorC> &in);
    //: Compute cluster means.
    
    SArray1dC<MeanCovarianceC> Cluster(const SampleC<VectorC> &in, const SampleC<RealT> &weight);
    //: Compute cluster means.

    UIntT K() const
    { return k; }
    //: Access number of clusters.
    
  protected:
    SArray1dC<VectorC> FindMeans(const SampleC<VectorC> &in);
    //: Find means for 'in'.
    
    SArray1dC<VectorC> FindMeans(const SampleC<VectorC> &in,const SampleC<RealT> &weights);
    //: Find weighted means for 'in'.
    
    DistanceC distance;
    UIntT k;
  };

  //! userlevel=Normal
  //: Design a k means classifier.
  
  class DesignKMeansC 
    : public  DesignClusterC
  {
  public:
    DesignKMeansC()
    {}
    //: Default constructor.
    // Creates an invalid constructor.
    
    DesignKMeansC(UIntT k,const DistanceC &distanceMetric = DistanceSqrEuclideanC())
      : DesignClusterC(*new DesignKMeansBodyC(k,distanceMetric))
    {}
    //: Default constructor.
    // Creates an invalid constructor.
    
    DesignKMeansC(istream &strm);
    //: Load from stream.
    
    DesignKMeansC(BinIStreamC &strm);
    //: Load from binary stream.
    
  protected:
    DesignKMeansC(DesignKMeansBodyC &bod)
      : DesignClusterC(bod)
    {}
    //: Body constructor.

    DesignKMeansC(DesignKMeansBodyC *bod)
      : DesignClusterC(bod)
    {}
    //: Body ptr constructor.
    
    DesignKMeansBodyC &Body()
    { return static_cast<DesignKMeansBodyC &>(DesignFunctionUnsupervisedC::Body()); }
    //: Access body.
    
    const DesignKMeansBodyC &Body() const
    { return static_cast<const DesignKMeansBodyC &>(DesignFunctionUnsupervisedC::Body()); }
    //: Access body.
    
  public:
    UIntT K() const
    { return Body().K(); }
    //: Access number of clusters.
    
  };
  
  inline istream &operator>>(istream &strm,DesignKMeansC &obj) {
    obj = DesignKMeansC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline ostream &operator<<(ostream &out,const DesignKMeansC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,DesignKMeansC &obj) {
    obj = DesignKMeansC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const DesignKMeansC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.

}


#endif
