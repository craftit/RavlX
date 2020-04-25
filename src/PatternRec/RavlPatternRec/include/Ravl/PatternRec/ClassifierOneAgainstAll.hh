// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_CLASSIFIERONEAGAINSTALL_HEADER
#define RAVL_CLASSIFIERONEAGAINSTALL_HEADER 1
//! rcsid="$Id: ClassifierOneAgainstAll.hh 5240 2005-12-06 17:16:50Z plugger $"
//! lib=RavlPatternRec
//! author="Kieron Messer"
//! docentry="Ravl.API.Pattern Recognition.Classifier"
//! file="Ravl/PatternRec/Classify/ClassifierOneAgainstAll.hh"

#include "Ravl/PatternRec/Classifier.hh"
#include "Ravl/Matrix.hh"
#include "Ravl/Vector.hh"

namespace RavlN {

  //! userlevel=Develop
  //: One against all classifier

  class ClassifierOneAgainstAllBodyC
    : public ClassifierBodyC
  {
  public:
    ClassifierOneAgainstAllBodyC();
    //: Default constructor.
    
    ClassifierOneAgainstAllBodyC(const RCHashC<UIntT, ClassifierC> & classifiers);
    //: Construct from a set of classifiers
    
    ClassifierOneAgainstAllBodyC(std::istream &strm);
    //: Load from stream.
    
    ClassifierOneAgainstAllBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (std::ostream &out) const;
    //: Writes object to stream.
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to binary stream.
    
    virtual UIntT Classify(const VectorC &data) const;
    //: Returns most likely label
    
    
  protected:
    RCHashC<UIntT, ClassifierC>m_classifiers;
  };
  
  //! userlevel=Normal
  //: One against all classifier
  
  class ClassifierOneAgainstAllC
    : public ClassifierC 
  {
  public:
    ClassifierOneAgainstAllC()
    {}
    //: Default constructor.
    
    ClassifierOneAgainstAllC(const RCHashC<UIntT, ClassifierC> & classifiers)
      : ClassifierC(*new ClassifierOneAgainstAllBodyC(classifiers))
    {}
    //: Construct from a mean and a projection matrix.
    
    ClassifierOneAgainstAllC(std::istream &is);
    //: Stream constructor.
    
    ClassifierOneAgainstAllC(BinIStreamC &is);
    //: Stream constructor.
    
    ClassifierOneAgainstAllC(const ClassifierC &func)
      : ClassifierC(dynamic_cast<const ClassifierOneAgainstAllBodyC *>(BodyPtr(func)))
    {}
    //: Attempt to create handle from base class.
    // If object is not a ClassifierOneAgainstAllC, an invalid handle will be created.
    
  protected:
    ClassifierOneAgainstAllC(ClassifierOneAgainstAllBodyC &bod)
      : ClassifierC(bod)
    {}
    //: Body constructor.
    
    ClassifierOneAgainstAllBodyC &Body()
    { return static_cast<ClassifierOneAgainstAllBodyC &>(ClassifierC::Body()); }
    //: Access body.

    const ClassifierOneAgainstAllBodyC &Body() const 
    { return static_cast<const ClassifierOneAgainstAllBodyC &>(ClassifierC::Body()); }
    //: Access body.
    
  public:
    
  };
  
  
  inline std::istream &operator>>(std::istream &strm,ClassifierOneAgainstAllC &obj) {
    obj = ClassifierOneAgainstAllC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline std::ostream &operator<<(std::ostream &out,const ClassifierOneAgainstAllC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,ClassifierOneAgainstAllC &obj) {
    obj = ClassifierOneAgainstAllC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const ClassifierOneAgainstAllC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.

}



#endif
