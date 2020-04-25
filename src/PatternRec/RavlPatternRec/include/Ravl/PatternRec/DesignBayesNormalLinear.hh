// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DESIGNBAYESNORMALLINEAR_HEADER
#define RAVL_DESIGNBAYESNORMALLINEAR_HEADER 1
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! author="Kieron Messer"
//! docentry="Ravl.API.Pattern Recognition.Classifier.DesignClassifier"
//! file="Ravl/PatternRec/Classify/DesignBayesNormalLinear.hh"

#include "Ravl/PatternRec/DesignClassifierSupervised.hh"


namespace RavlN {
  
  //! userlevel=Develop
  //: Design Bayes Normal Linear classifer
  
  class DesignBayesNormalLinearBodyC
    : public DesignClassifierSupervisedBodyC
  {
  public:
    DesignBayesNormalLinearBodyC();
    //: Default constructor.
    
    DesignBayesNormalLinearBodyC(std::istream &strm);
    //: Load from stream.
    
    DesignBayesNormalLinearBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (std::ostream &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor

    DesignBayesNormalLinearBodyC(bool equalPriors);
    //: Assume equal priors for the classes? otherwise compute from the training data
    
    DesignBayesNormalLinearBodyC(const SArray1dC<RealT> & priors);
    //: Specify the priors for each class
        
    DesignBayesNormalLinearBodyC(const XMLFactoryContextC &factory);
    //: Construct from XML factory

    virtual ClassifierC Apply(const SampleC<VectorC> &in,const SampleC<UIntT> &out);
    //: Create function from the a labelled dataset.
        
  protected:
    SArray1dC<RealT> priors;
    //: The means for each class

    bool equalPriors;
    

  };

  //! userlevel=Normal
  //: Design Bayes Normal Linear classifer  
  class DesignBayesNormalLinearC
    : public DesignClassifierSupervisedC
  {
  public:

    DesignBayesNormalLinearC(std::istream &strm);
    //: Load from stream.
    
    DesignBayesNormalLinearC(BinIStreamC &strm);
    //: Load from binary stream.

    DesignBayesNormalLinearC(bool equalPriors=true)
      : DesignClassifierSupervisedC(*new DesignBayesNormalLinearBodyC(equalPriors))
    {}
    //: Create designer.
    
    DesignBayesNormalLinearC(const SArray1dC<RealT> & priors)
      : DesignClassifierSupervisedC(*new DesignBayesNormalLinearBodyC(priors))
    {}
    //: Create designer.
    
    DesignBayesNormalLinearC(const XMLFactoryContextC &factory)
      :  DesignClassifierSupervisedC(*new DesignBayesNormalLinearBodyC(factory))
    {}
    //: Construct from XML factory

  protected:
    DesignBayesNormalLinearC(DesignBayesNormalLinearBodyC &bod)
      : DesignClassifierSupervisedC(bod)
    {}
    //: Body constructor.
    
    DesignBayesNormalLinearC(DesignBayesNormalLinearBodyC *bod)
      : DesignClassifierSupervisedC(bod)
    {}
    //: Body ptr constructor.
    
     DesignBayesNormalLinearBodyC &Body()
    { return static_cast<DesignBayesNormalLinearBodyC &>(DesignClassifierSupervisedC::Body()); }
    //: Access body.
    
    const DesignBayesNormalLinearBodyC &Body() const
    { return static_cast<const DesignBayesNormalLinearBodyC &>(DesignClassifierSupervisedC::Body()); }
    //: Access body.

  public:


  };
  
  inline std::istream &operator>>(std::istream &strm,DesignBayesNormalLinearC &obj) {
    obj = DesignBayesNormalLinearC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline std::ostream &operator<<(std::ostream &out,const DesignBayesNormalLinearC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,DesignBayesNormalLinearC &obj) {
    obj = DesignBayesNormalLinearC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const DesignBayesNormalLinearC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.

}


#endif
