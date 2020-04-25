// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_CLASSIFIERCASCADE_HEADER
#define RAVL_CLASSIFIERCASCADE_HEADER 1
///////////////////////////////////////////////////////////////////////
//! author="Kieron Messer"
//! docentry="Ravl.API.Pattern Recognition.Classifier"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Classify/ClassifierCascade.hh"

#include "Ravl/SArray1d.hh"
#include "Ravl/PatternRec/Classifier.hh"
#include "Ravl/PatternRec/FuncSubset.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: A classifier which is a cascade of classifiers
  
  class ClassifierCascadeBodyC : public ClassifierBodyC
  {
  public:
    ClassifierCascadeBodyC(const SArray1dC<ClassifierC> & classifiers, const SArray1dC<RealT> & thresholds, const SArray1dC<FuncSubsetC> & features);
    //: Create classifier from a normalisation function and some weights.

    ClassifierCascadeBodyC(std::istream &strm);
    //: Load from stream.
    
    ClassifierCascadeBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save(std::ostream &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual bool Save(BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual UIntT Classify(const VectorC &data) const;
    //: Classifier vector 'data' return the most likely label.
    
    virtual VectorC Apply(const VectorC &data) const;
    //: Estimate the confidence for each label.
    // The meaning of the confidence assigned to each label depends
    // on the function used. The higher the confidence the more likely
    // it is the label is correct. The sum of the vector has been normalised
    // to 1.

  protected:
    SArray1dC<ClassifierC> m_classifiers; //!< The classifier at each stage
    SArray1dC<RealT> m_thresholds; //!< The thresholds at each stage
    SArray1dC<FuncSubsetC> m_features; //!< The features to use at each stage
  };
  
  //! userlevel=Normal
  //: A classifier cascade

  class ClassifierCascadeC : public ClassifierC
  {
  public:
    ClassifierCascadeC()
    {
    }
    //: Default constructor.
    // Creates an invalid handle.
    
    ClassifierCascadeC(const SArray1dC<ClassifierC> & classifiers,
        const SArray1dC<RealT> & thresholds,
        const SArray1dC<FuncSubsetC> & features) :
        ClassifierC(*new ClassifierCascadeBodyC(classifiers, thresholds, features))
    {
    }
    //: Create cascade classifier

    ClassifierCascadeC(std::istream &strm);
    //: Load from stream.
    
    ClassifierCascadeC(BinIStreamC &strm);
    //: Load from binary stream.
    
  protected:
    ClassifierCascadeC(ClassifierCascadeBodyC &bod) :
        ClassifierC(bod)
    {
    }
    //: Body constructor.

    ClassifierCascadeC(ClassifierCascadeBodyC *bod) :
        ClassifierC(bod)
    {
    }
    //: Body constructor.
    
    ClassifierCascadeBodyC &Body()
    {
      return static_cast<ClassifierCascadeBodyC &>(ClassifierC::Body());
    }
    //: Access body.

    const ClassifierCascadeBodyC &Body() const
    {
      return static_cast<const ClassifierCascadeBodyC &>(ClassifierC::Body());
    }
    //: Access body.
  public:
  };
  
  inline std::istream &operator>>(std::istream &strm, ClassifierCascadeC &obj)
  {
    obj = ClassifierCascadeC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline std::ostream &operator<<(std::ostream &out, const ClassifierCascadeC &obj)
  {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm, ClassifierCascadeC &obj)
  {
    obj = ClassifierCascadeC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out, const ClassifierCascadeC &obj)
  {
    obj.Save(out);
    return out;
  }
//: Save to a stream.
// Uses virtual constructor.

}

#endif
