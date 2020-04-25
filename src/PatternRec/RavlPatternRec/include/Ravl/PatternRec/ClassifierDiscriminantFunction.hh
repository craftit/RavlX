// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_CLASSIFYDISCRIMINANTFUNCTION_HEADER
#define RAVL_CLASSIFYDISCRIMINANTFUNCTION_HEADER 1
///////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! author="Charles Galambos"
//! docentry="Ravl.API.Pattern Recognition.Classifier"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Classify/ClassifierDiscriminantFunction.hh"

#include "Ravl/PatternRec/Classifier.hh"
#include "Ravl/PatternRec/Function.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Create a classifier based on a descriminant function.
  // This classifier returns the label with the highest value.
  // This class can be used with <a href="RavlN.DesignDiscriminantFunctionC.html"> DesignDiscriminantFunctionC </a>  
  
  class ClassifierDiscriminantFunctionBodyC 
    : public ClassifierBodyC
  {
  public:
    ClassifierDiscriminantFunctionBodyC(const FunctionC &nfunc);
    //: Create classifier from a discriminant function.
    
    ClassifierDiscriminantFunctionBodyC(std::istream &strm);
    //: Load from stream.
    
    ClassifierDiscriminantFunctionBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (std::ostream &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual UIntT Classify(const VectorC &data) const;
    //: Classifier vector 'data' return the most likely label.
    
    virtual VectorC Apply(const VectorC &data) const;
    //: Estimate the confidence for each label.
    // The meaning of the confidence assigned to each label depends
    // on the function used. The higher the confidence the more likely
    // it is the label is correct. The sum of the vector has been normalised
    // to 1.
    
    const FunctionC &Discriminant() const
    { return func; }
    //: Access discriminant function.
    
  protected:
    FunctionC func;
  };
  
  //! userlevel=Develop
  //: Create a classifier based on a descriminant function.
  //  This classifier returns the label with the highest value.
  // This class can be used with <a href="RavlN.DesignDiscriminantFunctionC.html"> DesignDiscriminantFunctionC </a>  

  class ClassifierDiscriminantFunctionC 
    : public ClassifierC
  {
  public:
    ClassifierDiscriminantFunctionC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    ClassifierDiscriminantFunctionC(const FunctionC &nfunc)
      : ClassifierC(*new ClassifierDiscriminantFunctionBodyC(nfunc))
    {}
    //: Create classifier from a discriminant function.
    
    ClassifierDiscriminantFunctionC(std::istream &strm);
    //: Load from stream.
    
    ClassifierDiscriminantFunctionC(BinIStreamC &strm);
    //: Load from binary stream.
    
  protected:
    ClassifierDiscriminantFunctionC(ClassifierDiscriminantFunctionBodyC &bod)
      : ClassifierC(bod)
    {}
    //: Body constructor.

    ClassifierDiscriminantFunctionC(ClassifierDiscriminantFunctionBodyC *bod)
      : ClassifierC(bod)
    {}
    //: Body constructor.
    
    ClassifierDiscriminantFunctionBodyC &Body()
    { return static_cast<ClassifierDiscriminantFunctionBodyC &>(ClassifierC::Body()); }
    //: Access body.

    const ClassifierDiscriminantFunctionBodyC &Body() const
    { return static_cast<const ClassifierDiscriminantFunctionBodyC &>(ClassifierC::Body()); }
    //: Access body.
    
  public:
    const FunctionC &Discriminant() const
    { return Body().Discriminant(); }
    //: Access discriminant function.
    
  };
  
  inline std::istream &operator>>(std::istream &strm,ClassifierDiscriminantFunctionC &obj) {
    obj = ClassifierDiscriminantFunctionC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline std::ostream &operator<<(std::ostream &out,const ClassifierDiscriminantFunctionC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,ClassifierDiscriminantFunctionC &obj) {
    obj = ClassifierDiscriminantFunctionC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const ClassifierDiscriminantFunctionC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
}

#endif
