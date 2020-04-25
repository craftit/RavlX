// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here

#ifndef RAVL_CLASSIFIER2_HEADER
#define RAVL_CLASSIFIER2_HEADER 1

//////////////////////////////////////////////////////////////////////////////
//! file =      "Ravl/PatternRec/Svm/Classifier2.hh"
//! author =    "Alexey Kostin"
//! lib=RavlPatternRec
//! date =      "04/10/05"
//! rcsid =     "$Id: Classifier2.hh,v 1.2 2005/05/12 14:19:26 alex Exp $"
//! docentry =  "Ravl.API.Pattern_Recognition.Classifier.SVM"
//////////////////////////////////////////////////////////////////////////////

#include "Ravl/PatternRec/Classifier.hh"

namespace RavlN
{
using namespace RavlN;

//! userlevel = Develop
//: Two classes classifier
class Classifier2BodyC : public ClassifierBodyC
{
public:
  Classifier2BodyC()
   : ClassifierBodyC(2)
  {}
  //: Default constructor

  Classifier2BodyC(ClassifierBodyC& Cls)
  :ClassifierBodyC(Cls)
  {}
  //: Default constructor

  Classifier2BodyC(std::istream &Strm);
  //: Load from stream.

  Classifier2BodyC(BinIStreamC &Strm);
  //: Load from binary stream.

  virtual ~Classifier2BodyC();
  //: Destructor

  virtual bool Save(std::ostream &Out) const;
  //: Writes object to stream
  // returns always true (compatibility issue), on error an exception is thrown

  virtual bool Save(BinOStreamC &Out) const;
  //: Writes object to stream
  // returns always true (compatibility issue), on error an exception is thrown

  virtual UIntT Classify(const VectorC &Data) const;
  //: Classify vector 'data' and return class ID (0 or 1)

  //virtual UIntT Classify(const RealT* Data) const;
  //: Classify vector 'data' and return class ID (0 or 1)
  // classifier supposed to know length of 'Data' array

  virtual RealT Classify2(const VectorC &Data) const;
  //: Classify vector 'data' and return value of descriminant function

  //virtual RealT Classify2(const RealT* Data) const;
  //: Classify vector 'data' and return value of descriminant function

  virtual IntT GetDataSize() const;
  //: Get vector length of classifier
};
//---------------------------------------------------------------------------
//! userlevel = Normal
//: Two classes classifier
class Classifier2C : public ClassifierC
{
public:
  Classifier2C()
    {}
  //: Default constructor creates wrong object

  Classifier2C(std::istream &Strm);
  //: Load from stream.

  Classifier2C(BinIStreamC &Strm);
  //: Load from binary stream.

  Classifier2C(const ClassifierC &Cls)
    : ClassifierC(Cls)
  {}
  //: Constructor
  
  UIntT Classify(const VectorC &Data) const
  { return Body().Classify(Data); }
  //: Classify vector 'data' and return clas ID (0 or 1)

  //UIntT Classify(const RealT* Data) const
  //{ return Body().Classify(Data); }
  //: Classify vector 'data' and return class (0 or 1)
  
  RealT Classify2(const VectorC &Data) const
  { return Body().Classify2(Data); }
  //: Classify vector 'data' and return value of descriminant function

  //RealT Classify2(const RealT* Data) const
  //{ return Body().Classify2(Data); }
  //: Classify vector 'data' and return value of descriminant function
  // classifier supposed to know length of 'Data' array

  IntT GetDataSize() const
  { return Body().GetDataSize(); }
  //: Get vector length of classifier
  
protected:
  Classifier2C(Classifier2BodyC &Bod) 
    : ClassifierC(Bod)
  {}
  //: Body constructor.

  Classifier2C(Classifier2BodyC *Bod) 
    : ClassifierC(Bod)
  {}
  //: Body constructor.

  Classifier2BodyC &Body()
  { return static_cast<Classifier2BodyC &>(ClassifierC::Body()); }
  //: Access body.

  const Classifier2BodyC &Body() const
  { return static_cast<const Classifier2BodyC &>(ClassifierC::Body()); }
  //: Access body.
  
};

//---------------------------------------------------------------------------
//: Stream load operators defined in RAVL_INITVIRTUALCONSTRUCTOR_FULL macro
// Implementation of 'load from stream' constructors defined there as well
//
//inline std::ostream &operator<<(std::ostream &out, Classifier2C &obj)
//inline BinOStreamC &operator<<(BinOStreamC &out, Classifier2C &obj)

// Stream read operators are defined in RCHandleVC
//and call virtual Save function of body class
//inline std::ostream &operator<<(std::ostream &out, const Classifier2C &obj)
//inline BinOStreamC &operator<<(BinOStreamC &out, const Classifier2C &obj)

} //end of namespace RavlN
#endif
