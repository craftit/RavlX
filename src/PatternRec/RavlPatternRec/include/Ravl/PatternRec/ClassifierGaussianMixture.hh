// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_CLASSIFIERGUASSIANMIXTURE_HEADER
#define RAVL_CLASSIFIERGUASSIANMIXTURE_HEADER 1
///////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! author="Charles Galambos"
//! docentry="Ravl.API.Pattern Recognition.Classifier"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Classify/ClassifierGaussianMixture.hh"

#include "Ravl/PatternRec/Classifier.hh"
#include "Ravl/PatternRec/GaussianMixture.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: A classifier based on a mixture of gaussian distributions.
  //  This classifier returns the label with the highest value.
  // This class can be used with the <a href="RavlN.DesignClassifierGaussianMixtureC.html"> DesignClassifierGaussianMixtureC </a> class. 

  class ClassifierGaussianMixtureBodyC 
    : public ClassifierBodyC
  {
  public:
    ClassifierGaussianMixtureBodyC(const SArray1dC<GaussianMixtureC> &model);
    //: Create classifier from a discriminant function.
    
    ClassifierGaussianMixtureBodyC(std::istream &strm);
    //: Load from stream.
    
    ClassifierGaussianMixtureBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (std::ostream &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual UIntT Classify(const VectorC &data) const;
    //: Classifier vector 'data' return the most likely label.
    
    virtual VectorC Confidence(const VectorC &data) const;
    //: Estimate the confidence for each label.
    // The meaning of the confidence assigned to each label depends
    // on the function used. The higher the confidence the more likely
    // it is the label is correct. The sum of the vector has been normalised
    // to 1.
    
    const SArray1dC<GaussianMixtureC> &MixtureModel() const
    { return models; }
    //: Access mixture model directly.
    
  protected:
    SArray1dC<GaussianMixtureC > models;
  };
  
  //! userlevel=Normal
  //: A classifier based on a mixture of gaussian distributions.
  //  This classifier returns the label with the highest value.
  // This class can be used with the <a href="RavlN.DesignClassifierGaussianMixtureC.html"> DesignClassifierGaussianMixtureC </a> class. 

  class ClassifierGaussianMixtureC 
    : public ClassifierC
  {
  public:
    ClassifierGaussianMixtureC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    ClassifierGaussianMixtureC(const SArray1dC<GaussianMixtureC> &model)
      : ClassifierC(*new ClassifierGaussianMixtureBodyC(model))
    {}
    //: Create classifier from a discriminant function.
    
    ClassifierGaussianMixtureC(std::istream &strm);
    //: Load from stream.
    
    ClassifierGaussianMixtureC(BinIStreamC &strm);
    //: Load from binary stream.
    
  protected:
    ClassifierGaussianMixtureC(ClassifierGaussianMixtureBodyC &bod)
      : ClassifierC(bod)
    {}
    //: Body constructor.

    ClassifierGaussianMixtureC(ClassifierGaussianMixtureBodyC *bod)
      : ClassifierC(bod)
    {}
    //: Body constructor.
    
    ClassifierGaussianMixtureBodyC &Body()
    { return static_cast<ClassifierGaussianMixtureBodyC &>(ClassifierC::Body()); }
    //: Access body.

    const ClassifierGaussianMixtureBodyC &Body() const
    { return static_cast<const ClassifierGaussianMixtureBodyC &>(ClassifierC::Body()); }
    //: Access body.
    
  public:
    const SArray1dC<GaussianMixtureC > &MixtureModel() const
    { return Body().MixtureModel(); }
    //: Access mixture model directly.
    
  };
  
  inline std::istream &operator>>(std::istream &strm,ClassifierGaussianMixtureC &obj) {
    obj = ClassifierGaussianMixtureC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline std::ostream &operator<<(std::ostream &out,const ClassifierGaussianMixtureC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,ClassifierGaussianMixtureC &obj) {
    obj = ClassifierGaussianMixtureC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const ClassifierGaussianMixtureC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
}


#endif
