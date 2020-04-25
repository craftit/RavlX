// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DESIGNCLUSTER_HEADER
#define RAVL_DESIGNCLUSTER_HEADER 1
//! author="Charles Galambos"
//! lib=RavlPatternRec
//! docentry="Ravl.API.Pattern Recognition.Cluster;Ravl.API.Pattern Recognition.Classifier.DesignClassifier"
//! file="Ravl/PatternRec/Cluster/DesignCluster.hh"

#include "Ravl/PatternRec/DesignClassifierUnsupervised.hh"
#include "Ravl/MeanCovariance.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Design a cluster classifier.
  
  class DesignClusterBodyC
    : public DesignClassifierUnsupervisedBodyC
  {
  public:
    DesignClusterBodyC();
    //: Constructor.
    
    DesignClusterBodyC(istream &strm);
    //: Load from stream.
    
    DesignClusterBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (ostream &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual SArray1dC<MeanCovarianceC> Cluster(const SampleC<VectorC> &in);
    //: Create a cluster on the input data

    virtual SArray1dC<MeanCovarianceC> Cluster(const SampleC<VectorC> &in, const SampleC<RealT> & weights);
    //: Create a cluster based on weighted input data.
    
  protected:
  };

  //! userlevel=Normal
  //: Design a cluster classifier.
  
  class DesignClusterC 
    : public DesignClassifierUnsupervisedC
  {
  public:
    DesignClusterC()
    {}
    //: Default constructor.
    // Creates an invalid constructor.
    
    DesignClusterC(istream &strm);
    //: Load from stream.
    
    DesignClusterC(BinIStreamC &strm);
    //: Load from binary stream.
    
  protected:
    DesignClusterC(DesignClusterBodyC &bod)
      : DesignClassifierUnsupervisedC(bod)
    {}
    //: Body constructor.

    DesignClusterC(DesignClusterBodyC *bod)
      : DesignClassifierUnsupervisedC(bod)
    {}
    //: Body ptr constructor.
    
    DesignClusterBodyC &Body()
    { return static_cast<DesignClusterBodyC &>(DesignClassifierUnsupervisedC::Body()); }
    //: Access body.

    const DesignClusterBodyC &Body() const
    { return static_cast<const DesignClusterBodyC &>(DesignClassifierUnsupervisedC::Body()); }
    //: Access body.
    
  public:
    SArray1dC<MeanCovarianceC> Cluster(const SampleC<VectorC> &in)
    { return Body().Cluster(in); }
    //: Compute cluster means.
    
    SArray1dC<MeanCovarianceC> Cluster(const SampleC<VectorC> &in, const SampleC<RealT>&weights)
      { return Body().Cluster(in, weights); }
    //: Compute cluster means on weighted input data

  };
  
  inline istream &operator>>(istream &strm,DesignClusterC &obj) {
    obj = DesignClusterC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline ostream &operator<<(ostream &out,const DesignClusterC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,DesignClusterC &obj) {
    obj = DesignClusterC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const DesignClusterC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
}

#endif
