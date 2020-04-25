// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DESIGNWEAKLINEAR_HEADER
#define RAVL_DESIGNWEAKLINEAR_HEADER 1
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! docentry="Ravl.API.Pattern Recognition.Classifier.DesignClassifier"
//! file="Ravl/PatternRec/Classify/DesignWeakLinear.hh"

#include "Ravl/PatternRec/DesignClassifierSupervised.hh"
#include "Ravl/PatternRec/DesignFunctionSupervised.hh"
#include "Ravl/Vector2d.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Design a weak linear classifier.
  
  class DesignWeakLinearBodyC
    : public DesignClassifierSupervisedBodyC
  {
  public:
    DesignWeakLinearBodyC()
    {}
    //: Constructor.
    
    DesignWeakLinearBodyC(std::istream &strm);
    //: Load from stream.
    
    DesignWeakLinearBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (std::ostream &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor

    virtual ClassifierC Apply(const SampleC<VectorC> &in,const SampleC<UIntT> &out);
    //: Create a classifier.
    
    virtual ClassifierC Apply(const SampleC<VectorC> &in,const SampleC<UIntT> &out,const SampleC<RealT> &weight);
    //: Create a classifier with weights for the samples.
    
    virtual ClassifierC Apply(const SampleC<VectorC> &in,const SampleC<UIntT> &out,const SArray1dC<IndexC> &featureSet);
    //: Create a classifier
    
    virtual ClassifierC Apply(const SampleC<VectorC> &in,const SampleC<UIntT> &out,const SArray1dC<IndexC> &featureSet,const SampleC<RealT> &weight);
    //: Create a classifier
    
  private:
    void ThresholdAndParity(RealT &threshold1,RealT &parity1,RealT &threshold2,RealT &parity2);
  protected:
    Vector2dC m_mean;
    Vector2dC m_var;
    Vector2dC m_weight;
  };
  
  //! userlevel=Normal
  //: Design a weak linear classifier.
  // This class should be used with the <a href="RavlN.DesignWeakLinearC.html"> DesignWeakLinearC </a> class. 
  
  class DesignWeakLinearC
    : public DesignClassifierSupervisedC
  {
  public:
    DesignWeakLinearC()
      : DesignClassifierSupervisedC(*new DesignWeakLinearBodyC())
    {}
    //: Create a new designer.
    
    DesignWeakLinearC(std::istream &strm);
    //: Load from stream.
    
    DesignWeakLinearC(BinIStreamC &strm);
    //: Load from binary stream.

  protected:
    DesignWeakLinearC(DesignWeakLinearBodyC &bod)
      : DesignClassifierSupervisedC(bod)
    {}
    //: Body constructor.

    DesignWeakLinearC(DesignWeakLinearBodyC *bod)
      : DesignClassifierSupervisedC(bod)
    {}
    //: Body ptr constructor.
    
  };
  
}

#endif
