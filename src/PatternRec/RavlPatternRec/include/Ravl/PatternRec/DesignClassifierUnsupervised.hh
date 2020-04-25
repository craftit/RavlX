// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DESIGNCLASSIFIERUNSUPERVISED_HEADER
#define RAVL_DESIGNCLASSIFIERUNSUPERVISED_HEADER 1
///////////////////////////////////////////////////////////
//! userlevel=Normal
//! author="Charles Galambos"
//! lib=RavlPatternRec
//! docentry="Ravl.API.Pattern Recognition.Cluster;Ravl.API.Pattern Recognition.Classifier.DesignClassifier"
//! file="Ravl/PatternRec/Cluster/DesignClassifierUnsupervised.hh"
//! example=exKMeansCluster.cc

#include "Ravl/RCHandleV.hh"
#include "Ravl/Vector.hh"
#include "Ravl/PatternRec/Sample.hh"
#include "Ravl/PatternRec/Classifier.hh"
#include "Ravl/PatternRec/DesignFunctionUnsupervised.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Abstract unsupervised classifier designer.
  
  class DesignClassifierUnsupervisedBodyC
    : public DesignFunctionUnsupervisedBodyC
  {
  public:
    DesignClassifierUnsupervisedBodyC()
    {}
    //: Default constructor.
    
    DesignClassifierUnsupervisedBodyC(istream &strm);
    //: Load from stream.
    
    DesignClassifierUnsupervisedBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (ostream &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual FunctionC Apply(const SampleC<VectorC> &in);
    //: Create a clasifier.
    
    virtual FunctionC Apply(const SampleC<VectorC> &in,const SampleC<RealT> &weight);
    //: Create a clasifier with weights for the samples.
    
  };
  
  //! userlevel=Normal
  //: Abstract unsupervised classifier designer.
  
  class DesignClassifierUnsupervisedC
    : public DesignFunctionUnsupervisedC
  {
  public:
    DesignClassifierUnsupervisedC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    DesignClassifierUnsupervisedC(istream &strm);
    //: Load from stream.
    
    DesignClassifierUnsupervisedC(BinIStreamC &strm);
    //: Load from binary stream.

  protected:
    DesignClassifierUnsupervisedC(DesignClassifierUnsupervisedBodyC &bod)
      : DesignFunctionUnsupervisedC(bod)
    {}
    //: Body constructor.
    
    DesignClassifierUnsupervisedC(DesignClassifierUnsupervisedBodyC *bod)
      : DesignFunctionUnsupervisedC(*bod)
    {}
    //: Body ptr constructor.
    
    DesignClassifierUnsupervisedBodyC &Body()
    { return static_cast<DesignClassifierUnsupervisedBodyC &>(DesignerC::Body()); }
    //: Access body.
    
    const DesignClassifierUnsupervisedBodyC &Body() const
    { return static_cast<const DesignClassifierUnsupervisedBodyC &>(DesignerC::Body()); }
    //: Access body.
    
  public:
    ClassifierC Apply(const SampleC<VectorC> &in)
    { return Body().Apply(in); }
    //: Create a clasifier.
    
    ClassifierC Apply(const SampleC<VectorC> &in,const SampleC<RealT> &weight) 
    { return Body().Apply(in,weight); }
    //: Create a clasifier with weights for the samples.
  };

  
  inline istream &operator>>(istream &strm,DesignClassifierUnsupervisedC &obj) {
    obj = DesignClassifierUnsupervisedC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline ostream &operator<<(ostream &out,const DesignClassifierUnsupervisedC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,DesignClassifierUnsupervisedC &obj) {
    obj = DesignClassifierUnsupervisedC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const DesignClassifierUnsupervisedC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
}


#endif
