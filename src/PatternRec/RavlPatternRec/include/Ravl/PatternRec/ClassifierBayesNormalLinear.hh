// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_CLASSIFIERBAYESNORMALLINEAR_HEADER
#define RAVL_CLASSIFIERBAYESNORMALLINEAR_HEADER 1
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! author="Kieron Messer"
//! docentry="Ravl.API.Pattern Recognition.Classifier"
//! file="Ravl/PatternRec/Classify/ClassifierBayesNormalLinear.hh"

#include "Ravl/PatternRec/Classifier.hh"
#include "Ravl/Matrix.hh"
#include "Ravl/Vector.hh"

namespace RavlN {

  //! userlevel=Develop
  //: Bayes Normal Linear classifier
  // This class can be used with the <a href="RavlN.DesignBayesNormalLinearC.html"> DesignBayesNormalLinearC </a> class. 

  class ClassifierBayesNormalLinearBodyC
    : public ClassifierBodyC
  {
  public:
    ClassifierBayesNormalLinearBodyC();
    //: Default constructor.
    
    ClassifierBayesNormalLinearBodyC(const SArray1dC<VectorC> & means, const MatrixC & covariance, const SArray1dC<RealT> & priors);
    //: Construct from the mean for each, covariance and prior for each class
    
    ClassifierBayesNormalLinearBodyC(std::istream &strm);
    //: Load from stream.
    
    ClassifierBayesNormalLinearBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (std::ostream &out) const;
    //: Writes object to stream.
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to binary stream.
    
    virtual UIntT Classify(const VectorC &data) const;
    //: Returns most likely label
    
    virtual VectorC Apply(const VectorC & data) const;
    //: Get probability vector
    
  protected:
    SArray1dC<VectorC>mean;
    //: means of class

    MatrixC invCov;
    //: the inverse covariances
    
    SArray1dC<RealT>konst;
    //: the constant bit
  };
  
  //! userlevel=Normal
  //: Bayes Normal Linear classifier
  // This class can be used with the <a href="RavlN.DesignBayesNormalLinearC.html"> DesignBayesNormalLinearC </a> class. 

  class ClassifierBayesNormalLinearC
    : public ClassifierC 
  {
  public:
    ClassifierBayesNormalLinearC()
    {}
    //: Default constructor.
    
    ClassifierBayesNormalLinearC(const SArray1dC<VectorC> & mean, const MatrixC & covariance, const SArray1dC<RealT> & priors)
      : ClassifierC(*new ClassifierBayesNormalLinearBodyC(mean, covariance, priors))
    {}
    //: Construct from a mean and a projection matrix.
    
    ClassifierBayesNormalLinearC(std::istream &is);
    //: Stream constructor.
    
    ClassifierBayesNormalLinearC(BinIStreamC &is);
    //: Stream constructor.
    
    ClassifierBayesNormalLinearC(const ClassifierC &func)
      : ClassifierC(dynamic_cast<const ClassifierBayesNormalLinearBodyC *>(BodyPtr(func)))
    {}
    //: Attempt to create handle from base class.
    // If object is not a ClassifierBayesNormalLinearC, an invalid handle will be created.
    
  protected:
    ClassifierBayesNormalLinearC(ClassifierBayesNormalLinearBodyC &bod)
      : ClassifierC(bod)
    {}
    //: Body constructor.
    
    ClassifierBayesNormalLinearBodyC &Body()
    { return static_cast<ClassifierBayesNormalLinearBodyC &>(ClassifierC::Body()); }
    //: Access body.

    const ClassifierBayesNormalLinearBodyC &Body() const 
    { return static_cast<const ClassifierBayesNormalLinearBodyC &>(ClassifierC::Body()); }
    //: Access body.
    
  public:
    
  };
  
  
  inline std::istream &operator>>(std::istream &strm,ClassifierBayesNormalLinearC &obj) {
    obj = ClassifierBayesNormalLinearC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline std::ostream &operator<<(std::ostream &out,const ClassifierBayesNormalLinearC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,ClassifierBayesNormalLinearC &obj) {
    obj = ClassifierBayesNormalLinearC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const ClassifierBayesNormalLinearC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.

}



#endif
