// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DESIGNBAYESNORMALQUADRATIC_HEADER
#define RAVL_DESIGNBAYESNORMALQUADRATIC_HEADER 1
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! author="Kieron Messer"
//! docentry="Ravl.API.Pattern Recognition.Classifier.DesignClassifier"
//! file="Ravl/PatternRec/Classify/DesignBayesNormalQuadratic.hh"

#include "Ravl/PatternRec/DesignClassifierSupervised.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Design a Bayes Normal Quadratic Classifier

  class DesignBayesNormalQuadraticBodyC
    : public DesignClassifierSupervisedBodyC
  {
  public:
    DesignBayesNormalQuadraticBodyC();
    //: Default constructor.

    DesignBayesNormalQuadraticBodyC(std::istream &strm);
    //: Load from stream.
    
    DesignBayesNormalQuadraticBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (std::ostream &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor

    DesignBayesNormalQuadraticBodyC(bool equalPriors);
    //: Assume equal priors for the classes? otherwise compute from the training data
    
    DesignBayesNormalQuadraticBodyC(const SArray1dC<RealT> & priors);
    //: Specify the priors for each class
        
    DesignBayesNormalQuadraticBodyC(const XMLFactoryContextC &factory);
    //: Construct from XML factory

    virtual ClassifierC Apply(const SampleC<VectorC> &in,const SampleC<UIntT> &out);
    //: Create function from the a labelled dataset.
        
  protected:
    SArray1dC<RealT> priors;
    //: The means for each class

    bool equalPriors;
    //: Do we want to use equal priors

  };

  //! userlevel=Normal
  //: Design a Bayes Normal Quadratic Classifier  
  class DesignBayesNormalQuadraticC
    : public DesignClassifierSupervisedC
  {
  public:

    DesignBayesNormalQuadraticC(std::istream &strm);
    //: Load from stream.
    
    DesignBayesNormalQuadraticC(BinIStreamC &strm);
    //: Load from binary stream.

    DesignBayesNormalQuadraticC(bool equalPriors=true)
      : DesignClassifierSupervisedC(*new DesignBayesNormalQuadraticBodyC(equalPriors))
    {}
    //: Assume priors are equal.  If not it will estimate them from the training dataset
    
    DesignBayesNormalQuadraticC(const SArray1dC<RealT> & priors)
      : DesignClassifierSupervisedC(*new DesignBayesNormalQuadraticBodyC(priors))
    {}
    //: Create designer with user set priors for each class
    
    DesignBayesNormalQuadraticC(const XMLFactoryContextC &factory)
    :  DesignClassifierSupervisedC(*new DesignBayesNormalQuadraticBodyC(factory))
    {}
    //: Construct from XML factory

  protected:
    DesignBayesNormalQuadraticC(DesignBayesNormalQuadraticBodyC &bod)
      : DesignClassifierSupervisedC(bod)
    {}
    //: Body constructor.
    
    DesignBayesNormalQuadraticC(DesignBayesNormalQuadraticBodyC *bod)
      : DesignClassifierSupervisedC(bod)
    {}
    //: Body ptr constructor.
    
     DesignBayesNormalQuadraticBodyC &Body()
    { return static_cast<DesignBayesNormalQuadraticBodyC &>(DesignClassifierSupervisedC::Body()); }
    //: Access body.
    
    const DesignBayesNormalQuadraticBodyC &Body() const
    { return static_cast<const DesignBayesNormalQuadraticBodyC &>(DesignClassifierSupervisedC::Body()); }
    //: Access body.

  public:


  };
  
  inline std::istream &operator>>(std::istream &strm,DesignBayesNormalQuadraticC &obj) {
    obj = DesignBayesNormalQuadraticC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline std::ostream &operator<<(std::ostream &out,const DesignBayesNormalQuadraticC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,DesignBayesNormalQuadraticC &obj) {
    obj = DesignBayesNormalQuadraticC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const DesignBayesNormalQuadraticC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.

}


#endif
