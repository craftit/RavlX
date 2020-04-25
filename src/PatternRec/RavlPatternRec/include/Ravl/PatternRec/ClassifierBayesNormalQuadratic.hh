// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_CLASSIFIERBAYESNORMALQUADRATIC_HEADER
#define RAVL_CLASSIFIERBAYESNORMALQUADRATIC_HEADER 1
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! author="Kieron Messer"
//! docentry="Ravl.API.Pattern Recognition.Classifier"
//! file="Ravl/PatternRec/Classify/ClassifierBayesNormalQuadratic.hh"

#include "Ravl/PatternRec/Classifier.hh"
#include "Ravl/Matrix.hh"
#include "Ravl/Vector.hh"
#include "Ravl/MeanCovariance.hh"

namespace RavlN {

  //! userlevel=Develop
  //: A Bayes Normal Quadratic classifier
  // This class can be used with the <a href="RavlN.DesignBayesNormalQuadraticC.html"> DesignBayesNormalQuadraticC </a> class. 

  class ClassifierBayesNormalQuadraticBodyC
    : public ClassifierBodyC
  {
  public:
    ClassifierBayesNormalQuadraticBodyC() {}
    //: Default constructor.
    
    ClassifierBayesNormalQuadraticBodyC(const SArray1dC<MeanCovarianceC> & classStats, const SArray1dC<RealT> & priors);
    //: Construct from the mean for each, covariance and prior for each class
    
    ClassifierBayesNormalQuadraticBodyC(std::istream &strm);
    //: Load from stream.
    
    ClassifierBayesNormalQuadraticBodyC(BinIStreamC &strm);
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

    SArray1dC<MatrixC> invCov;
    //: the inverse covariances
    
    SArray1dC<RealT>konst;
    //: the constants
  };
  
  //! userlevel=Normal
  //: A Bayes Normal Quadratic classifier
  // This class can be used with the <a href="RavlN.DesignBayesNormalQuadraticC.html"> DesignBayesNormalQuadraticC </a> class. 

  class ClassifierBayesNormalQuadraticC
    : public ClassifierC 
  {
  public:
    ClassifierBayesNormalQuadraticC()
    {}
    //: Default constructor.
    
    ClassifierBayesNormalQuadraticC(const SArray1dC<MeanCovarianceC> & classStats, const SArray1dC<RealT> & priors)
      : ClassifierC(*new ClassifierBayesNormalQuadraticBodyC(classStats, priors))
    {}
    //: Construct from a mean and a projection matrix.
    
    ClassifierBayesNormalQuadraticC(std::istream &is);
    //: Stream constructor.
    
    ClassifierBayesNormalQuadraticC(BinIStreamC &is);
    //: Stream constructor.
    
    ClassifierBayesNormalQuadraticC(const ClassifierC &func)
      : ClassifierC(dynamic_cast<const ClassifierBayesNormalQuadraticBodyC *>(BodyPtr(func)))
    {}
    //: Attempt to create handle from base class.
    // If object is not a ClassifierBayesNormalQuadraticC, an invalid handle will be created.
    
  protected:
    ClassifierBayesNormalQuadraticC(ClassifierBayesNormalQuadraticBodyC &bod)
      : ClassifierC(bod)
    {}
    //: Body constructor.
    
    ClassifierBayesNormalQuadraticBodyC &Body()
    { return static_cast<ClassifierBayesNormalQuadraticBodyC &>(ClassifierC::Body()); }
    //: Access body.

    const ClassifierBayesNormalQuadraticBodyC &Body() const 
    { return static_cast<const ClassifierBayesNormalQuadraticBodyC &>(ClassifierC::Body()); }
    //: Access body.
    
  public:
    
  };
  
  
  inline std::istream &operator>>(std::istream &strm,ClassifierBayesNormalQuadraticC &obj) {
    obj = ClassifierBayesNormalQuadraticC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline std::ostream &operator<<(std::ostream &out,const ClassifierBayesNormalQuadraticC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,ClassifierBayesNormalQuadraticC &obj) {
    obj = ClassifierBayesNormalQuadraticC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const ClassifierBayesNormalQuadraticC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.

}



#endif
